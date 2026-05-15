function []=checkPoint(point)

  if ~isnumeric(point.x)
    error("Point has invalid x position.");
  elseif ~isnumeric(point.y)
    error("Point has invalid y position.");
  elseif ~islogical(point.visibility)
    error("Point has invalid visibility.");
  elseif ~isfield(point,'color');
    error("Point is missing color attribute.");
  elseif (point.tag~=1)
    error("Point has invalid tag.");
  end

end
