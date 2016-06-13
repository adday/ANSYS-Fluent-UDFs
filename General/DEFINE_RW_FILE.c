/******************************************************************************
Implementations of DEFINE_RW_FILE macro.
Use to specify customized information to write to case/data file,
or read from case/data file. Can store custom variables of any type.
Often used to save dynamic information while solution is calculated.
Returns: void

NOTE: CANNOT BE USED IN UDFs EXECUTED ON WINDOWS SYSTEMS
******************************************************************************/

#include "udf.h"

/******************************************************************************
UDF that increments a variable, writes it to a data file, and reads it back in
******************************************************************************/
int var = 10; // define global variable var for read/write

DEFINE_RW_FILE(writer,fp)
{
	printf("Writing UDF data to data file...\n");
	fprintf(fp,"%d",var);
}

DEFINE_RW_FILE(reader,fp)
{
	printf("Reading UDF data from data file...\n");
	fscanf(fp,"%d",&var);
}



