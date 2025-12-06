#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>

// --- Buzzer Definitions ---
#define BUZZER_PIN PB1          // Timer1 OC1A output pin (Pin 9 on Port B)
#define PRESCALER 8             // Prescaler N = 8
#define PRESCALER_BITS (1 << CS11) // Set CS11 for N=8

// Ascending notes
#define NOTE_G4 318
#define NOTE_B4 252
#define NOTE_D5 212

// Dissonant notes
#define NOTE_A4   283
#define NOTE_BFLAT4 267

void start_tone(uint16_t top_value) {

    // Configure Timer1 for Fast PWM Mode (Mode 14)
    TCCR1A = (1 << WGM11);
    TCCR1B = (1 << WGM13) | (1 << WGM12);

    // Frequency
    ICR1 = top_value;

    // 50% duty cycle
    OCR1A = top_value / 2;

    // non-inverting output on OC1A
    TCCR1A |= (1 << COM1A1);

    // Set Prescaler to 8 and START the timer
    TCCR1B |= PRESCALER_BITS;
}

// Stops the tone by disabling the timer clock
void stop_tone() {
    TCCR1A &= ~((1 << COM1A1) | (1 << COM1A0));
    TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10)); // Clear prescaler bits
    PORTB &= ~(1 << BUZZER_PIN);
}

void sound(uint16_t NOTE) {
    uint16_t duration_ms = 150;
    start_tone(NOTE);
    _delay_ms(duration_ms);
    stop_tone();
}

void success() {

    PORTC &= ~(1 << PC5);
    PORTC |= (1 << PC3);

    uint16_t notes[] = {NOTE_G4, NOTE_B4, NOTE_D5};
    uint16_t duration_ms = 150;
    for (int i = 0; i < 3; i++) {
        start_tone(notes[i]);
        _delay_ms(duration_ms);
    }
    stop_tone();
    
}

void failure() {
    
    PORTC &= ~(1 << PC5);
    PORTC |= (1 << PC4);
    
    uint16_t notes[] = {NOTE_A4, NOTE_BFLAT4, NOTE_A4};
    uint16_t duration_ms = 150;
    for (int i = 0; i < 3; i++) {
        start_tone(notes[i]);
        _delay_ms(duration_ms);
    }
    stop_tone();
}

uint16_t read_adc(uint8_t channel) {
    ADMUX = (1 << REFS0) | (channel & 0x0F);
    ADCSRA |= (1 << ADSC); 
    while (ADCSRA & (1 << ADSC)); 
    return ADC; 
}

void initialize_random_seed() {
    ADMUX = (1 << REFS0);
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

    // Read a few times to stabilize
    for (int i = 0; i < 5; i++) {
        read_adc(0);
        _delay_ms(1);
    }
    
    // Seed RNG using the noise from the unconnected ADC pin 0
    srand(read_adc(0));
}

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

int main(void) {

    // INIT BUZZER
    DDRB |= (1 << BUZZER_PIN);
    
    // INIT REED SWITCHES
    DDRC &= ~(1 << PC0);
    DDRC &= ~(1 << PC1);
    PORTC |= (1 << PC0);  
    PORTC |= (1 << PC1); 

    // INIT LEDS
    DDRC |= (1 << PC3);
    DDRC |= (1 << PC4);
    DDRC |= (1 << PC5);
    PORTC &= ~(1 << PC3);
    PORTC &= ~(1 << PC4);
    PORTC &= ~(1 << PC5);
    
    // INIT RAND
    initialize_random_seed();
    
    while (1) {

        int V0 = (rand() % 99) + 1; 
        int V1 = (rand() % 99) + 1;
        while (V1==V0)	{
        	int V1 = (rand() % 99) + 1;
	}

	TRY_AGAIN:

	max7219_send(1, V0/10); 
    	max7219_send(2, V0%10); 
    	max7219_send(3, V1/10); 
    	max7219_send(4, V1%10); 

	if (((!(PINC & (1 << PC0))) & (V0>V1)) | ((!(PINC & (1 << PC1))) & (V1>V0))) {
	  	success();
		while (!(PINC & (1 << PC0)));
    		PORTC &= ~(1 << PC3);
	}
	if (((!(PINC & (1 << PC0))) & (V0<V1)) | ((!(PINC & (1 << PC1))) & (V1<V0))) {
		failure();
		while (!(PINC & (1 << PC1)));
    		PORTC &= ~(1 << PC4);
		goto TRY_AGAIN;
	}
    		PORTC |= (1 << PC5);
    }

    return 0;
}
