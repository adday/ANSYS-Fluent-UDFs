#include "udf.h"

#define drag_coefficient = 2.0

FILE *fp;
DEFINE_INIT(open_file,domain)
{
	fp = fopen("paddle_forces.csv","w+");

}

DEFINE_ADJUST(paddle_force, domain)
{
	int rotateID = 16;
	int i = 0;
	Thread *t = Lookup_Thread(domain, rotateID);
	face_t f;
	real area,velocity,force;
	real total_area = 0;
	
	begin_f_loop(f,t)
	{
		if(PRINCIPAL_FACE_P(f,t)){
			i+=1;
			F_AREA(area,f,t);   /* computes area of each face  */
			total_area += NV_MAG(area);  /* computes total face area by accumulating
											magnitude of the each face's area  */
			velocity = F_V(f,t);
			force = area * pow(velocity,2.0) * drag_coefficient;
			fprintf(fp,"%d, %g \n",i,force);
		}		
	}
	end_f_loop(f,t)
	
}

DEFINE_EXECUTE_AT_EXIT(close_file)
{
	fflush(fp);
	fclose(fp);
}