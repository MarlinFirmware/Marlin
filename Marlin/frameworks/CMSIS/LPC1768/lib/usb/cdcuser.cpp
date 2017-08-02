/*----------------------------------------------------------------------------
 *      U S B  -  K e r n e l
 *----------------------------------------------------------------------------
 *      Name:    cdcuser.c
 *      Purpose: USB Communication Device Class User module
 *      Version: V1.10
 *----------------------------------------------------------------------------
 *      This software is supplied "AS IS" without any warranties, express,
 *      implied or statutory, including but not limited to the implied
 *      warranties of fitness for purpose, satisfactory quality and
 *      noninfringement. Keil extends you a royalty-free right to reproduce
 *      and distribute executable files created using this software for use
 *      on NXP Semiconductors LPC microcontroller devices only. Nothing else
 *      gives you the right to use this software.
 *
 * Copyright (c) 2009 Keil - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

extern "C" {
#include <lpc_types.h>
#include <debug_frmwrk.h>
}

#include "usb.h"
#include "usbhw.h"
#include "usbcfg.h"
#include "usbcore.h"
#include "cdc.h"
#include "cdcuser.h"

#include <HAL_LPC1768/serial.h>

unsigned char BulkBufIn[USB_CDC_BUFSIZE];            // Buffer to store USB IN  packet
unsigned char BulkBufOut[USB_CDC_BUFSIZE];            // Buffer to store USB OUT packet
unsigned char NotificationBuf[10];

CDC_LINE_CODING CDC_LineCoding = { 921600, 0, 0, 8 };
unsigned short CDC_DepInEmpty = 1;                   // Data IN EP is empty
unsigned short CDC_LineState = 0;
unsigned short CDC_SerialState = 0;


extern HalSerial usb_serial;
/*----------------------------------------------------------------------------
 write data to CDC_OutBuf
 *---------------------------------------------------------------------------*/
uint32_t CDC_WrOutBuf(const char *buffer, uint32_t *length) {
  uint32_t bytesToWrite, bytesWritten;

  // Write *length bytes
  bytesToWrite = *length;
  bytesWritten = bytesToWrite;

  while (bytesToWrite) {
    usb_serial.receive_buffer.write(*buffer++);           // Copy Data to buffer
    bytesToWrite--;
  }

  return (bytesWritten);
}

/*----------------------------------------------------------------------------
 check if character(s) are available at CDC_OutBuf
 *---------------------------------------------------------------------------*/
uint32_t CDC_OutBufAvailChar(uint32_t *availChar) {
  *availChar = usb_serial.transmit_buffer.available();
  return (0);
}
/* end Buffer handling */

/*----------------------------------------------------------------------------
 CDC Initialisation
 Initializes the data structures and serial port
 Parameters:   None
 Return Value: None
 *---------------------------------------------------------------------------*/
void CDC_Init() {
  CDC_DepInEmpty = 1;
}

/*----------------------------------------------------------------------------
 CDC SendEncapsulatedCommand Request Callback
 Called automatically on CDC SEND_ENCAPSULATED_COMMAND Request
 Parameters:   None                          (global SetupPacket and EP0Buf)
 Return Value: TRUE - Success, FALSE - Error
 *---------------------------------------------------------------------------*/
uint32_t CDC_SendEncapsulatedCommand(void) {

  return (TRUE);
}

/*----------------------------------------------------------------------------
 CDC GetEncapsulatedResponse Request Callback
 Called automatically on CDC Get_ENCAPSULATED_RESPONSE Request
 Parameters:   None                          (global SetupPacket and EP0Buf)
 Return Value: TRUE - Success, FALSE - Error
 *---------------------------------------------------------------------------*/
uint32_t CDC_GetEncapsulatedResponse(void) {

  /* ... add code to handle request */
  return (TRUE);
}

/*----------------------------------------------------------------------------
 CDC SetCommFeature Request Callback
 Called automatically on CDC Set_COMM_FATURE Request
 Parameters:   FeatureSelector
 Return Value: TRUE - Success, FALSE - Error
 *---------------------------------------------------------------------------*/
uint32_t CDC_SetCommFeature(unsigned short wFeatureSelector) {

  /* ... add code to handle request */
  return (TRUE);
}

/*----------------------------------------------------------------------------
 CDC GetCommFeature Request Callback
 Called automatically on CDC Get_COMM_FATURE Request
 Parameters:   FeatureSelector
 Return Value: TRUE - Success, FALSE - Error
 *---------------------------------------------------------------------------*/
uint32_t CDC_GetCommFeature(unsigned short wFeatureSelector) {

  /* ... add code to handle request */
  return (TRUE);
}

/*----------------------------------------------------------------------------
 CDC ClearCommFeature Request Callback
 Called automatically on CDC CLEAR_COMM_FATURE Request
 Parameters:   FeatureSelector
 Return Value: TRUE - Success, FALSE - Error
 *---------------------------------------------------------------------------*/
uint32_t CDC_ClearCommFeature(unsigned short wFeatureSelector) {

  /* ... add code to handle request */
  return (TRUE);
}

/*----------------------------------------------------------------------------
 CDC SetLineCoding Request Callback
 Called automatically on CDC SET_LINE_CODING Request
 Parameters:   none                    (global SetupPacket and EP0Buf)
 Return Value: TRUE - Success, FALSE - Error
 *---------------------------------------------------------------------------*/
