#include "udf.h"

#define num_cells 10000 /* somewhat less than total number of cells as we will only track in a region */
#define up_lim         /* upper limit of interface region */
#define low_lim			/* lower limit of interface region */
#define delta 0.001
#define len(x) (sizeof(x) / sizeof((x)[0]))


/* structure definitions */
struct Cell
{
   real x, y, z;
   real vof;
};  

/* global variables */
struct Cell * interface[num_cells];

/* function declarations */
real interpolate(real x, real y);


FILE *fp;

DEFINE_ON_DEMAND(open_file)
{
	fp = fopen("interface.csv","w+");
}

DEFINE_ON_DEMAND(close_file)
{
	fflush(fp);
	fclose(fp);
}

DEFINE_ON_DEMAND(print_interface)
{
	int i;
	for(i=0;i<num_cells;i++){
		fprintf(fp," %g , %g , %g , %g \n", interface[i]->centroid_x,interface[i]->centroid_y,interface[i]->centroid_z, interface[i]->vof);fflush(fp);
	}
} 

DEFINE_ON_DEMAND(init_interface)
{
	int i;
	for(i=0;i<num_cells;i++){
		interface[i] = malloc(sizeof(struct Cell));
	}
} 

DEFINE_EXECUTE_AT_END(track_interface)
{
	Domain * domain; /* domain is declared as a variable */
	domain = Get_Domain(2); /* returns fluid domain pointer */
	
	int interior_id = 1;
	Thread * mixture_thread = Lookup_Thread(domain, interior_id);

	int phase_domain_index = 1;    /* gas domain */        
	Thread * gas_thread = THREAD_SUB_THREAD(mixture_thread,phase_domain_index);
	
	int pos = 0;
	cell_t c;
	real centroid[ND_ND];
	begin_c_loop(c,mixture_thread)
	{
		C_CENTROID(centroid,c,mixture_thread)
		if(centroid[1] > low_lim && centroid[1] < up_lim && (abs(C_VOF(c,gas_thread) - 0.5) < 0.05) ) /* if in interface region and 0.45<VOF<0.55 */
			{
			interface[pos]->centroid_x = centroid[0];
			interface[pos]->centroid_y = centroid[1];
			interface[pos]->centroid_z = centroid[2];
			interface[pos]->vof = C_VOF(c,gas_thread);
			pos++;
			}
	}
	end_c_loop(c,t)
}

real interpolate(real x, real z)
{
	real numer = 0;
	real denom = 0;
	int i;
	for(i=0;i<len(interface);i++):
		if ((interface[i]->x == x) && (interface[i]->z == z))
			return interface[i]->y;
		else:
			numer += interface[i]->y / pow((pow((interface[i]->x - x),2) + pow((interface[i]->z - z),2)),4);
			denom += 1 / pow((pow((interface[i]->x - x),2) + pow((interface[i]->z - z),2)),4);
	return numer/denom;
}


void adjust_particle(Tracked_Particle * p)
{
	real x,y,z;
	x = P_POS(p)[0]; y = P_POS(p)[1]; z = P_POS(p)[2]; 
	real surf_y = interpolate(x,z);
	if(y > surf_y)
		P_POS[1] = surf_y - delta; /* move particle to surface */
		/* now compute surface normal vector */
	

}

real[] surf_normal(x,y,z)
{
	real x1 = x + delta;
	real z2 = z + delta;
	real y1 = interpolate(x1,z);
	real y2 = interpolate(x,z2);
	int [] v1 = {abs(x1-x),abs(y1-y),abs(z-z)}
}
