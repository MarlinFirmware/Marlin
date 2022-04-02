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

#include "../../../inc/MarlinConfig.h"

#if HAS_STEALTHCHOP

#if AXIS_COLLISION('I')
  #error "M569 parameter collision with axis name."
#endif

#include "../../gcode.h"
#include "../../../feature/tmc_util.h"
#include "../../../module/stepper/indirection.h"

template<typename TMC>
void tmc_say_stealth_status(TMC &st) {
  st.printLabel();
  SERIAL_ECHOPGM(" driver mode:\t");
  SERIAL_ECHOLNF(st.get_stealthChop() ? F("stealthChop") : F("spreadCycle"));
}
template<typename TMC>
void tmc_set_stealthChop(TMC &st, const bool enable) {
  st.stored.stealthChop_enabled = enable;
  st.refresh_stepping_mode();
}

static void set_stealth_status(const bool enable, const int8_t eindex) {
  #define TMC_SET_STEALTH(Q) tmc_set_stealthChop(stepper##Q, enable)

  #if X2_HAS_STEALTHCHOP || Y2_HAS_STEALTHCHOP || Z2_HAS_STEALTHCHOP || Z3_HAS_STEALTHCHOP || Z4_HAS_STEALTHCHOP
    const int8_t index = parser.byteval('I', -1);
  #else
    constexpr int8_t index = -1;
  #endif

  LOOP_LOGICAL_AXES(i) if (parser.seen(axis_codes[i])) {
    switch (i) {
      case X_AXIS:
        TERN_(X_HAS_STEALTHCHOP,  if (index < 0 || index == 0) TMC_SET_STEALTH(X));
        TERN_(X2_HAS_STEALTHCHOP, if (index < 0 || index == 1) TMC_SET_STEALTH(X2));
        break;

      #if HAS_Y_AXIS
        case Y_AXIS:
          TERN_(Y_HAS_STEALTHCHOP,  if (index < 0 || index == 0) TMC_SET_STEALTH(Y));
          TERN_(Y2_HAS_STEALTHCHOP, if (index < 0 || index == 1) TMC_SET_STEALTH(Y2));
          break;
      #endif

      #if HAS_Z_AXIS
        case Z_AXIS:
          TERN_(Z_HAS_STEALTHCHOP,  if (index < 0 || index == 0) TMC_SET_STEALTH(Z));
          TERN_(Z2_HAS_STEALTHCHOP, if (index < 0 || index == 1) TMC_SET_STEALTH(Z2));
          TERN_(Z3_HAS_STEALTHCHOP, if (index < 0 || index == 2) TMC_SET_STEALTH(Z3));
          TERN_(Z4_HAS_STEALTHCHOP, if (index < 0 || index == 3) TMC_SET_STEALTH(Z4));
          break;
      #endif

      #if I_HAS_STEALTHCHOP
        case I_AXIS: TMC_SET_STEALTH(I); break;
      #endif
      #if J_HAS_STEALTHCHOP
        case J_AXIS: TMC_SET_STEALTH(J); break;
      #endif
      #if K_HAS_STEALTHCHOP
        case K_AXIS: TMC_SET_STEALTH(K); break;
      #endif
      #if U_HAS_STEALTHCHOP
        case U_AXIS: TMC_SET_STEALTH(U); break;
      #endif
      #if V_HAS_STEALTHCHOP
        case V_AXIS: TMC_SET_STEALTH(V); break;
      #endif
      #if W_HAS_STEALTHCHOP
        case W_AXIS: TMC_SET_STEALTH(W); break;
      #endif

      #if E_STEPPERS
        case E_AXIS: {
          TERN_(E0_HAS_STEALTHCHOP, if (eindex < 0 || eindex == 0) TMC_SET_STEALTH(E0));
          TERN_(E1_HAS_STEALTHCHOP, if (eindex < 0 || eindex == 1) TMC_SET_STEALTH(E1));
          TERN_(E2_HAS_STEALTHCHOP, if (eindex < 0 || eindex == 2) TMC_SET_STEALTH(E2));
          TERN_(E3_HAS_STEALTHCHOP, if (eindex < 0 || eindex == 3) TMC_SET_STEALTH(E3));
          TERN_(E4_HAS_STEALTHCHOP, if (eindex < 0 || eindex == 4) TMC_SET_STEALTH(E4));
          TERN_(E5_HAS_STEALTHCHOP, if (eindex < 0 || eindex == 5) TMC_SET_STEALTH(E5));
          TERN_(E6_HAS_STEALTHCHOP, if (eindex < 0 || eindex == 6) TMC_SET_STEALTH(E6));
          TERN_(E7_HAS_STEALTHCHOP, if (eindex < 0 || eindex == 7) TMC_SET_STEALTH(E7));
        } break;
      #endif
    }
  }
}

