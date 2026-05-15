function [face,section]=setFaceVisibility(section,face,value)

  for i=1:length(section.faces)
    if (isequal(section.faces(i).x,face.x)&&isequal(section.faces(i).y,face.y))
      face.visibility=value;
      section.faces(i).visibility=value;
    end
  end

end
