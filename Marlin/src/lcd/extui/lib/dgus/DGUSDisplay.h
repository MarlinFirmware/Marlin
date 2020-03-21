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
#pragma once

/* DGUS implementation written by coldtobi in 2019 for Marlin */

#include "../../../../inc/MarlinConfigPre.h"

#include "../../../../MarlinCore.h"

#include "DGUSVPVariable.h"
#include "DGUSDisplayDef.h"

#define DEBUG_OUT ENABLED(DEBUG_DGUSLCD)
#include "../../../../core/debug_out.h"

// Low-Level access to the display.
class DGUSDisplay {
public:

  DGUSDisplay() = default;

  static void Init();

  // Variable access.
  static void WriteVariable(uint16_t adr, const void* values, uint8_t valueslen, bool isstr = false);
  static void WriteVariablePGM(uint16_t adr, const void* values, uint8_t valueslen, bool isstr = false);

  template<typename T>
  static void WriteVariable(uint16_t adr, T value) {
    WriteVariable(adr, static_cast<const void*>(&value), sizeof(T));
  }

  // Until now I did not need to actively read from the display. That's why there is no ReadVariable
  // (I extensively use the auto upload of the display)

  // Force display into another screen.
  static void SwitchScreen(DGUS_Screen screen);
  // Set the display brightness, ranging 0x00 - 0x64
  static void SetBrightness(uint8_t brightness);
  // Play sounds using the display speaker.
  //   start: position at which the sound was stored on the display.
  //   len: how many sounds to play. Sounds will play consecutively from start to start+len-1.
  //   volume: playback volume. 0 keeps the current volume.
  static void PlaySound(uint8_t start, uint8_t len = 1, uint8_t volume = 0);
  static void SetVolume(uint8_t volume);

  // Periodic tasks, eg. Rx-Queue handling.
  static void Loop();

  // Helper for users of this class to estimate if an interaction would be blocking.
  static size_t GetFreeTxBuffer();

  // Checks two things: Can we confirm the presence of the display and has we initiliazed it.
  // (both boils down that the display answered to our chatting)
  static inline bool IsInitialized() {
    return initialized;
  }

private:
  enum dgus_header : uint8_t {
    DGUS_HEADER1 = 0x5A,
    DGUS_HEADER2 = 0xA5
  };

  enum dgus_command : uint8_t {
    DGUS_WRITEVAR = 0x82,
    DGUS_READVAR = 0x83
  };

  enum rx_datagram_state_t : uint8_t {
    DGUS_IDLE,           //< waiting for DGUS_HEADER1.
    DGUS_HEADER1_SEEN,   //< DGUS_HEADER1 received
    DGUS_HEADER2_SEEN,   //< DGUS_HEADER2 received
    DGUS_WAIT_TELEGRAM,  //< LEN received, Waiting for to receive all bytes.
  };

  static void WriteHeader(uint16_t adr, uint8_t cmd, uint8_t payloadlen);
  static void ProcessRx();

  static rx_datagram_state_t rx_datagram_state;
  static uint8_t rx_datagram_len;

  static bool initialized;
  static bool no_reentrance;
};

extern DGUSDisplay dgus_display;

/// Find the flash address of the VP list for a screen.
extern const uint16_t* DGUS_FindScreenVPMapList(uint8_t screen);

/// Find the flash address of a DGUS_VP_Variable for the VP.
extern const DGUS_VP_Variable* DGUS_FindVPVar(const uint16_t vp);

/// Helper to populate a DGUS_VP_Variable for a given VP. Return false if not found.
extern bool DGUS_PopulateVPVar(const uint16_t VP, DGUS_VP_Variable * const ramcopy);
