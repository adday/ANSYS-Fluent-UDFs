/******************************************************************************
Implementations of DEFINE_EXECUTE_ON_LOADING macro.
Used to specify a function that executes as soon as a compiled
UDF library is loaded in FLUENT. Useful for setup of UDF models.
Best place to reserve user-defined scalar and user-defined memory.
Returns: void
******************************************************************************/

#include "udf.h"

/******************************************************************************
UDF to print version and release number of library being loaded
******************************************************************************/
//example of how to reference library specs (define them yourself)
static int version = 1;
static int release = 2;

DEFINE_EXECUTE_ON_LOADING(report_version, libname)
{
	Message("\nLoading %s version %d.%d\n",libname,version,release);
}