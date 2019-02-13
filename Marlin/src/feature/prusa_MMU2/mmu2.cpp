/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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

#include "../../inc/MarlinConfig.h"

#if ENABLED(PRUSA_MMU2)

#include "mmu2.h"
#include "../../lcd/menu/menu_mmu2.h"

MMU2 mmu2;

#include "../../gcode/gcode.h"
#include "../../lcd/ultralcd.h"
#include "../../libs/buzzer.h"
#include "../../libs/nozzle.h"
#include "../../module/temperature.h"
#include "../../module/planner.h"
#include "../../module/stepper_indirection.h"
#include "../../Marlin.h"

#if ENABLED(HOST_PROMPT_SUPPORT)
  #include "../../feature/host_actions.h"
#endif

#define MMU_TODELAY 100
#define MMU_TIMEOUT 10
#define MMU_CMD_TIMEOUT 60000ul //5min timeout for mmu commands (except P0)
#define MMU_P0_TIMEOUT 3000ul //timeout for P0 command: 3seconds

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

#if ENABLED(MMU2_MODE_12V)
  #define MMU_REQUIRED_FW_BUILDNR 132
#else
  #define MMU_REQUIRED_FW_BUILDNR 126
#endif

#define MMU2_NO_TOOL 99
#define MMU_BAUD    115200

#define mmuSerial   MMU2_SERIAL

bool MMU2::enabled, MMU2::ready, MMU2::mmu_print_saved;
uint8_t MMU2::cmd, MMU2::cmd_arg, MMU2::last_cmd, MMU2::extruder;
int8_t MMU2::state = 0;
volatile int8_t MMU2::finda = 1;
volatile bool MMU2::findaRunoutValid;
int16_t MMU2::version = -1, MMU2::buildnr = -1;
millis_t MMU2::last_request, MMU2::next_P0_request;
char MMU2::rx_buffer[16], MMU2::tx_buffer[16];

#if HAS_LCD_MENU && ENABLED(MMU2_MENUS)

  struct E_Step {
    float extrude;    //!< extrude distance in mm
    float feedRate;   //!< feed rate in mm/s
  };

  static constexpr E_Step ramming_sequence[] PROGMEM = { MMU2_RAMMING_SEQUENCE };
  static constexpr E_Step loadToNozzle_sequence[] PROGMEM = { MMU2_LOAD_TO_NOZZLE_SEQUENCE };

#endif // MMU2_MENUS

MMU2::MMU2() {
  rx_buffer[0] = '\0';
}

void MMU2::init() {

  set_runout_valid(false);

  #if PIN_EXISTS(MMU2_RST)
    // TODO use macros for this
    WRITE(MMU2_RST_PIN, HIGH);
    SET_OUTPUT(MMU2_RST_PIN);
  #endif

  mmuSerial.begin(MMU_BAUD);
  extruder = MMU2_NO_TOOL;

  safe_delay(10);
  reset();
  rx_buffer[0] = '\0';
  state = -1;
}

void MMU2::reset() {
  #if ENABLED(MMU2_DEBUG)
    SERIAL_ECHOLNPGM("MMU <= reset");
  #endif

  #if PIN_EXISTS(MMU2_RST)
    WRITE(MMU2_RST_PIN, LOW);
    safe_delay(20);
    WRITE(MMU2_RST_PIN, HIGH);
  #else
    tx_str_P(PSTR("X0\n")); // Send soft reset
  #endif
}

uint8_t MMU2::getCurrentTool() {
  return extruder == MMU2_NO_TOOL ? -1 : extruder;
}

