#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include <string.h>

// SPI instance
#define SPI_PORT spi0
#define PIN_SCK   6
#define PIN_MOSI  7
#define PIN_MISO  4
#define PIN_RESET 3

// AVR programming commands
#define CMD_PROGRAMMING_ENABLE    0xAC530000
#define CMD_CHIP_ERASE           0xAC800000
#define CMD_READ_SIGNATURE       0x30000000
#define CMD_READ_FUSE_LOW        0x50000000
#define CMD_READ_FUSE_HIGH       0x58080000
#define CMD_READ_FUSE_EXT        0x50080000
#define CMD_WRITE_FUSE_LOW       0xACA00000
#define CMD_WRITE_FUSE_HIGH      0xACA80000
#define CMD_WRITE_FUSE_EXT       0xACA40000
#define CMD_LOAD_PROG_PAGE_LOW   0x40000000
#define CMD_LOAD_PROG_PAGE_HIGH  0x48000000
#define CMD_WRITE_PROG_PAGE      0x4C000000
#define CMD_READ_PROG_LOW        0x20000000
#define CMD_READ_PROG_HIGH       0x28000000

// ATmega328P signature bytes
#define ATMEGA328P_SIG0 0x1E
#define ATMEGA328P_SIG1 0x95
#define ATMEGA328P_SIG2 0x0F

// Programming parameters
#define SPI_FREQ_HZ 50000  // 250 kHz - safe for most AVR clock speeds
#define MAX_ENABLE_TRIES 32
#define PAGE_SIZE 128     // ATmega328P flash page size in bytes
#define FLASH_SIZE 32768   // 32KB

typedef struct {
	bool connected;
	uint8_t signature[3];
	uint8_t fuse_low;
	uint8_t fuse_high;
	uint8_t fuse_ext;
} avr_info_t;

// Send 4-byte SPI command and return 4-byte response
static uint32_t spi_transaction(uint32_t cmd) {
	uint8_t tx[4], rx[4];

	tx[0] = (cmd >> 24) & 0xFF;
	tx[1] = (cmd >> 16) & 0xFF;
	tx[2] = (cmd >> 8) & 0xFF;
	tx[3] = cmd & 0xFF;

	spi_write_read_blocking(SPI_PORT, tx, rx, 4);

	return (rx[0] << 24) | (rx[1] << 16) | (rx[2] << 8) | rx[3];
}

// Initialize SPI and GPIO
void avr_prog_init(void) {
	// Initialize SPI at 250 kHz, SPI mode 0
	spi_init(SPI_PORT, SPI_FREQ_HZ);
	spi_set_format(SPI_PORT, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);

	// Configure GPIO pins
	gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
	gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
	gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);

	// RESET pin as output, start high (not in reset)
	gpio_init(PIN_RESET);
	gpio_set_dir(PIN_RESET, GPIO_OUT);
	gpio_put(PIN_RESET, 1);
}

// Enter programming mode
bool avr_enter_programming_mode(void) {
	// Pulse reset to ensure clean state
	gpio_put(PIN_RESET, 1);
	sleep_ms(10);
	gpio_put(PIN_RESET, 0);
	sleep_ms(25);  // Wait for target to reset (min 20ms recommended)

	// Try to enable programming mode
	for (int i = 0; i < MAX_ENABLE_TRIES; i++) {
		uint32_t response = spi_transaction(CMD_PROGRAMMING_ENABLE);
		//printf("tryna init\n");
		// Check if byte 3 echoes 0x53
		if (((response>>8) & 0xFF) == 0x53) {
		
			uint8_t success = 0x00;
			fwrite(&success, 1, 1, stdout);
			fflush(stdout);
			return true;
		}

		// Small delay before retry
		sleep_us(100);

		// On failure, try pulsing SCK (some implementations do this)
		if (i > 0 && (i % 8) == 0) {
			gpio_put(PIN_RESET, 1);
			sleep_us(10);
			gpio_put(PIN_RESET, 0);
			sleep_ms(25);
		}
	}

	uint8_t failure = 0x01;
	fwrite(&failure, 1, 1, stdout);
	fflush(stdout);

	return false;
}

// Exit programming mode
void avr_exit_programming_mode(void) {
	gpio_put(PIN_RESET, 1);
	sleep_ms(1);

	uint8_t success = 0x00;
	fwrite(&success, 1, 1, stdout);
	fflush(stdout);
}

// Read device signature
bool avr_read_signature(uint8_t sig[3]) {
	sig[0] = spi_transaction(CMD_READ_SIGNATURE | 0x0000) & 0xFF;
	sig[1] = spi_transaction(CMD_READ_SIGNATURE | 0x0100) & 0xFF;
	sig[2] = spi_transaction(CMD_READ_SIGNATURE | 0x0200) & 0xFF;

	// Verify it's a valid signature (manufacturer ID should be 0x1E for Atmel)
	return (sig[0] == 0x1E);
}

