# 12001 - RP2040 AVR PCB

AVR (Atmega329P) programmer via RP2040 zero dev board and custom in-house built PCB.

## Objective: 
To address a lot of pending questions for future designs involving microcontrollers on MS-produced PCBs along with some programming and toolchain investigations, this is an RP2040-powered programmer for the Atmega328P. The stretch goal is to have this investigation complete by 2025-12-21.

## Target Project Milestones: 

-[Completed 2025-12-15]: [Simple test blink program for atmega328p (easy)](marian-scientific/projects/12001%20-%20RP2040%20VR%20PCB/code/001_atmega328p_blink)

-[Completed 2025-12-15]: [Simple test blink program on waveshare rp2040 zero board (easy)](marian-scientific/projects/12001%20-%20RP2040%20VR%20PCB/code/002_rp2040_blink)

-[Completed 2025-12-15]: [Serial communication to control LED on rp2040 zero (medium)](marian-scientific/projects/12001%20-%20RP2040%20VR%20PCB/code/003_rp2040_serial_blink)

-Tuesday/Wednesday: Implement AVR ICSP protocol on rp2040, use oscilloscope to capture the output signal for debugging (hard)

-Thursday: computer program/script to send necessary serial commands to rp2040 to transmit the data and program the atmega328p (medium)

-Friday: PCB design for programmer circuit and some header pins/led on the atmega for simple debugging, like a dev board (medium)

-Saturday: Create the actual PCB in house (hard) 