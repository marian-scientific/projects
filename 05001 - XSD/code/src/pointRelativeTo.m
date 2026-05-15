function [point,section]=pointRelativeTo(section,x,y,referencePoint)

  point=struct('x',x+referencePoint.x,'y',y+referencePoint.y,'visibility',true,'color','k','tag',1);
  section.points=[section.points point];

end
