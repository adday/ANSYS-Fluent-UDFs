#include "udf.h"
#define num_cells 1000 /* greater than number cells in interface region to be tracked */
#define up_lim 0.15     /* upper limit of interface region */
#define low_lim	0.12	/* lower limit of interface region */
#define delta 0.001
#define len(x) (sizeof(x) / sizeof((x)[0]))

struct Cell { real x, y, z;};  
struct Cell * interface[num_cells];

real interpolate(real x, real y); 
void adjust_particle(Tracked_Particle * p);

DEFINE_ON_DEMAND(init_interface){
	int i;
	for(i=0;i<num_cells;i++){
		interface[i] = malloc(sizeof(struct Cell));
		interface[i]->x = -1;
		interface[i]->z = -1;}
} 

DEFINE_DPM_SCALAR_UPDATE(adjust_particles,c,t,initialize,p){
	if(initialize){return;} /* do nothing on first timestep */
	/* if particle above the low limit -> perform interpolation */
	if(P_POS(p)[1] > low_lim){
		real surf_y = interpolate(P_POS(p)[0],P_POS(p)[2]);
		/* if particle above the surface */
		if(P_POS(p)[1] > surf_y){
			P_POS(p)[1] = surf_y;
			adjust_particle(p);}
	}
}

DEFINE_EXECUTE_AT_END(track_interface){
	Domain * domain; /* domain is declared as a variable */
	domain = Get_Domain(1);
	int surface_region_id = 13; /* region defined according to Fluent, will change based on simulation */
	Thread * mixture_thread = Lookup_Thread(domain, surface_region_id);
	int phase_domain_index = 1;    /* gas domain */        
	Thread * gas_thread = THREAD_SUB_THREAD(mixture_thread,phase_domain_index);
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
			pos++;}
	}
	end_c_loop(c,t)
}

real interpolate(real x, real z){
	real numer = 0;
	real denom = 0;
	int i;
	for(i=0;i<len(interface);i++)
		/* ensure that valid data is in interface[i] */
		if(interface[i]->x == -1 && interface[i]->z == -1) {break;}
		if ((interface[i]->x == x) && (interface[i]->z == z)) {return interface[i]->y;}
		else{
			numer += interface[i]->y / pow((pow((interface[i]->x - x),2) + pow((interface[i]->z - z),2)),4);
			denom += 1 / pow((pow((interface[i]->x - x),2) + pow((interface[i]->z - z),2)),4);}
	return numer/denom;
}

void adjust_particle(Tracked_Particle * p){
	/* compute unit normal vector (a,b,c) */
	real x1 = P_POS(p)[0] + delta;
	real y1 = interpolate(x1,P_POS(p)[2]);
	real z2 = P_POS(p)[2] + delta;
	real y2 = interpolate(P_POS(p)[0],z2);
	real a = (y1-P_POS(p)[1])*delta;
	real b = (y2-P_POS(p)[1])*delta;
	real c = pow(delta,2);
	real norm = sqrt(pow(a,2)+pow(b,2)+pow(c,2));
	a = a/norm; b = b/norm; c = c/norm;
	/* velocity is (x0,y0,z0) */
	real x0 = P_VEL(p)[0]; real y0 =P_VEL(p)[1]; real z0 = P_VEL(p)[2];
	/* compute cosine factor and direction change */
	real cos = (a*x0 + b*y0 + c*z0) / sqrt(pow(a,2) + pow(b,2) + pow(c,2) + pow(x0,2) + pow(y0,2) + pow(z0,2) );
	/* adjust particle velocities and direction*/
	real vel_x = -(b*(a*y0 - b*x0))/(pow(a,2) + pow(b,2) + pow(c,2)) - (c*(a*z0 - c*x0))/(pow(a,2) +
			pow(b,2) + pow(c,2)) - (a*(a*x0 + b*y0 + c*z0))/(pow(a,2) + pow(b,2) + pow(c,2));
	real vel_y = ((a*y0 - b*x0)*(pow(a,2) + pow(c,2)))/(pow(a,3) + a*pow(b,2) + a*pow(c,2)) - 
		(b*(a*x0 + b*y0 + c*z0))/(pow(a,2) + pow(b,2) + pow(c,2)) - (b*c*(a*z0 - c*x0))/(pow(a,3) + a*pow(b,2) + a*pow(c,2));
	real vel_z = ((a*z0 - c*x0)*(pow(a,2) + pow(b,2)))/(pow(a,3) + a*pow(b,2) + a*pow(c,2)) - 
		(c*(a*x0 + b*y0 + c*z0))/(pow(a,2) + pow(b,2) + pow(c,2)) - (b*c*(a*y0 - b*x0))/(pow(a,3) + a*pow(b,2) + a*pow(c,2));
	/* scale velocity magnitudes by sin(v_dir < N) */
	real sin = sqrt(1-pow(cos,2));
	P_VEL(p)[0] = vel_x * sin;
	P_VEL(p)[1] = vel_y * sin;
	P_VEL(p)[2] = vel_z * sin;
}
