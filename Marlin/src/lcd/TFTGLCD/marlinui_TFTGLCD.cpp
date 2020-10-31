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

#include "../../inc/MarlinConfigPre.h"

#if IS_TFTGLCD_PANEL

/**
 * marlinui_TFTGLCD.cpp
 *
 * Implementation of the LCD display routines for a TFT GLCD displays with external controller.
 * This display looks like a REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER but has good text font
 * and supports color output.
 */

#if NONE(__AVR__, TARGET_LPC1768, __STM32F1__, STM32F4xx)
  #warning "Selected platform not yet tested. Please contribute your good pin mappings."
#endif

#if ENABLED(TFTGLCD_PANEL_SPI)
  #include <SPI.h>
#else
  #include <Wire.h>
#endif

#include "marlinui_TFTGLCD.h"
#include "../marlinui.h"
#include "../../libs/numtostr.h"

#include "../../sd/cardreader.h"
#include "../../module/temperature.h"
#include "../../module/printcounter.h"
#include "../../module/planner.h"
#include "../../module/motion.h"

#if DISABLED(LCD_PROGRESS_BAR) && BOTH(FILAMENT_LCD_DISPLAY, SDSUPPORT)
  #include "../../feature/filwidth.h"
  #include "../../gcode/parser.h"
#endif

#if ENABLED(AUTO_BED_LEVELING_UBL)
  #include "../../feature/bedlevel/bedlevel.h"
#endif

TFTGLCD lcd;

#define ICON_LOGO       B00000001
#define ICON_TEMP1      B00000010    //hotend 1
#define ICON_TEMP2      B00000100    //hotend 2
#define ICON_TEMP3      B00001000    //hotend 3
#define ICON_BED        B00010000
#define ICON_FAN        B00100000
#define ICON_HOT        B01000000    //when any T > 50deg
#define PIC_MASK        0x7F

// LEDs not used, for compatibility with Smoothieware
#define LED_HOTEND_ON   B00000001
#define LED_BED_ON      B00000010
#define LED_FAN_ON      B00000100
#define LED_HOT         B00001000
#define LED_MASK        0x0F

#define FBSIZE          (LCD_WIDTH * LCD_HEIGHT + 2)
#define MIDDLE_Y        ((LCD_HEIGHT - 1) / 2)

// Markers for change line colors
#define COLOR_EDIT      '#'
#define COLOR_ERROR     '!'

#ifdef CONVERT_TO_EXT_ASCII   //use standart pseudographic symbols in ASCII table
  #define LR            179   //vertical line
  #define TRC           191   //top right corner
  #define BLC           192   //bottom left corner
  #define GL            196   //horizontal line
  #define BRC           217   //bottom right corner, should be replaced to 12 for some languages
  #define TLC           218   //top left corner, should be replaced to 13 for some languages
#else //next symbols must be present in panel font
  #define LR            8     //equal to 179
  #define TRC           9     //equal to 191
  #define BLC           10    //equal to 192
  #define GL            11    //equal to 196
  #define BRC           12    //equal to 217
  #define TLC           13    //equal to 218
#endif

#define Marlin  0x01

enum Commands {         // based on Smoothieware commands
  GET_SPI_DATA = 0,
  READ_BUTTONS,         // read buttons
  READ_ENCODER,         // read encoder
  LCD_WRITE,            // write all screen to LCD
  BUZZER,               // beep buzzer
  CONTRAST,             // set contrast (brightnes)
  // Other commands... 0xE0 thru 0xFF
  GET_LCD_ROW = 0xE0,   // for detect panel
  GET_LCD_COL,          // reserved for compatibility with Smoothieware, not used
  LCD_PUT,              // write one line to LCD
  CLR_SCREEN,
  INIT_SCREEN = 0xFE    // clear panel buffer
};

static unsigned char framebuffer[FBSIZE];
static unsigned char *fb;
static uint8_t cour_line;
static uint8_t picBits, ledBits, hotBits;
static uint8_t PanelDetected = 0;

// Different platforms use different SPI methods
#if ANY(__AVR__, TARGET_LPC1768, __STM32F1__, ARDUINO_ARCH_SAM, __SAMD51__, __MK20DX256__, __MK64FX512__)
  #define SPI_SEND_ONE(V) SPI.transfer(V);
  #define SPI_SEND_TWO(V) SPI.transfer16(V);
#elif defined(STM32F4xx)
  #define SPI_SEND_ONE(V) SPI.transfer(V, SPI_CONTINUE);
  #define SPI_SEND_TWO(V) SPI.transfer16(V, SPI_CONTINUE);
#elif defined(ARDUINO_ARCH_ESP32)
  #define SPI_SEND_ONE(V) SPI.write(V);
  #define SPI_SEND_TWO(V) SPI.write16(V);
#endif

