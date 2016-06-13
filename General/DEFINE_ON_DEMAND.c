/******************************************************************************
Implementations of DEFINE_ON_DEMAND macro.
Used to specify a UDF executed "on demand" in FLUENT.
Executed immediately after it is activated but is not accessible
while the solver is iterating.
Returns: void
******************************************************************************/

#include "udf.h"

/******************************************************************************
UDF to calculate temperature field function and store in user-defined memory.
Also prints min, max, and average temperatures.
******************************************************************************/

DEFINE_ON_DEMAND(on_demand_calc)
{
	Domain *d ; // declare domain ptr as it is not passed as arg
	real tavg = 0;
	real tmax = 0;
	real tmin = 0;
	real temp, volume, vol_total;
	Thread *t;
	cell_t c;
	d = Get_Domain(1); // get domain using FLUENT utility
	
	// loop over all cell threads in domain
	thread_loop_c(t,d)
	{
		// compute max, min, volume-averaged temperature
		// loop over all cells
		begin_c_loop(c,t)
		{
			volume = C_VOLUME(c,t);	 // get cell volume
			temp = C_T(c,t);	// get cell temperature
		
			if (temp < tmin || tmin == 0.0) tmin = temp;
			if (temp > tmax || tmax == 0.0) tmax = temp;
		
			vol_total += volume;
			tavg += temp*volume;
		}
		end_c_loop(c,t)
		
		tavg /= vol_total;
		
		printf("\nTmin = %g	Tmax = %g	Tavg = %g\n", tmin, tmax, tavg);
		
		// compute temperature function and store in user-defined memory
		// location index 0
		begin_c_loop(c,t)
		{
			temp = C_T(c,t);
			C_UDMI(c,t,0) = (temp - tmin) / (tmax - tmin);
		}
		end_c_loop(c,t)
	}
}


