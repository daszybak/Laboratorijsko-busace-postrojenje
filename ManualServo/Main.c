/*********************************************************************************
 * Copyright:  
 * File: 	  ManualServoTest
 * Author:    Karlo Delmis
 * Created:   June 29, 2021/8:41 PM 
 *********************************************************************************/ 

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif


#define ON 1
#define OFF 0

// ----------------------------------- FU Drill -------------------------
void setFU2automatic(void)
{
	gFU_Command = 2; // index Modbus Table
	gFU_Remote = ON;	
}

void setFU2manuel(void)
{
	gFU_Command = 3; // index Modbus Table
	gFU_Remote = ON;		
}


void startFU (REAL frequency)
{
	gFU_Command = 0; // index Modbus Table
	gFU_Argument = frequency*100; // check min/max to do ...
	gFU_Remote = ON;	
	
}

void stopFU (void)
{
	gFU_Command = 1; // index Modbus Table
	gFU_Remote = ON;	
}


USINT FU_ready (void)
{
	return(!gFU_Remote);
	
}

//--------------------------- Servo ----------------------------
//torque
void set_Current(UDINT current)
{
	gServo_Command = 1;            // index Table
	gServo_Argument = current;     // check min/max to do ...
	gServo_Remote = ON;	

}

//velocity
void set_Velocity(UDINT velocity)
{
	gServo_Command = 2;            // index Table
	gServo_Argument = velocity;     // check min/max to do ...
	gServo_Remote = ON;

}

void set_Acceleration(UDINT acceleration)
{
	gServo_Command = 3;            // index Table
	gServo_Argument = acceleration;     // check min/max to do ...
	gServo_Remote = ON;

}

void set_ServoOFF(void)
{
	gServo_Command = 5;                
	gServo_Remote = ON;	

}

void set_ServoON(void)
{
	gServo_Command = 6;            
	gServo_Remote = ON;	

}

void set_readservoioconfig (void)
{	
	gServo_Command = 7;             
	gServo_Remote = ON;

}

void set_readpositionloop (void)
{	
	gServo_Command = 8;            
	gServo_Remote = ON;
}

void set_writepositionloop (void)
{
	gServo_Command = 9;           
	gServo_Remote = ON;
}

void set_hostlogin (void)
{
	gServo_Command = 10;           
	gServo_Remote = ON;
}

void set_readBIAS (INT linenumber)
{
	gServo_Command = 11;
	gServo_Argument = linenumber;
	gServo_Remote = ON;
}
	
void set_readBIASdefinition (INT linenumber1)
{
	gServo_Command = 12;
	gServo_Argument = linenumber1;
	gServo_Remote = ON;
}	

void set_BIASprocesspointer (INT process1)
{
	gServo_Command = 13; 
	gServo_Argument = process1;
	gServo_Remote = ON;
}

void set_readpositionset (INT numberofpositionset)
{
	gServo_Command = 14; 
	gServo_Argument = numberofpositionset;
	gServo_Remote = ON;
}


USINT Servo_ready (void)
{
	return(!gServo_Remote);	
}

//START
void _INIT ProgramInit(void)
{
	//PLC booted
	gLamp_Ready = ON; //Ready Lamp ON
	
	//actuators
	gActServo_Up = OFF;
	gActServo_Down = OFF;
	gActServo_Velocity = OFF;
	gActServo_Torque = OFF;
	gActFU_On = OFF;
	gAct_Pump = OFF;
	
	//lamps
	gLamp_Drawwork = OFF;
	gLamp_Automatic = OFF;
	gLamp_Invalid = OFF;
	gLamp_Manuel = OFF;
	gLamp_Pump = OFF;
	gLamp_Drill = OFF;
}
//CYCLIC
void _CYCLIC ProgramCyclic(void)
	
