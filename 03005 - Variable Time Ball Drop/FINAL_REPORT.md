03005 - Variable-Time Ball Drop (VTBD)
======================================
*Matt DiPalma, AMDG*


Schedule
--------
  * March 9, 2025 - RFP002 posted
  * March 15, 2025 - proposal submitted
  * March 17, 2025 - ontract awarded
  * March 21, 2025 - source-by date for all prototype raw materials
  * March 28, 2025 - underlying technology demonstrators due
  * April 11, 2025 - final prototypes complete
  * April 18, 2025 - documentation complete

Abstract
---------
This project investigates various mechanisms that show potential for the purpose of estimating the passage of time with varying degrees of accuracy and home-producibility: sunlight detection, water evaporation, and an RC-timer based electric circuit. Though data was collected for all of the phenomena, the RC-timer based circuit had a timing accuracy that far exceeded the others. Therefore, a circuit was designed, built, and tested that leveraged a RC timer and a binary ripple counter to implement a simple alarm. The output indicator for this alarm was also extremely simple: a hand-wound solenoid that, when activated, drew in a steel pin, allowing an acorn to fall into a cup, producing an audible sound. The demonstration prototype is capable of creating alarm delays of between 5.5 and 11.5 hours, which is appropriate for the purpose of an alarm clock.

Click for video of Variable-Time Ball Drop alarm:

