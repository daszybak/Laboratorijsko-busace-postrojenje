(********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: AsUSB
 * File: AsUSB.fun
 * Author: B+R
 ********************************************************************
 * Functions and function blocks of library AsUSB
 ********************************************************************)
                                                                      
FUNCTION_BLOCK UsbNodeListGet			(* get USB node list *)
	VAR_INPUT
		enable			: BOOL;			(* fub enable *)
		pBuffer			: UDINT;		(* pointer to input buffer *)
		bufferSize		: UDINT;		(* max length of input buffer *)
		filterInterfaceClass : UINT;	(* filter for interface class *)
		filterInterfaceSubClass : UINT;	(* filter for interface subclass *)
	END_VAR
	VAR_OUTPUT
		status			: UINT;			(* status *)
		allNodes		: UDINT;		(* all usb nodes on target *)
		listNodes		: UDINT;		(* usb nodes in the list *)
		attachDetachCount:UDINT;		(* attach/deatch actions *)
	END_VAR
	VAR
        i_state			: UINT;			(* internal variable *)
		i_result		: UINT;			(* internal variable *)
        i_tmp			: UDINT;		(* internal variable *)
	END_VAR
END_FUNCTION_BLOCK
FUNCTION_BLOCK UsbNodeGet 				(* get USB node description *)
	VAR_INPUT
		enable			: BOOL;			(* fub enable *)
		nodeId			: UDINT;		(* unique node identifier of USBNodeListGet *)
		pBuffer			: UDINT;		(* pointer to input buffer *)
		bufferSize		: UDINT;		(* max length of input buffer *)
	END_VAR
	VAR_OUTPUT
		status			: UINT;			(* status *)
	END_VAR
	VAR
        i_state			: UINT;			(* internal variable *)
		i_result		: UINT;			(* internal variable *)
        i_tmp			: UDINT;		(* internal variable *)
	END_VAR
END_FUNCTION_BLOCK
FUNCTION_BLOCK UsbDescriptorGet			(* get USB node descriptor *)
	VAR_INPUT
		enable			: BOOL;			(* fub enable *)
		nodeId			: UDINT;		(* unique node identifier of USBNodeListGet *)
		requestType		: USINT;		(* reqeust type *)
		descriptorType	: USINT;		(* descriptor type *)
		descriptorIndex	: USINT;		(* descriptor index *)
		languageId		: UINT;			(* language unicode *)
		pBuffer			: UDINT;		(* pointer to input buffer *)
		bufferSize		: UINT;			(* max length of input buffer *)
	END_VAR
	VAR_OUTPUT
		status			: UINT;			(* status *)
		actSize			: UINT;			(* length of received data *)
	END_VAR
	VAR
        i_state			: UINT;			(* internal variable *)
		i_result		: UINT;			(* internal variable *)
        i_tmp			: UDINT;		(* internal variable *)
	END_VAR
END_FUNCTION_BLOCK

