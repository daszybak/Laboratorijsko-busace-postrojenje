(********************************************************************
 * COPYRIGHT (C) BERNECKER + RAINER, AUSTRIA, A-5142 EGGELSBERG
 ********************************************************************
 * Library: AsWeigh
 * File: AsWeigh.typ
 * Created: 15.02.2005
 ********************************************************************
 * Global data types of library AsWeigh
 ********************************************************************)
TYPE
	WGHscalePara_typ : STRUCT		(*parameter for taring and the standardization line*)
		tare_std	: DINT ;		(*taring weight (standardized value)*)
		ref1_raw	: DINT ;		(*raw value at reference point 1 of the standardization lines*)
		ref1_std	: DINT ;		(*standardized value at reference point 1 of the standardization lines*)
		ref2_raw	: DINT ;		(*raw value at reference point 2 of the standardization lines*)
		ref2_std	: DINT ;		(*standardized value at reference point 2 of the standardization lines*)
	END_STRUCT;
END_TYPE