void MMU2::mmuLoop() {

  switch (state) {

    case 0: break;

    case -1:
      if (rx_start()) {
        #if ENABLED(MMU2_DEBUG)
          SERIAL_ECHOLNPGM("MMU => 'start'");
          SERIAL_ECHOLNPGM("MMU <= 'S1'");
        #endif

        // send "read version" request
        tx_str_P(PSTR("S1\n"));

        state = -2;
      }
      else if (millis() > 3000000) {
        SERIAL_ECHOLNPGM("MMU not responding - DISABLED");
        state = 0;
      }
      break;

    case -2:
      if (rx_ok()) {
        sscanf(rx_buffer, "%uok\n", &version);

        #if ENABLED(MMU2_DEBUG)
          SERIAL_ECHOLNPAIR("MMU => ", version);
          SERIAL_ECHOLNPGM("MMU <= 'S2'");
        #endif

        tx_str_P(PSTR("S2\n")); // read build number
        state = -3;
      }
      break;

    case -3:
      if (rx_ok()) {
        sscanf(rx_buffer, "%uok\n", &buildnr);
        #if ENABLED(MMU2_DEBUG)
          SERIAL_ECHOLNPAIR("MMU => ", buildnr);
        #endif

        checkVersion();

        #if ENABLED(MMU2_MODE_12V)
          #if ENABLED(MMU2_DEBUG)
            SERIAL_ECHOLNPGM("MMU <= 'M1'");
          #endif

          tx_str_P(PSTR("M1\n")); // switch to stealth mode
          state = -5;

        #else
          #if ENABLED(MMU2_DEBUG)
            SERIAL_ECHOLNPGM("MMU <= 'P0'");
          #endif

          tx_str_P(PSTR("P0\n")); // read finda
          state = -4;
        #endif
      }
      break;

    case -5:
      // response to M1
      if (rx_ok()) {
        #if ENABLED(MMU2_DEBUG)
          SERIAL_ECHOLNPGM("MMU => ok");
        #endif

        checkVersion();

        #if ENABLED(MMU2_DEBUG)
          SERIAL_ECHOLNPGM("MMU <= 'P0'");
        #endif

        tx_str_P(PSTR("P0\n")); // read finda
        state = -4;
      }
      break;

    case -4:
      if (rx_ok()) {
        sscanf(rx_buffer, "%hhuok\n", &finda);

        #if ENABLED(MMU2_DEBUG)
          SERIAL_ECHOLNPAIR("MMU => ", finda);
          SERIAL_ECHOLNPGM("MMU - ENABLED");
        #endif

        enabled = true;
        state = 1;
      }
      break;

    case 1:
      if (cmd) {
        if (WITHIN(cmd, MMU_CMD_T0, MMU_CMD_T4)) {
          // tool change
          int filament = cmd - MMU_CMD_T0;

          #if ENABLED(MMU2_DEBUG)
            SERIAL_ECHOLNPAIR("MMU <= T", filament);
          #endif

          tx_printf_P(PSTR("T%d\n"), filament);
          state = 3; // wait for response
        }
        else if (WITHIN(cmd, MMU_CMD_L0, MMU_CMD_L4)) {
          // load
          int filament = cmd - MMU_CMD_L0;

          #if ENABLED(MMU2_DEBUG)
            SERIAL_ECHOLNPAIR("MMU <= L", filament);
          #endif

          tx_printf_P(PSTR("L%d\n"), filament);
          state = 3; // wait for response
        }
        else if (cmd == MMU_CMD_C0) {
          // continue loading

          #if ENABLED(MMU2_DEBUG)
            SERIAL_ECHOLNPGM("MMU <= 'C0'");
          #endif

          tx_str_P(PSTR("C0\n"));
          state = 3; // wait for response
        }
        else if (cmd == MMU_CMD_U0) {
          // unload current
          #if ENABLED(MMU2_DEBUG)
            SERIAL_ECHOLNPGM("MMU <= 'U0'");
          #endif

          tx_str_P(PSTR("U0\n"));
          state = 3; // wait for response
        }
        else if (WITHIN(cmd, MMU_CMD_E0, MMU_CMD_E4)) {
          // eject filament
          int filament = cmd - MMU_CMD_E0;

          #if ENABLED(MMU2_DEBUG)
            SERIAL_ECHOLNPAIR("MMU <= E", filament);
          #endif
          tx_printf_P(PSTR("E%d\n"), filament);
          state = 3; // wait for response
        }
        else if (cmd == MMU_CMD_R0) {
          // recover after eject
          #if ENABLED(MMU2_DEBUG)
            SERIAL_ECHOLNPGM("MMU <= 'R0'");
          #endif

          tx_str_P(PSTR("R0\n"));
          state = 3; // wait for response
        }
        else if (WITHIN(cmd, MMU_CMD_F0, MMU_CMD_F4)) {
          // filament type
          int filament = cmd - MMU_CMD_F0;
          #if ENABLED(MMU2_DEBUG)
            SERIAL_ECHOPAIR("MMU <= F", filament);
            SERIAL_ECHOPGM(" ");
            SERIAL_ECHO_F(cmd_arg, DEC);
            SERIAL_ECHOPGM("\n");
          #endif

          tx_printf_P(PSTR("F%d %d\n"), filament, cmd_arg);
          state = 3; // wait for response
        }

        last_cmd = cmd;
        cmd = MMU_CMD_NONE;
      }
      else if (ELAPSED(millis(), next_P0_request)) {
        // read FINDA
        tx_str_P(PSTR("P0\n"));
        state = 2; // wait for response
      }
      break;

    case 2:   // response to command P0
      if (rx_ok()) {
        sscanf(rx_buffer, "%hhuok\n", &finda);

        #if ENABLED(MMU2_DEBUG)
          // This is super annoying. Only activate if necessary
          /*
            if (findaRunoutValid) {
              SERIAL_ECHOLNPGM("MMU <= 'P0'");
              SERIAL_ECHOPGM("MMU => ");
              SERIAL_ECHO_F(finda, DEC);
              SERIAL_ECHOPGM("\n");
            }
          */
        #endif

        state = 1;

        if (cmd == 0) ready = true;

        if (!finda && findaRunoutValid) filamentRunout();
      }
      else if (ELAPSED(millis(), last_request + MMU_P0_TIMEOUT)) // Resend request after timeout (30s)
        state = 1;

      break;

    case 3:   // response to mmu commands
      if (rx_ok()) {
        #if ENABLED(MMU2_DEBUG)
          SERIAL_ECHOLNPGM("MMU => 'ok'");
        #endif

        ready = true;
        state = 1;
        last_cmd = MMU_CMD_NONE;
      }
      else if (ELAPSED(millis(), last_request + MMU_CMD_TIMEOUT)) {
        // resend request after timeout
        if (last_cmd) {
          #if ENABLED(MMU2_DEBUG)
            SERIAL_ECHOLNPGM("MMU retry");
          #endif

          cmd = last_cmd;
          last_cmd = MMU_CMD_NONE;
        }
        state = 1;
      }
      break;
  }
}


