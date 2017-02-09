/******************************************************************************
UDF for integrating turbulent dissipation and displaying it in the console
******************************************************************************/

#include "udf.h"

DEFINE_ADJUST(int_turb_dissip, d)
{
	Thread *t;
	// vars to integrate dissipation
	real sum_diss=0;
	cell_t c;

	thread_loop_c(t,d) // loop over all threads
	{
		begin_c_loop(c,t) // loop over all cells
			sum_diss += C_D(c,t)*
			C_VOLUME(c,t);
		end_c_loop(c,t)
	}

	printf("Volume integral of tubulent dissipation: %g \n", sum_diss);
}


/******************************************************************************
UDF for defining user-defined scalars and their gradients
******************************************************************************/

DEFINE_ADJUST(def_scalars_gradients,d)
{
	Thread *t;
	cell_t c;
	real K_EL = 1.0;

	//do nothing if gradient is not yet allocated
	if ( ! Data_Valid_P())
		return;

	thread_loop_c(t,d)
	{
	if (FLUID_THREAD_P(t))
		{
		begin_c_loop_all(c,t)
			{
			C_UDSI(c,t,1) += K_EL*NV_MAG2(C_UDSI_G(c,t,0))*C_VOLUME(c,t);
			}
		end_c_loop_all(c,t)
		}
	}
}







}


