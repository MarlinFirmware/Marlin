/*----------------------------------------------------------------------------
 *      U S B  -  K e r n e l
 *----------------------------------------------------------------------------
 *      Name:    MSC.H
 *      Purpose: USB Mass Storage Class Definitions
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

#ifndef __MSC_H__
#define __MSC_H__


#if defined   (  __GNUC__  )
#define __packed __attribute__((__packed__))
#endif


/* MSC Subclass Codes */
#define MSC_SUBCLASS_RBC                0x01
#define MSC_SUBCLASS_SFF8020I_MMC2      0x02
#define MSC_SUBCLASS_QIC157             0x03
#define MSC_SUBCLASS_UFI                0x04
#define MSC_SUBCLASS_SFF8070I           0x05
#define MSC_SUBCLASS_SCSI               0x06

/* MSC Protocol Codes */
#define MSC_PROTOCOL_CBI_INT            0x00
#define MSC_PROTOCOL_CBI_NOINT          0x01
#define MSC_PROTOCOL_BULK_ONLY          0x50


/* MSC Request Codes */
#define MSC_REQUEST_RESET               0xFF
#define MSC_REQUEST_GET_MAX_LUN         0xFE


/* MSC Bulk-only Stage */
#define MSC_BS_CBW                      0       /* Command Block Wrapper */
#define MSC_BS_DATA_OUT                 1       /* Data Out Phase */
#define MSC_BS_DATA_IN                  2       /* Data In Phase */
#define MSC_BS_DATA_IN_LAST             3       /* Data In Last Phase */
#define MSC_BS_DATA_IN_LAST_STALL       4       /* Data In Last Phase with Stall */
#define MSC_BS_CSW                      5       /* Command Status Wrapper */
#define MSC_BS_ERROR                    6       /* Error */


/* Bulk-only Command Block Wrapper */
#if defined     (  __CC_ARM  )
typedef __packed struct _MSC_CBW {
#elif defined   (  __GNUC__  )
typedef struct __packed _MSC_CBW {
#elif defined   (  __IAR_SYSTEMS_ICC__  )
typedef __packed struct _MSC_CBW {
#endif
  uint32_t dSignature;
  uint32_t dTag;
  uint32_t dDataLength;
  uint8_t  bmFlags;
  uint8_t  bLUN;
  uint8_t  bCBLength;
  uint8_t  CB[16];
} MSC_CBW;

/* Bulk-only Command Status Wrapper */
#if defined     (  __CC_ARM  )
typedef __packed struct _MSC_CSW {
#elif defined   (  __GNUC__  )
typedef struct __packed _MSC_CSW {
#elif defined   (  __IAR_SYSTEMS_ICC__  )
typedef __packed struct _MSC_CSW {
#endif
  uint32_t dSignature;
  uint32_t dTag;
  uint32_t dDataResidue;
  uint8_t  bStatus;
} MSC_CSW;

#define MSC_CBW_Signature               0x43425355
#define MSC_CSW_Signature               0x53425355


/* CSW Status Definitions */
#define CSW_CMD_PASSED                  0x00
#define CSW_CMD_FAILED                  0x01
#define CSW_PHASE_ERROR                 0x02


/* SCSI Commands */
#define SCSI_TEST_UNIT_READY            0x00
#define SCSI_REQUEST_SENSE              0x03
#define SCSI_FORMAT_UNIT                0x04
#define SCSI_INQUIRY                    0x12
#define SCSI_MODE_SELECT6               0x15
#define SCSI_MODE_SENSE6                0x1A
#define SCSI_START_STOP_UNIT            0x1B
#define SCSI_MEDIA_REMOVAL              0x1E
#define SCSI_READ_FORMAT_CAPACITIES     0x23
#define SCSI_READ_CAPACITY              0x25
#define SCSI_READ10                     0x28
#define SCSI_WRITE10                    0x2A
#define SCSI_VERIFY10                   0x2F
#define SCSI_MODE_SELECT10              0x55
#define SCSI_MODE_SENSE10               0x5A


#endif  /* __MSC_H__ */
