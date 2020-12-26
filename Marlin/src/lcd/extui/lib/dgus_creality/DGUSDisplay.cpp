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

/* DGUS implementation written by coldtobi in 2019 for Marlin */

#include "../../../../inc/MarlinConfigPre.h"

#if ENABLED(DGUS_LCD_UI_CREALITY_TOUCH)

#if HOTENDS > 2
  #error "More than 2 hotends not implemented on the Display UI design."
#endif

#include "../../ui_api.h"

#include "../../../../MarlinCore.h"
#include "../../../../module/temperature.h"
#include "../../../../module/motion.h"
#include "../../../../gcode/queue.h"
#include "../../../../module/planner.h"
#include "../../../../sd/cardreader.h"
#include "../../../../libs/duration_t.h"
#include "../../../../module/printcounter.h"
#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../../../feature/powerloss.h"
#endif

#include "DGUSDisplay.h"
#include "../dgus/DGUSVPVariable.h"
#include "DGUSDisplayDef.h"

// Preamble... 2 Bytes, usually 0x5A 0xA5, but configurable
constexpr uint8_t DGUS_HEADER1 = 0x5A;
constexpr uint8_t DGUS_HEADER2 = 0xA5;

constexpr uint8_t DGUS_CMD_WRITEVAR = 0x82;
constexpr uint8_t DGUS_CMD_READVAR = 0x83;

#if ENABLED(DEBUG_DGUSLCD)
  bool dguslcd_local_debug; // = false;
#endif

#define dgusserial LCD_SERIAL

void DGUSDisplay::InitDisplay() {
  dgusserial.begin(LCD_BAUDRATE);

  /*delay(500); // Attempt to fix possible handshake error

  ResetDisplay(); // Reset for firmware update

  delay(500); // Attempt to fix possible handshake error
*/
  if (true
    #if ENABLED(POWER_LOSS_RECOVERY)
      && !recovery.valid()
    #endif
  )
    RequestScreen(
      #if ENABLED(SHOW_BOOTSCREEN)
        DGUSLCD_SCREEN_BOOT
      #else
        DGUSLCD_SCREEN_MAIN
      #endif
    );
}

void DGUSDisplay::ResetDisplay() {
  SERIAL_ECHOLN("ResetDisplay");
  const unsigned char resetCommand[] = { 0x55, 0xAA, 0x5A, 0xA5 };
  WriteVariable(0x04, resetCommand, sizeof(resetCommand));
}

void DGUSDisplay::ReadVariable(uint16_t adr) {
  WriteHeader(adr, DGUS_CMD_READVAR, sizeof(uint8_t));

  // Specify to read one byte
  dgusserial.write(static_cast<uint8_t>(1));
}

void DGUSDisplay::WriteVariable(uint16_t adr, const void* values, uint8_t valueslen, bool isstr) {
  const char* myvalues = static_cast<const char*>(values);
  bool strend = !myvalues;
  WriteHeader(adr, DGUS_CMD_WRITEVAR, valueslen);
  while (valueslen--) {
    char x;
    if (!strend) x = *myvalues++;
    if ((isstr && !x) || strend) {
      strend = true;
      x = ' ';
    }
    dgusserial.write(x);
  }
}

void DGUSDisplay::WriteVariable(uint16_t adr, uint16_t value) {
  value = (value & 0xffU) << 8U | (value >> 8U);
  WriteVariable(adr, static_cast<const void*>(&value), sizeof(uint16_t));
}

void DGUSDisplay::WriteVariable(uint16_t adr, int16_t value) {
  value = (value & 0xffU) << 8U | (value >> 8U);
  WriteVariable(adr, static_cast<const void*>(&value), sizeof(uint16_t));
}

void DGUSDisplay::WriteVariable(uint16_t adr, uint8_t value) {
  WriteVariable(adr, static_cast<const void*>(&value), sizeof(uint8_t));
}

void DGUSDisplay::WriteVariable(uint16_t adr, int8_t value) {
  WriteVariable(adr, static_cast<const void*>(&value), sizeof(int8_t));
}

void DGUSDisplay::WriteVariable(uint16_t adr, long value) {
    union { long l; char lb[4]; } endian;
    char tmp[4];
    endian.l = value;
    tmp[0] = endian.lb[3];
    tmp[1] = endian.lb[2];
    tmp[2] = endian.lb[1];
    tmp[3] = endian.lb[0];
    WriteVariable(adr, static_cast<const void*>(&tmp), sizeof(long));
}

