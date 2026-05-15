function [P]=naca4(naca4_airfoil)

  % number of panels describes number of linear segments on each surface

	m=str2num(naca4_airfoil.ID(1))/100;
	p=str2num(naca4_airfoil.ID(2))/10;
	t=str2num(naca4_airfoil.ID(3:4))/100;

	x=(1-cos(linspace(0,pi,naca4_airfoil.panels+1)))/2;

	yt=5*t*(.2969*sqrt(x)-.1260*x-.3516*x.^2+.2843*x.^3-.1015*x.^4);

	xFwd=x(find(x<=p));
	xAft=x(find(x>p));

	if p==0
		xUAF=x;
		xLAF=x;
		yUAF=yt;
		yLAF=-yt;
	else
		ycFwd=(m/p^2)*(2*p*xFwd-xFwd.^2);
		ycAft=(m/(1-p)^2)*((1-2*p)+2*p*xAft-xAft.^2);
		yc=[ycFwd ycAft];

		dycFwd=(2*m/p^2)*(p-xFwd);
		dycAft=(2*m/(1-p)^2)*(p-xAft);
		dyc=[dycFwd dycAft];
		o=atan(dyc);

		xUAF=x-yt.*sin(o);
		yUAF=yc+yt.*cos(o);
		xLAF=x+yt.*sin(o);
		yLAF=yc-yt.*cos(o);
	end

	xFinal=[fliplr(xUAF) xLAF(2:end)];
	yFinal=[fliplr(yUAF) yLAF(2:end)];

	P=[xFinal;yFinal]';

end
