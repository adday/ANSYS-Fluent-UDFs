/* UDF for computing the inertial lift force on particles */
#include "udf.h"

FILE *fp;
DEFINE_ON_DEMAND(open_file)
{
	fp = fopen("gradients.csv","w+");
	//x-pos and y-pos are the positions of the centroid of the cell containing particle
	fprintf(fp,"i,x-pos,y-pos, ux, uy, vx, vy \n");
	fflush(fp);
}

/* Calculate inertial lift force on a particle. Inertial lift  */
/* force FL=fL*p*V^2*a^4/H^2, the final version of accelaration  */
/* on particle is 6fL*a*V^2/pi/H^2  */
#define a 0.000002   /* particle diameter  */
#define H 0.00001  /* channel height */
#define fL 0.05  /* lift coefficent  */
#define Pi 3.14159265358979323  /* pi  */

DEFINE_DPM_BODY_FORCE(particle_body_force, p, i)

{
	cell_t c;
	Thread *t;
	t = P_CELL_THREAD(p);
	c = P_CELL(p);

	real x[ND_ND];
	C_CENTROID(x,c,t); 
	
	/*derivatives*/
	real du_dx = C_DUDX(c,t);	
	real du_dy = C_DUDY(c,t);	
	real dv_dx = C_DVDX(c,t);	
	real dv_dy = C_DVDY(c,t);
	
	/*gradients*/
	real u_vel_grad_x = C_U_G(c,t)[0];
	real u_vel_grad_y = C_U_G(c,t)[1];

	real v_vel_grad_x = C_V_G(c,t)[0];
	real v_vel_grad_y = C_V_G(c,t)[1];

	fprintf(fp,"%d, %g, %g, %g, %g, %g, %g \n",i, x[0],x[1],du_dx,du_dy,dv_dx,dv_dy);
	fflush(fp);
	fprintf(fp,"%d, %g, %g, %g, %g, %g, %g \n",i, x[0],x[1],u_vel_grad_x,u_vel_grad_y,v_vel_grad_x,v_vel_grad_y);
	fflush(fp);

	real particle_body_force = 0;
	/* an acceleration should be returned */
	return particle_body_force;
}

DEFINE_ON_DEMAND(close_file)
{
	fflush(fp);
	fclose(fp);
}