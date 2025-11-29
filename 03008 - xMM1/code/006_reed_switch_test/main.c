#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>

// --- Buzzer Definitions ---
#define BUZZER_PIN PB1          // Timer1 OC1A output pin (Pin 9 on Port B)
#define PRESCALER 8             // Prescaler N = 8
#define PRESCALER_BITS (1 << CS11) // Set CS11 for N=8

// Notes
#define NOTE_G4 318
#define NOTE_D5 212

void start_tone(uint16_t top_value) {
    DDRB |= (1 << BUZZER_PIN);

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

int main(void) {

    DDRC &= ~(1 << PC0);
    DDRC &= ~(1 << PC1);
    PORTC |= (1 << PC0);  
    PORTC |= (1 << PC1); 

    while (1) {
       if (!(PINC & (1 << PC0)))
	       sound(NOTE_G4);
       if (!(PINC & (1 << PC1)))
	       sound(NOTE_D5);
    }

    return 0;
}
