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
 * stepper_indirection.cpp
 *
 * Stepper motor driver indirection to allow some stepper functions to
 * be done via SPI/I2c instead of direct pin manipulation.
 *
 * Part of Marlin
 *
 * Copyright (c) 2015 Dominik Wenger
 */

#include "stepper_indirection.h"

#include "../inc/MarlinConfig.h"

//
// TMC26X Driver objects and inits
//
#if HAS_DRIVER(TMC26X)
  #include <SPI.h>

  #ifdef STM32F7
    #include "../HAL/HAL_STM32F7/TMC2660.h"
  #else
    #include <TMC26XStepper.h>
  #endif

  #define _TMC26X_DEFINE(ST) TMC26XStepper stepper##ST(200, ST##_CS_PIN, ST##_STEP_PIN, ST##_DIR_PIN, ST##_MAX_CURRENT, ST##_SENSE_RESISTOR)

  #if AXIS_DRIVER_TYPE(X, TMC26X)
    _TMC26X_DEFINE(X);
  #endif
  #if AXIS_DRIVER_TYPE(X2, TMC26X)
    _TMC26X_DEFINE(X2);
  #endif
  #if AXIS_DRIVER_TYPE(Y, TMC26X)
    _TMC26X_DEFINE(Y);
  #endif
  #if AXIS_DRIVER_TYPE(Y2, TMC26X)
    _TMC26X_DEFINE(Y2);
  #endif
  #if AXIS_DRIVER_TYPE(Z, TMC26X)
    _TMC26X_DEFINE(Z);
  #endif
  #if AXIS_DRIVER_TYPE(Z2, TMC26X)
    _TMC26X_DEFINE(Z2);
  #endif
  #if AXIS_DRIVER_TYPE(E0, TMC26X)
    _TMC26X_DEFINE(E0);
  #endif
  #if AXIS_DRIVER_TYPE(E1, TMC26X)
    _TMC26X_DEFINE(E1);
  #endif
  #if AXIS_DRIVER_TYPE(E2, TMC26X)
    _TMC26X_DEFINE(E2);
  #endif
  #if AXIS_DRIVER_TYPE(E3, TMC26X)
    _TMC26X_DEFINE(E3);
  #endif
  #if AXIS_DRIVER_TYPE(E4, TMC26X)
    _TMC26X_DEFINE(E4);
  #endif

  #define _TMC26X_INIT(A) do{ \
    stepper##A.setMicrosteps(A##_MICROSTEPS); \
    stepper##A.start(); \
  }while(0)

  void tmc26x_init_to_defaults() {
    #if AXIS_DRIVER_TYPE(X, TMC26X)
      _TMC26X_INIT(X);
    #endif
    #if AXIS_DRIVER_TYPE(X2, TMC26X)
      _TMC26X_INIT(X2);
    #endif
    #if AXIS_DRIVER_TYPE(Y, TMC26X)
      _TMC26X_INIT(Y);
    #endif
    #if AXIS_DRIVER_TYPE(Y2, TMC26X)
      _TMC26X_INIT(Y2);
    #endif
    #if AXIS_DRIVER_TYPE(Z, TMC26X)
      _TMC26X_INIT(Z);
    #endif
    #if AXIS_DRIVER_TYPE(Z2, TMC26X)
      _TMC26X_INIT(Z2);
    #endif
    #if AXIS_DRIVER_TYPE(E0, TMC26X)
      _TMC26X_INIT(E0);
    #endif
    #if AXIS_DRIVER_TYPE(E1, TMC26X)
      _TMC26X_INIT(E1);
    #endif
    #if AXIS_DRIVER_TYPE(E2, TMC26X)
      _TMC26X_INIT(E2);
    #endif
    #if AXIS_DRIVER_TYPE(E3, TMC26X)
      _TMC26X_INIT(E3);
    #endif
    #if AXIS_DRIVER_TYPE(E4, TMC26X)
      _TMC26X_INIT(E4);
    #endif
  }
#endif // TMC26X

