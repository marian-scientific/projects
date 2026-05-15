function [angle]=interiorAngle(x1,y1,x2,y2,x3,y3)

  angle=atan2(y1-y2,x1-x2)-atan2(y3-y2,x3-x2);

end