// Read fuses
void avr_read_fuses(uint8_t *low, uint8_t *high, uint8_t *ext) {
	*low = spi_transaction(CMD_READ_FUSE_LOW) & 0xFF;
	*high = spi_transaction(CMD_READ_FUSE_HIGH) & 0xFF;
	*ext = spi_transaction(CMD_READ_FUSE_EXT) & 0xFF;
}

// Chip erase (required before programming)
void avr_chip_erase(void) {
	spi_transaction(CMD_CHIP_ERASE);
	sleep_ms(10);  // Typical erase time ~9ms, use 10ms to be safe
	
	uint8_t success = 0x00;
	fwrite(&success, 1, 1, stdout);
	fflush(stdout);
}

void avr_process_data(uint32_t len) {
	uint8_t data_buffer[132];

	// 1. BLOCKING READ from PC
	// We wait for the full 132-byte payload to arrive over USB
	size_t read_count = fread(data_buffer, 1, 4+len, stdin);

	if (read_count != (4+len)) {
		// Handle error: partial packet received
		uint8_t err = 0x02; 
		fwrite(&err, 1, 1, stdout);
		fflush(stdout);
		return;
	}

	// 2. PARSE THE ADDRESS (First 4 bytes of the 132-byte packet)
	// AVR Flash is word-addressed (1 word = 2 bytes)
	uint32_t start_address = (data_buffer[0] << 24) | (data_buffer[1] << 16) | 
		(data_buffer[2] << 8)  | (data_buffer[3]);

	// LOAD THE AVR PAGE BUFFER (Remaining 128 bytes)
	// We send 64*2 "Load" commands (high & low) (each command sends 1 word = 2 bytes)
	for (uint16_t i = 0; i < 64; i++) {
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

	// COMMIT THE PAGE TO FLASH
	// Command: 0x4C, HighAddr, LowAddr, 0x00
	uint8_t write_page[4] = {
		0x4C, 
		(uint8_t)((start_address/2) >> 8), 
		(uint8_t)((start_address/2) & 0xFF), 
		0x00
	};
	spi_write_read_blocking(SPI_PORT, write_page, NULL, 4);

	// Physically writing to Flash takes time (approx 5-10ms)
	sleep_ms(10);

	uint8_t success = 0x00;
	fwrite(&success, 1, 1, stdout);
	fflush(stdout);
	return;
}

bool avr_get_info(avr_info_t *info) {
	memset(info, 0, sizeof(avr_info_t));

	if (!avr_enter_programming_mode()) {
		return false;
	}

	info->connected = avr_read_signature(info->signature);

	if (info->connected) {
		avr_read_fuses(&info->fuse_low, &info->fuse_high, &info->fuse_ext);
	}

	return info->connected;
}

bool avr_read_info(){
	avr_info_t info;
	if (avr_get_info(&info)) {
		printf("Device detected!\n");
		printf("Signature: 0x%02X 0x%02X 0x%02X\n", 
				info.signature[0], info.signature[1], info.signature[2]);

		if (info.signature[0] == ATMEGA328P_SIG0 &&
				info.signature[1] == ATMEGA328P_SIG1 &&
				info.signature[2] == ATMEGA328P_SIG2) {
			printf("Device: ATmega328P\n");
		}
		else {
			printf("Device model not supported.\n");
		}

		printf("Fuses: L=0x%02X H=0x%02X E=0x%02X\n",
				info.fuse_low, info.fuse_high, info.fuse_ext);
		return true;
	}
	else {
		printf("Failed to read device info.\n");
		printf("Check connections and target power.\n");
		return false;
	}

}

int main(void) {
	
	stdio_init_all();
	
	size_t bytes_read = 0;
	uint8_t buffer[6];


	sleep_ms(1000);


	while (1) {

		fread(buffer, 1, 6, stdin);
		uint8_t op_code=buffer[0];
		uint32_t payload = (buffer[1] << 24) | (buffer[2] << 16) | (buffer[3] << 8)  | buffer[4];

		switch (op_code) {
			case 0x01:
				avr_prog_init();
				avr_enter_programming_mode();
				break;
			case 0x02:
				avr_read_info();
				break;
			case 0x03:
				avr_chip_erase();
				break;
			case 0xFE:
				avr_exit_programming_mode();
				break;
			case 0xFF:
				avr_process_data(payload);
				break;
			default:
				break;
		}

	}

	return 0;
}
