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
//  #define DIO_COUNT 86
  #define DIO_COUNT 70  // digitalRead and other Arduino IDE routines only know about pins 0 through 69
#elif defined(DIO53_PIN)
  #define DIO_COUNT 54
#elif defined(DIO47_PIN)
  #define DIO_COUNT 48
#elif defined(DIO31_PIN)
  #define DIO_COUNT 32
#elif defined(DIO21_PIN)
  #define DIO_COUNT 22
#endif

#define  NAME_FORMAT "%-28s"

#define _PIN_SAY(NAME) { sprintf(buffer, NAME_FORMAT, NAME); SERIAL_ECHO(buffer); return true; }
#define PIN_SAY(NAME) if (pin == NAME) _PIN_SAY(#NAME);

#define _ANALOG_PIN_SAY(NAME)   { sprintf(buffer, NAME_FORMAT, NAME); SERIAL_ECHO(buffer); pin_is_analog = true; return true; }
#define ANALOG_PIN_SAY(NAME) if (pin == analogInputToDigitalPin(NAME)) _ANALOG_PIN_SAY(#NAME);

#define IS_ANALOG(P) ((P) >= analogInputToDigitalPin(0) && ((P) <= analogInputToDigitalPin(15) || (P) <= analogInputToDigitalPin(5)))



int digitalRead_mod(int8_t pin)  // same as digitalRead except the PWM stop section has been removed
{
	uint8_t bit = digitalPinToBitMask(pin);
	uint8_t port = digitalPinToPort(pin);
  
	if (port == NOT_A_PIN) return LOW;

	if (*portInputRegister(port) & bit) return HIGH;
	return LOW;
}

bool get_pinMode(int8_t pin)
{
	uint8_t bit = digitalPinToBitMask(pin);
	uint8_t port = digitalPinToPort(pin);
	volatile uint8_t *reg;
	reg = portModeRegister(port);
  return   *reg & bit;
}

// Report pin name for a given fastio digital pin index

