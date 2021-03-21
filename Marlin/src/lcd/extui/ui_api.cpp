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

/**************
 * ui_api.cpp *
 **************/

/****************************************************************************
 *   Written By Marcio Teixeira 2018 - Aleph Objects, Inc.                  *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   To view a copy of the GNU General Public License, go to the following  *
 *   location: <https://www.gnu.org/licenses/>.                             *
 ****************************************************************************/

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(EXTENSIBLE_UI)

#include "../marlinui.h"
#include "../../gcode/queue.h"
#include "../../module/motion.h"
#include "../../module/planner.h"
#include "../../module/probe.h"
#include "../../module/temperature.h"
#include "../../module/printcounter.h"
#include "../../libs/duration_t.h"
#include "../../HAL/shared/Delay.h"
#include "../../sd/cardreader.h"

#if ENABLED(PRINTCOUNTER)
  #include "../../core/utility.h"
  #include "../../libs/numtostr.h"
#endif

#if HAS_MULTI_EXTRUDER
  #include "../../module/tool_change.h"
#endif

#if ENABLED(EMERGENCY_PARSER)
  #include "../../feature/e_parser.h"
#endif

#if HAS_TRINAMIC_CONFIG
  #include "../../feature/tmc_util.h"
  #include "../../module/stepper/indirection.h"
#endif

#include "ui_api.h"

#if ENABLED(BACKLASH_GCODE)
  #include "../../feature/backlash.h"
#endif

#if HAS_LEVELING
  #include "../../feature/bedlevel/bedlevel.h"
#endif

#if HAS_FILAMENT_SENSOR
  #include "../../feature/runout.h"
#endif

#if ENABLED(CASE_LIGHT_ENABLE)
  #include "../../feature/caselight.h"
#endif

#if ENABLED(BABYSTEPPING)
  #include "../../feature/babystep.h"
#endif

#if ENABLED(HOST_PROMPT_SUPPORT)
  #include "../../feature/host_actions.h"
#endif

namespace ExtUI {
  static struct {
    uint8_t printer_killed : 1;
    TERN_(JOYSTICK, uint8_t jogging : 1);
    TERN_(SDSUPPORT, uint8_t was_sd_printing : 1);
  } flags;

  #ifdef __SAM3X8E__
    /**
     * Implement a special millis() to allow time measurement
     * within an ISR (such as when the printer is killed).
     *
     * To keep proper time, must be called at least every 1s.
     */
    uint32_t safe_millis() {
      // Not killed? Just call millis()
      if (!flags.printer_killed) return millis();

      static uint32_t currTimeHI = 0; /* Current time */

      // Machine was killed, reinit SysTick so we are able to compute time without ISRs
      if (currTimeHI == 0) {
        // Get the last time the Arduino time computed (from CMSIS) and convert it to SysTick
        currTimeHI = uint32_t((GetTickCount() * uint64_t(F_CPU / 8000)) >> 24);

        // Reinit the SysTick timer to maximize its period
        SysTick->LOAD  = SysTick_LOAD_RELOAD_Msk;                    // get the full range for the systick timer
        SysTick->VAL   = 0;                                          // Load the SysTick Counter Value
        SysTick->CTRL  = // MCLK/8 as source
                         // No interrupts
                         SysTick_CTRL_ENABLE_Msk;                    // Enable SysTick Timer
     }

      // Check if there was a timer overflow from the last read
      if (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) {
        // There was. This means (SysTick_LOAD_RELOAD_Msk * 1000 * 8)/F_CPU ms has elapsed
        currTimeHI++;
      }

      // Calculate current time in milliseconds
      uint32_t currTimeLO = SysTick_LOAD_RELOAD_Msk - SysTick->VAL; // (in MCLK/8)
      uint64_t currTime = ((uint64_t)currTimeLO) | (((uint64_t)currTimeHI) << 24);

      // The ms count is
      return (uint32_t)(currTime / (F_CPU / 8000));
    }
  #endif // __SAM3X8E__

  void delay_us(uint32_t us) { DELAY_US(us); }

  void delay_ms(uint32_t ms) {
    if (flags.printer_killed)
      DELAY_US(ms * 1000);
    else
      safe_delay(ms);
  }

  void yield() {
    if (!flags.printer_killed) thermalManager.manage_heater();
  }

  void enableHeater(const extruder_t extruder) {
    #if HAS_HOTEND && HEATER_IDLE_HANDLER
      thermalManager.reset_hotend_idle_timer(extruder - E0);
    #else
      UNUSED(extruder);
    #endif
  }

  void enableHeater(const heater_t heater) {
    #if HEATER_IDLE_HANDLER
      switch (heater) {
        #if HAS_HEATED_BED
          case BED: thermalManager.reset_bed_idle_timer(); return;
        #endif
        TERN_(HAS_HEATED_CHAMBER, case CHAMBER: return); // Chamber has no idle timer
        TERN_(HAS_COOLER, case COOLER: return); // Cooler has no idle timer
        default:
          TERN_(HAS_HOTEND, thermalManager.reset_hotend_idle_timer(heater - H0));
          break;
      }
    #else
      UNUSED(heater);
    #endif
  }

