/* Automation Studio generated header file */
/* Do not edit ! */

#ifndef _ADSWITS0_
#define _ADSWITS0_
#ifdef __cplusplus
extern "C" 
{
#endif

#include <bur/plctypes.h>

#ifndef _IEC_CONST
#define _IEC_CONST _WEAK const
#endif

/* Constants */
#ifdef _REPLACE_CONST
 #define WITS_PARERROR 10001U
 #define WITS_NOAREA 10002U
 #define WITS_RECORD_FULL 10003U
 #define WITS_REC_GENTIME "01"
 #define WITS_REC_DRILDEPTH "02"
 #define WITS_ITEM_WELLID "01"
 #define WITS_ITEM_DATE "05"
 #define WITS_ITEM_TIME "06"
 #define WITS_ITEM_BLOCKPOS "12"
 #define WITS_ITEM_RATEOFPEN "13"
 #define WITS_ITEM_ROTTORQUE_AVG "18"
 #define WITS_ITEM_ROTSPEED_AVG "20"
 #define WITS_ITEM_SPARE1 "41"
 #define WITS_ITEM_SPARE2 "42"
 #define WITS_ITEM_SPARE3 "43"
 #define WITS_ITEM_SPARE4 "44"
#else
 _IEC_CONST unsigned long WITS_PARERROR = 10001U;
 _IEC_CONST unsigned long WITS_NOAREA = 10002U;
 _IEC_CONST unsigned long WITS_RECORD_FULL = 10003U;
 _IEC_CONST plcstring WITS_REC_GENTIME[4] = "01";
 _IEC_CONST plcstring WITS_REC_DRILDEPTH[4] = "02";
 _IEC_CONST plcstring WITS_ITEM_WELLID[4] = "01";
 _IEC_CONST plcstring WITS_ITEM_DATE[4] = "05";
 _IEC_CONST plcstring WITS_ITEM_TIME[4] = "06";
 _IEC_CONST plcstring WITS_ITEM_BLOCKPOS[4] = "12";
 _IEC_CONST plcstring WITS_ITEM_RATEOFPEN[4] = "13";
 _IEC_CONST plcstring WITS_ITEM_ROTTORQUE_AVG[4] = "18";
 _IEC_CONST plcstring WITS_ITEM_ROTSPEED_AVG[4] = "20";
 _IEC_CONST plcstring WITS_ITEM_SPARE1[4] = "41";
 _IEC_CONST plcstring WITS_ITEM_SPARE2[4] = "42";
 _IEC_CONST plcstring WITS_ITEM_SPARE3[4] = "43";
 _IEC_CONST plcstring WITS_ITEM_SPARE4[4] = "44";
#endif




/* Datatypes and datatypes of function blocks */
typedef struct RECORD_INFO_TYP
{	plcstring recType[4];
	unsigned short recNum;
	unsigned short recSize;
	unsigned short wellIDSize;
	unsigned long pRecData;
	unsigned short recMaxSize;
	unsigned short recActSize;
} RECORD_INFO_TYP;

typedef struct DORECORD_INFO_TYP
{	plcstring recType[4];
	unsigned short recNum;
	unsigned long recSize;
	unsigned short wellIDSize;
	unsigned long pRecData;
	unsigned char ident;
} DORECORD_INFO_TYP;



/* Prototyping of functions and function blocks */
unsigned long witsInitRecordMin(plcstring* RecType, unsigned long pInfoStruct, unsigned long pDataArea, unsigned short AreaLen);
unsigned long witsInitRecord(plcstring* RecType, unsigned long pWellID, unsigned long pInfoStruct, unsigned long pDataArea, unsigned short AreaLen);
unsigned long witsAddData(unsigned long pInfoStruct, plcstring* ItemID, unsigned long pItemVal);
unsigned long witsUpdData(unsigned long pInfoStruct);


#ifdef __cplusplus
};
#endif
#endif /* _ADSWITS0_ */

