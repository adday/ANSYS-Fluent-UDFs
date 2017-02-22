#include "udf.h"

#define num_cells 1000 	// greater than number cells in interface region to be tracked 
#define up_lim 0.15    	// upper limit of interface region 
#define low_lim	0.12	// lower limit of interface region 
#define delta 0.001
#define len(x) (sizeof(x) / sizeof((x)[0]))
#define surface_region_id 13 
#define gas_domain_index  1            

struct Cell { real x, y, z;};  

struct Cell * interface[num_cells];

real interpolate( real x, real y); 
void adjust_particle( Tracked_Particle * p);

DEFINE_ON_DEMAND( init_interface)
{
	int i;
	for(i=0;i<num_cells;i++){
		interface[i] = malloc(sizeof(struct Cell));
		interface[i]->x = -1;
		interface[i]->z = -1;
	}
} 

DEFINE_DPM_SCALAR_UPDATE( adjust_particles, c, t, initialize, p){
	// do nothing on first timestep 
	if(initialize){return;} 
	
	// if particle above the low limit then perform interpolation 
	if(P_POS(p)[1] > low_lim){
		real surf_z = interpolate(P_POS(p)[0],P_POS(p)[2]);
		//if particle above the surface 
		if(P_POS(p)[1] > surf_z){
			P_POS(p)[1] = surf_z - delta;
			adjust_particle(p);}
	}
}

DEFINE_EXECUTE_AT_END( track_interface)
{
	Thread * mixture_thread = Lookup_Thread(domain, surface_region_id);
	Thread * gas_thread = THREAD_SUB_THREAD(mixture_thread,gas_domain_index);
	int pos = 0;
	cell_t c;
	real centroid[ND_ND];
	begin_c_loop(c,mixture_thread){
		C_CENTROID(centroid,c,mixture_thread);
		real vof = C_VOF(c,gas_thread);
		if( (centroid[1] > low_lim) && (centroid[1] < up_lim) && ((vof > 0.3) && (vof < 0.7 )) ){
			interface[pos]->x = centroid[0];
			interface[pos]->y = centroid[1];
			interface[pos]->z = centroid[2];
			pos++;
		}
	}
	end_c_loop(c,mixture_thread)
}

real interpolate( real x, real z)
{
	real numer = 0;
	real denom = 0;
	int i;
	while((interface[i]->x != -1) && (interface[i]->z != -1)) {
		if ((interface[i]->x == x) && (interface[i]->z == z)) {return interface[i]->y;}
		else{
			numer += interface[i]->y / pow((pow((interface[i]->x - x),2) + pow((interface[i]->z - z),2)),4);
			denom += 1 / pow((pow((interface[i]->x - x),2) + pow((interface[i]->z - z),2)),4);
		}
	}
	return numer/denom;
}

void adjust_particle( Tracked_Particle * p)
{
	// compute unit normal vector (a,b,c) using points on surface
	real x1 = P_POS(p)[0] + delta;
	real y1 = interpolate(x1,P_POS(p)[2]);
	real z2 = P_POS(p)[2] + delta;
	real y2 = interpolate(P_POS(p)[0],z2);
	real a = (y1-P_POS(p)[1])*delta;
	real b = (y2-P_POS(p)[1])*delta;
	real c = pow(delta,2);
	real norm = sqrt(pow(a,2)+pow(b,2)+pow(c,2));
	a = a/norm; b = b/norm; c = c/norm;
	
	// obtain velocity (u,v,w) 
	real u = P_VEL(p)[0]; real v =P_VEL(p)[1]; real w = P_VEL(p)[2];
	
	// compute velocity direction
	real vel_x = -(b*(a*v - b*u))/(pow(a,2) + pow(b,2) + pow(c,2)) - (c*(a*w - c*u))/(pow(a,2) +
			pow(b,2) + pow(c,2)) - (a*(a*u + b*v + c*w))/(pow(a,2) + pow(b,2) + pow(c,2));
	real vel_y = ((a*v - b*u)*(pow(a,2) + pow(c,2)))/(pow(a,3) + a*pow(b,2) + a*pow(c,2)) - 
		(b*(a*u + b*v + c*w))/(pow(a,2) + pow(b,2) + pow(c,2)) - (b*c*(a*w - c*u))/(pow(a,3) + a*pow(b,2) + a*pow(c,2));
	real vel_z = ((a*w - c*u)*(pow(a,2) + pow(b,2)))/(pow(a,3) + a*pow(b,2) + a*pow(c,2)) - 
		(c*(a*u + b*v + c*w))/(pow(a,2) + pow(b,2) + pow(c,2)) - (b*c*(a*v - b*u))/(pow(a,3) + a*pow(b,2) + a*pow(c,2));
	
	// compute cosine(theta) = <a,b>
	real cos = (a*u + b*v + c*w);
	
	// scale velocity magnitudes by sin(theta) 
	real sin = sqrt(1-pow(cos,2));
	P_VEL(p)[0] = vel_x * sin;
	P_VEL(p)[1] = vel_y * sin;
	P_VEL(p)[2] = vel_z * sin;
}
