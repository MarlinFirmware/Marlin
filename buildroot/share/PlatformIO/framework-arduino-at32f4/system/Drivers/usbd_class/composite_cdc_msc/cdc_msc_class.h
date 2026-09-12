/**
  **************************************************************************
  * @file     cdc_msc_class.h
  * @brief    usb cdc class file
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to 
  * download from Artery official website is the copyrighted work of Artery. 
  * Artery authorizes customers to use, copy, and distribute the BSP 
  * software and its related documentation for the purpose of design and 
  * development in conjunction with Artery microcontrollers. Use of the 
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */
  
 /* define to prevent recursive inclusion -------------------------------------*/
#ifndef __CDC_MSC_CLASS_H
#define __CDC_MSC_CLASS_H

#ifdef __cplusplus
extern "C" {
#endif
 
#include "usb_std.h"
#include "usbd_core.h"

/** @addtogroup AT32F403A_407_middlewares_usbd_class
  * @{
  */
  
/** @addtogroup USB_cdc_msc_class
  * @{
  */

/** @defgroup USB_cdc_class_definition 
  * @{
  */

/**
  * @brief usb cdc use endpoint define
  */
#define USBD_CDC_INT_EPT                 0x82
#define USBD_CDC_BULK_IN_EPT             0x83
#define USBD_CDC_BULK_OUT_EPT            0x02

#define USBD_MSC_BULK_IN_EPT             0x81
#define USBD_MSC_BULK_OUT_EPT            0x01

/**
  * @brief usb cdc in and out max packet size define
  */
#define USBD_CDC_MSC_IN_MAXPACKET_SIZE        0x40

#define USBD_CDC_MSC_OUT_MAXPACKET_SIZE       0x40
#define USBD_CDC_CMD_MAXPACKET_SIZE           0x08




/**
  * @}
  */

/** @defgroup USB_cdc_class_exported_types
  * @{
  */

/** @defgroup USB_msc_bot_scsi_definition 
  * @{
  */

#define MSC_SUPPORT_MAX_LUN              1
#define MSC_MAX_DATA_BUF_LEN             4096

#define MSC_CMD_FORMAT_UNIT              0x04
#define MSC_CMD_INQUIRY                  0x12
#define MSC_CMD_START_STOP               0x1B
#define MSC_CMD_MODE_SENSE6              0x1A
#define MSC_CMD_MODE_SENSE10             0x5A
#define MSC_CMD_ALLOW_MEDIUM_REMOVAL     0x1E
#define MSC_CMD_READ_10                  0x28
#define MSC_CMD_READ_12                  0xA8
#define MSC_CMD_READ_CAPACITY            0x25
#define MSC_CMD_READ_FORMAT_CAPACITY     0x23
#define MSC_CMD_REQUEST_SENSE            0x03
#define MSC_CMD_TEST_UNIT                0x00
#define MSC_CMD_VERIFY                   0x2F
#define MSC_CMD_WRITE_10                 0x2A
#define MSC_CMD_WRITE_12                 0xAA
#define MSC_CMD_WRITE_VERIFY             0x2E

#define MSC_REQ_GET_MAX_LUN              0xFE  /*!< get max lun */
#define MSC_REQ_BO_RESET                 0xFF  /*!< bulk only mass storage reset */

#define SET_LINE_CODING                  0x20
#define GET_LINE_CODING                  0x21

#define CBW_CMD_LENGTH                   31
#define CBW_DCBWSIGNATURE                0x43425355
#define CBW_BMCBWFLAGS_DIR_OUT           0x00
#define CBW_BMCBWFLAGS_DIR_IN            0x80

#define CSW_CMD_LENGTH                   13
#define CSW_DCSWSIGNATURE                0x53425355
#define CSW_BCSWSTATUS_PASS              0x00
#define CSW_BCSWSTATUS_FAILED            0x01
#define CSW_BCSWSTATUS_PHASE_ERR         0x02

#define MSC_STATE_MACHINE_CMD            0x00
#define MSC_STATE_MACHINE_DATA_IN        0x01
#define MSC_STATE_MACHINE_DATA_OUT       0x02
#define MSC_STATE_MACHINE_SEND_DATA      0x03
#define MSC_STATE_MACHINE_LAST_DATA      0x04
#define MSC_STATE_MACHINE_STATUS         0x05
#define MSC_STATE_MACHINE_FAILED         0x06
#define MSC_STATE_MACHINE_IDLE           0x07

#define MSC_BOT_STATE_IDLE               0x00
#define MSC_BOT_STATE_RECOVERY           0x01
#define MSC_BOT_STATE_ERROR              0x02

#define REQ_SENSE_STANDARD_DATA_LEN      0x12
#define SENSE_KEY_NO_SENSE               0x00
#define SENSE_KEY_RECOVERED_ERROR        0x01
#define SENSE_KEY_NOT_READY              0x02
#define SENSE_KEY_MEDIUM_ERROR           0x03
#define SENSE_KEY_HARDWARE_ERROR         0x04
#define SENSE_KEY_ILLEGAL_REQUEST        0x05
#define SENSE_KEY_UNIT_ATTENTION         0x06
#define SENSE_KEY_DATA_PROTECT           0x07
#define SENSE_KEY_BLANK_CHECK            0x08
#define SENSE_KEY_VENDERO_SPECIFIC       0x09
#define SENSE_KEY_ABORTED_COMMAND        0x0B
#define SENSE_KEY_VOLUME_OVERFLOW        0x0D
#define SENSE_KEY_MISCOMPARE             0x0E


#define INVALID_COMMAND                  0x20
#define INVALID_FIELED_IN_COMMAND        0x24
#define PARAMETER_LIST_LENGTH_ERROR      0x1A
#define INVALID_FIELD_IN_PARAMETER_LIST  0x26
#define ADDRESS_OUT_OF_RANGE             0x21
#define MEDIUM_NOT_PRESENT               0x3A
#define MEDIUM_HAVE_CHANGED              0x28

#define SCSI_INQUIRY_DATA_LENGTH         36

/**
  * @brief typical command block description
  */
typedef struct
{
  uint8_t  opcode;
  uint8_t  lun;
  uint32_t address;
  uint8_t  reserved1;
  uint32_t alloc_length;
  uint16_t reserved2;
}cbd_typical_type;

/**
  * @brief extended command block description
  */
typedef struct
{
  uint8_t  opcode;
  uint8_t  lun;
  uint32_t address;
  uint8_t  reserved1;
  uint32_t alloc_length;
  uint16_t reserved2;
}cbd_extended_type;

/**
  * @brief command block wrapper
  */
typedef struct
{
  uint32_t dCBWSignature;
  uint32_t dCBWTage;
  uint32_t dCBWDataTransferLength;
  uint8_t  bmCBWFlags;
  uint8_t  bCBWLUN;
  uint8_t  bCBWCBLength;
  uint8_t  CBWCB[16];
}cbw_type;

/**
  * @brief command block wrapper
  */
typedef struct
{
  uint32_t dCSWSignature;
  uint32_t dCSWTag;
  uint32_t dCSWDataResidue;
  uint32_t bCSWStatus;
}csw_type;

/**
  * @brief request sense standard data
  */
typedef struct
{
  uint8_t  err_code;
  uint8_t  reserved1;
  uint8_t  sense_key;
  uint32_t information;
  uint8_t  as_length;
  uint32_t reserved2;
  uint8_t  asc;
  uint8_t  ascq;
  uint32_t reserved3;
}sense_type;


typedef struct
{
  uint8_t msc_state;
  uint8_t bot_status;
  uint32_t max_lun;
  
  uint32_t blk_nbr[MSC_SUPPORT_MAX_LUN];
  uint32_t blk_size[MSC_SUPPORT_MAX_LUN];
  
  uint32_t blk_addr;
  uint32_t blk_len;
  
  uint32_t data_len;
  uint8_t data[MSC_MAX_DATA_BUF_LEN];
  
  uint32_t alt_setting;
  
  cbw_type cbw_struct;
  csw_type csw_struct;
  
}msc_type;

/**
  * @brief usb cdc class struct
  */
typedef struct
{
  //used for CDC
  uint32_t alt_setting;
  uint8_t g_rx_buff[USBD_CDC_MSC_OUT_MAXPACKET_SIZE];
  uint8_t g_cmd[USBD_CDC_CMD_MAXPACKET_SIZE];
  uint8_t g_req;
  uint16_t g_len, g_rxlen;
  __IO uint8_t g_tx_completed, g_rx_completed;
  linecoding_type linecoding;

  //used for MSC
  uint8_t msc_state;
  uint8_t bot_status;
  uint32_t max_lun;
  
  uint32_t blk_nbr[MSC_SUPPORT_MAX_LUN];
  uint32_t blk_size[MSC_SUPPORT_MAX_LUN];
  
  uint64_t blk_addr;
  uint32_t blk_len;
  
  uint32_t data_len;
  uint8_t data[MSC_MAX_DATA_BUF_LEN];
  
  cbw_type cbw_struct;
  csw_type csw_struct;
}cdc_msc_struct_type; //cdc_struct_type;


/**
  * @}
  */

/** @defgroup USB_cdc_msc_class_exported_functions
  * @{
  */
extern usbd_class_handler cdc_msc_class_handler;  //cdc_class_handler;
uint16_t usb_vcp_get_rxdata(void *udev, uint8_t *recv_data);
error_status usb_vcp_send_data(void *udev, uint8_t *send_data, uint16_t len);

void bot_scsi_init(void *udev);
void bot_scsi_reset(void *udev);
void bot_scsi_datain_handler(void *pudev, uint8_t ept_num);
void bot_scsi_dataout_handler(void *pudev, uint8_t ept_num);
void bot_cbw_decode(void *udev);
void bot_scsi_send_data(void *udev, uint8_t *buffer, uint32_t len);
void bot_scsi_send_csw(void *udev, uint8_t status);
void bot_scsi_sense_code(void *udev, uint8_t sense_key, uint8_t asc);
usb_sts_type bot_scsi_check_address(void *udev, uint8_t lun, uint32_t blk_offset, uint32_t blk_count);
void bot_scsi_stall(void *udev);
usb_sts_type bot_scsi_cmd_process(void *udev);

usb_sts_type bot_scsi_test_unit(void *udev, uint8_t lun);
usb_sts_type bot_scsi_inquiry(void *udev, uint8_t lun);
usb_sts_type bot_scsi_start_stop(void *udev, uint8_t lun);
usb_sts_type bot_scsi_allow_medium_removal(void *udev, uint8_t lun);
usb_sts_type bot_scsi_mode_sense6(void *udev, uint8_t lun);
usb_sts_type bot_scsi_mode_sense10(void *udev, uint8_t lun);
usb_sts_type bot_scsi_read10(void *udev, uint8_t lun);
usb_sts_type bot_scsi_capacity(void *udev, uint8_t lun);
usb_sts_type bot_scsi_format_capacity(void *udev, uint8_t lun);
usb_sts_type bot_scsi_request_sense(void *udev, uint8_t lun);
usb_sts_type bot_scsi_verify(void *udev, uint8_t lun);
usb_sts_type bot_scsi_write10(void *udev, uint8_t lun);
void bot_scsi_clear_feature(void *udev, uint8_t ept_num);

/**
  * @}
  */

/**
  * @}
  */
  
/**
  * @}
  */
#ifdef __cplusplus
}
#endif

#endif




