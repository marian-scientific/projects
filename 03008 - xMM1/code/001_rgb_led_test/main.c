#define F_CPU 16000000UL // (16MHz)
#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    
    DDRD |= (1 << PD5); // Set PD5 as output
    DDRD |= (1 << PD6); // Set PD6 as output
    DDRD |= (1 << PD7); // Set PD7 as output

    while (1) {
        // Toggle the LED
        PORTD &= -(1 << PD7);
        PORTD ^= (1 << PD5);
        _delay_ms(10);
        PORTD &= -(1 << PD5);
        PORTD ^= (1 << PD6);
        _delay_ms(10);
        PORTD &= -(1 << PD6);
        PORTD ^= (1 << PD7);
        _delay_ms(10);
    }

    return 0;
}
