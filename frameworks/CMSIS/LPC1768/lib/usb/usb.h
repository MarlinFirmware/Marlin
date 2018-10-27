/*----------------------------------------------------------------------------
 *      U S B  -  K e r n e l
 *----------------------------------------------------------------------------
 * Name:    usb.h
 * Purpose: USB Definitions
 * Version: V1.20
 *----------------------------------------------------------------------------
 *      This software is supplied "AS IS" without any warranties, express,
 *      implied or statutory, including but not limited to the implied
 *      warranties of fitness for purpose, satisfactory quality and
 *      noninfringement. Keil extends you a royalty-free right to reproduce
 *      and distribute executable files created using this software for use
 *      on NXP Semiconductors LPC family microcontroller devices only. Nothing
 *      else gives you the right to use this software.
 *
 * Copyright (c) 2009 Keil - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#ifndef __USB_H__
#define __USB_H__
extern "C" {
#include "lpc_types.h"
}
#if defined   (  __GNUC__  )
#define __packed __attribute__((__packed__))
#endif

#if defined     (  __CC_ARM  )
typedef __packed union {
#elif defined   (  __GNUC__  )
typedef union __packed {
#elif defined   (  __IAR_SYSTEMS_ICC__  )
#pragma pack(1)
typedef union {
#endif
  uint16_t W;
#if defined     (  __CC_ARM  )
  __packed struct {
#elif defined   (  __GNUC__  )
  struct __packed {
#elif defined   (  __IAR_SYSTEMS_ICC__  )
#pragma pack(1)
  struct {
#endif
    uint8_t L;
    uint8_t H;
  } WB;
#ifdef __IAR_SYSTEMS_ICC__
#pragma pack()
#endif
} WORD_BYTE;
#ifdef __IAR_SYSTEMS_ICC__
#pragma pack()
#endif


/* bmRequestType.Dir */
#define REQUEST_HOST_TO_DEVICE     0
#define REQUEST_DEVICE_TO_HOST     1

/* bmRequestType.Type */
#define REQUEST_STANDARD           0
#define REQUEST_CLASS              1
#define REQUEST_VENDOR             2
#define REQUEST_RESERVED           3

/* bmRequestType.Recipient */
#define REQUEST_TO_DEVICE          0
#define REQUEST_TO_INTERFACE       1
#define REQUEST_TO_ENDPOINT        2
#define REQUEST_TO_OTHER           3

