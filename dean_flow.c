/* UDF for computing the inertial lift force on particles */
#include "udf.h"

/* Calculate inertial lift force on a particle. Inertial lift  */
/* force FL=fL*p*V^2*a^4/H^2, the final version of accelaration  */
/* on particle is 6fL*a*V^2/pi/H^2  */
#define a 0.000002   /* particle diameter  */
#define H 0.00001  /* channel height */
#define fL 0.05 /* lift coefficent  */
#define Pi 3.14159265358979323  /* pi  */

#define y_0  -6.5e-5 //for new mesh
#define num_curves 13 //13 for new mesh
#define delta_x_curve  8e-5 //for new mesh

#define lf_magnitude 50000  
/* #define df_magnitude 478800  */



DEFINE_DPM_BODY_FORCE(particle_body_force, p, i)

{
	real df_magnitude=7*lf_magnitude;
	real particle_body_force = 0;
	real x_pos = P_POS(p)[0];
	real y_pos = P_POS(p)[1];

	cell_t c;
	Thread *t;
	t = P_CELL_THREAD(p);
	c = P_CELL(p);
	
	real ux=C_U(c,t);
	real uy=C_V(c,t);
	real ma_gradient_x=pow((ux*ux+uy*uy),-0.5)*(ux*C_U_G(c, t)[0]+uy*C_V_G(c, t)[0]);
	real ma_gradient_y=pow((ux*ux+uy*uy),-0.5)*(ux*C_U_G(c, t)[1]+uy*C_V_G(c, t)[1]);
	real lf_direction_0=ma_gradient_x/pow((ma_gradient_x*ma_gradient_x+ma_gradient_y*ma_gradient_y),0.5);
	real lf_direction_1=ma_gradient_y/pow((ma_gradient_x*ma_gradient_x+ma_gradient_y*ma_gradient_y),0.5);
	
	if (y_pos >= y_0){ //particle in curve -> apply lift and dean force
		real y_pos_corrected = y_0 - y_pos;
		/*new mesh values {1.444e-4,4.844e-4,8.244e-4,1.164e-3,1.504e-3,1.844e-3,2.184e-3,2.524e-3,2.864e-3,3.204e-3,3.544e-3,3.884e-3,4.224e-3};  */
		real x_offset[] = {1.444e-4,4.844e-4,8.244e-4,1.164e-3,1.504e-3,1.844e-3,2.184e-3,2.524e-3,2.864e-3,3.204e-3,3.544e-3,3.884e-3,4.224e-3};
		real x_pos_corrected = 0;
		int j;	
		for(j=num_curves-1;j>=0;j--){
			if(x_pos>x_offset[j]){
				x_pos_corrected = (x_offset[j]+delta_x_curve/2) - x_pos;
				break;
			}
		}
		real df_direction_0 = x_pos_corrected / pow(x_pos_corrected*x_pos_corrected+y_pos_corrected*y_pos_corrected,0.5);
		real df_direction_1 = y_pos_corrected / pow(x_pos_corrected*x_pos_corrected+y_pos_corrected*y_pos_corrected,0.5);

		if (i == 0) particle_body_force = (lf_magnitude*lf_direction_0) + (df_magnitude*df_direction_0);
		else if (i == 1)   particle_body_force = (lf_magnitude*lf_direction_1) + (df_magnitude*df_direction_1);
	}
	else{ // apply only lift force*/
		if (i == 0) particle_body_force = lf_magnitude*lf_direction_0;
		else if (i == 1)   particle_body_force = lf_magnitude*lf_direction_1;
	}
	/* an acceleration should be returned */
	return (particle_body_force);
}

