/********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Program: WITS0_TCP
 * File: WITS0_TCPCyclic.c
 * Author: fasch_f
 * Created: May 04, 2012
 ********************************************************************
 * Implementation of program WITS0_TCP
 ********************************************************************/

#include <bur/plctypes.h>
#include <string.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

void Val2WITS(void)
{
	//read current PLC Time via function RTC_gettime
	RTC_gettime(&ActTime);
	
	/*
	- generate WITS0 Item entries
	- used functions:
			to generate WITS0 complient date string: GetWITSDate(Time structure)
			to generate WITS0 complient time string: GetWITSTime(Time structure)
			to convert a REAL variable to a string: RealToString(real value, number of position after decimal point)
			to copy one string variable into another string variable: strcpy(target string, source string)
			to add a WITS0 item: witsAddData(Adress of WitsInfo Struct, Item number, Item value as a string)
			to update WITS0 items next time function is called: witsUpdData(Adress of WitsInfo Struct)
	- item value is converted to a string via strcpy-function before call of witsAddData
	*/
	
	// add first WITS0-Item (05) = Date
	strcpy(sDate, (STRING*)GetWITSDate((UDINT)&ActTime));
	witsAddData((UDINT)&strWitsInfo, "05", (UDINT)sDate);
	
	// second WITS0-Item (06) = Time
	strcpy(sTime, (STRING*)GetWITSTime((UDINT)&ActTime, 1));
	witsAddData((UDINT)&strWitsInfo, "06", (UDINT)sTime);
	
	// add measured values
	// WITS0-Item 12 = block position in m
	strcpy(sTemp, (STRING*)RealToString((gDrawServo_Pos/22252000.0), 9));
	witsAddData((UDINT)&strWitsInfo, "12", (UDINT)sTemp);
	grLogVal[0] = gDrawServo_Pos/22252000.0;
	
	//WITS0-Item 13 = rate of penetratioon in m/h
	strcpy(sTemp, (STRING*)RealToString((gDrawServo_Vel_fil*3.6/100), 9));
	witsAddData((UDINT)&strWitsInfo, "13", (UDINT)sTemp);
	grLogVal[1] = gDrawServo_Vel_fil*3.6/100;
	
	//WITS0-Item 14 = hookload in tonns
	strcpy(sTemp, (STRING*)RealToString((gWeight/1000), 9));
	witsAddData((UDINT)&strWitsInfo, "14", (UDINT)sTemp);
	grLogVal[2] = gWeight/1000;
	
	//WITS0-Item 15 = maximum hookload in tonns
	strcpy(sTemp, (STRING*)RealToString((gHookMax/1000), 9));
	witsAddData((UDINT)&strWitsInfo, "15", (UDINT)sTemp);
	grLogVal[3] = gHookMax/1000;
	
	//WITS0-Item 16 = weight on bit avg in tonns
	strcpy(sTemp, (STRING*)RealToString((gWOB/1000), 9));
	witsAddData((UDINT)&strWitsInfo, "16", (UDINT)sTemp);
	grLogVal[4] = gWOB/1000;

		//WITS0-Item 18 = rotary torque in kNm
		//to avoid a division by zero, zero has to be checked before:
		if(gFU_Frequency!=0)
		{
			strcpy(sTemp, (STRING*)RealToString((gFU_Power*25/6283.2/gFU_Frequency/WebPara_Gear), 9));
			grLogVal[5] = gFU_Power*25/6283.2/gFU_Frequency/WebPara_Gear;
		}
			else
			strcpy(sTemp, "0");
	
		witsAddData((UDINT)&strWitsInfo, "18", (UDINT)sTemp);
	
		//WITS0-Item 20 = rotary speed in rpm
		strcpy(sTemp, (STRING*)RealToString((gFU_Frequency*60*WebPara_Gear/25), 1));
		witsAddData((UDINT)&strWitsInfo, "20", (UDINT)sTemp);
		grLogVal[6] = gFU_Frequency*60*WebPara_Gear/25;

		//WITS0-Item 41 = distance to probe via ultrasonic sensor in m
		//may not workling because of vibration sensor box on drill string
		strcpy(sTemp, (STRING*)RealToString((gDistance/100), 1));
		witsAddData((UDINT)&strWitsInfo, "41", (UDINT)sTemp);
		grLogVal[7] = gDistance/100;

		//function witsUpdData has to be performed to reset memory address in structure strWitsInfo so that
		//data can be updated at next time function Val2WITS is called
		witsUpdData((UDINT)&strWitsInfo);
	
	
		for(i = 0; i < (sizeof(TcpServerInfo)/sizeof(TcpServerInfo[0])); i++)
		{
			if(TcpServerInfo[i].udiIdent != 0)
				TcpServerInfo[i].enTCPStatus	= DATA_TO_SEND;
			else
				TcpServerInfo[i].enTCPStatus	= DATA_SENT;
		}
	}

	void _CYCLIC WITS0_TCPCyclic(void)
	{	
		//send WITS0 stream every second if task is running in a 200ms task class
		if(CycleCounter++ == 4)	//0...4 = 5 --> 5 x 200 ms = 1000 s
		{
			CycleCounter 	= 0;	//reset cycle counter
			Val2WITS();				//generate WITS0 stream
			SendTCPStream	= 1;	//trigger sending operation
		}
		
		//open tcp interface and bind socket
		switch(enServerState)
		{
			//select used interface
			case SEL_IF:
		
				strcpy(ServerIPAddr, IPAddrIF2);
				TcpServerPort	= 10001;
			
				strcat(StatusString, ServerIPAddr);
				strcat(StatusString, " used for TCP Server");
			
				enServerState	= TCP_OPEN;
				break;
			
			//Open a TCP socket
			case TCP_OPEN:
				FUB_TcpOpen.enable	= 1;
				FUB_TcpOpen.pIfAddr	= (UDINT)ServerIPAddr;
				FUB_TcpOpen.port	= TcpServerPort;
				FUB_TcpOpen.options	= tcpOPT_REUSEADDR;
			
				TcpOpen(&FUB_TcpOpen);
			
				if(FUB_TcpOpen.status	== 65535)
				{}
				else if(FUB_TcpOpen.status == 0)
				{
					ServerIdent		= FUB_TcpOpen.ident;
					enServerState	= TCP_SERVER;
					strcpy(StatusString, "Tcp open succesfull");
				}
				else
				{
					ServerIdent		= 0;
					enServerState	= TCP_FAIL;
					strcpy(StatusString, "Tcp open for Server failed");
				}
				break;
		
			//Start TCP Server and wait for tcp clients
			case TCP_SERVER:
				FUB_TcpServer.enable	= 1;
				FUB_TcpServer.ident		= ServerIdent;
				FUB_TcpServer.backlog	= sizeof(TcpServerInfo)/sizeof(TcpServerInfo[0]);
		
				//find unconnected client structure if Server is not waiting for connection
				if(FUB_TcpServer.status != 65535)
				{
					for(i = 0; i < (sizeof(TcpServerInfo)/sizeof(TcpServerInfo[0])); i++)
					{
						if(TcpServerInfo[i].udiIdent == 0)
						{
							ServCount	= i;
							break;
						}
					}
			
					if(i == sizeof(TcpServerInfo)/sizeof(TcpServerInfo[0]))	//maximum number of allowed connections reached
					{
						enServerState	= TCP_WAIT;
						break;
					}
				}
			
				FUB_TcpServer.pIpAddr	= (UDINT)TcpServerInfo[ServCount].sIP;
		
				TcpServer(&FUB_TcpServer);
		
				if(FUB_TcpServer.status == 65535)
				{}
				else if(FUB_TcpServer.status == 0) //client connected
				{
					TcpServerInfo[ServCount].udiIdent	= FUB_TcpServer.identclnt;
					TcpServerInfo[ServCount].udiPort	= FUB_TcpServer.portclnt;
				}
				else
				{
					strcpy(StatusString, "Error at TCP Server !!!");
					enServerState	= TCP_FAIL;
					break;
				}
				break;
		
			//check if any connection breaks
			case TCP_WAIT:
				for(i = 0; i < sizeof(TcpServerInfo)/sizeof(TcpServerInfo[0]); i++)
				{
					if(TcpServerInfo[i].udiIdent == 0)	//Connection broken
					{
						enServerState	= TCP_SERVER;
						break;
					}
				}
				break;
		
			case TCP_CLIENT:
			case TCP_FAIL:
				break;
		
			case TCP_CLOSE:
				//close TCP connection
				FUB_TcpClose.enable	= 1;
				FUB_TcpClose.how	= (tcpSHUT_RD | tcpSHUT_WR);
				FUB_TcpClose.ident	= ServerIdent;
		
				TcpClose(&FUB_TcpClose);
		
				if(FUB_TcpClose.status == 65535)
				{}
				else if(FUB_TcpClose.status == 0)
				{
					strcpy(StatusString, "TcpClose successful");
					enServerState	= TCP_OPEN;
				}
				else
				{
					strcpy(StatusString, "TcpClose failed");
					enServerState	= TCP_FAIL;
				}
				break;
			
		}
	
		//send tcp message to all connected clients
		if(SendTCPStream)
		{
			for(i = 0; i < (sizeof(TcpServerInfo)/sizeof(TcpServerInfo[0])); i++)
			{
				if((TcpServerInfo[i].udiIdent != 0) && (TcpServerInfo[i].enTCPStatus == DATA_TO_SEND))	//ident present, client connected
				{
					FUB_TcpSend[i].enable	= 1;
					FUB_TcpSend[i].ident	= TcpServerInfo[i].udiIdent;
					FUB_TcpSend[i].flags	= 0;
				
					if(FUB_TcpSend[i].status != 65535)	//send new data only if last transmission finished! (see B+R Help TcpSend())
					{
						FUB_TcpSend[i].pData	= (UDINT)usiData;
						FUB_TcpSend[i].datalen	= strlen((STRING*)usiData);
					}
				
					TcpSend(&FUB_TcpSend[i]);
				
					if(FUB_TcpSend[i].status == 65535)
					{}
					else if(FUB_TcpSend[i].status == 0)
					{
						strcpy(StatusString, "TcpSend successfull");
						TcpServerInfo[i].enTCPStatus	= DATA_SENT;	//data sent successfully
					}
					else
					{
						//error at sending data via tcp; assumption client connection broken --> reset connection
						TcpServerInfo[i].udiIdent		= 0;
						TcpServerInfo[i].enTCPStatus	= DATA_SENT;
						strcpy(StatusString, "TcpSend failed!");
					}
				}
			}
		
			for(i = 0; i < (sizeof(TcpServerInfo)/sizeof(TcpServerInfo[0])); i++)
			{
				//check if all clients have received data
				if(TcpServerInfo[i].enTCPStatus != DATA_SENT)
				{
					SendTCPStream = 1;
					break;
				}
				else
					SendTCPStream = 0;
			}	
		}
	}
