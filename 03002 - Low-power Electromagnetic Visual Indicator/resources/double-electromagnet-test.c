#include "pico/stdlib.h"

#define OUT1 0
#define OUT2 1
#define IN 15

int main(){
	gpio_init(PICO_DEFAULT_LED_PIN);
	gpio_init(OUT1);
	gpio_init(OUT2);
	gpio_init(IN);
	gpio_set_dir(PICO_DEFAULT_LED_PIN,GPIO_OUT);
	gpio_set_dir(OUT1,GPIO_OUT);
	gpio_set_dir(OUT2,GPIO_OUT);
	gpio_set_dir(IN,GPIO_IN);
    	gpio_pull_up(IN);

	bool switch0;

	while (true){
		switch0=gpio_get(IN);
		if (switch0){
			gpio_put(PICO_DEFAULT_LED_PIN,true);
			gpio_put(OUT1,true);
			gpio_put(OUT2,false);
		}
		else
		{
			gpio_put(PICO_DEFAULT_LED_PIN,false);
			gpio_put(OUT1,false);
			gpio_put(OUT2,true);
		}
			gpio_put(PICO_DEFAULT_LED_PIN,false);
    	}
}
