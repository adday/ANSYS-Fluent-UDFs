/******************************************************************************
Implementations of DEFINE_DPM_DRAG macro.
Used to specify the drag coefficient between particles and fluid defined by 
equation on page 2-165 of FLUENT UDF manual.
Returns: real
******************************************************************************/

#include "udf.h"

/******************************************************************************
UDF for computing particle drag coefficient (18*Cd*Re/24) curve
******************************************************************************/

DEFINE_DPM_DRAG(particle_drag_force, Re, p)
{
	real w, drag_force;
	
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
}