[![timer final](http://i.ytimg.com/vi/wb-KzfldAP8/hqdefault.jpg)](https://youtube.com/shorts/wb-KzfldAP8)

Objective
---------
There are many objectives of this investigation. The primary objective is to prototype minimal implementations of a timer and integrate them alongside a simple output indicator to form a primitive alarm.

All of the designs considered for this investigation leveraged electrical components, though some required a greater number of components of higher complexity. To be clear, primitive versions of components like resistors, capacitors, and inductors can, if absolutely necessary, be produced in the home workshop, whereas components that leverage semiconductors are not reasonable to self-source. The objective of limiting the number and complexity of electrical components is especially beneficial to limit dependency on a wide and often import-heavy supply chain.

This project is also a foray into the development of a device with a degree of human interaction, as the device will contain a set of inputs that set the alarm time delay. This is the first project at Marian Scientific with such a criterion.

This investigation is also an opportunity to use various microcontroller devices for data collection, something that has not been pursued at Marian Scientific. The data collected in the course of this project spans a wide range of topics, from light-dependent resistor (LDR) voltage output through the day/night cycle, evaporation rates & conductivities for various solutions, and thresholds for various transistor devices.

Another goal of this project is to use more commonplace materials such as wood, cardboard, and otherwise naturally harvested materials, wherever possible, in accordance with [MP07](https://github.com/marian-scientific/wiki/wiki/MP07-%E2%80%90-Vertical-Integration). For example, as will be later described, a simple acorn was used in place of a manufactured ball, limiting a dependency and making the prototype more eco-friendly and cheaper.

The prototype also continued to pursue self-sourced electromagnets for the purposes of output indication. This parallels the efforts of [13001](https://github.com/marian-scientific/reports/tree/Christ/13001%20-%20GPIO%20Electromagnet) and [03002](https://github.com/marian-scientific/reports/tree/Christ/03002%20-%20Low-power%20Electromagnetic%20Visual%20Indicator) at Marian Scientific.

Lastly, this project was an opportunity to pilot the new reporting format at Marian Scientific (see [PROGRESS_TRACKER.md](https://github.com/marian-scientific/reports/blob/Christ/03005%20-%20Variable%20Time%20Ball%20Drop/PROGRESS_TRACKER.md)), which involved detailing summaries of daily tasks with photo, video, and other evidence, alongside a running tally of labor hours spent on the project. This serves many purposes. First, it serves as an excellent journal of tasks completed, which makes compiling this final report extremely straightforward, while being a great record of tasks completely and observations made, which can be referenced in the future. Second, it enables future project labor estimates to be more accurately made by serving as a basis of comparison. Finally, it allows for a formal archive of all endeavors relative to the project scope. As such, this report does not need to exhaustively regurgitate all steps of the investigation and design process, as that list already exists in the accompanying progress tracker document. To this end, this report will focus primarly on the downselected prototype, with only a brief summary of the investigations made on the alternate configurations.

Parts List
----------
The materials required for the construction of the solenoid are:
* 28 AWG enameled copper "magnet" wire
* 2.5" plastic tube (repurposed thick drinking straw)
* hot glue
* 600-grit sandpaper
* solid core wire
* heat-shrink tubing

The materials required for the construction of the output indicator are:
* 10mm square wood rod (dimensions unimportant)
* 3/4" wood project panel (dimensions unimportant)
* cardboard (dimensions unimportant)
* plastic tube (same diameter as required above)
* hot glue / PVA glue
* 0.032"-diameter stainless steel wire (alloy 430) (McMaster # 	89065K81)
* acorn

The materials used in the construction of the RC timer circuit:
* resistors: 100, 1e5, 1e6, & 2.2e6 ohms
* capacitors: 2x 1e-5, 1e-8 farads
* switch
* potentiometer: 0-1e6 ohms
* LED (debugging only)
* CD4060 ripple counter IC
* CD40106 inverter IC (or NOT gate)
* 555 timer IC
* RFP30N06LE N-channel MOSFET
* solid core wire
* 5V power supply

The materials used in the construction of test setup for the unpursued Concept B:
* GM5539 light-dependent resistor
* MCP3008 ADC
* RPI 4 or Zero for data collection
* misc. electronic components

The materials used in the construction of test setup for the unpursued Concept C:
* small dishes
* paper towel
* water (tap and salt)
* MCP3008 ADC
* RPI 4 or Zero for data collection
* misc. electronic components

Design & Procedure
------------------
### Electromagnets
Two 03002-001 electromagnets were fabricated and used in the implementation of both Concept A (electromagnetic compass visual indicator) and Concept B (electromagnetic pendulum visual indicator). They were each constructed by cutting 1" lengths of the 3/8"-diameter stainless steel rod using a hacksaw. The resulting sharp corners were filed to remove sharp burrs that otherwise pose a risk of cutting skin and/or the thin wire used downstream in the construction of the electromagnets.

A temporary wire-winding jig was constructed that suspended the coil of magnet wire in a way that could be ergonomically unraveled while the electromagnets were wound.

A small piece of tape was affixed to both ends of the rod in order to prevent any wire wound around the rod from sliding off either end. A strong permanent magnet with a hook (conventionally used for hanging various items) was then stuck to one of the ends of the small steel rod segments, in order to provide a handle by which each rod could be rotated in order to wind the coil around it. 

![Coil Winding Rig](resources/coil-winding-rig.jpg)

Leaving enough lead of magnet wire (2-3") free on both ends, 1000 turns of the 36 AWG magnet wire were wound around each rod, taking care to wind the coil neatly and evenly, such that the diameter was relatively constant along the entire length of the electromagnet.

![Coil](resources/coil.jpg)

A small region of the enamel at the end of both leads on each electromagnet was abraded off using the sandpaper. The leads were then connected to jumper wires by solder and each joint was strengthened with heat-shrink tubing. 

### Test Circuit & Code
The circuit implemented to test whether or not the prototypical visual indicators would function correctly leveraged a Raspberry Pi Pico and its GPIO pins, which are capable of outputing roughly 15 mA of current at a 3.3V logic level, in line with the design specifications outlined in the [RFP](https://github.com/marian-scientific/proposals/tree/Christ/RFP001%20-%20HOHMO). The circuit itself is trivial. The two electromagnets, each having a resistance of roughly 50 ohms, were wired each in series with 200 ohms of additional resistance, and connected between a GPIO (output) pin and a ground pin of the microcontroller. As such, a current of roughly 13 mA could be actively toggled on/off through either of the electromagnetics, activating their respective magnetic fields. A switch was also connected to another GPIO (input) pin for the purpose of switching between the active electromagnets. 

![Circuit](resources/circuit.jpg)

The code required for this test circuit, to be uploaded to the Pico, is [attached to this report](resources/double-electromagnet-test.c), leveraging the Pico C SDK.

### Concept A - Electromagnet Compass Visual Indicator
A small cup was 3D-printed and filled with water. A stick, having a thickness less than the inner diameter of the cup, was sliced into a  1/4" thick wafer. A notch was cut into the wooden disk, and a small length of stainless steel wire was glued into the notch. The wooden disk was then placed into the cup of water. This disk setup acts as compass needle, though one not inclined to reorient to point along Earth's magnetic field lines.

![Compass Parts](resources/compass_parts.jpg)

The electromagnets were then placed next to the cup, oriented radially, with roughly 60 deg spacing between them. A piece of paper with "0" and "1" written as output indicators was placed opposite the electromagnets.

The microcontroller was turned on, and the switch was flipped multiple times to test the function of the prototype.

### Concept B - Electromagnet Pendulum Visual Indicator
The below frame was constructed. The exact dimensions of the frame structure are not expected to be critical for its proper function. 

![Stick Frame](resources/stick_frame.jpg)

Simply, the frame suspends a magnetic stainless steel nut from two vertical posts by a thin thread, giving it enough clearance to swing freely above whatever frame structure is below it. The frame should also position the two electromagnets in such a way that their magnetic fields, when activated, would draw the steel weight towards them in the most efficient way possible.

![Pendulum](resources/pendulum.jpg)

One key detail of this apparatus is that it requires a small piece of tape to be applied to the ends of the electromagnets that are closest to the suspended weight. The critical function of this small piece of tape is described in the Conclusions below.

The microcontroller was turned on, and the switch was flipped multiple times to test the function of the prototype.

Results & Observations
----------------------
Both prototypes were successful in generating a visual output, albeit a small one. Videos of the prototypes working are linked in the Abstract section.

The compass indicator was significantly slower in its response time, as the needle had to slowly swing back and forth and equilibrate oriented along the active electromagnet. The pendulum indicator was able to render its visual output almost immediately.

Contrarily, the pendulum indicator required a lot of fine tuning of the electromagnet positions relative to the suspended weight, making it significantly more challenging to get working. The compass indicator was very much plug-n-play, and only took a matter of seconds to lay out a working configuration.

Conclusions
-----------
* Electromagnetism is an effective phenomenon for rendering a visual output with very minimal power and current requirements, requiring a very simple apparatus to visualize the changing magnetic fields.
* An electromagnet can be easily and quickly fabricated. Their geometries and electronic characteristics can easily be customized, as well as their core material.
* One ferromagnetic core material that has repeatedly shown to be effective is 416 stainless steel.
* The crude wire-winding jig depicted and described in this report significantly helped facilitate the construction of the electromagnets.
* The solder joints connecting the ends of the this 36 AWG magnet wire to the ends of the extension wire / jumper wire, although reinforced with heat-shrink tubing, seem qualitatively susceptible to fatigue-induced failure, although no such failure was experienced. It would be preferable if these magnets were potted in some type of epoxy resin, including the solder joint with a much more robust lead wire, such that only the thicker wire was exposed and capable of bending.
* Both of these visual indicators leveraged a very small magnetic attractive force. To amplify the effect of this small force, all other counteracting forces needed to be minimized or removed altogether. In the case of the compass indicator, the only force opposing the reorientation of the needle is friction against the water. In the case of the pendulum indicator, only a small component of gravity.
* The small piece of tape placed on the ends of the electromagnet in the for the pendulum electromagnetic visual indicator served a critical purpose. Normally, when these electromagnets are activated and deactivated, they maintain a small residual magnetic field, that is actually strong enough to keep the suspended weight weakly attached to the non-active electromagnet. Because the strength of the electromagnet decreased with the square of the distance away from it, even a small piece of tape is enough thickness to degrade this residual attraction to the degree necessary to allow an activated opposing electromagnet to pull the weight off the deactivated electromagnet.