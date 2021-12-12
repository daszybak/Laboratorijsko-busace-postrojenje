/********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Program: WITS_TCP
 * File: WITS_TCP.c
 * Author: scheibelmasser_a
 * Created: September 16, 2009
 ********************************************************************
 * Implementation of program WITS_TCP
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
 #include <AsDefault.h>
#endif

#include <AsTCP.h>
#include <string.h>
#include <asstring.h>
#include <sys_lib.h>

//------------------------------------------------------------


/*
TBD for proNova Migration
0108 Depth Bit [m] .... Berechnung Oberfläche Probenkörper mit Blockpos, Distance to Probe notwendig
0110 Depth Hole [m]
0130 Mud Flow In [l/min] ca 5 bei Wasserleitung
opt 0132 Mud Density in [kg/m3] 9982 Wasser 20 Grad

*/


#define PLCID "0101MUL-DAU-001\r\n"  // WITS Record 17 Zeichen fixe Länge!

#define BUFFER 1000 // 1 Stream max 60 Kanäle live

#define TCPMODE 1 // Client 0, Server 1

// Server Mode
#define SERVERPORT 10001
//#define SPSIP     "127.0.0.1" // local host
#define SPSIP     "192.168.0.82"

// Client Mode
#define SERVERIP1 "192.168.0.221" // Server Annemarie
#define SERVERIP2 "192.168.0.158" // PC Toni neu

//-----------------------------------------------------------

_LOCAL BOOL socket_mode;   // Client oder Server


// TCP

_LOCAL  TcpOpen_typ  	    TOpen;
_LOCAL  TcpClose_typ  	    TClose;
_LOCAL  TcpServer_typ  	    TServer;
_LOCAL  TcpSend_typ  	    TSend[2]; // 2 connections !
_LOCAL  TcpClient_typ  	    TClient;
_LOCAL  TcpSend_typ  	    TSendC;


_LOCAL  BOOL           fTOpen, fTClose, fTServer, fTSend, fTClient;
_LOCAL  STRING         ip[16],ip_s [2][16],ip_c[2][16], value[30];
_LOCAL  STRING         data[BUFFER]; 
_LOCAL  UINT           port,port_s,port_c[2], statusTOpen,statusTClose,statusTServer,statusTClient, statusTSend[2], statusTSendC;
_LOCAL  UDINT          ident,ident_c[2];
_LOCAL  UINT		   ch,i;

// WITS

//_LOCAL  UINT			len;
_LOCAL UINT			    StatusGetTime;       /* status values */
_LOCAL RTCtime_typ GetTime;                  /* variables of type RTCtime_typ */

_LOCAL BOOL socket_mode, ip_toggle, snd_toggle;


// SUBs ____________________________________________

