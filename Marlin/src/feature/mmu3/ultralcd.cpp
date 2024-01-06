
#include "ultralcd.h"
#include "src/lcd/menu/menu_item.h"
#include "src/gcode/gcode.h"
#include "src/lcd/marlinui.h"
#include "mmu2.h"
#include "mmu_hw/errors_list.h"


//! @brief Show a two-choice prompt on the last line of the LCD
//! @param selected Show first choice as selected if true, the second otherwise
//! @param first_choice text caption of first possible choice
//! @param second_choice text caption of second possible choice
//! @param second_col column on LCD where second choice is rendered.
//! @param third_choice text caption of third, optional, choice.
void lcd_show_choices_prompt_P(uint8_t selected, const char *first_choice, const char *second_choice, uint8_t second_col, const char *third_choice)
{
    lcd_put_lchar(0, 3, selected == LCD_LEFT_BUTTON_CHOICE ? '>': ' ');
    lcd_put_u8str(F(first_choice));
    lcd_put_lchar(second_col, 3, selected == LCD_MIDDLE_BUTTON_CHOICE ? '>': ' ');
    lcd_put_u8str(F(second_choice));
    if (third_choice) {
        lcd_put_lchar(18, 3, selected == LCD_RIGHT_BUTTON_CHOICE ? '>': ' ');
        lcd_put_u8str(F(third_choice));
    }
}

void lcd_space(uint8_t n)
{
	while (n--) lcd_put_lchar(' ');
}

// Print extruder status (5 chars total)
// Scenario 1: "F?"
//              There is no filament loaded and no tool change is in progress
// Scenario 2: "F[nr.]"
//              [nr.] ranges from 1 to 5.
//              Shows which filament is loaded. No tool change is in progress
// Scenario 3: "?>[nr.]"
//              [nr.] ranges from 1 to 5.
//              There is no filament currently loaded, but [nr.] is currently being loaded via tool change
// Scenario 4: "[nr.]>?"
//              [nr.] ranges from 1 to 5.
//              This scenario indicates a bug in the firmware if ? is on the right side
// Scenario 5: "[nr1.]>[nr2.]"
//              [nr1.] ranges from 1 to 5.
//              [nr2.] ranges from 1 to 5.
//              Filament [nr1.] was loaded, but [nr2.] is currently being loaded via tool change
// Scenario 6: "?>?"
//              This scenario should not be possible and indicates a bug in the firmware
uint8_t lcdui_print_extruder(void) {
    uint8_t chars = 1;
    lcd_space(1);
    if (MMU2::mmu2.get_current_tool() == MMU2::mmu2.get_tool_change_tool()) {
        lcd_put_lchar('F');
        lcd_put_lchar(MMU2::mmu2.get_current_tool() == (uint8_t)MMU2::FILAMENT_UNKNOWN ? '?' : MMU2::mmu2.get_current_tool() + '1');
        chars += 2;
    } else {
        lcd_put_lchar(MMU2::mmu2.get_current_tool() == (uint8_t)MMU2::FILAMENT_UNKNOWN ? '?' : MMU2::mmu2.get_current_tool() + '1');
        lcd_put_lchar('>');
        lcd_put_lchar(MMU2::mmu2.get_tool_change_tool() == (uint8_t)MMU2::FILAMENT_UNKNOWN ? '?' : MMU2::mmu2.get_tool_change_tool() + '1');
        chars += 3;
    }
    return chars;
}

