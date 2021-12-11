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
#pragma once

/* DGUS implementation written by coldtobi in 2019 for Marlin */

#include "config/DGUS_Screen.h"
#include "config/DGUS_Control.h"
#include "definition/DGUS_VP.h"

#include "../../../inc/MarlinConfigPre.h"
#include "../../../MarlinCore.h"

#define DEBUG_OUT ENABLED(DEBUG_DGUSLCD)
#include "../../../core/debug_out.h"

#define Swap16(val) ((uint16_t)(((uint16_t)(val) >> 8) |\
                                ((uint16_t)(val) << 8)))

// Low-Level access to the display.
class DGUSDisplay {
public:

  enum DGUS_ControlType : uint8_t {
    VARIABLE_DATA_INPUT = 0x00,
    POPUP_WINDOW = 0x01,
    INCREMENTAL_ADJUST = 0x02,
    SLIDER_ADJUST = 0x03,
    RTC_SETTINGS = 0x04,
    RETURN_KEY_CODE = 0x05,
    TEXT_INPUT = 0x06,
    FIRMWARE_SETTINGS = 0x07
  };

  DGUSDisplay() = default;

  static void Init();

  static void Read(uint16_t addr, uint8_t size);
  static void Write(uint16_t addr, const void* data_ptr, uint8_t size);

  static void WriteString(uint16_t addr, const void* data_ptr, uint8_t size, bool left = true, bool right = false, bool use_space = true);
  static void WriteStringPGM(uint16_t addr, const void* data_ptr, uint8_t size, bool left = true, bool right = false, bool use_space = true);

  template<typename T>
  static void Write(uint16_t addr, T data) {
    Write(addr, static_cast<const void*>(&data), sizeof(T));
  }

  // Until now I did not need to actively read from the display. That's why there is no ReadVariable
  // (I extensively use the auto upload of the display)

  // Force display into another screen.
  static void SwitchScreen(DGUS_Screen screen);
  // Play sounds using the display speaker.
  //   start: position at which the sound was stored on the display.
  //   len: how many sounds to play. Sounds will play consecutively from start to start+len-1.
  //   volume: playback volume. 0 keeps the current volume.
  static void PlaySound(uint8_t start, uint8_t len = 1, uint8_t volume = 0);
  // Enable/disable a specific touch control.
  //   type: control type.
  //   control: index of the control on the page (set during screen development).
  static void EnableControl(DGUS_Screen screen, DGUS_ControlType type, DGUS_Control control);
  static void DisableControl(DGUS_Screen screen, DGUS_ControlType type, DGUS_Control control);

  static uint8_t GetBrightness();
  static uint8_t GetVolume();

  // Set the display brightness/volume, ranging 0 - 100
  static void SetBrightness(uint8_t brightness);
  static void SetVolume(uint8_t volume);

  // Periodic tasks, eg. Rx-Queue handling.
  static void Loop();

  // Helper for users of this class to estimate if an interaction would be blocking.
  static size_t GetFreeTxBuffer();
  static void FlushTx();

  // Checks two things: Can we confirm the presence of the display and has we initialized it.
  // (both boils down that the display answered to our chatting)
  static inline bool IsInitialized() {
    return initialized;
  }

  static uint8_t gui_version;
  static uint8_t os_version;

  template<typename T>
  static T SwapBytes(const T value) {
    union {
      T val;
      char byte[sizeof(T)];
    } src, dst;

    src.val = value;
    LOOP_L_N(i, sizeof(T)) dst.byte[i] = src.byte[sizeof(T) - i - 1];
    return dst.val;
  }

  template<typename T_in, typename T_out, uint8_t decimals>
  T_out FromFixedPoint(const T_in value) {
    return (T_out)((float)value / POW(10, decimals));
  }

  template<typename T_in, typename T_out, uint8_t decimals>
  T_out ToFixedPoint(const T_in value) {
    return (T_out)LROUND((float)value * POW(10, decimals));
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

  enum dgus_system_addr : uint16_t {
    DGUS_VERSION = 0x000f // OS/GUI version
  };

  static void WriteHeader(uint16_t addr, uint8_t command, uint8_t len);
  static void ProcessRx();

  static uint8_t volume;
  static uint8_t brightness;

  static rx_datagram_state_t rx_datagram_state;
  static uint8_t rx_datagram_len;

  static bool initialized;
};

template<> inline uint16_t DGUSDisplay::SwapBytes(const uint16_t value) {
  return ((value << 8) | (value >> 8));
}

extern DGUSDisplay dgus_display;

/// Helper to populate a DGUS_VP for a given VP. Return false if not found.
extern bool DGUS_PopulateVP(const DGUS_Addr addr, DGUS_VP * const buffer);
