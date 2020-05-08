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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/* DGUS implementation written by coldtobi in 2019 for Marlin */

#include "../../../../inc/MarlinConfigPre.h"

#if HAS_DGUS_LCD

#include "DGUSDisplay.h"

DGUSDisplay::rx_datagram_state_t DGUSDisplay::rx_datagram_state = DGUS_IDLE;
uint8_t DGUSDisplay::rx_datagram_len = 0;

bool DGUSDisplay::initialized = false;
bool DGUSDisplay::no_reentrance = false;

void DGUSDisplay::Loop() {
  // protect against recursion… ProcessRx() may indirectly call idle() when injecting gcode commands.
  if (!no_reentrance) {
    no_reentrance = true;
    ProcessRx();
    no_reentrance = false;
  }
}

void DGUSDisplay::Init() {
  DGUS_SERIAL.begin(DGUS_BAUDRATE);
}

void DGUSDisplay::WriteVariable(uint16_t adr, const void* values, uint8_t valueslen, bool isstr) {
  const char* myvalues = static_cast<const char*>(values);
  bool strend = !myvalues;
  WriteHeader(adr, DGUS_WRITEVAR, valueslen);
  while (valueslen--) {
    char x;
    if (!strend) x = *myvalues++;
    if ((isstr && !x) || strend) {
      strend = true;
      x = ' ';
    }
    DGUS_SERIAL.write(x);
  }
}

void DGUSDisplay::WriteVariablePGM(uint16_t adr, const void* values, uint8_t valueslen, bool isstr) {
  const char* myvalues = static_cast<const char*>(values);
  bool strend = !myvalues;
  WriteHeader(adr, DGUS_WRITEVAR, valueslen);
  while (valueslen--) {
    char x;
    if (!strend) x = pgm_read_byte(myvalues++);
    if ((isstr && !x) || strend) {
      strend = true;
      x = ' ';
    }
    DGUS_SERIAL.write(x);
  }
}

void DGUSDisplay::SwitchScreen(DGUS_Screen screen) {
  DEBUG_ECHOLNPAIR("SwitchScreen ", screen);
  const uint8_t command[] = { 0x5A, 0x01, 0x00, screen };
  WriteVariable(0x84, command, sizeof(command));
}

void DGUSDisplay::SetBrightness(uint8_t brightness) {
  DEBUG_ECHOLNPAIR("SetBrightness ", brightness);
  if (brightness > 0x64) brightness = 0x64;
  const uint8_t command[] = { brightness, brightness };
  WriteVariable(0x82, command, sizeof(command));
}

void DGUSDisplay::PlaySound(uint8_t start, uint8_t len, uint8_t volume) {
  DEBUG_ECHOLNPAIR("PlaySound ", start, ":", len, "\nVolume ", volume);
  if (volume == 0) {
    const uint8_t command[] = { start, len };
    WriteVariable(0xA0, command, sizeof(command));
  }
  else {
    const uint8_t command[] = { start, len, volume, 0x00 };
    WriteVariable(0xA0, command, sizeof(command));
  }
}

void DGUSDisplay::SetVolume(uint8_t volume) {
  DEBUG_ECHOLNPAIR("SetVolume ", volume);
  const uint8_t command[] = { volume, 0x00 };
  WriteVariable(0xA1, command, sizeof(command));
}

void DGUSDisplay::EnableControl(DGUS_Screen screen, dgus_control_type type, uint8_t index) {
  DEBUG_ECHOLNPAIR("EnableControl ", index, "\nScreen ", screen, "\nType ", type);

  const uint8_t command[] = { 0x5A, 0xA5, 0x00, screen, index, type, 0x00, 0x01 };
  WriteVariable(0xB0, command, sizeof(command));
}

void DGUSDisplay::DisableControl(DGUS_Screen screen, dgus_control_type type, uint8_t index) {
  DEBUG_ECHOLNPAIR("DisableControl ", index, "\nScreen ", screen, "\nType ", type);

  const uint8_t command[] = { 0x5A, 0xA5, 0x00, screen, index, type, 0x00, 0x00 };
  WriteVariable(0xB0, command, sizeof(command));
}

