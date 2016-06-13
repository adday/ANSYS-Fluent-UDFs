/******************************************************************************
Implementations of DEFINE_DPM_SPRAY_COLLIDE macro.
Use to override default FLUENT spray collision algorithm.
Called during droplet tracking after every droplet time step and requires 
that collision is enabled in the DPM panel.
Returns: void
******************************************************************************/

#include "udf.h"

/******************************************************************************
UDF that is a non-physical collision and relaxes the particle velocity and 
diameter in a cell to the mean over the specified time scale
******************************************************************************/
#include "dpm.h"
#include "surf.h"
DEFINE_DPM_SPRAY_COLLIDE(man_spray_collide,tp,p)
{
	const real t_relax = 0.001; // seconds 
	
	// get the cell and Thread that the particle is currently in 
	cell_t c = RP_CELL(&(tp->cCell));
	Thread *t = RP_THREAD(&(tp->cCell));
	
	// Particle index for looping over all particles in the cell 
	Particle *pi;
	
	// loop over all particles in the cell to find their 
	//	mass weighted mean velocity and diameter 
	int i;
	real u_mean[3]={0.}, mass_mean=0.;
	real d_orig = tp->state.diam;
	real decay = 1. - exp(-t_relax);
	begin_particle_cell_loop(pi,c,t)
	{
		mass_mean += pi->state.mass;
		for(i=0;i<3;i++)
			u_mean[i] += pi->state.V[i]*pi->state.mass;
	}
	end_particle_cell_loop(pi,c,t)
	
	// relax particle velocity to the mean and diameter to the 
	// initial diameter over the relaxation time scale t_relax 
	if( mass_mean > 0. )
	{
		for(i=0;i<3;i++)
			u_mean[i] /= mass_mean;
		for(i=0;i<3;i++)
			tp->state.V[i] += decay*( u_mean[i] - tp->state.V[i] );
		tp->state.diam += decay*( P_INIT_DIAM(tp) - tp->state.diam );
		// adjust the number in the droplet parcel to conserve mass 
		tp->number_in_parcel *= CUB( d_orig/tp->state.diam );
	}
}