/********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Program: WITS0_TCP
 * File: WITS0_TCPInit.c
 * Author: fasch_f
 * Created: May 04, 2012
 ********************************************************************
 * Implementation of program WITS0_TCP
 ********************************************************************/
//
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

void _INIT WITS0_TCPInit(void)
{
	//initialize WITS0 Recorord
	//WITS_REC_GENTIME = 01
	//sWellID = MUL-Minirig-001
	
	witsInitRecord((STRING*)WITS_REC_GENTIME, (UDINT)&sWellID, (UDINT)&strWitsInfo, (UDINT)usiData, sizeof(usiData));
	
	//read IP address of ethernet interface
	FUB_GetIP.enable	= 1;
	FUB_GetIP.pDevice	= (UDINT) "IF2";
	FUB_GetIP.pIPAddr	= (UDINT) IPAddrIF2;
	FUB_GetIP.Len		= 16;
	
	CfgGetIPAddr(&FUB_GetIP);
	
}
