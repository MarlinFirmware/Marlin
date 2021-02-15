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

#include "../../inc/MarlinConfig.h"

#if HAS_PRUSA_MMU2

#include "mmu2.h"
#include "../../lcd/menu/menu_mmu2.h"

MMU2 mmu2;

#include "../../gcode/gcode.h"
#include "../../lcd/marlinui.h"
#include "../../libs/buzzer.h"
#include "../../libs/nozzle.h"
#include "../../module/temperature.h"
#include "../../module/planner.h"
#include "../../module/stepper/indirection.h"
#include "../../MarlinCore.h"

#if ENABLED(HOST_PROMPT_SUPPORT)
  #include "../../feature/host_actions.h"
#endif

#if ENABLED(EXTENSIBLE_UI)
  #include "../../lcd/extui/ui_api.h"
#endif

#define DEBUG_OUT ENABLED(MMU2_DEBUG)
#include "../../core/debug_out.h"

#define MMU_TODELAY 100
#define MMU_TIMEOUT 10
#define MMU_CMD_TIMEOUT 45000UL // 45s timeout for mmu commands (except P0)
#define MMU_P0_TIMEOUT 3000UL   // Timeout for P0 command: 3seconds

#define MMU2_COMMAND(S) tx_str_P(PSTR(S "\n"))

#if ENABLED(MMU_EXTRUDER_SENSOR)
  uint8_t mmu_idl_sens = 0;
  static bool mmu_loading_flag = false;
#endif

#define MMU_CMD_NONE 0
#define MMU_CMD_T0   0x10
#define MMU_CMD_T1   0x11
#define MMU_CMD_T2   0x12
#define MMU_CMD_T3   0x13
#define MMU_CMD_T4   0x14
#define MMU_CMD_L0   0x20
#define MMU_CMD_L1   0x21
#define MMU_CMD_L2   0x22
#define MMU_CMD_L3   0x23
#define MMU_CMD_L4   0x24
#define MMU_CMD_C0   0x30
#define MMU_CMD_U0   0x40
#define MMU_CMD_E0   0x50
#define MMU_CMD_E1   0x51
#define MMU_CMD_E2   0x52
#define MMU_CMD_E3   0x53
#define MMU_CMD_E4   0x54
#define MMU_CMD_R0   0x60
#define MMU_CMD_F0   0x70
#define MMU_CMD_F1   0x71
#define MMU_CMD_F2   0x72
#define MMU_CMD_F3   0x73
#define MMU_CMD_F4   0x74

#define MMU_REQUIRED_FW_BUILDNR TERN(MMU2_MODE_12V, 132, 126)

#define MMU2_NO_TOOL 99
#define MMU_BAUD    115200

bool MMU2::enabled, MMU2::ready, MMU2::mmu_print_saved;
#if HAS_PRUSA_MMU2S
  bool MMU2::mmu2s_triggered;
#endif
uint8_t MMU2::cmd, MMU2::cmd_arg, MMU2::last_cmd, MMU2::extruder;
int8_t MMU2::state = 0;
volatile int8_t MMU2::finda = 1;
volatile bool MMU2::finda_runout_valid;
int16_t MMU2::version = -1, MMU2::buildnr = -1;
millis_t MMU2::prev_request, MMU2::prev_P0_request;
char MMU2::rx_buffer[MMU_RX_SIZE], MMU2::tx_buffer[MMU_TX_SIZE];

struct E_Step {
  float extrude;        //!< extrude distance in mm
  feedRate_t feedRate;  //!< feed rate in mm/s
};

static constexpr E_Step
    ramming_sequence[] PROGMEM = { MMU2_RAMMING_SEQUENCE }
  , load_to_nozzle_sequence[] PROGMEM = { MMU2_LOAD_TO_NOZZLE_SEQUENCE }
  #if HAS_PRUSA_MMU2S
    , can_load_sequence[] PROGMEM = { MMU2_CAN_LOAD_SEQUENCE }
    , can_load_increment_sequence[] PROGMEM = { MMU2_CAN_LOAD_INCREMENT_SEQUENCE }
  #endif
;

MMU2::MMU2() {
  rx_buffer[0] = '\0';
}

