function [section] = evaluateInertias(section)

  % evaluates composite stiffness properties of a section's faces
  % returns section EA, EA-weighted centroid (tension center) ...
  %     and EIxx, EIyy, and EIzz about tension center
  %     under section.props

  As=0;
  EAs=0;
  EAs_cgx=0;
  EAs_cgy=0;

  for i = 1:size(section.faces,2)

    % values about coordinate origin
    [A,cgx,cgy,IxxO,IyyO,IxyO]=shoelace(section.faces(i));

    section.faces(i).A=A;
    section.faces(i).cgx=cgx;
    section.faces(i).cgy=cgy;
    section.faces(i).IxxO=IxxO;
    section.faces(i).IyyO=IyyO;
    section.faces(i).IxyO=IxyO;

    % shift each region to its own centroid
    IxxG=IxxO-A*cgy^2;
    IyyG=IyyO-A*cgx^2;
    IxyG=IxyO-A*cgx*cgy;

    section.faces(i).IxxG=IxxG;
    section.faces(i).IyyG=IyyG;
    section.faces(i).IxyG=IxyG;

    As=As+A;
    EAs=EAs+section.faces(i).E*A;
    EAs_cgx=EAs_cgx+section.faces(i).E*A*cgx;
    EAs_cgy=EAs_cgy+section.faces(i).E*A*cgy;

  end

    % composite EA-weighted centroid (tension center)
    xG=EAs_cgx/EAs;
    yG=EAs_cgy/EAs;

    EIxxPs=0;
    EIyyPs=0;
    EIxyPs=0;

  for i = 1:size(section.faces,2)

    % compute about tension center but do not save the intermediate values,
    %    as they are unnecessary and probably confusing

    IxxP=IxxO+A*(cgy-yG)^2;
    IyyP=IyyO+A*(cgx-xG)^2;
    IxyP=IxyO+A*(cgx-xG)*(cgy-yG);

    EIxxPs=EIxxPs+IxxP*section.faces(i).E;
    EIyyPs=EIyyPs+IyyP*section.faces(i).E;
    EIxyPs=EIxyPs+IxyP*section.faces(i).E;

  end

  section.props.EIxx_TC=EIxxPs;
  section.props.EIyy_TC=EIyyPs;
  section.props.EIxy_TC=EIxyPs;
  section.props.EA=EAs;
  section.props.A=As;
  %%% put something here (and above) to track sectional weight
  section.props.TC=[xG,yG];

 end
