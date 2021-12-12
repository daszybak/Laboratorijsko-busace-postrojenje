/* Automation Studio generated header file */
/* Do not edit ! */

#ifndef _ASUSB_
#define _ASUSB_
#ifdef __cplusplus
extern "C" 
{
#endif

#include <bur/plctypes.h>

#include <runtime.h>

#ifndef _IEC_CONST
#define _IEC_CONST _WEAK const
#endif

/* Constants */
#ifdef _REPLACE_CONST
 #define asusbERR_NULLPOINTER 32902U
 #define asusbERR_BUFSIZE 32901U
 #define asusbERR_USB_NOTFOUND 32900U
 #define asusb_USB_UNICODE_ENGLISH 1033U
 #define asusb_USB_DESCR_ENDPOINT 5U
 #define asusb_USB_DESCR_INTERFACE 4U
 #define asusb_USB_DESCR_STRING 3U
 #define asusb_USB_DESCR_CONFIG 2U
 #define asusb_USB_DESCR_DEVICE 1U
 #define asusb_USB_RT_VENDOR 64U
 #define asusb_USB_RT_CLASS 32U
 #define asusb_USB_RT_STANDARD 0U
 #define asusb_USB_RT_ENDPOINT 2U
 #define asusb_USB_RT_INTERFACE 1U
 #define asusb_USB_RT_DEVICE 0U
 #define asusb_DEVICENAMELENGTH 128U
 #define asusb_SUBCLASS_SCSI_COMMAND_SET 6U
 #define asusb_SUBCLASS_UFI_COMMAND_SET 4U
 #define asusb_SUBCLASS_HID_BOOT 1U
 #define asusb_SUBCLASS_PRINTER 1U
 #define asusb_CLASS_VENDOR_SPECIFIC 255U
 #define asusb_CLASS_HUB 9U
 #define asusb_CLASS_MASS_STORAGE 8U
 #define asusb_CLASS_PRINTER 7U
 #define asusb_CLASS_HID 3U
#else
 _IEC_CONST unsigned short asusbERR_NULLPOINTER = 32902U;
 _IEC_CONST unsigned short asusbERR_BUFSIZE = 32901U;
 _IEC_CONST unsigned short asusbERR_USB_NOTFOUND = 32900U;
 _IEC_CONST unsigned short asusb_USB_UNICODE_ENGLISH = 1033U;
 _IEC_CONST unsigned char asusb_USB_DESCR_ENDPOINT = 5U;
 _IEC_CONST unsigned char asusb_USB_DESCR_INTERFACE = 4U;
 _IEC_CONST unsigned char asusb_USB_DESCR_STRING = 3U;
 _IEC_CONST unsigned char asusb_USB_DESCR_CONFIG = 2U;
 _IEC_CONST unsigned char asusb_USB_DESCR_DEVICE = 1U;
 _IEC_CONST unsigned char asusb_USB_RT_VENDOR = 64U;
 _IEC_CONST unsigned char asusb_USB_RT_CLASS = 32U;
 _IEC_CONST unsigned char asusb_USB_RT_STANDARD = 0U;
 _IEC_CONST unsigned char asusb_USB_RT_ENDPOINT = 2U;
 _IEC_CONST unsigned char asusb_USB_RT_INTERFACE = 1U;
 _IEC_CONST unsigned char asusb_USB_RT_DEVICE = 0U;
 _IEC_CONST unsigned char asusb_DEVICENAMELENGTH = 128U;
 _IEC_CONST unsigned char asusb_SUBCLASS_SCSI_COMMAND_SET = 6U;
 _IEC_CONST unsigned char asusb_SUBCLASS_UFI_COMMAND_SET = 4U;
 _IEC_CONST unsigned char asusb_SUBCLASS_HID_BOOT = 1U;
 _IEC_CONST unsigned char asusb_SUBCLASS_PRINTER = 1U;
 _IEC_CONST unsigned char asusb_CLASS_VENDOR_SPECIFIC = 255U;
 _IEC_CONST unsigned char asusb_CLASS_HUB = 9U;
 _IEC_CONST unsigned char asusb_CLASS_MASS_STORAGE = 8U;
 _IEC_CONST unsigned char asusb_CLASS_PRINTER = 7U;
 _IEC_CONST unsigned char asusb_CLASS_HID = 3U;
#endif




/* Datatypes and datatypes of function blocks */
typedef struct usbNode_typ
{	unsigned short interfaceClass;
	unsigned short interfaceSubClass;
	unsigned short interfaceProtocol;
	unsigned short vendorId;
	unsigned short productId;
	unsigned short bcdDevice;
	plcstring ifName[128];
} usbNode_typ;

typedef struct usbDeviceDescr_typ
{	unsigned char length;
	unsigned char descriptorType;
	unsigned short bcdUsb;
	unsigned char deviceClass;
	unsigned char deviceSubClass;
	unsigned char deviceProtocol;
	unsigned char maxPacketSize0;
	unsigned short vendor;
	unsigned short product;
	unsigned short bcdDevice;
	unsigned char manufacturerIndex;
	unsigned char productIndex;
	unsigned char serialNumberIndex;
	unsigned char numConfigurations;
} usbDeviceDescr_typ;

typedef struct usbStringDescr_typ
{	unsigned char length;
	unsigned char descriptorType;
	plcstring string[2];
} usbStringDescr_typ;

typedef struct UsbNodeListGet
{
	/* VAR_INPUT (analog) */
	unsigned long pBuffer;
	unsigned long bufferSize;
	unsigned short filterInterfaceClass;
	unsigned short filterInterfaceSubClass;
	/* VAR_OUTPUT (analog) */
	unsigned short status;
	unsigned long allNodes;
	unsigned long listNodes;
	unsigned long attachDetachCount;
	/* VAR (analog) */
	unsigned short i_state;
	unsigned short i_result;
	unsigned long i_tmp;
	/* VAR_INPUT (digital) */
	plcbit enable;
} UsbNodeListGet_typ;

typedef struct UsbNodeGet
{
	/* VAR_INPUT (analog) */
	unsigned long nodeId;
	unsigned long pBuffer;
	unsigned long bufferSize;
	/* VAR_OUTPUT (analog) */
	unsigned short status;
	/* VAR (analog) */
	unsigned short i_state;
	unsigned short i_result;
	unsigned long i_tmp;
	/* VAR_INPUT (digital) */
	plcbit enable;
} UsbNodeGet_typ;

typedef struct UsbDescriptorGet
{
	/* VAR_INPUT (analog) */
	unsigned long nodeId;
	unsigned char requestType;
	unsigned char descriptorType;
	unsigned char descriptorIndex;
	unsigned short languageId;
	unsigned long pBuffer;
	unsigned short bufferSize;
	/* VAR_OUTPUT (analog) */
	unsigned short status;
	unsigned short actSize;
	/* VAR (analog) */
	unsigned short i_state;
	unsigned short i_result;
	unsigned long i_tmp;
	/* VAR_INPUT (digital) */
	plcbit enable;
} UsbDescriptorGet_typ;



/* Prototyping of functions and function blocks */
void UsbNodeListGet(struct UsbNodeListGet* inst);
void UsbNodeGet(struct UsbNodeGet* inst);
void UsbDescriptorGet(struct UsbDescriptorGet* inst);


#ifdef __cplusplus
};
#endif
#endif /* _ASUSB_ */

                                                           
