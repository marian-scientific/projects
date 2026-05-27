addpath('src');	% libs

close all
clear all
clc

section=initSection(0,0,0,0);
section=IbeamSection(section,pi,pi/3,6,e,.35,.25);
drawSection(section);

section=evaluateInertias(section);
bf=6;
h=e;
tw=.35;
tf=.25;
EI=.2e6*((bf*h^3)/12-((bf-tw)*(h-2*tf)^3)/12)
