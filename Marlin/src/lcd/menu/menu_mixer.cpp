/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
    ui.defer_status_screen(true);
    ui.encoder_direction_normal();
    ENCODER_RATE_MULTIPLY(true);
    if (ui.encoderPosition != 0) {
      mixer.gradient.start_z += float((int)ui.encoderPosition) * 0.1;
      ui.encoderPosition = 0;
      NOLESS(mixer.gradient.start_z, 0);
      NOMORE(mixer.gradient.start_z, Z_MAX_POS);
    }
    if (ui.should_draw()) {
      char tmp[21];
      sprintf_P(tmp, PSTR(MSG_START_Z ": %4d.%d mm"), int(mixer.gradient.start_z), int(mixer.gradient.start_z * 10) % 10);
      SETCURSOR(2, (LCD_HEIGHT - 1) / 2);
      LCDPRINT(tmp);
    }

    if (ui.lcd_clicked) {
      if (mixer.gradient.start_z > mixer.gradient.end_z)
        mixer.gradient.end_z = mixer.gradient.start_z;
      mixer.refresh_gradient();
      ui.goto_previous_screen();
    }
  }

  void lcd_mixer_gradient_z_end_edit() {
    ui.defer_status_screen(true);
    ui.encoder_direction_normal();
    ENCODER_RATE_MULTIPLY(true);
    if (ui.encoderPosition != 0) {
      mixer.gradient.end_z += float((int)ui.encoderPosition) * 0.1;
      ui.encoderPosition = 0;
      NOLESS(mixer.gradient.end_z, 0);
      NOMORE(mixer.gradient.end_z, Z_MAX_POS);
    }

    if (ui.should_draw()) {
      char tmp[21];
      sprintf_P(tmp, PSTR(MSG_END_Z ": %4d.%d mm"), int(mixer.gradient.end_z), int(mixer.gradient.end_z * 10) % 10);
      SETCURSOR(2, (LCD_HEIGHT - 1) / 2);
      LCDPRINT(tmp);
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
    MENU_BACK(MSG_MIXER);

    MENU_ITEM_EDIT_CALLBACK(int8, MSG_START_VTOOL, &mixer.gradient.start_vtool, 0, MIXING_VIRTUAL_TOOLS - 1, mixer.refresh_gradient);
    MENU_ITEM_EDIT_CALLBACK(int8, MSG_END_VTOOL, &mixer.gradient.end_vtool, 0, MIXING_VIRTUAL_TOOLS - 1, mixer.refresh_gradient);

    #if ENABLED(GRADIENT_VTOOL)
      MENU_ITEM_EDIT_CALLBACK(int8, MSG_GRADIENT_ALIAS, &mixer.gradient.vtool_index, 0, MIXING_VIRTUAL_TOOLS - 1, mixer.refresh_gradient);
    #endif

    char tmp[10];

    MENU_ITEM(submenu, MSG_START_Z ":", lcd_mixer_gradient_z_start_edit);
    MENU_ITEM_ADDON_START(9);
      sprintf_P(tmp, PSTR("%4d.%d mm"), int(mixer.gradient.start_z), int(mixer.gradient.start_z * 10) % 10);
      LCDPRINT(tmp);
    MENU_ITEM_ADDON_END();

    MENU_ITEM(submenu, MSG_END_Z ":", lcd_mixer_gradient_z_end_edit);
    MENU_ITEM_ADDON_START(9);
      sprintf_P(tmp, PSTR("%4d.%d mm"), int(mixer.gradient.end_z), int(mixer.gradient.end_z * 10) % 10);
      LCDPRINT(tmp);
    MENU_ITEM_ADDON_END();

    END_MENU();
  }

#endif // GRADIENT_MIX

static uint8_t v_index;

#if DUAL_MIXING_EXTRUDER
  void _lcd_draw_mix(const uint8_t y) {
    char tmp[21];
    sprintf_P(tmp, PSTR(MSG_MIX ":    %3d%% %3d%%"), int(mixer.mix[0]), int(mixer.mix[1]));
    SETCURSOR(2, y);
    LCDPRINT(tmp);
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

    #define EDIT_COLOR(N) MENU_MULTIPLIER_ITEM_EDIT(float52, MSG_MIX_COMPONENT " " STRINGIFY(N), &mixer.collector[N-1], 0, 10);

    START_MENU();
    MENU_BACK(MSG_MIXER);
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
    MENU_ITEM(function, MSG_CYCLE_MIX, _lcd_mixer_cycle_mix);
    MENU_ITEM(function, MSG_COMMIT_VTOOL, _lcd_mixer_commit_vtool);
    END_MENU();

  #elif DUAL_MIXING_EXTRUDER

    if (ui.encoderPosition != 0) {
      mixer.mix[0] += (int)ui.encoderPosition;
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
    MENU_BACK(MSG_MIXER);
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
inline void _lcd_reset_vtools() {
  LCD_MESSAGEPGM(MSG_VTOOLS_RESET);
  ui.return_to_status();
  mixer.reset_vtools();
}

void menu_mixer_vtools_reset_confirm() {
  START_MENU();
  MENU_BACK(MSG_BACK);
  MENU_ITEM(function, MSG_RESET_VTOOLS, _lcd_reset_vtools);
  END_MENU();
}

void menu_mixer() {
  START_MENU();
  MENU_BACK(MSG_MAIN);

  v_index = mixer.get_current_vtool();
  MENU_ITEM_EDIT_CALLBACK(uint8, MSG_ACTIVE_VTOOL, &v_index, 0, MIXING_VIRTUAL_TOOLS - 1, _lcd_mixer_select_vtool
    #if DUAL_MIXING_EXTRUDER
      , true
    #endif
  );

  MENU_ITEM(submenu, MSG_MIX,
    #if CHANNEL_MIX_EDITING
      _lcd_goto_mix_edit
    #elif DUAL_MIXING_EXTRUDER
      lcd_mixer_mix_edit
    #endif
  );

  #if DUAL_MIXING_EXTRUDER
  {
    char tmp[10];
    MENU_ITEM_ADDON_START(10);
      mixer.update_mix_from_vtool();
      sprintf_P(tmp, PSTR("%3d;%3d%%"), int(mixer.mix[0]), int(mixer.mix[1]));
      LCDPRINT(tmp);
    MENU_ITEM_ADDON_END();
    MENU_ITEM(function, MSG_TOGGLE_MIX, _lcd_mixer_toggle_mix);
  }
  #endif

  MENU_ITEM(submenu, MSG_RESET_VTOOLS, menu_mixer_vtools_reset_confirm);

  #if ENABLED(GRADIENT_MIX)
  {
    char tmp[10];
    MENU_ITEM(submenu, MSG_GRADIENT, lcd_mixer_edit_gradient_menu);
    MENU_ITEM_ADDON_START(10);
      sprintf_P(tmp, PSTR("T%i->T%i"), mixer.gradient.start_vtool, mixer.gradient.end_vtool);
      LCDPRINT(tmp);
    MENU_ITEM_ADDON_END();
    MENU_ITEM(function, MSG_REVERSE_GRADIENT, _lcd_mixer_reverse_gradient);
  }
  #endif

  END_MENU();
}

#endif // HAS_LCD_MENU && MIXING_EXTRUDER