{		
	
	//OFF mode
	if((gSwitch_Manuel == OFF) && (gSwitch_Automatic == OFF) && (gSwitch_NotAus == OFF)){
		//actuators
			
			//gActServo_Velocity = OFF;
			//gActServo_Up = OFF;
		
			gActServo_Down = OFF;
			gActServo_Torque = OFF;
			gActFU_On = OFF;
			gAct_Pump = OFF;

		
	//give half second input (run-in command)	
		if(halfsecondcounter < 5 && overcountingflag == OFF){
			gActServo_Velocity = ON;
			gActServo_Up = ON;
		}
		if(halfsecondcounter >= 5){
			overcountingflag = ON;
		}
		if(overcountingflag){
			gActServo_Velocity = OFF;
			gActServo_Up = OFF;
		}
		
		halfsecondcounter++;
	
		
			offstate++;
			manualservoflag = OFF;
			manualservo = OFF;
			//lamps	
			gLamp_Drawwork = OFF;
			gLamp_Invalid = OFF;
			gLamp_Pump = OFF;
			gLamp_Drill = OFF;
		
			//lamps blinking in selector switch off position
			if(lampinput > 1000){
				lampinput = 0;
			}
			lampinput += 10;
			if(lampinput%200 == 0){
				gLamp_Manuel = ON;
				gLamp_Automatic = ON;
			}
			else {
				gLamp_Manuel = OFF;
				gLamp_Automatic = OFF;
			}
		}
	
	
		//check if manual mode selected
		if (gSwitch_Manuel && (!gSwitch_NotAus) && !manualservoflag){
		
		if(Servo_ready){
			set_ServoON;
		}
			
			manualservoflag = ON;
			manualservo = ON;
			offstate = OFF;
			if(FU_ready())       // communication ready?
		{  
			setFU2manuel();   // VFD to manual mode
		}
		}
	

	
		//go into emergency state if 2nd lowest emergency stop is reached
		if(gSwitch_Limit_Down2){
			lampinputinvalid += 10;
			if(lampinputinvalid == 1000){
				lampinputinvalid = 0;
			}
			if(lampinputinvalid %10 == 0){
				gLamp_Invalid = ON;
			}
			emergencystateDown = ON;
		}
	else{
		emergencystateDown = OFF;
		stopFUflag = ON;
	}
		
		//go into emergency state if 2nd highest emergency stop is reached
		if(gSwitch_Limit_Up2){
			lampinputinvalid += 10;
			if(lampinputinvalid == 1000){
				lampinputinvalid = 0;
			}
			if(lampinputinvalid %10 == 0){
				gLamp_Invalid = ON;
			}
			emergencystateUp = ON;
		}
		//if you go out of emergency state it the drill motor will turn on again
	else{
		emergencystateUp = OFF;
		stopFUflag = ON;
	}
	
		//only go down/up (the opposite of the reached switch) if one of the emergency states is reached
		if(emergencystateUp){
			gActServo_Down = gSwitch_Down;
			gActServo_Up = OFF;
			gActServo_Velocity = ON;
		/*// stop drill motor
		stopFU();
		gLamp_Drill = OFF;
		stopFUflag = ON;*/
		}
	
		if(emergencystateDown){
			gActServo_Up = gSwitch_Up;
			gActServo_Down = OFF;
			gActServo_Velocity = ON;
		/*// stop drill motor
		stopFU();
		gLamp_Drill = OFF;
		stopFUflag = ON;*/
		}
	

	
		//joystick and potentiometer control in velocity mode
		//potentiometer control for top drive rpm
		if(manualservo && !emergencystateUp && !emergencystateDown){
		//servo
			gLamp_Manuel = ON;
			gLamp_Automatic = OFF;
			gLamp_Drawwork = ON;
			gActServo_Velocity = ON;
			gActServo_Torque = OFF;
			gActServo_Up = gSwitch_Up;
			gActServo_Down = gSwitch_Down;
			
		//top drive motor
		if (gSwitch_Drill && !stopFUflag) 
		{ 
			if(gActFU_On)
			{
				// Test Overtemperature Switch
				if(gSwitch_Limit_Temp)  
					gLamp_Invalid = OFF;
				else
					gLamp_Invalid = ON;
				     	
			}	
				
			gLamp_Drill = ON;
			//operate the VFD with the potentiometer
			gActFU_On = ON; 
		}
		else
		{
			gLamp_Drill = OFF;
			gActFU_On = OFF;
		}
		
		//give half second input (run-in command)
		if(halfsecondcounter < 5 && overcountingflag == OFF){
			gActServo_Up = ON;
		}
		if(halfsecondcounter >= 5){
			overcountingflag = ON;
		}
		if(overcountingflag){
			;
		}
		halfsecondcounter++;
		}

//add torque mode with flag	
	
	//turn on pump if switch is on    
	if (gSwitch_Pump && !stopFUflag) 
	{ 
		gLamp_Pump = ON;
		gAct_Pump = ON;
	}
	else
	{
		gLamp_Pump = OFF; 
		gAct_Pump = OFF;  
	}
	
	//servo disabled if the highest or lowest inductive proximity switches are reachead
	if(gSwitch_Limit_Down1){
		gLamp_Invalid = ON;
		if(Servo_ready()){
			set_ServoOFF();
		}
		// stop drill motor
		stopFU();
		gLamp_Drill = OFF;
		stopFUflag = ON;
	}
	
	if(gSwitch_Limit_Up1){
		gLamp_Invalid = ON;
		if(Servo_ready()){
			set_ServoOFF();
		}
		// stop drill motor
		stopFU();
		gLamp_Drill = OFF;
		stopFUflag = ON;
	}
	
	//reset run-in command
	if(gSwitch_NotAus){
		halfsecondcounter = 0;
		overcountingflag = OFF;
	}
}

	

void _EXIT ProgramExit(void)
{
	gLamp_Ready = OFF;
}