static bool report_pin_name(int8_t pin,bool &pin_is_analog) {
  
  char buffer[30];   // for the sprintf statements
  pin_is_analog = false;   // default to digital pin
  
  if (IS_ANALOG(pin)) {
    sprintf(buffer, "(A%2d)  ", int(pin - analogInputToDigitalPin(0)));
    SERIAL_ECHO(buffer);   
  }
  else SERIAL_ECHOPGM("       ");

  #if defined(RXD) && RXD > -1
    if (pin == 0) { sprintf(buffer, NAME_FORMAT, "RXD"); SERIAL_ECHO(buffer); return true; }
  #endif
  #if defined(TXD) && TXD > -1
    if (pin == 1) { sprintf(buffer, NAME_FORMAT, "TXD"); SERIAL_ECHO(buffer); return true; }
  #endif

  
  // Pin list updated from 7 OCT RCBugfix branch
  #if defined(__FD) && __FD > -1   
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
 
  #if PIN_EXISTS(AVR_MISO)
    PIN_SAY(AVR_MISO_PIN);
  #endif
  #if PIN_EXISTS(AVR_MOSI)
    PIN_SAY(AVR_MOSI_PIN);
  #endif
  #if PIN_EXISTS(AVR_SCK)
    PIN_SAY(AVR_SCK_PIN);
  #endif
  #if PIN_EXISTS(AVR_SS)
    PIN_SAY(AVR_SS_PIN);
  #endif
  #if PIN_EXISTS(BEEPER)
    PIN_SAY(BEEPER_PIN);
  #endif
  #if defined(BTN_CENTER) && BTN_CENTER > -1
    PIN_SAY(BTN_CENTER);
  #endif
  #if defined(BTN_DOWN) && BTN_DOWN > -1
    PIN_SAY(BTN_DOWN);
  #endif
  #if defined(BTN_DWN) && BTN_DWN > -1
    PIN_SAY(BTN_DWN);
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
  #if defined(BTN_HOME) && BTN_HOME > -1
    PIN_SAY(BTN_HOME);
  #endif
  #if defined(BTN_LEFT) && BTN_LEFT > -1
    PIN_SAY(BTN_LEFT);
  #endif
  #if defined(BTN_LFT) && BTN_LFT > -1
    PIN_SAY(BTN_LFT);
  #endif
  #if defined(BTN_RIGHT) && BTN_RIGHT > -1
    PIN_SAY(BTN_RIGHT);
  #endif
  #if defined(BTN_RT) && BTN_RT > -1
    PIN_SAY(BTN_RT);
  #endif
  #if defined(BTN_UP) && BTN_UP > -1
    PIN_SAY(BTN_UP);
  #endif
  #if PIN_EXISTS(CONTROLLERFAN)
    PIN_SAY(CONTROLLERFAN_PIN);
  #endif
  #if PIN_EXISTS(DAC_DISABLE)
    PIN_SAY(DAC_DISABLE_PIN);
  #endif
  #if defined(DAC_STEPPER_GAIN) && DAC_STEPPER_GAIN > -1
    PIN_SAY(DAC_STEPPER_GAIN);
  #endif
  #if defined(DAC_STEPPER_VREF) && DAC_STEPPER_VREF > -1
    PIN_SAY(DAC_STEPPER_VREF);
  #endif
  #if PIN_EXISTS(DEBUG)
    PIN_SAY(DEBUG_PIN);
  #endif
  #if PIN_EXISTS(DIGIPOTSS)
    PIN_SAY(DIGIPOTSS_PIN);
  #endif
  #if defined(DIO_COUNT) && DIO_COUNT > -1
    PIN_SAY(DIO_COUNT);
  #endif
  #if defined(DOGLCD_A0) && DOGLCD_A0 > -1
    PIN_SAY(DOGLCD_A0);
  #endif
  #if defined(DOGLCD_CS) && DOGLCD_CS > -1
    PIN_SAY(DOGLCD_CS);
  #endif
  #if defined(DOGLCD_MOSI) && DOGLCD_MOSI > -1
    PIN_SAY(DOGLCD_MOSI);
  #endif
  #if defined(DOGLCD_SCK) && DOGLCD_SCK > -1
    PIN_SAY(DOGLCD_SCK);
  #endif
  #if PIN_EXISTS(E0_ATT)
    PIN_SAY(E0_ATT_PIN);
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
  #if PIN_EXISTS(E0_STEP)
    PIN_SAY(E0_STEP_PIN);
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
  #if PIN_EXISTS(E1_STEP)
    PIN_SAY(E1_STEP_PIN);
  #endif
  #if PIN_EXISTS(E2_DIR)
    PIN_SAY(E2_DIR_PIN);
  #endif
  #if PIN_EXISTS(E2_ENABLE)
    PIN_SAY(E2_ENABLE_PIN);
  #endif
  #if PIN_EXISTS(E2_STEP)
    PIN_SAY(E2_STEP_PIN);
  #endif
  #if PIN_EXISTS(E3_DIR)
    PIN_SAY(E3_DIR_PIN);
  #endif
  #if PIN_EXISTS(E3_ENABLE)
    PIN_SAY(E3_ENABLE_PIN);
  #endif
  #if PIN_EXISTS(E3_STEP)
    PIN_SAY(E3_STEP_PIN);
  #endif
  #if PIN_EXISTS(E4_DIR)
    PIN_SAY(E4_DIR_PIN);
  #endif
  #if PIN_EXISTS(E4_ENABLE)
    PIN_SAY(E4_ENABLE_PIN);
  #endif
  #if PIN_EXISTS(E4_STEP)
    PIN_SAY(E4_STEP_PIN);
  #endif
  #if defined(encrot1) && encrot1 > -1
    PIN_SAY(encrot1);
  #endif
  #if defined(encrot2) && encrot2 > -1
    PIN_SAY(encrot2);
  #endif
  #if defined(encrot3) && encrot3 > -1
    PIN_SAY(encrot3);
  #endif
  #if defined(EXT_AUX_A0_IO) && EXT_AUX_A0_IO > -1
    PIN_SAY(EXT_AUX_A0_IO);
  #endif
  #if defined(EXT_AUX_A1) && EXT_AUX_A1 > -1
    PIN_SAY(EXT_AUX_A1);
  #endif
  #if defined(EXT_AUX_A1_IO) && EXT_AUX_A1_IO > -1
    PIN_SAY(EXT_AUX_A1_IO);
  #endif
  #if defined(EXT_AUX_A2) && EXT_AUX_A2 > -1
    PIN_SAY(EXT_AUX_A2);
  #endif
  #if defined(EXT_AUX_A2_IO) && EXT_AUX_A2_IO > -1
    PIN_SAY(EXT_AUX_A2_IO);
  #endif
  #if defined(EXT_AUX_A3) && EXT_AUX_A3 > -1
    PIN_SAY(EXT_AUX_A3);
  #endif
  #if defined(EXT_AUX_A3_IO) && EXT_AUX_A3_IO > -1
    PIN_SAY(EXT_AUX_A3_IO);
  #endif
  #if defined(EXT_AUX_A4) && EXT_AUX_A4 > -1
    PIN_SAY(EXT_AUX_A4);
  #endif
  #if defined(EXT_AUX_A4_IO) && EXT_AUX_A4_IO > -1
    PIN_SAY(EXT_AUX_A4_IO);
  #endif
  #if defined(EXT_AUX_PWM_D24) && EXT_AUX_PWM_D24 > -1
    PIN_SAY(EXT_AUX_PWM_D24);
  #endif
  #if defined(EXT_AUX_RX1_D2) && EXT_AUX_RX1_D2 > -1
    PIN_SAY(EXT_AUX_RX1_D2);
  #endif
  #if defined(EXT_AUX_SDA_D1) && EXT_AUX_SDA_D1 > -1
    PIN_SAY(EXT_AUX_SDA_D1);
  #endif
  #if defined(EXT_AUX_TX1_D3) && EXT_AUX_TX1_D3 > -1
    PIN_SAY(EXT_AUX_TX1_D3);
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
  #if PIN_EXISTS(FAN)
    PIN_SAY(FAN_PIN);
  #endif
  #if PIN_EXISTS(FAN0)
    PIN_SAY(FAN0_PIN);
  #endif
  #if PIN_EXISTS(FAN1)
    PIN_SAY(FAN1_PIN);
  #endif
  #if PIN_EXISTS(FAN2)
    PIN_SAY(FAN2_PIN);
  #endif
  #if PIN_EXISTS(FIL_RUNOUT)
    PIN_SAY(FIL_RUNOUT_PIN);
  #endif
  #if PIN_EXISTS(FILWIDTH)
    ANALOG_PIN_SAY(FILWIDTH_PIN);
  #endif
  #if defined(GEN7_VERSION) && GEN7_VERSION > -1
    PIN_SAY(GEN7_VERSION);
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
  #if PIN_EXISTS(HEATER_4)
    PIN_SAY(HEATER_4_PIN);
  #endif
  #if PIN_EXISTS(HEATER_5)
    PIN_SAY(HEATER_5_PIN);
  #endif
  #if PIN_EXISTS(HEATER_6)
    PIN_SAY(HEATER_6_PIN);
  #endif
  #if PIN_EXISTS(HEATER_7)
    PIN_SAY(HEATER_7_PIN);
  #endif
  #if PIN_EXISTS(HEATER_BED)
    PIN_SAY(HEATER_BED_PIN);
  #endif
  #if defined(I2C_SCL) && I2C_SCL > -1
    PIN_SAY(I2C_SCL);
  #endif
  #if defined(I2C_SDA) && I2C_SDA > -1
    PIN_SAY(I2C_SDA);
  #endif
  #if PIN_EXISTS(KILL)
    PIN_SAY(KILL_PIN);
  #endif
  #if PIN_EXISTS(LCD_BACKLIGHT)
    PIN_SAY(LCD_BACKLIGHT_PIN);
  #endif
  #if defined(LCD_CONTRAST) && LCD_CONTRAST > -1
    PIN_SAY(LCD_CONTRAST);
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
  #if defined(LCD_PINS_ENABLE) && LCD_PINS_ENABLE > -1
    PIN_SAY(LCD_PINS_ENABLE);
  #endif
  #if defined(LCD_PINS_RS) && LCD_PINS_RS > -1
    PIN_SAY(LCD_PINS_RS);
  #endif
  #if defined(LCD_SDSS) && LCD_SDSS > -1
    PIN_SAY(LCD_SDSS);
  #endif
  #if PIN_EXISTS(LED)
    PIN_SAY(LED_PIN);
  #endif
  #if PIN_EXISTS(MAIN_VOLTAGE_MEASURE)
    PIN_SAY(MAIN_VOLTAGE_MEASURE_PIN);
  #endif
  #if defined(MAX6675_SS) && MAX6675_SS > -1
    PIN_SAY(MAX6675_SS);
  #endif
  #if PIN_EXISTS(MISO)
    PIN_SAY(MISO_PIN);
  #endif
  #if PIN_EXISTS(MOSFET_D)
    PIN_SAY(MOSFET_D_PIN);
  #endif
  #if PIN_EXISTS(MOSI)
    PIN_SAY(MOSI_PIN);
  #endif
  #if PIN_EXISTS(MOTOR_CURRENT_PWM_E)
    PIN_SAY(MOTOR_CURRENT_PWM_E_PIN);
  #endif
  #if PIN_EXISTS(MOTOR_CURRENT_PWM_XY)
    PIN_SAY(MOTOR_CURRENT_PWM_XY_PIN);
  #endif
  #if PIN_EXISTS(MOTOR_CURRENT_PWM_Z)
    PIN_SAY(MOTOR_CURRENT_PWM_Z_PIN);
  #endif
  #if defined(NUM_TLCS) && NUM_TLCS > -1
    PIN_SAY(NUM_TLCS);
  #endif
  #if PIN_EXISTS(PHOTOGRAPH)
    PIN_SAY(PHOTOGRAPH_PIN);
  #endif
  #if PIN_EXISTS(PS_ON)
    PIN_SAY(PS_ON_PIN);
  #endif
  #if PIN_EXISTS(RAMPS_D10)
    PIN_SAY(RAMPS_D10_PIN);
  #endif
  #if PIN_EXISTS(RAMPS_D8)
    PIN_SAY(RAMPS_D8_PIN);
  #endif
  #if PIN_EXISTS(RAMPS_D9)
    PIN_SAY(RAMPS_D9_PIN);
  #endif
  #if PIN_EXISTS(RX_ENABLE)
    PIN_SAY(RX_ENABLE_PIN);
  #endif
  #if PIN_EXISTS(SAFETY_TRIGGERED)
    PIN_SAY(SAFETY_TRIGGERED_PIN);
  #endif
  #if PIN_EXISTS(SCK)
    PIN_SAY(SCK_PIN);
  #endif
  #if defined(SCL) && SCL > -1
    PIN_SAY(SCL);
  #endif
  #if PIN_EXISTS(SD_DETECT)
    PIN_SAY(SD_DETECT_PIN);
  #endif
  #if defined(SDA) && SDA > -1
    PIN_SAY(SDA);
  #endif
  #if defined(SDPOWER) && SDPOWER > -1
    PIN_SAY(SDPOWER);
  #endif
  #if defined(SDSS) && SDSS > -1
    PIN_SAY(SDSS);
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
  #if defined(SHIFT_CLK) && SHIFT_CLK > -1
    PIN_SAY(SHIFT_CLK);
  #endif
  #if defined(SHIFT_EN) && SHIFT_EN > -1
    PIN_SAY(SHIFT_EN);
  #endif
  #if defined(SHIFT_LD) && SHIFT_LD > -1
    PIN_SAY(SHIFT_LD);
  #endif
  #if defined(SHIFT_OUT) && SHIFT_OUT > -1
    PIN_SAY(SHIFT_OUT);
  #endif
  #if PIN_EXISTS(SLED)
    PIN_SAY(SLED_PIN);
  #endif
  #if PIN_EXISTS(SLEEP_WAKE)
    PIN_SAY(SLEEP_WAKE_PIN);
  #endif
  #if PIN_EXISTS(SOL1)
    PIN_SAY(SOL1_PIN);
  #endif
  #if PIN_EXISTS(SOL2)
    PIN_SAY(SOL2_PIN);
  #endif
  #if PIN_EXISTS(SPINDLE_ENABLE)
    PIN_SAY(SPINDLE_ENABLE_PIN);
  #endif
  #if PIN_EXISTS(SPINDLE_SPEED)
    PIN_SAY(SPINDLE_SPEED_PIN);
  #endif
  #if PIN_EXISTS(SS)
    PIN_SAY(SS_PIN);
  #endif
  #if PIN_EXISTS(STAT_LED_BLUE)
    PIN_SAY(STAT_LED_BLUE_PIN);
  #endif
  #if PIN_EXISTS(STAT_LED_RED)
    PIN_SAY(STAT_LED_RED_PIN);
  #endif
  #if PIN_EXISTS(STEPPER_RESET)
    PIN_SAY(STEPPER_RESET_PIN);
  #endif
  #if PIN_EXISTS(SUICIDE)
    PIN_SAY(SUICIDE_PIN);
  #endif
  #if defined(TC1) && TC1 > -1
    ANALOG_PIN_SAY(TC1);
  #endif
  #if defined(TC2) && TC2 > -1
    ANALOG_PIN_SAY(TC2);
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
  #if PIN_EXISTS(TEMP_4)
    ANALOG_PIN_SAY(TEMP_4_PIN);
  #endif
  #if PIN_EXISTS(TEMP_BED)
    ANALOG_PIN_SAY(TEMP_BED_PIN);
  #endif
  #if PIN_EXISTS(TEMP_X)
    ANALOG_PIN_SAY(TEMP_X_PIN);
  #endif
  #if defined(TLC_BLANK_BIT) && TLC_BLANK_BIT > -1
    PIN_SAY(TLC_BLANK_BIT);
  #endif
  #if PIN_EXISTS(TLC_BLANK)
    PIN_SAY(TLC_BLANK_PIN);
  #endif
  #if defined(TLC_CLOCK_BIT) && TLC_CLOCK_BIT > -1
    PIN_SAY(TLC_CLOCK_BIT);
  #endif
  #if PIN_EXISTS(TLC_CLOCK)
    PIN_SAY(TLC_CLOCK_PIN);
  #endif
  #if defined(TLC_DATA_BIT) && TLC_DATA_BIT > -1
    PIN_SAY(TLC_DATA_BIT);
  #endif
  #if PIN_EXISTS(TLC_DATA)
    PIN_SAY(TLC_DATA_PIN);
  #endif
  #if PIN_EXISTS(TLC_XLAT)
    PIN_SAY(TLC_XLAT_PIN);
  #endif
  #if PIN_EXISTS(TX_ENABLE)
    PIN_SAY(TX_ENABLE_PIN);
  #endif
  #if defined(UNUSED_PWM) && UNUSED_PWM > -1
    PIN_SAY(UNUSED_PWM);
  #endif
  #if PIN_EXISTS(X_ATT)
    PIN_SAY(X_ATT_PIN);
  #endif
  #if PIN_EXISTS(X_DIR)
    PIN_SAY(X_DIR_PIN);
  #endif
  #if PIN_EXISTS(X_ENABLE)
    PIN_SAY(X_ENABLE_PIN);
  #endif
  #if PIN_EXISTS(X_MAX)
    PIN_SAY(X_MAX_PIN);
  #endif
  #if PIN_EXISTS(X_MIN)
    PIN_SAY(X_MIN_PIN);
  #endif
  #if PIN_EXISTS(X_MS1)
    PIN_SAY(X_MS1_PIN);
  #endif
  #if PIN_EXISTS(X_MS2)
    PIN_SAY(X_MS2_PIN);
  #endif
  #if PIN_EXISTS(X_STEP)
    PIN_SAY(X_STEP_PIN);
  #endif
  #if PIN_EXISTS(X_STOP)
    PIN_SAY(X_STOP_PIN);
  #endif
  #if PIN_EXISTS(X2_DIR)
    PIN_SAY(X2_DIR_PIN);
  #endif
  #if PIN_EXISTS(X2_ENABLE)
    PIN_SAY(X2_ENABLE_PIN);
  #endif
  #if PIN_EXISTS(X2_STEP)
    PIN_SAY(X2_STEP_PIN);
  #endif
  #if PIN_EXISTS(Y_ATT)
    PIN_SAY(Y_ATT_PIN);
  #endif
  #if PIN_EXISTS(Y_DIR)
    PIN_SAY(Y_DIR_PIN);
  #endif
  #if PIN_EXISTS(Y_ENABLE)
    PIN_SAY(Y_ENABLE_PIN);
  #endif
  #if PIN_EXISTS(Y_MAX)
    PIN_SAY(Y_MAX_PIN);
  #endif
  #if PIN_EXISTS(Y_MIN)
    PIN_SAY(Y_MIN_PIN);
  #endif
  #if PIN_EXISTS(Y_MS1)
    PIN_SAY(Y_MS1_PIN);
  #endif
  #if PIN_EXISTS(Y_MS2)
    PIN_SAY(Y_MS2_PIN);
  #endif
  #if PIN_EXISTS(Y_STEP)
    PIN_SAY(Y_STEP_PIN);
  #endif
  #if PIN_EXISTS(Y_STOP)
    PIN_SAY(Y_STOP_PIN);
  #endif
  #if PIN_EXISTS(Y2_DIR)
    PIN_SAY(Y2_DIR_PIN);
  #endif
  #if PIN_EXISTS(Y2_ENABLE)
    PIN_SAY(Y2_ENABLE_PIN);
  #endif
  #if PIN_EXISTS(Y2_STEP)
    PIN_SAY(Y2_STEP_PIN);
  #endif
  #if PIN_EXISTS(Z_ATT)
    PIN_SAY(Z_ATT_PIN);
  #endif
  #if PIN_EXISTS(Z_DIR)
    PIN_SAY(Z_DIR_PIN);
  #endif
  #if PIN_EXISTS(Z_ENABLE)
    PIN_SAY(Z_ENABLE_PIN);
  #endif
  #if PIN_EXISTS(Z_MAX)
    PIN_SAY(Z_MAX_PIN);
  #endif
  #if PIN_EXISTS(Z_MIN)
    PIN_SAY(Z_MIN_PIN);
  #endif
  #if PIN_EXISTS(Z_MIN_PROBE)
    PIN_SAY(Z_MIN_PROBE_PIN);
  #endif
  #if PIN_EXISTS(Z_MS1)
    PIN_SAY(Z_MS1_PIN);
  #endif
  #if PIN_EXISTS(Z_MS2)
    PIN_SAY(Z_MS2_PIN);
  #endif
  #if PIN_EXISTS(Z_STEP)
    PIN_SAY(Z_STEP_PIN);
  #endif
  #if PIN_EXISTS(Z_STOP)
    PIN_SAY(Z_STOP_PIN);
  #endif
  #if PIN_EXISTS(Z2_DIR)
    PIN_SAY(Z2_DIR_PIN);
  #endif
  #if PIN_EXISTS(Z2_ENABLE)
    PIN_SAY(Z2_ENABLE_PIN);
  #endif
  #if PIN_EXISTS(Z2_STEP)
    PIN_SAY(Z2_STEP_PIN);
  #endif

  sprintf(buffer, NAME_FORMAT, "<unused> ");
  SERIAL_ECHO(buffer);
  
  return false;
}  //  report_pin_name