void Val2WITS(void)
// Setzt WITS im sendebuffer auf
{
// Time & Date

    StatusGetTime = RTC_gettime(&GetTime);           

// WITS Record 

strcpy(data, "&&\r\n");
strcat(data, PLCID);
strcat(data, "0105");			// Time Based Date
itoa(GetTime.year,(UDINT) &value[0]);
strcat(data,&value[2]);			//JJ
itoa(GetTime.month,(UDINT) &value[0]);
if (strlen(value)==1)
{
 strcat(data, "0");
 strcat(data,&value[0]);		//0M
}
else
{
 strcat(data,&value[0]);		//MM
}
itoa(GetTime.day,(UDINT) &value[0]);
if (strlen(value)==1)
{
 strcat(data, "0");
 strcat(data,&value[0]);    	//0T
}
else
{
 strcat(data,&value[0]);		//TT
}
strcat(data,"\r\n");
strcat(data, "0106");	 	// Time Based Time
itoa(GetTime.hour,(UDINT) &value[0]);
if (strlen(value)==1)
{
 strcat(data, "0");
 strcat(data,&value[0]);	//0H
}
else
{
 strcat(data,&value[0]);	//HH
}
itoa(GetTime.minute,(UDINT) &value[0]);
if (strlen(value)==1)
{
 strcat(data, "0");
 strcat(data,&value[0]);	//0M
}
else
{
 strcat(data,&value[0]);	//MM
}
itoa(GetTime.second,(UDINT) &value[0]);
if (strlen(value)==1)
{
 strcat(data, "0");
 strcat(data,&value[0]);	//0S
}
else
{
 strcat(data,&value[0]);	//SS
}
strcat(data,"\r\n");
                                    // local channels

strcat(data, "0112");				// Time Based block pos in m
ftoa(gDrawServo_Pos/22252000.0,(UDINT) &value[0]);     // Winde 90mm, Getriebeuntersetzung 48, Flaschenzug 2, 64k Incremente/Umd
                                                // Blockpos Servo Faktor: 22251.5 inc/mm
strcat(data,&value[0]);
strcat(data,"\r\n");

strcat(data, "0113");				// Time Based rate of penetration m/h
ftoa(gDrawServo_Vel*3.6/100, (UDINT)&value[0]);    // velocity Servo empiric: 6.6801mm/sec.
strcat(data,&value[0]);
strcat(data,"\r\n");

strcat(data, "0114");				// Time Based hook avg. hook in tonns
ftoa(gWeight/1000, (UDINT)&value[0]);      // Hookload
strcat(data,&value[0]);
strcat(data,"\r\n");

strcat(data, "0118");				// Time Based rotary torque kNm
if(gFU_Frequency!=0)
   ftoa(gFU_Power*25/6283.2/gFU_Frequency/WebPara_Gear,(UDINT) &value[0]);        // power -> M   P=M*2pi*n,  p=25
else
   ftoa(0.0, (UDINT)&value[0]);
strcat(data,&value[0]);
strcat(data,"\r\n");


strcat(data, "0120");				// Time Based rotary speed rpm
ftoa(gFU_Frequency*60*WebPara_Gear/25, (UDINT)&value[0]);      // frequency ->rpm   f=p*n,  p=25
strcat(data,&value[0]);                    // Stimmt nur in Getriebestufe Schalterpos 1 ! 120/240/360
strcat(data,"\r\n");

strcat(data, "0141");				// Time Based distance to probe m
ftoa(gDistance/100, (UDINT)&value[0]);     // Sound Sensor
strcat(data,&value[0]);
strcat(data,"\r\n");

strcat(data,"!!\r\n");        // Stream End

}

// __________________________________________________



void _INIT WITS_TCPINIT( void )
{
	
// Socket

 strcpy(&ip[0], SPSIP); //SPS LAN	
 port=10001;   //lokales Serverport 

// Client Mode
  
 strcpy(&ip_s[0][0], SERVERIP1); // mein PC als Server
 strcpy(&ip_s[1][0], SERVERIP2); // Werner PC als Server
 port_s=SERVERPORT; // Remote Serverport
  
 socket_mode=TCPMODE;   // ste mode Client or Server
  
 fTOpen=1;	// Start socket  
 snd_toggle=1;
 


}

