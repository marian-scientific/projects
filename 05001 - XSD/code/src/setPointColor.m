function [point,section]=setPointColor(section,point,value)

  for i=1:length(section.points)
    if ((section.points(i).x==point.x)&&(section.points(i).y==point.y))
      point.color=value;
      section.points(i).color=value;
    end
  end

end