  #if ENABLED(JOYSTICK)
    /**
     * Jogs in the direction given by the vector (dx, dy, dz).
     * The values range from -1 to 1 mapping to the maximum
     * feedrate for an axis.
     *
     * The axis will continue to jog until this function is
     * called with all zeros.
     */
    void jog(const xyz_float_t &dir) {
      // The "destination" variable is used as a scratchpad in
      // Marlin by GCODE routines, but should remain untouched
      // during manual jogging, allowing us to reuse the space
      // for our direction vector.
      destination = dir;
      flags.jogging = !NEAR_ZERO(dir.x) || !NEAR_ZERO(dir.y) || !NEAR_ZERO(dir.z);
    }

    // Called by the polling routine in "joystick.cpp"
    void _joystick_update(xyz_float_t &norm_jog) {
      if (flags.jogging) {
        #define OUT_OF_RANGE(VALUE) (VALUE < -1.0f || VALUE > 1.0f)

        if (OUT_OF_RANGE(destination.x) || OUT_OF_RANGE(destination.y) || OUT_OF_RANGE(destination.z)) {
          // If destination on any axis is out of range, it
          // probably means the UI forgot to stop jogging and
          // ran GCODE that wrote a position to destination.
          // To prevent a disaster, stop jogging.
          flags.jogging = false;
          return;
        }
        norm_jog = destination;
      }
    }
  #endif

  bool isHeaterIdle(const extruder_t extruder) {
    #if HAS_HOTEND && HEATER_IDLE_HANDLER
      return thermalManager.heater_idle[extruder - E0].timed_out;
    #else
      UNUSED(extruder);
      return false;
    #endif
  }

  bool isHeaterIdle(const heater_t heater) {
    #if HEATER_IDLE_HANDLER
      switch (heater) {
        TERN_(HAS_HEATED_BED, case BED: return thermalManager.heater_idle[thermalManager.IDLE_INDEX_BED].timed_out);
        TERN_(HAS_HEATED_CHAMBER, case CHAMBER: return false); // Chamber has no idle timer
        default:
          return TERN0(HAS_HOTEND, thermalManager.heater_idle[heater - H0].timed_out);
      }
    #else
      UNUSED(heater);
      return false;
    #endif
  }

  #ifdef TOUCH_UI_LCD_TEMP_SCALING
    #define GET_TEMP_ADJUSTMENT(A) (float(A) / (TOUCH_UI_LCD_TEMP_SCALING))
  #else
    #define GET_TEMP_ADJUSTMENT(A) A
  #endif

  float getActualTemp_celsius(const heater_t heater) {
    switch (heater) {
      TERN_(HAS_HEATED_BED, case BED: return GET_TEMP_ADJUSTMENT(thermalManager.degBed()));
      TERN_(HAS_HEATED_CHAMBER, case CHAMBER: return GET_TEMP_ADJUSTMENT(thermalManager.degChamber()));
      default: return GET_TEMP_ADJUSTMENT(thermalManager.degHotend(heater - H0));
    }
  }

  float getActualTemp_celsius(const extruder_t extruder) {
    return GET_TEMP_ADJUSTMENT(thermalManager.degHotend(extruder - E0));
  }

  float getTargetTemp_celsius(const heater_t heater) {
    switch (heater) {
      TERN_(HAS_HEATED_BED, case BED: return GET_TEMP_ADJUSTMENT(thermalManager.degTargetBed()));
      TERN_(HAS_HEATED_CHAMBER, case CHAMBER: return GET_TEMP_ADJUSTMENT(thermalManager.degTargetChamber()));
      default: return GET_TEMP_ADJUSTMENT(thermalManager.degTargetHotend(heater - H0));
    }
  }

  float getTargetTemp_celsius(const extruder_t extruder) {
    return GET_TEMP_ADJUSTMENT(thermalManager.degTargetHotend(extruder - E0));
  }

  float getTargetFan_percent(const fan_t fan) {
    #if HAS_FAN
      return thermalManager.fanPercent(thermalManager.fan_speed[fan - FAN0]);
    #else
      UNUSED(fan);
      return 0;
    #endif
  }

  float getActualFan_percent(const fan_t fan) {
    #if HAS_FAN
      return thermalManager.fanPercent(thermalManager.scaledFanSpeed(fan - FAN0));
    #else
      UNUSED(fan);
      return 0;
    #endif
  }

  float getAxisPosition_mm(const axis_t axis) {
    return TERN_(JOYSTICK, flags.jogging ? destination[axis] :) current_position[axis];
  }

  float getAxisPosition_mm(const extruder_t extruder) {
    const extruder_t old_tool = getActiveTool();
    setActiveTool(extruder, true);
    const float epos = TERN_(JOYSTICK, flags.jogging ? destination.e :) current_position.e;
    setActiveTool(old_tool, true);
    return epos;
  }

  void setAxisPosition_mm(const float &position, const axis_t axis, const feedRate_t feedrate/*=0*/) {
    // Get motion limit from software endstops, if any
    float min, max;
    soft_endstop.get_manual_axis_limits((AxisEnum)axis, min, max);

    // Delta limits XY based on the current offset from center
    // This assumes the center is 0,0
    #if ENABLED(DELTA)
      if (axis != Z) {
        max = SQRT(sq(float(DELTA_PRINTABLE_RADIUS)) - sq(current_position[Y - axis])); // (Y - axis) == the other axis
        min = -max;
      }
    #endif

    current_position[axis] = constrain(position, min, max);
    line_to_current_position(feedrate ?: manual_feedrate_mm_s[axis]);
  }

