/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * endstops.cpp - A singleton object to manage endstops
 */

#include "Marlin.h"
#include "cardreader.h"
#include "endstops.h"
#include "temperature.h"
#include "stepper.h"
#include "ultralcd.h"

// TEST_ENDSTOP: test the old and the current status of an endstop
#define TEST_ENDSTOP(ENDSTOP) (TEST(current_endstop_bits & old_endstop_bits, ENDSTOP))

Endstops endstops;

// public:

bool  Endstops::enabled = true,
      Endstops::enabled_globally =
        #if ENABLED(ENDSTOPS_ALWAYS_ON_DEFAULT)
          (true)
        #else
          (false)
        #endif
      ;
volatile char Endstops::endstop_hit_bits; // use X_MIN, Y_MIN, Z_MIN and Z_MIN_PROBE as BIT value

#if ENABLED(Z_DUAL_ENDSTOPS)
  uint16_t
#else
  byte
#endif
    Endstops::current_endstop_bits = 0,
    Endstops::old_endstop_bits = 0;

#if HAS_BED_PROBE
  volatile bool Endstops::z_probe_enabled = false;
#endif

/**
 * Class and Instance Methods
 */

void Endstops::init() {

  #if HAS_X_MIN
    SET_INPUT(X_MIN_PIN);
    #if ENABLED(ENDSTOPPULLUP_XMIN)
      WRITE(X_MIN_PIN,HIGH);
    #endif
  #endif

  #if HAS_Y_MIN
    SET_INPUT(Y_MIN_PIN);
    #if ENABLED(ENDSTOPPULLUP_YMIN)
      WRITE(Y_MIN_PIN,HIGH);
    #endif
  #endif

  #if HAS_Z_MIN
    SET_INPUT(Z_MIN_PIN);
    #if ENABLED(ENDSTOPPULLUP_ZMIN)
      WRITE(Z_MIN_PIN,HIGH);
    #endif
  #endif

  #if HAS_Z2_MIN
    SET_INPUT(Z2_MIN_PIN);
    #if ENABLED(ENDSTOPPULLUP_ZMIN)
      WRITE(Z2_MIN_PIN,HIGH);
    #endif
  #endif

  #if HAS_X_MAX
    SET_INPUT(X_MAX_PIN);
    #if ENABLED(ENDSTOPPULLUP_XMAX)
      WRITE(X_MAX_PIN,HIGH);
    #endif
  #endif

  #if HAS_Y_MAX
    SET_INPUT(Y_MAX_PIN);
    #if ENABLED(ENDSTOPPULLUP_YMAX)
      WRITE(Y_MAX_PIN,HIGH);
    #endif
  #endif

  #if HAS_Z_MAX
    SET_INPUT(Z_MAX_PIN);
    #if ENABLED(ENDSTOPPULLUP_ZMAX)
      WRITE(Z_MAX_PIN,HIGH);
    #endif
  #endif

  #if HAS_Z2_MAX
    SET_INPUT(Z2_MAX_PIN);
    #if ENABLED(ENDSTOPPULLUP_ZMAX)
      WRITE(Z2_MAX_PIN,HIGH);
    #endif
  #endif

  #if ENABLED(Z_MIN_PROBE_ENDSTOP)
    SET_INPUT(Z_MIN_PROBE_PIN);
    #if ENABLED(ENDSTOPPULLUP_ZMIN_PROBE)
      WRITE(Z_MIN_PROBE_PIN,HIGH);
    #endif
  #endif

} // Endstops::init