/**
 * Check if MMU was started
 */
bool MMU2::rx_start() {
  // check for start message
  if (rx_str_P(PSTR("start\n"))) {
    next_P0_request = millis() + 300;
    return true;
  }
  return false;
}


/**
 * Check if the data received ends with the given string.
 */
bool MMU2::rx_str_P(const char* str) {
  uint8_t i = strlen(rx_buffer);

  while (mmuSerial.available()) {
    rx_buffer[i++] = mmuSerial.read();
    rx_buffer[i] = '\0';

    if (i == sizeof(rx_buffer) - 1) {
      #if ENABLED(MMU2_DEBUG)
        SERIAL_ECHOLNPGM("rx buffer overrun");
      #endif

      break;
    }
  }

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
  for (uint8_t i = 0; i < len; i++) mmuSerial.write(pgm_read_byte(str++));
  rx_buffer[0] = '\0';
  last_request = millis();
}


/**
 * Transfer data to MMU, single argument
 */
void MMU2::tx_printf_P(const char* format, int argument = -1) {
  clear_rx_buffer();
  uint8_t len = sprintf_P(tx_buffer, format, argument);
  for (uint8_t i = 0; i < len; i++) mmuSerial.write(tx_buffer[i]);
  rx_buffer[0] = '\0';
  last_request = millis();
}


/**
 * Transfer data to MMU, two arguments
 */
void MMU2::tx_printf_P(const char* format, int argument1, int argument2) {
  clear_rx_buffer();
  uint8_t len = sprintf_P(tx_buffer, format, argument1, argument2);
  for (uint8_t i = 0; i < len; i++) mmuSerial.write(tx_buffer[i]);
  rx_buffer[0] = '\0';
  last_request = millis();
}


