function [nodeArray,elementArray]=meshFace(section,face,d)

% init boundary mesh
x_bounds=[]; % can eventually init this to a proper size for speed
y_bounds=[]; % can to eventually init this to a proper size for speed

% "equally" space nodes around the boundary about every 'd' units
for i=1:(length(face.x)-1)
	L=sqrt((face.x(i+1)-face.x(i))^2+(face.y(i+1)-face.y(i))^2);
	n_els=2*round(L/(d*2)); % ensure even # elements per side, an extra strict req
	x_bounds=[x_bounds linspace(face.x(i),face.x(i+1),n_els+1)];
	y_bounds=[y_bounds linspace(face.y(i),face.y(i+1),n_els+1)];
  x_bounds(end)=[];
  y_bounds(end)=[];
end

  % put back on if we need duplicate first element
  %x_bounds(end+1)=x_bounds(1);
  %y_bounds(end+1)=y_bounds(1);

  s=[pi/4,pi/4,pi/4,pi/4,pi/4,pi/4]

  paving_boundary=[x_bounds' y_bounds']; % init paving boundary
  % loop a bunch
  while size(paving_boundary,1)>0
    % classify each node on the boundary:
    for i=2:(size(paving_boundary,1)-1) % really needs to start at 1, add something above this
      interiorAngle(paving_boundary(i-1,1),paving_boundary(i-1,2),paving_boundary(i,1),paving_boundary(i,2),paving_boundary(i+1,1),paving_boundary(i+1,2))
    end
    paving_boundary=[];
  end



  % test plot of the boundary nodes
  figure(1)
  plot(x_bounds,y_bounds,'ok')

  % sample output for 3 square elements
  nodeArray=[0 0;.5 0;1 0;1.5 0;1.5 .5;1 .5;.5 .5;0 .5];
  elementArray=[1 2 7 8;2 3 6 7;3 4 5 6];

end
