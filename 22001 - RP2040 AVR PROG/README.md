# 22001 - RP2040-ZERO ATMEGA328P PROGRAMMER

AVR (ATMEGA328P) programmer via RP2040-ZERO dev board on a custom in-house built PCB using a homebrew software toolchain.

## CODE
- [Sample ATMEGA328 C code](code/001_ATMEGA328P_SAMPLE_CODE/)
- [RP2040-ZERO In-Circuit Serial Programmer](code/002_RP2040-ZERO_ICSP/)
- [Python script to flash the device](code/003_PYSERIAL_TRANSMIT_IHEX/)

## Objective: 
Productionized and improved version of [12001](/12001%20-%20RP2040%20AVR%20PCB/README.md).

Improvements:
- large ground pour covering most of the BG copper, and other smaller copper fills to reduce the amount of copper needed to be etched away
- thicker traces
- fatter pads
- repositioned rp2040-zero board so USB cables can plug in more easily
- moved all copper construction to the BG from the FG to simplify the soldering
- added text to the foreground silkscreen (to be toner transfered)