void MMU2::init() {

  set_runout_valid(false);

  #if PIN_EXISTS(MMU2_RST)
    WRITE(MMU2_RST_PIN, HIGH);
    SET_OUTPUT(MMU2_RST_PIN);
  #endif

  MMU2_SERIAL.begin(MMU_BAUD);
  extruder = MMU2_NO_TOOL;

  safe_delay(10);
  reset();
  rx_buffer[0] = '\0';
  state = -1;
}

void MMU2::reset() {
  DEBUG_ECHOLNPGM("MMU <= reset");

  #if PIN_EXISTS(MMU2_RST)
    WRITE(MMU2_RST_PIN, LOW);
    safe_delay(20);
    WRITE(MMU2_RST_PIN, HIGH);
  #else
    MMU2_COMMAND("X0"); // Send soft reset
  #endif
}

uint8_t MMU2::get_current_tool() {
  return extruder == MMU2_NO_TOOL ? -1 : extruder;
}

#if EITHER(HAS_PRUSA_MMU2S, MMU_EXTRUDER_SENSOR)
  #define FILAMENT_PRESENT() (READ(FIL_RUNOUT1_PIN) != FIL_RUNOUT1_STATE)
#endif

void MMU2::mmu_loop() {

  switch (state) {

    case 0: break;

    case -1:
      if (rx_start()) {
        prev_P0_request = millis();   // Initialize finda sensor timeout

        DEBUG_ECHOLNPGM("MMU => 'start'");
        DEBUG_ECHOLNPGM("MMU <= 'S1'");

        MMU2_COMMAND("S1");   // Read Version
        state = -2;
      }
      else if (millis() > 3000000) {
        SERIAL_ECHOLNPGM("MMU not responding - DISABLED");
        state = 0;
      }
      break;

    case -2:
      if (rx_ok()) {
        sscanf(rx_buffer, "%huok\n", &version);

        DEBUG_ECHOLNPAIR("MMU => ", version, "\nMMU <= 'S2'");

        MMU2_COMMAND("S2");   // Read Build Number
        state = -3;
      }
      break;

    case -3:
      if (rx_ok()) {
        sscanf(rx_buffer, "%huok\n", &buildnr);

        DEBUG_ECHOLNPAIR("MMU => ", buildnr);

        check_version();

        #if ENABLED(MMU2_MODE_12V)
          DEBUG_ECHOLNPGM("MMU <= 'M1'");

          MMU2_COMMAND("M1");   // Stealth Mode
          state = -5;

        #else
          DEBUG_ECHOLNPGM("MMU <= 'P0'");

          MMU2_COMMAND("P0");   // Read FINDA
          state = -4;
        #endif
      }
      break;

    #if ENABLED(MMU2_MODE_12V)
    case -5:
      // response to M1
      if (rx_ok()) {
        DEBUG_ECHOLNPGM("MMU => ok");

        DEBUG_ECHOLNPGM("MMU <= 'P0'");

        MMU2_COMMAND("P0");   // Read FINDA
        state = -4;
      }
      break;
    #endif

    case -4:
      if (rx_ok()) {
        sscanf(rx_buffer, "%hhuok\n", &finda);

        DEBUG_ECHOLNPAIR("MMU => ", finda, "\nMMU - ENABLED");

        enabled = true;
        state = 1;
        TERN_(HAS_PRUSA_MMU2S, mmu2s_triggered = false);
      }
      break;

    case 1:
      if (cmd) {
        if (WITHIN(cmd, MMU_CMD_T0, MMU_CMD_T4)) {
          // tool change
          int filament = cmd - MMU_CMD_T0;
          DEBUG_ECHOLNPAIR("MMU <= T", filament);
          tx_printf_P(PSTR("T%d\n"), filament);
          TERN_(MMU_EXTRUDER_SENSOR, mmu_idl_sens = 1); // enable idler sensor, if any
          state = 3; // wait for response
        }
        else if (WITHIN(cmd, MMU_CMD_L0, MMU_CMD_L4)) {
          // load
          int filament = cmd - MMU_CMD_L0;
          DEBUG_ECHOLNPAIR("MMU <= L", filament);
          tx_printf_P(PSTR("L%d\n"), filament);
          state = 3; // wait for response
        }
        else if (cmd == MMU_CMD_C0) {
          // continue loading
          DEBUG_ECHOLNPGM("MMU <= 'C0'");
          MMU2_COMMAND("C0");
          state = 3; // wait for response
        }
        else if (cmd == MMU_CMD_U0) {
          // unload current
          DEBUG_ECHOLNPGM("MMU <= 'U0'");

          MMU2_COMMAND("U0");
          state = 3; // wait for response
        }
        else if (WITHIN(cmd, MMU_CMD_E0, MMU_CMD_E4)) {
          // eject filament
          int filament = cmd - MMU_CMD_E0;
          DEBUG_ECHOLNPAIR("MMU <= E", filament);
          tx_printf_P(PSTR("E%d\n"), filament);
          state = 3; // wait for response
        }
        else if (cmd == MMU_CMD_R0) {
          // recover after eject
          DEBUG_ECHOLNPGM("MMU <= 'R0'");
          MMU2_COMMAND("R0");
          state = 3; // wait for response
        }
        else if (WITHIN(cmd, MMU_CMD_F0, MMU_CMD_F4)) {
          // filament type
          int filament = cmd - MMU_CMD_F0;
          DEBUG_ECHOLNPAIR("MMU <= F", filament, " ", cmd_arg);
          tx_printf_P(PSTR("F%d %d\n"), filament, cmd_arg);
          state = 3; // wait for response
        }

        last_cmd = cmd;
        cmd = MMU_CMD_NONE;
      }
      else if (ELAPSED(millis(), prev_P0_request + 300)) {
        MMU2_COMMAND("P0"); // Read FINDA
        state = 2; // wait for response
      }

      TERN_(HAS_PRUSA_MMU2S, check_filament());
      break;

    case 2:   // response to command P0
      if (rx_ok()) {
        sscanf(rx_buffer, "%hhuok\n", &finda);

        // This is super annoying. Only activate if necessary
        // if (finda_runout_valid) DEBUG_ECHOLNPAIR_F("MMU <= 'P0'\nMMU => ", finda, 6);

        if (!finda && finda_runout_valid) filament_runout();
        if (cmd == MMU_CMD_NONE) ready = true;
        state = 1;
      }
      else if (ELAPSED(millis(), prev_request + MMU_P0_TIMEOUT)) // Resend request after timeout (3s)
        state = 1;

      TERN_(HAS_PRUSA_MMU2S, check_filament());
      break;

    case 3:   // response to mmu commands
      #if ENABLED(MMU_EXTRUDER_SENSOR)
        if (mmu_idl_sens) {
          if (FILAMENT_PRESENT() && mmu_loading_flag) {
            DEBUG_ECHOLNPGM("MMU <= 'A'");
            MMU2_COMMAND("A"); // send 'abort' request
            mmu_idl_sens = 0;
            DEBUG_ECHOLNPGM("MMU IDLER_SENSOR = 0 - ABORT");
          }
        }
      #endif

      if (rx_ok()) {
        #if HAS_PRUSA_MMU2S
          // Respond to C0 MMU command in MMU2S model
          const bool keep_trying = !mmu2s_triggered && last_cmd == MMU_CMD_C0;
          if (keep_trying) {
            // MMU ok received but filament sensor not triggered, retrying...
            DEBUG_ECHOLNPGM("MMU => 'ok' (filament not present in gears)");
            DEBUG_ECHOLNPGM("MMU <= 'C0' (keep trying)");
            MMU2_COMMAND("C0");
          }
        #else
          constexpr bool keep_trying = false;
        #endif

        if (!keep_trying) {
          DEBUG_ECHOLNPGM("MMU => 'ok'");
          ready = true;
          state = 1;
          last_cmd = MMU_CMD_NONE;
        }
      }
      else if (ELAPSED(millis(), prev_request + MMU_CMD_TIMEOUT)) {
        // resend request after timeout
        if (last_cmd) {
          DEBUG_ECHOLNPGM("MMU retry");
          cmd = last_cmd;
          last_cmd = MMU_CMD_NONE;
        }
        state = 1;
      }
      TERN_(HAS_PRUSA_MMU2S, check_filament());
      break;
  }
}

