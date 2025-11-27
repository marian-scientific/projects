// #define F_CPU 16000000UL // (16MHz)
#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    
    DDRC |= (1 << PC3); // Set PC3 as output
    DDRC |= (1 << PC4); // Set PC4 as output
    DDRC |= (1 << PC5); // Set PC5 as output

    PORTC &= -(1 << PC3);
    PORTC &= -(1 << PC4);
    PORTC &= -(1 << PC5);

    while (1) {
        // Toggle the LED

        PORTC = (1 << PC5);
        _delay_ms(200);
      	
	PORTC = (1 << PC4);
        _delay_ms(200);
   	
	PORTC = (1 << PC3); 
 	_delay_ms(200);

    }


    return 0;
}
