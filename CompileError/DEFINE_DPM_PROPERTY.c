/******************************************************************************
Implementations of DEFINE_DPM_PROPERTY macro.
Use to specify properties of discrete phase materials such as 
particle emissivity, vapor pressure, particle scattering factor,
particle viscosity, and particle surface tension.  
Returns: real
******************************************************************************/

#include "udf.h"

/******************************************************************************
UDF that specifies discrete phase materials
******************************************************************************/

DEFINE_DPM_PROPERTY(coal_emissivity,c,t,p)
{
	real mp0 = P_INIT_MASS(p);
	real mp = P_MASS(p);
	real vf, cf;
	
	vf = DPM_VOLATILE_FRACTION(p); 
	
	cf = DPM_CHAR_FRACTION(p);
	
	if (!(((mp/mp0) >= 1) || ((mp/mp0) <= 0)))
	{
		if ((mp/mp0) < (1-(vf)-(cf)))
		{
			/* only ash left */
			/* vf = cf = 0; */
			return .001;
		}
		else if ((mp/mp0) < (1-(vf)))
		{
			/* only ash and char left */
			/* cf = 1 - (1-(vf)-(cf))/(mp/mp0); */
			/* vf = 0; */
			return 1.0;
		}
		else
		{
			/* volatiles, char, and ash left */
			/* cf = (cf)/(mp/mp0); */
			/* vf = 1. - (1.-(vf))/(mp/mp0); */
			return 1.0;
		}
	}
	return 1.0;
}