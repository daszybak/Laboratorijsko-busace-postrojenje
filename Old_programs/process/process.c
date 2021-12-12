/********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Program: process
 * File: process.c
 * Author: scheibelmasser_a
 * Created: September 16, 2009
 ********************************************************************
 * Implementation of program process
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
 #include <AsDefault.h>
#endif


_LOCAL  UINT uiState, uiCount, uiCurrent;
_LOCAL BOOL bFlag;

//------------------------------------ Sub-Routines ---------------------
// ----------------------------------- FU Drill -------------------------
void setFU2automatic(void)
{
gFU_Command=2; // index Modbus Table
gFU_Remote=1;	
}

void setFU2manuel(void)
{
gFU_Command=3; // index Modbus Table
gFU_Remote=1;		
}


void startFU (REAL frequency)
{
gFU_Command=0; // index Modbus Table
gFU_Argument=frequency*100; // check min/max to do ...
gFU_Remote=1;	
	
}

void stopFU (void)
{
gFU_Command=1; // index Modbus Table
gFU_Remote=1;	
}


USINT FU_ready (void)
{
	return(!gFU_Remote);
	
}

//--------------------------- Servo ----------------------------

void set_Current(UDINT current)
{
gServo_Command=1;            // index Table
gServo_Argument=current;     // check min/max to do ...
gServo_Remote=1;	

}

void set_Velocity(UDINT velocity)
{
gServo_Command=2;            // index Table
gServo_Argument=velocity;     // check min/max to do ...
gServo_Remote=1;

}

void set_Acceleration(UDINT acceleration)
{
gServo_Command=3;            // index Table
gServo_Argument=acceleration;     // check min/max to do ...
gServo_Remote=1;

}

USINT Servo_ready (void)
{
	return(!gServo_Remote);
	
}



//------------------------------------------------------------------------

void _INIT processINIT( void )
{
 // Switch on Ready Lamp
 
 gLamp_Ready=1;
}


void _CYCLIC processCYCLIC( void )
{

switch (uiState)  // control state machine
{
  case 0:  // Off
           
		   
		 // ------------State Function 
		 
		 // fixed Lamps
		  
           gLamp_Automatic=0;
		   gLamp_Manuel=0;
		   gLamp_Drawwork=0;
		   gLamp_Drill=0;
		   gLamp_Pump=0;
		  
		   
		 // fixed Actuators
		 
		   gActFU_On=0;
		   gActServo_Velocity=0;
		   gActServo_Torque=0;
		   gActServo_Up=0;
		   gActServo_Down=0;
		   gAct_Pump=0;
		 
		 // Conditioned Lamps & Actors
		 
		  if (gSwitch_NotAus)
		     {
		      gLamp_Invalid=1;
			  gLamp_Ready=0;
			 }
		  else
		     {
		      gLamp_Invalid=0;
			  gLamp_Ready=1;
			  }
		   
	     //------------ Transition
        
          if ( gSwitch_Manuel && (!gSwitch_NotAus))
		     {
			  
			  if(FU_ready())       // communication ready?
			  {  
			  	 setFU2manuel();   // FU communicatiom
				 uiState=1;
			  }
			 }
		  if ( gSwitch_Automatic && (!gSwitch_NotAus))
		     {
		     			  
			  if(FU_ready())       // communication ready?
			  {  
			   setFU2automatic();   // FU communicatiom
			   uiState=2;
			  }
			 }
  break;
  
  case 1:  // Manuel -------------------------------------------------------
  
         // ------------State Function 
         // fixed Lamps
		   gLamp_Automatic=0;
		   gLamp_Manuel=1;
		 
		 // fixed Actuators
		   gActServo_Torque=0; //no torque control
		   gActServo_Velocity=1; // velocity control with poti
		 // Conditioned Lamps & Actors
		 
	  	 if (gSwitch_Drill) 
		    { 
			  if(gActFU_On)
		      {
			   if(gSwitch_Limit_Temp)  // Test Overtemp Switch
		            gLamp_Invalid=0;
		       else
			        gLamp_Invalid=1;
				     	
		      }	
				
		     gLamp_Drill=1;
			 gActFU_On=1;
			}
		 	else
		    {
		     gLamp_Drill=0;
			 gActFU_On=0;
		    }
		    
		 //---------------------   
		 if (gSwitch_Up) 
		    { 
			 if(gActServo_Up)        // only if active
		      {
			   if(gSwitch_Limit_Up)  // Test Limit Switch
		            gLamp_Invalid=0;
		       else
			        gLamp_Invalid=1;
				     	
		      }
				
			   gLamp_Drawwork=1;
			   gActServo_Up=1;
			   gActServo_Down=0;
			   
			}
		 else
		    {
		     		   	
			 if (gSwitch_Down) 
			    { 
				
				 if(gActServo_Down)        // only if active
			      {
				   if(gSwitch_Limit_Down)  // Test Limit Switch
			            gLamp_Invalid=0;
			       else
			            gLamp_Invalid=1;	
			      }
				
			      gLamp_Drawwork=1;
				  gActServo_Down=1;
				  gActServo_Up=0;
				}
			 else
			    {
			       gLamp_Drawwork=0;
				   gActServo_Down=0;
				   gActServo_Up=0;
				   gLamp_Invalid=0;
			    }
		    }
		 //---------------------      
	 	 if (gSwitch_Pump) 
		    { 
		       gLamp_Pump=1;
			   gAct_Pump=1;
			}
		 else
		    {
		       gLamp_Pump=0; 
			   gAct_Pump=0;  
		    }
			
			
			 
	     //------------ Transition
  
         if ((!gSwitch_Manuel) || gSwitch_NotAus)
		     {
			   uiState=0;
		     }
		     
		  if ( gSwitch_Acknowledge)   // Mode Change to Torque Mode 
		     {
              if(Servo_ready())       // communication ready?
			   {
		         set_Velocity(600);    // ca 6mm/s
			  	 uiCurrent=950;
				 uiCount=0;
				 gActServo_Velocity=0;
		         gActServo_Torque=1;   // activate, otherwise limit switch
				 bFlag=0;
		//		 Pos_Init=1; // set blockpos to zero
			     uiState=11;
			  }	
            }
  break;
  
  case 2: // Automatic ----------------------------------------------------------
  
         // ------------State Function 
         // fixed Lamps
		 
		   gLamp_Manuel=0;
		   gLamp_Drawwork=0;
		   gLamp_Drill=0;
		   gLamp_Pump=0;
		   gLamp_Invalid=0;
		 
	 	 // fixed Actuators
		   gActFU_On=0;
		   gActServo_Velocity=1;
		   gActServo_Torque=0;
		   gActServo_Up=0;
		   gActServo_Down=0;
		   gAct_Pump=0;
		 
		 
		 // Conditioned Lamps & Actors
		 
		   gLamp_Automatic^=1;
		
		
			    
          //------------ Transition
  
         if ( (!gSwitch_Automatic) || gSwitch_NotAus)
		     {
		       if(FU_ready())       // communication ready?
			   {
			    setFU2manuel();
			    uiState=0;
			   }
		     }
/*	     
		 if ( gSwitch_Acknowledge)   // Test Drill, Up/Down
		     {
		      
			  if(FU_ready())       // communication ready?
			   {
			   	 startFU (28.28);   // start xx.yy Hz
			     uiCount=0;
			     uiState=3;
			   }
		     } 
 */
 
          if ( gSwitch_Acknowledge)   // Reference Run
		     {
              if(Servo_ready())       // communication ready?
			   {
		         set_Velocity(600);    // ca 6mm/s
			  	 uiCurrent=12000;
				 uiCount=0;
				 gActServo_Velocity=0;
		         gActServo_Torque=1;   // activate, otherwise limit switch
			     uiState=8;
			  }	
             
			 }
          
  break;
  
  case 3: // Dummy instead automation Program: Drill on , Wait State 5 sek.

         // ------------State Function 
         // fixed Lamps
		   gLamp_Automatic=1;
		   gLamp_Manuel=0;
		   gLamp_Drawwork=0;
		   gLamp_Drill=1;  // Drill!
		   gLamp_Pump=0;
		   gLamp_Invalid=0;
		 
	 	 // fixed Actuators
		   gActFU_On=0;
		   gActServo_Velocity=1;
		   gActServo_Torque=0;
		   gActServo_Up=0;
		   gActServo_Down=0;
		   gAct_Pump=0;
		 		 
		 // Conditioned Lamps & Actors
		        
		
          //------------ Transition
		  
           if (uiCount++ >40)
		    {
			 if(FU_ready())       // communication ready?
			  {
			    stopFU();
			    uiCount=0;
			    uiState=4;	
			  }
			}

          if ( (!gSwitch_Automatic) || gSwitch_NotAus)
		     {
		       if(FU_ready())       // communication ready?
			   {
			     stopFU();
			     uiState=2;	
			   }
		     }

  break;
  
  case 4: // Dummy instead automation Program: Drawwork up/down , Wait State 5 sek.

         // ------------State Function 
         // fixed Lamps
		   gLamp_Automatic=1;
		   gLamp_Manuel=0;
		   gLamp_Drill=0;
		   gLamp_Pump=0;
		   gLamp_Invalid=0;
		 
	 	 // fixed Actuators
		   gActFU_On=0;
		   gActServo_Velocity=1;
		   gActServo_Torque=0;
		   gAct_Pump=0;
		 		 
		 // Conditioned Lamps & Actors
		 
		 if(uiCount < 16)
		   {
		    gActServo_Up=1;
		    gActServo_Down=0;   
		    gLamp_Drawwork=1; // Drawwork Up
		   }
		 else
		  {
		  	if(uiCount>24)
			  {
		       gActServo_Up=0;
		       gActServo_Down=1; 
			   gLamp_Drawwork=1; // Drawwork Down
			  }
			  else
			   {
		        gActServo_Up=0;
		        gActServo_Down=0; 
			    gLamp_Drawwork=0; // Drawwork  
		       }
		   }
		 
		
          //------------ Transition
		  
           if (uiCount++ >40)
		    {
				
			   if(Servo_ready())       // communication ready?
			   {
		         set_Velocity(600);    // ca 6mm/s
			  	 uiCurrent=700;
				 gActServo_Velocity=0;
		         gActServo_Torque=1;   // activate, otherwise limit switch
			     uiState=5;
			  }	
			 
			}

          if ( (!gSwitch_Automatic) || gSwitch_NotAus)
		     {
		       uiState=2;	
			 }

  break;
  
  case 5:  // Current Mode Test
  
          // ------------State Function 
         // fixed Lamps
		  
		   gLamp_Manuel=0;
		   gLamp_Drill=0;
		   gLamp_Pump=0;
		   gLamp_Invalid=0;
		   gLamp_Automatic=1;
		 
	 	 // fixed Actuators
		   gActFU_On=0;
		   gActServo_Velocity=0;
		   gActServo_Torque=1;
		   gActServo_Up=0;
		   gActServo_Down=0;
		   gAct_Pump=0;
		 		 
		 // Conditioned Lamps & Actors
		 
		  gLamp_Drawwork^=1;
		 
		 if (gSwitch_Down) 
		    { 
		      if (uiCurrent>100)
			     uiCurrent-=25;
				 
		      if(Servo_ready())       // communication ready?
			   {
		         set_Current(uiCurrent);   
			   }
			   
			}
			    
		 if (gSwitch_Up) 
		    { 
		      if (uiCurrent<3000)
			     uiCurrent+=25;
				 
		       if(Servo_ready())       // communication ready?
			   {
		         set_Current(uiCurrent);   
			   }
			   
			}	
				
				
          //------------ Transition
		  
		   if ( gSwitch_Acknowledge)
           //if (uiCount++ >40)
		    {
		      if(Servo_ready())       // communication ready?
			   {
		         set_Current(1000);    // Servo Stop Haltestrom
				 uiCount=0;
			     uiState=6;
			   }
			    
			}
   
    
   
   
          if ( (!gSwitch_Automatic) || gSwitch_NotAus)
		     {
			  
			  if(Servo_ready())       // communication ready?
			   {
		         set_Current(1000);   // Servo Stop, set current
				 uiCount=0;
			     uiState=6;
			   }	
			 			   
		     }
		     
		
          if ( !gSwitch_Limit_Auto) // Limit Switch Auto, Servo out of Automatic, Set Current no influence
		     {
			   
			   gActServo_Velocity=1;  
		       gActServo_Torque=0;
			   uiState=7;
			
						  			   
			 }	 
			 

  break;
  
  case 6: // Stop I Control, wait for new set current
  
   // fixed Lamps
		   gLamp_Automatic=1;
		   gLamp_Manuel=0;
		   gLamp_Drawwork=0;
		   gLamp_Drill=0;
		   gLamp_Pump=0;
		   gLamp_Invalid=0;
		 
	 	 // fixed Actuators
		   gActFU_On=0;
		   gActServo_Velocity=0;
		   gActServo_Torque=1;
		   gActServo_Up=0;
		   gActServo_Down=0;
		   gAct_Pump=0;
		                          // Servo Stop, set current
           if(Servo_ready())      // zur Sicherheit mehrfach, Probleme beim Ausschalten des Strommodus
		     set_Current(1000);
    //------------ Transition
		 
           if (uiCount++ >20) //wait 2 sek. until automatic finished (servo communication)
		    {
		       uiState=2;	
			}

  
  break;
  
  case 7: // move from Endswitch
  
  
   // fixed Lamps
		   gLamp_Automatic=1;
		   gLamp_Manuel=0;
		   gLamp_Drawwork=0;
		   gLamp_Drill=0;
		   gLamp_Pump=0;
		   gLamp_Invalid=0;
		 
	 	 // fixed Actuators
		   gActFU_On=0;
		   gActServo_Velocity=1;
		   gActServo_Torque=0;
		   gActServo_Up=1; // both enabled, one limit switch pressed
		   gActServo_Down=1;
		   gAct_Pump=0;
  
    //------------ Transition
		  
           if (gSwitch_Limit_Up && gSwitch_Limit_Down ) //wait until limit switch off
		    {
		       uiState=2;	
			}
