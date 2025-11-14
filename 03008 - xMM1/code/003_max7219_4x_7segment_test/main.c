#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#define F_CPU 16000000UL

#define MAX7219_REG_SHUTDOWN      0x0C
#define MAX7219_REG_SCAN_LIMIT    0x0B
#define MAX7219_REG_INTENSITY     0x0A
#define MAX7219_REG_DECODE_MODE   0x09
#define MAX7219_REG_DISPLAY_TEST  0x0F

#define MAX7219_CS_PORT  PORTB
#define MAX7219_CS_DDR   DDRB
#define MAX7219_CS_PIN   PB2

void spi_init(void);
void max7219_send(uint8_t address, uint8_t data);
void max7219_init(void);

void spi_init(void) {
    DDRB |= (1 << PB3) | (1 << PB5) | (1 << MAX7219_CS_PIN);
    MAX7219_CS_PORT |= (1 << MAX7219_CS_PIN);
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0); 
}

void max7219_send(uint8_t address, uint8_t data) {
    MAX7219_CS_PORT &= ~(1 << MAX7219_CS_PIN);
    SPDR = address; 
    while(!(SPSR & (1 << SPIF)));
    SPDR = data;
    while(!(SPSR & (1 << SPIF)));
    MAX7219_CS_PORT |= (1 << MAX7219_CS_PIN);
}

void max7219_init(void) {
    max7219_send(MAX7219_REG_DISPLAY_TEST, 0x00);
    
    // scan from digit 0 to digit 3
    max7219_send(MAX7219_REG_SCAN_LIMIT, 0x03); 
    
    // set Decode Mode to BCD Decode for all digits
    max7219_send(MAX7219_REG_DECODE_MODE, 0xFF); 
    
    // set Intensity (Brightness)
    max7219_send(MAX7219_REG_INTENSITY, 0x05); 
    
    // exit Shutdown mode
    max7219_send(MAX7219_REG_SHUTDOWN, 0x01);
}

void max7219_display_digit(uint8_t digit, uint8_t value) {
}

int main(void) {
    spi_init();
    max7219_init();
    	max7219_send(1, 1); 
    	max7219_send(2, 2); 
    	max7219_send(3, 3); 
    	max7219_send(4, 4); 

    while(1) {
    }

    return 0;
}
