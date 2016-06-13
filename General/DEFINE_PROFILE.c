#include "udf.h"

DEFINE_PROFILE(x_velocity,thread,nv)
{
float x[3];
float y;
face_t f;
begin_f_loop(f, thread)
{
F_CENTROID(x,f,thread);
y=x[1];
F_PROFILE(f,thread,nv)=20.*y;
}
end_f_loop(f, thread)

}