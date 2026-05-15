function []=checkPath(path)

  if ~isnumeric(path.x)
    error("Path has invalid x positions.");
  elseif ~isnumeric(path.y)
    error("Path has invalid y positions.");
  elseif ~islogical(path.visibility)
    error("Path has invalid visibility.");
  elseif ~isequal(size(path.x),size(path.y))
    error("Mismatch in path x and y dimensions.");
  elseif ~isfield(path,'color');
    error("Path is missing color attribute.");
  elseif (path.tag~=2)
    error("Path has invalid tag.");
  end

end
