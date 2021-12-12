/********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Program: Servo_RS232
 * File: Servo_RS232.c
 * Author: scheibelmasser_a
 * Created: September 17, 2009
 ********************************************************************
 * Implementation of program Servo_RS232
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

#include <string.h>
#include <dvframe.h>
//-----------------------------------------------
#define MAXBUF 100    
//#define TIMEOUT 10  // 5 sek. Timeout

#define LINE  "IF1"
#define PARA  "PHY=RS232/BD=19200/PA=E/DB=8"
//-----------------------------------------------

/*
typedef struct SERVO_DIAGOSIS 
{
	unsigned char akn;
	unsigned short error;
	unsigned short status;
	unsigned short serial_stat;
	unsigned char operation;
	unsigned char input_def;
	signed short speed;
	unsigned char input_stat;
	unsigned char output_stat;
	signed long act_pos;
	unsigned char dummy1;
    unsigned char bcc;
	unsigned long dummy2;
	unsigned short dummy3;
	
} SERVO_DIAGNOSIS;
*/



/* variable declaration */

_LOCAL FRM_xopen_typ FrameXOpenStruct;                   /* variable of type FRMXOPEN */
_LOCAL FRM_close_typ FrameCloseStruct;                   /* variable of type FRMCLOSE */
_LOCAL FRM_gbuf_typ FrameGetBufferStruct;                /* variable of type FRMGBUF */
_LOCAL FRM_rbuf_typ FrameReleaseBufferStruct;            /* variable of type FRMRBUF */
_LOCAL FRM_robuf_typ FrameReleaseOutputBufferStruct;     /* variable of type FRMROBUF */
_LOCAL FRM_write_typ FrameWriteStruct;                   /* variable of type FRMWRITE */
_LOCAL FRM_read_typ FrameReadStruct;                     /* variable of type FRMREAD */
//_LOCAL FRM_mode_typ FrameModeStruct;                     /* variable of type FRMREAD */

_LOCAL XOPENCONFIG XOpenConfigStruct;                    /* variable of type XOPENCONFIG */

//_LOCAL SERVO_DIAGNOSIS ServoStruct;  					 // Servo Status Info  !! mapping von Stream auf struktur geht nicht ! alignment?


_LOCAL USINT Error, Write, Close, Read, Open, Wait;
_LOCAL UINT StatusOpen, StatusClose, StatusWrite, StatusRead, StatusGetBuffer, 
       StatusReleaseBuffer, StatusReleaseOutputBuffer;    /* status values */
//_LOCAL UINT StatusMode;
	   
_LOCAL UDINT Ident;                                      /* ident open info */
_LOCAL UDINT* SendBuffer;                                /* pointer to send buffer */
_LOCAL UDINT* ReadBuffer;                                /* pointer to read buffer */

_LOCAL DINT* Testl;
_LOCAL INT* Testi;  

_LOCAL SINT gReadServo_X102;
_LOCAL SINT gReadServo_X104;
_LOCAL SINT gReadServo_X1011;
_LOCAL SINT gReadServo_X1014;
_LOCAL SINT gReadServo_X1015;
_LOCAL SINT gReadServo_X1024;
_LOCAL SINT gReadServo_X1025;
_LOCAL SINT gReadServo_X1012;
_LOCAL SINT gReadServo_X1013;
_LOCAL SINT gReadServo_X1020;
_LOCAL SINT gReadServo_X1023;


_LOCAL USINT Test;  

_LOCAL UINT SendBufferLength;
_LOCAL UINT ReadBufferLength;

_LOCAL STRING StringDevice[30], StringMode[60];           /* initialize strings for FRM_xopen */
_LOCAL USINT WriteData[MAXBUF+1], ReadData[MAXBUF+1];

_LOCAL BOOL ok;//, init;
_LOCAL DINT init_offset;
_LOCAL USINT Set_Cmd,Cmd_Select, Bcc, i,len,cnt;
_LOCAL DINT* Argl;

typedef struct BIASprogs_typ{
	SINT acknowledge;
	SINT commandcode;
	SINT commanddatabyte1;
	SINT commanddatabyte2;
	SINT commanddatabyte3;
	SINT commanddatabyte4;
	SINT commanddatabyte5;
	SINT commanddatabyte6;
	SINT commanddatabyte7;
}BIASprog_typ;