/**
 * Check if MMU was started
 */
bool MMU2::rx_start() {
  // check for start message
  return rx_str_P(PSTR("start\n"));
}

/**
 * Check if the data received ends with the given string.
 */
bool MMU2::rx_str_P(const char* str) {
  uint8_t i = strlen(rx_buffer);

  while (MMU2_SERIAL.available()) {
    rx_buffer[i++] = MMU2_SERIAL.read();

    if (i == sizeof(rx_buffer) - 1) {
      DEBUG_ECHOLNPGM("rx buffer overrun");
      break;
    }
  }
  rx_buffer[i] = '\0';

  uint8_t len = strlen_P(str);

  if (i < len) return false;

  str += len;

  while (len--) {
    char c0 = pgm_read_byte(str--), c1 = rx_buffer[i--];
    if (c0 == c1) continue;
    if (c0 == '\r' && c1 == '\n') continue;  // match cr as lf
    if (c0 == '\n' && c1 == '\r') continue;  // match lf as cr
    return false;
  }
  return true;
}

/**
 * Transfer data to MMU, no argument
 */
void MMU2::tx_str_P(const char* str) {
  clear_rx_buffer();
  uint8_t len = strlen_P(str);
  LOOP_L_N(i, len) MMU2_SERIAL.write(pgm_read_byte(str++));
  prev_request = millis();
}

