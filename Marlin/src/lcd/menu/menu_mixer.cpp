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

//
// Mixer Menu
//
//   Mixer
//     In Use         What is being extruded now (the gradient, if one is in effect)
//     Active V-tool  The V-tool that is selected (T<n>). Takes effect at once.
//     Mix            Percent of each filament in the active V-tool. Takes effect at once.
//     Gradient       Blend from one V-tool to another over a range of Z.
//     Reset V-tools  Back to one pure filament per V-tool.
//

#include "../../inc/MarlinConfigPre.h"

#if ALL(HAS_MARLINUI_MENU, MIXING_EXTRUDER)

#include "menu_item.h"
#include "menu_addon.h"

#include "../../feature/mixing.h"

#if HAS_GRAPHICAL_TFT
  #include "../tft/tft.h"
#endif

static uint8_t v_index;                     // The V-tool being viewed and edited (the selected V-tool)
static uint8_t mix_pct[MIXING_STEPPERS];    // Percent of each filament in that V-tool. Always adds up to 100.

//
// Percent of each filament in a V-tool, rounded so the total is exactly 100
//
static void load_mix_pct(const uint8_t t) {
  float c[MIXING_STEPPERS];
  mixer.refresh_collector(100.0f, t, c);
  int total = 0;
  uint8_t biggest = 0;
  MIXER_STEPPER_LOOP(i) {
    if (!(c[i] >= 0.0f)) c[i] = 0.0f;             // A V-tool with nothing in it has no proportions
    mix_pct[i] = uint8_t(c[i] + 0.5f);
    total += mix_pct[i];
    if (c[i] > c[biggest]) biggest = i;
  }
  if (total == 0) { mix_pct[0] = 100; return; }   // Treat an empty V-tool as filament 1
  mix_pct[biggest] += 100 - total;                // Rounding error goes to the largest component
}

//
// Send the edited percentages to the V-tool. The mixer takes any proportions, so this is the whole commit.
//
static void apply_mix_pct() {
  MIXER_STEPPER_LOOP(i) mixer.collector[i] = mix_pct[i];
  mixer.normalize(v_index);                       // Also refreshes the gradient
}

//
// After editing component 'e', the other components share what is left in their current proportions
// (equally, if they were all zero), so the total stays 100.
//
static void rebalance_mix_pct(const uint8_t e) {
  const int left = 100 - mix_pct[e];
  int others = 0;
  MIXER_STEPPER_LOOP(i) if (i != e) others += mix_pct[i];
  int assigned = 0;
  uint8_t biggest = e == 0 ? 1 : 0;
  MIXER_STEPPER_LOOP(i) if (i != e) {
    mix_pct[i] = others ? uint8_t((mix_pct[i] * left + others / 2) / others) : uint8_t(left / (MIXING_STEPPERS - 1));
    assigned += mix_pct[i];
    if (mix_pct[i] > mix_pct[biggest]) biggest = i;
  }
  if (MIXING_STEPPERS > 1) mix_pct[biggest] += left - assigned;   // Rounding error goes to the largest other component
}

template<uint8_t E> static void _mix_edited() {
  rebalance_mix_pct(E);
  apply_mix_pct();
  ui.refresh();
}

static void _mix_even_split() {
  MIXER_STEPPER_LOOP(i) mix_pct[i] = 100 / (MIXING_STEPPERS);
  mix_pct[0] += 100 - (100 / (MIXING_STEPPERS)) * (MIXING_STEPPERS);
  apply_mix_pct();
  ui.refresh();
}

// "25/25/50", fixed width so it can be right-justified (up to 4 components fit)
#if MIXING_STEPPERS <= 4
  static void _put_mix(const uint8_t (&pct)[MIXING_STEPPERS]) {
    MString<4 * MIXING_STEPPERS> s;
    MIXER_STEPPER_LOOP(i) { if (i) s += '/'; s.appendf(F("%3i"), int(pct[i])); }
    lcd_put_u8str(s);
  }
#endif

