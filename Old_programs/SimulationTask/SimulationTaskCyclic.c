/********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Program: SimulationTask
 * File: SimulationTaskCyclic.c
 * Author: fasch_f
 * Created: June 27, 2012
 ********************************************************************
 * Implementation of program SimulationTask
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

void _CYCLIC SimulationTaskCyclic(void)
{
	IntBlockPos	= (INT)Web_Blockpos;
	
	//FU status OK
	gFU_Remote	= 0;
	
	//Servo status OK
	gServo_Remote = 0;
	
	//Servo Movement
	if(gActServo_Up && LimitUp)
	{
		gActServo_Down	= 0;
		gDrawServo_Pos 	= gDrawServo_Pos + DrawSpeed;
		gDrawServo_Vel	= 200;
	}
	else if(gActServo_Down && LimitDown)
	{
		gActServo_Up	= 0;
		gDrawServo_Pos = gDrawServo_Pos - (DrawSpeed - DrillOffset);
		gDrawServo_Vel	= -200;
	}
	else
	{
		gDrawServo_Vel	= 0;
	}
	
	if(gDrawServo_Pos >= -500)	//upper limit switch reached
	{
		LimitDown	= 1;
		LimitUp		= 0;
	}
	else if(gDrawServo_Pos <= -44200000)	//lower limit switch reached
	{
		LimitDown	= 0;
		LimitUp		= 1;
	}
	else
	{
		LimitUp		= 1;
		LimitDown	= 1;
	}
	
	if(IntBlockPos < -500)	//block will be drilled
	{
		//DrillOffset will lower the movement becuase of the block to be drilled
		if(DrillOffset < (DrawSpeed - 5) )
			DrillOffset	= DrillOffset + 75;
		else if(DrillOffset > (DrawSpeed - 5))
			DrillOffset	= (DrawSpeed - 5);
		
		Bridge_Raw	= 2796.2*IntBlockPos + 5e+06;
		
	}
	else
	{
		DrillOffset	= 0;
		Bridge_Raw	= 1118480;	//Hookload = 80kg
	}
	
	if(gSwitch_Drill == 1)
		gFU_Frequency	= ((REAL)DrillRPM*25.0)/60.0;
	else
		gFU_Frequency	= 0;
	
	
}
