addpath('src');	% libs

close all
clear all
clc

section=initSection(0,0,0,0);

[p1,section]=createPointInSpace(section,5,5);
[p2,section]=createPointInSpace(section,-5,5);
[p3,section]=createPointInSpace(section,-13,-5);
[p4,section]=createPointInSpace(section,13,-5);
[path1,section]=createPathFromPoints(section,[p1,p2,p3,p4,p1]);
[face1,section]=createFaceFromPaths(section,[path1]);
[section]=setVisibilities(section,[p1,p2,p3,p4,path1],false);

drawSection(section);

[nodeArray,elementArray]=meshFace(section,section.faces(1),.4);

plotMesh(nodeArray,elementArray);