//  True - currently a PWM pin
static bool PWM_status(uint8_t pin) {
  char buffer[20];   // for the sprintf statements
  
  switch(digitalPinToTimer(pin)) {

    #if defined(TCCR0A) && defined(COM0A1)
      case TIMER0A:
        if (TCCR0A & (_BV(COM0A1) | _BV(COM0A0))){
          sprintf(buffer, "PWM:  %4d", OCR0A); 
          SERIAL_ECHO(buffer);
          return true;
        }
        else return false;
        break;
      case TIMER0B:
        if (TCCR0A & (_BV(COM0B1) | _BV(COM0B0))){
          sprintf(buffer, "PWM:  %4d",OCR0B); 
          SERIAL_ECHO(buffer);
          return true;
        }
        else return false;
        break;
    #endif

    #if defined(TCCR1A) && defined(COM1A1)
      case TIMER1A:
        if (TCCR1A & (_BV(COM1A1) | _BV(COM1A0))){
          sprintf(buffer, "PWM:  %4d",OCR1A); 
          SERIAL_ECHO(buffer);
          return true;
        }
        else return false;
        break;
      case TIMER1B:
        if (TCCR1A & (_BV(COM1B1) | _BV(COM1B0))){
          sprintf(buffer, "PWM:  %4d",OCR1B); 
          SERIAL_ECHO(buffer);
          return true;
        }
        else return false;
        break; 
      case TIMER1C:
        if (TCCR1A & (_BV(COM1C1) | _BV(COM1C0))){
          sprintf(buffer, "PWM:  %4d",OCR1C); 
          SERIAL_ECHO(buffer);
          return true;
        }
        else return false;
        break;
    #endif

    #if defined(TCCR2A) && defined(COM2A1)
      case TIMER2A:
        if (TCCR2A & (_BV(COM2A1) | _BV(COM2A0))){
          sprintf(buffer, "PWM:  %4d",OCR2A); 
          SERIAL_ECHO(buffer);
          return true;
        }
        else return false;
        break;
      case TIMER2B:
        if (TCCR2A & (_BV(COM2B1) | _BV(COM2B0))){
          sprintf(buffer, "PWM:  %4d",OCR2B); 
          SERIAL_ECHO(buffer);
          return true;
        }
        else return false;
        break;
    #endif

    #if defined(TCCR3A) && defined(COM3A1)
      case TIMER3A:
        if (TCCR3A & (_BV(COM3A1) | _BV(COM3A0))){
          sprintf(buffer, "PWM:  %4d",OCR3A); 
          SERIAL_ECHO(buffer);
          return true;
        }
        else return false;
        break;
      case TIMER3B:
        if (TCCR3A & (_BV(COM3B1) | _BV(COM3B0))){
          sprintf(buffer, "PWM:  %4d",OCR3B); 
          SERIAL_ECHO(buffer);
          return true;
        }
        else return false;
        break;
      case TIMER3C:
        if (TCCR3A & (_BV(COM3C1) | _BV(COM3C0))){
          sprintf(buffer, "PWM:  %4d",OCR3C); 
          SERIAL_ECHO(buffer);
          return true;
        }
        else return false;
        break;
    #endif

    #if defined(TCCR4A)
      case TIMER4A:
        if (TCCR4A & (_BV(COM4A1) | _BV(COM4A0))){
          sprintf(buffer, "PWM:  %4d",OCR4A); 
          SERIAL_ECHO(buffer);
          return true;
        }
        else return false;
        break;
      case TIMER4B:
        if (TCCR4A & (_BV(COM4B1) | _BV(COM4B0))){
          sprintf(buffer, "PWM:  %4d",OCR4B); 
          SERIAL_ECHO(buffer);
          return true;
        }
        else return false;
        break;
      case TIMER4C:
        if (TCCR4A & (_BV(COM4C1) | _BV(COM4C0))){
          sprintf(buffer, "PWM:  %4d",OCR4C); 
          SERIAL_ECHO(buffer);
          return true;
        }
        else return false;
        break;
    #endif
          
    #if defined(TCCR5A) && defined(COM5A1)
      case TIMER5A:
        if (TCCR5A & (_BV(COM5A1) | _BV(COM5A0))){
          sprintf(buffer, "PWM:  %4d",OCR5A); 
          SERIAL_ECHO(buffer);
          return true;
        }
        else return false;
        break;
      case TIMER5B:
        if (TCCR5A & (_BV(COM5B1) | _BV(COM5B0))){
          sprintf(buffer, "PWM:  %4d",OCR5B); 
          SERIAL_ECHO(buffer);
          return true;
        }
        else return false;
        break;
      case TIMER5C:
        if (TCCR5A & (_BV(COM5C1) | _BV(COM5C0))){
          sprintf(buffer, "PWM:  %4d",OCR5C); 
          SERIAL_ECHO(buffer);
          return true;
        }
        else return false;
        break;
    #endif

    case NOT_ON_TIMER:
      return false;
      break;
     
    default:
      return false;

  }
  
  SERIAL_PROTOCOLPGM("  ");   
}  //PWM_status