#if ANY(__AVR__, ARDUINO_ARCH_SAM, __SAMD51__, __MK20DX256__, __MK64FX512__)
  #define SPI_SEND_SOME(V,L,Z)  SPI.transfer(&V[Z], L);
#elif defined(STM32F4xx)
  #define SPI_SEND_SOME(V,L,Z)  SPI.transfer(&V[Z], L, SPI_CONTINUE);
#elif ANY(TARGET_LPC1768, __STM32F1__, ARDUINO_ARCH_ESP32)
  #define SPI_SEND_SOME(V,L,Z)  do{ for (uint16_t i = 0; i < L; i++) SPI_SEND_ONE(V[(Z)+i]); }while(0)
#endif

// Constructor
TFTGLCD::TFTGLCD() {}

// Clear local buffer
void TFTGLCD::clear_buffer() {
  memset(&framebuffer[0], ' ', FBSIZE - 2);
  framebuffer[FBSIZE - 1] = framebuffer[FBSIZE - 2] = 0;
  picBits = ledBits = 0;
}

// Clear panel's screen
void TFTGLCD::clr_screen() {
  if (!PanelDetected) return;
  #if ENABLED(TFTGLCD_PANEL_SPI)
    WRITE(TFTGLCD_CS, LOW);
    SPI_SEND_ONE(CLR_SCREEN);
    WRITE(TFTGLCD_CS, HIGH);
  #else
    Wire.beginTransmission((uint8_t)LCD_I2C_ADDRESS);  //set I2C device address
    Wire.write(CLR_SCREEN);
    Wire.endTransmission(); //transmit data
  #endif
}

// Set new text cursor position
void TFTGLCD::setCursor(uint8_t col, uint8_t row) {
  fb = &framebuffer[0] + col + row * LCD_WIDTH;
  cour_line = row;
}

// Send char to buffer
void TFTGLCD::write(char c) {
  *fb++ = c;
}

// Send text line to buffer
void TFTGLCD::print(const char *line) {
  while (*line) *fb++ = *line++;
}

// For menu
void TFTGLCD::print_line() {
  if (!PanelDetected) return;
  #if ENABLED(TFTGLCD_PANEL_SPI)
    WRITE(TFTGLCD_CS, LOW);
    SPI_SEND_ONE(LCD_PUT);
    SPI_SEND_ONE(cour_line);
    SPI_SEND_SOME(framebuffer, LCD_WIDTH, cour_line * LCD_WIDTH);
    WRITE(TFTGLCD_CS, HIGH);
  #else
    Wire.beginTransmission((uint8_t)LCD_I2C_ADDRESS);  //set I2C device address
    Wire.write(LCD_PUT);
    Wire.write(cour_line);
    Wire.write(&framebuffer[cour_line * LCD_WIDTH], LCD_WIDTH);  //transfer 1 line to txBuffer
    Wire.endTransmission(); //transmit data
    safe_delay(1);
  #endif
}

void TFTGLCD::print_screen(){
  if (!PanelDetected) return;
  framebuffer[FBSIZE - 2] = picBits & PIC_MASK;
  framebuffer[FBSIZE - 1] = ledBits;
  #if ENABLED(TFTGLCD_PANEL_SPI)
    // Send all framebuffer to panel
    WRITE(TFTGLCD_CS, LOW);
    SPI_SEND_ONE(LCD_WRITE);
    SPI_SEND_SOME(framebuffer, FBSIZE, 0);
    WRITE(TFTGLCD_CS, HIGH);
  #else
    uint8_t r;
    // Send framebuffer to panel by line
    Wire.beginTransmission((uint8_t)LCD_I2C_ADDRESS);
    // First line
    Wire.write(LCD_WRITE);
    Wire.write(&framebuffer[0], LCD_WIDTH);
    Wire.endTransmission();
    for (r = 1; r < (LCD_HEIGHT - 1); r++) {
      Wire.beginTransmission((uint8_t)LCD_I2C_ADDRESS);
      Wire.write(&framebuffer[r * LCD_WIDTH], LCD_WIDTH);
      Wire.endTransmission();
    }
    // Last line
    Wire.beginTransmission((uint8_t)LCD_I2C_ADDRESS);
    Wire.write(&framebuffer[r * LCD_WIDTH], LCD_WIDTH);
    Wire.write(&framebuffer[FBSIZE - 2], 2);
    Wire.endTransmission();
  #endif
}

void TFTGLCD::setContrast(uint16_t contrast) {
  if (!PanelDetected) return;
  #if ENABLED(TFTGLCD_PANEL_SPI)
    WRITE(TFTGLCD_CS, LOW);
    SPI_SEND_ONE(CONTRAST);
    SPI_SEND_ONE((uint8_t)contrast);
    WRITE(TFTGLCD_CS, HIGH);
  #else
    Wire.beginTransmission((uint8_t)LCD_I2C_ADDRESS);
    Wire.write(CONTRAST);
    Wire.write((uint8_t)contrast);
    Wire.endTransmission();
  #endif
}

