/********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Program: FU_RS422
 * File: FU_RS422.c
 * Author: scheibelmasser_a
 * Created: September 21, 2009
 ********************************************************************
 * Implementation of program FU_RS422
 ********************************************************************/

#include  <bur/plc.h>
#include  <bur/plctypes.h>
#include  <drv_mbus.h>
#include  <string.h>
#include <dataobj.h>

#ifdef _DEFAULT_INCLUDES
 #include <AsDefault.h>
#endif

#define MB_MODE 0   /* 0 = RTU / 1 = ASCII */

_LOCAL  MBMOpen_typ  	    MOpen;
_LOCAL  MBMaster_typ        Master;
_LOCAL  MBMClose_typ        MClose;
_LOCAL  MBMCmd_typ   	    MCmd;

_LOCAL DatObjInfo_typ  DOInfo;
_LOCAL UINT Status;
_LOCAL UDINT Len;
_LOCAL USINT Type;



_LOCAL  BOOL            fMOpen, fMaster, fMClose, fMCmd, execute;
_LOCAL  USINT           ascii;
_LOCAL  UINT            statusMOpen, statusMaster, statusMClose;
_LOCAL  UINT          statusMCmd;
_LOCAL  UINT			timeout,recordnum;
_LOCAL  UDINT           ident,counter;
_LOCAL  STRING          device[32],mode[32],config[32];

/* modbus VAR */
// coils MB0, status register MB1, input register MB3, holding register MB4, B/R convention 
 
//_LOCAL  BOOL            LocalMB0[6],LocalMB1[5]; 
//_LOCAL  INT             LocalMB3[12],LocalMB4[12],*pLocalMB4;

_LOCAL  INT             LocalMB4[21];
_LOCAL  INT             LocalMB16[10];

//_LOCAL  USINT         data[150];
//_LOCAL  INT      	    EventMB0,EventMB1,EventMB3,EventMB4,*pEventMB4;
_LOCAL  UINT            i;


typedef struct CmdTable_typ
{
    USINT           mfc;
    USINT           node;
    UDINT           data;
    UINT            offset;
    UINT            length;
} CmdTable_typ;

//Testtabelle für MBMCmd 
CmdTable_typ CmdTable[] =
{
//     mfc    node          data            offset  length  

    {   16,     0x1f,      (UDINT) &LocalMB16[0],   1,      2       }, // Motor on
    {   16,     0x1f,      (UDINT) &LocalMB16[2],   1,      1       }, // Motor off
    {   16,     0x1f,      (UDINT) &LocalMB16[3],   0x180,  2       }, // Modbus Ctrl.
    {   16,     0x1f,      (UDINT) &LocalMB16[5],   0x180,  2       }, // Local Ctrl.	
	
    //{   16,     0x1f,      (UDINT) &LocalMB16[2],   0x910,  1       }, // Enter
	
//    {   3,      2,      (UDINT) &LocalMB3[0],   18,      1       },
//    {   6,      2,      (UDINT) &LocalMB4[3],   3,      1       },
//    {   2,      2,      (UDINT) &LocalMB1[0],   0,      5       },
//    {   1,      2,      (UDINT) &LocalMB0[0],   0,      3       },
//    {   5,      2,      (UDINT) &LocalMB0[3],   3,      1       },
//    {   15,     2,      (UDINT) &LocalMB0[4],   4,      2       },
//    {   7,      2,      (UDINT) &data[0], 	    0,      0       },
//    {   20,     2,      (UDINT) &data[0], 	    0,      6       },
};



// _______________________________________________________________________


void _INIT FU_RS422INIT( void )
{
// Test Data Object Properties

    DOInfo.enable   = 1;
    DOInfo.pName    = (UDINT) "mb_para";
  
   /* Call  FUB  */
    DatObjInfo(&DOInfo);
  
   /* Get status */
    Status = DOInfo.status;
    Len = DOInfo.len;
    Type = DOInfo.MemType;


 /* modbus VAR */
//    pEventMB4       = &EventMB4;
//    pLocalMB4       = &LocalMB4[0];

//    EventMB0        = 1;
//    EventMB1        = 1;
//    EventMB3        = 1;
//    EventMB4        = 1;

 //   LocalMB4[3]     = 8000;
 //   LocalMB4[4]     = 30000;
 //   LocalMB4[5]     = 0;
	
//	 LocalMB4[8]     = 0;
//     LocalMB4[0]     = 0;

//     i           = 0;


// Prepare FU Commands

	  LocalMB16[0]=0x0002 ; // Motor On
	  LocalMB16[1]=2727 ; // Frequency
	  
	  LocalMB16[2]=0x0000 ; // Motor Off		

	  LocalMB16[3]=0x0002 ; // Frequ. Ref. Modbus
  	  LocalMB16[4]=0x0002 ; // Run Cmd Modbus
	
	  LocalMB16[5]=0x0001 ; // Frequ. Ref. Local
  	  LocalMB16[6]=0x0001 ; // Run Cmd Local
	
	
    /* init for OPEN */
	
    ascii           = MB_MODE;    /* 0 = RTU / 1 = ASCII */
    timeout         = 500;
 
    strcpy(&device[0], "SS1.IF1");
	 
    	
	if (!ascii)    
	   strcpy(&mode[0], "PHY=RS232/BD=9600/PA=N/DB=8");  // RTU
    else         
	   strcpy(&mode[0], "PHY=RS232/BD=9600/PA=E/DB=7");  // ASCII
   
    strcpy(&config[0],  "mb_para");
	
  
	
 
  // data  for Fkt 7 and 20
  //  data[0]     = 0x04;
  //  data[1]     = 0x05;
   // data[2]     = 0x45;
    //data[3]     = 0xFF;
    //data[4]     = 0x00;
    //data[5]     = 0xFE;

 
		
    fMaster     = 0;   // set in the watch window to operate
    fMCmd       = 0;
    fMClose     = 0;
	fMOpen      = 1;
	
	
	
	
}

