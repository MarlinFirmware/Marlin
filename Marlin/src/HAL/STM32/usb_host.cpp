/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "../platforms.h"

#ifdef HAL_STM32

#include "../../inc/MarlinConfig.h"

#if BOTH(USE_OTG_USB_HOST, USBHOST)

#include "usb_host.h"
#include "../shared/Marduino.h"
#include "usbh_core.h"
#include "usbh_msc.h"

USBH_HandleTypeDef hUsbHost;
USBHost usb;
BulkStorage bulk(&usb);

static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id) {
  switch(id) {
    case HOST_USER_SELECT_CONFIGURATION:
      //SERIAL_ECHOLNPGM("APPLICATION_SELECT_CONFIGURATION");
      break;
    case HOST_USER_DISCONNECTION:
      //SERIAL_ECHOLNPGM("APPLICATION_DISCONNECT");
      usb.setUsbTaskState(USB_STATE_INIT);
      break;
    case HOST_USER_CLASS_ACTIVE:
      //SERIAL_ECHOLNPGM("APPLICATION_READY");
      usb.setUsbTaskState(USB_STATE_RUNNING);
      break;
    case HOST_USER_CONNECTION:
      break;
    default:
      break;
  }
}

bool USBHost::start() {
  if (USBH_Init(&hUsbHost, USBH_UserProcess, TERN(USE_USB_HS_IN_FS, HOST_HS, HOST_FS)) != USBH_OK) {
    SERIAL_ECHOLNPGM("Error: USBH_Init");
    return false;
  }
  if (USBH_RegisterClass(&hUsbHost, USBH_MSC_CLASS) != USBH_OK) {
    SERIAL_ECHOLNPGM("Error: USBH_RegisterClass");
    return false;
  }
  if (USBH_Start(&hUsbHost) != USBH_OK) {
    SERIAL_ECHOLNPGM("Error: USBH_Start");
    return false;
  }
  return true;
}

void USBHost::Task() {
  USBH_Process(&hUsbHost);
}

uint8_t USBHost::getUsbTaskState() {
  return usb_task_state;
}

void USBHost::setUsbTaskState(uint8_t state) {
  usb_task_state = state;
  if (usb_task_state == USB_STATE_RUNNING) {
    MSC_LUNTypeDef info;
    USBH_MSC_GetLUNInfo(&hUsbHost, usb.lun, &info);
    capacity = info.capacity.block_nbr / 2000;
    block_size = info.capacity.block_size;
    block_count = info.capacity.block_nbr;
    //SERIAL_ECHOLNPGM("info.capacity.block_nbr : %ld\n", info.capacity.block_nbr);
    //SERIAL_ECHOLNPGM("info.capacity.block_size: %d\n", info.capacity.block_size);
    //SERIAL_ECHOLNPGM("capacity                : %d MB\n", capacity);
  }
};

bool BulkStorage::LUNIsGood(uint8_t t) {
  return USBH_MSC_IsReady(&hUsbHost) && USBH_MSC_UnitIsReady(&hUsbHost, t);
}

uint32_t BulkStorage::GetCapacity(uint8_t lun) {
  return usb->block_count;
}

uint16_t BulkStorage::GetSectorSize(uint8_t lun) {
  return usb->block_size;
}

uint8_t BulkStorage::Read(uint8_t lun, uint32_t addr, uint16_t bsize, uint8_t blocks, uint8_t *buf) {
  return USBH_MSC_Read(&hUsbHost, lun, addr, buf, blocks) != USBH_OK;
}

uint8_t BulkStorage::Write(uint8_t lun, uint32_t addr, uint16_t bsize, uint8_t blocks, const uint8_t * buf) {
  return USBH_MSC_Write(&hUsbHost, lun, addr, const_cast<uint8_t*>(buf), blocks) != USBH_OK;
}

#endif // USE_OTG_USB_HOST && USBHOST
#endif // HAL_STM32