static void say_stealth_status() {
  #define TMC_SAY_STEALTH_STATUS(Q) tmc_say_stealth_status(stepper##Q)
  OPTCODE( X_HAS_STEALTHCHOP, TMC_SAY_STEALTH_STATUS(X))
  OPTCODE(X2_HAS_STEALTHCHOP, TMC_SAY_STEALTH_STATUS(X2))
  OPTCODE( Y_HAS_STEALTHCHOP, TMC_SAY_STEALTH_STATUS(Y))
  OPTCODE(Y2_HAS_STEALTHCHOP, TMC_SAY_STEALTH_STATUS(Y2))
  OPTCODE( Z_HAS_STEALTHCHOP, TMC_SAY_STEALTH_STATUS(Z))
  OPTCODE(Z2_HAS_STEALTHCHOP, TMC_SAY_STEALTH_STATUS(Z2))
  OPTCODE(Z3_HAS_STEALTHCHOP, TMC_SAY_STEALTH_STATUS(Z3))
  OPTCODE(Z4_HAS_STEALTHCHOP, TMC_SAY_STEALTH_STATUS(Z4))
  OPTCODE( I_HAS_STEALTHCHOP, TMC_SAY_STEALTH_STATUS(I))
  OPTCODE( J_HAS_STEALTHCHOP, TMC_SAY_STEALTH_STATUS(J))
  OPTCODE( K_HAS_STEALTHCHOP, TMC_SAY_STEALTH_STATUS(K))
  OPTCODE( U_HAS_STEALTHCHOP, TMC_SAY_STEALTH_STATUS(U))
  OPTCODE( V_HAS_STEALTHCHOP, TMC_SAY_STEALTH_STATUS(V))
  OPTCODE( W_HAS_STEALTHCHOP, TMC_SAY_STEALTH_STATUS(W))
  OPTCODE(E0_HAS_STEALTHCHOP, TMC_SAY_STEALTH_STATUS(E0))
  OPTCODE(E1_HAS_STEALTHCHOP, TMC_SAY_STEALTH_STATUS(E1))
  OPTCODE(E2_HAS_STEALTHCHOP, TMC_SAY_STEALTH_STATUS(E2))
  OPTCODE(E3_HAS_STEALTHCHOP, TMC_SAY_STEALTH_STATUS(E3))
  OPTCODE(E4_HAS_STEALTHCHOP, TMC_SAY_STEALTH_STATUS(E4))
  OPTCODE(E5_HAS_STEALTHCHOP, TMC_SAY_STEALTH_STATUS(E5))
  OPTCODE(E6_HAS_STEALTHCHOP, TMC_SAY_STEALTH_STATUS(E6))
  OPTCODE(E7_HAS_STEALTHCHOP, TMC_SAY_STEALTH_STATUS(E7))
}

/**
 * M569: Enable stealthChop on an axis
 *
 *   S[1|0] to enable or disable
 *   XYZE to target an axis
 *   No arguments reports the stealthChop status of all capable drivers.
 */
void GcodeSuite::M569() {
  if (parser.seen('S'))
    set_stealth_status(parser.value_bool(), get_target_e_stepper_from_command(-2));
  else
    say_stealth_status();
}

