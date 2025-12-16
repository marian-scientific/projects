#include "pico/stdlib.h"
#include <stdio.h>

int main() {
    stdio_init_all();
    
    // Wait for USB serial connection
    sleep_ms(2000);
    
    bool LED_VALUE=0;
    const uint LED_PIN = 11;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, LED_VALUE);

    printf("Press 'j' to toggle pin.\n");
    
    while (true) {
        
	int c = getchar_timeout_us(0);  // Non-blocking
        
        if (c != PICO_ERROR_TIMEOUT) {
            	printf("You typed: %c (ASCII %d)\n", c, c);
		if (c=='j'){
			LED_VALUE=!LED_VALUE;
    			gpio_put(LED_PIN, LED_VALUE);
            		printf("Toggled!\n");
		}
        }

        sleep_ms(100);

    }
}
