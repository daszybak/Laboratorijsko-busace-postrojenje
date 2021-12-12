/* Automation Studio generated header file */
/* Do not edit ! */

#ifndef _ADSUTIL_
#define _ADSUTIL_
#ifdef __cplusplus
extern "C" 
{
#endif

#include <bur/plctypes.h>

#ifndef _IEC_CONST
#define _IEC_CONST _WEAK const
#endif

#ifdef _SG4
		#include "astime.h"
#endif
#ifdef _SG3
		#include "astime.h"
#endif
#ifdef _SGC
		#include "astime.h"
#endif

/* Constants */
#ifdef _REPLACE_CONST
 #define sHexTable "0123456789ABCDEF"
#else
 _IEC_CONST plcstring sHexTable[17] = "0123456789ABCDEF";
#endif







/* Prototyping of functions and function blocks */
unsigned long RealToString(float RealVal, unsigned char DecNum);
unsigned long GetWITSDate(unsigned long pTimeStruct);
unsigned long GetWITSTime(unsigned long pTimeStruct, plcbit AddMilliSeconds);
unsigned char GenDOWCRC(unsigned char* DataAdr, unsigned long Len);
unsigned long Hex2ASCII(unsigned char* InputValues, plcstring* OutputString, unsigned char Len);
unsigned long ExtractStr(unsigned long StartAdr, unsigned char MaxSigns, unsigned char EndSign, unsigned long OutputStr);


#ifdef __cplusplus
};
#endif
#endif /* _ADSUTIL_ */