//
// TMC2130 Driver objects and inits
//
#if HAS_DRIVER(TMC2130)

  #include <SPI.h>
  #include <TMC2130Stepper.h>
  #include "planner.h"
  #include "../core/enum.h"

  #if TMC2130STEPPER_VERSION < 0x020201
    #error "Update TMC2130Stepper library to 2.2.1 or newer."
  #endif

  #if ENABLED(TMC_USE_SW_SPI)
    #define _TMC2130_DEFINE(ST) TMC2130Stepper stepper##ST(ST##_ENABLE_PIN, ST##_DIR_PIN, ST##_STEP_PIN, ST##_CS_PIN, TMC_SW_MOSI, TMC_SW_MISO, TMC_SW_SCK)
  #else
    #define _TMC2130_DEFINE(ST) TMC2130Stepper stepper##ST(ST##_ENABLE_PIN, ST##_DIR_PIN, ST##_STEP_PIN, ST##_CS_PIN)
  #endif

  // Stepper objects of TMC2130 steppers used
  #if AXIS_DRIVER_TYPE(X, TMC2130)
    _TMC2130_DEFINE(X);
  #endif
  #if AXIS_DRIVER_TYPE(X2, TMC2130)
    _TMC2130_DEFINE(X2);
  #endif
  #if AXIS_DRIVER_TYPE(Y, TMC2130)
    _TMC2130_DEFINE(Y);
  #endif
  #if AXIS_DRIVER_TYPE(Y2, TMC2130)
    _TMC2130_DEFINE(Y2);
  #endif
  #if AXIS_DRIVER_TYPE(Z, TMC2130)
    _TMC2130_DEFINE(Z);
  #endif
  #if AXIS_DRIVER_TYPE(Z2, TMC2130)
    _TMC2130_DEFINE(Z2);
  #endif
  #if AXIS_DRIVER_TYPE(E0, TMC2130)
    _TMC2130_DEFINE(E0);
  #endif
  #if AXIS_DRIVER_TYPE(E1, TMC2130)
    _TMC2130_DEFINE(E1);
  #endif
  #if AXIS_DRIVER_TYPE(E2, TMC2130)
    _TMC2130_DEFINE(E2);
  #endif
  #if AXIS_DRIVER_TYPE(E3, TMC2130)
    _TMC2130_DEFINE(E3);
  #endif
  #if AXIS_DRIVER_TYPE(E4, TMC2130)
    _TMC2130_DEFINE(E4);
  #endif

  // Use internal reference voltage for current calculations. This is the default.
  // Following values from Trinamic's spreadsheet with values for a NEMA17 (42BYGHW609)
  // https://www.trinamic.com/products/integrated-circuits/details/tmc2130/
  void tmc2130_init(TMC2130Stepper &st, const uint16_t mA, const uint16_t microsteps, const uint32_t thrs, const float spmm, const bool intpol = INTERPOLATE, const float holdmult = HOLD_MULTIPLIER,\
                    const uint8_t tblank = 24, const uint8_t toff = 5, const uint8_t hstart = 3, const int8_t hend = 2, const uint8_t sync = 0, const int8_t sgt = 0, \
                    const bool stealth = false, const uint8_t pwmfreq = 1, const bool pwmauto = true, const uint8_t pwmgrad = 5, const uint8_t pwmampl = 255, const bool pwmsym = false, \
                    const uint8_t wave = 0) {
    #if DISABLED(STEALTHCHOP) || DISABLED(HYBRID_THRESHOLD)
      UNUSED(thrs);
      UNUSED(spmm);
    #endif
    st.begin();
    st.setCurrent(mA, R_SENSE, holdmult);
    st.microsteps(microsteps);
    st.blank_time(tblank);
    st.off_time(toff);
    st.interpolate(intpol);
    st.power_down_delay(128); // ~2s until driver lowers to hold current
    st.hysteresis_start(hstart);
    st.hysteresis_end(hend);
    st.sync_phases(sync);
    st.stealth_freq(pwmfreq);
    st.stealth_autoscale(pwmauto);
    st.stealth_gradient(pwmgrad);
    st.stealth_amplitude(pwmampl);
    st.stealth_symmetric(pwmsym);
    st.sgt(sgt);
    #if ENABLED(STEALTHCHOP)
      st.stealthChop(stealth);
      #if ENABLED(HYBRID_THRESHOLD)
        st.stealth_max_speed(12650000UL*microsteps/(256*thrs*spmm));
      #endif
    #endif
    #if ENABLED(TMC2130_LINEARITY_CORRECTION_PRESET) 
      tmc2130_set_fixed_wave(st, wave);
    #endif
    st.GSTAT(); // Clear GSTAT
  }

  //#define _TMC2130_INIT(ST, SPMM) tmc2130_init(stepper##ST, ST##_CURRENT, ST##_MICROSTEPS, ST##_HYBRID_THRESHOLD, SPMM)
  #define _TMC2130_INIT(ST, SPMM) tmc2130_init(stepper##ST, ST##_CURRENT, ST##_MICROSTEPS, ST##_HYBRID_THRESHOLD, SPMM, INTERPOLATE, ST##_HOLD_MULTIPLIER, \
                                               ST##_TBL, ST##_TOFF, ST##_HSTRT, ST##_HEND, ST##_SYNC, ST##_SGT, \
                                               ST##_STEALTHCHOP, ST##_SFREQ, ST##_SAUTO, ST##_SGRAD, ST##_SAMPL, ST##_SSYM, \
                                               ST##_TMC2130_LINEARITY_CORRECTION_WAVE)

  void tmc2130_init_to_defaults() {
    #if AXIS_DRIVER_TYPE(X, TMC2130)
      _TMC2130_INIT( X, planner.axis_steps_per_mm[X_AXIS]);
    #endif
    #if AXIS_DRIVER_TYPE(X2, TMC2130)
      _TMC2130_INIT(X2, planner.axis_steps_per_mm[X_AXIS]);
    #endif
    #if AXIS_DRIVER_TYPE(Y, TMC2130)
      _TMC2130_INIT( Y, planner.axis_steps_per_mm[Y_AXIS]);
    #endif
    #if AXIS_DRIVER_TYPE(Y2, TMC2130)
      _TMC2130_INIT(Y2, planner.axis_steps_per_mm[Y_AXIS]);
    #endif
    #if AXIS_DRIVER_TYPE(Z, TMC2130)
      _TMC2130_INIT( Z, planner.axis_steps_per_mm[Z_AXIS]);
    #endif
    #if AXIS_DRIVER_TYPE(Z2, TMC2130)
      _TMC2130_INIT(Z2, planner.axis_steps_per_mm[Z_AXIS]);
    #endif
    #if AXIS_DRIVER_TYPE(E0, TMC2130)
      _TMC2130_INIT(E0, planner.axis_steps_per_mm[E_AXIS]);
    #endif
    #if AXIS_DRIVER_TYPE(E1, TMC2130)
      { constexpr int extruder = 1; _TMC2130_INIT(E1, planner.axis_steps_per_mm[E_AXIS_N]); }
    #endif
    #if AXIS_DRIVER_TYPE(E2, TMC2130)
      { constexpr int extruder = 2; _TMC2130_INIT(E2, planner.axis_steps_per_mm[E_AXIS_N]); }
    #endif
    #if AXIS_DRIVER_TYPE(E3, TMC2130)
      { constexpr int extruder = 3; _TMC2130_INIT(E3, planner.axis_steps_per_mm[E_AXIS_N]); }
    #endif
    #if AXIS_DRIVER_TYPE(E4, TMC2130)
      { constexpr int extruder = 4; _TMC2130_INIT(E4, planner.axis_steps_per_mm[E_AXIS_N]); }
    #endif

    #if ENABLED(SENSORLESS_HOMING)
      #define TMC_INIT_SGT(P,Q) stepper##Q.sgt(P##_HOMING_SENSITIVITY);
      #if X_SENSORLESS
        #if AXIS_DRIVER_TYPE(X, TMC2130)
          stepperX.sgt(X_HOMING_SENSITIVITY);
        #endif
        #if AXIS_DRIVER_TYPE(X2, TMC2130)
          stepperX2.sgt(X_HOMING_SENSITIVITY);
        #endif
      #endif
      #if Y_SENSORLESS
        #if AXIS_DRIVER_TYPE(Y, TMC2130)
          stepperY.sgt(Y_HOMING_SENSITIVITY);
        #endif
        #if AXIS_DRIVER_TYPE(Y2, TMC2130)
          stepperY2.sgt(Y_HOMING_SENSITIVITY);
        #endif
      #endif
      #if Z_SENSORLESS
        #if AXIS_DRIVER_TYPE(Z, TMC2130)
          stepperZ.sgt(Z_HOMING_SENSITIVITY);
        #endif
        #if AXIS_DRIVER_TYPE(Z2, TMC2130)
          stepperZ2.sgt(Z_HOMING_SENSITIVITY);
        #endif
      #endif
    #endif
  }

  #if ENABLED(TMC2130_LINEARITY_CORRECTION) // TMC2130_LINEARITY_CORRECTION

    void tmc2130_wr_MSLUTSTART(TMC2130Stepper &st, uint8_t start_sin, uint8_t start_sin90)
    {
      uint32_t val = 0;
      val |= (uint32_t)start_sin;
      val |= ((uint32_t)start_sin90) << 16;
      SERIAL_PROTOCOLPGM(" MSLUTSTART: ");
      SERIAL_PRINTLN(val, HEX);
      st.lut_mslutstart(val);
    }

    void tmc2130_wr_MSLUTSEL(TMC2130Stepper &st, uint8_t x1, uint8_t x2, uint8_t x3, int8_t w0, int8_t w1, int8_t w2, int8_t w3)
    {
      uint32_t val = 0;
      val |= ((uint32_t)w0);
      val |= ((uint32_t)w1) << 2;
      val |= ((uint32_t)w2) << 4;
      val |= ((uint32_t)w3) << 6;
      val |= ((uint32_t)x1) << 8;
      val |= ((uint32_t)x2) << 16;
      val |= ((uint32_t)x3) << 24;
      SERIAL_PROTOCOLPGM(" MSLUTSEL: ");
      SERIAL_PRINTLN(val, HEX);
      st.lut_msutsel(val);
    }

    void tmc2130_wr_MSLUT(TMC2130Stepper &st, uint8_t i, uint32_t val)
    {
      SERIAL_PROTOCOLPGM(" MSLUT");
      SERIAL_PRINT(i, DEC);
      SERIAL_PROTOCOLPGM(" : ");
      SERIAL_PRINTLN(val, HEX);
      switch (i) {
        case 0: st.lut_mslut0(val); break;
        case 1: st.lut_mslut1(val); break;
        case 2: st.lut_mslut2(val); break;
        case 3: st.lut_mslut3(val); break;
        case 4: st.lut_mslut4(val); break;
        case 5: st.lut_mslut5(val); break;
        case 6: st.lut_mslut6(val); break;
        case 7: st.lut_mslut7(val); break;
        default: break;
      }
    }

    void tmc2130_reset_wave(TMC2130Stepper &st) // TMC2130_LINEARITY_CORRECTION
    {
      SERIAL_PROTOCOLLNPGM(" MSLUT RESET ");
      st.lut_mslutstart(0x00F70000UL);      // 0x00F70000 16187392
      st.lut_mslut0(0xAAAAB554UL);          // 0xAAAAB554 2863314260
      st.lut_mslut1(0x4A9554AAUL);          // 0x4A9554AA 1251300522
      st.lut_mslut2(0x24492929UL);          // 0x24492929 608774441
      st.lut_mslut3(0x10104222UL);          // 0x10104222 269500962
      st.lut_mslut4(0xFBFFFFFFUL);          // 0xFBFFFFFF 4227858431
      st.lut_mslut5(0xB5BB777DUL);          // 0xB5BB777D 3048961917
      st.lut_mslut6(0x49295556UL);          // 0x49295556 1227445590
      st.lut_mslut7(0x00404222UL);          // 0x00404222 4211234
      st.lut_msutsel(0xFFFF8056UL);         // 0xFFFF8056 4294934614  
    }

    void tmc2130_set_fixed_wave(TMC2130Stepper &st, uint8_t i) // TMC2130_LINEARITY_CORRECTION
    {
      if (i == 1)
      {
        SERIAL_PROTOCOLLNPGM(" MSLUT Wave: SoundCorrection [X]");
        st.lut_mslutstart(0x00FC0000UL); 
        st.lut_mslut0(0xAAAAB554UL); 
        st.lut_mslut1(0x52AAAAAAUL); 
        st.lut_mslut2(0x91252529UL); 
        st.lut_mslut3(0x10208848UL); 
        st.lut_mslut4(0xBFFC0200UL); 
        st.lut_mslut5(0xB6DBBBDFUL); 
        st.lut_mslut6(0x24A55556UL); 
        st.lut_mslut7(0x00041089UL); 
        st.lut_msutsel(0xFFFF9156UL); 				
      }	
      else if (i == 2)
      {
        SERIAL_PROTOCOLLNPGM(" MSLUT Wave: LaserCorrection [X]");
        st.lut_mslutstart(0x00FC0000UL); 					
        st.lut_mslut0(0xD52ADB54UL); 					
        st.lut_mslut1(0x52AAAAAAUL); 					
        st.lut_mslut2(0x91252529UL); 					
        st.lut_mslut3(0x10208848UL); 					
        st.lut_mslut4(0xBFFC0200UL); 					
        st.lut_mslut5(0xB6DBBBDFUL); 					
        st.lut_mslut6(0x24A55556UL); 					
        st.lut_mslut7(0x00040089UL); 					
        st.lut_msutsel(0xFFFF9156UL); 														 				
      }	
      else if (i == 3)
      {
        SERIAL_PROTOCOLLNPGM(" MSLUT Wave: SoundCorrection [E]");
        st.lut_mslutstart(0x00FC0000UL); 
        st.lut_mslut0(0x6B6DBBDEUL); 
        st.lut_mslut1(0x4AAAAAADUL); 
        st.lut_mslut2(0x444924A5UL); 
        st.lut_mslut3(0x20041044UL); 
        st.lut_mslut4(0xBEFF8000UL); 
        st.lut_mslut5(0x5ADB76F7UL); 
        st.lut_mslut6(0x894A5555UL); 
        st.lut_mslut7(0x00020844UL); 
        st.lut_msutsel(0xFFFF8E56UL); 																		 				
      }	
      else if (i == 4)
      {
        SERIAL_PROTOCOLLNPGM(" MSLUT Wave: LaserCorrection [E]");
        st.lut_mslutstart(0x00FD0000UL); 					
        st.lut_mslut0(0xD5BAD554UL); 					
        st.lut_mslut1(0x92A4AAABUL); 					
        st.lut_mslut2(0x91356529UL); 					
        st.lut_mslut3(0x10248848UL); 					
        st.lut_mslut4(0xBFDC2200UL); 					
        st.lut_mslut5(0xB6DABBDBUL); 					
        st.lut_mslut6(0xA4A85552UL); 					
        st.lut_mslut7(0x00040089UL); 					
        st.lut_msutsel(0xFFFF9156UL); 																									 				
      }	
      else
      {
        SERIAL_PROTOCOLLNPGM(" ERROR: Not a preset value. ");
      }
    }

    void tmc2130_set_wave(TMC2130Stepper &st, uint8_t amp, int16_t fac1000, int8_t xoff, int8_t yoff, uint8_t wavetype, bool config, uint8_t addto) // TMC2130_LINEARITY_CORRECTION
    {
    // TMC2130 wave compression algorithm
    // optimized for minimal memory requirements
    // wavetype: 0 (default) pow(Sin,1+x); 1 pow(Cycle,2+x)
      if (fac1000 < TMC2130_WAVE_FAC1000_MIN) fac1000 = TMC2130_WAVE_FAC1000_MIN;
      if (fac1000 > TMC2130_WAVE_FAC1000_MAX) fac1000 = TMC2130_WAVE_FAC1000_MAX;
      float fac;
      if (wavetype == 1) 
      {
        SERIAL_PROTOCOLPGM(" tmc2130_set_wave: Cycle-Factor: ");
        fac = ((float)(2000 + fac1000) / 1000);
      }
      else 
      {
        SERIAL_PROTOCOLPGM(" tmc2130_set_wave: Sin-Factor: ");
        fac = ((float)(1000 + fac1000) / 1000);
      }
      SERIAL_PROTOCOLLN(fac);
      uint8_t vA = 0;                //value of currentA
      uint8_t va = 0;                //previous vA
      uint8_t w[4] = {1,1,1,1};      //W bits (MSLUTSEL)
      uint8_t x[3] = {255,255,255};  //X segment bounds (MSLUTSEL)
      int8_t s = 0;                  //current segment
      int8_t b;                      //encoded bit value
      int8_t dA;                     //delta value
      int i;                         //microstep index
      uint32_t reg;                  //tmc2130 register
      tmc2130_wr_MSLUTSTART(st, 0, amp);
      //set first W-Bit
      if (wavetype == 1) 
      { // Cycle
        w[0] = (uint8_t)((amp-1) *  pow(1 - pow((float)xoff/256-1,2), fac/2) + yoff/10);
      }
      else 
      { // Sin
        w[0] = (uint8_t)((amp-1) * pow(sin((2*PI*xoff)/1024), fac) + yoff/10);
      }
      for (i = 0; i < 256; i++)
      {
        if (wavetype == 1) 
        { // Cycle
          vA = (uint8_t)((amp-1) *  pow(1 - pow(((float)i+xoff)/256-1,2), fac/2) + yoff/10);
        }
        else 
        { // Sin
          vA = (uint8_t)((amp-1) * pow(sin((2*PI*(i+xoff))/1024), fac) + yoff/10);
        }
        dA = (int8_t)(vA - va);
        if (dA > w[0]) 
        {
          w[0]++; 
          break;
        }
        if (dA < w[0]) 
        {
          //w[0]--; 
          break;
        }
        va = vA;
      }
      int8_t d0 = (int8_t)w[0] -1;   
      int8_t d1 = (int8_t)w[0];
      // calc MSLUT
      va = 0;
      for (i =0; i < 256 ; i++)
      {
        if (( i & 31) == 0) 
        {
          reg = 0;
        }  
        if (wavetype == 1) 
        { // Cycle
          vA = (uint8_t)((amp-1) *  pow(1 - pow(((float)i+xoff)/256-1,2), fac/2) + yoff/10);
        }
        else 
        { // Sin
          vA = (uint8_t)((amp-1) * pow(sin((2*PI*(i+xoff))/1024), fac) + yoff/10); // + 0.5 for Round (compiler will truncate)
        }

        dA = (int8_t)(vA - va); // calculate delta
        va = vA;
        b = -1;
        if (dA == d0)      //delta == delta0 => bit=0
        {
          b = 0; 
        }
        else if (dA == d1)  //delta == delta1 => bit=1
        {
          b = 1;
        }
        else
        {
          if (dA < d0) // delta < delta0 => switch wbit down
          {
            b = 0;
            switch (dA)
            {
            case -1: d0 = -1; d1 = 0; w[s+1] = 0; break;
            case  0: d0 =  0; d1 = 1; w[s+1] = 1; break;
            case  1: d0 =  1; d1 = 2; w[s+1] = 2; break;
            default: b = -1; break;
            }
            if (b >= 0) 
            { 
              x[s] = i; 
              s++; 
            }
          }
          else if (dA > d1) // delta > delta0 => switch wbit up
          {
            b = 1;
            switch (dA)
            {
            case  1: d0 =  0; d1 = 1; w[s+1] = 1; break;
            case  2: d0 =  1; d1 = 2; w[s+1] = 2; break;
            case  3: d0 =  2; d1 = 3; w[s+1] = 3; break;
            default: b = -1; break;
            }
              if (b >= 0) 
              {
                 x[s] = i;
                 s++; 
              }
          }
        }
        if (config == 1) if (i == addto)  // Additional Configuration
        {
          if (b == 0) b = 1;
          else if (b == 1) b = 0;
        }
        if (b < 0) break; // delta out of range (<-1 or >3)
        if (s > 3) break; // segment out of range (> 3)
        if (b == 1) 
        {
          reg |= 0x80000000;
        }
        //SERIAL_PRINT(i, DEC); 
        //SERIAL_PROTOCOLPGM(" \t- vA ");
        //SERIAL_PRINT(vA, DEC);
        //SERIAL_PROTOCOLPGM(" \t- dA ");
        //SERIAL_PRINT(dA, DEC);
        //SERIAL_PROTOCOLPGM(" \t- s ");
        //SERIAL_PRINT(s, DEC);
        //SERIAL_PROTOCOLPGM(" \t- b ");
        //SERIAL_PRINT(b, DEC);
        //SERIAL_PROTOCOLPGM(" \t- w ");
        //SERIAL_PRINTLN(w[s], DEC);
        if ((i & 31) == 31)
        { 
          tmc2130_wr_MSLUT(st, (uint8_t)(i >> 5), reg);
        }
        else  
        {
          reg >>= 1;
        }
      }
      tmc2130_wr_MSLUTSEL(st, x[0], x[1], x[2], w[0], w[1], w[2], w[3]);
      SERIAL_PROTOCOLPGM(" X = ");
      SERIAL_PRINT(x[0], DEC);
      SERIAL_PROTOCOLPGM(" - ");
      SERIAL_PRINT(x[1], DEC);
      SERIAL_PROTOCOLPGM(" - ");
      SERIAL_PRINT(x[2], DEC);
      SERIAL_PROTOCOLPGM(" W = ");
      SERIAL_PRINT(w[0], DEC);
      SERIAL_PROTOCOLPGM(" - ");
      SERIAL_PRINT(w[1], DEC);
      SERIAL_PROTOCOLPGM(" - ");
      SERIAL_PRINT(w[2], DEC);
      SERIAL_PROTOCOLPGM(" - ");
      SERIAL_PRINTLN(w[3], DEC);
    }

  #endif // TMC2130_LINEARITY_CORRECTION
