#include "udf.h"

/******************************************************************************
 UDFs for modifying particle drag force as function of volume fraction
 of water in cell by altering velocities.  
******************************************************************************/

DEFINE_DPM_SCALAR_UPDATE(modify_drag_by_velocity,cell,thread,initialize,p)
{
	cell_t c = P_CELL(p);
	Thread *t = P_CELL_THREAD(p);
	
	//obtain volume of fraction of liquid
	real alpha_q = C_LIQF(c,t);
	
	// scale velocities
	P_VEL(p)[0] *= alpha_q;
	P_VEL(p)[1] *= alpha_q;
	P_VEL(p)[2] *= alpha_q;

	C_U(c,t) *= alpha_q;
	C_V(c,t) *= alpha_q;
	C_W(c,t) *= alpha_q;	
}


/******************************************************************************
UDF for modifying particle drag coefficient
******************************************************************************/

DEFINE_DPM_DRAG(modify_drag_coeff, Re, p)
{
	real w, drag_force, alpha_q;
	
	if (Re < 0.01)
	{
		drag_force = 18.0;
		return drag_force;
	}
	else if (Re < 20.0)
	{
		w = log10(Re);
		drag_force = 18.0 + 2.367 * pow(Re, 0.82 - 0.05 * w);
		return drag_force;
	}
	else
	{
		drag_force = 18.0 + 3.483 * pow(Re, 0.6305);
		return drag_force;
	}
	
	cell_t c = P_CELL(p);
	Thread *t = P_CELL_THREAD(p);
	
	alpha_q = C_LIQF(c,t);
	
	return drag_force * alpha_q;
}