/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * lcd/extui/anycubic_kobra/kobra_extui.cpp
 *
 * Anycubic Kobra TFT (DGUS) ExtUI implementation
 * Based on maikramer/MarlinKobraMax with clean-room port to mainline
 */

#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(ANYCUBIC_LCD_KOBRA)

  #include "../ui_api.h"
  #include "dgus_tft.h"

  using namespace Anycubic;

  namespace ExtUI {

    void onStartup()  { Dgus.Startup(); }
    void onIdle()     { Dgus.IdleLoop(); }

    void onPrinterKilled(FSTR_P const error, FSTR_P const component) {
      Dgus.PrinterKilled(error, component);
    }

    void onMediaMounted() { Dgus.MediaEvent(AC_media_inserted); }
    void onMediaError()   { Dgus.MediaEvent(AC_media_error); }
    void onMediaRemoved() { Dgus.MediaEvent(AC_media_removed); }

    void onPlayTone(const uint16_t frequency, const uint16_t duration/*=0*/) {
      #if ENABLED(SPEAKER)
        // ::tone(BEEPER_PIN, frequency, duration);
      #endif
    }

    void onPrintTimerStarted() { Dgus.TimerEvent(AC_timer_started); }
    void onPrintTimerPaused()  { Dgus.TimerEvent(AC_timer_paused); }
    void onPrintTimerStopped() { Dgus.TimerEvent(AC_timer_stopped); }

    void onFilamentRunout(const extruder_t) { Dgus.FilamentRunout(); }

    void onUserConfirmRequired(const char * const msg) { Dgus.ConfirmationRequest(msg); }

    #if HAS_RESUME_CONTINUE
      void onUserConfirmRequired(const int icon, const char * const cstr, FSTR_P const) {
        onUserConfirmRequired(cstr);
      }
      void onUserConfirmRequired(const int icon, FSTR_P const fstr, FSTR_P const) {
        onUserConfirmRequired(FTOP(fstr));
      }
    #endif

    void onStatusChanged(const char * const msg) { Dgus.StatusChange(msg); }

    void onHomingStart() { Dgus.HomingStart(); }
    void onHomingDone()  { Dgus.HomingComplete(); }

    void onFactoryReset() {
      Dgus.page_index_now     = 121;
      Dgus.lcd_info.language  = ExtUI::ENG;
      Dgus.lcd_info.audio     = ExtUI::ON;
    }

    void onStoreSettings(char *buff) {
      static_assert(sizeof(Dgus.lcd_info) <= ExtUI::eeprom_data_size,
        "Anycubic Kobra lcd_info exceeds ExtUI eeprom_data_size");
      memcpy(buff, &Dgus.lcd_info, sizeof(Dgus.lcd_info));
    }

    void onLoadSettings(const char *buff) {
      memcpy(&Dgus.lcd_info,      buff, sizeof(Dgus.lcd_info));
      memcpy(&Dgus.lcd_info_back, buff, sizeof(Dgus.lcd_info_back));
    }

    #if HAS_LEVELING
      void onLevelingStart() { Dgus.LevelingStart(); }
      void onLevelingDone()  { Dgus.LevelingDone(); }
    #endif

    #if HAS_MESH
      void onMeshUpdate(const int8_t xpos, const int8_t ypos, const ExtUI::probe_state_t state) {
        Dgus.MeshUpdate(xpos, ypos, state);
      }
    #endif

    #if ENABLED(POWER_LOSS_RECOVERY)
      void onPowerLoss()               { Dgus.PowerLoss(); }
    #endif

  } // namespace ExtUI

#endif // ANYCUBIC_LCD_KOBRA
