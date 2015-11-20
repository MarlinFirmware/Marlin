///////////////////////////////////////////////////////////////////////////////
/// \file GuiManager.h
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza
///         Joaquin Herrero
///
/// \brief Management class for the UI.
///
/// Copyright (c) 2015 BQ - Mundo Reader S.L.
/// http://www.bq.com
///
/// This file is free software; you can redistribute it and/or modify
/// it under the terms of either the GNU General Public License version 2 or
/// later or the GNU Lesser General Public License version 2.1 or later, both
/// as published by the Free Software Foundation.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
/// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
/// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
/// DEALINGS IN THE SOFTWARE.
///////////////////////////////////////////////////////////////////////////////

#ifndef GUI_MANAGER_H
#define GUI_MANAGER_H

#include <inttypes.h>

//
// Defines
//
#  ifndef ENCODER_STEPS_PER_MENU_ITEM
#define ENCODER_STEPS_PER_MENU_ITEM 2
#  endif // ENCODER_STEPS_PER_MENU_ITEM

#define LCD_TIMEOUT_STATUS      30000
#define LCD_REFRESH              10
#define LCD_REFRESH_LIMIT          60
#define LCD_MOVE_RESIDENCY_TIME   500

#define NO_UPDATE_SCREEN            0
#define UPDATE_SCREEN               1
#define CLEAR_AND_UPDATE_SCREEN     2

#define MAX_WINDOW_SIZE LCD_HEIGHT
#define MAX_CACHE_SIZE LCD_HEIGHT+2

//
// Typedefs
//

//
// TODO: Review these variables
//
extern int plaPreheatHotendTemp;
extern int plaPreheatHPBTemp;
extern int plaPreheatFanSpeed;

extern int absPreheatHotendTemp;
extern int absPreheatHPBTemp;
extern int absPreheatFanSpeed;

extern bool cancel_heatup;
// END TODO

//
// LCD API
//
void lcd_init();
void lcd_update(bool force = false);
void lcd_emergency_stop();

// Get and clear trigger functions
bool lcd_get_button_updated();
bool lcd_get_encoder_updated();
bool lcd_get_button_clicked();
uint8_t lcd_get_encoder_right();
uint8_t lcd_get_encoder_left();
void lcd_clear_triggered_flags();

// Enable/disable functions
void lcd_enable_button();
void lcd_disable_button();
void lcd_enable_encoder();
void lcd_disable_encoder();
void lcd_enable_display_timeout();
void lcd_disable_display_timeout();

void lcd_enable_interrupt();
void lcd_disable_interrupt();

// Control flow functions
void lcd_wizard_set_page(uint8_t page);

// Temporal
void lcd_beep();
void lcd_beep_ms(uint16_t ms);
void lcd_beep_hz_ms(uint16_t frequency, uint16_t ms);
void lcd_set_refresh(uint8_t mode);
// uint8_t lcd_get_encoder_position();

#ifdef DOGLCD
	extern int lcd_contrast;
	void lcd_setcontrast(uint8_t value);
#endif

// Alert/status messages
void lcd_setstatus(const char* message);
void lcd_setstatuspgm(const char* message);
void lcd_setalertstatuspgm(const char* message);
void lcd_reset_alert_level();

// Macros
#define LCD_CLICKED lcd_get_button_clicked()
#define LCD_MESSAGEPGM(x) do { lcd_setstatuspgm(PSTR(x)); lcd_set_refresh(CLEAR_AND_UPDATE_SCREEN); } while (0)
#define LCD_ALERTMESSAGEPGM(x) lcd_setalertstatuspgm(PSTR(x))

void draw_status_screen();
void draw_wizard_change_filament();



// Print utilities
char *itostr2(const uint8_t &x);
char *itostr31(const int &xx);
char *itostr3(const int &xx);
char *itostr3left(const int &xx);
char *itostr4(const int &xx);

char *ftostr3(const float &x);
char *ftostr31ns(const float &x); // float to string without sign character
char *ftostr31(const float &x);
char *ftostr32(const float &x);
char *ftostr5(const float &x);
char *ftostr51(const float &x);
char *ftostr52(const float &x);

#endif // GUI_MANAGER_H