extern volatile int8_t encoderDiff;

// Read buttons and encoder states
uint8_t MarlinUI::read_slow_buttons(void) {
  if (!PanelDetected)    return 0;
  #if ENABLED(TFTGLCD_PANEL_SPI)
    uint8_t b = 0;
    WRITE(TFTGLCD_CS, LOW);
    SPI_SEND_ONE(READ_ENCODER);
    #ifndef STM32F4xx
      WRITE(TFTGLCD_CS, LOW); // for delay
    #endif
    encoderDiff += SPI_SEND_ONE(READ_BUTTONS);
    #ifndef STM32F4xx
      WRITE(TFTGLCD_CS, LOW); // for delay
      WRITE(TFTGLCD_CS, LOW);
    #endif
    b = SPI_SEND_ONE(GET_SPI_DATA);
    WRITE(TFTGLCD_CS, HIGH);
    return b;
  #else
    Wire.beginTransmission((uint8_t)LCD_I2C_ADDRESS);
    Wire.write(READ_ENCODER);
    Wire.endTransmission();
    #ifdef __AVR__
      Wire.requestFrom((uint8_t)LCD_I2C_ADDRESS, 2, 0, 0, 1);
    #elif defined(__STM32F1__)
      Wire.requestFrom((uint8_t)LCD_I2C_ADDRESS, (uint8_t)2);
    #elif EITHER(STM32F4xx, TARGET_LPC1768)
      Wire.requestFrom(LCD_I2C_ADDRESS, 2);
    #endif
    encoderDiff += Wire.read();
    return Wire.read();   //buttons
  #endif
}

// Duration in ms, freq in Hz
void MarlinUI::buzz(const long duration, const uint16_t freq) {
  if (!PanelDetected) return;
  if (!buzzer_enabled) return;
  #if ENABLED(TFTGLCD_PANEL_SPI)
    WRITE(TFTGLCD_CS, LOW);
    SPI_SEND_ONE(BUZZER);
    SPI_SEND_TWO((uint16_t)duration);
    SPI_SEND_TWO(freq);
    WRITE(TFTGLCD_CS, HIGH);
  #else
    Wire.beginTransmission((uint8_t)LCD_I2C_ADDRESS);
    Wire.write(BUZZER);
    Wire.write((uint8_t)(duration >> 8));
    Wire.write((uint8_t)duration);
    Wire.write((uint8_t)(freq >> 8));
    Wire.write((uint8_t)freq);
    Wire.endTransmission();
  #endif
}

void MarlinUI::init_lcd() {
  uint8_t t;
  lcd.clear_buffer();
  t = 0;
  #if ENABLED(TFTGLCD_PANEL_SPI)
    // SPI speed must be less 10MHz
    _SET_OUTPUT(TFTGLCD_CS);
    WRITE(TFTGLCD_CS, HIGH);
    spiInit(TERN(__STM32F1__, SPI_QUARTER_SPEED, SPI_FULL_SPEED));
    WRITE(TFTGLCD_CS, LOW);
    SPI_SEND_ONE(GET_LCD_ROW);
    t = SPI_SEND_ONE(GET_SPI_DATA);
  #else
    #ifdef TARGET_LPC1768
      Wire.begin();   //init twi/I2C
    #else
      Wire.begin((uint8_t)LCD_I2C_ADDRESS); //init twi/I2C
    #endif
    Wire.beginTransmission((uint8_t)LCD_I2C_ADDRESS);
    Wire.write((uint8_t)GET_LCD_ROW); // put command to buffer
    Wire.endTransmission(); // send buffer
    #ifdef __AVR__
      Wire.requestFrom((uint8_t)LCD_I2C_ADDRESS, 1, 0, 0, 1);
    #elif ANY(__STM32F1__, STM32F4xx, TARGET_LPC1768)
      Wire.requestFrom(LCD_I2C_ADDRESS, 1);
    #endif
    t = (uint8_t)Wire.read();
  #endif

  if (t == LCD_HEIGHT) {
    PanelDetected = 1;
    #if ENABLED(TFTGLCD_PANEL_SPI)
      SPI_SEND_ONE(INIT_SCREEN);
      SPI_SEND_ONE(Marlin);
      WRITE(TFTGLCD_CS, HIGH);
    #else
      Wire.beginTransmission((uint8_t)LCD_I2C_ADDRESS);
      Wire.write((uint8_t)INIT_SCREEN);
      Wire.write(Marlin);
      Wire.endTransmission();
    #endif
  }
  else
    PanelDetected = 0;
  safe_delay(100);
}

bool MarlinUI::detected() {
  return PanelDetected;
}

void MarlinUI::clear_lcd() {
  if (!PanelDetected) return;
  lcd.clr_screen();
  lcd.clear_buffer();
}

