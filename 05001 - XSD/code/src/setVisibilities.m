function [section]=setVisibilities(section,objects,value)

  for j=1:length(objects)
    if (objects(j).tag==1)
      for i=1:length(section.points)
        if (isequal(section.points(i).x,objects(j).x)&&isequal(section.points(i).y,objects(j).y))
          objects(j).visibility=value;
          section.points(i).visibility=value;
        end
      end
    elseif (objects(j).tag==2)
      for i=1:length(section.paths)
        if (isequal(section.paths(i).x,objects(j).x)&&isequal(section.paths(i).y,objects(j).y))
          objects(j).visibility=value;
          section.paths(i).visibility=value;
        end
      end
    elseif (objects(j).tag==3)
      for i=1:length(section.faces)
        if (isequal(section.faces(i).x,objects(j).x)&&isequal(section.faces(i).y,objects(j).y))
          objects(j).visibility=value;
          section.faces(i).visibility=value;
        end
      end
    end
  end

end