function []=drawSection(section)

  figure
  hold on

  for i=1:length(section.points)
    if section.points(i).visibility
      plot(section.points(i).x,section.points(i).y,strcat(section.points(i).color,'*'));
    end
  end

  for i=1:length(section.paths)
    if section.paths(i).visibility
      plot(section.paths(i).x,section.paths(i).y,section.paths(i).color);
    end
  end

  for i=1:length(section.faces)
    if section.faces(i).visibility
      fill(section.faces(i).x,section.faces(i).y,section.faces(i).color);
    end
  end

  % maybe plot the origin as well

  axis equal

end
