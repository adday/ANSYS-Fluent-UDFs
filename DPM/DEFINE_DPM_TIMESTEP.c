/******************************************************************************
Implementations of DEFINE_DPM_TIMESTEP macro.
Use to change the time step for DPM particle tracking based on user-specified 
input. 
Returns: real
******************************************************************************/

#include "udf.h"
#include "dpm.h"

/******************************************************************************
UDF for timestep control in DPM. Sets the time step to a
maximum value of 1e−4. If the time step computed by FLUENT is smaller than 
1e−4, then FLUENT’s time step is returned.
******************************************************************************/

DEFINE_DPM_TIMESTEP(limit_to_e_minus_four, p, dt)
{
	if (dt > 1.e-4)
	{
		/* p->next_time_step = 1.e-4; */
		return 1.e-4;
	}
	return dt;
}
