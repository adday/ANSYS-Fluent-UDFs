#include "udf.h"

//cuboid height = 0.10325
#define fluid_level 0.10325 //filled cuboid
DEFINE_DPM_SCALAR_UPDATE(adjust_particle_pos_vel,cell,thread,initialize,p)
{	
	if (P_POS(p)[1] > fluid_level)
	{
			printf("Particle in air. Position: %g Velocity: %g\n", P_POS(p)[1], P_VEL(p)[1]); fflush(stdout);
			
			// adjust particle position and velocity
			P_POS(p)[1] = fluid_level;
			P_VEL(p)[1] = - 0.5 * P_VEL(p)[1];
			
			printf("Particle adjusted. Position: %g Velocity: %g \n", P_POS(p)[1], P_VEL(p)[1]); fflush(stdout);
	}
	
	if (P_POS(p)[1] > (fluid_level - (fluid_level/10) && P_VEL(p)[1] > 0.01)
	{
		printf("Particle close to air. Position: %g Velocity: %g\n", P_POS(p)[1], P_VEL(p)[1]);	fflush(stdout);
			
			// adjust particle position and velocity
			P_VEL(p)[1] = - 0.5 * P_VEL(p)[1];
			
			printf("Particle adjusted. Position: %g Velocity: %g \n", P_POS(p)[1], P_VEL(p)[1]); fflush(stdout);
	}
	
}
