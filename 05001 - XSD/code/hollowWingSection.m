function [section]=hollowWingSection(section,chord,skinThickness,sparPos1,sparPos2,sparPos3)

[sparPos1]=sparPos1*chord-chord/4;
[sparPos2]=sparPos2*chord-chord/4;
[sparPos3]=sparPos3*chord-chord/4;

naca_airfoil=struct('ID','4428','panels',100);
P=naca4(naca_airfoil);

P=P*chord;
P(:,1)=P(:,1)-chord/4;

##%%%
##[P0,section]=createPointInSpace(section,0,1);
##[P1,section]=createPointInSpace(section,1,1.1);
##[P2,section]=createPointInSpace(section,4,.5);
##[P3,section]=createPointInSpace(section,5,2.5);
##
##[L0,section]=createPathFromPoints(section,[P0,P1]);
##[L1,section]=createPathFromPoints(section,[P2,P3]);
##m0=getSlopeFromPoints(P0,P1)
##m1=getSlopeFromPoints(P2,P3)
##
##[S1,section]=createSplineFromPoints(section,[P1,P2],[m0,m1],200);
##
##drawSection(section);
##%%%

[airfoilSurface,section]=createPathFromArray(section,P);

[skin_IML,section]=offsetPath(section,airfoilSurface,skinThickness);

[TE_UAF,section]=createPointOnPath(section,airfoilSurface,0,'ratio');
[TE_LAF,section]=createPointOnPath(section,airfoilSurface,1,'ratio');
[TE_Flat,section]=createPathFromPoints(section,[TE_UAF,TE_LAF]);

[TE_Middle,section]=createPointOnPath(section,TE_Flat,.5,'ratio');
[TE_Flat_upper,section]=createPathFromPoints(section,[TE_UAF,TE_Middle]);
[TE_Flat_lower,section]=createPathFromPoints(section,[TE_LAF,TE_Middle]);

[TE_Fwd_Extension,section]=pointRelativeTo(section,-.05*chord,0,TE_Middle);
[TE_Midplane,section]=createPathFromPoints(section,[TE_Middle,TE_Fwd_Extension]);

[PointAbove,section]=createPointInSpace(section,.5*chord-chord/4,.2*chord);
[PointBelow,section]=createPointInSpace(section,.5*chord-chord/4,-.2*chord);
[PointAhead,section]=createPointInSpace(section,.5-chord/4,0);
[PointLessAhead,section]=createPointInSpace(section,-.5-chord/4,0);
[LE_Midplane,section]=createPathFromPoints(section,[PointAhead,PointLessAhead]);

[UAF_skin_IML,section]=splitPathWithPath(section,skin_IML,LE_Midplane,PointAbove);
[LAF_skin_IML,section]=splitPathWithPath(section,skin_IML,LE_Midplane,PointBelow);

[UAF_skin_IML2,section]=splitPathWithPath(section,UAF_skin_IML,TE_Midplane,PointAbove);
[LAF_skin_IML2,section]=splitPathWithPath(section,LAF_skin_IML,TE_Midplane,PointBelow);

[UAF_IML_TE,section]=getNearestPoint(section,UAF_skin_IML2,TE_Middle);
[LAF_IML_TE,section]=getNearestPoint(section,LAF_skin_IML2,TE_Middle);
[UAF_TE_Midplane,section]=createPathFromPoints(section,[UAF_IML_TE,TE_Middle]);
[LAF_TE_Midplane,section]=createPathFromPoints(section,[LAF_IML_TE,TE_Middle]);

[section]=setVisibilities(section,[skin_IML,UAF_skin_IML,LAF_skin_IML,TE_UAF,TE_LAF,TE_Middle,TE_Flat,TE_Fwd_Extension,TE_Midplane,UAF_IML_TE,LAF_IML_TE,LE_Midplane,PointAbove,PointBelow,PointLessAhead,PointAhead],false);

[spar1_center_above,section]=createPointInSpace(section,sparPos1,.5*chord);
[spar1_center_below,section]=createPointInSpace(section,sparPos1,-.5*chord);
[spar1_centerline,section]=createPathFromPoints(section,[spar1_center_above,spar1_center_below]);
[spar1_offset1,section]=offsetPath(section,spar1_centerline,skinThickness/2);
[spar1_offset2,section]=offsetPath(section,spar1_centerline,-skinThickness/2);

[spar1_upper_intersect1,section]=intersectPathWithPath(section,spar1_offset1,UAF_skin_IML2,PointAbove);
[spar1_upper_intersect2,section]=intersectPathWithPath(section,spar1_offset2,UAF_skin_IML2,PointAbove);
[spar1_upper,section]=createPathFromPoints(section,[spar1_upper_intersect1,spar1_upper_intersect2]);

[spar1_lower_intersect1,section]=intersectPathWithPath(section,spar1_offset1,LAF_skin_IML2,PointBelow);
[spar1_lower_intersect2,section]=intersectPathWithPath(section,spar1_offset2,LAF_skin_IML2,PointBelow);
[spar1_lower,section]=createPathFromPoints(section,[spar1_lower_intersect1,spar1_lower_intersect2]);
[spar1_side1,section]=createPathFromPoints(section,[spar1_upper_intersect1,spar1_lower_intersect1]);
[spar1_side2,section]=createPathFromPoints(section,[spar1_upper_intersect2,spar1_lower_intersect2]);

