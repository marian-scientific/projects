# 03005 - Progress Tracker

### March 9, 2025:
* [RFP](https://github.com/marian-scientific/proposals/tree/Christ/RFP002%20-%20MCA) announced.

### March 15, 2025:
* [Proposal](https://github.com/marian-scientific/proposals/tree/Christ/RFP002%20-%20MCA/submitted%20proposals/VTBD) written and submitted.

### March 17, 2025: 0.5 hours (0.5/60)
* [Contract](https://github.com/marian-scientific/proposals/blob/Christ/RFP002%20-%20MCA/submitted%20proposals/EVALUATION_CRITERIA.md) awarded.
* Most required components and raw materials inventoried or ordered.

### March 18, 2025: 0.5 hours (1/60)
* Created this project directory
* LDR (apparently GM5539) measured 2 kOhms in light, 3 MOhms in dark, but that doesn't exactly match the spec sheet.

### March 19, 2025: 0.5 hours (1.5/60)
* Booted up old RPI4 as testbed for the sensors
* Created and tested below Python code to record sensor data on pin 2

```
import time
from datetime import datetime
import os
import sys
import RPi.GPIO as GPIO

orig_stdout=sys.stdout
f=open('out.txt','w')
sys.stdout=f

GPIO.setmode(GPIO.BCM)
pin=2
GPIO.setup(pin,GPIO.IN)

while(1):
    print(datetime.now(),", ",GPIO.input(pin))
    time.sleep(30)

sys.stdout=orig_stdout
f.close()
```

### March 20, 2025: 0 hours (1.5/60)
* can actually use shell redirection to simplify the above and remove all the sys stuff, and do python xxx.py > out.csv

### March 21, 2025: 3 hours (4.5/60)
* trialed and errored a working light sensor circuit with a BJT and a voltage threshold, with outputs both via LED and GPIO input to RPI4 (voltage divider with 1.7 V threshold, very close to LED anyway)

### March 22, 2025: 3 hours (7.5/60)
* wound solenoid with 500 turns of 28 AWG magnet wire around a plastic straw. soldered extension and breadboard leads to the solenoid
* tested the solenoid with 5V at 175mA, capable of drawing a thin 430 alloy stainless plunger, see video

[![solenoid](http://i.ytimg.com/vi/aOZ0N7JlEQg/hqdefault.jpg)](https://www.youtube.com/watch?v=aOZ0N7JlEQg)

### March 23, 2025: 1 hour (8.5/60)
* purchased materials for the frame of the output indicator
* retested the solenoid with 5V at 700mA, which should be safe for short bursts on this 28 AWG wire, and the solenoid was now able to draw in a 1/8" steel cylinder with a considerable amount of force, see video

[![solenoid, more current](http://i.ytimg.com/vi/GkqKA8dyDBk/hqdefault.jpg)](https://www.youtube.com/watch?v=GkqKA8dyDBk)