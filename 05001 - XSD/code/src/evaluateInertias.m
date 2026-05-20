function [section] = evaluateInertias(section)

  % evaluates composite stiffness properties of a section's faces
  % returns section EA, EA-weighted centroid (tension center) ...
  %     and EIxx, EIyy, and EIzz about tension center
  %     under section.props

  As=0;
  mpl=0;
  EAs=0;
  EAs_cgx=0; % EA-weighted centroid, aka tension center
  EAs_cgy=0;
  m_cgx=0; % mass-weighted centroid, aka CG
  m_cgy=0;

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
    mpl=mpl+section.faces(i).rho*A;
    EAs=EAs+section.faces(i).E*A;

    EAs_cgx=EAs_cgx+section.faces(i).E*A*cgx;
    EAs_cgy=EAs_cgy+section.faces(i).E*A*cgy;

    m_cgx=m_cgx+section.faces(i).rho*A*cgx;
    m_cgy=m_cgy+section.faces(i).rho*A*cgy;

  end

    % composite EA-weighted centroid (tension center)
    xG=EAs_cgx/EAs;
    yG=EAs_cgy/EAs;

    xCG=m_cgx/mpl;
    yCG=m_cgy/mpl;

    EIxxPs=0;
    EIyyPs=0;
    EIxyPs=0;

    EIxxGs=0;
    EIyyGs=0;
    EIxyGs=0;

##    rhoIxxPs=0;
##    rhoIyyPs=0;
##    rhoIxyPs=0;

  for i = 1:size(section.faces,2)

    % compute about tension center and CG but do not save the intermediate values,
    %    as they are unnecessary and probably confusing

    A=section.faces(i).A;
    cgx=section.faces(i).cgx;
    cgx=section.faces(i).cgx;

    Ixx0=section.faces(i).IxxO;
    iyy0=section.faces(i).IyyO;
    Ixy0=section.faces(i).IxyO;

    IxxP=IxxO+A*(cgy-yG)^2;
    IyyP=IyyO+A*(cgx-xG)^2;
    IxyP=IxyO+A*(cgx-xG)*(cgy-yG);

    IxxG=IxxO+A*(cgy-yCG)^2;
    IyyG=IyyO+A*(cgx-xCG)^2;
    IxyG=IxyO+A*(cgx-xCG)*(cgy-yCG);

    EIxxPs=EIxxPs+IxxP*section.faces(i).E;
    EIyyPs=EIyyPs+IyyP*section.faces(i).E;
    EIxyPs=EIxyPs+IxyP*section.faces(i).E;

    EIxxGs=EIxxGs+IxxG*section.faces(i).E;
    EIyyGs=EIyyGs+IyyG*section.faces(i).E;
    EIxyGs=EIxyGs+IxyG*section.faces(i).E;

##    rhoIxxPs=rhoIxxPs+IxxG*section.faces(i).rho; % computing mass moment of inertia
##    rhoIyyPs=rhoIyyPs+IyyG*section.faces(i).rho;
##    rhoIxyPs=rhoIxyPs+IxyG*section.faces(i).rho;

  end

  section.props.EA=EAs;
  section.props.A=As;
  section.props.m=mpl;
  section.props.tension_center=[xG,yG];
  section.props.mass_center=[xCG,yCG];

  section.props.about_tension_center.EIxx=EIxxPs;
  section.props.about_tension_center.EIyy=EIyyPs;
  section.props.about_tension_center.EIxy=EIxyPs;

  section.props.about_mass_center.EIxx=EIxxGs;
  section.props.about_mass_center.EIyy=EIyyGs;
  section.props.about_mass_center.EIxy=EIxyGs;


 end
