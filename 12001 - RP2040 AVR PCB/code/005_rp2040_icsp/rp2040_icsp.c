#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"

#define WS2812_PIN 16 
#define IS_RGBW false
#define NUM_PIXELS 1

static inline void set_neopixel_color(uint32_t color) {
	pio_sm_put_blocking(pio0, 0, color); 
}

uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
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
//#define SPI_FREQ 250


void setup_spi() {

	spi_init(SPI_PORT, SPI_FREQ);

	gpio_set_function(SCK_PIN, GPIO_FUNC_SPI);
	gpio_set_function(MOSI_PIN, GPIO_FUNC_SPI);
	gpio_set_function(MISO_PIN, GPIO_FUNC_SPI);

	// CS not needed because reset acts as CS
	//	gpio_set_function(CS_PIN, GPIO_FUNC_SPI);

	gpio_init(RESET_PIN);//, GPIO_OUT); 
	gpio_set_dir(RESET_PIN, GPIO_OUT); 
	gpio_put(RESET_PIN, 1); // normal operation

	spi_set_format(
			SPI_PORT,
			8,          // 8 bits per transfer
			SPI_CPOL_0, // CPOL=0
			SPI_CPHA_0, // CPHA=0
			SPI_MSB_FIRST // Data is sent MSB first
		      );
}

#define PACKET_SIZE 6
typedef struct {
	uint8_t op_code;    // 1 byte: The command (e.g., 0x01, 0xFE, 0xFF)
	uint32_t payload;   // 4 bytes: Parameter, Address, or Size
	uint8_t checksum;   // 1 byte: Integrity check
} command_packet_t; // Total 6 bytes

void start_programming() {

	uint8_t command[4];
	uint8_t response[4];

	// pull reset pin low
	gpio_put(RESET_PIN, 0);
	sleep_ms(200);

	// programming enable
	// AVR Command: 0xAC 0x53 0x00 0x00
	command[0] = 0xAC; // Programming Enable
	command[1] = 0x53; // Check value
	command[2] = 0x00;
	command[3] = 0x00;
/*	while(1)
	{	gpio_put(RESET_PIN, 1);
	sleep_ms(20);
	gpio_put(RESET_PIN, 0);
	sleep_ms(20);*/

	spi_write_read_blocking(SPI_PORT, command, response, 4);
	//}

	printf("RESPONSE Byte 0: 0x%02X\n", response[0]);
	printf("RESPONSE Byte 1: 0x%02X\n", response[1]);
	printf("RESPONSE Byte 2: 0x%02X\n", response[2]);
	printf("RESPONSE Byte 3: 0x%02X\n", response[3]);

	// AVR usually responds with 0x53 0x00 in bytes 2 and 3 if successful.
	if (response[2] != 0x53) {
		printf("Failed to enter programming mode!\n");
		return;
	}
	printf("Entered Programming Mode\n");

	// chip erase
	// AVR Command: 0xAC 0x80 0x00 0x00
	command[0] = 0xAC; 
	command[1] = 0x80;
	command[2] = 0x00;
	command[3] = 0x00;
	spi_write_read_blocking(SPI_PORT, command, response, 4);
	sleep_ms(10); // must wait until it's done
	printf("Chip Erased\n");

	uint8_t success = 0x00;
	fwrite(&success, 1, 1, stdout);
	fflush(stdout);
	return;

}

void process_data(uint32_t len) {
	uint8_t data_buffer[260];

	// 1. BLOCKING READ from PC
	// We wait for the full 260-byte payload to arrive over USB
	size_t read_count = fread(data_buffer, 1, len, stdin);

	if (read_count != len) {
		// Handle error: partial packet received
		uint8_t err = 0x02; 
		fwrite(&err, 1, 1, stdout);
		fflush(stdout);
		return;
	}

	// 2. PARSE THE ADDRESS (First 4 bytes of the 260-byte packet)
	// AVR Flash is word-addressed (1 word = 2 bytes)
	uint32_t start_address = (data_buffer[0] << 24) | (data_buffer[1] << 16) | 
		(data_buffer[2] << 8)  | (data_buffer[3]);

	// 3. LOAD THE AVR PAGE BUFFER (Remaining 256 bytes)
	// We send 128 "Load" commands (each command sends 1 word = 2 bytes)
	for (uint16_t i = 0; i < 128; i++) {
		uint8_t low_byte  = data_buffer[4 + (i * 2)];
		uint8_t high_byte = data_buffer[4 + (i * 2) + 1];
		uint16_t word_addr = i; // Offset within the page

		// Load Low Byte Command: 0x40, 0x00, LowAddr, Data
		uint8_t load_low[4] = {0x40, 0x00, (uint8_t)word_addr, low_byte};
		spi_write_read_blocking(SPI_PORT, load_low, NULL, 4);

		// Load High Byte Command: 0x48, 0x00, LowAddr, Data
		uint8_t load_high[4] = {0x48, 0x00, (uint8_t)word_addr, high_byte};
		spi_write_read_blocking(SPI_PORT, load_high, NULL, 4);
	}

	// 4. COMMIT THE PAGE TO FLASH
	// This physically writes the buffer into the Flash memory at the start_address
	// Command: 0x4C, HighAddr, LowAddr, 0x00
	uint8_t write_page[4] = {
		0x4C, 
		(uint8_t)(start_address >> 8), 
		(uint8_t)(start_address & 0xFF), 
		0x00
	};
	spi_write_read_blocking(SPI_PORT, write_page, NULL, 4);

	// 5. WAIT FOR WRITE CYCLE
	// Physically writing to Flash takes time (approx 5-10ms)
	sleep_ms(10);

	// 6. RESPONSE TO PC
	uint8_t success = 0x00;
	fwrite(&success, 1, 1, stdout);
	fflush(stdout);
	return;
}

void test(){
while(true) {
    uint8_t test_data = 0xAA;
    spi_write_read_blocking(spi0, &test_data, NULL, 1);
  //  sleep_ms(10);
    printf("testing...\n");
}
}

int main() {

	unsigned char greenness=0;
	command_packet_t packet;
	size_t bytes_read = 0;

	stdio_init_all();
	setup_spi();


	PIO pio = pio0; // Use PIO block 0
	int sm = 0;     // Use State Machine 0
	uint offset = pio_add_program(pio, &ws2812_program);
	ws2812_init(pio, sm, WS2812_PIN, 800000);

	set_neopixel_color(urgb_u32(0x00, greenness, 0x00));

	sleep_ms(1000); // Wait for USB serial connection

	printf("spi setup complete\n");

//	test();

	while (true) {

		bytes_read = fread(&packet, 1, PACKET_SIZE, stdin);

		if (bytes_read != PACKET_SIZE) {
			printf("\nTRANSFER ERROR\n");
			continue; 
		}

		printf("COMMAND RECEIVED: 0x%02X\n", packet.op_code);

		switch (packet.op_code) {
			case 0x01:
				printf("OP_CODE 0x01: START PROGRAMMING\n");
				start_programming();
				break;

			case 0xFE:
				printf("OP_CODE 0xFE: STOP PROGRAMMING\n");
				gpio_put(RESET_PIN,1);
				break;

			case 0xFF:
				printf("OP_CODE 0xFF: DATA TRANSFER (%lu BYTES)\n", packet.payload);
				process_data(packet.payload);
				break;

			default:
	//			start_programming();
				printf("UNKNOWN OP_CODE: 0x%02X\n", packet.op_code);
				break;
		}

		printf("ACK - COMMAND PROCESSED\n"); 
	}
	return 0;
}