  void setAxisPosition_mm(const float &position, const extruder_t extruder, const feedRate_t feedrate/*=0*/) {
    setActiveTool(extruder, true);

    current_position.e = position;
    line_to_current_position(feedrate ?: manual_feedrate_mm_s.e);
  }

  void setActiveTool(const extruder_t extruder, bool no_move) {
    #if HAS_MULTI_EXTRUDER
      const uint8_t e = extruder - E0;
      if (e != active_extruder) tool_change(e, no_move);
      active_extruder = e;
    #else
      UNUSED(extruder);
      UNUSED(no_move);
    #endif
  }

  extruder_t getTool(const uint8_t extruder) {
    switch (extruder) {
      case 7:  return E7;
      case 6:  return E6;
      case 5:  return E5;
      case 4:  return E4;
      case 3:  return E3;
      case 2:  return E2;
      case 1:  return E1;
      default: return E0;
    }
  }

  extruder_t getActiveTool() { return getTool(active_extruder); }

  bool isMoving() { return planner.has_blocks_queued(); }

  bool canMove(const axis_t axis) {
    switch (axis) {
      #if IS_KINEMATIC || ENABLED(NO_MOTION_BEFORE_HOMING)
        case X: return axis_should_home(X_AXIS);
        case Y: return axis_should_home(Y_AXIS);
        case Z: return axis_should_home(Z_AXIS);
      #else
        case X: case Y: case Z: return true;
      #endif
      default: return false;
    }
  }

  bool canMove(const extruder_t extruder) {
    return !thermalManager.tooColdToExtrude(extruder - E0);
  }

  #if HAS_SOFTWARE_ENDSTOPS
    bool getSoftEndstopState() { return soft_endstop._enabled; }
    void setSoftEndstopState(const bool value) { soft_endstop._enabled = value; }
  #endif

  #if HAS_TRINAMIC_CONFIG
    float getAxisCurrent_mA(const axis_t axis) {
      switch (axis) {
        #if AXIS_IS_TMC(X)
          case X: return stepperX.getMilliamps();
        #endif
        #if AXIS_IS_TMC(X2)
          case X2: return stepperX2.getMilliamps();
        #endif
        #if AXIS_IS_TMC(Y)
          case Y: return stepperY.getMilliamps();
        #endif
        #if AXIS_IS_TMC(Y2)
          case Y2: return stepperY2.getMilliamps();
        #endif
        #if AXIS_IS_TMC(Z)
          case Z: return stepperZ.getMilliamps();
        #endif
        #if AXIS_IS_TMC(Z2)
          case Z2: return stepperZ2.getMilliamps();
        #endif
        default: return NAN;
      };
    }

    float getAxisCurrent_mA(const extruder_t extruder) {
      switch (extruder) {
        #if AXIS_IS_TMC(E0)
          case E0: return stepperE0.getMilliamps();
        #endif
        #if AXIS_IS_TMC(E1)
          case E1: return stepperE1.getMilliamps();
        #endif
        #if AXIS_IS_TMC(E2)
          case E2: return stepperE2.getMilliamps();
        #endif
        #if AXIS_IS_TMC(E3)
          case E3: return stepperE3.getMilliamps();
        #endif
        #if AXIS_IS_TMC(E4)
          case E4: return stepperE4.getMilliamps();
        #endif
        #if AXIS_IS_TMC(E5)
          case E5: return stepperE5.getMilliamps();
        #endif
        #if AXIS_IS_TMC(E6)
          case E6: return stepperE6.getMilliamps();
        #endif
        #if AXIS_IS_TMC(E7)
          case E7: return stepperE7.getMilliamps();
        #endif
        default: return NAN;
      };
    }

    void setAxisCurrent_mA(const float &mA, const axis_t axis) {
      switch (axis) {
        #if AXIS_IS_TMC(X)
          case X: stepperX.rms_current(constrain(mA, 400, 1500)); break;
        #endif
        #if AXIS_IS_TMC(X2)
          case X2: stepperX2.rms_current(constrain(mA, 400, 1500)); break;
        #endif
        #if AXIS_IS_TMC(Y)
          case Y: stepperY.rms_current(constrain(mA, 400, 1500)); break;
        #endif
        #if AXIS_IS_TMC(Y2)
          case Y2: stepperY2.rms_current(constrain(mA, 400, 1500)); break;
        #endif
        #if AXIS_IS_TMC(Z)
          case Z: stepperZ.rms_current(constrain(mA, 400, 1500)); break;
        #endif
        #if AXIS_IS_TMC(Z2)
          case Z2: stepperZ2.rms_current(constrain(mA, 400, 1500)); break;
        #endif
        default: break;
      };
    }

