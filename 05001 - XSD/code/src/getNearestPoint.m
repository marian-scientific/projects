function [point,section]=getNearestPoint(section,path,referencePoint)

  point=struct('x',[],'y',[],'visibility',true,'color','k','tag',1);
  d=Inf;

  for i=1:length(path.x)
    dist=sqrt((path.x(i)-referencePoint.x)^2+(path.y(i)-referencePoint.y)^2);
    if dist<d
      point.x=path.x(i);
      point.y=path.y(i);
      d=dist;
    end
  end

  section.points=[section.points point];

end
