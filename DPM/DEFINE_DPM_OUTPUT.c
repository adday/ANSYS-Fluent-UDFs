/******************************************************************************
Implementations of DEFINE_DPM_OUTPUT macro.
Use to modify what is written to sampling device output.
Allows access to the variables that are written as a particle passes through
a sampler.
Returns: void
******************************************************************************/

#include "udf.h"

/******************************************************************************
UDF that samples discrete phase size and velocity distributions within domain
******************************************************************************/

#define REMOVE_PARTICLES FALSE

DEFINE_DPM_OUTPUT(discrete_phase_sample_output,header,fp,p,t,plane)
{
	#if RP_2D
		real flow_time = solver_par.flow_time;
		real y;
		
		if(header)
			par_fprintf_head(fp," #Time[s] R [m] X-velocity[m/s]
				W-velocity[m/s] R-velocity[m/s] Drop Diameter[m]
				Number of Drops Temperature [K] Initial Diam [m]
				Injection Time [s] \n");

		if(NULLP(p))
			return;
		
		if (rp_axi && (sg_swirl || rp_ke))
			y = MAX(sqrt(SQR(p->state.pos[1]) + SQR(p->state.pos[2])),DPM_SMALL);
		else
			y = p->state.pos[1];

	#if PARALLEL
		par_fprintf(fp,"%d %d %e %f %f %f %f %e %e %f %e %f \n",
			p->injection->try_id,p->part_id, P_TIME(p),y,p->state.V[0],
			p->state.V[1],p->state.V[2],P_DIAM(p),p->number_in_parcel,
			P_T(p), P_INIT_DIAM(p),p->time_of_birth);
	#else
		par_fprintf(fp,"%e %f %f %f %f %e %e %f %e %f \n",
			P_TIME(p), y,p->state.V[0],p->state.V[1],p->state.V[2],P_DIAM(p),
			p->number_in_parcel, P_T(p), P_INIT_DIAM(p), p->time_of_birth);
	
	#endif // PARALLEL

	#else // SERIAL
		real flow_time = solver_par.flow_time;
		real r, x, y;
		
		if(header)
			par_fprintf_head(fp," #Time[s] R [m] x-velocity[m/s] y-velocity[m/s] z-velocity[m/s] Drop Diameter[m] Number of Drops Temperature [K] Initial Diam [m] Injection Time [s] \n");

		if(NULLP(p))
			return;

	x = p->state.pos[0];
	y = p->state.pos[1];
	r = sqrt(SQR(x) + SQR(y));
	
	#if PARALLEL
		par_fprintf(fp,"%d %d %e %f %f %f %f %e %e %f %e %f \n",
			p->injection->try_id, p->part_id, P_TIME(p), r,p->state.V[0],
			p->state.V[1],p->state.V[2],P_DIAM(p),p->number_in_parcel,
			P_T(p), P_INIT_DIAM(p), p->time_of_birth);
	
	#else // SERIAL
		par_fprintf(fp,"%e %f %f %f %f %e %e %f %e %f \n",
			P_TIME(p), r,p->state.V[0],p->state.V[1],p->state.V[2],
			P_DIAM(p),p->number_in_parcel,P_T(p), P_INIT_DIAM(p),
			p->time_of_birth);
			
	#endif // PARALLEL 
	
	#endif //RP_2D
	
	#if REMOVE_PARCELS
		p->stream_index=-1;
	#endif // REMOVE_PARCELS
}