    void setAxisCurrent_mA(const float &mA, const extruder_t extruder) {
      switch (extruder) {
        #if AXIS_IS_TMC(E0)
          case E0: stepperE0.rms_current(constrain(mA, 400, 1500)); break;
        #endif
        #if AXIS_IS_TMC(E1)
          case E1: stepperE1.rms_current(constrain(mA, 400, 1500)); break;
        #endif
        #if AXIS_IS_TMC(E2)
          case E2: stepperE2.rms_current(constrain(mA, 400, 1500)); break;
        #endif
        #if AXIS_IS_TMC(E3)
          case E3: stepperE3.rms_current(constrain(mA, 400, 1500)); break;
        #endif
        #if AXIS_IS_TMC(E4)
          case E4: stepperE4.rms_current(constrain(mA, 400, 1500)); break;
        #endif
        #if AXIS_IS_TMC(E5)
          case E5: stepperE5.rms_current(constrain(mA, 400, 1500)); break;
        #endif
        #if AXIS_IS_TMC(E6)
          case E6: stepperE6.rms_current(constrain(mA, 400, 1500)); break;
        #endif
        #if AXIS_IS_TMC(E7)
          case E7: stepperE7.rms_current(constrain(mA, 400, 1500)); break;
        #endif
        default: break;
      };
    }

    int getTMCBumpSensitivity(const axis_t axis) {
      switch (axis) {
        TERN_(X_SENSORLESS,  case X:  return stepperX.homing_threshold());
        TERN_(X2_SENSORLESS, case X2: return stepperX2.homing_threshold());
        TERN_(Y_SENSORLESS,  case Y:  return stepperY.homing_threshold());
        TERN_(Y2_SENSORLESS, case Y2: return stepperY2.homing_threshold());
        TERN_(Z_SENSORLESS,  case Z:  return stepperZ.homing_threshold());
        TERN_(Z2_SENSORLESS, case Z2: return stepperZ2.homing_threshold());
        TERN_(Z3_SENSORLESS, case Z3: return stepperZ3.homing_threshold());
        TERN_(Z4_SENSORLESS, case Z4: return stepperZ4.homing_threshold());
        default: return 0;
      }
    }

    void setTMCBumpSensitivity(const float &value, const axis_t axis) {
      switch (axis) {
        #if X_SENSORLESS || Y_SENSORLESS || Z_SENSORLESS
          #if X_SENSORLESS
            case X:  stepperX.homing_threshold(value);  break;
          #endif
          #if X2_SENSORLESS
            case X2: stepperX2.homing_threshold(value); break;
          #endif
          #if Y_SENSORLESS
            case Y: stepperY.homing_threshold(value); break;
          #endif
          #if Y2_SENSORLESS
            case Y2: stepperY2.homing_threshold(value); break;
          #endif
          #if Z_SENSORLESS
            case Z: stepperZ.homing_threshold(value); break;
          #endif
          #if Z2_SENSORLESS
            case Z2: stepperZ2.homing_threshold(value); break;
          #endif
          #if Z3_SENSORLESS
            case Z3: stepperZ3.homing_threshold(value); break;
          #endif
          #if Z4_SENSORLESS
            case Z4: stepperZ4.homing_threshold(value); break;
          #endif
        #else
          UNUSED(value);
        #endif
        default: break;
      }
    }
  #endif

  float getAxisSteps_per_mm(const axis_t axis) {
    return planner.settings.axis_steps_per_mm[axis];
  }

  float getAxisSteps_per_mm(const extruder_t extruder) {
    UNUSED_E(extruder);
    return planner.settings.axis_steps_per_mm[E_AXIS_N(extruder - E0)];
  }

  void setAxisSteps_per_mm(const float &value, const axis_t axis) {
    planner.settings.axis_steps_per_mm[axis] = value;
    planner.refresh_positioning();
  }

  void setAxisSteps_per_mm(const float &value, const extruder_t extruder) {
    UNUSED_E(extruder);
    planner.settings.axis_steps_per_mm[E_AXIS_N(extruder - E0)] = value;
    planner.refresh_positioning();
  }

  feedRate_t getAxisMaxFeedrate_mm_s(const axis_t axis) {
    return planner.settings.max_feedrate_mm_s[axis];
  }

  feedRate_t getAxisMaxFeedrate_mm_s(const extruder_t extruder) {
    UNUSED_E(extruder);
    return planner.settings.max_feedrate_mm_s[E_AXIS_N(extruder - E0)];
  }

  void setAxisMaxFeedrate_mm_s(const feedRate_t value, const axis_t axis) {
    planner.set_max_feedrate(axis, value);
  }

  void setAxisMaxFeedrate_mm_s(const feedRate_t value, const extruder_t extruder) {
    UNUSED_E(extruder);
    planner.set_max_feedrate(E_AXIS_N(extruder - E0), value);
  }

  float getAxisMaxAcceleration_mm_s2(const axis_t axis) {
    return planner.settings.max_acceleration_mm_per_s2[axis];
  }

  float getAxisMaxAcceleration_mm_s2(const extruder_t extruder) {
    UNUSED_E(extruder);
    return planner.settings.max_acceleration_mm_per_s2[E_AXIS_N(extruder - E0)];
  }

  void setAxisMaxAcceleration_mm_s2(const float &value, const axis_t axis) {
    planner.set_max_acceleration(axis, value);
  }

  void setAxisMaxAcceleration_mm_s2(const float &value, const extruder_t extruder) {
    UNUSED_E(extruder);
    planner.set_max_acceleration(E_AXIS_N(extruder - E0), value);
  }

