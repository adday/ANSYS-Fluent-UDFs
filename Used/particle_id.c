#define N // number of particles
#define Q1  // prime numbers ~ kN for some 0<k<1
#define Q2
#define Q3 
#define A // number of overflow slots
#define M // size of hash

struct Parti 
{
   real x,y,z;
   real x_prev,y_prev,z_prev;
   real vof_prev;
}; 

Parti * particle_id[M][A];


void initialize_particle_id()
{
	int i;
	real x,y,z;
	if(initialize){
		struct Parti particle;
		particle.x = P_POS0(0);
		particle.y = P_POS0(0);
		particle.z = P_POS0(0);
		insert(&particle);
	}
}

void update_particle_id()
{
	int i;
	real x,y,z;
		struct Parti particle;
		particle.x = P_POS0(0);
		particle.y = P_POS0(0);
		particle.z = P_POS0(0);
		particle = search(&particle);
		
		// update information
		particle.x_prev = P_POS(0);
		particle.x_prev = P_POS(1);
		particle.x_prev = P_POS(2);
		
		Thread t = P_CELL_THREAD(p);
		cell c = P_CELL(p);
		particle.vof_prev = C_VOF(c,t);
}

int insert(struct Parti *particle)
{	
	int i,j;
	i = 0;
	while(i != A){
		j = hash(particle.x,particle.y,particle.z);
		if(particle_id[j][i] == NULL){
			particle_id[j][i] = &particle;
			return j;
		else 
			i+=1;
		}
	}
	return -1;
}

Parti search(struct Particle *particle)
{
	int i,m,j;
	i = 0;
	while (particle_id[j][i] != NULL || i != A){
		j = hash(particle.x,particle.y,particle.z);
		if ((particle_id[j][i]).x == particle.x && (particle_id[j][i]).y == particle.y 
						&& (particle_id[j][i]).z == particle.z)
			{
			return &particle_id[j][i]; 
			}
	}
	return NULL;
}


int hash(real x, real y, real z)
{
	return ( ((x*A)%Q1) + ((y*A)%Q2) + ((z*A)%Q3) ) % M
}