//
// Mix: edit the percent of each filament in the active V-tool
//
void lcd_mixer_mix_edit() {
  START_MENU();
  BACK_ITEM(MSG_MIXER);

  STATIC_ITEM(MSG_ACTIVE_VTOOL, SS_DEFAULT, i8tostr3rj(v_index));

  #define _MIX_ITEM(E) EDIT_ITEM_N(percent_3, (E) + 1, MSG_MIX_COMPONENT_N, &mix_pct[E], 0, 100, _mix_edited<E>, true);
  REPEAT(MIXING_STEPPERS, _MIX_ITEM)

  ACTION_ITEM(MSG_MIX_EVEN_SPLIT, _mix_even_split);

  END_MENU();
}

inline void _lcd_goto_mix_edit() {
  v_index = mixer.get_current_vtool();
  load_mix_pct(v_index);
  ui.goto_screen(lcd_mixer_mix_edit);
}

#if ENABLED(GRADIENT_MIX)

  void _lcd_mixer_gradient_z_edit(const bool isend) {
    ui.defer_status_screen();
    ui.enable_encoder_multiplier(true);

    float &zvar = isend ? mixer.gradient.end_z : mixer.gradient.start_z;

    if (ui.encoderPosition) {
      zvar += float(int32_t(ui.encoderPosition)) * 0.1;
      ui.encoderPosition = 0;
      LIMIT(zvar, 0, Z_MAX_POS);
    }

    if (ui.should_draw()) {
      SETCURSOR(1, (LCD_HEIGHT - 1) / 2);
      lcd_put_u8str(isend ? GET_TEXT_F(MSG_END_Z) : GET_TEXT_F(MSG_START_Z));
      SETCURSOR_RJ(9, (LCD_HEIGHT - 1) / 2);
      lcd_put_u8str(MString<15>{}.setf_P(PSTR("%4d.%d mm"), int(zvar), int(zvar * 10) % 10));
    }

    if (ui.lcd_clicked) {
      if (isend && zvar < mixer.gradient.start_z)
        mixer.gradient.start_z = zvar;
      else if (!isend && zvar > mixer.gradient.end_z)
        mixer.gradient.end_z = zvar;
      mixer.refresh_gradient();
      ui.goto_previous_screen();
    }
    else {
      TERN_(HAS_GRAPHICAL_TFT, tft.draw_edit_screen_buttons());
    }
  }

  //
  // Why the gradient is, or is not, in effect (the mixer needs all of these)
  //
  static const char* gradient_status() {
    if (mixer.gradient.enabled) return PSTR("In effect");
    if (!mixer.gradient.allowed) return PSTR("Switched off");
    if (mixer.gradient.start_vtool == mixer.gradient.end_vtool) return PSTR("Same V-tools");
    if (mixer.gradient.start_z >= mixer.gradient.end_z) return PSTR("End Z <= Start Z");
    return PSTR("Not this V-tool");   // GRADIENT_VTOOL: the alias V-tool is not selected
  }

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

  void lcd_mixer_edit_gradient_menu() {
    START_MENU();
    BACK_ITEM(MSG_MIXER);

    // Say what the gradient is doing, and when it is not, why not
    PSTRING_ITEM_P(MSG_GRADIENT, gradient_status(), SS_DEFAULT);

    EDIT_ITEM(bool, MSG_GRADIENT_MIX, &mixer.gradient.allowed, mixer.refresh_gradient);

    EDIT_ITEM(int8, MSG_START_VTOOL, &mixer.gradient.start_vtool, 0, MIXING_VIRTUAL_TOOLS - 1, mixer.refresh_gradient);
    EDIT_ITEM(int8, MSG_END_VTOOL, &mixer.gradient.end_vtool, 0, MIXING_VIRTUAL_TOOLS - 1, mixer.refresh_gradient);

    FSTR_P const slabel = GET_TEXT_F(MSG_START_Z);
    SUBMENU_F(slabel, []{ _lcd_mixer_gradient_z_edit(false); });
    MENU_ITEM_ADDON_START_RJ(11);
      lcd_put_u8str(MString<15>{}.setf_P(PSTR("%4d.%d mm"), int(mixer.gradient.start_z), int(mixer.gradient.start_z * 10) % 10));
    MENU_ITEM_ADDON_END();

    FSTR_P const elabel = GET_TEXT_F(MSG_END_Z);
    SUBMENU_F(elabel, []{ _lcd_mixer_gradient_z_edit(true); });
    MENU_ITEM_ADDON_START_RJ(11);
      lcd_put_u8str(MString<15>{}.setf_P(PSTR("%4d.%d mm"), int(mixer.gradient.end_z), int(mixer.gradient.end_z * 10) % 10));
    MENU_ITEM_ADDON_END();

    ACTION_ITEM(MSG_REVERSE_GRADIENT, _lcd_mixer_reverse_gradient);

    #if ENABLED(GRADIENT_VTOOL)
      EDIT_ITEM(int8, MSG_GRADIENT_ALIAS, &mixer.gradient.vtool_index, -1, MIXING_VIRTUAL_TOOLS - 1, mixer.refresh_gradient);
    #endif

    END_MENU();
  }

