(********************************************************************
 * COPYRIGHT (C) BERNECKER + RAINER, AUSTRIA, A-5142 EGGELSBERG
 ********************************************************************
 * Library: AsCont
 * File: AsCont.fun
 * Created: 11.11.2003
 ********************************************************************
 * Functions and function blocks of library AsCont
 ********************************************************************)
FUNCTION_BLOCK AO_4_20 			(*converts from 0 .. 32767 for 4..20mA output*)
	VAR_INPUT
		enable	:BOOL;			(*enables execution*)
		IN	:INT;				(*value to convert*)
	END_VAR
	VAR_OUTPUT
		OUT	:INT;				(*converted value*)
	END_VAR
END_FUNCTION_BLOCK
FUNCTION_BLOCK AI_4_20 			(*converts value from 4..20mA input to 0 .. 32767*)
	VAR_INPUT
		enable	:BOOL;			(*enables execution*)
		IN	:INT;				(*value to convert*)
	END_VAR
	VAR_OUTPUT
		OUT	:INT;				(*converted value*)
	END_VAR
END_FUNCTION_BLOCK