  #if HAS_FILAMENT_SENSOR
    bool getFilamentRunoutEnabled()                 { return runout.enabled; }
    void setFilamentRunoutEnabled(const bool value) { runout.enabled = value; }
    bool getFilamentRunoutState()                   { return runout.filament_ran_out; }
    void setFilamentRunoutState(const bool value)   { runout.filament_ran_out = value; }

    #if HAS_FILAMENT_RUNOUT_DISTANCE
      float getFilamentRunoutDistance_mm()                 { return runout.runout_distance(); }
      void setFilamentRunoutDistance_mm(const float &value) { runout.set_runout_distance(constrain(value, 0, 999)); }
    #endif
  #endif

  #if ENABLED(CASE_LIGHT_ENABLE)
    bool getCaseLightState()                 { return caselight.on; }
    void setCaseLightState(const bool value) {
      caselight.on = value;
      caselight.update_enabled();
    }

    #if CASELIGHT_USES_BRIGHTNESS
      float getCaseLightBrightness_percent()                 { return ui8_to_percent(caselight.brightness); }
      void setCaseLightBrightness_percent(const float &value) {
         caselight.brightness = map(constrain(value, 0, 100), 0, 100, 0, 255);
         caselight.update_brightness();
      }
    #endif
  #endif

  #if ENABLED(LIN_ADVANCE)
    float getLinearAdvance_mm_mm_s(const extruder_t extruder) {
      return (extruder < EXTRUDERS) ? planner.extruder_advance_K[extruder - E0] : 0;
    }

    void setLinearAdvance_mm_mm_s(const float &value, const extruder_t extruder) {
      if (extruder < EXTRUDERS)
        planner.extruder_advance_K[extruder - E0] = constrain(value, 0, 10);
    }
  #endif

  #if HAS_JUNCTION_DEVIATION

    float getJunctionDeviation_mm() {
      return planner.junction_deviation_mm;
    }

    void setJunctionDeviation_mm(const float &value) {
      planner.junction_deviation_mm = constrain(value, 0.001, 0.3);
      TERN_(LIN_ADVANCE, planner.recalculate_max_e_jerk());
    }

  #else
    float getAxisMaxJerk_mm_s(const axis_t axis) { return planner.max_jerk[axis]; }
    float getAxisMaxJerk_mm_s(const extruder_t) { return planner.max_jerk.e; }
    void setAxisMaxJerk_mm_s(const float &value, const axis_t axis) { planner.set_max_jerk((AxisEnum)axis, value); }
    void setAxisMaxJerk_mm_s(const float &value, const extruder_t) { planner.set_max_jerk(E_AXIS, value); }
  #endif

  #if ENABLED(DUAL_X_CARRIAGE)
    uint8_t getIDEX_Mode() { return dual_x_carriage_mode; }
  #endif

  #if PREHEAT_COUNT
      uint16_t getMaterial_preset_E(const uint16_t index) { return ui.material_preset[index].hotend_temp; }
    #if HAS_HEATED_BED
      uint16_t getMaterial_preset_B(const uint16_t index) { return ui.material_preset[index].bed_temp; }
    #endif
  #endif

  feedRate_t getFeedrate_mm_s()                       { return feedrate_mm_s; }
  int16_t getFlowPercentage(const extruder_t extr)    { return planner.flow_percentage[extr]; }
  feedRate_t getMinFeedrate_mm_s()                    { return planner.settings.min_feedrate_mm_s; }
  feedRate_t getMinTravelFeedrate_mm_s()              { return planner.settings.min_travel_feedrate_mm_s; }
  float getPrintingAcceleration_mm_s2()               { return planner.settings.acceleration; }
  float getRetractAcceleration_mm_s2()                { return planner.settings.retract_acceleration; }
  float getTravelAcceleration_mm_s2()                 { return planner.settings.travel_acceleration; }
  void setFeedrate_mm_s(const feedRate_t fr)          { feedrate_mm_s = fr; }
  void setFlow_percent(const int16_t flow, const extruder_t extr) { planner.set_flow(extr, flow); }
  void setMinFeedrate_mm_s(const feedRate_t fr)       { planner.settings.min_feedrate_mm_s = fr; }
  void setMinTravelFeedrate_mm_s(const feedRate_t fr) { planner.settings.min_travel_feedrate_mm_s = fr; }
  void setPrintingAcceleration_mm_s2(const float &acc) { planner.settings.acceleration = acc; }
  void setRetractAcceleration_mm_s2(const float &acc) { planner.settings.retract_acceleration = acc; }
  void setTravelAcceleration_mm_s2(const float &acc)  { planner.settings.travel_acceleration = acc; }

  #if ENABLED(BABYSTEPPING)

    bool babystepAxis_steps(const int16_t steps, const axis_t axis) {
      switch (axis) {
        #if ENABLED(BABYSTEP_XY)
          case X: babystep.add_steps(X_AXIS, steps); break;
          case Y: babystep.add_steps(Y_AXIS, steps); break;
        #endif
        case Z: babystep.add_steps(Z_AXIS, steps); break;
        default: return false;
      };
      return true;
    }