int16_t MarlinUI::contrast; // Initialized by settings.load()

void MarlinUI::set_contrast(const int16_t value) {
  contrast = constrain(value, LCD_CONTRAST_MIN, LCD_CONTRAST_MAX);
  lcd.setContrast(contrast);
}

static void center_text_P(PGM_P pstart, uint8_t y) {
  uint8_t len = utf8_strlen_P(pstart);
  if (len < LCD_WIDTH)
    lcd.setCursor((LCD_WIDTH - len) / 2, y);
  else
    lcd.setCursor(0, y);
  lcd_put_u8str_P(pstart);
}

#if ENABLED(SHOW_BOOTSCREEN)

  void MarlinUI::show_bootscreen() {
    if (!PanelDetected) return;
    //
    // Show the Marlin logo, splash line1, and splash line 2
    //
    uint8_t indent = (LCD_WIDTH - 8) / 2;
    // symbols 217 (bottom right corner) and 218 (top left corner) are using for letters in some languages
    // and they should be moved to begining ASCII table as spetial symbols
    lcd.setCursor(indent, 0); lcd.write(TLC); lcd_put_u8str_P(PSTR("------"));  lcd.write(TRC);
    lcd.setCursor(indent, 1); lcd.write(LR);  lcd_put_u8str_P(PSTR("Marlin"));  lcd.write(LR);
    lcd.setCursor(indent, 2); lcd.write(BLC); lcd_put_u8str_P(PSTR("------"));  lcd.write(BRC);
    center_text_P(PSTR(SHORT_BUILD_VERSION), 3);
    center_text_P(PSTR(MARLIN_WEBSITE_URL), 4);
    picBits = ICON_LOGO;
    lcd.print_screen();
    safe_delay(1500);
  }

#endif // SHOW_BOOTSCREEN

void MarlinUI::draw_kill_screen() {
  if (!PanelDetected) return;
  lcd.clear_buffer();
  lcd.setCursor(0, 3);  lcd.write(COLOR_ERROR);
  lcd.setCursor((LCD_WIDTH - utf8_strlen(status_message)) / 2 + 1, 3);
  lcd_put_u8str(status_message);
  center_text_P(GET_TEXT(MSG_HALTED), 5);
  center_text_P(GET_TEXT(MSG_PLEASE_RESET), 6);
  lcd.print_screen();
}

//
// Before homing, blink '123' <-> '???'.
// Homed but unknown... '123' <-> '   '.
// Homed and known, display constantly.
//
FORCE_INLINE void _draw_axis_value(const AxisEnum axis, const char *value, const bool blink) {
  lcd.write('X' + uint8_t(axis));
  if (blink)
    lcd.print(value);
  else {
    if (!TEST(axis_homed, axis))
      while (const char c = *value++) lcd.write(c <= '.' ? c : '?');
    else {
      #if NONE(HOME_AFTER_DEACTIVATE, DISABLE_REDUCED_ACCURACY_WARNING)
        if (!TEST(axis_known_position, axis))
          lcd_put_u8str_P(axis == Z_AXIS ? PSTR("       ") : PSTR("    "));
        else
      #endif
          lcd_put_u8str(value);
    }
  }
}

FORCE_INLINE void _draw_heater_status(const heater_id_t heater_id, const char *prefix, const bool blink) {
  uint8_t pic_hot_bits;
  #if HAS_HEATED_BED
    const bool isBed = heater_id < 0;
    const float t1 = (isBed ? thermalManager.degBed() : thermalManager.degHotend(heater_id));
    const float t2 = (isBed ? thermalManager.degTargetBed() : thermalManager.degTargetHotend(heater_id));
  #else
    const float t1 = thermalManager.degHotend(heater_id);
    const float t2 = thermalManager.degTargetHotend(heater_id);
  #endif

  #if HOTENDS < 2
    if (heater_id == H_E0) {
      lcd.setCursor(2, 5);  lcd.print(prefix); //HE
      lcd.setCursor(1, 6);  lcd.print(i16tostr3rj(t1 + 0.5));
      lcd.setCursor(1, 7);
    }
    else {
      lcd.setCursor(6, 5);  lcd.print(prefix); //BED
      lcd.setCursor(6, 6);  lcd.print(i16tostr3rj(t1 + 0.5));
      lcd.setCursor(6, 7);
    }
  #else
    if (heater_id > H_BED) {
      lcd.setCursor(heater_id * 4, 5);  lcd.print(prefix); //HE1 or HE2 or HE3
      lcd.setCursor(heater_id * 4, 6);  lcd.print(i16tostr3rj(t1 + 0.5));
      lcd.setCursor(heater_id * 4, 7);
    }
    else {
      lcd.setCursor(13, 5);  lcd.print(prefix); //BED
      lcd.setCursor(13, 6);  lcd.print(i16tostr3rj(t1 + 0.5));
      lcd.setCursor(13, 7);
    }
  #endif // HOTENDS <= 1

  #if !HEATER_IDLE_HANDLER
    UNUSED(blink);
  #else
    if (!blink && thermalManager.heater_idle[thermalManager.idle_index_for_id(heater_id)].timed_out) {
      lcd.write(' ');
      if (t2 >= 10) lcd.write(' ');
      if (t2 >= 100) lcd.write(' ');
    }
    else
  #endif // !HEATER_IDLE_HANDLER
      lcd.print(i16tostr3rj(t2 + 0.5));

  switch (heater_id) {
    case H_BED: pic_hot_bits = ICON_BED;   break;
    case H_E0:  pic_hot_bits = ICON_TEMP1; break;
    case H_E1:  pic_hot_bits = ICON_TEMP2; break;
    case H_E2:  pic_hot_bits = ICON_TEMP3;
    default:    break;
  }

  if (t2) picBits |= pic_hot_bits;
  else    picBits &= ~pic_hot_bits;

  if (t1 > 50)  hotBits |= pic_hot_bits;
  else          hotBits &= ~pic_hot_bits;

  if (hotBits)  picBits |= ICON_HOT;
  else          picBits &= ~ICON_HOT;
}

