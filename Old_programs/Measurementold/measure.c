/********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Program: Measurement
 * File: measure.c
 * Author: scheibelmasser_a
 * Created: July 01, 2009
 ********************************************************************
 * Implementation of sensor data acquisition
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
 #include <AsDefault.h>
#endif

_LOCAL USINT old_gear; 

void _INIT NewProgramInit( void )

{
		gADC_Config=0x32; // 2mV/V, 10 scans/sec.
		
		WebPara_Gear=1;
		old_gear=1;
}

void _CYCLIC NewProgramCyclic( void )
{
	
	//---------- check webpage update

	if(WebPara_Gear!=old_gear)
	{
		// do neccessary state changes if neccessary
	
		old_gear=WebPara_Gear;
	
	}
	
	//-------------------------------
	//Hookload
	//gWeight=Bridge_Raw*300.0*8*2/8388607+0.58/8;  // kg Calculation cell 2mV/V, ADC 16mV/V -> Faktor 8
	// max.load: 300kg 2mV/V cell, ADC 24 Bit: 8388607
	// Factor: 8*300/8388607, Korrektor 0.58kg
	// Flaschenzug: Faktor 2
	gWeight=Bridge_Raw*2.0*300.0/8388607.0; 											  
	 	 	 
	//Blockpos
	
	gDistance=Sound_Raw * 10.0/4096+20.0; // cm : Calculation-> Output 0-10V skaliert auf 15 Bit (8<*4096) + 20cm offset */


	//-------------------------- Web Readings 

	Web_Blockpos=gDrawServo_Pos/22251.5; //[mm}
	
	Web_ROP=gDrawServo_Vel/100.0;//[mm/s]

	Web_Hookload=gWeight; //[kg]

	if(gFU_Frequency!=0)
		Web_Torque=gFU_Power*25.0/6.2832/gFU_Frequency/WebPara_Gear; // [Nm] M=P/omega
	else
		Web_Torque=0;
	Web_RPM=gFU_Frequency*60.0*WebPara_Gear/25.0; //[upm} f=p*n
	Web_Sonic=gDistance*10.0; // [mm]

	//get maximum hookload
	if(gHookMax < gWeight)
		gHookMax = gWeight;
	
	gWOB	= gHookMax - gWeight;
	
	Web_MaxHookload = gHookMax;	
		
	Web_WOB	= gWOB;
	
	if(Web_ResetMaxHook)
	{
		Web_ResetMaxHook	= 0;
		gHookMax			= 0;
	}
	
	if(Web_PosInit)
	{
		Web_PosInit = 0;
		Pos_Init	= 1;
	}

}