#endif // TMC2130

//
// TMC2208 Driver objects and inits
//
#if HAS_DRIVER(TMC2208)
  #include <HardwareSerial.h>
  #include <TMC2208Stepper.h>
  #include "planner.h"

  #if TMC2208STEPPER_VERSION < 0x000101
    #error "Update TMC2208Stepper library to 0.1.1 or newer."
  #endif

  #define _TMC2208_DEFINE_HARDWARE(ST) TMC2208Stepper stepper##ST(&ST##_HARDWARE_SERIAL)
  #define _TMC2208_DEFINE_SOFTWARE(ST) TMC2208Stepper stepper##ST(ST##_SERIAL_RX_PIN, ST##_SERIAL_TX_PIN, ST##_SERIAL_RX_PIN > -1)

  // Stepper objects of TMC2208 steppers used
  #if AXIS_DRIVER_TYPE(X, TMC2208)
    #ifdef X_HARDWARE_SERIAL
      _TMC2208_DEFINE_HARDWARE(X);
    #else
      _TMC2208_DEFINE_SOFTWARE(X);
    #endif
  #endif
  #if AXIS_DRIVER_TYPE(X2, TMC2208)
    #ifdef X2_HARDWARE_SERIAL
      _TMC2208_DEFINE_HARDWARE(X2);
    #else
      _TMC2208_DEFINE_SOFTWARE(X2);
    #endif
  #endif
  #if AXIS_DRIVER_TYPE(Y, TMC2208)
    #ifdef Y_HARDWARE_SERIAL
      _TMC2208_DEFINE_HARDWARE(Y);
    #else
      _TMC2208_DEFINE_SOFTWARE(Y);
    #endif
  #endif
  #if AXIS_DRIVER_TYPE(Y2, TMC2208)
    #ifdef Y2_HARDWARE_SERIAL
      _TMC2208_DEFINE_HARDWARE(Y2);
    #else
      _TMC2208_DEFINE_SOFTWARE(Y2);
    #endif
  #endif
  #if AXIS_DRIVER_TYPE(Z, TMC2208)
    #ifdef Z_HARDWARE_SERIAL
      _TMC2208_DEFINE_HARDWARE(Z);
    #else
      _TMC2208_DEFINE_SOFTWARE(Z);
    #endif
  #endif
  #if AXIS_DRIVER_TYPE(Z2, TMC2208)
    #ifdef Z2_HARDWARE_SERIAL
      _TMC2208_DEFINE_HARDWARE(Z2);
    #else
      _TMC2208_DEFINE_SOFTWARE(Z2);
    #endif
  #endif
  #if AXIS_DRIVER_TYPE(E0, TMC2208)
    #ifdef E0_HARDWARE_SERIAL
      _TMC2208_DEFINE_HARDWARE(E0);
    #else
      _TMC2208_DEFINE_SOFTWARE(E0);
    #endif
  #endif
  #if AXIS_DRIVER_TYPE(E1, TMC2208)
    #ifdef E1_HARDWARE_SERIAL
      _TMC2208_DEFINE_HARDWARE(E1);
    #else
      _TMC2208_DEFINE_SOFTWARE(E1);
    #endif
  #endif
  #if AXIS_DRIVER_TYPE(E2, TMC2208)
    #ifdef E2_HARDWARE_SERIAL
      _TMC2208_DEFINE_HARDWARE(E2);
    #else
      _TMC2208_DEFINE_SOFTWARE(E2);
    #endif
  #endif
  #if AXIS_DRIVER_TYPE(E3, TMC2208)
    #ifdef E3_HARDWARE_SERIAL
      _TMC2208_DEFINE_HARDWARE(E3);
    #else
      _TMC2208_DEFINE_SOFTWARE(E3);
    #endif
  #endif
  #if AXIS_DRIVER_TYPE(E4, TMC2208)
    #ifdef E4_HARDWARE_SERIAL
      _TMC2208_DEFINE_HARDWARE(E4);
    #else
      _TMC2208_DEFINE_SOFTWARE(E4);
    #endif
  #endif

  void tmc2208_serial_begin() {
    #if AXIS_DRIVER_TYPE(X, TMC2208)
      #ifdef X_HARDWARE_SERIAL
        X_HARDWARE_SERIAL.begin(115200);
      #else
        stepperX.beginSerial(115200);
      #endif
    #endif
    #if AXIS_DRIVER_TYPE(X2, TMC2208)
      #ifdef X2_HARDWARE_SERIAL
        X2_HARDWARE_SERIAL.begin(115200);
      #else
        stepperX2.beginSerial(115200);
      #endif
    #endif
    #if AXIS_DRIVER_TYPE(Y, TMC2208)
      #ifdef Y_HARDWARE_SERIAL
        Y_HARDWARE_SERIAL.begin(115200);
      #else
        stepperY.beginSerial(115200);
      #endif
    #endif
    #if AXIS_DRIVER_TYPE(Y2, TMC2208)
      #ifdef Y2_HARDWARE_SERIAL
        Y2_HARDWARE_SERIAL.begin(115200);
      #else
        stepperY2.beginSerial(115200);
      #endif
    #endif
    #if AXIS_DRIVER_TYPE(Z, TMC2208)
      #ifdef Z_HARDWARE_SERIAL
        Z_HARDWARE_SERIAL.begin(115200);
      #else
        stepperZ.beginSerial(115200);
      #endif
    #endif
    #if AXIS_DRIVER_TYPE(Z2, TMC2208)
      #ifdef Z2_HARDWARE_SERIAL
        Z2_HARDWARE_SERIAL.begin(115200);
      #else
        stepperZ2.beginSerial(115200);
      #endif
    #endif
    #if AXIS_DRIVER_TYPE(E0, TMC2208)
      #ifdef E0_HARDWARE_SERIAL
        E0_HARDWARE_SERIAL.begin(115200);
      #else
        stepperE0.beginSerial(115200);
      #endif
    #endif
    #if AXIS_DRIVER_TYPE(E1, TMC2208)
      #ifdef E1_HARDWARE_SERIAL
        E1_HARDWARE_SERIAL.begin(115200);
      #else
        stepperE1.beginSerial(115200);
      #endif
    #endif
    #if AXIS_DRIVER_TYPE(E2, TMC2208)
      #ifdef E2_HARDWARE_SERIAL
        E2_HARDWARE_SERIAL.begin(115200);
      #else
        stepperE2.beginSerial(115200);
      #endif
    #endif
    #if AXIS_DRIVER_TYPE(E3, TMC2208)
      #ifdef E3_HARDWARE_SERIAL
        E3_HARDWARE_SERIAL.begin(115200);
      #else
        stepperE3.beginSerial(115200);
      #endif
    #endif
    #if AXIS_DRIVER_TYPE(E4, TMC2208)
      #ifdef E4_HARDWARE_SERIAL
        E4_HARDWARE_SERIAL.begin(115200);
      #else
        stepperE4.beginSerial(115200);
      #endif
    #endif
  }

  // Use internal reference voltage for current calculations. This is the default.
  // Following values from Trinamic's spreadsheet with values for a NEMA17 (42BYGHW609)
  void tmc2208_init(TMC2208Stepper &st, const uint16_t mA, const uint16_t microsteps, const uint32_t thrs, const float spmm) {
    st.pdn_disable(true); // Use UART
    st.mstep_reg_select(true); // Select microsteps with UART
    st.I_scale_analog(false);
    st.rms_current(mA, HOLD_MULTIPLIER, R_SENSE);
    st.microsteps(microsteps);
    st.blank_time(24);
    st.toff(5);
    st.intpol(INTERPOLATE);
    st.TPOWERDOWN(128); // ~2s until driver lowers to hold current
    st.hysteresis_start(3);
    st.hysteresis_end(2);
    #if ENABLED(STEALTHCHOP)
      st.pwm_lim(12);
      st.pwm_reg(8);
      st.pwm_autograd(1);
      st.pwm_autoscale(1);
      st.pwm_freq(1);
      st.pwm_grad(14);
      st.pwm_ofs(36);
      st.en_spreadCycle(false);
      #if ENABLED(HYBRID_THRESHOLD)
        st.TPWMTHRS(12650000UL*microsteps/(256*thrs*spmm));
      #else
        UNUSED(thrs);
        UNUSED(spmm);
      #endif
    #else
      st.en_spreadCycle(true);
    #endif
    st.GSTAT(0b111); // Clear
    delay(200);
  }

  #define _TMC2208_INIT(ST, SPMM) tmc2208_init(stepper##ST, ST##_CURRENT, ST##_MICROSTEPS, ST##_HYBRID_THRESHOLD, SPMM)

  void tmc2208_init_to_defaults() {
    #if AXIS_DRIVER_TYPE(X, TMC2208)
      _TMC2208_INIT(X, planner.axis_steps_per_mm[X_AXIS]);
    #endif
    #if AXIS_DRIVER_TYPE(X2, TMC2208)
      _TMC2208_INIT(X2, planner.axis_steps_per_mm[X_AXIS]);
    #endif
    #if AXIS_DRIVER_TYPE(Y, TMC2208)
      _TMC2208_INIT(Y, planner.axis_steps_per_mm[Y_AXIS]);
    #endif
    #if AXIS_DRIVER_TYPE(Y2, TMC2208)
      _TMC2208_INIT(Y2, planner.axis_steps_per_mm[Y_AXIS]);
    #endif
    #if AXIS_DRIVER_TYPE(Z, TMC2208)
      _TMC2208_INIT(Z, planner.axis_steps_per_mm[Z_AXIS]);
    #endif
    #if AXIS_DRIVER_TYPE(Z2, TMC2208)
      _TMC2208_INIT(Z2, planner.axis_steps_per_mm[Z_AXIS]);
    #endif
    #if AXIS_DRIVER_TYPE(E0, TMC2208)
      _TMC2208_INIT(E0, planner.axis_steps_per_mm[E_AXIS]);
    #endif
    #if AXIS_DRIVER_TYPE(E1, TMC2208)
      { constexpr int extruder = 1; _TMC2208_INIT(E1, planner.axis_steps_per_mm[E_AXIS_N]); }
    #endif
    #if AXIS_DRIVER_TYPE(E2, TMC2208)
      { constexpr int extruder = 2; _TMC2208_INIT(E2, planner.axis_steps_per_mm[E_AXIS_N]); }
    #endif
    #if AXIS_DRIVER_TYPE(E3, TMC2208)
      { constexpr int extruder = 3; _TMC2208_INIT(E3, planner.axis_steps_per_mm[E_AXIS_N]); }
    #endif
    #if AXIS_DRIVER_TYPE(E4, TMC2208)
      { constexpr int extruder = 4; _TMC2208_INIT(E4, planner.axis_steps_per_mm[E_AXIS_N]); }
    #endif
  }
