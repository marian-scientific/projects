function [face,section]=setFaceColor(section,face,value)

  for i=1:length(section.faces)
    if (isequal(section.faces(i).x,face.x)&&isequal(section.faces(i).y,face.y))
      face.color=value;
      section.faces(i).color=value;
    end
  end

end
