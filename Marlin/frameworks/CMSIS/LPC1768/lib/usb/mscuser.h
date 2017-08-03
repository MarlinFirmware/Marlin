/*----------------------------------------------------------------------------
 *      U S B  -  K e r n e l
 *----------------------------------------------------------------------------
 *      Name:    MSCUSER.H
 *      Purpose: Mass Storage Class Custom User Definitions
 *      Version: V1.10
 *----------------------------------------------------------------------------
 *      This software is supplied "AS IS" without any warranties, express,
 *      implied or statutory, including but not limited to the implied
 *      warranties of fitness for purpose, satisfactory quality and
 *      noninfringement. Keil extends you a royalty-free right to reproduce
 *      and distribute executable files created using this software for use
 *      on NXP Semiconductors LPC family microcontroller devices only. Nothing 
 *      else gives you the right to use this software.
 *
 *      Copyright (c) 2005-2009 Keil Software.
 *---------------------------------------------------------------------------*/

#ifndef __MSCUSER_H__
#define __MSCUSER_H__


/* Max In/Out Packet Size */
#define MSC_MAX_PACKET  64
#define MSC_BLOCK_SIZE 512

/* MSC In/Out Endpoint Address */
#define MSC_EP_IN       0x85
#define MSC_EP_OUT      0x05

/* MSC Requests Callback Functions */
extern uint32_t MSC_Reset     (void);
extern uint32_t MSC_GetMaxLUN (void);

/* MSC Bulk Callback Functions */
extern void MSC_GetCBW (void);
extern void MSC_SetCSW (void);
extern void MSC_BulkIn (void);
extern void MSC_BulkOut(void);

enum class Sense_KEY : uint8_t {
  NO_SENSE,
  RECOVERED_ERROR,
  NOT_READY,
  MEDIUM_ERROR,
  HARDWARE_ERROR,
  ILLEGAL_REQUEST,
  UNIT_ATTENTION,
  DATA_PROTECT
};

enum class Sense_ASC : uint8_t {
  CANNOT_READ_MEDIUM = 0x30,
  MEDIUM_NOT_PRESENT = 0x3A
};

enum class Sense_ASCQ : uint8_t {
  // CANNOT_READ_MEDIUM
  UNKNOWN_FORMAT = 0x01,
  // MEDIUM_NOT_PRESENT
  REASON_UNKNOWN = 0x00,
  TRAY_CLOSED,
  TRAY_OPEN,
  LOADABLE,
  AUXILIARY_MEMORY_ACCESSIBLE
};


#endif  /* __MSCUSER_H__ */