/**
 * Transfer data to MMU, single argument
 */
void MMU2::tx_printf_P(const char* format, int argument = -1) {
  clear_rx_buffer();
  uint8_t len = sprintf_P(tx_buffer, format, argument);
  LOOP_L_N(i, len) MMU2_SERIAL.write(tx_buffer[i]);
  prev_request = millis();
}

/**
 * Transfer data to MMU, two arguments
 */
void MMU2::tx_printf_P(const char* format, int argument1, int argument2) {
  clear_rx_buffer();
  uint8_t len = sprintf_P(tx_buffer, format, argument1, argument2);
  LOOP_L_N(i, len) MMU2_SERIAL.write(tx_buffer[i]);
  prev_request = millis();
}

/**
 * Empty the rx buffer
 */
void MMU2::clear_rx_buffer() {
  while (MMU2_SERIAL.available()) MMU2_SERIAL.read();
  rx_buffer[0] = '\0';
}

/**
 * Check if we received 'ok' from MMU
 */
bool MMU2::rx_ok() {
  if (rx_str_P(PSTR("ok\n"))) {
    prev_P0_request = millis();
    return true;
  }
  return false;
}

/**
 * Check if MMU has compatible firmware
 */
void MMU2::check_version() {
  if (buildnr < MMU_REQUIRED_FW_BUILDNR) {
    SERIAL_ERROR_MSG("Invalid MMU2 firmware. Version >= " STRINGIFY(MMU_REQUIRED_FW_BUILDNR) " required.");
    kill(GET_TEXT(MSG_KILL_MMU2_FIRMWARE));
  }
}

static void mmu2_not_responding() {
  LCD_MESSAGEPGM(MSG_MMU2_NOT_RESPONDING);
  BUZZ(100, 659);
  BUZZ(200, 698);
  BUZZ(100, 659);
  BUZZ(300, 440);
  BUZZ(100, 659);
}

