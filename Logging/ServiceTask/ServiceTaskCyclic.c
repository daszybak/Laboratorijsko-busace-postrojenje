/********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Program: ServiceTask
 * File: ServiceTaskCyclic.c
 * Author: fasch_f
 * Created: March 06, 2014
 ********************************************************************
 * Implementation of program ServiceTask
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

void _CYCLIC ServiceTaskCyclic(void)
{
	FUB_GetTime.enable			= 1;
	FUB_GetTime.pDTStructure	= (UDINT)&strCurrentDT;
	
	DTStructureGetTime(&FUB_GetTime);
}
