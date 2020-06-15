#include "../include/headers.h"

int seq()
{
	/**
	 * this function ...
	 * 
	 * Args:
	 * 	None
	 * 
	 * Returns:
	 * 	int: 0 
	 * 
	**/

	return 0
}

void set_val(float val, struct sensor *s)
{
	/**
	 * This function sets the value of a pressure or temperature sensor object, based on suryas matlab code 
	 * nitrogen_tank_pressure_loss.m
	 * 
	 * Args:
	 * 	val (float): The value you want to set the sensor as
	 * 	s (sensor *): The sensor whose value you want to set
	 * 
	 * Returns:
	 * 	void
	 * 
	**/
	//Variable declaration, these will be made into globals later (maybe)

	int t_total = 32;//(t_total), total burn time in seconds
	double dt = .1;//Time step in seconds
	double t[320];//time duration array
  //fills time duration array
	for(double i = .1; i<=(double)(t_total+.1); i = i + .1)
		{
			t[(int)(i*10) - 1] = i;
		}
	double p1[320];
	double rho[320];
	double V2[320];
	double m_dot[320];
	p1[0] = 41368543.8; // Pressure in the tank @ t = 0
	double p2 = 101352.932; // Pressure outside the tank
	int  CV = 13; // control volume coefficient
	int T = 293; // Kelvin
	int T_o = 293;  // Kelvin
	double R = 8.314; // Gas constant [J/kg*K]
	int N = 6;
	double M = 0.028;
	double Rs = 296.9286; // = R / M
	double Volume = .2343;// = N * 0.03905037
	double d_e = 0.0254; // diameter of tank port in meters
	double port_area = .0005067074791;// = pi/4*(d_e)^2; % port area in m^2
	double Area = port_area; // m^2
	double density_n2 = 376.09856; //[kg/m^3] @ 6000 psi
	double density_n2_atm = 1.16643434075; //[kg/m^3] @ 14.7 psi
	double n = 3978.954718;//p1(1)*Volume/(R*T);
	double m_i = 111.4107; // = n*M;
	rho[0] = density_n2; // initial density
	double m_old = m_i;
	double dm;
	double m_new;
	double p_new;

	for (int i = 0; i < 320;)
	{
		V2[i] = sqrt(((p1[i]-p2)/rho[i]*2));// velocity at exit point in streamline [m/s]
		
		m_dot[i] = rho[i]*V2[i]*Area; // mass flow rate at t = 0s [kg/s]
		dm = m_dot[i] * dt;// change of mass [kg]
		m_new = m_old - dm;// New mass [kg]
		i = i+1;
		if(i != 319)
		{
		rho[i] = m_new / Volume; 
		n = m_new / M;
		p_new = (n*R*T) / Volume;
		p1[i] = p_new;// reassigning pressure 1 to new pressure [Pa]
		}
		m_old = m_new;
	}

	double p11[320];

	//next loop can possibly be simplified by adding it to the first for loop
	for(int i = 0; i< 320; i++)
	{
		p11[i] = p1[i]/6895;
	}

	return;
}