#if HAS_PRUSA_MMU2S

  bool MMU2::load_to_gears() {
    command(MMU_CMD_C0);
    manage_response(true, true);
    LOOP_L_N(i, MMU2_C0_RETRY) {  // Keep loading until filament reaches gears
      if (mmu2s_triggered) break;
      command(MMU_CMD_C0);
      manage_response(true, true);
      check_filament();
    }
    const bool success = mmu2s_triggered && can_load();
    if (!success) mmu2_not_responding();
    return success;
  }

  /**
   * Handle tool change
   */
  void MMU2::tool_change(const uint8_t index) {

    if (!enabled) return;

    set_runout_valid(false);

    if (index != extruder) {

      DISABLE_AXIS_E0();
      ui.status_printf_P(0, GET_TEXT(MSG_MMU2_LOADING_FILAMENT), int(index + 1));

      command(MMU_CMD_T0 + index);
      manage_response(true, true);

      if (load_to_gears()) {
        extruder = index; // filament change is finished
        active_extruder = 0;
        ENABLE_AXIS_E0();
        SERIAL_ECHO_MSG(STR_ACTIVE_EXTRUDER, extruder);
      }
      ui.reset_status();
    }

    set_runout_valid(true);
  }

  /**
   * Handle special T?/Tx/Tc commands
   *
   * T? Gcode to extrude shouldn't have to follow, load to extruder wheels is done automatically
   * Tx Same as T?, except nozzle doesn't have to be preheated. Tc must be placed after extruder nozzle is preheated to finish filament load.
   * Tc Load to nozzle after filament was prepared by Tx and extruder nozzle is already heated.
   */
  void MMU2::tool_change(const char* special) {
      if (!enabled) return;

      set_runout_valid(false);

      switch (*special) {
        case '?': {
          #if ENABLED(MMU2_MENUS)
            const uint8_t index = mmu2_choose_filament();
            while (!thermalManager.wait_for_hotend(active_extruder, false)) safe_delay(100);
            load_filament_to_nozzle(index);
          #else
            BUZZ(400, 40);
          #endif
        } break;

        case 'x': {
          #if ENABLED(MMU2_MENUS)
            planner.synchronize();
            const uint8_t index = mmu2_choose_filament();
            DISABLE_AXIS_E0();
            command(MMU_CMD_T0 + index);
            manage_response(true, true);

            if (load_to_gears()) {
              mmu_loop();
              ENABLE_AXIS_E0();
              extruder = index;
              active_extruder = 0;
            }
          #else
            BUZZ(400, 40);
          #endif
        } break;

        case 'c': {
          while (!thermalManager.wait_for_hotend(active_extruder, false)) safe_delay(100);
          load_to_nozzle();
        } break;
      }

      set_runout_valid(true);
  }

#elif ENABLED(MMU_EXTRUDER_SENSOR)

  /**
   * Handle tool change
   */
  void MMU2::tool_change(const uint8_t index) {
    if (!enabled) return;

    set_runout_valid(false);

    if (index != extruder) {
      DISABLE_AXIS_E0();
      if (FILAMENT_PRESENT()) {
        DEBUG_ECHOLNPGM("Unloading\n");
        mmu_loading_flag = false;
        command(MMU_CMD_U0);
        manage_response(true, true);
      }
      ui.status_printf_P(0, GET_TEXT(MSG_MMU2_LOADING_FILAMENT), int(index + 1));
      mmu_loading_flag = true;
      command(MMU_CMD_T0 + index);
      manage_response(true, true);
      mmu_continue_loading();
      command(MMU_CMD_C0);
      extruder = index;
      active_extruder = 0;

      ENABLE_AXIS_E0();
      SERIAL_ECHO_MSG(STR_ACTIVE_EXTRUDER, extruder);

      ui.reset_status();
    }

    set_runout_valid(true);
  }

  /**
   * Handle special T?/Tx/Tc commands
   *
   * T? Gcode to extrude shouldn't have to follow, load to extruder wheels is done automatically
   * Tx Same as T?, except nozzle doesn't have to be preheated. Tc must be placed after extruder nozzle is preheated to finish filament load.
   * Tc Load to nozzle after filament was prepared by Tx and extruder nozzle is already heated.
   */
  void MMU2::tool_change(const char* special) {
    if (!enabled) return;

    set_runout_valid(false);

    switch (*special) {
      case '?': {
        DEBUG_ECHOLNPGM("case ?\n");
        #if ENABLED(MMU2_MENUS)
          uint8_t index = mmu2_choose_filament();
          while (!thermalManager.wait_for_hotend(active_extruder, false)) safe_delay(100);
          load_filament_to_nozzle(index);
        #else
          BUZZ(400, 40);
        #endif
      } break;

      case 'x': {
        DEBUG_ECHOLNPGM("case x\n");
        #if ENABLED(MMU2_MENUS)
          planner.synchronize();
          uint8_t index = mmu2_choose_filament();
          DISABLE_AXIS_E0();
          command(MMU_CMD_T0 + index);
          manage_response(true, true);
          mmu_continue_loading();
          command(MMU_CMD_C0);
          mmu_loop();

          ENABLE_AXIS_E0();
          extruder = index;
          active_extruder = 0;
        #else
          BUZZ(400, 40);
        #endif
      } break;

      case 'c': {
        DEBUG_ECHOLNPGM("case c\n");
        while (!thermalManager.wait_for_hotend(active_extruder, false)) safe_delay(100);
        execute_extruder_sequence((const E_Step *)load_to_nozzle_sequence, COUNT(load_to_nozzle_sequence));
      } break;
    }

    set_runout_valid(true);
  }

  void MMU2::mmu_continue_loading() {
    for (uint8_t i = 0; i < MMU_LOADING_ATTEMPTS_NR; i++) {
      DEBUG_ECHOLNPAIR("Additional load attempt #", i);
      if (FILAMENT_PRESENT()) break;
      command(MMU_CMD_C0);
      manage_response(true, true);
    }
    if (!FILAMENT_PRESENT()) {
      DEBUG_ECHOLNPGM("Filament never reached sensor, runout");
      filament_runout();
    }
    mmu_idl_sens = 0;
  }