/**
 * Empty the rx buffer
 */
void MMU2::clear_rx_buffer() {
  while (mmuSerial.available()) mmuSerial.read();
  rx_buffer[0] = '\0';
}


/**
 * Check if we received 'ok' from MMU
 */
bool MMU2::rx_ok() {
  if (rx_str_P(PSTR("ok\n"))) {
    next_P0_request = millis() + 300;
    return true;
  }
  return false;
}


/**
 * Check if MMU has compatible firmware
 */
void MMU2::checkVersion() {
  if (buildnr < MMU_REQUIRED_FW_BUILDNR) {
    SERIAL_ERROR_START();
    SERIAL_ECHOPGM("MMU2 firmware version invalid. Required version >= ");
    SERIAL_ECHOLN(MMU_REQUIRED_FW_BUILDNR);
    kill(MSG_MMU2_WRONG_FIRMWARE);
  }
}


/**
 * Handle tool change
 */
void MMU2::toolChange(uint8_t index) {

  if (!enabled) return;

  set_runout_valid(false);

  if (index != extruder) {

    KEEPALIVE_STATE(IN_HANDLER);
    disable_E0();
    ui.status_printf_P(0, PSTR(MSG_MMU2_LOADING_FILAMENT), int(index + 1));

    command(MMU_CMD_T0 + index);

    manageResponse(true, true);
    KEEPALIVE_STATE(IN_HANDLER);

    command(MMU_CMD_C0);
    extruder = index; //filament change is finished
    active_extruder = 0;

    enable_E0();

    SERIAL_ECHO_START();
    SERIAL_ECHOLNPAIR(MSG_ACTIVE_EXTRUDER, int(extruder));

    ui.reset_status();
    KEEPALIVE_STATE(NOT_BUSY);
  }

  set_runout_valid(true);
}


/**
 *
 * Handle special T?/Tx/Tc commands
 *
 * T? Gcode to extrude shouldn't have to follow, load to extruder wheels is done automatically
 * Tx Same as T?, except nozzle doesn't have to be preheated. Tc must be placed after extruder nozzle is preheated to finish filament load.
 * Tc Load to nozzle after filament was prepared by Tx and extruder nozzle is already heated.
 *
 */