void DGUSDisplay::ProcessRx() {

  #if ENABLED(DGUS_SERIAL_STATS_RX_BUFFER_OVERRUNS)
    if (!DGUS_SERIAL.available() && DGUS_SERIAL.buffer_overruns()) {
      // Overrun, but reset the flag only when the buffer is empty
      // We want to extract as many as valid datagrams possible...
      DEBUG_ECHOPGM("OVFL");
      rx_datagram_state = DGUS_IDLE;
      //DGUS_SERIAL.reset_rx_overun();
      DGUS_SERIAL.flush();
    }
  #endif

  uint8_t receivedbyte;
  while (DGUS_SERIAL.available()) {
    switch (rx_datagram_state) {

      case DGUS_IDLE: // Waiting for the first header byte
        receivedbyte = DGUS_SERIAL.read();
        //DEBUG_ECHOPAIR("< ",x);
        if (DGUS_HEADER1 == receivedbyte) rx_datagram_state = DGUS_HEADER1_SEEN;
        break;

      case DGUS_HEADER1_SEEN: // Waiting for the second header byte
        receivedbyte = DGUS_SERIAL.read();
        //DEBUG_ECHOPAIR(" ",x);
        rx_datagram_state = (DGUS_HEADER2 == receivedbyte) ? DGUS_HEADER2_SEEN : DGUS_IDLE;
        break;

      case DGUS_HEADER2_SEEN: // Waiting for the length byte
        rx_datagram_len = DGUS_SERIAL.read();
        DEBUG_ECHOPAIR(" (", rx_datagram_len, ") ");

        // Telegram min len is 3 (command and one word of payload)
        rx_datagram_state = WITHIN(rx_datagram_len, 3, DGUS_RX_BUFFER_SIZE) ? DGUS_WAIT_TELEGRAM : DGUS_IDLE;
        break;

      case DGUS_WAIT_TELEGRAM: // wait for complete datagram to arrive.
        if (DGUS_SERIAL.available() < rx_datagram_len) return;

        initialized = true; // We've talked to it, so we defined it as initialized.
        uint8_t command = DGUS_SERIAL.read();

        DEBUG_ECHOPAIR("# ", command);

        uint8_t readlen = rx_datagram_len - 1;  // command is part of len.
        unsigned char tmp[rx_datagram_len - 1];
        unsigned char *ptmp = tmp;

        while (readlen--) {
          receivedbyte = DGUS_SERIAL.read();
          DEBUG_ECHOPAIR(" ", receivedbyte);
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
          const uint16_t vp = tmp[0] << 8 | tmp[1];
          const uint8_t dlen = tmp[2] << 1;  // Convert to Bytes. (Display works with words)
          //DEBUG_ECHOPAIR(" vp=", vp, " dlen=", dlen);
          DGUS_VP_Variable ramcopy;
          if (DGUS_PopulateVPVar(vp, &ramcopy)) {
            if (ramcopy.size && dlen != ramcopy.size)
              DEBUG_ECHOLNPGM("SIZE MISMATCH");
            else if (ramcopy.set_by_display_handler)
              ramcopy.set_by_display_handler(ramcopy, ramcopy.size ? &tmp[3] : nullptr);
            else
              DEBUG_ECHOLNPGM(" VPVar found, no handler.");
          }
          else
            DEBUG_ECHOLNPAIR(" VPVar not found:", vp);

          rx_datagram_state = DGUS_IDLE;
          break;
        }

        // discard anything else
        rx_datagram_state = DGUS_IDLE;
        break;
    }
  }
}

size_t DGUSDisplay::GetFreeTxBuffer() {
  return DGUS_SERIAL_GET_TX_BUFFER_FREE();
}

void DGUSDisplay::WriteHeader(uint16_t adr, uint8_t cmd, uint8_t payloadlen) {
  DGUS_SERIAL.write(DGUS_HEADER1);
  DGUS_SERIAL.write(DGUS_HEADER2);
  DGUS_SERIAL.write(payloadlen + 3);
  DGUS_SERIAL.write(cmd);
  DGUS_SERIAL.write(adr >> 8);
  DGUS_SERIAL.write(adr & 0xFF);
}

const uint16_t* DGUS_FindScreenVPMapList(uint8_t screen) {
  const uint16_t *ret;
  const struct VPMapping *map = VPMap;
  while (ret = (uint16_t*) pgm_read_ptr(&map->VPList)) {
    if (pgm_read_byte(&map->screen) == screen) return ret;
    map++;
  }
  return nullptr;
}

const DGUS_VP_Variable* DGUS_FindVPVar(const uint16_t vp) {
  const DGUS_VP_Variable *ret = ListOfVP;
  do {
    const uint16_t vpcheck = pgm_read_word(&ret->VP);
    if (vpcheck == 0) break;
    if (vpcheck == vp) return ret;
    ++ret;
  } while (1);

  DEBUG_ECHOLNPAIR("DGUS_FindVPVar NOT FOUND ", vp);
  return nullptr;
}

bool DGUS_PopulateVPVar(const uint16_t VP, DGUS_VP_Variable * const ramcopy) {
  const DGUS_VP_Variable *pvp = DGUS_FindVPVar(VP);

  if (!pvp) return false;
  memcpy_P(ramcopy, pvp, sizeof(*pvp));
  return true;
}

#endif // HAS_DGUS_LCD