BIASprog_typ BIASprog[1500];





// Send Sequences: Number of Bytes, Data ....
//char Cmd1[]={ 4, 0x1B, 0x01,0x06,0x1C}; // get Firmwareversion
//char Cmd2[]={ 4, 0x1B, 0x01,0x07,0x1D}; // get Diagnosis
//char Cmd3[]={ 5, 0x1B, 0x01,0x41,0x00,0x5B}; // get Configuration
//	{ 17, 0x1B, 0x01,0x17,0x15,0x00, 0x00,0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,0x00, 0x00,0x00},

char Cmd[21][20]={
	{ 4, 0x1B, 0x01,0x06,0x00,0x00, 0x00,0x00,0x00,0x00,0x00}, // get Firmwareversion	0
	{ 4, 0x1B, 0x01,0x07,0x00,0x00, 0x00,0x00,0x00,0x00,0x00}, // get Diagnosis			1
	{ 5, 0x1B, 0x01,0x41,0x00,0x00, 0x00,0x00,0x00,0x00,0x00}, // get Configuration		2
	{ 4, 0x1B, 0x01,0x03,0x00,0x00, 0x00,0x00,0x00,0x00,0x00}, // Hostanmeldung			3
	{ 4, 0x1B, 0x01,0x04,0x00,0x00, 0x00,0x00,0x00,0x00,0x00}, // Hostabmeldung			4
	{ 6, 0x1B, 0x01,0x22,0x00,0x00, 0x00,0x00,0x00,0x00,0x00}, // Variable group 0 read	5
	{ 10,0x1B, 0x01,0x27,0x00,0x01, 0x00,0x00,0x00,0x00,0x00}, // Variable 1 schreiben	6
	{ 10,0x1B, 0x01,0x27,0x00,0x02, 0x00,0x00,0x00,0x00,0x00}, // Variable 2 schreiben	7
	{ 10,0x1B, 0x01,0x27,0x00,0x03, 0x00,0x00,0x00,0x00,0x00}, // Variable 3 schreiben	8
	{ 5, 0x1B, 0x01,0x3E,0x00,0x00, 0x00,0x00,0x00,0x00,0x00}, // Nennstrom lesen		9
	{ 5, 0x1B, 0x01,0x43,0x00,0x00, 0x00,0x00,0x00,0x00,0x00}, // Stromreglerparameter  10	
	{ 5, 0x1B, 0x01,0x56,0x00,0x00, 0x00,0x00,0x00,0x00,0x00}, // Quick stop			11
	{ 4, 0x1B, 0x01,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,0x00}, // Deactivate servo		12
	{ 4, 0x1B, 0x01,0x01,0x00,0x00, 0x00,0x00,0x00,0x00,0x00}, // Activate servo		13
	{ 5, 0x1B, 0x01,0x4A,0x00,0x00, 0x00,0x00,0x00,0x00,0x00}, // Read x10 I/O config	14
	{ 5, 0x1B, 0x01,0x44,0x00,0x00, 0x00,0x00,0x00,0x00,0x00}, // Read parameters of the position loop 15
	{ 19,0x1B, 0x01,0x44,0x01,0x00, 0x00,0x00,0x00,0x00,0x00, 0x00,0x01,0x00,0x00,0x00, 0x00,0x00,0x00,0x00}, // Write parameters of the position loop 16
	{ 7, 0x1B, 0x01,0x4C,0x00,0x00, 0x00,0x00,0x00,0x00,0x00}, // Read BIAS program commands 17
	{ 6, 0x1B, 0x01,0x4D,0x00,0x00, 0x00,0x00,0x00,0x00,0x00}, // Read BIAS program definitions 18
	{ 6, 0x1B, 0x01,0x0D,0x00,0x00, 0x00,0x00,0x00,0x00,0x00}, // Set BIAS process pointer to line (15) 19
	{ 6, 0x1B, 0x01,0x45,0x00,0x00, 0x00,0x00,0x00,0x00,0x00}, // Read position set 20
	};
	//CAREFUL ->//	{ 16,0x1B, 0x01,0x4A,0x01,0x03, 0x03,0x01,0x00,0x00,0x03, 0x03,0x00,0x01,0x03,0x00, 0x00}, // Write x10 I/O config	15

