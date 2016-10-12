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

// How many DIO pins are defined?
#if defined(DIO85_PIN)
  #define DIO_COUNT 86
#elif defined(DIO53_PIN)
  #define DIO_COUNT 54
#elif defined(DIO47_PIN)
  #define DIO_COUNT 48
#elif defined(DIO31_PIN)
  #define DIO_COUNT 32
#elif defined(DIO21_PIN)
  #define DIO_COUNT 22
#endif

#define _PIN_SAY(NAME) { SERIAL_ECHOPGM(STRINGIFY(NAME)); return true; }
#define PIN_SAY(NAME) if (pin == NAME) _PIN_SAY(_##NAME##_);
#define ANALOG_PIN_SAY(NAME) if (pin == analogInputToDigitalPin(NAME)) _PIN_SAY(_##NAME##_);
#define IS_ANALOG(P) ((P) >= analogInputToDigitalPin(0) && ((P) <= analogInputToDigitalPin(15) || (P) <= analogInputToDigitalPin(5)))

// Report pin name for a given fastio digital pin index
static bool report_pin_name(int8_t pin) {

  SERIAL_ECHO((int)pin);
  SERIAL_CHAR(' ');

  if (IS_ANALOG(pin)) {
    SERIAL_CHAR('('); SERIAL_CHAR('A');
    SERIAL_ECHO(int(pin - analogInputToDigitalPin(0)));
    SERIAL_CHAR(')'); SERIAL_CHAR(' ');
  }

  #if defined(RXD) && RXD > -1
    if (pin == 0) { SERIAL_ECHOPGM("RXD"); return true; }
  #endif
  #if defined(TXD) && TXD > -1
    if (pin == 1) { SERIAL_ECHOPGM("TXD"); return true; }
  #endif

  #if defined(__FD) && __FD > -1   // Pin list updated from 7 OCT RCBugfix branch
    PIN_SAY(__FD)
  #endif
  #if defined(__FS) && __FS > -1
    PIN_SAY(__FS)
  #endif
  #if defined(__GD) && __GD > -1
    PIN_SAY(__GD)
  #endif
  #if defined(__GS) && __GS > -1
    PIN_SAY(__GS)
  #endif
  #if defined(AVR_MISO_PIN) && AVR_MISO_PIN > -1
    PIN_SAY(AVR_MISO_PIN)
  #endif
  #if defined(AVR_MOSI_PIN) && AVR_MOSI_PIN > -1
    PIN_SAY(AVR_MOSI_PIN)
  #endif
  #if defined(AVR_SCK_PIN) && AVR_SCK_PIN > -1
    PIN_SAY(AVR_SCK_PIN)
  #endif
  #if defined(AVR_SS_PIN) && AVR_SS_PIN > -1
    PIN_SAY(AVR_SS_PIN)
  #endif
  #if defined(BEEPER_PIN) && BEEPER_PIN > -1
    PIN_SAY(BEEPER_PIN)
  #endif
  #if defined(BTN_CENTER) && BTN_CENTER > -1
    PIN_SAY(BTN_CENTER)
  #endif
  #if defined(BTN_DOWN) && BTN_DOWN > -1
    PIN_SAY(BTN_DOWN)
  #endif
  #if defined(BTN_DWN) && BTN_DWN > -1
    PIN_SAY(BTN_DWN)
  #endif
  #if defined(BTN_EN1) && BTN_EN1 > -1
    PIN_SAY(BTN_EN1)
  #endif
  #if defined(BTN_EN2) && BTN_EN2 > -1
    PIN_SAY(BTN_EN2)
  #endif
  #if defined(BTN_ENC) && BTN_ENC > -1
    PIN_SAY(BTN_ENC)
  #endif
  #if defined(BTN_HOME) && BTN_HOME > -1
    PIN_SAY(BTN_HOME)
  #endif
  #if defined(BTN_LEFT) && BTN_LEFT > -1
    PIN_SAY(BTN_LEFT)
  #endif
  #if defined(BTN_LFT) && BTN_LFT > -1
    PIN_SAY(BTN_LFT)
  #endif
  #if defined(BTN_RIGHT) && BTN_RIGHT > -1
    PIN_SAY(BTN_RIGHT)
  #endif
  #if defined(BTN_RT) && BTN_RT > -1
    PIN_SAY(BTN_RT)
  #endif
  #if defined(BTN_UP) && BTN_UP > -1
    PIN_SAY(BTN_UP)
  #endif
  #if defined(CONTROLLERFAN_PIN) && CONTROLLERFAN_PIN > -1
    PIN_SAY(CONTROLLERFAN_PIN)
  #endif
  #if defined(DAC_DISABLE_PIN) && DAC_DISABLE_PIN > -1
    PIN_SAY(DAC_DISABLE_PIN)
  #endif
  #if defined(DAC_STEPPER_GAIN) && DAC_STEPPER_GAIN > -1
    PIN_SAY(DAC_STEPPER_GAIN)
  #endif
  #if defined(DAC_STEPPER_VREF) && DAC_STEPPER_VREF > -1
    PIN_SAY(DAC_STEPPER_VREF)
  #endif
  #if defined(DEBUG_PIN) && DEBUG_PIN > -1
    PIN_SAY(DEBUG_PIN)
  #endif
  #if defined(DIGIPOTSS_PIN) && DIGIPOTSS_PIN > -1
    PIN_SAY(DIGIPOTSS_PIN)
  #endif
  #if defined(DIO_COUNT) && DIO_COUNT > -1
    PIN_SAY(DIO_COUNT)
  #endif
  #if defined(DOGLCD_A0) && DOGLCD_A0 > -1
    PIN_SAY(DOGLCD_A0)
  #endif
  #if defined(DOGLCD_CS) && DOGLCD_CS > -1
    PIN_SAY(DOGLCD_CS)
  #endif
  #if defined(DOGLCD_MOSI) && DOGLCD_MOSI > -1
    PIN_SAY(DOGLCD_MOSI)
  #endif
  #if defined(DOGLCD_SCK) && DOGLCD_SCK > -1
    PIN_SAY(DOGLCD_SCK)
  #endif
  #if defined(E0_ATT_PIN) && E0_ATT_PIN > -1
    PIN_SAY(E0_ATT_PIN)
  #endif
  #if defined(E0_DIR_PIN) && E0_DIR_PIN > -1
    PIN_SAY(E0_DIR_PIN)
  #endif
  #if defined(E0_ENABLE_PIN) && E0_ENABLE_PIN > -1
    PIN_SAY(E0_ENABLE_PIN)
  #endif
  #if defined(E0_MS1_PIN) && E0_MS1_PIN > -1
    PIN_SAY(E0_MS1_PIN)
  #endif
  #if defined(E0_MS2_PIN) && E0_MS2_PIN > -1
    PIN_SAY(E0_MS2_PIN)
  #endif
  #if defined(E0_STEP_PIN) && E0_STEP_PIN > -1
    PIN_SAY(E0_STEP_PIN)
  #endif
  #if defined(E1_DIR_PIN) && E1_DIR_PIN > -1
    PIN_SAY(E1_DIR_PIN)
  #endif
  #if defined(E1_ENABLE_PIN) && E1_ENABLE_PIN > -1
    PIN_SAY(E1_ENABLE_PIN)
  #endif
  #if defined(E1_MS1_PIN) && E1_MS1_PIN > -1
    PIN_SAY(E1_MS1_PIN)
  #endif
  #if defined(E1_MS2_PIN) && E1_MS2_PIN > -1
    PIN_SAY(E1_MS2_PIN)
  #endif
  #if defined(E1_STEP_PIN) && E1_STEP_PIN > -1
    PIN_SAY(E1_STEP_PIN)
  #endif
  #if defined(E2_DIR_PIN) && E2_DIR_PIN > -1
    PIN_SAY(E2_DIR_PIN)
  #endif
  #if defined(E2_ENABLE_PIN) && E2_ENABLE_PIN > -1
    PIN_SAY(E2_ENABLE_PIN)
  #endif
  #if defined(E2_STEP_PIN) && E2_STEP_PIN > -1
    PIN_SAY(E2_STEP_PIN)
  #endif
  #if defined(E3_DIR_PIN) && E3_DIR_PIN > -1
    PIN_SAY(E3_DIR_PIN)
  #endif
  #if defined(E3_ENABLE_PIN) && E3_ENABLE_PIN > -1
    PIN_SAY(E3_ENABLE_PIN)
  #endif
  #if defined(E3_STEP_PIN) && E3_STEP_PIN > -1
    PIN_SAY(E3_STEP_PIN)
  #endif
  #if defined(E4_DIR_PIN) && E4_DIR_PIN > -1
    PIN_SAY(E4_DIR_PIN)
  #endif
  #if defined(E4_ENABLE_PIN) && E4_ENABLE_PIN > -1
    PIN_SAY(E4_ENABLE_PIN)
  #endif
  #if defined(E4_STEP_PIN) && E4_STEP_PIN > -1
    PIN_SAY(E4_STEP_PIN)
  #endif
  #if defined(encrot1) && encrot1 > -1
    PIN_SAY(encrot1)
  #endif
  #if defined(encrot2) && encrot2 > -1
    PIN_SAY(encrot2)
  #endif
  #if defined(encrot3) && encrot3 > -1
    PIN_SAY(encrot3)
  #endif
  #if defined(EXT_AUX_A0_IO) && EXT_AUX_A0_IO > -1
    PIN_SAY(EXT_AUX_A0_IO)
  #endif
  #if defined(EXT_AUX_A1) && EXT_AUX_A1 > -1
    PIN_SAY(EXT_AUX_A1)
  #endif
  #if defined(EXT_AUX_A1_IO) && EXT_AUX_A1_IO > -1
    PIN_SAY(EXT_AUX_A1_IO)
  #endif
  #if defined(EXT_AUX_A2) && EXT_AUX_A2 > -1
    PIN_SAY(EXT_AUX_A2)
  #endif
  #if defined(EXT_AUX_A2_IO) && EXT_AUX_A2_IO > -1
    PIN_SAY(EXT_AUX_A2_IO)
  #endif
  #if defined(EXT_AUX_A3) && EXT_AUX_A3 > -1
    PIN_SAY(EXT_AUX_A3)
  #endif
  #if defined(EXT_AUX_A3_IO) && EXT_AUX_A3_IO > -1
    PIN_SAY(EXT_AUX_A3_IO)
  #endif
  #if defined(EXT_AUX_A4) && EXT_AUX_A4 > -1
    PIN_SAY(EXT_AUX_A4)
  #endif
  #if defined(EXT_AUX_A4_IO) && EXT_AUX_A4_IO > -1
    PIN_SAY(EXT_AUX_A4_IO)
  #endif
  #if defined(EXT_AUX_PWM_D24) && EXT_AUX_PWM_D24 > -1
    PIN_SAY(EXT_AUX_PWM_D24)
  #endif
  #if defined(EXT_AUX_RX1_D2) && EXT_AUX_RX1_D2 > -1
    PIN_SAY(EXT_AUX_RX1_D2)
  #endif
  #if defined(EXT_AUX_SDA_D1) && EXT_AUX_SDA_D1 > -1
    PIN_SAY(EXT_AUX_SDA_D1)
  #endif
  #if defined(EXT_AUX_TX1_D3) && EXT_AUX_TX1_D3 > -1
    PIN_SAY(EXT_AUX_TX1_D3)
  #endif
  #if defined(EXTRUDER_0_AUTO_FAN_PIN) && EXTRUDER_0_AUTO_FAN_PIN > -1
    PIN_SAY(EXTRUDER_0_AUTO_FAN_PIN)
  #endif
  #if defined(EXTRUDER_1_AUTO_FAN_PIN) && EXTRUDER_1_AUTO_FAN_PIN > -1
    PIN_SAY(EXTRUDER_1_AUTO_FAN_PIN)
  #endif
  #if defined(EXTRUDER_2_AUTO_FAN_PIN) && EXTRUDER_2_AUTO_FAN_PIN > -1
    PIN_SAY(EXTRUDER_2_AUTO_FAN_PIN)
  #endif
  #if defined(EXTRUDER_3_AUTO_FAN_PIN) && EXTRUDER_3_AUTO_FAN_PIN > -1
    PIN_SAY(EXTRUDER_3_AUTO_FAN_PIN)
  #endif
  #if defined(FAN_PIN) && FAN_PIN > -1
    PIN_SAY(FAN_PIN)
  #endif
  #if defined(FAN0_PIN) && FAN0_PIN > -1
    PIN_SAY(FAN0_PIN)
  #endif
  #if defined(FAN1_PIN) && FAN1_PIN > -1
    PIN_SAY(FAN1_PIN)
  #endif
  #if defined(FAN2_PIN) && FAN2_PIN > -1
    PIN_SAY(FAN2_PIN)
  #endif
  #if defined(FIL_RUNOUT_PIN) && FIL_RUNOUT_PIN > -1
    PIN_SAY(FIL_RUNOUT_PIN)
  #endif
  #if defined(FILWIDTH_PIN) && FILWIDTH_PIN > -1
    ANALOG_PIN_SAY(FILWIDTH_PIN)
  #endif
  #if defined(GEN7_VERSION) && GEN7_VERSION > -1
    PIN_SAY(GEN7_VERSION)
  #endif
  #if defined(HEATER_0_PIN) && HEATER_0_PIN > -1
    PIN_SAY(HEATER_0_PIN)
  #endif
  #if defined(HEATER_1_PIN) && HEATER_1_PIN > -1
    PIN_SAY(HEATER_1_PIN)
  #endif
  #if defined(HEATER_2_PIN) && HEATER_2_PIN > -1
    PIN_SAY(HEATER_2_PIN)
  #endif
  #if defined(HEATER_3_PIN) && HEATER_3_PIN > -1
    PIN_SAY(HEATER_3_PIN)
  #endif
  #if defined(HEATER_4_PIN) && HEATER_4_PIN > -1
    PIN_SAY(HEATER_4_PIN)
  #endif
  #if defined(HEATER_5_PIN) && HEATER_5_PIN > -1
    PIN_SAY(HEATER_5_PIN)
  #endif
  #if defined(HEATER_6_PIN) && HEATER_6_PIN > -1
    PIN_SAY(HEATER_6_PIN)
  #endif
  #if defined(HEATER_7_PIN) && HEATER_7_PIN > -1
    PIN_SAY(HEATER_7_PIN)
  #endif
  #if defined(HEATER_BED_PIN) && HEATER_BED_PIN > -1
    PIN_SAY(HEATER_BED_PIN)
  #endif
  #if defined(I2C_SCL) && I2C_SCL > -1
    PIN_SAY(I2C_SCL)
  #endif
  #if defined(I2C_SDA) && I2C_SDA > -1
    PIN_SAY(I2C_SDA)
  #endif
  #if defined(KILL_PIN) && KILL_PIN > -1
    PIN_SAY(KILL_PIN)
  #endif
  #if defined(LCD_BACKLIGHT_PIN) && LCD_BACKLIGHT_PIN > -1
    PIN_SAY(LCD_BACKLIGHT_PIN)
  #endif
  #if defined(LCD_CONTRAST) && LCD_CONTRAST > -1
    PIN_SAY(LCD_CONTRAST)
  #endif
  #if defined(LCD_PINS_D4) && LCD_PINS_D4 > -1
    PIN_SAY(LCD_PINS_D4)
  #endif
  #if defined(LCD_PINS_D5) && LCD_PINS_D5 > -1
    PIN_SAY(LCD_PINS_D5)
  #endif
  #if defined(LCD_PINS_D6) && LCD_PINS_D6 > -1
    PIN_SAY(LCD_PINS_D6)
  #endif
  #if defined(LCD_PINS_D7) && LCD_PINS_D7 > -1
    PIN_SAY(LCD_PINS_D7)
  #endif
  #if defined(LCD_PINS_ENABLE) && LCD_PINS_ENABLE > -1
    PIN_SAY(LCD_PINS_ENABLE)
  #endif
  #if defined(LCD_PINS_RS) && LCD_PINS_RS > -1
    PIN_SAY(LCD_PINS_RS)
  #endif
  #if defined(LCD_SDSS) && LCD_SDSS > -1
    PIN_SAY(LCD_SDSS)
  #endif
  #if defined(LED_PIN) && LED_PIN > -1
    PIN_SAY(LED_PIN)
  #endif
  #if defined(MAIN_VOLTAGE_MEASURE_PIN) && MAIN_VOLTAGE_MEASURE_PIN > -1
    PIN_SAY(MAIN_VOLTAGE_MEASURE_PIN)
  #endif
  #if defined(MAX6675_SS) && MAX6675_SS > -1
    PIN_SAY(MAX6675_SS)
  #endif
  #if defined(MISO_PIN) && MISO_PIN > -1
    PIN_SAY(MISO_PIN)
  #endif
  #if defined(MOSFET_D_PIN) && MOSFET_D_PIN > -1
    PIN_SAY(MOSFET_D_PIN)
  #endif
  #if defined(MOSI_PIN) && MOSI_PIN > -1
    PIN_SAY(MOSI_PIN)
  #endif
  #if defined(MOTOR_CURRENT_PWM_E_PIN) && MOTOR_CURRENT_PWM_E_PIN > -1
    PIN_SAY(MOTOR_CURRENT_PWM_E_PIN)
  #endif
  #if defined(MOTOR_CURRENT_PWM_XY_PIN) && MOTOR_CURRENT_PWM_XY_PIN > -1
    PIN_SAY(MOTOR_CURRENT_PWM_XY_PIN)
  #endif
  #if defined(MOTOR_CURRENT_PWM_Z_PIN) && MOTOR_CURRENT_PWM_Z_PIN > -1
    PIN_SAY(MOTOR_CURRENT_PWM_Z_PIN)
  #endif
  #if defined(NUM_TLCS) && NUM_TLCS > -1
    PIN_SAY(NUM_TLCS)
  #endif
  #if defined(PHOTOGRAPH_PIN) && PHOTOGRAPH_PIN > -1
    PIN_SAY(PHOTOGRAPH_PIN)
  #endif
  #if defined(PS_ON_PIN) && PS_ON_PIN > -1
    PIN_SAY(PS_ON_PIN)
  #endif
  #if defined(RAMPS_D10_PIN) && RAMPS_D10_PIN > -1
    PIN_SAY(RAMPS_D10_PIN)
  #endif
  #if defined(RAMPS_D8_PIN) && RAMPS_D8_PIN > -1
    PIN_SAY(RAMPS_D8_PIN)
  #endif
  #if defined(RAMPS_D9_PIN) && RAMPS_D9_PIN > -1
    PIN_SAY(RAMPS_D9_PIN)
  #endif
  #if defined(RX_ENABLE_PIN) && RX_ENABLE_PIN > -1
    PIN_SAY(RX_ENABLE_PIN)
  #endif
  #if defined(SAFETY_TRIGGERED_PIN) && SAFETY_TRIGGERED_PIN > -1
    PIN_SAY(SAFETY_TRIGGERED_PIN)
  #endif
  #if defined(SCK_PIN) && SCK_PIN > -1
    PIN_SAY(SCK_PIN)
  #endif
  #if defined(SCL) && SCL > -1
    PIN_SAY(SCL)
  #endif
  #if defined(SD_DETECT_PIN) && SD_DETECT_PIN > -1
    PIN_SAY(SD_DETECT_PIN)
  #endif
  #if defined(SDA) && SDA > -1
    PIN_SAY(SDA)
  #endif
  #if defined(SDPOWER) && SDPOWER > -1
    PIN_SAY(SDPOWER)
  #endif
  #if defined(SDSS) && SDSS > -1
    PIN_SAY(SDSS)
  #endif
  #if defined(SERVO0_PIN) && SERVO0_PIN > -1
    PIN_SAY(SERVO0_PIN)
  #endif
  #if defined(SERVO1_PIN) && SERVO1_PIN > -1
    PIN_SAY(SERVO1_PIN)
  #endif
  #if defined(SERVO2_PIN) && SERVO2_PIN > -1
    PIN_SAY(SERVO2_PIN)
  #endif
  #if defined(SERVO3_PIN) && SERVO3_PIN > -1
    PIN_SAY(SERVO3_PIN)
  #endif
  #if defined(SHIFT_CLK) && SHIFT_CLK > -1
    PIN_SAY(SHIFT_CLK)
  #endif
  #if defined(SHIFT_EN) && SHIFT_EN > -1
    PIN_SAY(SHIFT_EN)
  #endif
  #if defined(SHIFT_LD) && SHIFT_LD > -1
    PIN_SAY(SHIFT_LD)
  #endif
  #if defined(SHIFT_OUT) && SHIFT_OUT > -1
    PIN_SAY(SHIFT_OUT)
  #endif
  #if defined(SLED_PIN) && SLED_PIN > -1
    PIN_SAY(SLED_PIN)
  #endif
  #if defined(SLEEP_WAKE_PIN) && SLEEP_WAKE_PIN > -1
    PIN_SAY(SLEEP_WAKE_PIN)
  #endif
  #if defined(SOL1_PIN) && SOL1_PIN > -1
    PIN_SAY(SOL1_PIN)
  #endif
  #if defined(SOL2_PIN) && SOL2_PIN > -1
    PIN_SAY(SOL2_PIN)
  #endif
  #if defined(SPINDLE_ENABLE_PIN) && SPINDLE_ENABLE_PIN > -1
    PIN_SAY(SPINDLE_ENABLE_PIN)
  #endif
  #if defined(SPINDLE_SPEED_PIN) && SPINDLE_SPEED_PIN > -1
    PIN_SAY(SPINDLE_SPEED_PIN)
  #endif
  #if defined(SS_PIN) && SS_PIN > -1
    PIN_SAY(SS_PIN)
  #endif
  #if defined(STAT_LED_BLUE_PIN) && STAT_LED_BLUE_PIN > -1
    PIN_SAY(STAT_LED_BLUE_PIN)
  #endif
  #if defined(STAT_LED_RED_PIN) && STAT_LED_RED_PIN > -1
    PIN_SAY(STAT_LED_RED_PIN)
  #endif
  #if defined(STEPPER_RESET_PIN) && STEPPER_RESET_PIN > -1
    PIN_SAY(STEPPER_RESET_PIN)
  #endif
  #if defined(SUICIDE_PIN) && SUICIDE_PIN > -1
    PIN_SAY(SUICIDE_PIN)
  #endif
  #if defined(TC1) && TC1 > -1
    ANALOG_PIN_SAY(TC1)
  #endif
  #if defined(TC2) && TC2 > -1
    ANALOG_PIN_SAY(TC2)
  #endif
  #if defined(TEMP_0_PIN) && TEMP_0_PIN > -1
    ANALOG_PIN_SAY(TEMP_0_PIN)
  #endif
  #if defined(TEMP_1_PIN) && TEMP_1_PIN > -1
    ANALOG_PIN_SAY(TEMP_1_PIN)
  #endif
  #if defined(TEMP_2_PIN) && TEMP_2_PIN > -1
    ANALOG_PIN_SAY(TEMP_2_PIN)
  #endif
  #if defined(TEMP_3_PIN) && TEMP_3_PIN > -1
    ANALOG_PIN_SAY(TEMP_3_PIN)
  #endif
  #if defined(TEMP_4_PIN) && TEMP_4_PIN > -1
    ANALOG_PIN_SAY(TEMP_4_PIN)
  #endif
  #if defined(TEMP_BED_PIN) && TEMP_BED_PIN > -1
    ANALOG_PIN_SAY(TEMP_BED_PIN)
  #endif
  #if defined(TEMP_X_PIN) && TEMP_X_PIN > -1
    ANALOG_PIN_SAY(TEMP_X_PIN)
  #endif
  #if defined(TLC_BLANK_BIT) && TLC_BLANK_BIT > -1
    PIN_SAY(TLC_BLANK_BIT)
  #endif
  #if defined(TLC_BLANK_PIN) && TLC_BLANK_PIN > -1
    PIN_SAY(TLC_BLANK_PIN)
  #endif
  #if defined(TLC_CLOCK_BIT) && TLC_CLOCK_BIT > -1
    PIN_SAY(TLC_CLOCK_BIT)
  #endif
  #if defined(TLC_CLOCK_PIN) && TLC_CLOCK_PIN > -1
    PIN_SAY(TLC_CLOCK_PIN)
  #endif
  #if defined(TLC_DATA_BIT) && TLC_DATA_BIT > -1
    PIN_SAY(TLC_DATA_BIT)
  #endif
  #if defined(TLC_DATA_PIN) && TLC_DATA_PIN > -1
    PIN_SAY(TLC_DATA_PIN)
  #endif
  #if defined(TLC_XLAT_PIN) && TLC_XLAT_PIN > -1
    PIN_SAY(TLC_XLAT_PIN)
  #endif
  #if defined(TX_ENABLE_PIN) && TX_ENABLE_PIN > -1
    PIN_SAY(TX_ENABLE_PIN)
  #endif
  #if defined(UNUSED_PWM) && UNUSED_PWM > -1
    PIN_SAY(UNUSED_PWM)
  #endif
  #if defined(X_ATT_PIN) && X_ATT_PIN > -1
    PIN_SAY(X_ATT_PIN)
  #endif
  #if defined(X_DIR_PIN) && X_DIR_PIN > -1
    PIN_SAY(X_DIR_PIN)
  #endif
  #if defined(X_ENABLE_PIN) && X_ENABLE_PIN > -1
    PIN_SAY(X_ENABLE_PIN)
  #endif
  #if defined(X_MAX_PIN) && X_MAX_PIN > -1
    PIN_SAY(X_MAX_PIN)
  #endif
  #if defined(X_MIN_PIN) && X_MIN_PIN > -1
    PIN_SAY(X_MIN_PIN)
  #endif
  #if defined(X_MS1_PIN) && X_MS1_PIN > -1
    PIN_SAY(X_MS1_PIN)
  #endif
  #if defined(X_MS2_PIN) && X_MS2_PIN > -1
    PIN_SAY(X_MS2_PIN)
  #endif
  #if defined(X_STEP_PIN) && X_STEP_PIN > -1
    PIN_SAY(X_STEP_PIN)
  #endif
  #if defined(X_STOP_PIN) && X_STOP_PIN > -1
    PIN_SAY(X_STOP_PIN)
  #endif
  #if defined(X2_DIR_PIN) && X2_DIR_PIN > -1
    PIN_SAY(X2_DIR_PIN)
  #endif
  #if defined(X2_ENABLE_PIN) && X2_ENABLE_PIN > -1
    PIN_SAY(X2_ENABLE_PIN)
  #endif
  #if defined(X2_STEP_PIN) && X2_STEP_PIN > -1
    PIN_SAY(X2_STEP_PIN)
  #endif
  #if defined(Y_ATT_PIN) && Y_ATT_PIN > -1
    PIN_SAY(Y_ATT_PIN)
  #endif
  #if defined(Y_DIR_PIN) && Y_DIR_PIN > -1
    PIN_SAY(Y_DIR_PIN)
  #endif
  #if defined(Y_ENABLE_PIN) && Y_ENABLE_PIN > -1
    PIN_SAY(Y_ENABLE_PIN)
  #endif
  #if defined(Y_MAX_PIN) && Y_MAX_PIN > -1
    PIN_SAY(Y_MAX_PIN)
  #endif
  #if defined(Y_MIN_PIN) && Y_MIN_PIN > -1
    PIN_SAY(Y_MIN_PIN)
  #endif
  #if defined(Y_MS1_PIN) && Y_MS1_PIN > -1
    PIN_SAY(Y_MS1_PIN)
  #endif
  #if defined(Y_MS2_PIN) && Y_MS2_PIN > -1
    PIN_SAY(Y_MS2_PIN)
  #endif
  #if defined(Y_STEP_PIN) && Y_STEP_PIN > -1
    PIN_SAY(Y_STEP_PIN)
  #endif
  #if defined(Y_STOP_PIN) && Y_STOP_PIN > -1
    PIN_SAY(Y_STOP_PIN)
  #endif
  #if defined(Y2_DIR_PIN) && Y2_DIR_PIN > -1
    PIN_SAY(Y2_DIR_PIN)
  #endif
  #if defined(Y2_ENABLE_PIN) && Y2_ENABLE_PIN > -1
    PIN_SAY(Y2_ENABLE_PIN)
  #endif
  #if defined(Y2_STEP_PIN) && Y2_STEP_PIN > -1
    PIN_SAY(Y2_STEP_PIN)
  #endif
  #if defined(Z_ATT_PIN) && Z_ATT_PIN > -1
    PIN_SAY(Z_ATT_PIN)
  #endif
  #if defined(Z_DIR_PIN) && Z_DIR_PIN > -1
    PIN_SAY(Z_DIR_PIN)
  #endif
  #if defined(Z_ENABLE_PIN) && Z_ENABLE_PIN > -1
    PIN_SAY(Z_ENABLE_PIN)
  #endif
  #if defined(Z_MAX_PIN) && Z_MAX_PIN > -1
    PIN_SAY(Z_MAX_PIN)
  #endif
  #if defined(Z_MIN_PIN) && Z_MIN_PIN > -1
    PIN_SAY(Z_MIN_PIN)
  #endif
  #if defined(Z_MIN_PROBE_PIN) && Z_MIN_PROBE_PIN > -1
    PIN_SAY(Z_MIN_PROBE_PIN)
  #endif
  #if defined(Z_MS1_PIN) && Z_MS1_PIN > -1
    PIN_SAY(Z_MS1_PIN)
  #endif
  #if defined(Z_MS2_PIN) && Z_MS2_PIN > -1
    PIN_SAY(Z_MS2_PIN)
  #endif
  #if defined(Z_STEP_PIN) && Z_STEP_PIN > -1
    PIN_SAY(Z_STEP_PIN)
  #endif
  #if defined(Z_STOP_PIN) && Z_STOP_PIN > -1
    PIN_SAY(Z_STOP_PIN)
  #endif
  #if defined(Z2_DIR_PIN) && Z2_DIR_PIN > -1
    PIN_SAY(Z2_DIR_PIN)
  #endif
  #if defined(Z2_ENABLE_PIN) && Z2_ENABLE_PIN > -1
    PIN_SAY(Z2_ENABLE_PIN)
  #endif
  #if defined(Z2_STEP_PIN) && Z2_STEP_PIN > -1
    PIN_SAY(Z2_STEP_PIN)
  #endif


  SERIAL_ECHOPGM("<unused>");
  return false;
}

inline void report_pin_state(int8_t pin) {
  if (report_pin_name(pin)) {
    if (pin_is_protected(pin))
      SERIAL_ECHOPGM(" (protected)");
    else {
      SERIAL_ECHOPGM(" = ");
      pinMode(pin, INPUT_PULLUP);
      SERIAL_ECHO(digitalRead(pin));
      if (IS_ANALOG(pin)) {
        SERIAL_CHAR(' '); SERIAL_CHAR('(');
        SERIAL_ECHO(analogRead(pin - analogInputToDigitalPin(0)));
        SERIAL_CHAR(')');
      }
    }
  }
  SERIAL_EOL;
}