bool pgm_is_whitespace(const char *c_addr)
{
    const char c = pgm_read_byte(c_addr);
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

bool pgm_is_interpunction(const char *c_addr)
{
    const char c = pgm_read_byte(c_addr);
    return c == '.' || c == ',' || c == ':'|| c == ';' || c == '?' || c == '!' || c == '/';
}


/**
 * @brief show full screen message
 *
 * This function is non-blocking
 * @param msg message to be displayed from PROGMEM
 * @return rest of the text (to be displayed on next page)
 */
static const char* lcd_display_message_fullscreen_nonBlocking_P(const char *msg)
{
    const char *msgend = msg;
    bool multi_screen = false;
    START_SCREEN();
    for (uint8_t row = 0; row < LCD_HEIGHT; ++ row) {
        SETCURSOR(0, row);

        // Previous row ended with a complete word, so the first character in the
        // next row is a whitespace. We can skip the whitespace on a new line.
        if (pgm_is_whitespace(msg) && ++msg == nullptr)
        {
            // End of the message.
            break;
        }

        uint8_t linelen = (strlen_P(msg) > LCD_WIDTH) ? LCD_WIDTH : strlen_P(msg);
        const char *msgend2 = msg + linelen;
        msgend = msgend2;
        if (row == 3 && linelen == LCD_WIDTH) {
            // Last line of the display, full line shall be displayed.
            // Find out, whether this message will be split into multiple screens.
            multi_screen = pgm_read_byte(msgend) != 0;
            if (multi_screen)
                msgend = (msgend2 -= 2);
        }
        if (pgm_read_byte(msgend) != 0 && !pgm_is_whitespace(msgend) && !pgm_is_interpunction(msgend)) {
            // Splitting a word. Find the start of the current word.
            while (msgend > msg && ! pgm_is_whitespace(msgend - 1))
                -- msgend;
            if (msgend == msg)
                // Found a single long word, which cannot be split. Just cut it.
                msgend = msgend2;
        }
        for (; msg < msgend; ++ msg) {
            char c = char(pgm_read_byte(msg));
            if (c == '\n') {
                // Abort early if '\n' is encontered.
                // This character is used to force the following words to be printed on the next line.
                break;
            }
            lcd_put_lchar(c);
        }
    }

    if (multi_screen) {
        // Display the double down arrow.
        lcd_put_lchar(19, 3, LCD_STR_ARROW_2_DOWN[0]);
    }
    END_SCREEN();
    ui.refresh(LCDVIEW_CALL_REDRAW_NEXT);

    return multi_screen ? msgend : NULL;
}


const char* lcd_display_message_fullscreen_P(const char *msg)
{
    // Disable update of the screen by the usual lcd_update(0) routine.
    ui.lcdDrawUpdate = LCDViewAction::LCDVIEW_NONE;
    ui.clear_lcd();
    return lcd_display_message_fullscreen_nonBlocking_P(msg);
}


/**
 * @brief show full screen message and wait
 *
 * This function is blocking.
 * @param msg message to be displayed from PROGMEM
 */
void lcd_show_fullscreen_message_and_wait_P(const char *msg)
{
    LcdUpdateDisabler lcdUpdateDisabler;
    const char *msg_next = lcd_display_message_fullscreen_P(msg);
    bool multi_screen = msg_next != NULL;
    ui.lcd_clicked = false;
    KEEPALIVE_STATE(PAUSED_FOR_USER);
    // Until confirmed by a button click.
    for (;;) {
        if (msg_next == NULL) {
            // Display the confirm char.
            lcd_put_lchar(19, 3, LCD_STR_CONFIRM[0]);
        }
        // Wait for 5 seconds before displaying the next text.
        for (uint8_t i = 0; i < 100; ++ i) {
            idle(true);
            safe_delay(50);
            if (ui.lcd_clicked) {
                if (msg_next == NULL) {
                    KEEPALIVE_STATE(IN_HANDLER);
                    return;
                } else {
                    break;
                }
            }
        }
        if (multi_screen) {
            if (msg_next == NULL)
                msg_next = msg;
            msg_next = lcd_display_message_fullscreen_P(msg_next);
        }
    }
}

void lcd_insert_char_into_status(uint8_t position, const char message)
{
    if (position > LCD_WIDTH - 1) return;
    int size = ui.status_message.length();
    char *str = ui.status_message.buffer();
    str[position] = message;
    ui.lcdDrawUpdate = LCDViewAction::LCDVIEW_REDRAW_NOW; // force redraw
}