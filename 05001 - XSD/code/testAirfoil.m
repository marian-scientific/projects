function []=testAirfoil()

naca_airfoil=struct('ID','2412','panels',100);
P=naca4(naca_airfoil);
airfoilSurface=createPathFromArray(P);

LE0=createPointInSpace(-.01,0);
LE1=createPointInSpace(.01,0);
LESplitPath=createPathFromPoints([LE0,LE1]);

PointAbove=createPointInSpace(.5,1);
PointBelow=createPointInSpace(.5,-1);
UAF=splitPathWithPath(airfoilSurface,LESplitPath,PointAbove);
LAF=splitPathWithPath(airfoilSurface,LESplitPath,PointBelow);

TE_UAF=createPointOnPath(UAF,0,'ratio');
TE_LAF=createPointOnPath(LAF,1,'ratio');

TE_flat=createPathFromPoints([TE_UAF,TE_LAF]);
TE_middle=createPointOnPath(TE_flat,.5,'ratio');

TE0=createPointInSpace(0.99,0);
TE1=createPointInSpace(1.01,0);
TESplitPath=createPathFromPoints([TE0,TE1]);

TE_split_top=splitPathWithPath(TE_flat,TESplitPath,PointAbove);
TE_split_bot=splitPathWithPath(TE_flat,TESplitPath,PointBelow);

LE=createPointOnPath(UAF,1,'ratio');
chord=createPathFromPoints([LE,TE_middle]);

UAF_face=createFaceFromPaths([UAF,chord,TE_split_top]);
LAF_face=createFaceFromPaths([LAF,TE_split_bot,chord]);

highestPoint=getNearestPoint(UAF,PointAbove)

%setVisibilities([LE0,LE1,LESplitPath,TE0,TE1,TESplitPath,PointAbove,PointBelow,LE,TE_UAF,TE_LAF,TE_middle],false);
setFaceColor(UAF_face,'m');

UAF_skin=offsetPath(UAF,-.01);
LAF_skin=offsetPath(LAF,-.01);


end
