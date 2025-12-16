#define F_CPU 16000000UL // (16MHz)
#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    
    DDRD |= (1 << PD5); // Set PD5 as output

    while (1) {
        // Toggle the LED
        PORTD ^= (1 << PD5);
        _delay_ms(500);
        PORTD &= -(1 << PD5);
        _delay_ms(500);
    }

    return 0;
}
