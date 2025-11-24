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

// ascending sound
void play_jingle() {
    uint16_t notes[] = {NOTE_G4, NOTE_B4, NOTE_D5};
    uint16_t duration_ms = 150;
    for (int i = 0; i < 3; i++) {
        start_tone(notes[i]);
        _delay_ms(duration_ms);
    }
    stop_tone();
}

// dissonant sound (A4 -> Bflat4 -> A4)
void play_bad_jingle() {
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

// Function to initialize ADC and get a seed
void initialize_random_seed() {
    // Initialize ADC: AVCC reference, MUX set to ADC0
    ADMUX = (1 << REFS0);
    // Enable ADC, set prescaler to 128
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

    // Read a few times to stabilize
    for (int i = 0; i < 5; i++) {
        read_adc(0);
        _delay_ms(1);
    }
    
    // Seed the random number generator using the noise from the unconnected ADC pin 0
    srand(read_adc(0));
}

int main(void) {

    initialize_random_seed();

    DDRC |= (1 << PC3);
    
    while (1) {
        int seconds = (rand() % 9) + 1; 
        PORTC |= (1 << PC3); 
        
	play_jingle();
        for (int i = 0; i < seconds; i++) {
            _delay_ms(1000); 
        }
        PORTC &= ~(1 << PC3);
       
	play_bad_jingle();

       	// short delay after
        _delay_ms(500); 
    }

    return 0;
}