#endif // GRADIENT_MIX

//
// Select the active V-tool. The mix of the new V-tool is in effect at once.
//
static void _lcd_mixer_select_vtool() { mixer.T(v_index); }

void menu_mixer() {
  START_MENU();
  BACK_ITEM(MSG_MAIN_MENU);

  v_index = mixer.get_current_vtool();

  // What is coming out of the nozzle now: the gradient, if one is in effect, or the active V-tool
  {
    uint8_t now[MIXING_STEPPERS];
    #if ENABLED(GRADIENT_MIX)
      if (mixer.gradient.enabled) {
        float total = 0;
        MIXER_STEPPER_LOOP(i) total += mixer.gradient.color[i];
        MIXER_STEPPER_LOOP(i) now[i] = total > 0 ? uint8_t(100.0f * mixer.gradient.color[i] / total + 0.5f) : 0;
      }
      else
    #endif
    {
      load_mix_pct(v_index);
      MIXER_STEPPER_LOOP(i) now[i] = mix_pct[i];
    }
    #if MIXING_STEPPERS <= 4
      STATIC_ITEM(MSG_MIX_IN_USE, SS_LEFT);
      MENU_ITEM_ADDON_START_RJ(4 * MIXING_STEPPERS - 1);
        _put_mix(now);
      MENU_ITEM_ADDON_END();
    #endif
  }

  EDIT_ITEM(uint8, MSG_ACTIVE_VTOOL, &v_index, 0, MIXING_VIRTUAL_TOOLS - 1, _lcd_mixer_select_vtool, true);

  SUBMENU(MSG_MIX, _lcd_goto_mix_edit);
  #if MIXING_STEPPERS <= 4
    MENU_ITEM_ADDON_START_RJ(4 * MIXING_STEPPERS);     // One column in from the edge, for the submenu arrow
      load_mix_pct(mixer.get_current_vtool());
      _put_mix(mix_pct);
    MENU_ITEM_ADDON_END();
  #endif

  #if ENABLED(GRADIENT_MIX)
    SUBMENU(MSG_GRADIENT, lcd_mixer_edit_gradient_menu);
    MENU_ITEM_ADDON_START_RJ(9);
      if (mixer.gradient.enabled)
        lcd_put_u8str(MString<16>{}.setf_P(PSTR("T%i->T%i"), mixer.gradient.start_vtool, mixer.gradient.end_vtool));
      else
        lcd_put_u8str(GET_TEXT_F(MSG_OFF));
    MENU_ITEM_ADDON_END();
  #endif

  //
  // Reset All V-Tools
  //
  CONFIRM_ITEM(MSG_RESET_VTOOLS,
    MSG_BUTTON_RESET, MSG_BUTTON_CANCEL,
    []{
      mixer.reset_vtools();
      LCD_MESSAGE(MSG_VTOOLS_RESET);
      ui.return_to_status();
    },
    nullptr,
    GET_TEXT_F(MSG_RESET_VTOOLS), (const char *)nullptr, F("?")
  );

  END_MENU();
}

#endif // HAS_MARLINUI_MENU && MIXING_EXTRUDER