#endif // TMC2208

void restore_stepper_drivers() {
  #if AXIS_IS_TMC(X)
    stepperX.push();
  #endif
  #if AXIS_IS_TMC(X2)
    stepperX2.push();
  #endif
  #if AXIS_IS_TMC(Y)
    stepperY.push();
  #endif
  #if AXIS_IS_TMC(Y2)
    stepperY2.push();
  #endif
  #if AXIS_IS_TMC(Z)
    stepperZ.push();
  #endif
  #if AXIS_IS_TMC(Z2)
    stepperZ2.push();
  #endif
  #if AXIS_IS_TMC(E0)
    stepperE0.push();
  #endif
  #if AXIS_IS_TMC(E1)
    stepperE1.push();
  #endif
  #if AXIS_IS_TMC(E2)
    stepperE2.push();
  #endif
  #if AXIS_IS_TMC(E3)
    stepperE3.push();
  #endif
  #if AXIS_IS_TMC(E4)
    stepperE4.push();
  #endif
}

void reset_stepper_drivers() {
  #if HAS_DRIVER(TMC26X)
    tmc26x_init_to_defaults();
  #endif
  #if HAS_DRIVER(TMC2130)
    delay(100);
    tmc2130_init_to_defaults();
  #endif
  #if HAS_DRIVER(TMC2208)
    delay(100);
    tmc2208_init_to_defaults();
  #endif
  #ifdef TMC_ADV
    TMC_ADV()
  #endif
  #if HAS_DRIVER(L6470)
    L6470_init_to_defaults();
  #endif
}

