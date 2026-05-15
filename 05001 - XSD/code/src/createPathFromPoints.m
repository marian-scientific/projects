function [path,section]=createPathFromPoints(section,points)

  path=struct('x',[],'y',[],'visibility',true,'color','g','tag',2);

  for i=1:length(points)
    path.x=[path.x points(i).x];
    path.y=[path.y points(i).y];
  end

  section.paths=[section.paths path];

end
