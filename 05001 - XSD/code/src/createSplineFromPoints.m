function [path,section]=createSplineFromPoints(section,points,slopes,N)

  path=struct('x',[],'y',[],'visibility',true,'color','g','tag',2);

  dx=(points(2).x-points(1).x)/(N-1);
  x=points(1).x;
  x_scale=(points(2).x-points(1).x);

  for i=1:N

    t=(x-points(1).x)/x_scale;
    y=(2*t^3-3*t^2+1)*points(1).y+...
    (t^3-2*t^2+t)*x_scale*slopes(1)+...
    (-2*t^3+3*t^2)*points(2).y+...
    (t^3-t^2)*x_scale*slopes(2);



    path.x=[path.x x];
    path.y=[path.y y];

    if length(path.x)>1
    slope=(path.y(end)-path.y(end-1))/(path.x(end)-path.x(end-1))
  end
    x=x+dx;
  end


  %for i=1:length(points)
  %  path.x=[path.x points(i).x];
  %  path.y=[path.y points(i).y];
  %end


   section.paths=[section.paths path];

end
