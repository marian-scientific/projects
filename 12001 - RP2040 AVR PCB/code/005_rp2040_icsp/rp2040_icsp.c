#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h" // PIO helper functions
#include <stdio.h>

// --- Configuration ---
#define WS2812_PIN 16 
#define IS_RGBW false // This LED is typically RGB, not RGBW
#define NUM_PIXELS 1 // Only one LED on the board

// --- Global PIO State Machine ---
static inline void set_neopixel_color(uint32_t color) {
    // Put the 32-bit color value (GGRRBBTT) into the PIO TX FIFO
    pio_sm_put_blocking(pio0, 0, color); 
}

// Function to convert R, G, B bytes into the 32-bit format the PIO expects
uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    // The PIO expects the color data in GGRRBB format, with the 4th byte being trash (TT).
    return ((uint32_t)g << 24) | ((uint32_t)r << 16) | ((uint32_t)b << 8) | 0;
}

void ws2812_init(PIO pio, uint sm, uint pin, float freq) {
    uint offset = pio_add_program(pio, &ws2812_program);
    
    pio_gpio_init(pio, pin);
    pio_sm_set_consecutive_pindirs(pio, sm, pin, 1, true);
    
    pio_sm_config c = ws2812_program_get_default_config(offset);
    sm_config_set_sideset_pins(&c, pin);
    sm_config_set_out_shift(&c, false, true, 24);
    sm_config_set_fifo_join(&c, PIO_FIFO_JOIN_TX);
    
    int cycles_per_bit = ws2812_T1 + ws2812_T2 + ws2812_T3;
    float div = clock_get_hz(clk_sys) / (freq * cycles_per_bit);
    sm_config_set_clkdiv(&c, div);
    
    pio_sm_init(pio, sm, offset, &c);
    pio_sm_set_enabled(pio, sm, true);
}

// Define SPI pins
#define SPI_PORT spi0
#define SCK_PIN 6
#define MOSI_PIN 7
#define MISO_PIN 4
#define RESET_PIN 3
#define SPI_FREQ 250000

void setup_spi() {
    
	spi_init(SPI_PORT, SPI_FREQ);
    
	gpio_set_function(SCK_PIN, GPIO_FUNC_SPI);
	gpio_set_function(MOSI_PIN, GPIO_FUNC_SPI);
	gpio_set_function(MISO_PIN, GPIO_FUNC_SPI);
  
// CS not needed because reset acts as CS
//	gpio_set_function(CS_PIN, GPIO_FUNC_SPI);

	gpio_init(RESET_PIN);//, GPIO_OUT);  
	gpio_put(RESET_PIN, 1); // normal operation

	spi_set_format(
        SPI_PORT,
        8,          // 8 bits per transfer
        SPI_CPOL_0, // CPOL=0
        SPI_CPHA_0, // CPHA=0
        SPI_MSB_FIRST // Data is sent MSB first
    	);
}

// Blocking function to send an ISP command and get the response
void send_isp_command(uint8_t *tx_buf, uint8_t *rx_buf, size_t len) {
    
    // 1. Manually assert /RESET (if CS is acting as the /RESET pin)
    // gpio_put(SPI_CS_PIN, 0); 
    
    // --- BLOCKING SPI TRANSFER ---
    // Sends the data in tx_buf and reads the response into rx_buf simultaneously.
    spi_write_read_blocking(SPI_PORT, tx_buf, rx_buf, len);

    // 2. De-assert /RESET (if CS is acting as the /RESET pin)
    // gpio_put(SPI_CS_PIN, 1);
}

// --- Configuration ---
#define PACKET_SIZE 6

// --- Packet Structure Definition ---
typedef struct {
    uint8_t op_code;    // 1 byte: The command (e.g., 0x01, 0xFE, 0xFF)
    uint32_t payload;   // 4 bytes: Parameter, Address, or Size
    uint8_t checksum;   // 1 byte: Integrity check
} command_packet_t; // Total 6 bytes

