#pragma once

#include "src/lcd/marlinui.h"


#define LCD_LEFT_BUTTON_CHOICE 0
#define LCD_MIDDLE_BUTTON_CHOICE 1
#define LCD_RIGHT_BUTTON_CHOICE 2

#define LCD_STR_ARROW_2_DOWN "\x88"
#define LCD_STR_CONFIRM      "\x89"
#define LCD_STR_SOLID_BLOCK  "\xFF"  //from the default character set



//! @brief Helper class to temporarily disable LCD updates
//!
//! When constructed (on stack), original state state of lcd_update_enabled is stored
//! and LCD updates are disabled.
//! When destroyed (gone out of scope), original state of LCD update is restored.
//! It has zero overhead compared to storing bool saved = lcd_update_enabled
//! and calling lcd_update_enable(false) and lcd_update_enable(saved).
class LcdUpdateDisabler
{
public:
    LcdUpdateDisabler(): m_updateEnabled(ui.lcdDrawUpdate)
    {
        ui.lcdDrawUpdate = TERN(HAS_WIRED_LCD, LCDViewAction::LCDVIEW_NONE, nullptr);
    }
    ~LcdUpdateDisabler()
    {
        ui.lcdDrawUpdate = m_updateEnabled;
    }

private:
    LCDViewAction m_updateEnabled;
};


bool pgm_is_whitespace(const char *c_addr);
bool pgm_is_interpunction(const char *c_addr);
static const char* lcd_display_message_fullscreen_nonBlocking_P(const char *msg);
extern const char* lcd_display_message_fullscreen_P(const char *msg);
void lcd_show_choices_prompt_P(uint8_t selected, const char *first_choice, const char *second_choice, uint8_t second_col, const char *third_choice = nullptr);
extern void lcd_show_fullscreen_message_and_wait_P(const char *msg);
uint8_t lcdui_print_extruder(void);
extern void lcd_space(uint8_t n);
void lcd_insert_char_into_status(uint8_t position, const char message);