void _INIT Servo_RS232INIT( void )
{
	
	
	/* initialize start values */
	Error = 0;
 
	Write = 0;
	Close = 0;
	Read = 0;
	Wait=0;
	Open= 1;
  
 

	strcpy(StringDevice,LINE);     /* sub slot #1, interface #1 */
	strcpy(StringMode,PARA);       /* RS232 interface Parametr */
  
	/* initialize config structure */
	XOpenConfigStruct.idle = 5;   // empfangen auf Pause im Packet 
	XOpenConfigStruct.delimc =0;  // kein Endzeichen
  
    
	XOpenConfigStruct.delim[0] = 0;   
	XOpenConfigStruct.delim[1] = 0;
  
	XOpenConfigStruct.tx_cnt = 1; // Buffer
	XOpenConfigStruct.rx_cnt = 1;  
	XOpenConfigStruct.tx_len = MAXBUF;
	XOpenConfigStruct.rx_len = MAXBUF;
	XOpenConfigStruct.argc = 0;
	XOpenConfigStruct.argv = 0;

	Pos_Init=1;
	Set_Cmd=1;

}



void _CYCLIC Servo_RS232CYCLIC( void )
{

	if(Wait)// New Command after TX/RX cycle
	{	
		if(gServo_Remote) // Servo Command from Process Ctrl.
		{
			switch (gServo_Command)
			{
   
				case 1: Set_Cmd=6;  // Set Current
					Argl=(DINT*)&Cmd[6][6];
					*Argl=gServo_Argument;     
					break;
				case 2: Set_Cmd=7;  // Velocity
					Argl=(DINT*)&Cmd[7][6];
					*Argl=gServo_Argument;  
					break;
				case 3: Set_Cmd=8;  // Acceleration
					Argl=(DINT*)&Cmd[8][6];
					*Argl=gServo_Argument;
					break;
				case 4: Set_Cmd = 11;  // Quick stop
					break;
				case 5: Set_Cmd = 12;  // Activate servo
					break;
				case 6: Set_Cmd = 13;  // Deactivate servo
					break;
				case 7: Set_Cmd = 14;  // Read x10 I/O config
					break;
				case 8: Set_Cmd = 15;  // Read position loop parameters
					break;
				case 9: Set_Cmd = 16;  // Write position loop parameters
					break;
				case 10: Set_Cmd = 3;  // Host login
					break;
				case 11: Set_Cmd = 17; // Read BIAS program commands
					Argl=(DINT*)&Cmd[17][5];
					*Argl=gServo_Argument;
					break;
				case 12: Set_Cmd = 18;  // Read BIAS program definitions
					Argl=(SINT*)&Cmd[18][5];
					*Argl=gServo_Argument;
					break;
				case 13: Set_Cmd = 19;  // Set BIAS process pointer
					Argl=(DINT*)&Cmd[19][4];
					break;
				case 14: Set_Cmd = 20;  // Read position set
					Argl=(DINT*)&Cmd[20][5];
					*Argl=gServo_Argument;
					break;
			}
		}
		else
			Set_Cmd=1;  // Diagnose		
		// Set_Cmd=10; // Test Stromregler		
		//Set_Cmd=5;  // Test Variablen lesen	
	}
	
	if (Open==1)
	{
		/* initialize open structure */
		FrameXOpenStruct.device = (UDINT) StringDevice;
		FrameXOpenStruct.mode = (UDINT) StringMode;
		FrameXOpenStruct.config = (UDINT) &XOpenConfigStruct;
		FrameXOpenStruct.enable = 1;

		FRM_xopen(&FrameXOpenStruct);                                       /* open an interface */

		StatusOpen = FrameXOpenStruct.status;                  		      /* get status */
		Ident = FrameXOpenStruct.ident;                                    /* get ident */

		if (StatusOpen != 0)                                    /* check status */
		{
			Error = 1;                                      /* set error level for FRM_xopen */
			Read = 0;
			Write =0;
			Close=1;
			ok=0;
		}
		else
		{
			Write =1;
			Wait=0;
			Read = 0;
			Open= 0;
			ok=1;

		}
	
	}	


	if (Read == 1)
	{
		/* initialize read structure */
		FrameReadStruct.enable = 1;
		FrameReadStruct.ident = Ident;

		FRM_read(&FrameReadStruct);                      /* read data form interface */

		ReadBuffer = (UDINT*) FrameReadStruct.buffer;    /* get adress of read buffer */
		ReadBufferLength = FrameReadStruct.buflng;       /* get length of read buffer */
		StatusRead = FrameReadStruct.status;             /* get status */

		if (StatusRead == 0)                             /* check status */
		{
		  	     
			
			      
			Test=(USINT)*ReadBuffer;
			if (Test=='\x06') // only if ACKN
		        
			{				
				memcpy(ReadData, ReadBuffer, ReadBufferLength); /* copy read data into array */
					
				//	if (gServo_Remote) // sync Fehler!
				if(Set_Cmd != 1 && Set_Cmd != 16 && Set_Cmd != 17 && Set_Cmd != 18 && Set_Cmd != 14)
				{
					gServo_Remote=0;
				}
				else if(Set_Cmd == 1)
				{
        			
					Testl=(DINT*)&ReadData[13]; // increments
					Testi=(INT*)&ReadData[9]; // speed
					if (Pos_Init)
					{
						Pos_Init=0;
						init_offset=*Testl;
					}
					gDrawServo_Vel=*Testi;
					gDrawServo_Pos=*Testl-init_offset;
				}
				else if (Set_Cmd == 14){
					gReadServo_X102 = ReadData[1];
					gReadServo_X104 = ReadData[2];
					gReadServo_X1011 = ReadData[3];
					gReadServo_X1014 = ReadData[4];
					gReadServo_X1015 = ReadData[5];
					gReadServo_X1024 = ReadData[6];
					gReadServo_X1025 = ReadData[8];
					gReadServo_X1012 = ReadData[9];
					gReadServo_X1013 = ReadData[10];
					gReadServo_X1020 = ReadData[11];
					gReadServo_X1023 = ReadData[12];
				}
				else if(Set_Cmd == 16){
					gServo_Remote = 0;
				}
				else if(Set_Cmd == 17){
					BIASprog[BIASlinenumber].acknowledge 	  = ReadData[0];
					BIASprog[BIASlinenumber].commandcode      = ReadData[1];
					BIASprog[BIASlinenumber].commanddatabyte1 = ReadData[2];
					BIASprog[BIASlinenumber].commanddatabyte2 = ReadData[3];
					BIASprog[BIASlinenumber].commanddatabyte3 = ReadData[4];
					BIASprog[BIASlinenumber].commanddatabyte4 = ReadData[5];
					BIASprog[BIASlinenumber].commanddatabyte5 = ReadData[6];
					BIASprog[BIASlinenumber].commanddatabyte6 = ReadData[7];
					BIASprog[BIASlinenumber].commanddatabyte7 = ReadData[8];
					gServo_Remote = 0;
				}
				else if(Set_Cmd == 18){
					gServo_Remote = 0;
				}
			}
         
			Read = 0;
			Wait = 1;

			/* initialize release buffer structure */
			FrameReleaseBufferStruct.enable = 1;
			FrameReleaseBufferStruct.ident = Ident;
			FrameReleaseBufferStruct.buffer = (UDINT) ReadBuffer;
			FrameReleaseBufferStruct.buflng = ReadBufferLength;

			FRM_rbuf(&FrameReleaseBufferStruct);      /* release read buffer */

			StatusReleaseBuffer = FrameReleaseBufferStruct.status;  /* get status */
                 			 

			if (StatusReleaseBuffer != 0)          /* check status */
			{
				Error = 3;                     /* set error level for FRM_rbuf */
			}
			else
			{
				ok=1;
				Error = 0;
				  
			}
		}
		else
		{
			Error = 2;                             /* set error level for FRM_read */
			      
				  
		}
 
            
			  
 
	}
  
 

	if (Write == 1)
	{
        

		/* initialize get buffer structure */
		FrameGetBufferStruct.enable = 1;
		FrameGetBufferStruct.ident = Ident;
		  
		FRM_gbuf(&FrameGetBufferStruct);                   /* get send buffer */

		SendBuffer = (UDINT*) FrameGetBufferStruct.buffer; /* get adress of send buffer */
		SendBufferLength = FrameGetBufferStruct.buflng;    /* get length of send buffer */
		StatusGetBuffer = FrameGetBufferStruct.status;     /* get status */

		if (StatusGetBuffer == 0)                          /* check status */
		{      
		  	     
			//strcpy(WriteData,"\x1B\x01\x07\x1D");           // Diagnoseinfo
			//SendBufferLength=strlen( WriteData)% MAXBUF;    //copy write data nur für String !!!!!! keine bin. 0!
				  
			// BCC generate
			Bcc=0;
			Cmd_Select=Set_Cmd; // take over command
				  
			for (i=1;i<Cmd[Cmd_Select][0];i++)
			{
				Bcc^=Cmd[Cmd_Select][i];
			}
				  				 
			len=Cmd[Cmd_Select][0];
			SendBufferLength=len;
			Cmd[Cmd_Select][SendBufferLength]=Bcc;
				  
			memcpy(WriteData, &Cmd[Cmd_Select][1],len);
			memcpy(SendBuffer, WriteData,len);
                

			/* initialize write structure */
			FrameWriteStruct.ident = Ident;
			FrameWriteStruct.buffer = (UDINT) SendBuffer;
			FrameWriteStruct.buflng = SendBufferLength;
			FrameWriteStruct.enable = 1;

			FRM_write(&FrameWriteStruct);               /* write data to interface */

			StatusWrite = FrameWriteStruct.status;      /* get status */
			// Close = 1;

			if (StatusWrite != 0)                       /* check status */
			{
				/* initialize release output buffer structure */
				FrameReleaseOutputBufferStruct.enable = 1;
				FrameReleaseOutputBufferStruct.ident = Ident;
				FrameReleaseOutputBufferStruct.buffer = (UDINT) SendBuffer;
				FrameReleaseOutputBufferStruct.buflng = SendBufferLength;

				FRM_robuf(&FrameReleaseOutputBufferStruct); /* release output buffer */

				StatusReleaseOutputBuffer = FrameReleaseOutputBufferStruct.status; /* get status */
				Error = 5;                                /* set error level for FRM_write */

				if (StatusReleaseOutputBuffer != 0)       /* check status */
				{
					Error = 6;                    /* set error level for FRM_robuf */
							   
				}
				ok=0;
                     
			}
			else
			{
				Write=0;
				Wait=0;
				Read=1;
				ok=1;
			}
		}
		else
		{
			Error = 4;                                  /* set error level for FRM_gbuf */
		}
	}


	if (Wait == 1)
	{
		// 1 cycle time dleay between receive & send
		if(++cnt>1)
		{
			cnt=0;
			Wait = 0;
			Write = 1;
		}
  	
	}



	if (Close == 1)
	{
		Close = 0;

		/* initialize close structure */
		FrameCloseStruct.ident = Ident;
		FrameCloseStruct.enable = 1;

		FRM_close(&FrameCloseStruct);                        /* close interface */

		StatusClose = FrameCloseStruct.status;               /* get status */

		if (StatusClose == 0)                                /* check status */
		{
			ok = 1;
			Open=1;
		}
		else
		{
			Error = 7;                                    /* set error level for FRM_close */
		}
	}
	
	
	
	
	
	
	
	
	
	
	
	
}

void _EXIT Servo_RS232EXIT( void )
{
	

	/* initialize close structure */
	FrameCloseStruct.ident = Ident;
	FrameCloseStruct.enable = 1;

	FRM_close(&FrameCloseStruct);                        /* close interface */

	StatusClose = FrameCloseStruct.status;               /* get status */

	if (StatusClose == 0)                                /* check status */
	{
		ok = 1;
	}
	else
	{
		Error = 7;                                    /* set error level for FRM_close */
	}	
	
}
