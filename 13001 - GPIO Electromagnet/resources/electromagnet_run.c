// using pico C sdk

#include "pico/stdlib.h"

#define PIN0 0

int main(){
	gpio_init(PICO_DEFAULT_LED_PIN);
	gpio_init(PIN0);
	gpio_set_dir(PICO_DEFAULT_LED_PIN,GPIO_OUT);
	gpio_set_dir(PIN0,GPIO_OUT);

	while (true){
		gpio_put(PICO_DEFAULT_LED_PIN,true);
		gpio_put(PIN0,true);
    }
}