/*
          if ( (!gSwitch_Automatic) || gSwitch_NotAus || !gSwitch_Limit_Auto)
		     {
			  
		       uiState=2;	
			 }
 */
  
  
  
  break;   
  
  
  case 8: // Home Run
  
         // ------------State Function 
         // fixed Lamps
		  
		   gLamp_Manuel=0;
		   gLamp_Drill=0;
		   gLamp_Pump=0;
		   gLamp_Invalid=0;
		   gLamp_Automatic=1;
		 
	 	 // fixed Actuators
		   gActFU_On=0;
		   gActServo_Velocity=0;
		   gActServo_Torque=1;
		   gActServo_Up=0;
		   gActServo_Down=0;
		   gAct_Pump=0;
		 		 
		   		 
				 
		 // Conditioned Lamps & Actors
		 
		  gLamp_Drawwork^=1;
		  
		  if(Servo_ready() && uiCount==0)       // communication ready?
			   {
			   	 uiCount++;
		         set_Current(uiCurrent);   
			   }
	
					
          //------------ Transition
		  
		 
          if ( (!gSwitch_Automatic) || gSwitch_NotAus)
		     {
			  
			  if(Servo_ready())       // communication ready?
			   {
		         set_Current(1000);   // Servo Stop, set current
				 uiCount=0;
			     uiState=6;
			   }	
			 			   
		     }
		     
		
          if ( !gSwitch_Limit_Auto) // Limit Switch Auto, Servo out of Automatic, Set Current no influence
		     {
			   
			   gActServo_Velocity=1;  
		       gActServo_Torque=0;
			   uiState=9;
								  			   
			 }	 
   
   break;
   
   
   case 9: // move from endswitch
   
    // fixed Lamps
		   gLamp_Automatic=1;
		   gLamp_Manuel=0;
		   gLamp_Drawwork=0;
		   gLamp_Drill=0;
		   gLamp_Pump=0;
		   gLamp_Invalid=0;
		 
	 	 // fixed Actuators
		   gActFU_On=0;
		   gActServo_Velocity=1;
		   gActServo_Torque=0;
		   gActServo_Up=1; // both enabled, one limit switch pressed
		   gActServo_Down=1;
		   gAct_Pump=0;
  
    //------------ Transition
		  
           if (gSwitch_Limit_Up && gSwitch_Limit_Down ) //wait until limit switch off
		    {
		       uiState=10;
			   Pos_Init=1;	// Set Position to zero
			   
			   
			}
   
   
   
   break;
   
   
   case 10:  // move down a few cm
   
         // ------------State Function 
         // fixed Lamps
		   gLamp_Automatic=1;
		   gLamp_Manuel=0;
		   gLamp_Drill=0;
		   gLamp_Pump=0;
		   gLamp_Invalid=0;
		   gLamp_Drawwork=1; // Drawwork Down
		 
	 	 // fixed Actuators
		 
		   gActFU_On=0;
		   gActServo_Velocity=1;
		   gActServo_Torque=0;
		   gAct_Pump=0;
		   gActServo_Up=0;
		   gActServo_Down=1; 
		   
		 		 
		 // Conditioned Lamps & Actors
		 
		 //------------ Transition
		  
		if (Web_Blockpos < -125) // mm
		{
		  gActServo_Down=0; 
		  gLamp_Drawwork=0;	
		  
		  if(FU_ready())       // communication ready?
			   {
			   	 startFU (28.28);   // start xx.yy Hz
			     uiCount=0;
			     uiState=3;
			   }
		  
			
		}
           

          if ( (!gSwitch_Automatic) || gSwitch_NotAus)
		     {
		       uiState=2;	
			 }
   
      
   break;
   
 
   case 11: // Manuel with Torque Mode -----------------------------
   
      // ------------State Function 
         // fixed Lamps
		   gLamp_Automatic=0;
		   gLamp_Manuel=1;
		 
		 // fixed Actuators
		   gActServo_Torque=1;   //no torque control
		   gActServo_Velocity=0; // velocity control with poti
		 // Conditioned Lamps & Actors
		 
	  	 if (gSwitch_Drill) 
		    { 
			  if(gActFU_On)
		      {
			   if(gSwitch_Limit_Temp)  // Test Overtemp Switch
		            gLamp_Invalid=0;
		       else
			        gLamp_Invalid=1;
				     	
		      }	
				
		     gLamp_Drill=1;
			 gActFU_On=1;
			}
		 else
		    {
		     gLamp_Drill=0;
			 gActFU_On=0;
		    }
		    
		 //------Up/Down Current Control ---------------   
		 
		 gLamp_Drawwork^=1;
		 
		 if (gSwitch_Down) 
		    { 
			  bFlag=1;
		      if (uiCurrent>100)
			     uiCurrent-=50;
				 
		      if(Servo_ready())       // communication ready?
			   {
		         set_Current(uiCurrent);   
			   }
			   
			}
			    
		 if (gSwitch_Up) 
		    { 
			  bFlag=1;
		      if (uiCurrent<3000)
			     uiCurrent+=50;
				 
		       if(Servo_ready())       // communication ready?
			   {
		         set_Current(uiCurrent);   
			   }
			   
			}	
			 
		 
		 //---------------------      
	 	 if (gSwitch_Pump) 
		    { 
		       gLamp_Pump=1;
			   gAct_Pump=1;
			}
		 else
		    {
		       gLamp_Pump=0; 
			   gAct_Pump=0;  
		    }
			
			
			 
	     //------------ Transition
  
        
  
        if ( gSwitch_Acknowledge && bFlag)
           //if (uiCount++ >40)
		    {
		      if(Servo_ready())       // communication ready?
			   {
		         set_Current(1000);    // Servo Stop Haltestrom
				 uiCount=0;
			     uiState=12; //6;
			   }
			    
			}
   
      
   
          if ( (!gSwitch_Manuel) || gSwitch_NotAus)
		     {
			  
			  if(Servo_ready())       // communication ready?
			   {
		         set_Current(1000);   // Servo Stop, set current
				 uiCount=0;
			     uiState=12; //6;
			   }	
			 			   
		     }
		     
		
          if ( !gSwitch_Limit_Auto) // Limit Switch Auto, Servo out of Automatic, Set Current no influence
		     {
			   
			   gActServo_Velocity=1;  
		       gActServo_Torque=0;
			   uiState=13; //7;
			 }	 
			 
   
   break;
  
   case 12: // Stop I-Control, wait for new set current
   
   
      // fixed Lamps
		   gLamp_Drawwork=0;
		   		 
	 	 // fixed Actuators
		 
		   gActServo_Velocity=0;
		   gActServo_Torque=1;
		   gActServo_Up=0;
		   gActServo_Down=0;
		                          // Servo Stop, set current
           if(Servo_ready())      // zur Sicherheit mehrfach, Probleme beim Ausschalten des Strommodus
		      set_Current(1000);
    //------------ Transition
		
		  
           if ((uiCount++ >20)) //wait 2 sek. until automatic finished (servo communication)
		    {
		       uiState=1;	
			}
   
   break;
   
   
   case 13:
   
    // fixed Lamps
		   gLamp_Drawwork=1;
		  		 
	 	 // fixed Actuators
		 
		   gActServo_Velocity=1;
		   gActServo_Torque=0;
		   gActServo_Up=1; // both enabled, one limit switch pressed
		   gActServo_Down=1;
		  
  
    //------------ Transition
		  
           if (gSwitch_Limit_Up && gSwitch_Limit_Down ) //wait until limit switch off
		    {
		       uiState=1;	
			}
   
   break;
   

 }

}

void _EXIT processEXIT( void )
{
 // Ready Lamp off
 gLamp_Ready=0;
}
