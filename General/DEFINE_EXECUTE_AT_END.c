/******************************************************************************
Implementations of DEFINE_EXECUTE_AT_END macro.
Executed at the end of an iteration in a steady state run, or at the end of a
time step in a transient run based on selection of steady or unsteady time method.
Use to calculate flow quantites at these particular times.
Returns: void
******************************************************************************/

#include "udf.h"


/******************************************************************************
UDF for integrating turbulent dissipation and displaying it in the console
at the end of the current iteration or time step
******************************************************************************/

DEFINE_EXECUTE_AT_END(execute_at_end)
{
	Domain *d;
	Thread *t;
	// variables to integrate dissipation
	real sum_diss=0;
	cell_t c;
	d = Get_Domain(1); //mixture domain if multiphase
	
	thread_loop_c(t,d) // loop over all threads
	{
	 if(FLUID_THREAD_P(t))
		{
		begin_c_loop(c,t) // loop over all cells
			sum_diss += C_D(c,t)*C_VOLUME(c,t);
		end_c_loop(c,t)
		}
	}

	printf("Volume integral of tubulent dissipation: %g \n", sum_diss);
	fflush(stdout);	
}