void GcodeSuite::M569_report(const bool forReplay/*=true*/) {
  report_heading(forReplay, F(STR_DRIVER_STEPPING_MODE));

  auto say_M569 = [](const bool forReplay, FSTR_P const etc=nullptr, const bool eol=false) {
    if (!forReplay) SERIAL_ECHO_START();
    SERIAL_ECHOPGM("  M569 S1");
    if (etc) {
      SERIAL_CHAR(' ');
      SERIAL_ECHOF(etc);
    }
    if (eol) SERIAL_EOL();
  };

  const bool chop_x = TERN0(X_HAS_STEALTHCHOP, stepperX.get_stored_stealthChop()),
             chop_y = TERN0(Y_HAS_STEALTHCHOP, stepperY.get_stored_stealthChop()),
             chop_z = TERN0(Z_HAS_STEALTHCHOP, stepperZ.get_stored_stealthChop()),
             chop_i = TERN0(I_HAS_STEALTHCHOP, stepperI.get_stored_stealthChop()),
             chop_j = TERN0(J_HAS_STEALTHCHOP, stepperJ.get_stored_stealthChop()),
             chop_k = TERN0(K_HAS_STEALTHCHOP, stepperK.get_stored_stealthChop()),
             chop_u = TERN0(U_HAS_STEALTHCHOP, stepperU.get_stored_stealthChop()),
             chop_v = TERN0(V_HAS_STEALTHCHOP, stepperV.get_stored_stealthChop()),
             chop_w = TERN0(W_HAS_STEALTHCHOP, stepperW.get_stored_stealthChop());

  if (chop_x || chop_y || chop_z || chop_i || chop_j || chop_k || chop_u || chop_v || chop_w) {
    say_M569(forReplay);
    NUM_AXIS_CODE(
      if (chop_x) SERIAL_ECHOPGM_P(SP_X_STR),
      if (chop_y) SERIAL_ECHOPGM_P(SP_Y_STR),
      if (chop_z) SERIAL_ECHOPGM_P(SP_Z_STR),
      if (chop_i) SERIAL_ECHOPGM_P(SP_I_STR),
      if (chop_j) SERIAL_ECHOPGM_P(SP_J_STR),
      if (chop_k) SERIAL_ECHOPGM_P(SP_K_STR),
      if (chop_u) SERIAL_ECHOPGM_P(SP_U_STR),
      if (chop_v) SERIAL_ECHOPGM_P(SP_V_STR),
      if (chop_w) SERIAL_ECHOPGM_P(SP_W_STR)
    );
    SERIAL_EOL();
  }

  const bool chop_x2 = TERN0(X2_HAS_STEALTHCHOP, stepperX2.get_stored_stealthChop()),
             chop_y2 = TERN0(Y2_HAS_STEALTHCHOP, stepperY2.get_stored_stealthChop()),
             chop_z2 = TERN0(Z2_HAS_STEALTHCHOP, stepperZ2.get_stored_stealthChop());

  if (chop_x2 || chop_y2 || chop_z2) {
    say_M569(forReplay, F("I1"));
    if (chop_x2) SERIAL_ECHOPGM_P(SP_X_STR);
    if (chop_y2) SERIAL_ECHOPGM_P(SP_Y_STR);
    if (chop_z2) SERIAL_ECHOPGM_P(SP_Z_STR);
    SERIAL_EOL();
  }

  if (TERN0(Z3_HAS_STEALTHCHOP, stepperZ3.get_stored_stealthChop())) { say_M569(forReplay, F("I2 Z"), true); }
  if (TERN0(Z4_HAS_STEALTHCHOP, stepperZ4.get_stored_stealthChop())) { say_M569(forReplay, F("I3 Z"), true); }

  if (TERN0( I_HAS_STEALTHCHOP, stepperI.get_stored_stealthChop()))  { say_M569(forReplay, FPSTR(SP_I_STR), true); }
  if (TERN0( J_HAS_STEALTHCHOP, stepperJ.get_stored_stealthChop()))  { say_M569(forReplay, FPSTR(SP_J_STR), true); }
  if (TERN0( K_HAS_STEALTHCHOP, stepperK.get_stored_stealthChop()))  { say_M569(forReplay, FPSTR(SP_K_STR), true); }
  if (TERN0( U_HAS_STEALTHCHOP, stepperU.get_stored_stealthChop()))  { say_M569(forReplay, FPSTR(SP_U_STR), true); }
  if (TERN0( V_HAS_STEALTHCHOP, stepperV.get_stored_stealthChop()))  { say_M569(forReplay, FPSTR(SP_V_STR), true); }
  if (TERN0( W_HAS_STEALTHCHOP, stepperW.get_stored_stealthChop()))  { say_M569(forReplay, FPSTR(SP_W_STR), true); }

  if (TERN0(E0_HAS_STEALTHCHOP, stepperE0.get_stored_stealthChop())) { say_M569(forReplay, F("T0 E"), true); }
  if (TERN0(E1_HAS_STEALTHCHOP, stepperE1.get_stored_stealthChop())) { say_M569(forReplay, F("T1 E"), true); }
  if (TERN0(E2_HAS_STEALTHCHOP, stepperE2.get_stored_stealthChop())) { say_M569(forReplay, F("T2 E"), true); }
  if (TERN0(E3_HAS_STEALTHCHOP, stepperE3.get_stored_stealthChop())) { say_M569(forReplay, F("T3 E"), true); }
  if (TERN0(E4_HAS_STEALTHCHOP, stepperE4.get_stored_stealthChop())) { say_M569(forReplay, F("T4 E"), true); }
  if (TERN0(E5_HAS_STEALTHCHOP, stepperE5.get_stored_stealthChop())) { say_M569(forReplay, F("T5 E"), true); }
  if (TERN0(E6_HAS_STEALTHCHOP, stepperE6.get_stored_stealthChop())) { say_M569(forReplay, F("T6 E"), true); }
  if (TERN0(E7_HAS_STEALTHCHOP, stepperE7.get_stored_stealthChop())) { say_M569(forReplay, F("T7 E"), true); }
}

#endif // HAS_STEALTHCHOP
