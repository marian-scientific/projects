function [point,section]=intersectPathWithPath(section,path1,path2,referencePoint)

  point=struct('x',[],'y',[],'visibility',true,'color','k','tag',1);

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
        point.x=C(1)+h*F(1);
        point.y=C(2)+h*F(2);
        section.points=[section.points point];
        return
      end
    end
  end

  error('No intersection detected.');

end
