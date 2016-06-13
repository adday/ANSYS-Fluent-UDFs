/******************************************************************************
Implementation of DEFINE_EXECUTE_FROM_GUI macro.
Exectued from GUI when an event triggered such as button click.
Returns: void
******************************************************************************/

#include "udf.h"

/******************************************************************************
UDF called from a user-defined GUI dialog box to reset all user-defined
memory (udm) locations.
******************************************************************************/

DEFINE_EXECUTE_FROM_GUI(reset_udm, myudflib, mode)
{
	Domain *d = Get_Domain(1); // get domain pointer
	Thread *t;
	cell_t c;
	int i;
	
	// return if mode is not zero
	if (mode != 0) return;
	
	// return if no udm is defined in FLUENT
	if (n_udm == 0) return;
	
	// loop over all cell threads in domain
	thread_loop_c(t,d)
		{
		// loop over all cells
		begin_c_loop(c,t)
			{
			for(i=0;i<n_udm;i++)
				C_UDMI(c,t,i) = 0.0;
			}
		end_c_loop(c,t);
		}
}