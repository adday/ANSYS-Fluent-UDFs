/******************************************************************************
Implementation of DEFINE_EXECUTE_AT_EXIT macro.
Used to execute a function at the end of a FLUENT session.
Returns: void
******************************************************************************/


#include "udf.h"

/******************************************************************************
UDF example of macro
******************************************************************************/

DEFINE_EXECUTE_AT_EXIT(execute_at_exit)
{
	printf("...and now we are exiting, so no one will ever read this.");
}