#if HAS_PRINT_PROGRESS

  FORCE_INLINE void _draw_print_progress() {
    if (!PanelDetected) return;
    const uint8_t progress = ui._get_progress();
    #if ENABLED(SDSUPPORT)
      lcd_put_u8str_P(PSTR("SD"));
    #elif ENABLED(LCD_SET_PROGRESS_MANUALLY)
      lcd_put_u8str_P(PSTR("P:"));
    #endif
    if (progress)
      lcd.print(ui8tostr3rj(progress));
    else
      lcd_put_u8str_P(PSTR("---"));
    lcd.write('%');
  }

#endif // HAS_PRINT_PROGRESS

#if ENABLED(LCD_PROGRESS_BAR)

  void MarlinUI::draw_progress_bar(const uint8_t percent) {
    if (!PanelDetected) return;
    if (fb == &framebuffer[0] + LCD_WIDTH * 2) {  // For status screen
      lcd.write('%'); lcd.write(percent);
    }
    else { // For progress bar test
      lcd.setCursor(LCD_WIDTH / 2 - 2, MIDDLE_Y);
      lcd.print(i16tostr3rj(percent));  lcd.write('%');
      lcd.print_line();
      lcd.setCursor(0, MIDDLE_Y + 1);
      lcd.write('%'); lcd.write(percent);
      lcd.print_line();
    }
  }

#endif

void MarlinUI::draw_status_message(const bool blink) {
  if (!PanelDetected) return;
  lcd.setCursor(0, 3);
  #if BOTH(FILAMENT_LCD_DISPLAY, SDSUPPORT)

    // Alternate Status message and Filament display
    if (ELAPSED(millis(), next_filament_display)) {
      lcd_put_u8str_P(PSTR("Dia "));
      lcd.print(ftostr12ns(filament_width_meas));
      lcd_put_u8str_P(PSTR(" V"));
      lcd.print(i16tostr3rj(100.0 * (
          parser.volumetric_enabled
            ? planner.volumetric_area_nominal / planner.volumetric_multiplier[FILAMENT_SENSOR_EXTRUDER_NUM]
            : planner.volumetric_multiplier[FILAMENT_SENSOR_EXTRUDER_NUM]
        )
      ));
      lcd.write('%');
      return;
    }

  #endif // FILAMENT_LCD_DISPLAY && SDSUPPORT

  // Get the UTF8 character count of the string
  uint8_t slen = utf8_strlen(status_message);

  #if ENABLED(STATUS_MESSAGE_SCROLLING)

    static bool last_blink = false;

    // If the string fits into the LCD, just print it and do not scroll it
    if (slen <= LCD_WIDTH) {

      // The string isn't scrolling and may not fill the screen
      lcd_put_u8str(status_message);

      // Fill the rest with spaces
      while (slen < LCD_WIDTH) { lcd.write(' '); ++slen; }
    }
    else {
      // String is larger than the available space in screen.

      // Get a pointer to the next valid UTF8 character
      // and the string remaining length
      uint8_t rlen;
      const char *stat = status_and_len(rlen);
      lcd_put_u8str_max(stat, LCD_WIDTH);     // The string leaves space

      // If the remaining string doesn't completely fill the screen
      if (rlen < LCD_WIDTH) {
        lcd.write('.');                       // Always at 1+ spaces left, draw a dot
        uint8_t chars = LCD_WIDTH - rlen;     // Amount of space left in characters
        if (--chars) {                        // Draw a second dot if there's space
          lcd.write('.');
          if (--chars)
            lcd_put_u8str_max(status_message, chars); // Print a second copy of the message
        }
      }
      if (last_blink != blink) {
        last_blink = blink;
        advance_status_scroll();
      }
    }

  #else

    UNUSED(blink);

    // Just print the string to the LCD
    lcd_put_u8str_max(status_message, LCD_WIDTH);

    // Fill the rest with spaces if there are missing spaces
    while (slen < LCD_WIDTH) {
      lcd.write(' ');
      ++slen;
    }

  #endif
}

