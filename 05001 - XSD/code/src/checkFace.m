function []=checkFace(face)

  if ~isnumeric(face.x)
    error("Face has invalid x positions.");
  elseif ~isnumeric(face.y)
    error("Face has invalid y positions.");
  elseif ~islogical(face.visibility)
    error("Face has invalid visibility.");
  elseif ~isequal(size(face.x),size(face.y))
    error("Mismatch in face x and y dimensions.");
  elseif ~isfield(face,'color');
    error("Face is missing color attribute.");
  elseif (face.tag~=3)
    error("Face has invalid tag.");
  end

end
