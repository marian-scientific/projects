function [path,section]=offsetPath(section,path1,d)

  path=struct('x',zeros(size(path1.x)),'y',zeros(size(path1.y)),'visibility',true,'color','g','tag',2);

  vec0=[-(path1.y(2)-path1.y(1)) (path1.x(2)-path1.x(1))];
  del=d*vec0/sqrt(vec0(1)^2+vec0(2)^2);
  path.x(1)=path1.x(1)+del(1);
  path.y(1)=path1.y(1)+del(2);

  vec0=[-(path1.y(end)-path1.y(end-1)) (path1.x(end)-path1.x(end-1))];
  del=d*vec0/sqrt(vec0(1)^2+vec0(2)^2);
  path.x(end)=path1.x(end)+del(1);
  path.y(end)=path1.y(end)+del(2);

  for i=2:(length(path1.x)-1)
    vecL=[-(path1.y(i)-path1.y(i-1)) (path1.x(i)-path1.x(i-1))];
    delL=d*vecL/sqrt(vecL(1)^2+vecL(2)^2);
    vecR=[-(path1.y(i+1)-path1.y(i)) (path1.x(i+1)-path1.x(i))];
    delR=d*vecR/sqrt(vecR(1)^2+vecR(2)^2);
    A=[path1.x(i-1)+delL(1) path1.y(i-1)+delL(2)];
    B=[path1.x(i)+delL(1) path1.y(i)+delL(2)];
    C=[path1.x(i)+delR(1) path1.y(i)+delR(2)];
    D=[path1.x(i+1)+delR(1) path1.y(i+1)+delR(2)];
    E=B-A;
    F=D-C;
    P=[-E(2),E(1)];
    h=(dot((A-C),P)/dot(F,P));
    %intersect at C+h*F
    path.x(i)=C(1)+h*F(1);
    path.y(i)=C(2)+h*F(2);
  end

  section.paths=[section.paths path];

end
