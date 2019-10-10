/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

//
// Mixer Menu
//

#include "../../inc/MarlinConfigPre.h"

#if HAS_LCD_MENU && ENABLED(MIXING_EXTRUDER)

#include "menu.h"
#include "../../feature/mixing.h"

#include "../dogm/ultralcd_DOGM.h"
#include "../ultralcd.h"
#include "../lcdprint.h"

#define CHANNEL_MIX_EDITING !DUAL_MIXING_EXTRUDER

#if ENABLED(GRADIENT_MIX)

  void lcd_mixer_gradient_z_start_edit() {
    ui.defer_status_screen();
    ENCODER_RATE_MULTIPLY(true);
    if (ui.encoderPosition != 0) {
      mixer.gradient.start_z += float(int16_t(ui.encoderPosition)) * 0.1;
      ui.encoderPosition = 0;
      NOLESS(mixer.gradient.start_z, 0);
      NOMORE(mixer.gradient.start_z, Z_MAX_POS);
    }
    if (ui.should_draw()) {
      char tmp[21];
      strcpy_P(tmp, GET_TEXT(MSG_START_Z));
      sprintf_P(tmp + strlen(tmp), PSTR(": %4d.%d mm"), int(mixer.gradient.start_z), int(mixer.gradient.start_z * 10) % 10);
      SETCURSOR(2, (LCD_HEIGHT - 1) / 2);
      lcd_put_u8str(tmp);
    }

    if (ui.lcd_clicked) {
      if (mixer.gradient.start_z > mixer.gradient.end_z)
        mixer.gradient.end_z = mixer.gradient.start_z;
      mixer.refresh_gradient();
      ui.goto_previous_screen();
    }
  }

  void lcd_mixer_gradient_z_end_edit() {
    ui.defer_status_screen();
    ENCODER_RATE_MULTIPLY(true);
    if (ui.encoderPosition != 0) {
      mixer.gradient.end_z += float(int16_t(ui.encoderPosition)) * 0.1;
      ui.encoderPosition = 0;
      NOLESS(mixer.gradient.end_z, 0);
      NOMORE(mixer.gradient.end_z, Z_MAX_POS);
    }

    if (ui.should_draw()) {
      char tmp[21];
      strcpy_P(tmp, GET_TEXT(MSG_END_Z));
      sprintf_P(tmp + strlen(tmp), PSTR(": %4d.%d mm"), int(mixer.gradient.end_z), int(mixer.gradient.end_z * 10) % 10);
      SETCURSOR(2, (LCD_HEIGHT - 1) / 2);
      lcd_put_u8str(tmp);
    }

    if (ui.lcd_clicked) {
      if (mixer.gradient.end_z < mixer.gradient.start_z)
        mixer.gradient.start_z = mixer.gradient.end_z;
      mixer.refresh_gradient();
      ui.goto_previous_screen();
    }
  }

  void lcd_mixer_edit_gradient_menu() {
    START_MENU();
    BACK_ITEM(MSG_MIXER);

    EDIT_ITEM(int8, MSG_START_VTOOL, &mixer.gradient.start_vtool, 0, MIXING_VIRTUAL_TOOLS - 1, mixer.refresh_gradient);
    EDIT_ITEM(int8, MSG_END_VTOOL, &mixer.gradient.end_vtool, 0, MIXING_VIRTUAL_TOOLS - 1, mixer.refresh_gradient);

    #if ENABLED(GRADIENT_VTOOL)
      EDIT_ITEM(int8, MSG_GRADIENT_ALIAS, &mixer.gradient.vtool_index, 0, MIXING_VIRTUAL_TOOLS - 1, mixer.refresh_gradient);
    #endif

    char tmp[18];

    SUBMENU(MSG_START_Z, lcd_mixer_gradient_z_start_edit);
    MENU_ITEM_ADDON_START(9);
      sprintf_P(tmp, PSTR("%4d.%d mm"), int(mixer.gradient.start_z), int(mixer.gradient.start_z * 10) % 10);
      lcd_put_u8str(tmp);
    MENU_ITEM_ADDON_END();

    SUBMENU(MSG_END_Z, lcd_mixer_gradient_z_end_edit);
    MENU_ITEM_ADDON_START(9);
      sprintf_P(tmp, PSTR("%4d.%d mm"), int(mixer.gradient.end_z), int(mixer.gradient.end_z * 10) % 10);
      lcd_put_u8str(tmp);
    MENU_ITEM_ADDON_END();

    END_MENU();
  }

#endif // GRADIENT_MIX

static uint8_t v_index;

#if DUAL_MIXING_EXTRUDER
  void _lcd_draw_mix(const uint8_t y) {
    char tmp[10]; // "100%_100%"
    SETCURSOR(2, y);
    lcd_put_u8str_P(GET_TEXT(MSG_MIX));
    SETCURSOR(LCD_WIDTH - 9, y);
    sprintf_P(tmp, PSTR("%3d%% %3d%%"), int(mixer.mix[0]), int(mixer.mix[1]));
    lcd_put_u8str(tmp);
  }
#endif

void _lcd_mixer_select_vtool() {
  mixer.T(v_index);
  #if DUAL_MIXING_EXTRUDER
    _lcd_draw_mix(LCD_HEIGHT - 1);
  #endif
}

#if CHANNEL_MIX_EDITING

  void _lcd_mixer_cycle_mix() {
    uint16_t bits = 0;
    MIXER_STEPPER_LOOP(i) if (mixer.collector[i]) SBI(bits, i);
    bits = (bits + 1) & (_BV(MIXING_STEPPERS) - 1);
    if (!bits) ++bits;
    MIXER_STEPPER_LOOP(i) mixer.collector[i] = TEST(bits, i) ? 10.0f : 0.0f;
    ui.refresh();
  }

  void _lcd_mixer_commit_vtool() {
    mixer.normalize();
    ui.goto_previous_screen();
  }

