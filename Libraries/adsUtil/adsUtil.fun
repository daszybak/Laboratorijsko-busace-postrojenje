(********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Library: adsUtil
 * File: adsUtil.fun
 * Author: fasch_f
 * Created: August 11, 2010
 ********************************************************************
 * Functions and function blocks of library adsUtil
 ********************************************************************)

FUNCTION RealToString : UDINT (*Converts a real to string with adjustable decimal places*)
	VAR_INPUT
		RealVal : REAL;
		DecNum : USINT;
	END_VAR
END_FUNCTION

FUNCTION GetWITSDate : UDINT (*Formats a RTCtime_typ structure to a string where date is formated for a WITS-REcord*)
	VAR_INPUT
		pTimeStruct : UDINT;
	END_VAR
END_FUNCTION

FUNCTION GetWITSTime : UDINT (*Convert a TimeStruct_typ to a string where time is stored in WITS-Time-format*)
	VAR_INPUT
		pTimeStruct : UDINT;
		AddMilliSeconds : BOOL; (*add milliseconds if flag set*)
	END_VAR
END_FUNCTION

FUNCTION GenDOWCRC : USINT (*Generates a CRC for Maxim-Dallas OneWire ID Chip*)
	VAR_INPUT
		DataAdr : REFERENCE TO USINT;
		Len : UDINT;
	END_VAR
END_FUNCTION

FUNCTION Hex2ASCII : UDINT (*TODO: Add your comment here*)
	VAR_INPUT
		InputValues : REFERENCE TO USINT;
		OutputString : STRING[80];
		Len : USINT;
	END_VAR
END_FUNCTION

FUNCTION ExtractStr : UDINT (*Extracts a string of a longer string until a specified sign*)
	VAR_INPUT
		StartAdr : UDINT;
		MaxSigns : USINT;
		EndSign : USINT;
		OutputStr : UDINT;
	END_VAR
END_FUNCTION
