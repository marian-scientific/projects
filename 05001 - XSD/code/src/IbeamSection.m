function [section]=IbeamSection(section,cx,cy,w,h,tw,tf)

x0=cx-w/2;
y0=cy-h/2;

P=[x0,y0;
  x0,y0+tf;
  x0+w/2-tw/2,y0+tf;
  x0+w/2-tw/2,y0+h-tf;
  x0,y0+h-tf;
  x0,y0+h;
  x0+w,y0+h;
  x0+w,y0+h-tf;
  x0+w/2+tw/2,y0+h-tf;
  x0+w/2+tw/2,y0+tf;
  x0+w,y0+tf;
  x0+w,y0;
  x0,y0];

[I_beam_path,section]=createPathFromArray(section,P);

[I_beam_face,section]=createFaceFromPaths(section,I_beam_path,'I-beam');

[section]=setFaceProperties(section,I_beam_face,"ABS");

end
