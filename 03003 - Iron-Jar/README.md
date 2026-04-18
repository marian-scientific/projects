03003 Iron-Jar
========================
*Anthony Remark*

This Project served as largely an investigation on the possibility of a MCU being able to produced an observable phenomenon. Static electricity was the main Idea for use. Essentially This Project is inspired by the Leydan Jar as seen in Physics II experiments. It's the idea of showing capacitance between two conducters. 

The Experiment had an MCU hooked up to two conductors that have capacitance in between them. The MCU was programmed to randomly charge the capacitor or not charge the capacitor. The Experimentor had the responsibility to charge up either a ebonite rod or a glass rod. Iron Filings where placed on the positive conductor and it was observed to see how many filings where attracted to each rod. 

The MCU would charge the capacitance (or not charge) and the audience would observe the change in phenomenon if there was any. The audience and Experimentor were not aware of the decisions by the MCU to charge the capacitance or not. The Results were sort of inconclusive but also reliable. 

The Programing of the MCU is described as follows:

IRON JAR MCU program

The Arduino Uno was used for this experimental set up.

We are conducting a set of experiments where we have two conductor nodes connected to the Arduino. Typically these conductors are copper tape. There’s a capacitor connected to the nodes, or several capacitors. Iron filings are placed on both conductors, or one of them and either a glass rod or an ebonite rod is used to pick up the iron filings. The idea is that the Arduino charges the capacitors and therefore the conductors are charged positive or negative. The rods are rubbed and used to determine which way the Arduino charged the capacitors. 

So therefore the Arduino is programmed to use randomness to perform the task at hand. 

So the setup for the code proceeds. 

Pin 13 is setup for output because it’s going to control the built in LED. This LED will be used to indicate if the push button is pressed. Pin 12 is set for output as well, when this LED is turned on, then it should indicate that the capacitors should be charged. They are charged if the experiment is set up correctly. Pin 11 is setup for input because it’s going to read if the push button is pressed. The Arduino can tell if the push button is pressed because if it it pressed, a circuit is complete and the Arduino will read a sufficient voltage. 

Pin 10 is configured for output because this pin is responsible for charging the capacitor and consequently should be configured the same as whatever pin 12 is.

After the code is setup, then we proceed to the loop. What ever is written here will be ran repeatedly until the Arduino is shut down. 

The phrase, int sensor = digitalRead(11); operates on pin 11. This line reads the input to pin 11 which the push button should be connected to. The phrase returns an integer. An integer is used because the rest of the program utilizes this. 

Pin 12 is set to Low (digitalWrite(12, LOW)) which means that the LED responsible to telling us if the Capacitor is supposed to be charged or not is turned off.

Next we would like to tell the user if the push button is pressed. So an IF statement is written. If the integer ‘sensor’ is a value above 0, this indicates that the button was pressed. So then pin 13 is set to high which lights up the built in LED.

When the button is pressed, the Arduino randomly selects a number between 1 and 11. There are two IF statements that follow. 

If the random number is above 5, then the capacitor is configured to be charged. The Arduino does this by setting pins 12 and 10 to HIGH. In the configuration for the experiment, there’s a buffer chip used in between the conductor and the Arduino. 

If the random number is equal to 5 or lower, then pin 12 and 10 are set to low, which should configure the capacitors to not be charged either way. 

For either case, the program will wait approximately 10 seconds. After this duration, the built in LED is turned off. It does this by setting pin 13 to low. In theory the capacitor should still be in the same state. Either charged or discharged. 

In either case, the loop proceeds to start again, but the program waits for the button to be pressed. Otherwise the program will wait. 

 * [Request for Proposals](https://github.com/marian-scientific/proposals/tree/Christ/RFP001%20-%20HOHMO) - February 19, 2025
 * [Initial Proposal](https://github.com/marian-scientific/proposals/tree/Christ/RFP001%20-%20HOHMO/submitted%20proposals/IRON-JAR) - February 22, 2025 
 * [Final Report](https://github.com/marian-scientific/reports/blob/Christ/03003%20-%20Iron-Jar/FINAL_REPORT.md) - March XX, 2025
