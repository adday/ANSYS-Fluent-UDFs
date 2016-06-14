#include "udf.h"

/******************************************************************************
UDF to record information on density of cell containing particle.
Tracks the number of particles that have a density below a threshold.
******************************************************************************/


DEFINE_DPM_SCALAR_UPDATE(particle_reflect_on_position,cell,thread,initialize,p)
{	
	if (P_POS(p)[1] > 0.15)
	{
			printf("Particle in air. Position: %g Velocity: %g\n", P_POS(p)[1], P_VEL(p)[1]); fflush(stdout);
			
			// adjust particle position and velocity
			P_POS(p)[1] = 0.15;
			P_VEL(p)[1] = - P_VEL(p)[1];
			
			printf("Particle adjusted. Position: %g Velocity: %g \n", P_POS(p)[1], P_VEL(p)[1]); fflush(stdout);
	}
	
	if (P_POS(p)[1] > 0.12 && P_VEL(p)[1] > 0)
	{
		printf("Particle close to air. Position: %g Velocity: %g\n", P_POS(p)[1], P_VEL(p)[1]);	fflush(stdout);
			
			// adjust particle position and velocity
			P_VEL(p)[1] = - P_VEL(p)[1];
			
			printf("Particle adjusted. Position: %g Velocity: %g \n", P_POS(p)[1], P_VEL(p)[1]); fflush(stdout);
	}
	
}
