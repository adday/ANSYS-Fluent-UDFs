/******************************************************************************
Implementations of DEFINE_INIT macro.
Use to specify a set of initial values for solution.
Accomplishes the same result as patching but with a UDF.
Executed once per initialization and called after default
initialization is performed by solver.
As called after flow field is initialized it is typically
used to set initial values of flow quantities.
Returns: void
******************************************************************************/

#include "udf.h"

/******************************************************************************
UDF for initializing flow field variables
******************************************************************************/

DEFINE_INIT(init_func,d)
{
	cell_t c;
	Thread *t;
	real xc[ND_ND];
	
	// loop over all cell threads in domain
	thread_loop_c(t,d)
	{
		//loop over all cells
		begin_c_loop_all(c,t)
			{
			C_CENTROID(xc,c,t);
			if (sqrt(ND_SUM(pow(xc[0]-0.5,2.0),
							pow(xc[1]-0.5,2.0),
							pow(xc[2]-0.5,2.0))) < 0.25)
				C_T(c,t) = 400;
			else
				C_T(c,t) = 300;
			}
		end_c_loop_all(c,t)
	}
}