/******************************************************************************
Implementations of DEFINE_DPM_SCALAR_UPDATE macro.
Use to update scalar quantities every time a particle position is updated.
Allows particle-related variables to be updated or integrated over the life
of the particle.
Executed at the start of particle integration (initialize = 1) and then after 
each time step for particle trajectory integration.
Returns: void
******************************************************************************/

#include "udf.h"

/******************************************************************************
UDF for initializing flow field variables
******************************************************************************/
static real viscosity_0;

DEFINE_DPM_SCALAR_UPDATE(melting_index,cell,thread,initialize,p)
{
	cphase_state_t *c = &(p->cphase);
	if (initialize)
	{
		p->user[0] = 0.0; // initialize melting index to 0
		viscosity_0 = c->mu; // initialize viscosity at start time
	}
	else
	{
		// use a trapezoidal rule to integrate the melting index 
		p->user[0] += P_DT(p) * .5 * (1/viscosity_0 + 1/c->mu);
		viscosity_0 = c->mu; // save current fluid viscosity for start of next step
	}
}