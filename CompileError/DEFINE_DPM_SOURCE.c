/******************************************************************************
Implementations of DEFINE_DPM_SOURCE macro.
Use to specify particle source terms. 
Allows access to the accumulated source terms for a particle in a given cell 
before they are added to the mass, momentum, and energy exchange terms 
for coupled DPM calculations.
Returns: void
******************************************************************************/

#include "udf.h"
#include "dpm.h"

/******************************************************************************
UDF that specifies discrete phase materials
******************************************************************************/
#define C_DPMS_ENERGY(c,t)C_STORAGE_R(c,t,SV_DPMS_ENERGY)

static real dpm_relax = 1.0;

DEFINE_DPM_SOURCE(dpm_source,c,t,S,strength,p)
{
	real mp_dot;
	Material *sp = P_MATERIAL(p);
	/* 	mp_dot is the (positive) mass source to the continuous phase
		Difference in mass between entry and exit from cell) 
		multiplied by strength (Number of particles/s in stream) */
	mp_dot = (P_MASS0(p) - P_MASS(p)) * strength;
	C_DPMS_YI(c,t,0) += mp_dot*dpm_relax;
	C_DPMS_ENERGY(c,t) -= mp_dot*dpm_relax*
	MATERIAL_PROP(sp,PROP_Cp)*(C_T(c,t)-298.15);
	C_DPMS_ENERGY(c,t) -= mp_dot*dpm_relax*
	MATERIAL_PROP(sp,PROP_latent_heat);
}
