/******************************************************************************
Implementations of DEFINE_DPM_BODY_FORCE macro.
Used to specify a body force on particles other than a gravitational/drag force.
Returns: real
******************************************************************************/

#include "udf.h"

#define Q 1.0  // particle electric charge
#define BZ 3.0 //z component of magnetic field
#define TSTART 18.0 //field applied at t = TSTART


/******************************************************************************
UDF to calculate magnetic force on charged particle.Magnetic force is charge 
times particle velocity cross product magnetic field:Fx=q*bz*Vy,Fy=-q*bz*Vx
******************************************************************************/

DEFINE_DPM_BODY_FORCE(particle_magnetic_body_force, p, i)
{
	real bforce = 0;
	if (P_TIME(p) >= TSTART)
	{
		if (i == 0) bforce = Q * BZ * P_VEL(p)[1];
		else if (i == 1) bforce = -Q * BZ * P_VEL(p)[0];
	}
	else 
		bforce = 0.0;
	// an acceleration should be returned
	return (bforce / P_MASS(p));
}


/******************************************************************************
UDF to calculate inertial lift force on a particle. Inertial lift 
force FL=fL*p*V^2*a^4/H^2, the final version of accelaration
on particle is 6fL*a*V^2/pi/H^2
******************************************************************************/

DEFINE_DPM_BODY_FORCE(particle_inertial_body_force, p, i)

{
#define a 0.000002   /* particle diameter  */
#define H 0.00001  /* channel height */
#define fL 0.05  /* lift coefficent  */
#define Pi 3.14159265358979323  /* pi  */
	cell_t c;
	Thread *t;
	//face_t f;
	t = P_CELL_THREAD(p);
	c = P_CELL(p);
	

		real paticle_body_force = 0;

		if (i == 1) paticle_body_force = 6000 * fL*a*pow(C_V_G(c, t)[1], 2) / Pi / pow(H, 2);

		/* an acceleration should be returned */
		return (paticle_body_force);
	
}