//
// L6470 Driver objects and inits
//
#if HAS_DRIVER(L6470)

  #include <SPI.h>
  #include <L6470.h>

  #define _L6470_DEFINE(ST) L6470 stepper##ST(ST##_ENABLE_PIN)

  // L6470 Stepper objects
  #if AXIS_DRIVER_TYPE(X, L6470)
    _L6470_DEFINE(X);
  #endif
  #if AXIS_DRIVER_TYPE(X2, L6470)
    _L6470_DEFINE(X2);
  #endif
  #if AXIS_DRIVER_TYPE(Y, L6470)
    _L6470_DEFINE(Y);
  #endif
  #if AXIS_DRIVER_TYPE(Y2, L6470)
    _L6470_DEFINE(Y2);
  #endif
  #if AXIS_DRIVER_TYPE(Z, L6470)
    _L6470_DEFINE(Z);
  #endif
  #if AXIS_DRIVER_TYPE(Z2, L6470)
    _L6470_DEFINE(Z2);
  #endif
  #if AXIS_DRIVER_TYPE(E0, L6470)
    _L6470_DEFINE(E0);
  #endif
  #if AXIS_DRIVER_TYPE(E1, L6470)
    _L6470_DEFINE(E1);
  #endif
  #if AXIS_DRIVER_TYPE(E2, L6470)
    _L6470_DEFINE(E2);
  #endif
  #if AXIS_DRIVER_TYPE(E3, L6470)
    _L6470_DEFINE(E3);
  #endif
  #if AXIS_DRIVER_TYPE(E4, L6470)
    _L6470_DEFINE(E4);
  #endif

  #define _L6470_INIT(A) do{ \
    stepper##A.init(); \
    stepper##A.softFree(); \
    stepper##A.setMicroSteps(A##_MICROSTEPS); \
    stepper##A.setOverCurrent(A##_OVERCURRENT); \
    stepper##A.setStallCurrent(A##_STALLCURRENT); \
  }while(0)

  void L6470_init_to_defaults() {
    #if AXIS_DRIVER_TYPE(X, L6470)
      _L6470_INIT(X);
    #endif
    #if AXIS_DRIVER_TYPE(X2, L6470)
      _L6470_INIT(X2);
    #endif
    #if AXIS_DRIVER_TYPE(Y, L6470)
      _L6470_INIT(Y);
    #endif
    #if AXIS_DRIVER_TYPE(Y2, L6470)
      _L6470_INIT(Y2);
    #endif
    #if AXIS_DRIVER_TYPE(Z, L6470)
      _L6470_INIT(Z);
    #endif
    #if AXIS_DRIVER_TYPE(Z2, L6470)
      _L6470_INIT(Z2);
    #endif
    #if AXIS_DRIVER_TYPE(E0, L6470)
      _L6470_INIT(E0);
    #endif
    #if AXIS_DRIVER_TYPE(E1, L6470)
      _L6470_INIT(E1);
    #endif
    #if AXIS_DRIVER_TYPE(E2, L6470)
      _L6470_INIT(E2);
    #endif
    #if AXIS_DRIVER_TYPE(E3, L6470)
      _L6470_INIT(E3);
    #endif
    #if AXIS_DRIVER_TYPE(E4, L6470)
      _L6470_INIT(E4);
    #endif
  }

#endif // L6470
