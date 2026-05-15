function [path,section]=splitPathWithPath(section,path1,path2,referencePoint)

  path=struct('x',[],'y',[],'visibility',true,'color','g','tag',2);

  for i=1:(length(path1.x)-1)
    for j=1:(length(path2.x)-1)
      A=[path1.x(i) path1.y(i)];
      B=[path1.x(i+1) path1.y(i+1)];
      C=[path2.x(j) path2.y(j)];
      D=[path2.x(j+1) path2.y(j+1)];
      E=B-A;
      F=D-C;
      P=[-E(2),E(1)];
      Q=[-F(2),F(1)];
      h=(dot((A-C),P)/dot(F,P));
      g=(dot((C-A),Q)/dot(E,Q));
      if ((h>=0)&&(h<=1)&&(g>=0)&&(g<=1))
        %intersect at C+h*F
        dLeft=0;
        dRight=0;
        for k=1:i
          dLeft=dLeft+sqrt((path1.x(k)-referencePoint.x)^2+(path1.y(k)-referencePoint.y)^2);
        end
        for k=(i+1):(length(path1.x))
          dRight=dRight+sqrt((path1.x(k)-referencePoint.x)^2+(path1.y(k)-referencePoint.y)^2);
        end
        dLeft=dLeft/i;
        dRight=dRight/(length(path1.x)-i);
        if dLeft<dRight
          path.x=[path1.x(1:i) C(1)+h*F(1)];
          path.y=[path1.y(1:i) C(2)+h*F(2)];
        else
          path.x=[C(1)+h*F(1) path1.x(i+1:end)];
          path.y=[C(2)+h*F(2) path1.y(i+1:end)];
        end
        section.paths=[section.paths path];
        return
      end
    end
  end

  error('No intersection detected.');

end
