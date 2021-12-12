(********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Library: adsWITS0
 * File: adsWITS0.fun
 * Author: fasch_f
 * Created: August 11, 2010
 ********************************************************************
 * Functions and function blocks of library adsWITS0
 ********************************************************************)

FUNCTION witsInitRecordMin : UDINT (*function initialises a WITS Infotyp-Structure, without Well-Identifier-String*)
	VAR_INPUT
		RecType : STRING[3];
		pInfoStruct : UDINT;
		pDataArea : UDINT;
		AreaLen : UINT;
	END_VAR
END_FUNCTION

FUNCTION witsInitRecord : UDINT (*function initialises a WITS Infotyp-Structure*)
	VAR_INPUT
		RecType : STRING[3];
		pWellID : UDINT;
		pInfoStruct : UDINT;
		pDataArea : UDINT;
		AreaLen : UINT;
	END_VAR
END_FUNCTION

FUNCTION witsAddData : UDINT (*Adds data with ID and value to a WITS-Record*)
	VAR_INPUT
		pInfoStruct : UDINT;
		ItemID : STRING[3];
		pItemVal : UDINT;
	END_VAR
END_FUNCTION

FUNCTION witsUpdData : UDINT (*resets InfoStructure, so that data can updated by using witsAddData*)
	VAR_INPUT
		pInfoStruct : UDINT;
	END_VAR
END_FUNCTION