/* bmRequestType Definition */
#if defined     (  __CC_ARM  )
typedef __packed union _REQUEST_TYPE {
#elif defined   (  __GNUC__  )
typedef union __packed _REQUEST_TYPE {
#elif defined   (  __IAR_SYSTEMS_ICC__  )
#pragma pack(1)
typedef union _REQUEST_TYPE {
#endif
#if defined     (  __CC_ARM  )
  __packed struct _BM {
#elif defined   (  __GNUC__  )
  struct __packed _BM {
#elif defined   (  __IAR_SYSTEMS_ICC__  )
#pragma pack(1)
  struct _BM {
#endif
    uint8_t Recipient : 5;
    uint8_t Type      : 2;
    uint8_t Dir       : 1;
  } BM;
#ifdef __IAR_SYSTEMS_ICC__
#pragma pack()
#endif
  uint8_t B;
} REQUEST_TYPE;
#ifdef __IAR_SYSTEMS_ICC__
#pragma pack()
#endif

/* USB Standard Request Codes */
#define USB_REQUEST_GET_STATUS                 0
#define USB_REQUEST_CLEAR_FEATURE              1
#define USB_REQUEST_SET_FEATURE                3
#define USB_REQUEST_SET_ADDRESS                5
#define USB_REQUEST_GET_DESCRIPTOR             6
#define USB_REQUEST_SET_DESCRIPTOR             7
#define USB_REQUEST_GET_CONFIGURATION          8
#define USB_REQUEST_SET_CONFIGURATION          9
#define USB_REQUEST_GET_INTERFACE              10
#define USB_REQUEST_SET_INTERFACE              11
#define USB_REQUEST_SYNC_FRAME                 12

/* USB GET_STATUS Bit Values */
#define USB_GETSTATUS_SELF_POWERED             0x01
#define USB_GETSTATUS_REMOTE_WAKEUP            0x02
#define USB_GETSTATUS_ENDPOINT_STALL           0x01

/* USB Standard Feature selectors */
#define USB_FEATURE_ENDPOINT_STALL             0
#define USB_FEATURE_REMOTE_WAKEUP              1

/* USB Default Control Pipe Setup Packet */
#if defined     (  __CC_ARM  )
typedef __packed struct _USB_SETUP_PACKET {
#elif defined   (  __GNUC__  )
typedef struct __packed _USB_SETUP_PACKET {
#elif defined   (  __IAR_SYSTEMS_ICC__  )
#pragma pack(1)
typedef struct _USB_SETUP_PACKET {
#endif
  REQUEST_TYPE bmRequestType;
  uint8_t         bRequest;
  WORD_BYTE    wValue;
  WORD_BYTE    wIndex;
  uint16_t         wLength;
} USB_SETUP_PACKET;
#ifdef __IAR_SYSTEMS_ICC__
#pragma pack()
#endif


/* USB Descriptor Types */
#define USB_DEVICE_DESCRIPTOR_TYPE             1
#define USB_CONFIGURATION_DESCRIPTOR_TYPE      2
#define USB_STRING_DESCRIPTOR_TYPE             3
#define USB_INTERFACE_DESCRIPTOR_TYPE          4
#define USB_ENDPOINT_DESCRIPTOR_TYPE           5
#define USB_DEVICE_QUALIFIER_DESCRIPTOR_TYPE   6
#define USB_OTHER_SPEED_CONFIG_DESCRIPTOR_TYPE 7
#define USB_INTERFACE_POWER_DESCRIPTOR_TYPE    8
#define USB_OTG_DESCRIPTOR_TYPE                     9
#define USB_DEBUG_DESCRIPTOR_TYPE                  10
#define USB_INTERFACE_ASSOCIATION_DESCRIPTOR_TYPE  11

/* USB Device Classes */
#define USB_DEVICE_CLASS_RESERVED              0x00
#define USB_DEVICE_CLASS_AUDIO                 0x01
#define USB_DEVICE_CLASS_COMMUNICATIONS        0x02
#define USB_DEVICE_CLASS_HUMAN_INTERFACE       0x03
#define USB_DEVICE_CLASS_MONITOR               0x04
#define USB_DEVICE_CLASS_PHYSICAL_INTERFACE    0x05
#define USB_DEVICE_CLASS_POWER                 0x06
#define USB_DEVICE_CLASS_PRINTER               0x07
#define USB_DEVICE_CLASS_STORAGE               0x08
#define USB_DEVICE_CLASS_HUB                   0x09
#define USB_DEVICE_CLASS_MISCELLANEOUS         0xEF
#define USB_DEVICE_CLASS_VENDOR_SPECIFIC       0xFF

/* bmAttributes in Configuration Descriptor */
#define USB_CONFIG_POWERED_MASK                0x40
#define USB_CONFIG_BUS_POWERED                 0x80
#define USB_CONFIG_SELF_POWERED                0xC0
#define USB_CONFIG_REMOTE_WAKEUP               0x20

/* bMaxPower in Configuration Descriptor */
#define USB_CONFIG_POWER_MA(mA)                ((mA)/2)

/* bEndpointAddress in Endpoint Descriptor */
#define USB_ENDPOINT_DIRECTION_MASK            0x80
#define USB_ENDPOINT_OUT(addr)                 ((addr) | 0x00)
#define USB_ENDPOINT_IN(addr)                  ((addr) | 0x80)

/* bmAttributes in Endpoint Descriptor */
#define USB_ENDPOINT_TYPE_MASK                 0x03
#define USB_ENDPOINT_TYPE_CONTROL              0x00
#define USB_ENDPOINT_TYPE_ISOCHRONOUS          0x01
#define USB_ENDPOINT_TYPE_BULK                 0x02
#define USB_ENDPOINT_TYPE_INTERRUPT            0x03
#define USB_ENDPOINT_SYNC_MASK                 0x0C
#define USB_ENDPOINT_SYNC_NO_SYNCHRONIZATION   0x00
#define USB_ENDPOINT_SYNC_ASYNCHRONOUS         0x04
#define USB_ENDPOINT_SYNC_ADAPTIVE             0x08
#define USB_ENDPOINT_SYNC_SYNCHRONOUS          0x0C
#define USB_ENDPOINT_USAGE_MASK                0x30
#define USB_ENDPOINT_USAGE_DATA                0x00
#define USB_ENDPOINT_USAGE_FEEDBACK            0x10
#define USB_ENDPOINT_USAGE_IMPLICIT_FEEDBACK   0x20
#define USB_ENDPOINT_USAGE_RESERVED            0x30

/* USB Standard Device Descriptor */
#if defined     (  __CC_ARM  )
typedef __packed struct _USB_DEVICE_DESCRIPTOR {
#elif defined   (  __GNUC__  )
typedef struct __packed _USB_DEVICE_DESCRIPTOR {
#elif defined   (  __IAR_SYSTEMS_ICC__  )
#pragma pack(1)
typedef struct _USB_DEVICE_DESCRIPTOR {
#endif
  uint8_t  bLength;
  uint8_t  bDescriptorType;
  uint16_t  bcdUSB;
  uint8_t  bDeviceClass;
  uint8_t  bDeviceSubClass;
  uint8_t  bDeviceProtocol;
  uint8_t  bMaxPacketSize0;
  uint16_t  idVendor;
  uint16_t  idProduct;
  uint16_t  bcdDevice;
  uint8_t  iManufacturer;
  uint8_t  iProduct;
  uint8_t  iSerialNumber;
  uint8_t  bNumConfigurations;
} USB_DEVICE_DESCRIPTOR;
#ifdef __IAR_SYSTEMS_ICC__
#pragma pack()
#endif

/* USB 2.0 Device Qualifier Descriptor */
#if defined     (  __CC_ARM  )
typedef __packed struct _USB_DEVICE_QUALIFIER_DESCRIPTOR {
#elif defined   (  __GNUC__  )
typedef struct __packed _USB_DEVICE_QUALIFIER_DESCRIPTOR {
#elif defined   (  __IAR_SYSTEMS_ICC__  )
#pragma pack(1)
typedef struct _USB_DEVICE_QUALIFIER_DESCRIPTOR {
#endif
  uint8_t  bLength;
  uint8_t  bDescriptorType;
  uint16_t  bcdUSB;
  uint8_t  bDeviceClass;
  uint8_t  bDeviceSubClass;
  uint8_t  bDeviceProtocol;
  uint8_t  bMaxPacketSize0;
  uint8_t  bNumConfigurations;
  uint8_t  bReserved;
} USB_DEVICE_QUALIFIER_DESCRIPTOR;
#ifdef __IAR_SYSTEMS_ICC__
#pragma pack()
#endif

#if defined     (  __CC_ARM  )
typedef __packed struct _USB_CONFIGURATION_DESCRIPTOR {
#elif defined   (  __GNUC__  )
typedef struct __packed _USB_CONFIGURATION_DESCRIPTOR {
#elif defined   (  __IAR_SYSTEMS_ICC__  )
#pragma pack(1)
typedef struct _USB_CONFIGURATION_DESCRIPTOR {
#endif
  uint8_t  bLength;
  uint8_t  bDescriptorType;
  uint16_t  wTotalLength;
  uint8_t  bNumInterfaces;
  uint8_t  bConfigurationValue;
  uint8_t  iConfiguration;
  uint8_t  bmAttributes;
  uint8_t  bMaxPower;
} USB_CONFIGURATION_DESCRIPTOR;
#ifdef __IAR_SYSTEMS_ICC__
#pragma pack()
#endif

/* USB Standard Interface Descriptor */
#if defined     (  __CC_ARM  )
typedef __packed struct _USB_INTERFACE_DESCRIPTOR {
#elif defined   (  __GNUC__  )
typedef struct __packed _USB_INTERFACE_DESCRIPTOR {
#elif defined   (  __IAR_SYSTEMS_ICC__  )
#pragma pack(1)
typedef struct _USB_INTERFACE_DESCRIPTOR {
#endif
  uint8_t  bLength;
  uint8_t  bDescriptorType;
  uint8_t  bInterfaceNumber;
  uint8_t  bAlternateSetting;
  uint8_t  bNumEndpoints;
  uint8_t  bInterfaceClass;
  uint8_t  bInterfaceSubClass;
  uint8_t  bInterfaceProtocol;
  uint8_t  iInterface;
} USB_INTERFACE_DESCRIPTOR;
#ifdef __IAR_SYSTEMS_ICC__
#pragma pack()
#endif

/* USB Standard Endpoint Descriptor */
#if defined     (  __CC_ARM  )
typedef __packed struct _USB_ENDPOINT_DESCRIPTOR {
#elif defined   (  __GNUC__  )
typedef struct __packed _USB_ENDPOINT_DESCRIPTOR {
#elif defined   (  __IAR_SYSTEMS_ICC__  )
#pragma pack(1)
typedef struct _USB_ENDPOINT_DESCRIPTOR {
#endif
  uint8_t  bLength;
  uint8_t  bDescriptorType;
  uint8_t  bEndpointAddress;
  uint8_t  bmAttributes;
  uint16_t  wMaxPacketSize;
  uint8_t  bInterval;
} USB_ENDPOINT_DESCRIPTOR;
#ifdef __IAR_SYSTEMS_ICC__
#pragma pack()
#endif

/* USB String Descriptor */
#if defined     (  __CC_ARM  )
typedef __packed struct _USB_STRING_DESCRIPTOR {
#elif defined   (  __GNUC__  )
typedef struct __packed _USB_STRING_DESCRIPTOR {
#elif defined   (  __IAR_SYSTEMS_ICC__  )
#pragma pack(1)
typedef struct _USB_STRING_DESCRIPTOR {
#endif
  uint8_t  bLength;
  uint8_t  bDescriptorType;
  uint16_t  bString/*[]*/;
} USB_STRING_DESCRIPTOR;
#ifdef __IAR_SYSTEMS_ICC__
#pragma pack()
#endif

/* USB Common Descriptor */
#if defined     (  __CC_ARM  )
typedef __packed struct _USB_COMMON_DESCRIPTOR {
#elif defined   (  __GNUC__  )
typedef struct __packed _USB_COMMON_DESCRIPTOR {
#elif defined   (  __IAR_SYSTEMS_ICC__  )
#pragma pack(1)
typedef struct _USB_COMMON_DESCRIPTOR {
#endif
  uint8_t  bLength;
  uint8_t  bDescriptorType;
} USB_COMMON_DESCRIPTOR;
#ifdef __IAR_SYSTEMS_ICC__
#pragma pack()
#endif



#endif  /* __USB_H__ */