#endif

void lcd_mixer_mix_edit() {

  #if CHANNEL_MIX_EDITING

    #define EDIT_COLOR(N) EDIT_ITEM_FAST(float52, MSG_MIX_COMPONENT_##N, &mixer.collector[N-1], 0, 10);

    START_MENU();
    BACK_ITEM(MSG_MIXER);
    EDIT_COLOR(1);
    EDIT_COLOR(2);
    #if MIXING_STEPPERS > 2
      EDIT_COLOR(3);
      #if MIXING_STEPPERS > 3
        EDIT_COLOR(4);
        #if MIXING_STEPPERS > 4
          EDIT_COLOR(5);
          #if MIXING_STEPPERS > 5
            EDIT_COLOR(6);
          #endif
        #endif
      #endif
    #endif
    ACTION_ITEM(MSG_CYCLE_MIX, _lcd_mixer_cycle_mix);
    ACTION_ITEM(MSG_COMMIT_VTOOL, _lcd_mixer_commit_vtool);
    END_MENU();

  #elif DUAL_MIXING_EXTRUDER

    if (ui.encoderPosition != 0) {
      mixer.mix[0] += int16_t(ui.encoderPosition);
      ui.encoderPosition = 0;
      if (mixer.mix[0] < 0) mixer.mix[0] += 101;
      if (mixer.mix[0] > 100) mixer.mix[0] -= 101;
      mixer.mix[1] = 100 - mixer.mix[0];
    }
    _lcd_draw_mix((LCD_HEIGHT - 1) / 2);

    if (ui.lcd_clicked) {
      mixer.update_vtool_from_mix();
      ui.goto_previous_screen();
    }

  #else

    START_MENU();
    BACK_ITEM(MSG_MIXER);
    END_MENU();

  #endif
}

#if DUAL_MIXING_EXTRUDER

  //
  // Toggle Dual-Mix
  //
  inline void _lcd_mixer_toggle_mix() {
    mixer.mix[0] = mixer.mix[0] == 100 ? 0 : 100;
    mixer.mix[1] = 100 - mixer.mix[0];
    mixer.update_vtool_from_mix();
    ui.refresh(LCDVIEW_CALL_REDRAW_NEXT);
  }

#endif

#if CHANNEL_MIX_EDITING

  //
  // Prepare and Edit Mix
  //
  inline void _lcd_goto_mix_edit() {
    mixer.refresh_collector(10, v_index);
    ui.goto_screen(lcd_mixer_mix_edit);
    lcd_mixer_mix_edit();
  }

#endif

#if ENABLED(GRADIENT_MIX)
  //
  // Reverse Gradient
  //
  inline void _lcd_mixer_reverse_gradient() {
    const uint8_t sv = mixer.gradient.start_vtool;
    mixer.gradient.start_vtool = mixer.gradient.end_vtool;
    mixer.gradient.end_vtool = sv;
    mixer.refresh_gradient();
    ui.refresh(LCDVIEW_CALL_REDRAW_NEXT);
  }
#endif

//
// Reset All V-Tools
//
void menu_mixer_vtools_reset_confirm() {
  do_select_screen(
    GET_TEXT(MSG_BUTTON_RESET), GET_TEXT(MSG_BUTTON_CANCEL),
    []{
      mixer.reset_vtools();
      LCD_MESSAGEPGM(MSG_VTOOLS_RESET);
      ui.return_to_status();
    },
    ui.goto_previous_screen,
    GET_TEXT(MSG_RESET_VTOOLS), nullptr, PSTR("?")
  );
}

void menu_mixer() {
  START_MENU();
  BACK_ITEM(MSG_MAIN);

  v_index = mixer.get_current_vtool();
  EDIT_ITEM(uint8, MSG_ACTIVE_VTOOL, &v_index, 0, MIXING_VIRTUAL_TOOLS - 1, _lcd_mixer_select_vtool
    #if DUAL_MIXING_EXTRUDER
      , true
    #endif
  );

  #if DUAL_MIXING_EXTRUDER
  {
    char tmp[10];
    SUBMENU(MSG_MIX, lcd_mixer_mix_edit);
    MENU_ITEM_ADDON_START(10);
      mixer.update_mix_from_vtool();
      sprintf_P(tmp, PSTR("%3d;%3d%%"), int(mixer.mix[0]), int(mixer.mix[1]));
      lcd_put_u8str(tmp);
    MENU_ITEM_ADDON_END();
    ACTION_ITEM(MSG_TOGGLE_MIX, _lcd_mixer_toggle_mix);
  }
  #else
    SUBMENU(MSG_MIX, _lcd_goto_mix_edit);
  #endif

  SUBMENU(MSG_RESET_VTOOLS, menu_mixer_vtools_reset_confirm);

  #if ENABLED(GRADIENT_MIX)
  {
    char tmp[13];
    SUBMENU(MSG_GRADIENT, lcd_mixer_edit_gradient_menu);
    MENU_ITEM_ADDON_START(10);
      sprintf_P(tmp, PSTR("T%i->T%i"), mixer.gradient.start_vtool, mixer.gradient.end_vtool);
      lcd_put_u8str(tmp);
    MENU_ITEM_ADDON_END();
    ACTION_ITEM(MSG_REVERSE_GRADIENT, _lcd_mixer_reverse_gradient);
  }
  #endif

  END_MENU();
}

#endif // HAS_LCD_MENU && MIXING_EXTRUDER