/**
Possible status screens:

Equal to 20x10 text LCD

|X 000 Y 000 Z 000.00|
|FR100% SD100% C--:--|
| Progress bar line  |
|Status message      |
|                    |
|  HE  BED  FAN      |
| ttc  ttc   %       | ttc - current temperature
| tts  tts  %%%      | tts - setted temperature, %%% - percent for FAN
| ICO  ICO  ICO  ICO | ICO - icon 48x48, placed in 2 text lines
| ICO  ICO  ICO  ICO | ICO /

or

|X 000 Y 000 Z 000.00|
|FR100% SD100% C--:--|
| Progress bar line  |
|Status message      |
|                    |
|HE1 HE2 HE3  BED ICO|
|ttc ttc ttc  ttc ICO|
|tts tts tts  tts %%%|
|ICO ICO ICO  ICO ICO|
|ICO ICO ICO  ICO ICO|

or

Equal to 24x10 text LCD

|X 000 Y 000 Z 000.00    |
|FR100%   SD100%   C--:--|
|   Progress bar line    |
|Status message          |
|                        |
|HE1 HE2 HE3  BED FAN    |
|ttc ttc ttc  ttc  %     |
|tts tts tts  tts %%%    |
|ICO ICO ICO  ICO ICO ICO|
|ICO ICO ICO  ICO ICO ICO|
*/

void MarlinUI::draw_status_screen() {
  if (!PanelDetected) return;

  const bool blink = get_blink();

  lcd.clear_buffer();

  //
  // Line 1 - XYZ coordinates
  //

  lcd.setCursor(0, 0);
  _draw_axis_value(X_AXIS, ftostr4sign(LOGICAL_X_POSITION(current_position[X_AXIS])), blink);  lcd.write(' ');
  _draw_axis_value(Y_AXIS, ftostr4sign(LOGICAL_Y_POSITION(current_position[Y_AXIS])), blink);  lcd.write(' ');
  _draw_axis_value(Z_AXIS, ftostr52sp(LOGICAL_Z_POSITION(current_position[Z_AXIS])), blink);

  #if HAS_LEVELING && !HAS_HEATED_BED
    lcd.write(planner.leveling_active || blink ? '_' : ' ');
  #endif

  //
  // Line 2 - feedrate, , time
  //

  lcd.setCursor(0, 1);
  lcd_put_u8str_P(PSTR("FR")); lcd.print(i16tostr3rj(feedrate_percentage)); lcd.write('%');

  #if BOTH(SDSUPPORT, HAS_PRINT_PROGRESS)
    lcd.setCursor(LCD_WIDTH / 2 - 3, 1);
    _draw_print_progress();
  #endif

  char buffer[10];
  duration_t elapsed = print_job_timer.duration();
  uint8_t len = elapsed.toDigital(buffer);

  lcd.setCursor((LCD_WIDTH - 1) - len, 1);
  lcd.write(0x07); lcd.print(buffer); // LCD_CLOCK_CHAR

  //
  // Line 3 - progressbar
  //

  lcd.setCursor(0, 2);
  #if ENABLED(LCD_PROGRESS_BAR)
    draw_progress_bar(_get_progress());
  #else
    lcd.write('%'); lcd.write(0);
  #endif

  //
  // Line 4 - Status Message (which may be a Filament display)
  //

  draw_status_message(blink);

  //
  // Line 5
  //

  #if HOTENDS <= 1 || (HOTENDS <= 2 && !HAS_HEATED_BED)
    #if DUAL_MIXING_EXTRUDER
      lcd.setCursor(0, 4);
      // Two-component mix / gradient instead of XY
      char mixer_messages[12];
      const char *mix_label;
      #if ENABLED(GRADIENT_MIX)
        if (mixer.gradient.enabled) {
          mixer.update_mix_from_gradient();
          mix_label = "Gr";
        }
        else
      #endif
        {
          mixer.update_mix_from_vtool();
          mix_label = "Mx";
        }
      sprintf_P(mixer_messages, PSTR("%s %d;%d%% "), mix_label, int(mixer.mix[0]), int(mixer.mix[1]));
      lcd_put_u8str(mixer_messages);
    #endif
  #endif

  //
  // Line 6..8 Temperatures, FAN
  //

  #if HOTENDS < 2
    _draw_heater_status(H_E0, "HE", blink);    // Hotend Temperature
  #else
    _draw_heater_status(H_E0, "HE1", blink);   // Hotend 1 Temperature
    _draw_heater_status(H_E1, "HE2", blink);   // Hotend 2 Temperature
    #if HOTENDS > 2
      _draw_heater_status(H_E2, "HE3", blink); // Hotend 3 Temperature
    #endif
  #endif // HOTENDS <= 1

  #if HAS_HEATED_BED
    #if HAS_LEVELING
      _draw_heater_status(H_BED, (planner.leveling_active && blink ? "___" : "BED"), blink);
    #else
      _draw_heater_status(H_BED, "BED", blink);
    #endif
  #endif // HAS_HEATED_BED

  #if FAN_COUNT > 0
    uint16_t spd = thermalManager.fan_speed[0];

    #if ENABLED(ADAPTIVE_FAN_SLOWING)
      if (!blink) spd = thermalManager.scaledFanSpeed(0, spd);
    #endif

    uint16_t per = thermalManager.fanPercent(spd);
    #if HOTENDS < 2
      #define FANX 11
    #else
      #define FANX 17
    #endif
    lcd.setCursor(FANX, 5); lcd_put_u8str_P(PSTR("FAN"));
    lcd.setCursor(FANX + 1, 6); lcd.write('%');
    lcd.setCursor(FANX, 7);
    lcd.print(i16tostr3rj(per));

    if (TERN0(HAS_FAN0, thermalManager.fan_speed[0]) || TERN0(HAS_FAN1, thermalManager.fan_speed[1]) || TERN0(HAS_FAN2, thermalManager.fan_speed[2]))
      picBits |= ICON_FAN;
    else
      picBits &= ~ICON_FAN;

  #endif // FAN_COUNT > 0

  //
  // Line 9, 10 - icons
  //
  lcd.print_screen();
}