void MMU2::toolChange(const char* special) {

  if (!enabled) return;

  #if ENABLED(MMU2_MENUS)

    set_runout_valid(false);
    KEEPALIVE_STATE(IN_HANDLER);

    switch(*special) {
      case '?': {
        uint8_t index = mmu2_chooseFilament();
        while (!thermalManager.wait_for_hotend(active_extruder, false)) safe_delay(100);
        loadFilamentToNozzle(index);
      } break;

      case 'x': {
        planner.synchronize();
        uint8_t index = mmu2_chooseFilament();
        disable_E0();
        command(MMU_CMD_T0 + index);
        manageResponse(true, true);
        command(MMU_CMD_C0);
        mmuLoop();

        enable_E0();
        extruder = index;
        active_extruder = 0;
      } break;

      case 'c': {
        while (!thermalManager.wait_for_hotend(active_extruder, false)) safe_delay(100);
        executeExtruderSequence((const E_Step *)loadToNozzle_sequence, COUNT(loadToNozzle_sequence));
      } break;
    }

    KEEPALIVE_STATE(NOT_BUSY);

    set_runout_valid(true);

  #endif
}


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
bool MMU2::getResponse(void) {
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
 * Wait for response and deal with timeout if nexcessary
 */
void MMU2::manageResponse(bool move_axes, bool turn_off_nozzle) {

  bool response = false;
  mmu_print_saved = false;
  point_t park_point = NOZZLE_PARK_POINT;
  float resume_position[XYZE];
  int16_t resume_hotend_temp;

  while (!response) {

    response = getResponse(); //wait for "ok" from mmu

    if (!response) { //no "ok" was received in reserved time frame, user will fix the issue on mmu unit
      if (!mmu_print_saved) { //first occurence, we are saving current position, park print head in certain position and disable nozzle heater

        planner.synchronize();

        mmu_print_saved = true;

        SERIAL_ECHOLNPGM("MMU not responding");

        resume_hotend_temp = thermalManager.degTargetHotend(active_extruder);
        COPY(resume_position, current_position);

        if (move_axes && all_axes_homed())
          Nozzle::park(2, park_point /*= NOZZLE_PARK_POINT*/);

        if (turn_off_nozzle) thermalManager.setTargetHotend(0, active_extruder);

        LCD_MESSAGEPGM(MSG_MMU2_NOT_RESPONDING);
        BUZZ(100, 659);
        BUZZ(200, 698);
        BUZZ(100, 659);
        BUZZ(300, 440);
        BUZZ(100, 659);

        KEEPALIVE_STATE(PAUSED_FOR_USER);
      }
    }
    else if (mmu_print_saved) {
      SERIAL_ECHOLNPGM("MMU starts responding\n");
      KEEPALIVE_STATE(IN_HANDLER);

      if (turn_off_nozzle && resume_hotend_temp) {
        thermalManager.setTargetHotend(resume_hotend_temp, active_extruder);
        LCD_MESSAGEPGM(MSG_HEATING);
        BUZZ(200, 40);

        while (!thermalManager.wait_for_hotend(active_extruder, false)) safe_delay(1000);
      }

      if (move_axes && all_axes_homed()) {
        LCD_MESSAGEPGM(MSG_MMU2_RESUMING);
        BUZZ(200, 404);
        BUZZ(200, 404);

        // Move XY to starting position, then Z
        do_blocking_move_to_xy(resume_position[X_AXIS], resume_position[Y_AXIS], NOZZLE_PARK_XY_FEEDRATE);

        // Move Z_AXIS to saved position
        do_blocking_move_to_z(resume_position[Z_AXIS], NOZZLE_PARK_Z_FEEDRATE);
      }
      else {
        BUZZ(200, 404);
        BUZZ(200, 404);
        LCD_MESSAGEPGM(MSG_MMU2_RESUMING);
      }
    }
  }
}

void MMU2::setFilamentType(uint8_t index, uint8_t filamentType) {
  if (!enabled) return;

  KEEPALIVE_STATE(IN_HANDLER);

  cmd_arg = filamentType;
  command(MMU_CMD_F0 + index);

  manageResponse(true, true);

  KEEPALIVE_STATE(NOT_BUSY);
}

void MMU2::filamentRunout() {
  enqueue_and_echo_commands_P(PSTR(MMU2_FILAMENT_RUNOUT_SCRIPT));
  planner.synchronize();
}

#if HAS_LCD_MENU && ENABLED(MMU2_MENUS)

  // Load filament into MMU2
  void MMU2::loadFilament(uint8_t index) {
    if (!enabled) return;
    command(MMU_CMD_L0 + index);
    manageResponse(false, false);
    BUZZ(200, 404);
  }

  /**
   *
   * Switch material and load to nozzle
   *
   */
  bool MMU2::loadFilamentToNozzle(uint8_t index) {

    if (!enabled) return false;

    if (thermalManager.tooColdToExtrude(active_extruder)) {
      BUZZ(200, 404);
      LCD_ALERTMESSAGEPGM(MSG_HOTEND_TOO_COLD);
      return false;
    }
    else {
      KEEPALIVE_STATE(IN_HANDLER);

      command(MMU_CMD_T0 + index);
      manageResponse(true, true);
      command(MMU_CMD_C0);
      mmuLoop();

      extruder = index;
      active_extruder = 0;

      loadToNozzle();

      BUZZ(200, 404);

      KEEPALIVE_STATE(NOT_BUSY);
      return true;
    }
  }

  /**
   *
   * Load filament to nozzle of multimaterial printer
   *
   * This function is used only only after T? (user select filament) and M600 (change filament).
   * It is not used after T0 .. T4 command (select filament), in such case, gcode is responsible for loading
   * filament to nozzle.
   */
  void MMU2::loadToNozzle() {
    if (!enabled) return;
    executeExtruderSequence((const E_Step *)loadToNozzle_sequence, COUNT(loadToNozzle_sequence));
  }

  bool MMU2::ejectFilament(uint8_t index, bool recover) {

    if (!enabled) return false;

    if (thermalManager.tooColdToExtrude(active_extruder)) {
      BUZZ(200, 404);
      LCD_ALERTMESSAGEPGM(MSG_HOTEND_TOO_COLD);
      return false;
    }

    KEEPALIVE_STATE(IN_HANDLER);
    LCD_MESSAGEPGM(MSG_MMU2_EJECTING_FILAMENT);
    const bool saved_e_relative_mode = gcode.axis_relative_modes[E_AXIS];
    gcode.axis_relative_modes[E_AXIS] = true;

    enable_E0();
    current_position[E_AXIS] -= MMU2_FILAMENTCHANGE_EJECT_FEED;
    planner.buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS], current_position[E_AXIS], 2500 / 60, active_extruder);
    planner.synchronize();
    command(MMU_CMD_E0 + index);
    manageResponse(false, false);

    if (recover)  {
      LCD_MESSAGEPGM(MSG_MMU2_EJECT_RECOVER);
      BUZZ(200, 404);
      wait_for_user = true;
      #if ENABLED(HOST_PROMPT_SUPPORT)
        host_prompt_do(PROMPT_USER_CONTINUE, PSTR("MMU2 Eject Recover"), PSTR("Continue"));
      #endif
      while (wait_for_user) idle();
      BUZZ(200, 404);
      BUZZ(200, 404);

      command(MMU_CMD_R0);
      manageResponse(false, false);
    }

    ui.reset_status();

    // no active tool
    extruder = MMU2_NO_TOOL;

    set_runout_valid(false);

    BUZZ(200, 404);

    KEEPALIVE_STATE(NOT_BUSY);

    gcode.axis_relative_modes[E_AXIS] = saved_e_relative_mode;

    disable_E0();

    return true;
  }

  /**
   *
   * unload from hotend and retract to MMU
   *
   */
  bool MMU2::unload() {

    if (!enabled) return false;

    if (thermalManager.tooColdToExtrude(active_extruder)) {
      BUZZ(200, 404);
      LCD_ALERTMESSAGEPGM(MSG_HOTEND_TOO_COLD);
      return false;
    }

    KEEPALIVE_STATE(IN_HANDLER);

    filamentRamming();

    command(MMU_CMD_U0);
    manageResponse(false, true);

    BUZZ(200, 404);

    // no active tool
    extruder = MMU2_NO_TOOL;

    set_runout_valid(false);

    KEEPALIVE_STATE(NOT_BUSY);

    return true;
  }

  /**
   * Unload sequence to optimize shape of the tip of the unloaded filament
   */
  void MMU2::filamentRamming() {
    executeExtruderSequence((const E_Step *)ramming_sequence, sizeof(ramming_sequence) / sizeof(E_Step));
  }

  void MMU2::executeExtruderSequence(const E_Step * sequence, int steps) {

    planner.synchronize();
    enable_E0();

    const bool saved_e_relative_mode = gcode.axis_relative_modes[E_AXIS];
    gcode.axis_relative_modes[E_AXIS] = true;

    const E_Step* step = sequence;

    for (uint8_t i = 0; i < steps; i++) {
      const float es = pgm_read_float(&(step->extrude)),
                  fr = pgm_read_float(&(step->feedRate));

      #if ENABLED(MMU2_DEBUG)
        SERIAL_ECHO_START();
        SERIAL_ECHOPAIR("E step ", es);
        SERIAL_CHAR('/');
        SERIAL_ECHOLN(fr);
      #endif

      current_position[E_AXIS] += es;
      planner.buffer_line(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS],
                          current_position[E_AXIS], MMM_TO_MMS(fr), active_extruder);
      planner.synchronize();

      step++;
    }

    gcode.axis_relative_modes[E_AXIS] = saved_e_relative_mode;

    disable_E0();
  }

#endif // HAS_LCD_MENU && MMU2_MENUS

#endif // PRUSA_MMU2
