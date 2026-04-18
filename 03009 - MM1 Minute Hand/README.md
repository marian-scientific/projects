03009 MM1 Minute Hand
========================
*Anthony Remark*

The design calls for a simple UI that's able to be decipered by elementary school students. 

The big idea is that the MM1 Minute Hand is a user adjustable clock that digitally reads out the position of the Minute Hand within a resolution of 5 minutes. This simple function requires a multitued of disciplines in order to manufacture. FreeCAD was used to design gears that could be fashioned onto the shaft on an EN16AB series encoder. There was a large process in learning FreeCAD software to be able to accomplish these deisgns. Particularly it was a challenged not only to learn the software but to make a gear ratio design that's compatible with an hour-minute hand design that could be secured to the shaft of the chosen encoder.

Ideally, one would like to consider the whole design before purchasing any parts or commiting to anything, but at times the designer is forced to change plans and design around chosen parameters. In such cases where the parameters in question where rapidly chosen to meet deadlines. 

A plan was devised to drive the Encoders but an alternative was employed my means of the max7219 chip. This choice of max7219 ultimatly drove a lot of the electrical design choices. At first the stm32 bluepill was to be used but it proved insufficient for the max7219 (at least in the way the chip was used), so an Atmega328P was employed for use. 

A lot of learning was required to develop this project. And unfortunately this project has yet to be completed due to difficulties with the program and how the atmega328P interfaced with the max7219. Fortunately, the communication has seemed to be resolved but the software side still need to be worked on.

If these setbacks are resolved, then the next phase can begin where the project can be assembled. The electronics will be soldered to a more permanent fixture that can be placed inside the housing. The gears and minute hand will be assembled and fastened to the encoder. 


Below is the Logic of the MCU Programming:

MM1 Minute Hand MCU Program

For the minute hand, and bit bashing method is utilized to operate the Max7219 chip. A max7219 chip was chosen to be able to operate the seven segments with relative ease, but this requires a bit of code that the programmer is not used to. Lots of code and tips are taken from https://www.instructables.com/Arduino-Nano-8-Digit-Segment-Display/.

The program was once configured to operate on an stm32 bluepill but is now configured to operate an atmega328P. 

The phrase ‘#define MAX7219_DIN PD0’ means Pin 2 (PD0) is configured to be the Data In to the max7219. PD0 loads data into the internal 16 bit shift register on the CLK’s Rising Edge. The Phrase ‘MAX7219_CS PD1’ configured pin 3 to act as the chip select. Serial Data is loaded into the shift register while CS^{_} is Low (active low input). The phrase ‘MAX7219_CLK PD2’ configures pin 4 to be the CLK. 

The Encoder used is the EN16AB 12 position encoder. This encoder doesn’t have a sequential numeric output. The outputs are seemingly randomly chosen. The programmer decided to operate on a sequential logic anyway, so an array is chosen to implement this. The logic here is that we know what the output from the encoder will be given from this datasheet: https://www.ttelectronics.com/TTElectronics/media/ProductFiles/Datasheet/EN16AB.pdf.

On page 6 in the datasheet shows the output given the position. We want to create an array that takes in the literal output of the encoder and returns a numerical sequential number. Essentially the first position outputs a binary 0010 which is decimal 2. Therefor the array takes in the 0010 and outputs a decimal 1. Decimal 1 corresponds to position 1. We note that the arithmetic used to calculate the segment display output will benefit from every element in the array subtracting an integer value of 1.

This is why we have:

int myArray[]={11, 2, 0, 1, 10, 3, 5, 4, 8, 50, 7, 50, 9, 50, 6, 50}; // original array but minus 1, for the planned arithmetic

We declare a global variable integer delaytime = 5000. 

The pins on the Atmega328P are defined in the function “void initialise()” of the code.

We create a function called “void output(byte address, byte data)” which takes in data defined in the setup section of the program. Output() activates the chip select and shiftout(). Shiftout is an Arduino function that shifts out a byte of data one bit at a time. We configure that it starts with the most significant bit first, MSBFIRST. The program accesses the address of the register and then writes data to that register in the max7219. After that is done, the Chip select is deactivated. 

This output() function is called many times in the program. 

We proceed to the setup() section of the code before the loop section. First, the initialise() function is performed which makes the CS (chip select) High (therefore deactivates the CS) and readies the DIN, CS, CLK pins of the Atmega328P. The output() function is called five times in a row which doesn’t write data to the segment displays but instead sets up the max7219 to be used by the Atmega328P. The last two times output() is performed is to write 6 and 5 to the seven segment displays. 

After the outputs are performed, 4 pins on the Atmega328P are initialized for input so they can read the input from the encoder. 

We then proceed to the loop section of the program. This section will run repeatedly until the system is powered down. 

We have an initial delay of about 1 second. And integer is declared which takes in data read from the encoder and returns the decimal version. This integer is called ‘encoder’. The phrase myArray[encoder] returns the position of the encoder minus 1. There are two integers that are declared called dig1 and dig0. The integer dig1 should be the tenths place of the encoder position while dig0 is the ones place. 

Output() is then called which accesses the address of the register and writes the integer that’s supplied. Basically dig1 is written to the left segment (tenths place) and dig0 is written to the right segment (ones place). 

Another one second delay is called on.

Hypothetically speaking, the output of the two seven segment displays should correspond to the position of the encoder via a decimal output. This should operate as such until the whole system is shutdown. 