#else // !HAS_PRUSA_MMU2S && !MMU_EXTRUDER_SENSOR

  /**
   * Handle tool change
   */
  void MMU2::tool_change(const uint8_t index) {
    if (!enabled) return;

    set_runout_valid(false);

    if (index != extruder) {
      DISABLE_AXIS_E0();
      ui.status_printf_P(0, GET_TEXT(MSG_MMU2_LOADING_FILAMENT), int(index + 1));
      command(MMU_CMD_T0 + index);
      manage_response(true, true);
      command(MMU_CMD_C0);
      extruder = index; //filament change is finished
      active_extruder = 0;
      ENABLE_AXIS_E0();
      SERIAL_ECHO_MSG(STR_ACTIVE_EXTRUDER, extruder);
      ui.reset_status();
    }

    set_runout_valid(true);
  }

  /**
   * Handle special T?/Tx/Tc commands
   *
   * T? Gcode to extrude shouldn't have to follow, load to extruder wheels is done automatically
   * Tx Same as T?, except nozzle doesn't have to be preheated. Tc must be placed after extruder nozzle is preheated to finish filament load.
   * Tc Load to nozzle after filament was prepared by Tx and extruder nozzle is already heated.
   */
  void MMU2::tool_change(const char* special) {
    if (!enabled) return;

    set_runout_valid(false);

    switch (*special) {
      case '?': {
        DEBUG_ECHOLNPGM("case ?\n");
        #if ENABLED(MMU2_MENUS)
          uint8_t index = mmu2_choose_filament();
          while (!thermalManager.wait_for_hotend(active_extruder, false)) safe_delay(100);
          load_filament_to_nozzle(index);
        #else
          BUZZ(400, 40);
        #endif
      } break;

      case 'x': {
        DEBUG_ECHOLNPGM("case x\n");
        #if ENABLED(MMU2_MENUS)
          planner.synchronize();
          uint8_t index = mmu2_choose_filament();
          DISABLE_AXIS_E0();
          command(MMU_CMD_T0 + index);
          manage_response(true, true);
          command(MMU_CMD_C0);
          mmu_loop();

          ENABLE_AXIS_E0();
          extruder = index;
          active_extruder = 0;
        #else
          BUZZ(400, 40);
        #endif
      } break;

      case 'c': {
        DEBUG_ECHOLNPGM("case c\n");
        while (!thermalManager.wait_for_hotend(active_extruder, false)) safe_delay(100);
        execute_extruder_sequence((const E_Step *)load_to_nozzle_sequence, COUNT(load_to_nozzle_sequence));
      } break;
    }

    set_runout_valid(true);
  }

#endif // HAS_PRUSA_MMU2S

/**
 * Set next command
 */
void MMU2::command(const uint8_t mmu_cmd) {
  if (!enabled) return;
  cmd = mmu_cmd;
  ready = false;
}

/**
 * Wait for response from MMU
 */
bool MMU2::get_response() {
  while (cmd != MMU_CMD_NONE) idle();

  while (!ready) {
    idle();
    if (state != 3) break;
  }

  const bool ret = ready;
  ready = false;

  return ret;
}

/**
 * Wait for response and deal with timeout if necessary
 */