    /**
     * This function adjusts an axis during a print.
     *
     * When linked_nozzles is false, each nozzle in a multi-nozzle
     * printer can be babystepped independently of the others. This
     * lets the user to fine tune the Z-offset and Nozzle Offsets
     * while observing the first layer of a print, regardless of
     * what nozzle is printing.
     */
    void smartAdjustAxis_steps(const int16_t steps, const axis_t axis, bool linked_nozzles) {
      const float mm = steps * planner.steps_to_mm[axis];
      UNUSED(mm);

      if (!babystepAxis_steps(steps, axis)) return;

      #if ENABLED(BABYSTEP_ZPROBE_OFFSET)
        // Make it so babystepping in Z adjusts the Z probe offset.
        if (axis == Z && TERN1(HAS_MULTI_EXTRUDER, (linked_nozzles || active_extruder == 0)))
          probe.offset.z += mm;
      #endif

      #if HAS_MULTI_EXTRUDER && HAS_HOTEND_OFFSET
        /**
         * When linked_nozzles is false, as an axis is babystepped
         * adjust the hotend offsets so that the other nozzles are
         * unaffected by the babystepping of the active nozzle.
         */
        if (!linked_nozzles) {
          HOTEND_LOOP()
            if (e != active_extruder)
              hotend_offset[e][axis] += mm;

          normalizeNozzleOffset(X);
          normalizeNozzleOffset(Y);
          normalizeNozzleOffset(Z);
        }
      #else
        UNUSED(linked_nozzles);
      #endif
    }

    /**
     * Converts a mm displacement to a number of whole number of
     * steps that is at least mm long.
     */
    int16_t mmToWholeSteps(const float &mm, const axis_t axis) {
      const float steps = mm / planner.steps_to_mm[axis];
      return steps > 0 ? CEIL(steps) : FLOOR(steps);
    }

  #endif // BABYSTEPPING

  float getZOffset_mm() {
    return (0.0f
      #if HAS_BED_PROBE
        + probe.offset.z
      #elif ENABLED(BABYSTEP_DISPLAY_TOTAL)
        + planner.steps_to_mm[Z_AXIS] * babystep.axis_total[BS_AXIS_IND(Z_AXIS)]
      #endif
    );
  }

  void setZOffset_mm(const float &value) {
    #if HAS_BED_PROBE
      if (WITHIN(value, Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX))
        probe.offset.z = value;
    #elif ENABLED(BABYSTEP_DISPLAY_TOTAL)
      babystep.add_mm(Z_AXIS, value - getZOffset_mm());
    #else
      UNUSED(value);
    #endif
  }

  #if HAS_HOTEND_OFFSET

    float getNozzleOffset_mm(const axis_t axis, const extruder_t extruder) {
      if (extruder - E0 >= HOTENDS) return 0;
      return hotend_offset[extruder - E0][axis];
    }

    void setNozzleOffset_mm(const float &value, const axis_t axis, const extruder_t extruder) {
      if (extruder - E0 >= HOTENDS) return;
      hotend_offset[extruder - E0][axis] = value;
    }

    /**
     * The UI should call this if needs to guarantee the first
     * nozzle offset is zero (such as when it doesn't allow the
     * user to edit the offset the first nozzle).
     */
    void normalizeNozzleOffset(const axis_t axis) {
      const float offs = hotend_offset[0][axis];
      HOTEND_LOOP() hotend_offset[e][axis] -= offs;
    }

  #endif // HAS_HOTEND_OFFSET

  #if HAS_BED_PROBE
    float getProbeOffset_mm(const axis_t axis) { return probe.offset.pos[axis]; }
    void setProbeOffset_mm(const float &val, const axis_t axis) { probe.offset.pos[axis] = val; }
  #endif

  #if ENABLED(BACKLASH_GCODE)
    float getAxisBacklash_mm(const axis_t axis)       { return backlash.distance_mm[axis]; }
    void setAxisBacklash_mm(const float &value, const axis_t axis)
                                                      { backlash.distance_mm[axis] = constrain(value,0,5); }

    float getBacklashCorrection_percent()             { return ui8_to_percent(backlash.correction); }
    void setBacklashCorrection_percent(const float &value) { backlash.correction = map(constrain(value, 0, 100), 0, 100, 0, 255); }

    #ifdef BACKLASH_SMOOTHING_MM
      float getBacklashSmoothing_mm()                 { return backlash.smoothing_mm; }
      void setBacklashSmoothing_mm(const float &value) { backlash.smoothing_mm = constrain(value, 0, 999); }
    #endif
  #endif

  uint32_t getProgress_seconds_elapsed() {
    const duration_t elapsed = print_job_timer.duration();
    return elapsed.value;
  }

  #if HAS_LEVELING
    bool getLevelingActive() { return planner.leveling_active; }
    void setLevelingActive(const bool state) { set_bed_leveling_enabled(state); }
    bool getMeshValid() { return leveling_is_valid(); }
    #if HAS_MESH
      bed_mesh_t& getMeshArray() { return Z_VALUES_ARR; }
      float getMeshPoint(const xy_uint8_t &pos) { return Z_VALUES(pos.x, pos.y); }
      void setMeshPoint(const xy_uint8_t &pos, const float &zoff) {
        if (WITHIN(pos.x, 0, GRID_MAX_POINTS_X) && WITHIN(pos.y, 0, GRID_MAX_POINTS_Y)) {
          Z_VALUES(pos.x, pos.y) = zoff;
          TERN_(ABL_BILINEAR_SUBDIVISION, bed_level_virt_interpolate());
        }
      }
    #endif
  #endif

