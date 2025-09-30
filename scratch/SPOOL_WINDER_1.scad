include <BOSL-master/constants.scad>
use <BOSL-master/involute_gears.scad>
use <BOSL-master/shapes.scad>

spool_axis_y=0;
spool_axis_z=80;

shaft_OD=6.5;
pinion_thickness=8;
gear_thickness=7;
mount_face_diameter=1.25*25.4;
nut_depth=6;
nut_diameter=10;

drive_axis_y=64+8+10;
drive_axis_z=80;

indicator_axis_y=144+20;
indicator_axis_z=80;

$fn=500;

// pinion and spool mount
translate([-pinion_thickness,spool_axis_y,spool_axis_z]) rotate([0,-90,0])
    difference(){
        union(){
            translate([0,0,0]) gear(mm_per_tooth=5, number_of_teeth=10, thickness=2*pinion_thickness, hole_diameter=0);
            translate([0,0,-1.5*pinion_thickness]) cylinder(h=pinion_thickness/2,r=mount_face_diameter/2);
        }
        translate([0,0,-2*pinion_thickness]) cylinder(h=4*pinion_thickness,r=shaft_OD/2);
    }

// drive gear
translate([0,drive_axis_y,drive_axis_z]) rotate([0,90,0])
    difference(){
        union(){
            translate([0,0,-pinion_thickness/2]) gear(mm_per_tooth=5, number_of_teeth=80, thickness=gear_thickness, hole_diameter=0);
            translate([0,0,-pinion_thickness-3]) gear(mm_per_tooth=5, number_of_teeth=10, thickness=pinion_thickness, hole_diameter=0);
        }
        translate([0,0,-2*pinion_thickness]) cylinder(h=3*pinion_thickness,r=shaft_OD/2);
        translate([0,50,-100]) cylinder(h=100,r=6.5/2);
    }
    
// indicator gear
translate([-1,indicator_axis_y,indicator_axis_z]) rotate([0,90,0])
    difference(){
        union(){
            translate([0,0,-pinion_thickness-3]) gear(mm_per_tooth=5, number_of_teeth=80, thickness=gear_thickness, hole_diameter=0);
            translate([0,0,-pinion_thickness]) cylinder(r=2+shaft_OD/2,h=7);
        }
        translate([0,0,-2*pinion_thickness]) cylinder(h=3*pinion_thickness,r=shaft_OD/2);
    }