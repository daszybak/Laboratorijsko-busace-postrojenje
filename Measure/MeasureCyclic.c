/********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Program: Measure
 * File: MeasureCyclic.c
 * Author: fasch_f
 * Created: June 27, 2012
 ********************************************************************
 * Implementation of program Measure
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

void _CYCLIC MeasureCyclic(void)
{
	//---------- check webpage update

	if(WebPara_Gear!=old_gear)
	{
		// do neccessary state changes if neccessary
		old_gear=WebPara_Gear;
	}
	
	DisableWeightFilter		= Web_DisableWeightFilter;
	DisableServoVelFilter	= Web_DisableROPFilter;
	WeightFilterBase		= Web_WeightFilterBase;
	ROPFilterBase			= Web_ROPFilterBase;
	
	//-------------------------------
	//Hookload
	//gWeight=Bridge_Raw*300.0*8*2/8388607+0.58/8;  // kg Calculation cell 2mV/V, ADC 16mV/V -> Faktor 8
	// max.load: 300kg 2mV/V cell, ADC 24 Bit: 8388607
	// Factor: 8*300/8388607, Korrektor 0.58kg
	// Flaschenzug: Faktor 2
	
	//Filter Bridge_Raw weight for a smooth value of Hookload
	FUB_FilterWeight.enable	= 1;
	FUB_FilterWeight.base	= WeightFilterBase;
	FUB_FilterWeight.x		= Bridge_Raw1;
	
	bridgeraw1t = Bridge_Raw1;
	bridgeraw2t = Bridge_Raw2;
	bridgeraw3t = Bridge_Raw3;
	bridgeraw4t = Bridge_Raw4;
	
	if(DisableWeightFilter == 0)
		LCRMovAvgFlt(&FUB_FilterWeight);
	
	if(FUB_FilterWeight.status == 0)
		gWeight	= FUB_FilterWeight.y*2.0*300.0/8388607.0;
	
//	gWeight=Bridge_Raw*2.0*300.0/8388607.0; 											  
	 	 	 
	//Blockpos
	
	//DISTANCE TO SPECIMEN EDITED BY ANTON - Bypassed the Ultrasonic Sensor
	//gDistance=Sound_Raw * 10.0/4096+20.0; // cm : Calculation-> Output 0-10V skaliert auf 15 Bit (8<*4096) + 20cm offset */
	gDistance =((gDrawServo_Pos/22251.5)+26.057)/10;

	//-------------------------- Web Readings 

	Web_Blockpos=gDrawServo_Pos/22251.5; //[mm}
	
	//Filter Servo velocity for a smooth value of ROP
	FUB_FilterServoVel.enable	= 1;
	FUB_FilterServoVel.base		= ROPFilterBase;
	FUB_FilterServoVel.x		= gDrawServo_Vel;
	
	if(DisableServoVelFilter == 0)
		LCRMovAvgFlt(&FUB_FilterServoVel);
	
	if(FUB_FilterServoVel.status == 0)
		gDrawServo_Vel_fil	= FUB_FilterServoVel.y;
	
	Web_ROP	= (gDrawServo_Vel_fil/100.0);
	
//	Web_ROP=gDrawServo_Vel/100.0;//[mm/s]

	Web_Hookload= Bridge_Raw1; //[kg]

	if(gFU_Frequency!=0)
		Web_Torque=gFU_Power*25.0/6.2832/gFU_Frequency/WebPara_Gear; // [Nm] M=P/omega
	else
		Web_Torque=0;
	
	Web_RPM=gFU_Frequency*60.0*WebPara_Gear/25.0; //[upm} f=p*n
	Web_Sonic=gDistance*10.0; // [mm]
	
	//get minimum torque---Added by Anton
	if(Web_RPM >0)
	{if(gTorqMin > Web_Torque)
			gTorqMin = Web_Torque;
		Web_Torque = Web_Torque-gTorqMin;
	}
	
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