static void PWM_details(uint8_t pin)
{

  uint8_t  WGM;

  switch(digitalPinToTimer(pin)) {
 
	#if defined(TCCR0A) && defined(COM0A1)
    case TIMER0A:
      SERIAL_PROTOCOLPGM("    TIMER0A");
      WGM = ((TCCR0B & _BV(WGM02)) >> 1 ) | (TCCR0A & (_BV(WGM00) | _BV(WGM01) ));
      SERIAL_PROTOCOLPAIR("    WGM: ", WGM);
      SERIAL_PROTOCOLPAIR("    TIMSK0: ", TIMSK0);
      if (WGM == 0 || WGM == 2 || WGM == 4 || WGM == 6) SERIAL_PROTOCOLPGM("   Can't be used as a PWM because of counter mode");
      else if (TIMSK0 & _BV(OCIE0A))  SERIAL_PROTOCOLPGM("   Can't be used as a PWM because being used to generate an interrupt");
      else if (TIMSK0 & _BV(TOIE0) )  SERIAL_PROTOCOLPGM("   Probably can't be used as a PWM because counter/timer being used to generate an interrupt");
      else SERIAL_PROTOCOLPGM("   can be used as PWM   ");
      break;         
    case TIMER0B:
      SERIAL_PROTOCOLPGM("    TIMER0B");
      WGM = ((TCCR0B & _BV(WGM02)) >> 1 ) | (TCCR0A & (_BV(WGM00) | _BV(WGM01) ));
      SERIAL_PROTOCOLPAIR("    WGM: ", WGM);
      SERIAL_PROTOCOLPAIR("    TIMSK0: ", TIMSK0);
      if (WGM == 0 || WGM == 2 || WGM == 4 || WGM == 6) SERIAL_PROTOCOLPGM("   Can't be used as a PWM because of counter mode");
      else if (TIMSK0 & _BV(OCIE0B))  SERIAL_PROTOCOLPGM("   Can't be used as a PWM because being used to generate an interrupt");
      else if (TIMSK0 & _BV(TOIE0) )  SERIAL_PROTOCOLPGM("   Probably can't be used as a PWM because counter/timer being used to generate an interrupt");
      else SERIAL_PROTOCOLPGM("   can be used as PWM   ");
      break;
  #endif

  #if defined(TCCR1A) && defined(COM1A1)
    case TIMER1A:
      SERIAL_PROTOCOLPGM("    TIMER1A");
      WGM =   ((TCCR1B & (_BV(WGM12) | _BV(WGM13) )) >> 1 ) | (TCCR1A & (_BV(WGM10) | _BV(WGM11) ));   
      SERIAL_PROTOCOLPAIR("    WGM: ", WGM);
      SERIAL_PROTOCOLPAIR("    TIMSK1: ", TIMSK1);
      if (WGM == 0 || WGM == 4 || WGM == 12 || WGM == 13)   SERIAL_PROTOCOLPGM("   Can't be used as a PWM because of counter mode");
      else if (TIMSK1 &  _BV(OCIE1A))  SERIAL_PROTOCOLPGM("   Can't be used as a PWM because being used to generate an interrupt");
      else if (TIMSK1 & (_BV(TOIE1) | _BV(ICIE1)) )  SERIAL_PROTOCOLPGM("   Probably can't be used as a PWM because counter/timer being used to generate an interrupt");
      else SERIAL_PROTOCOLPGM("   can be used as PWM   ");
      break;
    case TIMER1B:
      SERIAL_PROTOCOLPGM("    TIMER1B");
      WGM =   ((TCCR1B & (_BV(WGM12) | _BV(WGM13) )) >> 1 ) | (TCCR1A & (_BV(WGM10) | _BV(WGM11) ));   
      SERIAL_PROTOCOLPAIR("    WGM: ", WGM);
      SERIAL_PROTOCOLPAIR("    TIMSK1: ", TIMSK1);
      if (WGM == 0 || WGM == 4 || WGM == 12 || WGM == 13)   SERIAL_PROTOCOLPGM("   Can't be used as a PWM because of counter mode");
      else if (TIMSK1 &  _BV(OCIE1B))  SERIAL_PROTOCOLPGM("   Can't be used as a PWM because being used to generate an interrupt");
      else if (TIMSK1 & (_BV(TOIE1) | _BV(ICIE1)) )  SERIAL_PROTOCOLPGM("   Probably can't be used as a PWM because counter/timer being used to generate an interrupt");
      else SERIAL_PROTOCOLPGM("   can be used as PWM   ");          
      break;
    case TIMER1C:
      SERIAL_PROTOCOLPGM("    TIMER1C");
      WGM =   ((TCCR1B & (_BV(WGM12) | _BV(WGM13) )) >> 1 ) | (TCCR1A & (_BV(WGM10) | _BV(WGM11) ));   
      SERIAL_PROTOCOLPAIR("    WGM: ", WGM);
      SERIAL_PROTOCOLPAIR("    TIMSK1: ", TIMSK1);
      if (WGM == 0 || WGM == 4 || WGM == 12 || WGM == 13)   SERIAL_PROTOCOLPGM("   Can't be used as a PWM because of counter mode");
      else if (TIMSK1 &  _BV(OCIE1C))  SERIAL_PROTOCOLPGM("   Can't be used as a PWM because being used to generate an interrupt");
      else if (TIMSK1 & (_BV(TOIE1) | _BV(ICIE1)) )  SERIAL_PROTOCOLPGM("   Probably can't be used as a PWM because counter/timer being used to generate an interrupt");
      else SERIAL_PROTOCOLPGM("   can be used as PWM   ");
      break;
  #endif

  #if defined(TCCR2A) && defined(COM2A1)
    case TIMER2A:
      SERIAL_PROTOCOLPGM("    TIMER2A");
      WGM =   ((TCCR2B & _BV(WGM22) ) >> 1 ) | (TCCR2A & (_BV(WGM20) | _BV(WGM21) ));
      SERIAL_PROTOCOLPAIR("    WGM: ", WGM);
      SERIAL_PROTOCOLPAIR("    TIMSK2: ", TIMSK2);
      if (WGM == 0 || WGM == 2 || WGM == 4 || WGM == 6) SERIAL_PROTOCOLPGM("   Can't be used as a PWM because of counter mode");
      else if (TIMSK2 & (_BV(TOIE2) | _BV(OCIE2A)))  SERIAL_PROTOCOLPGM("   Can't be used as a PWM because being used to generate an interrupt");
      else if (TIMSK2 & _BV(TOIE2) )  SERIAL_PROTOCOLPGM("   Probably can't be used as a PWM because counter/timer being used to generate an interrupt");
      else SERIAL_PROTOCOLPGM("   can be used as PWM   ");
      break;
    case TIMER2B:
      SERIAL_PROTOCOLPGM("    TIMER2B");
      WGM =   ((TCCR2B & _BV(WGM22) ) >> 1 ) | (TCCR2A & (_BV(WGM20) | _BV(WGM21) ));
      SERIAL_PROTOCOLPAIR("    WGM: ", WGM);
      SERIAL_PROTOCOLPAIR("    TIMSK2: ", TIMSK2);
      if (WGM == 0 || WGM == 2 || WGM == 4 || WGM == 6) SERIAL_PROTOCOLPGM("   Can't be used as a PWM because of counter mode");
      else if (TIMSK2 & _BV(OCIE2B))  SERIAL_PROTOCOLPGM("   Can't be used as a PWM because being used to generate an interrupt");
      else if (TIMSK2 & _BV(TOIE2) )  SERIAL_PROTOCOLPGM("   Probably can't be used as a PWM because counter/timer being used to generate an interrupt");
      else SERIAL_PROTOCOLPGM("   can be used as PWM   ");
      break; 
  #endif

  #if defined(TCCR3A) && defined(COM3A1)
    case TIMER3A:
      SERIAL_PROTOCOLPGM("    TIMER3A");
      WGM =   ((TCCR3B & _BV(WGM32) ) >> 1 ) | (TCCR3A & (_BV(WGM30) | _BV(WGM31) ));
      SERIAL_PROTOCOLPAIR("    WGM: ", WGM);
      SERIAL_PROTOCOLPAIR("    TIMSK3: ", TIMSK3);
      if (WGM == 0 || WGM == 4 || WGM == 12 || WGM == 13)   SERIAL_PROTOCOLPGM("   Can't be used as a PWM because of counter mode");
      else if (TIMSK3 &  _BV(OCIE3A))  SERIAL_PROTOCOLPGM("   Can't be used as a PWM because being used to generate an interrupt");
      else if (TIMSK3 & (_BV(TOIE3) | _BV(ICIE3)) )  SERIAL_PROTOCOLPGM("   Probably can't be used as a PWM because counter/timer being used to generate an interrupt");
      else SERIAL_PROTOCOLPGM("   can be used as PWM   ");
      break;      
    case TIMER3B:
      SERIAL_PROTOCOLPGM("    TIMER3B");
      WGM =   ((TCCR3B & _BV(WGM32) ) >> 1 ) | (TCCR3A & (_BV(WGM30) | _BV(WGM31) ));
      SERIAL_PROTOCOLPAIR("    WGM: ", WGM);
      SERIAL_PROTOCOLPAIR("    TIMSK3: ", TIMSK3);
      if (WGM == 0 || WGM == 4 || WGM == 12 || WGM == 13)   SERIAL_PROTOCOLPGM("   Can't be used as a PWM because of counter mode");
      else if (TIMSK3 &  _BV(OCIE3B))  SERIAL_PROTOCOLPGM("   Can't be used as a PWM because being used to generate an interrupt");
      else if (TIMSK3 & (_BV(TOIE3) | _BV(ICIE3)) )  SERIAL_PROTOCOLPGM("   Probably can't be used as a PWM because counter/timer being used to generate an interrupt");
      else SERIAL_PROTOCOLPGM("   can be used as PWM   ");
      break;
    case TIMER3C:
      SERIAL_PROTOCOLPGM("    TIMER3C");
      WGM =   ((TCCR3B & _BV(WGM32) ) >> 1 ) | (TCCR3A & (_BV(WGM30) | _BV(WGM31) ));
      SERIAL_PROTOCOLPAIR("    WGM: ", WGM);
      SERIAL_PROTOCOLPAIR("    TIMSK3: ", TIMSK3);
      if (WGM == 0 || WGM == 4 || WGM == 12 || WGM == 13)   SERIAL_PROTOCOLPGM("   Can't be used as a PWM because of counter mode");
      else if (TIMSK3 &  _BV(OCIE3C))  SERIAL_PROTOCOLPGM("   Can't be used as a PWM because being used to generate an interrupt");
      else if (TIMSK3 & (_BV(TOIE3) | _BV(ICIE3)) )  SERIAL_PROTOCOLPGM("   Probably can't be used as a PWM because counter/timer being used to generate an interrupt");
      else SERIAL_PROTOCOLPGM("   can be used as PWM   ");
      break;
  #endif

  #if defined(TCCR4A)
    case TIMER4A:
      SERIAL_PROTOCOLPGM("    TIMER4A");
      WGM =   ((TCCR4B & (_BV(WGM42) | _BV(WGM43) )) >> 1 ) | (TCCR4A & (_BV(WGM40) | _BV(WGM41) ));
      SERIAL_PROTOCOLPAIR("    WGM: ", WGM);
      SERIAL_PROTOCOLPAIR("    TIMSK4: ", TIMSK4);
      if (WGM == 0 || WGM == 4 || WGM == 12 || WGM == 13)   SERIAL_PROTOCOLPGM("   Can't be used as a PWM because of counter mode");
      else if (TIMSK4 &  _BV(OCIE4A))  SERIAL_PROTOCOLPGM("   Can't be used as a PWM because being used to generate an interrupt");
      else if (TIMSK4 & (_BV(TOIE4) | _BV(ICIE4)) )  SERIAL_PROTOCOLPGM("   Probably can't be used as a PWM because counter/timer being used to generate an interrupt");
      else SERIAL_PROTOCOLPGM("   can be used as PWM   ");
      break;      
    case TIMER4B:
      SERIAL_PROTOCOLPGM("    TIMER4B");
      WGM =   ((TCCR4B & (_BV(WGM42) | _BV(WGM43) )) >> 1 ) | (TCCR4A & (_BV(WGM40) | _BV(WGM41) ));
      SERIAL_PROTOCOLPAIR("    WGM: ", WGM);
      SERIAL_PROTOCOLPAIR("    TIMSK4: ", TIMSK4);
      if (WGM == 0 || WGM == 4 || WGM == 12 || WGM == 13)   SERIAL_PROTOCOLPGM("   Can't be used as a PWM because of counter mode");
      else if (TIMSK4 &  _BV(OCIE4B))  SERIAL_PROTOCOLPGM("   Can't be used as a PWM because being used to generate an interrupt");
      else if (TIMSK4 & (_BV(TOIE4) | _BV(ICIE4)) )  SERIAL_PROTOCOLPGM("   Probably can't be used as a PWM because counter/timer being used to generate an interrupt");
      else SERIAL_PROTOCOLPGM("   can be used as PWM   ");
      break;      
    case TIMER4C:
      SERIAL_PROTOCOLPGM("    TIMER4C");
      WGM =   ((TCCR4B & (_BV(WGM42) | _BV(WGM43) )) >> 1 ) | (TCCR4A & (_BV(WGM40) | _BV(WGM41) ));
      SERIAL_PROTOCOLPAIR("    WGM: ", WGM);
      SERIAL_PROTOCOLPAIR("    TIMSK4: ", TIMSK4);
      if (WGM == 0 || WGM == 4 || WGM == 12 || WGM == 13)   SERIAL_PROTOCOLPGM("   Can't be used as a PWM because of counter mode");
      else if (TIMSK4 &  _BV(OCIE4C))  SERIAL_PROTOCOLPGM("   Can't be used as a PWM because being used to generate an interrupt");
      else if (TIMSK4 & (_BV(TOIE4) | _BV(ICIE4)) )  SERIAL_PROTOCOLPGM("   Probably can't be used as a PWM because counter/timer being used to generate an interrupt");
      else SERIAL_PROTOCOLPGM("   can be used as PWM   ");
      break;      

  #endif
          
  #if defined(TCCR5A) && defined(COM5A1)
    case TIMER5A:
      SERIAL_PROTOCOLPGM("    TIMER5A");
      WGM =   ((TCCR5B & (_BV(WGM52) | _BV(WGM53) )) >> 1 ) | (TCCR5A & (_BV(WGM50) | _BV(WGM51) ));
      SERIAL_PROTOCOLPAIR("    WGM: ", WGM);
      SERIAL_PROTOCOLPAIR("    TIMSK5: ", TIMSK5);
      if (WGM == 0 || WGM == 4 || WGM == 12 || WGM == 13)   SERIAL_PROTOCOLPGM("   Can't be used as a PWM because of counter mode");
      else if (TIMSK5 &  _BV(OCIE5A))  SERIAL_PROTOCOLPGM("   Can't be used as a PWM because being used to generate an interrupt");
      else if (TIMSK5 & (_BV(TOIE5) | _BV(ICIE5)) )  SERIAL_PROTOCOLPGM("   Probably can't be used as a PWM because counter/timer being used to generate an interrupt");
      else SERIAL_PROTOCOLPGM("   can be used as PWM   ");
      break;      
    case TIMER5B:
      SERIAL_PROTOCOLPGM("    TIMER5B");
      WGM =   ((TCCR5B & (_BV(WGM52) | _BV(WGM53) )) >> 1 ) | (TCCR5A & (_BV(WGM50) | _BV(WGM51) ));
      SERIAL_PROTOCOLPAIR("    WGM: ", WGM);
      SERIAL_PROTOCOLPAIR("    TIMSK5: ", TIMSK5);
      if (WGM == 0 || WGM == 4 || WGM == 12 || WGM == 13)   SERIAL_PROTOCOLPGM("   Can't be used as a PWM because of counter mode");
      else if (TIMSK5 &  _BV(OCIE5B))  SERIAL_PROTOCOLPGM("   Can't be used as a PWM because being used to generate an interrupt");
      else if (TIMSK5 & (_BV(TOIE5) | _BV(ICIE5)) )  SERIAL_PROTOCOLPGM("   Probably can't be used as a PWM because counter/timer being used to generate an interrupt");
      else SERIAL_PROTOCOLPGM("   can be used as PWM   ");
      break;      
    case TIMER5C:
      SERIAL_PROTOCOLPGM("    TIMER5C");
      WGM =   ((TCCR5B & (_BV(WGM52) | _BV(WGM53) )) >> 1 ) | (TCCR5A & (_BV(WGM50) | _BV(WGM51) ));
      SERIAL_PROTOCOLPAIR("    WGM: ", WGM);
      SERIAL_PROTOCOLPAIR("    TIMSK5: ", TIMSK5);
      if (WGM == 0 || WGM == 4 || WGM == 12 || WGM == 13)   SERIAL_PROTOCOLPGM("   Can't be used as a PWM because of counter mode");
      else if (TIMSK5 &  _BV(OCIE5C))  SERIAL_PROTOCOLPGM("   Can't be used as a PWM because being used to generate an interrupt");
      else if (TIMSK5 & (_BV(TOIE5) | _BV(ICIE5)) )  SERIAL_PROTOCOLPGM("   Probably can't be used as a PWM because counter/timer being used to generate an interrupt");
      else SERIAL_PROTOCOLPGM("   can be used as PWM   ");
      break;      
  #endif

	case NOT_ON_TIMER:
    break;
	}
  SERIAL_PROTOCOLPGM("  ");   
}  // PWM_details



