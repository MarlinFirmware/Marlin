/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/* DGUS implementation written by coldtobi in 2019 for Marlin */

#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(DGUS_LCD_UI_RELOADED)

#include "DGUSDisplay.h"

#include "config/DGUS_Addr.h"
#include "config/DGUS_Constants.h"
#include "definition/DGUS_VPList.h"

#include "../ui_api.h"
#include "../../../gcode/gcode.h"

long map_precise(float x, long in_min, long in_max, long out_min, long out_max) {
  return LROUND((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);
}

uint8_t DGUSDisplay::gui_version = 0;
uint8_t DGUSDisplay::os_version = 0;

uint8_t DGUSDisplay::volume = 255;
uint8_t DGUSDisplay::brightness = 100;

DGUSDisplay::rx_datagram_state_t DGUSDisplay::rx_datagram_state = DGUS_IDLE;
uint8_t DGUSDisplay::rx_datagram_len = 0;

bool DGUSDisplay::initialized = false;

void DGUSDisplay::Loop() {
  ProcessRx();
}

void DGUSDisplay::Init() {
  LCD_SERIAL.begin(LCD_BAUDRATE);

  ReadVersions();
}

void DGUSDisplay::Read(uint16_t addr, uint8_t size) {
  WriteHeader(addr, DGUS_READVAR, size);

  LCD_SERIAL.write(size);
}

void DGUSDisplay::Write(uint16_t addr, const void* data_ptr, uint8_t size) {
  if (!data_ptr) return;

  WriteHeader(addr, DGUS_WRITEVAR, size);

  const char* data = static_cast<const char*>(data_ptr);

  while (size--) {
    LCD_SERIAL.write(*data++);
  }
}

void DGUSDisplay::WriteString(uint16_t addr, const void* data_ptr, uint8_t size, bool left, bool right, bool use_space) {
  if (!data_ptr) return;

  WriteHeader(addr, DGUS_WRITEVAR, size);

  const char* data = static_cast<const char*>(data_ptr);
  size_t len = strlen(data);
  uint8_t left_spaces = 0;
  uint8_t right_spaces = 0;

  if (len < size) {
    if (!len) {
      right_spaces = size;
    }
    else if ((left && right) || (!left && !right)) {
      left_spaces = (size - len) / 2;
      right_spaces = size - len - left_spaces;
    }
    else if (left) {
      right_spaces = size - len;
    }
    else {
      left_spaces = size - len;
    }
  }
  else {
    len = size;
  }

  while (left_spaces--) {
    LCD_SERIAL.write(' ');
  }
  while (len--) {
    LCD_SERIAL.write(*data++);
  }
  while (right_spaces--) {
    LCD_SERIAL.write(use_space ? ' ' : '\0');
  }
}

void DGUSDisplay::WriteStringPGM(uint16_t addr, const void* data_ptr, uint8_t size, bool left, bool right, bool use_space) {
  if (!data_ptr) return;

  WriteHeader(addr, DGUS_WRITEVAR, size);

  const char* data = static_cast<const char*>(data_ptr);
  size_t len = strlen_P(data);
  uint8_t left_spaces = 0;
  uint8_t right_spaces = 0;

  if (len < size) {
    if (!len) {
      right_spaces = size;
    }
    else if ((left && right) || (!left && !right)) {
      left_spaces = (size - len) / 2;
      right_spaces = size - len - left_spaces;
    }
    else if (left) {
      right_spaces = size - len;
    }
    else {
      left_spaces = size - len;
    }
  }
  else {
    len = size;
  }

  while (left_spaces--) {
    LCD_SERIAL.write(' ');
  }
  while (len--) {
    LCD_SERIAL.write(pgm_read_byte(data++));
  }
  while (right_spaces--) {
    LCD_SERIAL.write(use_space ? ' ' : '\0');
  }
}

void DGUSDisplay::ReadVersions() {
  if (gui_version != 0 && os_version != 0) return;
  Read(DGUS_VERSION, 1);
}

void DGUSDisplay::SwitchScreen(DGUS_Screen screen) {
  DEBUG_ECHOLNPGM("SwitchScreen ", (uint8_t)screen);
  const uint8_t command[] = { 0x5A, 0x01, 0x00, (uint8_t)screen };
  Write(0x84, command, sizeof(command));
}

void DGUSDisplay::PlaySound(uint8_t start, uint8_t len, uint8_t volume) {
  if (volume == 0) volume = DGUSDisplay::volume;
  if (volume == 0) return;
  DEBUG_ECHOLNPGM("PlaySound ", start, ":", len, "\nVolume ", volume);
  const uint8_t command[] = { start, len, volume, 0x00 };
  Write(0xA0, command, sizeof(command));
}

void DGUSDisplay::EnableControl(DGUS_Screen screen, DGUS_ControlType type, DGUS_Control control) {
  DEBUG_ECHOLNPGM("EnableControl ", (uint8_t)control, "\nScreen ", (uint8_t)screen, "\nType ", (uint8_t)type);

  const uint8_t command[] = { 0x5A, 0xA5, 0x00, (uint8_t)screen, (uint8_t)control, type, 0x00, 0x01 };
  Write(0xB0, command, sizeof(command));

  FlushTx();
  delay(50);
}

void DGUSDisplay::DisableControl(DGUS_Screen screen, DGUS_ControlType type, DGUS_Control control) {
  DEBUG_ECHOLNPGM("DisableControl ", (uint8_t)control, "\nScreen ", (uint8_t)screen, "\nType ", (uint8_t)type);

  const uint8_t command[] = { 0x5A, 0xA5, 0x00, (uint8_t)screen, (uint8_t)control, type, 0x00, 0x00 };
  Write(0xB0, command, sizeof(command));

  FlushTx();
  delay(50);
}

uint8_t DGUSDisplay::GetBrightness() {
  return brightness;
}

uint8_t DGUSDisplay::GetVolume() {
  return map_precise(volume, 0, 255, 0, 100);
}

void DGUSDisplay::SetBrightness(uint8_t new_brightness) {
  brightness = constrain(new_brightness, 0, 100);
  new_brightness = map_precise(brightness, 0, 100, 5, 100);
  DEBUG_ECHOLNPGM("SetBrightness ", new_brightness);
  const uint8_t command[] = { new_brightness, new_brightness };
  Write(0x82, command, sizeof(command));
}

void DGUSDisplay::SetVolume(uint8_t new_volume) {
  volume = map_precise(constrain(new_volume, 0, 100), 0, 100, 0, 255);
  DEBUG_ECHOLNPGM("SetVolume ", volume);
  const uint8_t command[] = { volume, 0x00 };
  Write(0xA1, command, sizeof(command));
}

void DGUSDisplay::ProcessRx() {

  #if ENABLED(LCD_SERIAL_STATS_RX_BUFFER_OVERRUNS)
    if (!LCD_SERIAL.available() && LCD_SERIAL.buffer_overruns()) {
      // Overrun, but reset the flag only when the buffer is empty
      // We want to extract as many as valid datagrams possible...
      DEBUG_ECHOPGM("OVFL");
      rx_datagram_state = DGUS_IDLE;
      //LCD_SERIAL.reset_rx_overun();
      LCD_SERIAL.flush();
    }
  #endif

  uint8_t receivedbyte;
  while (LCD_SERIAL.available()) {
    switch (rx_datagram_state) {

      case DGUS_IDLE: // Waiting for the first header byte
        receivedbyte = LCD_SERIAL.read();
        DEBUG_ECHOPGM("< ", receivedbyte);
        if (DGUS_HEADER1 == receivedbyte) rx_datagram_state = DGUS_HEADER1_SEEN;
        break;

      case DGUS_HEADER1_SEEN: // Waiting for the second header byte
        receivedbyte = LCD_SERIAL.read();
        DEBUG_ECHOPGM(" ", receivedbyte);
        rx_datagram_state = (DGUS_HEADER2 == receivedbyte) ? DGUS_HEADER2_SEEN : DGUS_IDLE;
        break;

      case DGUS_HEADER2_SEEN: // Waiting for the length byte
        rx_datagram_len = LCD_SERIAL.read();
        DEBUG_ECHOPGM(" (", rx_datagram_len, ") ");

        // Telegram min len is 3 (command and one word of payload)
        rx_datagram_state = WITHIN(rx_datagram_len, 3, DGUS_RX_BUFFER_SIZE) ? DGUS_WAIT_TELEGRAM : DGUS_IDLE;
        break;

      case DGUS_WAIT_TELEGRAM: // wait for complete datagram to arrive.
        if (LCD_SERIAL.available() < rx_datagram_len) return;

        initialized = true; // We've talked to it, so we defined it as initialized.
        uint8_t command = LCD_SERIAL.read();

        DEBUG_ECHOPGM("# ", command);

        uint8_t readlen = rx_datagram_len - 1;  // command is part of len.
        unsigned char tmp[rx_datagram_len - 1];
        unsigned char *ptmp = tmp;

        while (readlen--) {
          receivedbyte = LCD_SERIAL.read();
          DEBUG_ECHOPGM(" ", receivedbyte);
          *ptmp++ = receivedbyte;
        }
        DEBUG_ECHOPGM(" # ");
        // mostly we'll get this: 5A A5 03 82 4F 4B -- ACK on 0x82, so discard it.
        if (command == DGUS_WRITEVAR && 'O' == tmp[0] && 'K' == tmp[1]) {
          DEBUG_ECHOLNPGM(">");
          rx_datagram_state = DGUS_IDLE;
          break;
        }

        /* AutoUpload, (and answer to) Command 0x83 :
        |      tmp[0  1  2  3  4 ... ]
        | Example 5A A5 06 83 20 01 01 78 01 ……
        |          / /  |  |   \ /   |  \     \
        |        Header |  |    |    |   \_____\_ DATA (Words!)
        |     DatagramLen  /  VPAdr  |
        |           Command          DataLen (in Words) */
        if (command == DGUS_READVAR) {
          const uint16_t addr = tmp[0] << 8 | tmp[1];
          const uint8_t dlen = tmp[2] << 1;  // Convert to Bytes. (Display works with words)
          DEBUG_ECHOPGM("addr=", addr, " dlen=", dlen, "> ");

          if (addr == DGUS_VERSION && dlen == 2) {
            DEBUG_ECHOLNPGM("VERSIONS");
            gui_version = tmp[3];
            os_version = tmp[4];
            rx_datagram_state = DGUS_IDLE;
            break;
          }

          DGUS_VP vp;
          if (!DGUS_PopulateVP((DGUS_Addr)addr, &vp)) {
            DEBUG_ECHOLNPGM("VP not found");
            rx_datagram_state = DGUS_IDLE;
            break;
          }

          if (!vp.rx_handler) {
            DEBUG_ECHOLNPGM("VP found, no handler.");
            rx_datagram_state = DGUS_IDLE;
            break;
          }

          gcode.reset_stepper_timeout();

          if (!vp.size) {
            DEBUG_EOL();
            vp.rx_handler(vp, nullptr);

            rx_datagram_state = DGUS_IDLE;
            break;
          }

          if (vp.flags & VPFLAG_RXSTRING) {
            unsigned char buffer[vp.size];
            memset(buffer, 0, vp.size);

            for (uint8_t i = 0; i < dlen; i++) {
              if (i >= vp.size) break;

              if (i + 1 < dlen && tmp[i + 3] == 0xFF && tmp[i + 4] == 0xFF)
                break;

              buffer[i] = tmp[i + 3];
            }

            DEBUG_EOL();
            vp.rx_handler(vp, buffer);

            rx_datagram_state = DGUS_IDLE;
            break;
          }

          if (dlen != vp.size) {
            DEBUG_ECHOLNPGM("VP found, size mismatch.");
            rx_datagram_state = DGUS_IDLE;
            break;
          }

          DEBUG_EOL();
          vp.rx_handler(vp, &tmp[3]);

          rx_datagram_state = DGUS_IDLE;
          break;
        }

        DEBUG_ECHOLNPGM(">");
        rx_datagram_state = DGUS_IDLE;
        break;
    }
  }
}

size_t DGUSDisplay::GetFreeTxBuffer() {
  #ifdef LCD_SERIAL_GET_TX_BUFFER_FREE
    return LCD_SERIAL_GET_TX_BUFFER_FREE();
  #else
    return SIZE_MAX;
  #endif
}

void DGUSDisplay::FlushTx() {
  #ifdef ARDUINO_ARCH_STM32
    LCD_SERIAL.flush();
  #else
    LCD_SERIAL.flushTX();
  #endif
}

void DGUSDisplay::WriteHeader(uint16_t addr, uint8_t command, uint8_t len) {
  LCD_SERIAL.write(DGUS_HEADER1);
  LCD_SERIAL.write(DGUS_HEADER2);
  LCD_SERIAL.write(len + 3);
  LCD_SERIAL.write(command);
  LCD_SERIAL.write(addr >> 8);
  LCD_SERIAL.write(addr & 0xFF);
}

bool DGUS_PopulateVP(const DGUS_Addr addr, DGUS_VP * const buffer) {
  const DGUS_VP *ret = vp_list;

  do {
    const uint16_t *paddr = (uint16_t *)(&ret->addr);
    const uint16_t addrcheck = pgm_read_word(paddr);
    if (addrcheck == 0) break;
    if ((DGUS_Addr)addrcheck == addr) {
      memcpy_P(buffer, ret, sizeof(*ret));
      return true;
    }
  } while (++ret);
  DEBUG_ECHOLNPGM("VP not found: ", (uint16_t)addr);
  return false;
}

#endif // DGUS_LCD_UI_RELOADED