  #if ENABLED(HOST_PROMPT_SUPPORT)
    void setHostResponse(const uint8_t response) { host_response_handler(response); }
  #endif

  #if ENABLED(PRINTCOUNTER)
    char* getFailedPrints_str(char buffer[21])   { strcpy(buffer,i16tostr3left(print_job_timer.getStats().totalPrints - print_job_timer.getStats().finishedPrints)); return buffer; }
    char* getTotalPrints_str(char buffer[21])    { strcpy(buffer,i16tostr3left(print_job_timer.getStats().totalPrints));    return buffer; }
    char* getFinishedPrints_str(char buffer[21]) { strcpy(buffer,i16tostr3left(print_job_timer.getStats().finishedPrints)); return buffer; }
    char* getTotalPrintTime_str(char buffer[21]) { return duration_t(print_job_timer.getStats().printTime).toString(buffer); }
    char* getLongestPrint_str(char buffer[21])   { return duration_t(print_job_timer.getStats().longestPrint).toString(buffer); }
    char* getFilamentUsed_str(char buffer[21])   {
      printStatistics stats = print_job_timer.getStats();
      sprintf_P(buffer, PSTR("%ld.%im"), long(stats.filamentUsed / 1000), int16_t(stats.filamentUsed / 100) % 10);
      return buffer;
    }
  #endif

  float getFeedrate_percent() { return feedrate_percentage; }

  #if ENABLED(PIDTEMP)
    float getPIDValues_Kp(const extruder_t tool) { return PID_PARAM(Kp, tool); }
    float getPIDValues_Ki(const extruder_t tool) { return unscalePID_i(PID_PARAM(Ki, tool)); }
    float getPIDValues_Kd(const extruder_t tool) { return unscalePID_d(PID_PARAM(Kd, tool)); }

    void setPIDValues(const float &p, const float &i, const float &d, extruder_t tool) {
      thermalManager.temp_hotend[tool].pid.Kp = p;
      thermalManager.temp_hotend[tool].pid.Ki = scalePID_i(i);
      thermalManager.temp_hotend[tool].pid.Kd = scalePID_d(d);
      thermalManager.updatePID();
    }

    void startPIDTune(const float &temp, extruder_t tool) {
      thermalManager.PID_autotune(temp, (heater_id_t)tool, 8, true);
    }
  #endif

  #if ENABLED(PIDTEMPBED)
    float getBedPIDValues_Kp() { return thermalManager.temp_bed.pid.Kp; }
    float getBedPIDValues_Ki() { return unscalePID_i(thermalManager.temp_bed.pid.Ki); }
    float getBedPIDValues_Kd() { return unscalePID_d(thermalManager.temp_bed.pid.Kd); }

    void setBedPIDValues(const float &p, const float &i, const float &d) {
      thermalManager.temp_bed.pid.Kp = p;
      thermalManager.temp_bed.pid.Ki = scalePID_i(i);
      thermalManager.temp_bed.pid.Kd = scalePID_d(d);
      thermalManager.updatePID();
    }

    void startBedPIDTune(const float &temp) {
      thermalManager.PID_autotune(temp, H_BED, 4, true);
    }
  #endif

  void injectCommands_P(PGM_P const gcode) { queue.inject_P(gcode); }
  void injectCommands(char * const gcode)  { queue.inject(gcode); }

  bool commandsInQueue() { return (planner.movesplanned() || queue.has_commands_queued()); }

  bool isAxisPositionKnown(const axis_t axis) { return axis_is_trusted((AxisEnum)axis); }
  bool isAxisPositionKnown(const extruder_t) { return axis_is_trusted(E_AXIS); }
  bool isPositionKnown() { return all_axes_trusted(); }
  bool isMachineHomed() { return all_axes_homed(); }

  PGM_P getFirmwareName_str() {
    static PGMSTR(firmware_name, "Marlin " SHORT_BUILD_VERSION);
    return firmware_name;
  }

  void setTargetTemp_celsius(const float &inval, const heater_t heater) {
    float value = inval;
    #ifdef TOUCH_UI_LCD_TEMP_SCALING
      value *= TOUCH_UI_LCD_TEMP_SCALING;
    #endif
    enableHeater(heater);
    switch (heater) {
      #if HAS_HEATED_CHAMBER
        case CHAMBER: thermalManager.setTargetChamber(LROUND(constrain(value, 0, CHAMBER_MAXTEMP - 10))); break;
      #endif
      #if HAS_COOLER
        case COOLER: thermalManager.setTargetCooler(LROUND(constrain(value, 0, COOLER_MAXTEMP))); break;
      #endif
      #if HAS_HEATED_BED
        case BED: thermalManager.setTargetBed(LROUND(constrain(value, 0, BED_MAX_TARGET))); break;
      #endif
      default: {
        #if HAS_HOTEND
          const int16_t e = heater - H0;
          thermalManager.setTargetHotend(LROUND(constrain(value, 0, thermalManager.heater_maxtemp[e] - HOTEND_OVERSHOOT)), e);
        #endif
      } break;
    }
  }

