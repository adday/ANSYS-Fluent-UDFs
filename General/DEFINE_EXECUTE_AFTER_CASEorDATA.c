/******************************************************************************

Implementations of DEFINE_EXECUTE_AFTER_CASE and DEFINE_EXECUTE_AFTER_DATA macros.
Executes after the case/data file is read by FLUENT.
Returns: void
******************************************************************************/

#include "udf.h"

DEFINE_EXECUTE_AFTER_CASE(after_case,libname)
{
	Message("EXECUTE_AFTER_CASE called from %s\n",libname);
}

DEFINE_EXECUTE_AFTER_DATA(after_data,libname)
{
	Message("EXECUTE_AFTER_DATA called from %s\n",libname);
}