inline void report_pin_state(int8_t pin) {
  SERIAL_ECHO((int)pin);
  SERIAL_CHAR(' ');
  bool dummy;
  if (report_pin_name(pin, dummy)) {
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

// pretty report with PWM info
inline void report_pin_state_extended(int8_t pin, bool ignore) {

  char buffer[30];   // for the sprintf statements
    
 // report pin number 
  sprintf(buffer, "PIN:% 3d ", pin);
  SERIAL_ECHO(buffer);   

 // report pin name 
  bool analog_pin;
  report_pin_name(pin, analog_pin); 
  
// report pin state
  if (pin_is_protected(pin) && ignore == false) 
    SERIAL_ECHOPGM("protected ");
  else {
    if (analog_pin) {
        sprintf(buffer, "Analog in =% 5d", analogRead(pin - analogInputToDigitalPin(0)));
        SERIAL_ECHO(buffer); 
       }
    else {
      if (!get_pinMode(pin)) {
        pinMode(pin, INPUT_PULLUP);  // make sure input isn't floating
        SERIAL_PROTOCOLPAIR("Input  = ", digitalRead_mod(pin));
      }  
      else if  (PWM_status(pin)) ;
      else SERIAL_PROTOCOLPAIR("Output = ", digitalRead_mod(pin));  
    }
  }

// report PWM capabilities
  PWM_details(pin);
  SERIAL_EOL;
}