void programming_sequence_example() {
    uint8_t command[4];
    uint8_t response[4];

    // --- 1. Enable Programming Mode (4-byte sequence) ---
    // AVR Command: 0xAC 0x53 0x00 0x00
    command[0] = 0xAC; // Programming Enable
    command[1] = 0x53; // Check value
    command[2] = 0x00;
    command[3] = 0x00;

    send_isp_command(command, response, 4);
    
    // AVR usually responds with 0x53 0x00 in bytes 2 and 3 if successful.
    if (response[2] != 0x53) {
        // Handle error: Programming mode failed
        printf("Failed to enter programming mode!\n");
        return;
    }
    
    // --- 2. Example: Read Signature Byte (3 bytes) ---
    // AVR Command: 0x30 0x00 0x00 0x00 (reads byte 0)
    command[0] = 0x30; // Read Signature
    command[1] = 0x00;
    command[2] = 0x00;
    command[3] = 0x00;

    send_isp_command(command, response, 4);
    
    // The signature byte will be in response[3]
    printf("Signature Byte 1: 0x%02X\n", response[3]);

    // --- 3. Example: Write Flash Page ---
    // This is more complex, involving: 
    // - Sending the memory address (MSB then LSB)
    // - Sending the low byte of the data
    // - Sending the high byte of the data
    // - Sending the write command and wait for the page write cycle to complete.
    // ...
}

int main() {
    stdio_init_all();
	// 1. Initialize the PIO State Machine
	PIO pio = pio0; // Use PIO block 0
	int sm = 0;     // Use State Machine 0
	uint offset = pio_add_program(pio, &ws2812_program);
    	ws2812_init(pio, sm, WS2812_PIN, 800000);

	setup_spi();

    // Wait for USB serial connection
    sleep_ms(1000);
    

    unsigned char greenness=0;

	set_neopixel_color(urgb_u32(0x00, greenness, 0x00));
    //const uint LED_PIN = 11;
    //gpio_init(LED_PIN);
    //gpio_set_dir(LED_PIN, GPIO_OUT);
    //gpio_put(LED_PIN, LED_VALUE);

//    printf("Press 'j' to toggle pin.\n");
    
command_packet_t packet;
    size_t bytes_read = 0;
    
    printf("Ready. Waiting for a %d-byte command packet via USB...\n", PACKET_SIZE);

    while (true) {
        
        // --- 1. BLOCKING USB READ (using fread) ---
        // Reads exactly 6 items (bytes) of size 1 byte into the 'packet' struct.
        // This line halts program execution until the PC sends all 6 bytes.
        bytes_read = fread(&packet, 1, PACKET_SIZE, stdin);
        
        // Check if the expected number of bytes were read
        if (bytes_read != PACKET_SIZE) {
             // This might happen if the connection is dropped mid-transfer.
             printf("\nError: Only received %zu out of %d bytes. Restarting wait.\n", bytes_read, PACKET_SIZE);
             continue; 
        }

        // --- 2. ACT ACCORDINGLY (State Logic) ---
        printf("Received %zu bytes. Processing command: 0x%02X\n", bytes_read, packet.op_code);

        switch (packet.op_code) {
            case 0x01:
                printf("OP_CODE 0x01: Start Programming. Address: 0x%08lX\n", packet.payload);
                // Your logic to signal the AVR to enter programming mode
                break;
                
            case 0xFE:
                printf("OP_CODE 0xFE: End Programming.\n");
                // Your logic to signal the AVR to exit programming mode
                break;
                
            case 0xFF:
                printf("OP_CODE 0xFF: Data Transfer Command (Size: %lu bytes expected)\n", packet.payload);
                // Here you would start the next blocking read for the actual large data packet (e.g., 260 bytes)
                break;
                
            default:
                printf("Unknown OP_CODE: 0x%02X\n", packet.op_code);
                // send_error_response();
                break;
        }
        
        // Send a response back to the PC to acknowledge the command
        // printf("ACK: Command processed.\n"); 
    }
    return 0;
}

