/********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Program: Measure
 * File: MeasureInit.c
 * Author: fasch_f
 * Created: June 27, 2012
 ********************************************************************
 * Implementation of program Measure
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

void _INIT MeasureInit(void)
{
	gADC_Config=0x32; // 2mV/V, 10 scans/sec.
		
	WebPara_Gear=1;
	old_gear=1;
}