void Endstops::report_state() {
  if (endstop_hit_bits) {
    #if ENABLED(ULTRA_LCD)
      char chrX = ' ', chrY = ' ', chrZ = ' ', chrP = ' ';
      #define _SET_STOP_CHAR(A,C) (chr## A = C)
    #else
      #define _SET_STOP_CHAR(A,C) ;
    #endif

    #define _ENDSTOP_HIT_ECHO(A,C) do{ \
      SERIAL_ECHOPAIR(" " STRINGIFY(A) ":", stepper.triggered_position_mm(A ##_AXIS)); \
      _SET_STOP_CHAR(A,C); }while(0)

    #define _ENDSTOP_HIT_TEST(A,C) \
      if (TEST(endstop_hit_bits, A ##_MIN) || TEST(endstop_hit_bits, A ##_MAX)) \
        _ENDSTOP_HIT_ECHO(A,C)

    SERIAL_ECHO_START;
    SERIAL_ECHOPGM(MSG_ENDSTOPS_HIT);
    _ENDSTOP_HIT_TEST(X, 'X');
    _ENDSTOP_HIT_TEST(Y, 'Y');
    _ENDSTOP_HIT_TEST(Z, 'Z');

    #if ENABLED(Z_MIN_PROBE_ENDSTOP)
      #define P_AXIS Z_AXIS
      if (TEST(endstop_hit_bits, Z_MIN_PROBE)) _ENDSTOP_HIT_ECHO(P, 'P');
    #endif
    SERIAL_EOL;

    #if ENABLED(ULTRA_LCD)
      char msg[3 * strlen(MSG_LCD_ENDSTOPS) + 8 + 1]; // Room for a UTF 8 string
      sprintf_P(msg, PSTR(MSG_LCD_ENDSTOPS " %c %c %c %c"), chrX, chrY, chrZ, chrP);
      lcd_setstatus(msg);
    #endif

    hit_on_purpose();

    #if ENABLED(ABORT_ON_ENDSTOP_HIT_FEATURE_ENABLED) && ENABLED(SDSUPPORT)
      if (stepper.abort_on_endstop_hit) {
        card.sdprinting = false;
        card.closefile();
        quickstop_stepper();
        thermalManager.disable_all_heaters(); // switch off all heaters.
      }
    #endif
  }
} // Endstops::report_state

void Endstops::M119() {
  SERIAL_PROTOCOLLNPGM(MSG_M119_REPORT);
  #if HAS_X_MIN
    SERIAL_PROTOCOLPGM(MSG_X_MIN);
    SERIAL_PROTOCOLLN(((READ(X_MIN_PIN)^X_MIN_ENDSTOP_INVERTING) ? MSG_ENDSTOP_HIT : MSG_ENDSTOP_OPEN));
  #endif
  #if HAS_X_MAX
    SERIAL_PROTOCOLPGM(MSG_X_MAX);
    SERIAL_PROTOCOLLN(((READ(X_MAX_PIN)^X_MAX_ENDSTOP_INVERTING) ? MSG_ENDSTOP_HIT : MSG_ENDSTOP_OPEN));
  #endif
  #if HAS_Y_MIN
    SERIAL_PROTOCOLPGM(MSG_Y_MIN);
    SERIAL_PROTOCOLLN(((READ(Y_MIN_PIN)^Y_MIN_ENDSTOP_INVERTING) ? MSG_ENDSTOP_HIT : MSG_ENDSTOP_OPEN));
  #endif
  #if HAS_Y_MAX
    SERIAL_PROTOCOLPGM(MSG_Y_MAX);
    SERIAL_PROTOCOLLN(((READ(Y_MAX_PIN)^Y_MAX_ENDSTOP_INVERTING) ? MSG_ENDSTOP_HIT : MSG_ENDSTOP_OPEN));
  #endif
  #if HAS_Z_MIN
    SERIAL_PROTOCOLPGM(MSG_Z_MIN);
    SERIAL_PROTOCOLLN(((READ(Z_MIN_PIN)^Z_MIN_ENDSTOP_INVERTING) ? MSG_ENDSTOP_HIT : MSG_ENDSTOP_OPEN));
  #endif
  #if HAS_Z2_MIN
    SERIAL_PROTOCOLPGM(MSG_Z2_MIN);
    SERIAL_PROTOCOLLN(((READ(Z2_MIN_PIN)^Z2_MIN_ENDSTOP_INVERTING) ? MSG_ENDSTOP_HIT : MSG_ENDSTOP_OPEN));
  #endif
  #if HAS_Z_MAX
    SERIAL_PROTOCOLPGM(MSG_Z_MAX);
    SERIAL_PROTOCOLLN(((READ(Z_MAX_PIN)^Z_MAX_ENDSTOP_INVERTING) ? MSG_ENDSTOP_HIT : MSG_ENDSTOP_OPEN));
  #endif
  #if HAS_Z2_MAX
    SERIAL_PROTOCOLPGM(MSG_Z2_MAX);
    SERIAL_PROTOCOLLN(((READ(Z2_MAX_PIN)^Z2_MAX_ENDSTOP_INVERTING) ? MSG_ENDSTOP_HIT : MSG_ENDSTOP_OPEN));
  #endif
  #if ENABLED(Z_MIN_PROBE_ENDSTOP)
    SERIAL_PROTOCOLPGM(MSG_Z_PROBE);
    SERIAL_PROTOCOLLN(((READ(Z_MIN_PROBE_PIN)^Z_MIN_PROBE_ENDSTOP_INVERTING) ? MSG_ENDSTOP_HIT : MSG_ENDSTOP_OPEN));
  #endif
} // Endstops::M119

#if ENABLED(Z_DUAL_ENDSTOPS)

  // Pass the result of the endstop test
  void Endstops::test_dual_z_endstops(const EndstopEnum es1, const EndstopEnum es2) {
    byte z_test = TEST_ENDSTOP(es1) | (TEST_ENDSTOP(es2) << 1); // bit 0 for Z, bit 1 for Z2
    if (z_test && stepper.current_block->steps[Z_AXIS] > 0) {
      SBI(endstop_hit_bits, Z_MIN);
      if (!stepper.performing_homing || (z_test == 0x3))  //if not performing home or if both endstops were trigged during homing...
        stepper.kill_current_block();
    }
  }

#endif

// Check endstops - Called from ISR!
void Endstops::update() {

  #define _ENDSTOP(AXIS, MINMAX) AXIS ##_## MINMAX
  #define _ENDSTOP_PIN(AXIS, MINMAX) AXIS ##_## MINMAX ##_PIN
  #define _ENDSTOP_INVERTING(AXIS, MINMAX) AXIS ##_## MINMAX ##_ENDSTOP_INVERTING
  #define _ENDSTOP_HIT(AXIS) SBI(endstop_hit_bits, _ENDSTOP(AXIS, MIN))

  // UPDATE_ENDSTOP_BIT: set the current endstop bits for an endstop to its status
  #define UPDATE_ENDSTOP_BIT(AXIS, MINMAX) SET_BIT(current_endstop_bits, _ENDSTOP(AXIS, MINMAX), (READ(_ENDSTOP_PIN(AXIS, MINMAX)) != _ENDSTOP_INVERTING(AXIS, MINMAX)))
  // COPY_BIT: copy the value of SRC_BIT to DST_BIT in DST
  #define COPY_BIT(DST, SRC_BIT, DST_BIT) SET_BIT(DST, DST_BIT, TEST(DST, SRC_BIT))

  #define _UPDATE_ENDSTOP(AXIS,MINMAX,CODE) do { \
      UPDATE_ENDSTOP_BIT(AXIS, MINMAX); \
      if (TEST_ENDSTOP(_ENDSTOP(AXIS, MINMAX)) && stepper.current_block->steps[_AXIS(AXIS)] > 0) { \
        _ENDSTOP_HIT(AXIS); \
        stepper.endstop_triggered(_AXIS(AXIS)); \
        CODE; \
      } \
    } while(0)

  #if ENABLED(G38_PROBE_TARGET) && PIN_EXISTS(Z_MIN)  // If G38 command then check Z_MIN for every axis and every direction

    #define UPDATE_ENDSTOP(AXIS,MINMAX) do { \
        _UPDATE_ENDSTOP(AXIS,MINMAX,NOOP); \
        if (G38_move) _UPDATE_ENDSTOP(Z, MIN, G38_endstop_hit = true); \
      } while(0)

  #else

    #define UPDATE_ENDSTOP(AXIS,MINMAX) _UPDATE_ENDSTOP(AXIS,MINMAX,NOOP)

  #endif

  #if CORE_IS_XY || CORE_IS_XZ
    // Head direction in -X axis for CoreXY and CoreXZ bots.
    // If DeltaA == -DeltaB, the movement is only in Y or Z axis
    if ((stepper.current_block->steps[CORE_AXIS_1] != stepper.current_block->steps[CORE_AXIS_2]) || (stepper.motor_direction(CORE_AXIS_1) == stepper.motor_direction(CORE_AXIS_2))) {
      if (stepper.motor_direction(X_HEAD))
  #else
    if (stepper.motor_direction(X_AXIS))   // stepping along -X axis (regular Cartesian bot)
  #endif
      { // -direction
        #if ENABLED(DUAL_X_CARRIAGE)
          // with 2 x-carriages, endstops are only checked in the homing direction for the active extruder
          if ((stepper.current_block->active_extruder == 0 && X_HOME_DIR == -1) || (stepper.current_block->active_extruder != 0 && X2_HOME_DIR == -1))
        #endif
          {
            #if HAS_X_MIN
              UPDATE_ENDSTOP(X, MIN);
            #endif
          }
      }
      else { // +direction
        #if ENABLED(DUAL_X_CARRIAGE)
          // with 2 x-carriages, endstops are only checked in the homing direction for the active extruder
          if ((stepper.current_block->active_extruder == 0 && X_HOME_DIR == 1) || (stepper.current_block->active_extruder != 0 && X2_HOME_DIR == 1))
        #endif
          {
            #if HAS_X_MAX
              UPDATE_ENDSTOP(X, MAX);
            #endif
          }
      }
  #if CORE_IS_XY || CORE_IS_XZ
    }
  #endif

  #if CORE_IS_XY || CORE_IS_YZ
    // Head direction in -Y axis for CoreXY / CoreYZ bots.
    // If DeltaA == DeltaB, the movement is only in X or Y axis
    if ((stepper.current_block->steps[CORE_AXIS_1] != stepper.current_block->steps[CORE_AXIS_2]) || (stepper.motor_direction(CORE_AXIS_1) != stepper.motor_direction(CORE_AXIS_2))) {
      if (stepper.motor_direction(Y_HEAD))
  #else
      if (stepper.motor_direction(Y_AXIS))   // -direction
  #endif
      { // -direction
        #if HAS_Y_MIN
          UPDATE_ENDSTOP(Y, MIN);
        #endif
      }
      else { // +direction
        #if HAS_Y_MAX
          UPDATE_ENDSTOP(Y, MAX);
        #endif
      }
  #if CORE_IS_XY || CORE_IS_YZ
    }
  #endif

  #if CORE_IS_XZ || CORE_IS_YZ
    // Head direction in -Z axis for CoreXZ or CoreYZ bots.
    // If DeltaA == DeltaB, the movement is only in X or Y axis
    if ((stepper.current_block->steps[CORE_AXIS_1] != stepper.current_block->steps[CORE_AXIS_2]) || (stepper.motor_direction(CORE_AXIS_1) != stepper.motor_direction(CORE_AXIS_2))) {
      if (stepper.motor_direction(Z_HEAD))
  #else
      if (stepper.motor_direction(Z_AXIS))
  #endif
      { // Z -direction. Gantry down, bed up.
        #if HAS_Z_MIN

          #if ENABLED(Z_DUAL_ENDSTOPS)

            UPDATE_ENDSTOP_BIT(Z, MIN);
            #if HAS_Z2_MIN
              UPDATE_ENDSTOP_BIT(Z2, MIN);
            #else
              COPY_BIT(current_endstop_bits, Z_MIN, Z2_MIN);
            #endif

            test_dual_z_endstops(Z_MIN, Z2_MIN);

          #else // !Z_DUAL_ENDSTOPS

            #if ENABLED(Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN)
              if (z_probe_enabled) UPDATE_ENDSTOP(Z, MIN);
            #else
              UPDATE_ENDSTOP(Z, MIN);
            #endif

          #endif // !Z_DUAL_ENDSTOPS

        #endif // HAS_Z_MIN

        // When closing the gap check the enabled probe
        #if ENABLED(Z_MIN_PROBE_ENDSTOP)
          if (z_probe_enabled) {
            UPDATE_ENDSTOP(Z, MIN_PROBE);
            if (TEST_ENDSTOP(Z_MIN_PROBE)) SBI(endstop_hit_bits, Z_MIN_PROBE);
          }
        #endif
      }
      else { // Z +direction. Gantry up, bed down.
        #if HAS_Z_MAX

          // Check both Z dual endstops
          #if ENABLED(Z_DUAL_ENDSTOPS)

            UPDATE_ENDSTOP_BIT(Z, MAX);
            #if HAS_Z2_MAX
              UPDATE_ENDSTOP_BIT(Z2, MAX);
            #else
              COPY_BIT(current_endstop_bits, Z_MAX, Z2_MAX);
            #endif

            test_dual_z_endstops(Z_MAX, Z2_MAX);

          // If this pin is not hijacked for the bed probe
          // then it belongs to the Z endstop
          #elif DISABLED(Z_MIN_PROBE_ENDSTOP) || Z_MAX_PIN != Z_MIN_PROBE_PIN

            UPDATE_ENDSTOP(Z, MAX);

          #endif // !Z_MIN_PROBE_PIN...
        #endif // Z_MAX_PIN
      }
  #if CORE_IS_XZ || CORE_IS_YZ
    }
  #endif

  old_endstop_bits = current_endstop_bits;

} // Endstops::update()
