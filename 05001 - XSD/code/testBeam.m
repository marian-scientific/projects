addpath('src');	% libs

close all
clear all
clc

section=initSection(0,0,0,0);
section=BeamSection(section,pi,pi/3,6,e);
drawSection(section);

w=6;
h=e;
EA=.2e6*w*h;
section=evaluateInertias(section);
EIxx=.2e6*(w*h^3)/12;
EIyy=.2e6*(w^3*h)/12;
I=(w*h^3)/12;
