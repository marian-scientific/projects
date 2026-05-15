function [pt]=XYOnPath(path,dL)
L0=0;
for i=1:(length(path.x)-1)
  dL=sqrt((path.x(i+1)-path.x(i))^2+(path.y(i+1)-path.y(i))^2);
  if (L0+dL)>d
    pt=(d-L0)/dL*[(path.x(i+1)-path.x(i)) (path.y(i+1)-path.y(i))]+[path.x(i) path.y(i)];
    break
  end
  L0=L0+dL;
end

end