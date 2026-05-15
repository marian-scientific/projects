function []=plotMesh(nodeArray,elementArray)

figure
hold on
axis equal

plot(nodeArray(:,1),nodeArray(:,2),'or')

for i=1:size(elementArray,1)
  x_temp=[nodeArray(elementArray(i,[1 2 3 4 1]),1)];
  y_temp=[nodeArray(elementArray(i,[1 2 3 4 1]),2)];
  plot(x_temp,y_temp,'-g');
end

end
