function [] = evaluateInertias(section)

  EAs=0;
  EAs_cgx=0;
  EAs_cgy=0;

  for i = 1:size(section.faces,2)
    % values about coordinate origin
    [A,cgx,cgy,IxxO,IyyO,IxyO]=shoelace(section.faces(i));

    % shift each region to its own centroid
    IxxG=IxxO-A*cgy^2;
    IyyG=IyyO-A*cgx^2;
    IxyG=IxyO-A*cgx*cgy;

    EAs=EAs+section.faces(i).E*A;
    EAs_cgx=EAs_cgx+section.faces(i).E*A*cgx;
    EAs_cgy=EAs_cgy+section.faces(i).E*A*cgy;

    end

    % composite EA-weighted centroid
    xG=EAs_cgx/EAs;
    yG=EAs_cgy/EAs;

    for i = 1:size(section.faces,2)

    % todo, shift each one back to composite centroid and then sum up
    % you will need to track all these things in the same struct, so probably add that in first


    end

 end
