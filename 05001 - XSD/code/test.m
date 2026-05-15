addpath('src');	% libs

close all
clear all
clc

section=initSection(0,0,0,0);
section=hollowWingSection(section,12,.01*12,.2,.4,.65);
drawSection(section);
% meshFace(section,section.faces(2),.01);

evaluateInertias(section);
