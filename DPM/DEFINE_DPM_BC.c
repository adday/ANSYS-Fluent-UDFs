/******************************************************************************
Implementations of DEFINE_DPM_BC macro.
Used to specify custom boundary conditions for particles.
Executes every time a particle touches a boundary of the domain,
except for symmetric or periodic boundaries. 
Can define a separate UDF for each boundary.
Returns: int
******************************************************************************/

#include "udf.h"

/******************************************************************************
UDF for relect boundary condition for inert particles
******************************************************************************/

DEFINE_DPM_BC(bc_reflect,p,t,f,f_normal,dim)
{
	real alpha; //angle of particle path with face normal
	real vn = 0;
	real nor_coeff = 1.0;
	real tan_coeff = 0.3;
	real normal[3];
	int i, idim = dim;
	real NV_VEC(x);
	
	#if RP_2D // dim is always 2 in 2D compilation so need special treatment
			  // for 2d axisymmetric and swirl flows
		if (rp_axi_swirl)
		{
			real R = sqrt(P_POS(p)[1] * P_POS(p)[1] + P_POS(p)[2] * P_POS(p)[2]);
			
			if (R > 1.0e-20)
			{
				idim = 3;
				normal[0] = f_normal[0];
				normal[1] = (f_normal[1] * P_POS(p)[1])/R;
				normal[2] = (f_normal[2] * P_POS(p)[2])/R; 
			}
			else
			{
				for(i=0;i<idim;i++)
					normal[i] = f_normal[i];
			}
		}
		else
	#endif
			for(i=0;i<idim;i++)
				normal[i] = f_normal[i];
				
		if(p->type == DPM_TYPE_INERT)
		{
			alpha = M_PI/2.0 - acos(MAX(-1.0,MIN(1.0, NV_DOT(normal, P_VEL(p)) /
										MAX(NV_MAG(P_VEL(p)), DPM_SMALL))));
			if ((NNULLP(t)) && (THREAD_TYPE(t) == THREAD_F_WALL))
				F_CENTROID(x,f,t);
				
			// calculate teh normal component, rescale it's magnitude by the
			// coefficient of restitution and subtract the change
			
			// compute normal velocity
			for(i=0;i<idim;i++)
				vn += P_VEL(p)[i] * normal[i];
				
			// subtract off normal velocity
			for(i=0;i<idim;i++)
				P_VEL(p)[i] -= vn * normal[i];
				
			// apply tangential coefficient of restitution
			for(i=0;i<idim;i++)
				P_VEL(p)[i] *= tan_coeff;
				
			// add reflected normal velocity
			for(i=0;i<idim;i++)
				P_VEL(p)[i] -= nor_coeff * vn * normal[i];
			
			// store new velocity in P_VELO of particle
			for(i=0;i<idim;i++)
				P_VEL0(p)[i] = P_VEL(p)[i];
				
			return PATH_ACTIVE;
		}
		
	return PATH_ABORT;
}