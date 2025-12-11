avr-gcc -mmcu=atmega328p -Wall -Os main.c -o main.elf
avr-objcopy -j .text -j .data -O ihex main.elf main.hex
sudo avrdude -c usbasp -p m328p \
-U lfuse:w:0xE2:m \
-U hfuse:w:0xD9:m \
-U efuse:w:0xFF:m
sudo avrdude -c usbasp -p m328p -U flash:w:main.hex:i
