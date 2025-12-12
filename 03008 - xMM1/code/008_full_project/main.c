#define F_CPU 8000000UL
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

void init_timer1(void) {

    // Configure Timer1 for Fast PWM Mode (Mode 14)
    TCCR1A = (1 << WGM11);
    TCCR1B = (1 << WGM13) | (1 << WGM12);

    // non-inverting output on OC1A
    TCCR1A |= (1 << COM1A1);

    // Set Prescaler to 8 and START the timer
    TCCR1B |= PRESCALER_BITS;
}

void start_tone(uint16_t top_value) {

    // Frequency
    ICR1 = top_value;

    // 50% duty cycle
    OCR1A = top_value / 2;

//    _delay_ms(20);
}

// Stops the tone by disabling the timer clock
void stop_tone() {
    TCCR1A &= ~((1 << COM1A1) | (1 << COM1A0));
    TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10)); // Clear prescaler bits
    PORTB &= ~(1 << BUZZER_PIN);
}

void success() {

    PORTC &= ~(1 << PC5);
    PORTC |= (1 << PC3);
    
    init_timer1();
    uint16_t notes[] = {NOTE_G4, NOTE_B4, NOTE_D5};
    for (int i = 0; i < 3; i++) {
        start_tone(notes[i]);
	for (int q=0; q<10; q++)
		_delay_ms(20);
    }
    stop_tone();
    
}

void failure() {
    
    PORTC &= ~(1 << PC5);
    PORTC |= (1 << PC4);
    
    init_timer1();
    uint16_t notes[] = {NOTE_A4, NOTE_BFLAT4, NOTE_A4};
    for (int i = 0; i < 3; i++) {
        start_tone(notes[i]);
	for (int q=0; q<10; q++)
		_delay_ms(20);
    }
    stop_tone();
}

uint16_t read_adc_channel(uint8_t channel) {
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
    ADCSRA |= _BV(ADSC);

    while (ADCSRA & _BV(ADSC));

    return ADC;
}

void initialize_random_seed() {

	_delay_ms(10);
	
	ADMUX = _BV(REFS0) | 2;
	ADCSRA = _BV( ADEN) | _BV(ADPS2);
	DIDR0 |= _BV(ADC2D);

	uint32_t seed=0;
    
    	for (int i = 0; i < 100; i++) {
        	uint16_t sample = read_adc_channel(2);

        	uint8_t noisy_bits = (uint8_t)(sample & 0x03); 

        	seed = (seed << 2) | noisy_bits;
        
        // _delay_us(10); 
   	 }

    	srand(seed);
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
    MAX7219_CS_DDR |= (1 << PB3) | (1 << PB5) | (1 << MAX7219_CS_PIN);
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

    max7219_send(0x00, 0x00);

    _delay_ms(10);

    max7219_send(MAX7219_REG_DISPLAY_TEST, 0x00);
    
    // scan from digit 0 to digit 3
    max7219_send(MAX7219_REG_SCAN_LIMIT, 0x03); 
    
    // set Decode Mode to BCD Decode for all digits
    max7219_send(MAX7219_REG_DECODE_MODE, 0xFF); 
    
    // set Intensity (Brightness)
    max7219_send(MAX7219_REG_INTENSITY, 0x02); 
    
    // exit Shutdown mode
    max7219_send(MAX7219_REG_SHUTDOWN, 0x01);

    _delay_ms(10);
}

int main(void) {

    // INIT BUZZER
    DDRB |= (1 << BUZZER_PIN);
    init_timer1();
    stop_tone();
    
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

    // INIT 7219MAX    
    spi_init();
    _delay_ms(10);
    max7219_init();
 
    while (1) {

        int V0 = (rand() % 99) + 1; 
        int V1 = (rand() % 99) + 1;
        while (V1==V0)	{
        	V1 = (rand() % 99) + 1;
	}

	max7219_send(1, 0xF); 
    	max7219_send(2, 0xF); 
	max7219_send(3, 0xF); 
    	max7219_send(4, 0xF); 

	if (V0/10)	
		max7219_send(1, V0/10); 
    	max7219_send(2, V0%10); 

	if (V1/10)	
		max7219_send(3, V1/10); 
    	max7219_send(4, V1%10); 

    while (1) {
  
    	PORTC |= (1 << PC5);
        _delay_ms(100);
	
	if (((!(PINC & (1 << PC0))) && (V0>V1)) || ((!(PINC & (1 << PC1))) && (V1>V0))) {
	  	_delay_ms(50);
		if (((!(PINC & (1 << PC0))) && (V0>V1)) || ((!(PINC & (1 << PC1))) && (V1>V0))) {
			success();
			while (!(PINC & (1 << PC0)) || !(PINC & (1 << PC1))){
        			_delay_ms(20);
			}
    			PORTC &= ~(1 << PC3);
			goto GO_AGAIN;
		}
	}
	else if (((!(PINC & (1 << PC0))) && (V0<V1)) || ((!(PINC & (1 << PC1))) && (V1<V0))) {
		_delay_ms(50);
		if (((!(PINC & (1 << PC0))) && (V0<V1)) || ((!(PINC & (1 << PC1))) && (V1<V0))) {
			failure();
			while (!(PINC & (1 << PC0)) || !(PINC & (1 << PC1))){
        	 	   	_delay_ms(20);
			}
    			PORTC &= ~(1 << PC4);
		}
	}
    	PORTC |= (1 << PC5);
    }
    GO_AGAIN:
}

    return 0;
}
