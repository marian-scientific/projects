# 03002 - Progress Tracker

### February 19, 2025

* [RFP](https://github.com/marian-scientific/proposals/tree/Christ/RFP001%20-%20HOHMO) announced.
* [Proposal](https://github.com/marian-scientific/proposals/tree/Christ/RFP001%20-%20HOHMO/submitted%20proposals/LEVI) written and submitted.

### February 22, 2025

* [Contract](https://github.com/marian-scientific/proposals/blob/Christ/RFP001%20-%20HOHMO/submitted%20proposals/EVALUATION_CRITERIA.md) awarded.
* 2x 1"-long 3/8"-diameter stainless steel (416) cylinders cut and measured (used for Concepts A & B)
![Rods](resources/rods.jpg)
* Very-basic coil-winding-rig cobbled together (used for Concepts A, B & C)
![Coil Winding Rig](resources/coil-winding-rig.jpg)
* First (of at least two) 1000-turn electromagnet wound with 36 AWG wire, with enamel removed & leads soldered. Resistance measured at roughly 50 ohms.
![Coil](resources/coil.jpg)
* Tested electromagnet @ 3.3V, 200 ohms supplemental circuit resistance, so around 13mA, quantitatively much stronger than the 300-turn 30 AWG version tested in investigation [13001](https://github.com/marian-scientific/reports/13001%20-%20GPIO%20Electromagnet).

### February 23, 2025

* (This) reporting structure created as a template for all future projects.

### February 24, 2025

* Cup found to serve as container for DIY compass.
* Steel wire in inventory to use for needle for DIY compass.
* OpenSCAD code to generate ring to be 3D-printed for floating needle support for DIY compass:

```
$fn=100;
OD=28;
ID=20;
thickness=5;

difference(){
    cylinder(h=thickness,d=OD,center=true);
    cylinder(h=2*thickness,d=ID,center=true);
    };
```

### February 26, 2025

* Brainstorming done for required materials for Concept B
* OpenSCAD code to generate basin to be 3D-printed for DIY compass in respect of the new [MP07](https://github.com/marian-scientific/wiki/wiki/MP07-%E2%80%90-Vertical-Integration):

```
$fn=100;
OD=32;
wall_thickness=1;
base_thickness=5;
height=15;

difference(){
    cylinder(h=height,d=OD);
    translate([0,0,base_thickness]){cylinder(h=2*height,d=OD-2*wall_thickness);}
    };
```

### February 27, 2025

* printed first set of preliminary compass models
![Compass](resources/compass.jpg)

### February 28, 2025

```
$fn=100;

difference(){
    cube([30,10,10],center=true);
    translate([0,0,10]) {rotate([0,90,0]) {cylinder(h=50,d=22,center=true);}};
    translate([10,0,0]){cylinder(h=50,d=1,center=true);}
    translate([-10,0,0]){cylinder(h=50,d=1,center=true);}
    };
```

### March 1, 2025

* collected and cut eco-friendly materials for Concept A structural frame
![Sticks](resources/sticks.jpg)
![Stick Diameter](resources/stick_diameter.jpg)

* carve and assemble preliminary stick frame for concept A
![Sticks](resources/stick_frame.jpg)

* printed second set of compass ring and chassis models
![Updated Print](resources/updated_print.jpg)

* preliminary stick frame test with electromagnet
![Stick Frame Test](resources/stick_frame_test.jpg)

### March 2, 2025

* wrapped another 1000-turn electromagnet
* drill chassis for electromagnet mounting
![Drill Chassis](resources/drill_chassis.jpg)

* glue up frame for Concept A
![Glued Frame](resources/glued_frame.jpg)