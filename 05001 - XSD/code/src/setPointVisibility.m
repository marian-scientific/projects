function [point,section]=setPointVisibility(section,point,value)

  for i=1:length(section.points)
    if ((section.points(i).x==point.x)&&(section.points(i).y==point.y))
      point.visibility=value;
      section.points(i).visibility=value;
    end
  end

end