function [section]=setFaceProperties(section,face,material)

  if strcmp(material,"Aluminum")
    face.E=10e6;
    face.G=11.5e3;
    face.v=.33;
  elseif strcmp(material,"ABS")
    face.E=.2e6;
    face.G=127e3;
    face.v=.35;
  else
    return
  end

    for i=1:length(section.faces)
      if (isequal(section.faces(i).x,face.x)&&isequal(section.faces(i).y,face.y))
        section.faces(i).E=face.E;
        section.faces(i).G=face.G;
        section.faces(i).v=face.v;
      end
    end

end
