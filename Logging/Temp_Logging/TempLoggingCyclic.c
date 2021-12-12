/********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Program: TempLogging
 * File: TempLoggingCyclic.c
 * Author: fasch_f
 * Created: March 06, 2014
 ********************************************************************
 * Implementation of program TempLogging
 ********************************************************************/

#include <bur/plctypes.h>
#include <string.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

//Logging Line:
//DateTime;grLogVal[0];grLogVal[1];grLogVal[n]

// Beschreibung der Loggingkanäle kann in jedem Programm passieren. Am Ende des Programmes folgende Zeilen einfügen und anpassen...
// grLogVal[0]		= fHochdruck;
// grLogVal[1]		= gfWeight;
// grLogVal[2]		= gfTorque;
// grLogVal[3]		= fDruckRegler;
// grLogVal[4]		= fPumpendruck;
// grLogVal[5]		= gfSollDruckRampe;

void _CYCLIC TempLoggingCyclic(void)
{
	
	if (gSwitch_Up || gSwitch_Down || gSwitch_Drill)
	{
		bRec = 1;
		gbStartLogging = 1;
	}
	else
	{
		bRec = 0;
		gbStartLogging = 0;
	}
	
	if(usiCycleCnt++ == usiCycleScale)
	{
		usiCycleCnt	= 0;
		//check if data changed
		bChanged	= 0;
		
		for(i = 0; i < 7; i++)
		{
			if(grLogVal[i] != rLastValues[i])
			{
				bChanged = 1;
				break;
			}
		}
		
		if(bRec)
		{
			//Date and Time 
			strcpy(sValues[usiIndex],(STRING*) GetWITSDate((UDINT)&strCurrentDT));
			strcat(sValues[usiIndex], ";");
			strcat(sValues[usiIndex],(STRING*) GetWITSTime((UDINT)&strCurrentDT, 0));
			strcat(sValues[usiIndex], ";");
	
			//Actual values
			for(i = 0; i < 7; i++)
			{
				rLastValues[i]	= grLogVal[i];
				strcat(sValues[usiIndex], (STRING*)RealToString(grLogVal[i], 0));
				strcat(sValues[usiIndex], ";");
			}
			
			//add CRLF
			strcat(sValues[usiIndex], "\r\n");
			
			usiIndex ++;
		}
			
		
		if(usiIndex >= 15) //was >10
		{
			usiIndex = 0;
			if(!bWriteToFile || !gbStartLogging)	//if the write operation to file is not finished till now, skip the values
			{
				strcpy(sValues_Write, sValues[0]);
				uiStringOffset	= strlen(sValues[0]);
				for (i = 1; i < 20; i++)
				{
					strcat((STRING*)sValues_Write + uiStringOffset, sValues[i]);
					uiStringOffset += strlen(sValues[i]);
					if(uiStringOffset > 1600)
						break;
				}
				//memcpy((UDINT)sValues_Write, (UDINT)sValues, 1680);
				bWriteToFile	= 1;
				udiWriteLen		= strlen(sValues_Write);
		//		bRec			= 0;
			}
		}
	}
	
	switch (enLoggingState)
	{
		case WAIT_FOR_START:
			if (gbStartLogging)
			{
				enLoggingState	= CREATE_FILE;
				//create filename
				strcpy(sFilename, "Log_");
				strcpy(sTemp,(STRING*) GetWITSDate((UDINT)&strCurrentDT));
				strcat(sFilename, sTemp);
				strcat(sFilename, "_");
				strcpy(sTemp,(STRING*) GetWITSTime((UDINT)&strCurrentDT, 0));
				strcat(sFilename, sTemp);
				strcat(sFilename, ".csv");
				
				udiWriteOffset	= 0;	//start writing file from offset 0
				bWriteToFile	= 0;	//reset writing flag to 0, to ensure continuous measurement from beginning, otherwise it could be, that the first 
			}
			break;
		
		case CREATE_FILE:
			FUB_FileCreate.enable	= 1;
			FUB_FileCreate.pDevice	= (UDINT)"DEVICE1";
			FUB_FileCreate.pFile	= (UDINT)sFilename;
			
			FileCreate(&FUB_FileCreate);
			
			if(FUB_FileCreate.status == 65535)
			{}
			else if (FUB_FileCreate.status	== 0)
			{
				udiFileIdent	= FUB_FileCreate.ident;
				enLoggingState	= WAIT_FOR_WRITE;
			}
			else
			{
				udiError		= FUB_FileCreate.status;
				enLoggingState	= ERROR;
			}
			break;
		
		case WAIT_FOR_WRITE:
			if(bWriteToFile)
			{
				if((udiWriteOffset + udiWriteLen) > udiMaxFileSize)
				{
					udiWriteOffset	= 0;
					enLoggingState	= CLOSE_FILE;
				}
				else
					enLoggingState	= WRITE_FILE;
				//enLoggingState	= OPEN_FILE;
			}
			else if(!gbStartLogging)
				enLoggingState	= CLOSE_FILE;
			break;
		
		case OPEN_FILE:
			FUB_FileOpen.enable	= 1;
			FUB_FileOpen.mode	= FILE_W;
			FUB_FileOpen.pDevice= (UDINT)"DEVICE1";
			FUB_FileOpen.pFile	= (UDINT)sFilename;
		
			FileOpen(&FUB_FileOpen);
		
			if(FUB_FileOpen.status == 65535)
			{}
			else if (FUB_FileOpen.status == 0)
			{
				udiFileIdent	= FUB_FileOpen.ident;
				enLoggingState	= WRITE_FILE;
			}
			else
			{
				enLoggingState	= ERROR;
			}
			break;
				
		
		case WRITE_FILE:
			FUB_FileWrite.enable	= 1;
			FUB_FileWrite.ident		= udiFileIdent;
			FUB_FileWrite.pSrc		= (UDINT)sValues_Write;
			FUB_FileWrite.len		= udiWriteLen;
			FUB_FileWrite.offset	= udiWriteOffset;
			
			FileWrite(&FUB_FileWrite);
			
			if(FUB_FileWrite.status == 65535)
			{}
			else if(FUB_FileWrite.status == 0)
			{
				udiWriteOffset += udiWriteLen;
				bWriteToFile	= 0;
				enLoggingState	= WAIT_FOR_WRITE;
			}
			else
			{
				udiError		= FUB_FileWrite.status;
				enLoggingState	= ERROR;
			}
			break;
		
		case CLOSE_FILE:
			FUB_FileClose.enable	= 1;
			FUB_FileClose.ident		= udiFileIdent;
			
			FileClose(&FUB_FileClose);
			
			if(FUB_FileClose.status == 65535)
			{}
			else if(FUB_FileClose.status == 0)
			{
				enLoggingState	= WAIT_FOR_START;
			}
			else
			{
				udiError		= FUB_FileClose.status;
				enLoggingState	= ERROR;
			}
			break;
		
		case ERROR:
			if(strcmp(sErrorText, "") == 0)	//no Errortext
			{
				switch(udiError)
				{
					case 20700:		//fiERR_INVALID_PATH:
						strcpy(sErrorText, "Inavlid File path");
						break;
					case 20705:		//fiERR_EXIST:
						strcpy(sErrorText, "File with the same name already existing");
						break;
					case 20706:		//fiERR_ACCESS:
						strcpy(sErrorText, "Access to File denied");
						break;
					case 20708:		//fiERR_FILE_NOT_FOUND:
						strcpy(sErrorText, "File not existing");
						break;
					case 20709:		//fiERR_FILE_DEVICE:
						strcpy(sErrorText, "Illegal File Device");
						break;
					case 20721:		//fiERR_FILE_NOT_OPENED:
						strcpy(sErrorText, "File not open for write");
						break;
					case 20722:		//fiERR_INVALID_DIRECTORY:
						strcpy(sErrorText, "Invalid file directory");
						break;
					case 20723:		//fiERR_DIR_NOT_EXIST:
						strcpy(sErrorText, "Directory is not existing");
						break;
					case 20727:		//fiERR_NOT_ENOUGH_FREEMEM:
						strcpy(sErrorText, "Not enough free memory");
						break;
					default:	
						strcpy(sErrorText, "Errornumber not translated");
						break;
				}
			}
			
			if(bIgnoreErrors)
				bQuitError = 1;
		
			if(bQuitError)
			{
				bQuitError	= 0;
				strcpy(sLastError, sErrorText);
				strcpy(sErrorText, "");
				enLoggingState	= WAIT_FOR_START;
			}
			break;

	}
}