  void setTargetTemp_celsius(const float &inval, const extruder_t extruder) {
    float value = inval;
    #ifdef TOUCH_UI_LCD_TEMP_SCALING
      value *= TOUCH_UI_LCD_TEMP_SCALING;
    #endif
    #if HAS_HOTEND
      const int16_t e = extruder - E0;
      enableHeater(extruder);
      thermalManager.setTargetHotend(LROUND(constrain(value, 0, thermalManager.heater_maxtemp[e] - HOTEND_OVERSHOOT)), e);
    #endif
  }

  void setTargetFan_percent(const float &value, const fan_t fan) {
    #if HAS_FAN
      if (fan < FAN_COUNT)
        thermalManager.set_fan_speed(fan - FAN0, map(constrain(value, 0, 100), 0, 100, 0, 255));
    #else
      UNUSED(value);
      UNUSED(fan);
    #endif
  }

  void setFeedrate_percent(const float &value) { feedrate_percentage = constrain(value, 10, 500); }

  void coolDown() {
    #if HAS_HOTEND
      HOTEND_LOOP() thermalManager.setTargetHotend(0, e);
    #endif
    TERN_(HAS_HEATED_BED, thermalManager.setTargetBed(0));
    TERN_(HAS_FAN, thermalManager.zero_fan_speeds());
  }

  bool awaitingUserConfirm() { return wait_for_user; }

  void setUserConfirmed() { TERN_(HAS_RESUME_CONTINUE, wait_for_user = false); }

  void printFile(const char *filename) {
    UNUSED(filename);
    TERN_(SDSUPPORT, card.openAndPrintFile(filename));
  }

  bool isPrintingFromMediaPaused() {
    return TERN0(SDSUPPORT, isPrintingFromMedia() && !IS_SD_PRINTING());
  }

  bool isPrintingFromMedia() {
    #if ENABLED(SDSUPPORT)
      // Account for when IS_SD_PRINTING() reports the end of the
      // print when there is still SD card data in the planner.
      flags.was_sd_printing = card.isFileOpen() || (flags.was_sd_printing && commandsInQueue());
      return flags.was_sd_printing;
    #else
      return false;
    #endif
  }

  bool isPrinting() {
    return (commandsInQueue() || isPrintingFromMedia() || TERN0(SDSUPPORT, IS_SD_PRINTING())) || print_job_timer.isRunning() || print_job_timer.isPaused();
  }

  bool isPrintingPaused() {
    return isPrinting() && (isPrintingFromMediaPaused() || print_job_timer.isPaused());
  }

  bool isMediaInserted() { return TERN0(SDSUPPORT, IS_SD_INSERTED() && card.isMounted()); }

  void pausePrint() { ui.pause_print(); }
  void resumePrint() { ui.resume_print(); }
  void stopPrint() { ui.abort_print(); }

  void onUserConfirmRequired_P(PGM_P const pstr) {
    char msg[strlen_P(pstr) + 1];
    strcpy_P(msg, pstr);
    onUserConfirmRequired(msg);
  }

  void onStatusChanged_P(PGM_P const pstr) {
    char msg[strlen_P(pstr) + 1];
    strcpy_P(msg, pstr);
    onStatusChanged(msg);
  }

  FileList::FileList() { refresh(); }

  void FileList::refresh() { num_files = 0xFFFF; }

  bool FileList::seek(const uint16_t pos, const bool skip_range_check) {
    #if ENABLED(SDSUPPORT)
      if (!skip_range_check && (pos + 1) > count()) return false;
      card.getfilename_sorted(SD_ORDER(pos, count()));
      return card.filename[0] != '\0';
    #else
      UNUSED(pos);
      UNUSED(skip_range_check);
      return false;
    #endif
  }

  const char* FileList::filename() {
    return TERN(SDSUPPORT, card.longest_filename(), "");
  }

  const char* FileList::shortFilename() {
    return TERN(SDSUPPORT, card.filename, "");
  }

  const char* FileList::longFilename() {
    return TERN(SDSUPPORT, card.longFilename, "");
  }

  bool FileList::isDir() {
    return TERN0(SDSUPPORT, card.flag.filenameIsDir);
  }

  uint16_t FileList::count() {
    return TERN0(SDSUPPORT, (num_files = (num_files == 0xFFFF ? card.get_num_Files() : num_files)));
  }

  bool FileList::isAtRootDir() {
    return TERN1(SDSUPPORT, card.flag.workDirIsRoot);
  }

  void FileList::upDir() {
    #if ENABLED(SDSUPPORT)
      card.cdup();
      num_files = 0xFFFF;
    #endif
  }

  void FileList::changeDir(const char * const dirname) {
    #if ENABLED(SDSUPPORT)
      card.cd(dirname);
      num_files = 0xFFFF;
    #else
      UNUSED(dirname);
    #endif
  }

} // namespace ExtUI

// At the moment we hook into MarlinUI methods, but this could be cleaned up in the future

void MarlinUI::init() { ExtUI::onStartup(); }

void MarlinUI::update() { ExtUI::onIdle(); }

void MarlinUI::kill_screen(PGM_P const error, PGM_P const component) {
  using namespace ExtUI;
  if (!flags.printer_killed) {
    flags.printer_killed = true;
    onPrinterKilled(error, component);
  }
}

#endif // EXTENSIBLE_UI
