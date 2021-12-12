(********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Library: adsWITS0
 * File: adsWITS0.typ
 * Author: fasch_f
 * Created: August 11, 2010
 ********************************************************************
 * Data types of library adsWITS0
 ********************************************************************)

TYPE
	RECORD_INFO_TYP : 	STRUCT 
		recType : STRING[3];
		recNum : UINT := 0; (*actual number of records*)
		recSize : UINT := 0; (*record size in bytes*)
		wellIDSize : UINT := 0; (*size of the first record entry (Record-ID)*)
		pRecData : UDINT; (*pointer to record data*)
		recMaxSize : UINT := 0; (*maximum size of WITS-Record*)
		recActSize : UINT := 0; (*size of record with actual data*)
	END_STRUCT;
	DORECORD_INFO_TYP : 	STRUCT 
		recType : STRING[3];
		recNum : UINT;
		recSize : UDINT;
		wellIDSize : UINT;
		pRecData : UDINT;
		ident : USINT;
	END_STRUCT;
END_TYPE