[section]=setVisibilities(section,[spar1_center_above,spar1_center_below,spar1_centerline,spar1_offset1,spar1_offset2,spar1_upper_intersect1,spar1_upper_intersect2,spar1_lower_intersect1,spar1_lower_intersect2],false);

[spar2_center_above,section]=createPointInSpace(section,sparPos2,.5*chord);
[spar2_center_below,section]=createPointInSpace(section,sparPos2,-.5*chord);
[spar2_centerline,section]=createPathFromPoints(section,[spar2_center_above,spar2_center_below]);
[spar2_offset1,section]=offsetPath(section,spar2_centerline,skinThickness/2);
[spar2_offset2,section]=offsetPath(section,spar2_centerline,-skinThickness/2);

[spar2_upper_intersect1,section]=intersectPathWithPath(section,spar2_offset1,UAF_skin_IML2,PointAbove);
[spar2_upper_intersect2,section]=intersectPathWithPath(section,spar2_offset2,UAF_skin_IML2,PointAbove);
[spar2_upper,section]=createPathFromPoints(section,[spar2_upper_intersect1,spar2_upper_intersect2]);
[spar2_lower_intersect1,section]=intersectPathWithPath(section,spar2_offset1,LAF_skin_IML2,PointBelow);
[spar2_lower_intersect2,section]=intersectPathWithPath(section,spar2_offset2,LAF_skin_IML2,PointBelow);
[spar2_lower,section]=createPathFromPoints(section,[spar2_lower_intersect1,spar2_lower_intersect2]);
[spar2_side1,section]=createPathFromPoints(section,[spar2_upper_intersect1,spar2_lower_intersect1]);
[spar2_side2,section]=createPathFromPoints(section,[spar2_upper_intersect2,spar2_lower_intersect2]);

[section]=setVisibilities(section,[spar2_center_above,spar2_center_below,spar2_centerline,spar2_offset1,spar2_offset2,spar2_upper_intersect1,spar2_upper_intersect2,spar2_lower_intersect1,spar2_lower_intersect2],false);

[spar3_center_above,section]=createPointInSpace(section,sparPos3,.5*chord);
[spar3_center_below,section]=createPointInSpace(section,sparPos3,-.5*chord);
[spar3_centerline,section]=createPathFromPoints(section,[spar3_center_above,spar3_center_below]);
[spar3_offset1,section]=offsetPath(section,spar3_centerline,skinThickness/2);
[spar3_offset2,section]=offsetPath(section,spar3_centerline,-skinThickness/2);

[spar3_upper_intersect1,section]=intersectPathWithPath(section,spar3_offset1,UAF_skin_IML2,PointAbove);
[spar3_upper_intersect2,section]=intersectPathWithPath(section,spar3_offset2,UAF_skin_IML2,PointAbove);
[spar3_upper,section]=createPathFromPoints(section,[spar3_upper_intersect1,spar3_upper_intersect2]);
[spar3_lower_intersect1,section]=intersectPathWithPath(section,spar3_offset1,LAF_skin_IML2,PointBelow);
[spar3_lower_intersect2,section]=intersectPathWithPath(section,spar3_offset2,LAF_skin_IML2,PointBelow);
[spar3_lower,section]=createPathFromPoints(section,[spar3_lower_intersect1,spar3_lower_intersect2]);
[spar3_side1,section]=createPathFromPoints(section,[spar3_upper_intersect1,spar3_lower_intersect1]);
[spar3_side2,section]=createPathFromPoints(section,[spar3_upper_intersect2,spar3_lower_intersect2]);

[section]=setVisibilities(section,[spar3_center_above,spar3_center_below,spar3_centerline,spar3_offset1,spar3_offset2,spar3_upper_intersect1,spar3_upper_intersect2,spar3_lower_intersect1,spar3_lower_intersect2],false);

[skin_face,section]=createFaceFromPaths(section,[airfoilSurface,TE_Flat_lower,LAF_TE_Midplane,LAF_skin_IML2,UAF_skin_IML2,UAF_TE_Midplane,TE_Flat_upper],'Skin Wrap');
[spar1_face,section]=createFaceFromPaths(section,[spar1_upper,spar1_side2,spar1_lower,spar1_side1],'Shear Web 1');
[spar2_face,section]=createFaceFromPaths(section,[spar2_upper,spar2_side2,spar2_lower,spar2_side1],'Shear Web 2');
[spar3_face,section]=createFaceFromPaths(section,[spar3_upper,spar3_side2,spar3_lower,spar3_side1],'Shear Web 3');

[section]=setFaceProperties(section,skin_face,"ABS");
[section]=setFaceProperties(section,spar1_face,"Aluminum");
[section]=setFaceProperties(section,spar2_face,"Aluminum");
[section]=setFaceProperties(section,spar3_face,"Aluminum");

end
