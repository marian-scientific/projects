function [L]=getPathLength(path)

  D=diff([path.x' path.y']);
  L=sum(sqrt(sum(D.^2,2)));

end