void DGUSDisplay::WriteVariablePGM(uint16_t adr, const void* values, uint8_t valueslen, bool isstr) {
  const char* myvalues = static_cast<const char*>(values);
  bool strend = !myvalues;
  WriteHeader(adr, DGUS_CMD_WRITEVAR, valueslen);
  while (valueslen--) {
    char x;
    if (!strend) x = pgm_read_byte(myvalues++);
    if ((isstr && !x) || strend) {
      strend = true;
      x = ' ';
    }
    dgusserial.write(x);
  }
}

void DGUSDisplay::SetVariableDisplayColor(uint16_t sp, uint16_t color) {
  WriteVariable(sp + 0x03, color);
}

void DGUSDisplay::SetVariableAppendText(uint16_t sp, PGM_P appendText) {
  // High byte is length, low byte is first char
  if (!appendText) {
    WriteVariable(sp + 0x07, static_cast<uint8_t>(0));
    return;
  }

  uint8_t lengthFirstChar = strlen_P(appendText);// << 8;
  WriteVariable(sp + 0x07, lengthFirstChar);
  WriteVariablePGM(sp + 0x08, appendText, strlen_P(appendText));
}

void DGUSDisplay::ProcessRx() {

  #if ENABLED(DGUS_SERIAL_STATS_RX_BUFFER_OVERRUNS)
    if (!dgusserial.available() && dgusserial.buffer_overruns()) {
      // Overrun, but reset the flag only when the buffer is empty
      // We want to extract as many as valid datagrams possible...
      DEBUG_ECHOPGM("OVFL");
      rx_datagram_state = DGUS_IDLE;
      //dgusserial.reset_rx_overun();
      dgusserial.flush();
    }
  #endif

  uint8_t receivedbyte;
  while (dgusserial.available()) {
    switch (rx_datagram_state) {

      case DGUS_IDLE: // Waiting for the first header byte
        receivedbyte = dgusserial.read();
        //DEBUGLCDCOMM_ECHOPAIR("< ",receivedbyte);
        if (DGUS_HEADER1 == receivedbyte) rx_datagram_state = DGUS_HEADER1_SEEN;
        break;

      case DGUS_HEADER1_SEEN: // Waiting for the second header byte
        receivedbyte = dgusserial.read();
        //DEBUGLCDCOMM_ECHOPAIR(" ", receivedbyte);
        rx_datagram_state = (DGUS_HEADER2 == receivedbyte) ? DGUS_HEADER2_SEEN : DGUS_IDLE;
        break;

      case DGUS_HEADER2_SEEN: // Waiting for the length byte
        rx_datagram_len = dgusserial.read();
        //DEBUGLCDCOMM_ECHOPAIR(" (", rx_datagram_len, ") ");

        // Telegram min len is 3 (command and one word of payload)
        rx_datagram_state = WITHIN(rx_datagram_len, 3, DGUS_RX_BUFFER_SIZE) ? DGUS_WAIT_TELEGRAM : DGUS_IDLE;
        break;

      case DGUS_WAIT_TELEGRAM: // wait for complete datagram to arrive.
        if (dgusserial.available() < rx_datagram_len) return;

        Initialized = true; // We've talked to it, so we defined it as initialized.
        uint8_t command = dgusserial.read();

       // DEBUGLCDCOMM_ECHOPAIR("# ", command);

        uint8_t readlen = rx_datagram_len - 1;  // command is part of len.
        unsigned char tmp[rx_datagram_len - 1];
        unsigned char *ptmp = tmp;
        while (readlen--) {
          receivedbyte = dgusserial.read();
          //DEBUGLCDCOMM_ECHOPAIR(" ", receivedbyte);
          *ptmp++ = receivedbyte;
        }
        //DEBUGLCDCOMM_ECHOPGM(" # ");
        // mostly we'll get this: 5A A5 03 82 4F 4B -- ACK on 0x82, so discard it.
        if (command == DGUS_CMD_WRITEVAR && 'O' == tmp[0] && 'K' == tmp[1]) {
          //DEBUG_ECHOLNPGM(">");
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
        if (command == DGUS_CMD_READVAR) {
          const uint16_t vp = tmp[0] << 8 | tmp[1];

          if (vp == 0x14 /*PIC_Now*/) {
            const uint16_t screen_id = tmp[3] << 8 | tmp[4];

            // A display was requested. If the screen didn't yet switch to that display, we won't give that value back, otherwise the code gets confused.
            // The DWIN display mostly honours the PIC_SET requests from the firmware, so after a while we may want to nudge it to the correct screen
            DEBUG_ECHOPAIR(" Got a response on the current screen: ", screen_id);
            DEBUG_ECHOLNPAIR(" - however, we've requested screen ", displayRequest);
            UNUSED(screen_id);
          } else {
            //const uint8_t dlen = tmp[2] << 1;  // Convert to Bytes. (Display works with words)
            //DEBUG_ECHOPAIR(" vp=", vp, " dlen=", dlen);
            DGUS_VP_Variable ramcopy;
            DEBUG_ECHOLNPAIR("VP received: ", vp , " - val ", tmp[3]);
            if (populate_VPVar(vp, &ramcopy)) {
              if (ramcopy.set_by_display_handler)
                ramcopy.set_by_display_handler(ramcopy, &tmp[3]);
              else
                DEBUG_ECHOLNPGM(" VPVar found, no handler.");
            }
            else
              DEBUG_ECHOLNPAIR(" VPVar not found:", vp);
          }

          rx_datagram_state = DGUS_IDLE;
          break;
        }

      // discard anything else
      rx_datagram_state = DGUS_IDLE;
    }
  }
}

size_t DGUSDisplay::GetFreeTxBuffer() { return SERIAL_GET_TX_BUFFER_FREE(); }

void DGUSDisplay::WriteHeader(uint16_t adr, uint8_t cmd, uint8_t payloadlen) {
  dgusserial.write(DGUS_HEADER1);
  dgusserial.write(DGUS_HEADER2);
  dgusserial.write(payloadlen + 3);
  dgusserial.write(cmd);
  dgusserial.write(adr >> 8);
  dgusserial.write(adr & 0xFF);
}

void DGUSDisplay::WritePGM(const char str[], uint8_t len) {
  while (len--) dgusserial.write(pgm_read_byte(str++));
}

void DGUSDisplay::loop() {
  // protect against recursion… ProcessRx() may indirectly call idle() when injecting gcode commands.
  if (!no_reentrance) {
    no_reentrance = true;
    ProcessRx();
    no_reentrance = false;
  }
}

void DGUSDisplay::RequestScreen(DGUSLCD_Screens screen) {
  displayRequest = screen;

  DEBUG_ECHOLNPAIR("GotoScreen ", screen);
  const unsigned char gotoscreen[] = { 0x5A, 0x01, (unsigned char) (screen >> 8U), (unsigned char) (screen & 0xFFU) };
  WriteVariable(0x84, gotoscreen, sizeof(gotoscreen));
}

void DGUSDisplay::SetTouchScreenConfiguration(bool enable_standby, bool enable_sound, uint8_t standby_brightness) {
  // Main configuration (System_Config)
  unsigned char cfg_bits = 0x0;
  cfg_bits |= 1UL << 5; // 5: load 22 touch file
  cfg_bits |= 1UL << 4; // 4: auto-upload should always be enabled
  if (enable_sound) cfg_bits |= 1UL << 3; // 3: audio
  if (enable_standby) cfg_bits |= 1UL << 2; // 2: backlight on standby
  cfg_bits |= 1UL << 1; // 1 & 0: 270 degrees orientation of display
  cfg_bits |= 1UL << 0; 

  DEBUG_ECHOLNPAIR("Update touch screen config - standby ", enable_standby);
  DEBUG_ECHOLNPAIR("Update touch screen config - sound ", enable_sound);

  const unsigned char config_set[] = { 0x5A, 0x00, (unsigned char) (cfg_bits >> 8U), (unsigned char) (cfg_bits & 0xFFU) };
  WriteVariable(0x80 /*System_Config*/, config_set, sizeof(config_set));

  // Standby brightness (LED_Config)
  const unsigned char brightness_set[] = { 100 /*% active*/,  standby_brightness /*% standby*/ };
  WriteVariable(0x82 /*LED_Config*/, brightness_set, sizeof(brightness_set));
}

rx_datagram_state_t DGUSDisplay::rx_datagram_state = DGUS_IDLE;
uint8_t DGUSDisplay::rx_datagram_len = 0;
bool DGUSDisplay::Initialized = false;
bool DGUSDisplay::no_reentrance = false;
DGUSLCD_Screens DGUSDisplay::displayRequest = DGUSLCD_SCREEN_BOOT;

// A SW memory barrier, to ensure GCC does not overoptimize loops
#define sw_barrier() asm volatile("": : :"memory");

bool populate_VPVar(const uint16_t VP, DGUS_VP_Variable * const ramcopy) {
  //DEBUG_ECHOLNPAIR("populate_VPVar ", VP);
  const DGUS_VP_Variable *pvp = DGUSLCD_FindVPVar(VP);
  // DEBUG_ECHOLNPAIR(" pvp ", (uint16_t )pvp);
  if (!pvp) return false;
  memcpy_P(ramcopy, pvp, sizeof(DGUS_VP_Variable));
  return true;
}

#endif // HAS_DGUS_LCD
