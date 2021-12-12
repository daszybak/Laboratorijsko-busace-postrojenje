(********************************************************************
 * COPYRIGHT -- Bernecker + Rainer
 ********************************************************************
 * Library: AsUSB
 * File: AsUSB.typ
 * Author: B+R
 ********************************************************************
 * Data types of library AsUSB
 ********************************************************************)
                                                                      
TYPE
    usbNode_typ			: STRUCT		(* statistics *)
	interfaceClass		: UINT;			(* Interface Class des USB Gerätes *)
	interfaceSubClass	: UINT;			(* Interface SubClass des USB Gerätes *)
	interfaceProtocol	: UINT;			(* Interface Protocol des USB Gerätes *)
	vendorId			: UINT;			(* Device Vendor ID *)
	productId			: UINT;			(* Device Product ID *)
	bcdDevice			: UINT;			(* USB device release version binary coded decimal *)
	ifName				: STRING[127];	(* USB IF Name *)
	END_STRUCT;
END_TYPE

TYPE
	usbDeviceDescr_typ	: STRUCT		(* statistics *)
    length				: USINT;		(* bLength *)
    descriptorType		: USINT;		(* bDescriptorType *)
    bcdUsb				: UINT;		    (* bcdUSB - USB release in BCD *)
    deviceClass			: USINT;	    (* bDeviceClass *)
    deviceSubClass		: USINT;		(* bDeviceSubClass *)
    deviceProtocol		: USINT;		(* bDeviceProtocol *)
    maxPacketSize0		: USINT;		(* bMaxPacketSize0 *)
    vendor				: UINT;			(* idVendor *)
    product				: UINT;			(* idProduct *)
    bcdDevice			: UINT;			(* bcdDevice - dev release in BCD *)
    manufacturerIndex	: USINT;		(* iManufacturer *)
    productIndex		: USINT;		(* iProduct *)
    serialNumberIndex	: USINT;		(* iSerialNumber *)
    numConfigurations	: USINT;		(* bNumConfigurations *)
	END_STRUCT;
END_TYPE

TYPE
	usbStringDescr_typ	: STRUCT		(* statistics *)
    length				: USINT;		(* bLength *)
    descriptorType		: USINT;	    (* bDescriptorType *)
    string				: STRING[1];    (* bString - variable len *)
	END_STRUCT;
END_TYPE


