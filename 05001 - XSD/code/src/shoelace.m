function [A,cgx,cgy,Ixx,Iyy,Ixy]=shoelace(face)

  N=length(face.x)

  A=0;
  cgx=0;
  cgy=0;
  Ixx=0;
  Iyy=0;
  Ixy=0;

  for i=1:N
    xi=face.x(i);
    yi=face.y(i);
    xi1=face.x(mod(i,N)+1);
    yi1=face.y(mod(i,N)+1);

    Q=(xi*yi1-xi1*yi);

    A=A+Q;
    cgx=cgx+Q*(xi+xi1);
    cgy=cgy+Q*(yi+yi1);
    Ixx=Ixx+Q*(yi^2+yi*yi1+yi1^2);
    Iyy=Iyy+Q*(xi^2+xi*xi1+xi1^2);
    Ixy=Ixy+Q*(xi*yi1+2*xi*yi+2*xi1*yi1+xi1*yi);
  end

  A=A/2;
  cgx=cgx/(6*A);
  cgy=cgy/(6*A);
  Ixx=Ixx/12;
  Iyy=Iyy/12;
  Ixy=Ixy/24;
  A=abs(A);

end