uint32_t CDC_SetLineCoding(void) {

  CDC_LineCoding.dwDTERate = (EP0Buf[0] << 0) | (EP0Buf[1] << 8) | (EP0Buf[2] << 16) | (EP0Buf[3] << 24);
  CDC_LineCoding.bCharFormat = EP0Buf[4];
  CDC_LineCoding.bParityType = EP0Buf[5];
  CDC_LineCoding.bDataBits = EP0Buf[6];

  return (TRUE);
}

/*----------------------------------------------------------------------------
 CDC GetLineCoding Request Callback
 Called automatically on CDC GET_LINE_CODING Request
 Parameters:   None                         (global SetupPacket and EP0Buf)
 Return Value: TRUE - Success, FALSE - Error
 *---------------------------------------------------------------------------*/
uint32_t CDC_GetLineCoding(void) {

  EP0Buf[0] = (CDC_LineCoding.dwDTERate >> 0) & 0xFF;
  EP0Buf[1] = (CDC_LineCoding.dwDTERate >> 8) & 0xFF;
  EP0Buf[2] = (CDC_LineCoding.dwDTERate >> 16) & 0xFF;
  EP0Buf[3] = (CDC_LineCoding.dwDTERate >> 24) & 0xFF;
  EP0Buf[4] = CDC_LineCoding.bCharFormat;
  EP0Buf[5] = CDC_LineCoding.bParityType;
  EP0Buf[6] = CDC_LineCoding.bDataBits;

  return (TRUE);
}

/*----------------------------------------------------------------------------
 CDC SetControlLineState Request Callback
 Called automatically on CDC SET_CONTROL_LINE_STATE Request
 Parameters:   ControlSignalBitmap
 Return Value: TRUE - Success, FALSE - Error
 *---------------------------------------------------------------------------*/
uint32_t CDC_SetControlLineState(unsigned short wControlSignalBitmap) {
  CDC_LineState = wControlSignalBitmap;
  usb_serial.host_connected = wControlSignalBitmap > 0 ? true : false;
  return true;
}

/*----------------------------------------------------------------------------
 CDC SendBreak Request Callback
 Called automatically on CDC Set_COMM_FATURE Request
 Parameters:   0xFFFF  start of Break
 0x0000  stop  of Break
 0x####  Duration of Break
 Return Value: TRUE - Success, FALSE - Error
 *---------------------------------------------------------------------------*/
uint32_t CDC_SendBreak(unsigned short wDurationOfBreak) {

  /* ... add code to handle request */
  return (TRUE);
}

/*----------------------------------------------------------------------------
 CDC_BulkIn call on DataIn Request
 Parameters:   none
 Return Value: none
 *---------------------------------------------------------------------------*/
void CDC_BulkIn(void) {
  uint32_t numBytesAvail = usb_serial.transmit_buffer.available();

  if (numBytesAvail > 0) {
    numBytesAvail = numBytesAvail > (USB_CDC_BUFSIZE - 1) ? (USB_CDC_BUFSIZE - 1) : numBytesAvail;
    for(uint32_t i = 0; i < numBytesAvail; ++i) {
      BulkBufIn[i] = usb_serial.transmit_buffer.read(); //todo: optimise
    }
    USB_WriteEP(CDC_DEP_IN, &BulkBufIn[0], numBytesAvail);
  } else {
    CDC_DepInEmpty = 1;
  }
}

/*----------------------------------------------------------------------------
 CDC_BulkOut call on DataOut Request
 Parameters:   none
 Return Value: none
 *---------------------------------------------------------------------------*/
void CDC_BulkOut(void) {
  uint32_t numBytesRead = USB_ReadEP(CDC_DEP_OUT, &BulkBufOut[0]);
  CDC_WrOutBuf((char *) &BulkBufOut[0], &numBytesRead);
}

/*----------------------------------------------------------------------------
 Get the SERIAL_STATE as defined in usbcdc11.pdf, 6.3.5, Table 69.
 Parameters:   none
 Return Value: SerialState as defined in usbcdc11.pdf
 *---------------------------------------------------------------------------*/
unsigned short CDC_GetSerialState(void) {
  CDC_SerialState = CDC_LineState;
  //todo: detect buffer overrun
  return (CDC_SerialState);
}

/*----------------------------------------------------------------------------
 Send the SERIAL_STATE notification as defined in usbcdc11.pdf, 6.3.5.
 *---------------------------------------------------------------------------*/
void CDC_NotificationIn(void) {

  NotificationBuf[0] = 0xA1;                           // bmRequestType
  NotificationBuf[1] = CDC_NOTIFICATION_SERIAL_STATE;     // bNotification (SERIAL_STATE)
  NotificationBuf[2] = 0x00;                           // wValue
  NotificationBuf[3] = 0x00;
  NotificationBuf[4] = 0x00;                           // wIndex (Interface #, LSB first)
  NotificationBuf[5] = 0x00;
  NotificationBuf[6] = 0x02;                           // wLength (Data length = 2 bytes, LSB first)
  NotificationBuf[7] = 0x00;
  NotificationBuf[8] = (CDC_SerialState >> 0) & 0xFF;     // UART State Bitmap (16bits, LSB first)
  NotificationBuf[9] = (CDC_SerialState >> 8) & 0xFF;

  USB_WriteEP(CDC_CEP_IN, &NotificationBuf[0], 10);     // send notification
}
