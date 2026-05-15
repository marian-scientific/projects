function [path,section]=createPathFromArray(section,P)

  path=struct('x',[],'y',[],'visibility',true,'color','g','tag',2);

  for i=1:size(P,1)
    path.x=[path.x P(i,1)];
    path.y=[path.y P(i,2)];
  end

  section.paths=[section.paths path];

end