#if HAS_LCD_MENU

  #include "../menu/menu.h"

  #if ENABLED(ADVANCED_PAUSE_FEATURE)

    void MarlinUI::draw_hotend_status(const uint8_t row, const uint8_t extruder) {
      if (!PanelDetected) return;
      lcd.setCursor((LCD_WIDTH - 14) / 2, row + 1);
      lcd.write(0x02);  lcd_put_u8str_P(" E"); lcd.write('1' + extruder); lcd.write(' ');
      lcd.print(i16tostr3rj(thermalManager.degHotend(extruder))); lcd.write(0x01);  lcd.write('/');
      lcd.print(i16tostr3rj(thermalManager.degTargetHotend(extruder)));  lcd.write(0x01);
      lcd.print_line();
    }

  #endif // ADVANCED_PAUSE_FEATURE

  // Draw a static item with no left-right margin required. Centered by default.
  void MenuItem_static::draw(const uint8_t row, PGM_P const pstr, const uint8_t style/*=SS_DEFAULT*/, const char * const valstr/*=nullptr*/) {
    if (!PanelDetected) return;
    uint8_t n = LCD_WIDTH;
    lcd.setCursor(0, row);
    if ((style & SS_CENTER) && !valstr) {
      int8_t pad = (LCD_WIDTH - utf8_strlen_P(pstr)) / 2;
      while (--pad >= 0) { lcd.write(' '); n--; }
    }
    n = lcd_put_u8str_ind_P(pstr, itemIndex, itemString, n);
    if (valstr) n -= lcd_put_u8str_max(valstr, n);
    for (; n; --n) lcd.write(' ');
    lcd.print_line();
  }

  // Draw a generic menu item with pre_char (if selected) and post_char
  void MenuItemBase::_draw(const bool sel, const uint8_t row, PGM_P const pstr, const char pre_char, const char post_char) {
    if (!PanelDetected) return;
    lcd.setCursor(0, row);
    lcd.write(sel ? pre_char : ' ');
    uint8_t n = lcd_put_u8str_ind_P(pstr, itemIndex, itemString, LCD_WIDTH - 2);
    for (; n; --n) lcd.write(' ');
    lcd.write(post_char);
    lcd.print_line();
  }

  // Draw a menu item with a (potentially) editable value
  void MenuEditItemBase::draw(const bool sel, const uint8_t row, PGM_P const pstr, const char* const data, const bool pgm) {
    if (!PanelDetected) return;
    const uint8_t vlen = data ? (pgm ? utf8_strlen_P(data) : utf8_strlen(data)) : 0;
    lcd.setCursor(0, row);
    lcd.write(sel ? LCD_STR_ARROW_RIGHT[0] : ' ');
    uint8_t n = lcd_put_u8str_ind_P(pstr, itemIndex, itemString, LCD_WIDTH - 2 - vlen);
    if (vlen) {
      lcd.write(':');
      for (; n; --n) lcd.write(' ');
      if (pgm) lcd_put_u8str_P(data); else lcd_put_u8str(data);
    }
    lcd.print_line();
  }

  // Low-level draw_edit_screen can be used to draw an edit screen from anyplace
  // This line moves to the last line of the screen for UBL plot screen on the panel side
  void MenuEditItemBase::draw_edit_screen(PGM_P const pstr, const char* const value/*=nullptr*/) {
    if (!PanelDetected) return;
    ui.encoder_direction_normal();
    lcd.setCursor(0, MIDDLE_Y);
    lcd.write(COLOR_EDIT);
    lcd_put_u8str_P(pstr);
    if (value) {
      lcd.write(':');
      lcd.setCursor((LCD_WIDTH - 1) - (utf8_strlen(value) + 1), MIDDLE_Y);  // Right-justified, padded by spaces
      lcd.write(' ');     // Overwrite char if value gets shorter
      lcd.print(value);
      lcd.write(' ');
      lcd.print_line();
    }
  }

  // The Select Screen presents a prompt and two "buttons"
  void MenuItem_confirm::draw_select_screen(PGM_P const yes, PGM_P const no, const bool yesno, PGM_P const pref, const char * const string, PGM_P const suff) {
    if (!PanelDetected) return;
    ui.draw_select_screen_prompt(pref, string, suff);
    lcd.setCursor(0, MIDDLE_Y);
    lcd.write(COLOR_EDIT);
    lcd.write(yesno ? ' ' : '['); lcd_put_u8str_P(no); lcd.write(yesno ? ' ' : ']');
    lcd.setCursor(LCD_WIDTH - utf8_strlen_P(yes) - 3, MIDDLE_Y);
    lcd.write(yesno ? '[' : ' '); lcd_put_u8str_P(yes); lcd.write(yesno ? ']' : ' ');
    lcd.print_line();
  }

  #if ENABLED(SDSUPPORT)

    void MenuItem_sdbase::draw(const bool sel, const uint8_t row, PGM_P const, CardReader &theCard, const bool isDir) {
      if (!PanelDetected) return;
      lcd.setCursor(0, row);
      lcd.write(sel ? LCD_STR_ARROW_RIGHT[0] : ' ');
      constexpr uint8_t maxlen = LCD_WIDTH - 2;
      uint8_t n = maxlen - lcd_put_u8str_max(ui.scrolled_filename(theCard, maxlen, row, sel), maxlen);
      for (; n; --n) lcd.write(' ');
      lcd.write(isDir ? LCD_STR_FOLDER[0] : ' ');
      lcd.print_line();
    }

  #endif // SDSUPPORT

  #if ENABLED(LCD_HAS_STATUS_INDICATORS)

    void MarlinUI::update_indicators() {}

  #endif  // LCD_HAS_STATUS_INDICATORS

  #if ENABLED(AUTO_BED_LEVELING_UBL)
    /**
     * Map screen:
     * |/---------\ (00,00) |
     * || . . . . | X:000.00|
     * || . . . . | Y:000.00|
     * || . . . . | Z:00.000|
     * || . . . . |         |
     * || . . . . |         |
     * || . . . . |         |
     * |+---------/         |
     * |                    |
     * |____________________|
     */
    void MarlinUI::ubl_plot(const uint8_t x_plot, const uint8_t y_plot) {
      if (!PanelDetected) return;

      #define _LCD_W_POS 12

      lcd.clear_buffer();

      //print only top left corner. All frame with grid points will be printed by panel
      lcd.setCursor(0, 0);
      *fb++ = TLC;   //top left corner - marker for plot parameters
      *fb = (GRID_MAX_POINTS_X << 4) + GRID_MAX_POINTS_Y; //set mesh size

      // Print plot position
      lcd.setCursor(_LCD_W_POS, 0);
      *fb++ = '(';  lcd.print(i16tostr3left(x_plot));
      *fb++ = ',';  lcd.print(i16tostr3left(y_plot)); *fb = ')';

      // Show all values
      lcd.setCursor(_LCD_W_POS, 1); lcd_put_u8str_P(PSTR("X:"));
      lcd.print(ftostr52(LOGICAL_X_POSITION(pgm_read_float(&ubl._mesh_index_to_xpos[x_plot]))));
      lcd.setCursor(_LCD_W_POS, 2); lcd_put_u8str_P(PSTR("Y:"));
      lcd.print(ftostr52(LOGICAL_Y_POSITION(pgm_read_float(&ubl._mesh_index_to_ypos[y_plot]))));

      // Show the location value
      lcd.setCursor(_LCD_W_POS, 3); lcd_put_u8str_P(PSTR("Z:"));

      if (!isnan(ubl.z_values[x_plot][y_plot]))
        lcd.print(ftostr43sign(ubl.z_values[x_plot][y_plot]));
      else
        lcd_put_u8str_P(PSTR(" -----"));

      center_text_P(GET_TEXT(MSG_UBL_FINE_TUNE_MESH), 8);

      lcd.print_screen();
    }

  #endif // AUTO_BED_LEVELING_UBL

#endif // HAS_LCD_MENU

#endif // IS_TFTGLCD_PANEL
