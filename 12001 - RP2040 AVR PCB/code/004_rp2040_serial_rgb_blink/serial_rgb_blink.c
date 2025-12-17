#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h" // PIO helper functions
#include <stdio.h>

// --- Configuration ---
#define WS2812_PIN 16 
#define IS_RGBW false // This LED is typically RGB, not RGBW
#define NUM_PIXELS 1 // Only one LED on the board

// --- Global PIO State Machine ---
static inline void set_neopixel_color(uint32_t color) {
    // Put the 32-bit color value (GGRRBBTT) into the PIO TX FIFO
    pio_sm_put_blocking(pio0, 0, color); 
}

// Function to convert R, G, B bytes into the 32-bit format the PIO expects
uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    // The PIO expects the color data in GGRRBB format, with the 4th byte being trash (TT).
    return ((uint32_t)g << 24) | ((uint32_t)r << 16) | ((uint32_t)b << 8) | 0;
}

void ws2812_init(PIO pio, uint sm, uint pin, float freq) {
    uint offset = pio_add_program(pio, &ws2812_program);
    
    pio_gpio_init(pio, pin);
    pio_sm_set_consecutive_pindirs(pio, sm, pin, 1, true);
    
    pio_sm_config c = ws2812_program_get_default_config(offset);
    sm_config_set_sideset_pins(&c, pin);
    sm_config_set_out_shift(&c, false, true, 24);
    sm_config_set_fifo_join(&c, PIO_FIFO_JOIN_TX);
    
    int cycles_per_bit = ws2812_T1 + ws2812_T2 + ws2812_T3;
    float div = clock_get_hz(clk_sys) / (freq * cycles_per_bit);
    sm_config_set_clkdiv(&c, div);
    
    pio_sm_init(pio, sm, offset, &c);
    pio_sm_set_enabled(pio, sm, true);
}

int main() {
    stdio_init_all();
	// 1. Initialize the PIO State Machine
	PIO pio = pio0; // Use PIO block 0
	int sm = 0;     // Use State Machine 0
	uint offset = pio_add_program(pio, &ws2812_program);
    	ws2812_init(pio, sm, WS2812_PIN, 800000);

    // Wait for USB serial connection
    sleep_ms(1000);
    

    unsigned char greenness=0;

	set_neopixel_color(urgb_u32(0x00, greenness, 0x00));
    //const uint LED_PIN = 11;
    //gpio_init(LED_PIN);
    //gpio_set_dir(LED_PIN, GPIO_OUT);
    //gpio_put(LED_PIN, LED_VALUE);

    printf("Press 'j' to toggle pin.\n");
    
    while (true) {
        
	int c = getchar_timeout_us(0);  // Non-blocking
        
        if (c != PICO_ERROR_TIMEOUT) {
            	printf("You typed: %c (ASCII %d)\n", c, c);
		if (c=='j'){
			greenness=~greenness;
			set_neopixel_color(urgb_u32(0x00, greenness, 0x00));
    			//gpio_put(LED_PIN, LED_VALUE);
            		printf("Toggled!\n");
		}
        }

        sleep_ms(100);

    }
}
