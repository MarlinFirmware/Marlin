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

  #if PIN_EXISTS(SERVO0)
    PIN_SAY(SERVO0_PIN);
  #endif
  #if PIN_EXISTS(SERVO1)
    PIN_SAY(SERVO1_PIN);
  #endif
  #if PIN_EXISTS(SERVO2)
    PIN_SAY(SERVO2_PIN);
  #endif
  #if PIN_EXISTS(SERVO3)
    PIN_SAY(SERVO3_PIN);
  #endif

  #if PIN_EXISTS(X_MIN)
    PIN_SAY(X_MIN_PIN);
  #endif
  #if PIN_EXISTS(X_MAX)
    PIN_SAY(X_MAX_PIN);
  #endif
  #if PIN_EXISTS(Y_MIN)
    PIN_SAY(Y_MIN_PIN);
  #endif
  #if PIN_EXISTS(Y_MAX)
    PIN_SAY(Y_MAX_PIN);
  #endif
  #if PIN_EXISTS(Z_MIN)
    PIN_SAY(Z_MIN_PIN);
  #endif
  #if PIN_EXISTS(Z_MAX)
    PIN_SAY(Z_MAX_PIN);
  #endif
  #if PIN_EXISTS(Z_MIN_PROBE)
    PIN_SAY(Z_MIN_PROBE_PIN);
  #endif
  #if PIN_EXISTS(X_STEP)
    PIN_SAY(X_STEP_PIN);
  #endif
  #if PIN_EXISTS(X_DIR)
    PIN_SAY(X_DIR_PIN);
  #endif
  #if PIN_EXISTS(X_ENABLE)
    PIN_SAY(X_ENABLE_PIN);
  #endif
  #if PIN_EXISTS(X_MS1)
    PIN_SAY(X_MS1_PIN);
  #endif
  #if PIN_EXISTS(X_MS2)
    PIN_SAY(X_MS2_PIN);
  #endif
  #if PIN_EXISTS(X2_STEP)
    PIN_SAY(X2_STEP_PIN);
  #endif
  #if PIN_EXISTS(X2_DIR)
    PIN_SAY(X2_DIR_PIN);
  #endif
  #if PIN_EXISTS(X2_ENABLE)
    PIN_SAY(X2_ENABLE_PIN);
  #endif
  #if PIN_EXISTS(Y_STEP)
    PIN_SAY(Y_STEP_PIN);
  #endif
  #if PIN_EXISTS(Y_DIR)
    PIN_SAY(Y_DIR_PIN);
  #endif
  #if PIN_EXISTS(Y_ENABLE)
    PIN_SAY(Y_ENABLE_PIN);
  #endif
  #if PIN_EXISTS(Y_MS1)
    PIN_SAY(Y_MS1_PIN);
  #endif
  #if PIN_EXISTS(Y_MS2)
    PIN_SAY(Y_MS2_PIN);
  #endif
  #if PIN_EXISTS(Y2_STEP)
    PIN_SAY(Y2_STEP_PIN);
  #endif
  #if PIN_EXISTS(Y2_DIR)
    PIN_SAY(Y2_DIR_PIN);
  #endif
  #if PIN_EXISTS(Y2_ENABLE)
    PIN_SAY(Y2_ENABLE_PIN);
  #endif
  #if PIN_EXISTS(Z_STEP)
    PIN_SAY(Z_STEP_PIN);
  #endif
  #if PIN_EXISTS(Z_DIR)
    PIN_SAY(Z_DIR_PIN);
  #endif
  #if PIN_EXISTS(Z_ENABLE)
    PIN_SAY(Z_ENABLE_PIN);
  #endif
  #if PIN_EXISTS(Z_MS1)
    PIN_SAY(Z_MS1_PIN);
  #endif
  #if PIN_EXISTS(Z_MS2)
    PIN_SAY(Z_MS2_PIN);
  #endif
  #if PIN_EXISTS(Z2_STEP)
    PIN_SAY(Z2_STEP_PIN);
  #endif
  #if PIN_EXISTS(Z2_DIR)
    PIN_SAY(Z2_DIR_PIN);
  #endif
  #if PIN_EXISTS(Z2_ENABLE)
    PIN_SAY(Z2_ENABLE_PIN);
  #endif

  #if PIN_EXISTS(E0_STEP)
    PIN_SAY(E0_STEP_PIN);
  #endif
  #if PIN_EXISTS(E0_DIR)
    PIN_SAY(E0_DIR_PIN);
  #endif
  #if PIN_EXISTS(E0_ENABLE)
    PIN_SAY(E0_ENABLE_PIN);
  #endif
  #if PIN_EXISTS(E0_MS1)
    PIN_SAY(E0_MS1_PIN);
  #endif
  #if PIN_EXISTS(E0_MS2)
    PIN_SAY(E0_MS2_PIN);
  #endif
  #if PIN_EXISTS(E1_STEP)
    PIN_SAY(E1_STEP_PIN);
  #endif
  #if PIN_EXISTS(E1_DIR)
    PIN_SAY(E1_DIR_PIN);
  #endif
  #if PIN_EXISTS(E1_ENABLE)
    PIN_SAY(E1_ENABLE_PIN);
  #endif
  #if PIN_EXISTS(E1_MS1)
    PIN_SAY(E1_MS1_PIN);
  #endif
  #if PIN_EXISTS(E1_MS2)
    PIN_SAY(E1_MS2_PIN);
  #endif
  #if PIN_EXISTS(E2_STEP)
    PIN_SAY(E2_STEP_PIN);
  #endif
  #if PIN_EXISTS(E2_DIR)
    PIN_SAY(E2_DIR_PIN);
  #endif
  #if PIN_EXISTS(E2_ENABLE)
    PIN_SAY(E2_ENABLE_PIN);
  #endif
  #if PIN_EXISTS(E3_STEP)
    PIN_SAY(E3_STEP_PIN);
  #endif
  #if PIN_EXISTS(E3_DIR)
    PIN_SAY(E3_DIR_PIN);
  #endif
  #if PIN_EXISTS(E3_ENABLE)
    PIN_SAY(E3_ENABLE_PIN);
  #endif
  #if PIN_EXISTS(E4_STEP)
    PIN_SAY(E4_STEP_PIN);
  #endif
  #if PIN_EXISTS(E4_DIR)
    PIN_SAY(E4_DIR_PIN);
  #endif
  #if PIN_EXISTS(E4_ENABLE)
    PIN_SAY(E4_ENABLE_PIN);
  #endif

  #if PIN_EXISTS(FAN)
    PIN_SAY(FAN_PIN);
  #endif
  #if PIN_EXISTS(FAN1)
    PIN_SAY(FAN1_PIN);
  #endif
  #if PIN_EXISTS(FAN2)
    PIN_SAY(FAN2_PIN);
  #endif
  #if PIN_EXISTS(CONTROLLERFAN)
    PIN_SAY(CONTROLLERFAN_PIN);
  #endif
  #if PIN_EXISTS(EXTRUDER_0_AUTO_FAN)
    PIN_SAY(EXTRUDER_0_AUTO_FAN_PIN);
  #endif
  #if PIN_EXISTS(EXTRUDER_1_AUTO_FAN)
    PIN_SAY(EXTRUDER_1_AUTO_FAN_PIN);
  #endif
  #if PIN_EXISTS(EXTRUDER_2_AUTO_FAN)
    PIN_SAY(EXTRUDER_2_AUTO_FAN_PIN);
  #endif
  #if PIN_EXISTS(EXTRUDER_3_AUTO_FAN)
    PIN_SAY(EXTRUDER_3_AUTO_FAN_PIN);
  #endif
  #if PIN_EXISTS(HEATER_0)
    PIN_SAY(HEATER_0_PIN);
  #endif
  #if PIN_EXISTS(HEATER_1)
    PIN_SAY(HEATER_1_PIN);
  #endif
  #if PIN_EXISTS(HEATER_2)
    PIN_SAY(HEATER_2_PIN);
  #endif
  #if PIN_EXISTS(HEATER_3)
    PIN_SAY(HEATER_3_PIN);
  #endif
  #if PIN_EXISTS(HEATER_BED)
    PIN_SAY(HEATER_BED_PIN);
  #endif

  #if PIN_EXISTS(X_ATT)
    PIN_SAY(X_ATT_PIN);
  #endif
  #if PIN_EXISTS(Y_ATT)
    PIN_SAY(Y_ATT_PIN);
  #endif
  #if PIN_EXISTS(Z_ATT)
    PIN_SAY(Z_ATT_PIN);
  #endif
  #if PIN_EXISTS(E0_ATT)
    PIN_SAY(E0_ATT_PIN);
  #endif

  #if PIN_EXISTS(TEMP_0)
    ANALOG_PIN_SAY(TEMP_0_PIN);
  #endif
  #if PIN_EXISTS(TEMP_1)
    ANALOG_PIN_SAY(TEMP_1_PIN);
  #endif
  #if PIN_EXISTS(TEMP_2)
    ANALOG_PIN_SAY(TEMP_2_PIN);
  #endif
  #if PIN_EXISTS(TEMP_3)
    ANALOG_PIN_SAY(TEMP_3_PIN);
  #endif
  #if PIN_EXISTS(TEMP_BED)
    ANALOG_PIN_SAY(TEMP_BED_PIN);
  #endif
  #if PIN_EXISTS(FILWIDTH)
    ANALOG_PIN_SAY(FILWIDTH_PIN);
  #endif

  #if PIN_EXISTS(BEEPER)
    PIN_SAY(BEEPER_PIN);
  #endif
  #if PIN_EXISTS(SLED)
    PIN_SAY(SLED_PIN);
  #endif
  #if PIN_EXISTS(FIL_RUNOUT)
    PIN_SAY(FIL_RUNOUT_PIN);
  #endif

  #if PIN_EXISTS(LED)
    PIN_SAY(LED_PIN);
  #endif
  // #if defined(DEBUG_LED) && DEBUG_LED > -1
  //   PIN_SAY(DEBUG_LED);
  // #endif
  #if PIN_EXISTS(STAT_LED_RED)
    PIN_SAY(STAT_LED_RED_PIN);
  #endif
  #if PIN_EXISTS(STAT_LED_BLUE)
    PIN_SAY(STAT_LED_BLUE_PIN);
  #endif

  #if PIN_EXISTS(DIGIPOTSS)
    PIN_SAY(DIGIPOTSS_PIN);
  #endif

  #if PIN_EXISTS(SCK)
    PIN_SAY(SCK_PIN);
  #endif
  #if PIN_EXISTS(MISO)
    PIN_SAY(MISO_PIN);
  #endif
  #if PIN_EXISTS(MOSI)
    PIN_SAY(MOSI_PIN);
  #endif
  #if PIN_EXISTS(SS)
    PIN_SAY(SS_PIN);
  #endif

  #if PIN_EXISTS(SD_DETECT)
    PIN_SAY(SD_DETECT_PIN);
  #endif

  #if defined(SDPOWER) && SDPOWER > -1
    PIN_SAY(SDPOWER);
  #endif
  #if defined(SDSS) && SDSS > -1
    PIN_SAY(SDSS);
  #endif
  #if defined(I2C_SCL) && I2C_SCL > -1
    PIN_SAY(I2C_SCL);
  #endif
  #if defined(I2C_SDA) && I2C_SDA > -1
    PIN_SAY(I2C_SDA);
  #endif
  #if defined(SCL) && SCL > -1
    PIN_SAY(SCL);
  #endif
  #if defined(SDA) && SDA > -1
    PIN_SAY(SDA);
  #endif

  #if PIN_EXISTS(PS_ON)
    PIN_SAY(PS_ON_PIN);
  #endif
  #if PIN_EXISTS(KILL)
    PIN_SAY(KILL_PIN);
  #endif
  #if PIN_EXISTS(SUICIDE)
    PIN_SAY(SUICIDE_PIN);
  #endif
  #if PIN_EXISTS(DEBUG)
    PIN_SAY(DEBUG_PIN);
  #endif
  #if PIN_EXISTS(PHOTOGRAPH)
    PIN_SAY(PHOTOGRAPH_PIN);
  #endif

  #if PIN_EXISTS(BEEPER)
    PIN_SAY(BEEPER_PIN);
  #endif
  #if defined(BTN_EN1) && BTN_EN1 > -1
    PIN_SAY(BTN_EN1);
  #endif
  #if defined(BTN_EN2) && BTN_EN2 > -1
    PIN_SAY(BTN_EN2);
  #endif
  #if defined(BTN_ENC) && BTN_ENC > -1
    PIN_SAY(BTN_ENC);
  #endif
  #if defined(LCD_PINS_RS) && LCD_PINS_RS > -1
    PIN_SAY(LCD_PINS_RS);
  #endif
  #if defined(LCD_PINS_ENABLE) && LCD_PINS_ENABLE > -1
    PIN_SAY(LCD_PINS_ENABLE);
  #endif
  #if defined(LCD_PINS_D4) && LCD_PINS_D4 > -1
    PIN_SAY(LCD_PINS_D4);
  #endif
  #if defined(LCD_PINS_D5) && LCD_PINS_D5 > -1
    PIN_SAY(LCD_PINS_D5);
  #endif
  #if defined(LCD_PINS_D6) && LCD_PINS_D6 > -1
    PIN_SAY(LCD_PINS_D6);
  #endif
  #if defined(LCD_PINS_D7) && LCD_PINS_D7 > -1
    PIN_SAY(LCD_PINS_D7);
  #endif

  #if PIN_EXISTS(RAMPS_D8)
    PIN_SAY(RAMPS_D8_PIN);
  #endif
  #if PIN_EXISTS(RAMPS_D9)
    PIN_SAY(RAMPS_D9_PIN);
  #endif
  #if PIN_EXISTS(RAMPS_D10)
    PIN_SAY(RAMPS_D10_PIN);
  #endif
  #if PIN_EXISTS(MOSFET_D)
    PIN_SAY(MOSFET_D_PIN);
  #endif

  #if PIN_EXISTS(TX_ENABLE)
    PIN_SAY(TX_ENABLE_PIN);
  #endif
  #if PIN_EXISTS(RX_ENABLE)
    PIN_SAY(RX_ENABLE_PIN);
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
