/******************************************************************************
Implementations of DEFINE_DPM_INJECTION_INIT macro.
Used to initialize particle's injection properties such as location, diameter,
and velocity.
Returns: void
******************************************************************************/

#include "udf.h"
#include "surf.h" // RP_CELL and RP_THREAD are defined in surf.h 

/******************************************************************************
UDF that initializes particles on a surface injection due to a surface reaction
******************************************************************************/

// arrhenius constants
#define PRE_EXP 1e+15
#define ACTIVE 1e+08
#define BETA 0.0

#define HF 0 // species numbers, match order in FLUENT
#define HF_EXP 2.0 // reaction exponent

real arrhenius_rate(real temp);
real reaction_rate(cell_t c, Thread *cthread,real mw[],real yi[]);
real contact_area(cell_t c, Thread *t, int s_id, int *n);

DEFINE_DPM_INJECTION_INIT(init_surf_inj,I)
{
	int count,i;
	real area, mw[MAX_SPE_EQNS], yi[MAX_SPE_EQNS];
	// MAX_SPE_EQNS is a FLUENT constant in materials.h 
	Particle *p;
	cell_t cell;
	Thread *cthread;
	Material *mix, *sp;
	
	Message("Initializing Injection: %s\n",I->name);
	
	loop(p,I->p) // loop over particle streams in an Injection 
	{
		cell = P_CELL(p); // get current particle cell
		cthread = P_CELL_THREAD(p); // get current particle thread
		// set up molecular weight & mass fraction arrays 
		mix = THREAD_MATERIAL(cthread);
		mixture_species_loop(mix,sp,i)
		{
			mw[i] = MATERIAL_PROP(sp,PROP_mwi);
			yi[i] = C_YI(cell,cthread,i);
		}
		
		//get total area of REACTING_SURFACE faces in contact with cell
		area = contact_area(cell, cthread, REACTING_SURFACE_ID,&count);

		// count is the number of contacting faces
		if (count > 0) // cell is in contact with REACTING_SURFACE 
		{
		//get correct total flow rate when multiple faces contact the same cell
		P_FLOW_RATE(p) = (area * MW_H2 * STOIC_H2 
							* reaction_rate(cell, cthread, mw, yi))
							/ (real) count; 
				
		P_DIAM(p) = 1e-3;
		P_RHO(p) = 1.0;
		P_MASS(p) = P_RHO(p)*M_PI*pow(P_DIAM(p),3.0)/6.0;
		}
		else
			P_FLOW_RATE(p) = 0.0;
		}
	} // end particle loop

// arrhenius_rate function
real arrhenius_rate(real temp)
{
	return PRE_EXP * pow(temp,BETA) * exp(-ACTIVE/(UNIVERSAL_GAS_CONSTANT * temp));
}

// contact area function
real contact_area(cell_t c, Thread *t, int s_id, int *n)
{
	int i = 0;
	real area = 0.0, A[ND_ND];
	*n = 0;
	c_face_loop(c,t,i)
	{
		if(THREAD_ID(C_FACE_THREAD(c,t,i)) == s_id)
		{
			(*n)++;
			F_AREA(A,C_FACE(c,t,i), C_FACE_THREAD(c,t,i));
			area += NV_MAG(A);
		}
	}
	return area;
}

// reaction rate function
real reaction_rate(cell_t c, Thread *cthread,real mw[],real yi[])
{
	real concenHF = C_R(c,cthread)*yi[HF]/mw[HF];
	return arrhenius_rate(C_T(c,cthread))*pow(concenHF,HF_EXP);
}