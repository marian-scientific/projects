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
    
    while (1) {
  
	if (!(PINC & (1 << PC0))) {
	  	success();
		while (!(PINC & (1 << PC0)));
    		PORTC &= ~(1 << PC3);
	}
 	if (!(PINC & (1 << PC1))) {
		failure();
		while (!(PINC & (1 << PC1)));
    		PORTC &= ~(1 << PC4);
	}
    		PORTC |= (1 << PC5);
    }

    return 0;
}
