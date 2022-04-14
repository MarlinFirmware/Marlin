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
#pragma once

/**
 * lcd/extui/dgus/DGUSDisplay.h
 */

#include "../../../inc/MarlinConfigPre.h"

#include <stdlib.h>    // size_t

#if HAS_BED_PROBE
  #include "../../../module/probe.h"
#endif
#include "DGUSVPVariable.h"

enum DGUSLCD_Screens : uint8_t;

//#define DEBUG_DGUSLCD
#define DEBUG_OUT ENABLED(DEBUG_DGUSLCD)
#include "../../../core/debug_out.h"

typedef enum : uint8_t {
  DGUS_IDLE,           //< waiting for DGUS_HEADER1.
  DGUS_HEADER1_SEEN,   //< DGUS_HEADER1 received
  DGUS_HEADER2_SEEN,   //< DGUS_HEADER2 received
  DGUS_WAIT_TELEGRAM,  //< LEN received, Waiting for to receive all bytes.
} rx_datagram_state_t;

// Low-Level access to the display.
class DGUSDisplay {
public:

  DGUSDisplay() = default;

  static void InitDisplay();

  // Variable access.
  static void WriteVariable(uint16_t adr, const void *values, uint8_t valueslen, bool isstr=false);
  static void WriteVariablePGM(uint16_t adr, const void *values, uint8_t valueslen, bool isstr=false);
  static void WriteVariable(uint16_t adr, int16_t value);
  static void WriteVariable(uint16_t adr, uint16_t value);
  static void WriteVariable(uint16_t adr, uint8_t value);
  static void WriteVariable(uint16_t adr, int8_t value);
  static void WriteVariable(uint16_t adr, long value);
  static void MKS_WriteVariable(uint16_t adr, uint8_t value);


  // Utility functions for bridging ui_api and dbus
  template<typename T, float(*Getter)(const T), T selector, typename WireType=uint16_t>
  static void SetVariable(DGUS_VP_Variable &var) {
    WriteVariable(var.VP, (WireType)Getter(selector));
  }

  template<typename T, void(*Setter)(const float V, const T), T selector>
  static void GetVariable(DGUS_VP_Variable &var, void *val_ptr) {
    uint16_t newvalue = swap16(*(uint16_t*)val_ptr);
    Setter(newvalue, selector);
  }

  // Until now I did not need to actively read from the display. That's why there is no ReadVariable
  // (I extensively use the auto upload of the display)

  // Force display into another screen.
  // (And trigger update of containing VPs)
  // (to implement a pop up message, which may not be nested)
  static void RequestScreen(DGUSLCD_Screens screen);

  // Periodic tasks, eg. Rx-Queue handling.
  static void loop();

public:
  // Helper for users of this class to estimate if an interaction would be blocking.
  static size_t GetFreeTxBuffer();

  // Checks two things: Can we confirm the presence of the display and has we initialized it.
  // (both boils down that the display answered to our chatting)
  static bool isInitialized() { return Initialized; }

private:
  static void WriteHeader(uint16_t adr, uint8_t cmd, uint8_t payloadlen);
  static void WritePGM(const char str[], uint8_t len);
  static void ProcessRx();

  static uint16_t swap16(const uint16_t value) { return (value & 0xFFU) << 8U | (value >> 8U); }
  static rx_datagram_state_t rx_datagram_state;
  static uint8_t rx_datagram_len;
  static bool Initialized, no_reentrance;
};

extern DGUSDisplay dgusdisplay;

// compile-time x^y
constexpr float cpow(const float x, const int y) { return y == 0 ? 1.0 : x * cpow(x, y - 1); }

///
const uint16_t* DGUSLCD_FindScreenVPMapList(uint8_t screen);

/// Find the flash address of a DGUS_VP_Variable for the VP.
const DGUS_VP_Variable* DGUSLCD_FindVPVar(const uint16_t vp);

/// Helper to populate a DGUS_VP_Variable for a given VP. Return false if not found.
bool populate_VPVar(const uint16_t VP, DGUS_VP_Variable * const ramcopy);