void _CYCLIC WITS_TCPCYCLIC( void )
{
	
// WITS Protocol gernerate


if(snd_toggle) // 1 Hz, if TCP empty
{

Val2WITS();	// Timestamp+local Values -> data[x]

}


// TCPIP open (socket bind)
if(fTOpen)
{
  TOpen.enable=1;	// Input Parameter
  TOpen.pIfAddr=(UDINT)&ip[0];
  
	if (socket_mode)
	  {
	  	TOpen.port=port; 
        TOpen.options=tcpOPT_REUSEADDR;
	  }
	else
	 {		    
	   TOpen.port=0;  // lokaler port auto
       TOpen.options=0;
 	 } 
 
  TcpOpen(&TOpen);
  
  statusTOpen=TOpen.status;
  
     if (!statusTOpen)
        {
            ident       = TOpen.ident;
            fTOpen      = 0;
			if (socket_mode)
			    fTServer=1;
			else
			    fTClient=1;
				
			ch=0;
        } 
        else
        {
            	if (socket_mode)
				    ident       = 0;
		  	   
        
        }
  
}

if(fTServer)
{
TServer.enable=1;
TServer.ident=ident;
TServer.backlog=2; // 2 Connections
TServer.pIpAddr=(UDINT)&ip_c[ch][0];

TcpServer(&TServer);

 statusTServer=TServer.status;
  
     if (!statusTServer)
        {
           ident_c[ch]=TServer.identclnt;
		   port_c[ch]=TServer.portclnt;
		   fTSend=1;
		   if (ch==1)
		   { 
		       fTServer=0;
			   ch++;
		    }
		   else
		   	 { 
		       fTServer=1;
			   ch++;
			 }
        } 
        else
        {
          
        }
	
	
	
}

if(fTClient)
{
// TCP Client

TClient.enable=1;
TClient.ident=ident;
if (ip_toggle)
    TClient.pServer=(UDINT)&ip_s[1][0];
else
    TClient.pServer=(UDINT)&ip_s[0][0];	

TClient.portserv=port_s;

TcpClient(&TClient);

 statusTClient=TClient.status;
  
     if (!statusTClient)
        {
           		   
		   fTSend=1;
		   fTClient=0;
		   		  
        } 
        else
        {
			
			if(statusTClient==tcpERR_INVALID)
			  {
			  	 //close connection
				  if (ip_toggle)
                      ip_toggle=0;	
                  else
                      ip_toggle=1;

			  	 
				  fTClient=0;
				  fTClose=1;
							 
		        }
          
        }

}


if(fTSend)
{


// WITS Protokoll send 

if (socket_mode)  // Server
{

// TCP Send 


for (i=0;i<ch; i++)
{

TSend[i].enable=1;
TSend[i].ident=ident_c[i];
TSend[i].pData=(UDINT)&data[0];
TSend[i].datalen=strlen(data);
TSend[i].flags=tcpMSG_DONTROUTE;


TcpSend(&TSend[i]);

statusTSend[i]=TSend[i].status;
  
     if (!statusTSend[i])
        {
          snd_toggle=1;
        } 
        else
        {
			snd_toggle=0;
			
			if(statusTSend[i]==tcpERR_NOT_CONNECTED)
			  {
			  	snd_toggle=1;
			  	if(ch==1) // close only one connection
				 {
			  	  fTSend=0;
				  ident_c[0]=0;
				  ch=0;
		         
				 }
				 else	// 2 connections to one
				 {
				  fTServer=1;
				  ch=1;	
				  
				  if (i==0) //close first conection
				  {
				  	 ident_c[0]=ident_c[1];
					 ident_c[1]=0;
					 strcpy(&ip_c[0][0],&ip_c[1][0]);
					 strcpy(&ip_c[1][0]," ");
					 port_c[0]=port_c[1];
				     port_c[1]=0;
					 i=1;
					 
				  }
				 }
				 
		       }
          
        }


}

}
else  // act as a client
{
	

// TCP Send 

TSendC.enable=1;
TSendC.ident=ident;
TSendC.pData=(UDINT)&data[0];
TSendC.datalen=strlen(data);
TSendC.flags=tcpMSG_DONTROUTE;


TcpSend(&TSendC);

statusTSendC=TSendC.status;
  
     if (!statusTSendC)
        {
          snd_toggle=1;
        } 
        else
        {
			snd_toggle=0;
			
			if(statusTSendC==tcpERR_NOT_CONNECTED)
			  {
			  
			  	  fTSend=0;
				  fTClose=1;
				  snd_toggle=1;
					 
		      }
          
        }	

}

}


if(fTClose)
{

 TClose.enable=1;
 TClose.ident=ident;
 TClose.how=0;
 TcpClose(&TClose);
 
 statusTClose    = TClose.status;
        if (!statusTClose)
        {
            ident       = 0;
			fTClose		= 0;
            fTOpen      = 1;
			fTServer	= 0;
			fTSend		= 0;
		 
        }
 



}

	
}

void _EXIT WITS_TCPEXIT( void )
{
 fTServer	= 0;

 TClose.enable=1;
 TClose.ident=ident;
 TClose.how=0;
 TcpClose(&TClose);
 
 statusTClose    = TClose.status;

 if (!statusTClose)
        {
            ident       = 0;
			fTClose		= 0;
            fTOpen      = 0;
			fTServer	= 0;
			fTSend		= 0;
			
        }




}
