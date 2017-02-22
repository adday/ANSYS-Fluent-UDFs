#include "udf.h"

/******************************************************************************************************************************
Modify particle vertical position and velocity as a function of position and velocity
******************************************************************************************************************************/

#define fluid_level 0.10325 // fluid level in simulation
#define fluid_thresh 0.1 // upper fraction of fluid to consider
#define vel_thresh 0.01 // velocity lower threshold for adjustment
#define vel_adjust -0.5 // scale factor for velocity adjustment, negative for direction flip

DEFINE_DPM_SCALAR_UPDATE( adjust_particle_pos_vel, cell, thread, initialize, p)
{	
	// if particle in gas phase
	if (P_POS(p)[1] > fluid_level)
	{
		printf("Particle in air. Position: %g Velocity: %g\n", P_POS(p)[1], P_VEL(p)[1]); 
		fflush(stdout);
			
		// adjust particle vertical position and velocity
		P_POS(p)[1] = fluid_level;
		P_VEL(p)[1] = vel_adjust * P_VEL(p)[1];
			
		printf("Particle adjusted. Position: %g Velocity: %g \n", P_POS(p)[1], P_VEL(p)[1]); 
		fflush(stdout);
	}
	
	// if particle within fluid_thresh of gas phase and particle velocity greater than threshold
	if ( (P_POS(p)[1] > (fluid_level - (fluid_level * fluid_thresh))) && (P_VEL(p)[1] > vel_thresh) )
	{
		printf("Particle close to air. Position: %g Velocity: %g \n", P_POS(p)[1], P_VEL(p)[1]); 
		fflush(stdout);
			
		// adjust particle vertical position and velocity
		P_VEL(p)[1] = vel_adjust * P_VEL(p)[1];
			
		printf("Particle adjusted. Position: %g Velocity: %g \n", P_POS(p)[1], P_VEL(p)[1]); 
		fflush(stdout);
	}
	
}