void _CYCLIC FU_RS422CYCLIC( void )
{
	
if(gFU_Remote) // FU Command from Process Ctrl.
{
  if (fMaster==0)  // wait till master mode finished
     {
      fMCmd=1;
	  
	 }
}	
	
if (fMOpen) // Subroutine open if flag is set in Monitor
    {
        /* modbus - Master OPEN */
        MOpen.enable    = 1;

 /*     
         strcpy(&device[0], "IF1");
		
			
			if (!ascii)    
			   strcpy(&mode[0], "PHY=RS232/BD=115200/PA=E/DB=8");
            else         
	           strcpy(&mode[0], "PHY=RS232/BD=115200/PA=E/DB=7");

 */     

        MOpen.pDevice   =	(UDINT) &device[0];
        MOpen.pMode = 		(UDINT) &mode[0];
        MOpen.pConfig   = 	(UDINT) &config[0];
        MOpen.timeout   = timeout;
        MOpen.ascii = ascii;
		
        MBMOpen(&MOpen);
        statusMOpen     = MOpen.status;
        if (!statusMOpen)
        {
            ident       = MOpen.ident;
            fMOpen      = 0;
            fMaster     = 1;
            fMCmd       = 0;
            fMClose     = 0;
        }
        else
        {
            ident       = 0;
            fMOpen      = 0;
        }
  //       i           = 0;
    }
    
//_____________________________________________________________________


    if (fMaster)   // Subroutine open if flag is set in Monitor
    {
        /* modbus - Master */
		

		
        Master.enable   = 1;
        Master.ident    = ident;
        MBMaster(&Master);
		
        statusMaster    = Master.status;
        execute     = Master.execute;
        recordnum   = Master.recordnum;
		
	
        if (!statusMaster) // no error 
        {
            if (gFU_Remote)
			    fMaster     = 0;
			
            counter++;
		
	    }
        else if (statusMaster == 65535)
        {
            fMaster     = 1;
        }
        else
        {
            ;//fMaster     = 0; kein Stillstand
		
        }
    }
      
  
 
 // ____________________________________________________________________________   

	
    if (fMCmd) // Subroutine cmd if flag is set 
    {
       // modbus - Master
		
		i=gFU_Command;   // Index CMD Table
		
		if (i==0) // Start Motor with Frequency
		    LocalMB16[1]=gFU_Argument; // Frequency
		
        MCmd.enable     = 1;
        MCmd.ident      = ident;
        MCmd.mfc        = CmdTable[i].mfc;
        MCmd.node       = CmdTable[i].node;
        MCmd.data       = CmdTable[i].data;
        MCmd.offset     = CmdTable[i].offset;
        MCmd.len        = CmdTable[i].length;
        MBMCmd(&MCmd);
        statusMCmd      = MCmd.status;
		
        if (!statusMCmd)
        {
            fMCmd       = 0;  // one Modbus command, than master mode for monitoring
			gFU_Remote =0;
			fMaster=1;
           
        }
        else if (statusMCmd == 65535)
        {
            fMCmd       = 1;
        }
        else
        {
            fMCmd       = 0;  // monitor and try again
		    fMaster=1;
        }
    }
    

    
    if (fMClose) // Subroutine close if flag is set in Monitor
    {
        fMaster         = 0;
        fMClose         = 0;

        /* modbus - Master CLOSE */
        MClose.enable   = 1;
        MClose.ident    = ident;
        MBMClose(&MClose);

        statusMClose    = MClose.status;
        if (!statusMClose)
        {
            ident       = 0;
            fMOpen      = 0;
            fMCmd       = 0;
        }
    }

    /* Funktopn  06 / 16  Variablen*/
    //LocalMB4[3]++;
    //LocalMB4[4]++;
    //LocalMB4[8]++;



gFU_Frequency=LocalMB4[4]/100.0;	
gFU_Power=LocalMB4[7];

}

void _EXIT FU_RS422EXIT( void )
{

    MClose.enable   = 1;
    MClose.ident    = ident;
    MBMClose(&MClose);


}
