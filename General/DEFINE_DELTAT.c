/******************************************************************************
Implementations of DEFINE_DELTAT macro.
Used to control the size of the time step during the solution of a transient
problem. 
NOTE: Used only if "Adaptive" time step is selected.
Returns: real
******************************************************************************/

#include "udf.h"


/******************************************************************************
UDF that changes the time step value for a time-dependent solution
******************************************************************************/

DEFINE_DELTAT(change_time_step, d)
{
	real time_step;
	real flow_time = CURRENT_TIME;
	if(flow_time < 0.5)
		time_step = 0.1;
	else
		time_step = 0.2;
	return time_step;
}