void MMU2::manage_response(const bool move_axes, const bool turn_off_nozzle) {

  constexpr xyz_pos_t park_point = NOZZLE_PARK_POINT;
  bool response = false;
  mmu_print_saved = false;
  xyz_pos_t resume_position;
  int16_t resume_hotend_temp = thermalManager.degTargetHotend(active_extruder);

  KEEPALIVE_STATE(PAUSED_FOR_USER);

  while (!response) {

    response = get_response(); // wait for "ok" from mmu

    if (!response) {          // No "ok" was received in reserved time frame, user will fix the issue on mmu unit
      if (!mmu_print_saved) { // First occurrence. Save current position, park print head, disable nozzle heater.

        planner.synchronize();

        mmu_print_saved = true;

        SERIAL_ECHOLNPGM("MMU not responding");

        resume_hotend_temp = thermalManager.degTargetHotend(active_extruder);
        resume_position = current_position;

        if (move_axes && all_axes_homed())
          nozzle.park(0, park_point /*= NOZZLE_PARK_POINT*/);

        if (turn_off_nozzle) thermalManager.setTargetHotend(0, active_extruder);

        mmu2_not_responding();
      }
    }
    else if (mmu_print_saved) {
      SERIAL_ECHOLNPGM("MMU starts responding\n");

      if (turn_off_nozzle && resume_hotend_temp) {
        thermalManager.setTargetHotend(resume_hotend_temp, active_extruder);
        LCD_MESSAGEPGM(MSG_HEATING);
        BUZZ(200, 40);

        while (!thermalManager.wait_for_hotend(active_extruder, false)) safe_delay(1000);
      }

      if (move_axes && all_axes_homed()) {
        LCD_MESSAGEPGM(MSG_MMU2_RESUMING);
        BUZZ(198, 404); BUZZ(4, 0); BUZZ(198, 404);

        // Move XY to starting position, then Z
        do_blocking_move_to_xy(resume_position, feedRate_t(NOZZLE_PARK_XY_FEEDRATE));

        // Move Z_AXIS to saved position
        do_blocking_move_to_z(resume_position.z, feedRate_t(NOZZLE_PARK_Z_FEEDRATE));
      }
      else {
        BUZZ(198, 404); BUZZ(4, 0); BUZZ(198, 404);
        LCD_MESSAGEPGM(MSG_MMU2_RESUMING);
      }
    }
  }
}

void MMU2::set_filament_type(const uint8_t index, const uint8_t filamentType) {
  if (!enabled) return;

  cmd_arg = filamentType;
  command(MMU_CMD_F0 + index);

  manage_response(true, true);
}

void MMU2::filament_runout() {
  queue.inject_P(PSTR(MMU2_FILAMENT_RUNOUT_SCRIPT));
  planner.synchronize();
}

#if HAS_PRUSA_MMU2S

  void MMU2::check_filament() {
    const bool present = FILAMENT_PRESENT();
    if (cmd == MMU_CMD_NONE && last_cmd == MMU_CMD_C0) {
      if (present && !mmu2s_triggered) {
        DEBUG_ECHOLNPGM("MMU <= 'A'");
        tx_str_P(PSTR("A\n"));
      }
      // Slowly spin the extruder during C0
      else {
        while (planner.movesplanned() < 3) {
          current_position.e += 0.25;
          line_to_current_position(MMM_TO_MMS(120));
        }
      }
    }
    mmu2s_triggered = present;
  }

  bool MMU2::can_load() {
    execute_extruder_sequence((const E_Step *)can_load_sequence, COUNT(can_load_sequence));

    int filament_detected_count = 0;
    const int steps = (MMU2_CAN_LOAD_RETRACT) / (MMU2_CAN_LOAD_INCREMENT);
    DEBUG_ECHOLNPGM("MMU can_load:");
    LOOP_L_N(i, steps) {
      execute_extruder_sequence((const E_Step *)can_load_increment_sequence, COUNT(can_load_increment_sequence));
      check_filament(); // Don't trust the idle function
      DEBUG_CHAR(mmu2s_triggered ? 'O' : 'o');
      if (mmu2s_triggered) ++filament_detected_count;
    }

    if (filament_detected_count <= steps - (MMU2_CAN_LOAD_DEVIATION) / (MMU2_CAN_LOAD_INCREMENT)) {
      DEBUG_ECHOLNPGM(" failed.");
      return false;
    }

    DEBUG_ECHOLNPGM(" succeeded.");
    return true;
  }

#endif

// Load filament into MMU2
void MMU2::load_filament(const uint8_t index) {
  if (!enabled) return;

  command(MMU_CMD_L0 + index);
  manage_response(false, false);
  BUZZ(200, 404);
}

