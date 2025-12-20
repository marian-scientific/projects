avr-gcc -mmcu=atmega328p -Wall -Os main.c -o main.elf
avr-objcopy -j .text -j .data -O ihex main.elf main.hex
# sudo avrdude -c usbasp -p m328p -U flash:w:main.hex:i
