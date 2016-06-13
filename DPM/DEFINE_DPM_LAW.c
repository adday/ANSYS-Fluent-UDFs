/******************************************************************************
Implementations of DEFINE_DPM_LAW macro.
Used to customize laws for particlesfor mass, diameter, and temperature
properties as the particle exchanges mass and energy with its surroundings.
Executes every iteration and is called at the beginning of every iteration 
before transport equations are solved.
Returns: void
******************************************************************************/

#include "udf.h"

/******************************************************************************
UDF that models a custom law for evaporation swelling of particles
******************************************************************************/

DEFINE_DPM_LAW(evapororation_swelling_law,p,ci)
{
	real swelling_coeff = 1.1;
	// standard evaporation routine to calculate mass and heat transfer 
	VaporizationLaw(p);
	// compute new particle diameter and density
	P_DIAM(p) = P_INIT_DIAM(p) * (1. + (swelling_coeff - 1.) *
				(P_INIT_MASS(p) - P_MASS(p)) /
				(DPM_VOLATILE_FRACTION(p) * P_INIT_MASS(p)));
	P_RHO(p) = P_MASS(p) / (3.14159*P_DIAM(p) * P_DIAM(p) * P_DIAM(p)/6);
	P_RHO(p) = MAX(0.1, MIN(1e5, P_RHO(p)));
}