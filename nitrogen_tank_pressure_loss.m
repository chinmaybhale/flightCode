clear all;
clc;
format long;


t_total = 32; % Burn time in seconds 
dt = 0.1; % time step in seconds
t = (0.1:dt:t_total); % time duration in steps of .1 s
p1 = zeros(length(t),1);
rho = zeros(length(t),1);
V2 = zeros(length(t),1);
m_dot = zeros(length(t),1);




p1(1) = 41368543.8; % pressure inside tank at t=0s [Pa]
p2 = 101352.932; % pressure outside tank [Pa]
CV = 13;    % control volume coefficient
T = 293; % Kelvin
T_o = 293;  % Kelvin
R = 8.314; % Gas constant [J/kg*K]
N = 6;

M = 0.028;
Rs = R/M; % Specific gas constant (nitrogen)


Volume = N*0.03905037; % cu. m 
d_e = 0.0254; % diameter of tank port in meters
port_area = pi/4*(d_e)^2; % port area in m^2
Area = port_area; % m^2
density_n2 = 376.09856; % [kg/m^3] @ 6000 psi
density_n2_atm = 1.16643434075; % [kg/m^3] @ 14.7 psi
n = p1(1)*Volume/(R*T);
m_i = n*M;
%m_i = density_n2*Volume; % initial mass of nitrogen
rho(1) = density_n2; % initial density
%n = m_i/M;
%n = (p1(1)*Volume)/(R*T);
%V_line = Volume/n;
%Z = (p1(1)*V_line)/(R*T);

m_old = m_i;
for i = 1:length(t)

    V2(i) = sqrt((p1(i)-p2)/rho(i)*2); % velocity at exit point in streamline [m/s]
    
    m_dot(i) = rho(i)*V2(i)*Area; % mass flow rate at t = 0s [kg/s]
    dm = m_dot(i) * dt;  % change of mass [kg]
    m_new = m_old - dm;    % New mass [kg]
    i = i+1;
    if i < length(t)
    rho(i) = m_new/Volume; 
    n = m_new/M;
    %p_new1 = rho(i)*Rs*T;
    p_new = (n*R*T)/Volume;
    p1(i) = p_new;     % reassigning pressure 1 to new pressure [Pa] 
    end
    m_old = m_new;
end
p11 = p1/6895;


plot(t,p11)