/**
 * Switch material and load to nozzle
 */
bool MMU2::load_filament_to_nozzle(const uint8_t index) {

  if (!enabled) return false;

  if (thermalManager.tooColdToExtrude(active_extruder)) {
    BUZZ(200, 404);
    LCD_ALERTMESSAGEPGM(MSG_HOTEND_TOO_COLD);
    return false;
  }

  DISABLE_AXIS_E0();
  command(MMU_CMD_T0 + index);
  manage_response(true, true);

  const bool success = load_to_gears();
  if (success) {
    mmu_loop();
    extruder = index;
    active_extruder = 0;
    load_to_nozzle();
    BUZZ(200, 404);
  }
  return success;
}

/**
 * Load filament to nozzle of multimaterial printer
 *
 * This function is used only after T? (user select filament) and M600 (change filament).
 * It is not used after T0 .. T4 command (select filament), in such case, gcode is responsible for loading
 * filament to nozzle.
 */
void MMU2::load_to_nozzle() {
  execute_extruder_sequence((const E_Step *)load_to_nozzle_sequence, COUNT(load_to_nozzle_sequence));
}

bool MMU2::eject_filament(const uint8_t index, const bool recover) {

  if (!enabled) return false;

  if (thermalManager.tooColdToExtrude(active_extruder)) {
    BUZZ(200, 404);
    LCD_ALERTMESSAGEPGM(MSG_HOTEND_TOO_COLD);
    return false;
  }

  LCD_MESSAGEPGM(MSG_MMU2_EJECTING_FILAMENT);

  ENABLE_AXIS_E0();
  current_position.e -= MMU2_FILAMENTCHANGE_EJECT_FEED;
  line_to_current_position(MMM_TO_MMS(2500));
  planner.synchronize();
  command(MMU_CMD_E0 + index);
  manage_response(false, false);

  if (recover)  {
    LCD_MESSAGEPGM(MSG_MMU2_EJECT_RECOVER);
    BUZZ(200, 404);
    TERN_(HOST_PROMPT_SUPPORT, host_prompt_do(PROMPT_USER_CONTINUE, PSTR("MMU2 Eject Recover"), CONTINUE_STR));
    TERN_(EXTENSIBLE_UI, ExtUI::onUserConfirmRequired_P(PSTR("MMU2 Eject Recover")));
    wait_for_user_response();
    BUZZ(200, 404);
    BUZZ(200, 404);

    command(MMU_CMD_R0);
    manage_response(false, false);
  }

  ui.reset_status();

  // no active tool
  extruder = MMU2_NO_TOOL;

  set_runout_valid(false);

  BUZZ(200, 404);

  DISABLE_AXIS_E0();

  return true;
}

/**
 * Unload from hotend and retract to MMU
 */
bool MMU2::unload() {

  if (!enabled) return false;

  if (thermalManager.tooColdToExtrude(active_extruder)) {
    BUZZ(200, 404);
    LCD_ALERTMESSAGEPGM(MSG_HOTEND_TOO_COLD);
    return false;
  }

  // Unload sequence to optimize shape of the tip of the unloaded filament
  execute_extruder_sequence((const E_Step *)ramming_sequence, sizeof(ramming_sequence) / sizeof(E_Step));

  command(MMU_CMD_U0);
  manage_response(false, true);

  BUZZ(200, 404);

  // no active tool
  extruder = MMU2_NO_TOOL;

  set_runout_valid(false);

  return true;
}

void MMU2::execute_extruder_sequence(const E_Step * sequence, int steps) {

  planner.synchronize();
  ENABLE_AXIS_E0();

  const E_Step* step = sequence;

  LOOP_L_N(i, steps) {
    const float es = pgm_read_float(&(step->extrude));
    const feedRate_t fr_mm_m = pgm_read_float(&(step->feedRate));

    DEBUG_ECHO_START();
    DEBUG_ECHOLNPAIR("E step ", es, "/", fr_mm_m);

    current_position.e += es;
    line_to_current_position(MMM_TO_MMS(fr_mm_m));
    planner.synchronize();

    step++;
  }

  DISABLE_AXIS_E0();
}

#endif // HAS_PRUSA_MMU2
