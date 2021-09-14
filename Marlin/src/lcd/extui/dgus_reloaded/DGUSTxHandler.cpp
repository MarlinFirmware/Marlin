/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(DGUS_LCD_UI_RELOADED)

#include "DGUSTxHandler.h"

#include "DGUSScreenHandler.h"
#include "config/DGUS_Data.h"

#include "../ui_api.h"
#include "../../../module/stepper.h"
#include "../../../module/printcounter.h"
#if ENABLED(ADVANCED_PAUSE_FEATURE)
  #include "../../../feature/pause.h"
#endif

#if ENABLED(SDSUPPORT)
  void DGUSTxHandler::SetFileControlState(int file, bool state) {
    DGUS_Control control;

    switch (file) {
      default: return;
      case 0:
        control = DGUS_Control::FILE0;
        break;
      case 1:
        control = DGUS_Control::FILE1;
        break;
      case 2:
        control = DGUS_Control::FILE2;
        break;
      case 3:
        control = DGUS_Control::FILE3;
        break;
      case 4:
        control = DGUS_Control::FILE4;
        break;
    }

    if (state) {
      dgus_display.EnableControl(DGUS_Screen::PRINT,
                                 DGUSDisplay::RETURN_KEY_CODE,
                                 control);
    }
    else {
      dgus_display.DisableControl(DGUS_Screen::PRINT,
                                  DGUSDisplay::RETURN_KEY_CODE,
                                  control);
    }
  }

  void DGUSTxHandler::FileType(DGUS_VP &vp) {
    // Batch send
    uint16_t data[DGUS_FILE_COUNT];

    for (int i = 0; i < DGUS_FILE_COUNT; i++) {
      if (!dgus_screen_handler.filelist.seek(dgus_screen_handler.filelist_offset + i)) {
        data[i] = Swap16((uint16_t)DGUS_Data::SDType::NONE);

        SetFileControlState(i, false);
        continue;
      }

      data[i] = dgus_screen_handler.filelist.isDir() ?
                  Swap16((uint16_t)DGUS_Data::SDType::DIRECTORY)
                : Swap16((uint16_t)DGUS_Data::SDType::FILE);

      SetFileControlState(i, true);
    }

    dgus_display.Write((uint16_t)vp.addr, data, sizeof(*data) * DGUS_FILE_COUNT);
  }

  void DGUSTxHandler::FileName(DGUS_VP &vp) {
    uint8_t offset;

    switch (vp.addr) {
      default: return;
      case DGUS_Addr::SD_FileName0:
        offset = 0;
        break;
      case DGUS_Addr::SD_FileName1:
        offset = 1;
        break;
      case DGUS_Addr::SD_FileName2:
        offset = 2;
        break;
      case DGUS_Addr::SD_FileName3:
        offset = 3;
        break;
      case DGUS_Addr::SD_FileName4:
        offset = 4;
        break;
    }

    if (dgus_screen_handler.filelist.seek(dgus_screen_handler.filelist_offset + offset)) {
      dgus_display.WriteString((uint16_t)vp.addr, dgus_screen_handler.filelist.filename(), vp.size);
    }
    else {
      dgus_display.WriteStringPGM((uint16_t)vp.addr, NUL_STR, vp.size);
    }
  }

  void DGUSTxHandler::ScrollIcons(DGUS_VP &vp) {
    uint16_t icons = 0;

    if (!dgus_screen_handler.filelist.isAtRootDir()) {
      icons |= (uint16_t)DGUS_Data::ScrollIcon::GO_BACK;

      dgus_display.EnableControl(DGUS_Screen::PRINT,
                                 DGUSDisplay::RETURN_KEY_CODE,
                                 DGUS_Control::GO_BACK);
    }
    else {
      dgus_display.DisableControl(DGUS_Screen::PRINT,
                                  DGUSDisplay::RETURN_KEY_CODE,
                                  DGUS_Control::GO_BACK);
    }

    if (dgus_screen_handler.filelist_offset > 0) {
      icons |= (uint16_t)DGUS_Data::ScrollIcon::UP;

      dgus_display.EnableControl(DGUS_Screen::PRINT,
                                 DGUSDisplay::RETURN_KEY_CODE,
                                 DGUS_Control::SCROLL_UP);
    }
    else {
      dgus_display.DisableControl(DGUS_Screen::PRINT,
                                  DGUSDisplay::RETURN_KEY_CODE,
                                  DGUS_Control::SCROLL_UP);
    }

    if (dgus_screen_handler.filelist_offset + DGUS_FILE_COUNT < dgus_screen_handler.filelist.count()) {
      icons |= (uint16_t)DGUS_Data::ScrollIcon::DOWN;

      dgus_display.EnableControl(DGUS_Screen::PRINT,
                                 DGUSDisplay::RETURN_KEY_CODE,
                                 DGUS_Control::SCROLL_DOWN);
    }
    else {
      dgus_display.DisableControl(DGUS_Screen::PRINT,
                                  DGUSDisplay::RETURN_KEY_CODE,
                                  DGUS_Control::SCROLL_DOWN);
    }

    dgus_display.Write((uint16_t)vp.addr, Swap16(icons));
  }

  void DGUSTxHandler::SelectedFileName(DGUS_VP &vp) {
    if (dgus_screen_handler.filelist_selected < 0
        || !dgus_screen_handler.filelist.seek(dgus_screen_handler.filelist_selected)) {
      dgus_display.WriteStringPGM((uint16_t)vp.addr, NUL_STR, vp.size);
      return;
    }

    dgus_display.WriteString((uint16_t)vp.addr, dgus_screen_handler.filelist.filename(), vp.size);
  }
#endif // SDSUPPORT

void DGUSTxHandler::PositionZ(DGUS_VP &vp) {
  float position = ExtUI::isAxisPositionKnown(ExtUI::Z) ?
                     planner.get_axis_position_mm(Z_AXIS)
                   : 0;

  const int16_t data = dgus_display.ToFixedPoint<float, int16_t, 1>(position);
  dgus_display.Write((uint16_t)vp.addr, Swap16(data));
}

void DGUSTxHandler::Ellapsed(DGUS_VP &vp) {
  char buffer[21];
  duration_t(print_job_timer.duration()).toString(buffer);

  dgus_display.WriteString((uint16_t)vp.addr, buffer, vp.size);
}

void DGUSTxHandler::Percent(DGUS_VP &vp) {
  uint16_t progress;

  switch (vp.addr) {
    default: return;
    case DGUS_Addr::STATUS_Percent:
      progress = constrain(ExtUI::getProgress_percent(), 0, 100);
      break;
    case DGUS_Addr::STATUS_Percent_Complete:
      progress = 100;
      break;
  }

  dgus_display.Write((uint16_t)DGUS_Addr::STATUS_Percent, Swap16(progress));
}

void DGUSTxHandler::StatusIcons(DGUS_VP &vp) {
  uint16_t icons = 0;

  if (ExtUI::isPrinting()) {
    icons |= (uint16_t)DGUS_Data::StatusIcon::PAUSE;

    dgus_display.EnableControl(DGUS_Screen::PRINT_STATUS,
                               DGUSDisplay::POPUP_WINDOW,
                               DGUS_Control::PAUSE);
  }
  else {
    dgus_display.DisableControl(DGUS_Screen::PRINT_STATUS,
                                DGUSDisplay::POPUP_WINDOW,
                                DGUS_Control::PAUSE);
  }

  if (ExtUI::isPrintingPaused()) {
    icons |= (uint16_t)DGUS_Data::StatusIcon::RESUME;

    dgus_display.EnableControl(DGUS_Screen::PRINT_STATUS,
                               DGUSDisplay::POPUP_WINDOW,
                               DGUS_Control::RESUME);
  }
  else {
    dgus_display.DisableControl(DGUS_Screen::PRINT_STATUS,
                                DGUSDisplay::POPUP_WINDOW,
                                DGUS_Control::RESUME);
  }

  dgus_display.Write((uint16_t)vp.addr, Swap16(icons));
}

void DGUSTxHandler::Flowrate(DGUS_VP &vp) {
  int16_t flowrate;

  switch (vp.addr) {
    default: return;
    case DGUS_Addr::ADJUST_Flowrate_CUR:
      #if EXTRUDERS > 1
        flowrate = ExtUI::getFlow_percent(ExtUI::getActiveTool());
      #else
        flowrate = ExtUI::getFlow_percent(ExtUI::E0);
      #endif
      break;
    #if EXTRUDERS > 1
      case DGUS_Addr::ADJUST_Flowrate_E0:
        flowrate = ExtUI::getFlow_percent(ExtUI::E0);
        break;
      case DGUS_Addr::ADJUST_Flowrate_E1:
        flowrate = ExtUI::getFlow_percent(ExtUI::E1);
        break;
    #endif
  }

  dgus_display.Write((uint16_t)vp.addr, Swap16(flowrate));
}

void DGUSTxHandler::TempMax(DGUS_VP &vp) {
  uint16_t temp;

  switch (vp.addr) {
    default: return;
    case DGUS_Addr::TEMP_Max_Bed:
      temp = BED_MAX_TARGET;
      break;
    case DGUS_Addr::TEMP_Max_H0:
      temp = HEATER_0_MAXTEMP - HOTEND_OVERSHOOT;
      break;
    #if HOTENDS > 1
      case DGUS_Addr::TEMP_Max_H1:
        temp = HEATER_1_MAXTEMP - HOTEND_OVERSHOOT;
        break;
    #endif
  }

  dgus_display.Write((uint16_t)vp.addr, Swap16(temp));
}

void DGUSTxHandler::StepperStatus(DGUS_VP &vp) {
  if (X_ENABLE_READ() == X_ENABLE_ON
      && Y_ENABLE_READ() == Y_ENABLE_ON
      && Z_ENABLE_READ() == Z_ENABLE_ON) {
    dgus_display.Write((uint16_t)vp.addr, Swap16((uint16_t)DGUS_Data::Status::ENABLED));
  }
  else {
    dgus_display.Write((uint16_t)vp.addr, Swap16((uint16_t)DGUS_Data::Status::DISABLED));
  }
}

void DGUSTxHandler::StepIcons(DGUS_VP &vp) {
  if (!vp.extra) return;
  uint16_t icons = 0;
  DGUS_Data::StepSize size = *(DGUS_Data::StepSize*)vp.extra;

  switch (size) {
    case DGUS_Data::StepSize::MM10:
      icons |= (uint16_t)DGUS_Data::StepIcon::MM10;
      break;
    case DGUS_Data::StepSize::MM1:
      icons |= (uint16_t)DGUS_Data::StepIcon::MM1;
      break;
    case DGUS_Data::StepSize::MMP1:
      icons |= (uint16_t)DGUS_Data::StepIcon::MMP1;
      break;
    case DGUS_Data::StepSize::MMP01:
      icons |= (uint16_t)DGUS_Data::StepIcon::MMP01;
      break;
  }

  dgus_display.Write((uint16_t)vp.addr, Swap16(icons));
}

void DGUSTxHandler::ABLDisableIcon(DGUS_VP &vp) {
  uint16_t data;

  if (ExtUI::getLevelingActive()) {
    data = (uint16_t)DGUS_Data::Status::ENABLED;

    dgus_display.EnableControl(DGUS_Screen::LEVELING_AUTOMATIC,
                               DGUSDisplay::RETURN_KEY_CODE,
                               DGUS_Control::DISABLE);
  }
  else {
    data = (uint16_t)DGUS_Data::Status::DISABLED;

    dgus_display.DisableControl(DGUS_Screen::LEVELING_AUTOMATIC,
                                DGUSDisplay::RETURN_KEY_CODE,
                                DGUS_Control::DISABLE);
  }

  dgus_display.Write((uint16_t)vp.addr, Swap16(data));
}

void DGUSTxHandler::ABLGrid(DGUS_VP &vp) {
  // Batch send
  int16_t data[DGUS_LEVEL_GRID_SIZE];
  xy_uint8_t point;
  int16_t fixed;

  for (int i = 0; i < DGUS_LEVEL_GRID_SIZE; i++) {
    point.x = i % GRID_MAX_POINTS_X;
    point.y = i / GRID_MAX_POINTS_X;
    fixed = dgus_display.ToFixedPoint<float, int16_t, 3>(ExtUI::getMeshPoint(point));
    data[i] = Swap16(fixed);
  }

  dgus_display.Write((uint16_t)vp.addr, data, sizeof(*data) * DGUS_LEVEL_GRID_SIZE);
}

void DGUSTxHandler::FilamentIcons(DGUS_VP &vp) {
  uint16_t icons = 0;

  switch (dgus_screen_handler.filament_extruder) {
    default: return;
    case DGUS_Data::Extruder::CURRENT:
      #if EXTRUDERS > 1
        switch (ExtUI::getActiveTool()) {
          default: break;
          case ExtUI::E0:
            icons |= (uint16_t)DGUS_Data::ExtruderIcon::E0;
            break;
          case ExtUI::E1:
            icons |= (uint16_t)DGUS_Data::ExtruderIcon::E1;
            break;
        }
        break;
      #endif
    case DGUS_Data::Extruder::E0:
      icons |= (uint16_t)DGUS_Data::ExtruderIcon::E0;
      break;
    case DGUS_Data::Extruder::E1:
      icons |= (uint16_t)DGUS_Data::ExtruderIcon::E1;
      break;
  }

  dgus_display.Write((uint16_t)vp.addr, Swap16(icons));
}

void DGUSTxHandler::BLTouch(DGUS_VP &vp) {
  #if ENABLED(BLTOUCH)
    dgus_display.EnableControl(DGUS_Screen::SETTINGS_MENU2,
                               DGUSDisplay::RETURN_KEY_CODE,
                               DGUS_Control::EXTRA2);

    dgus_display.Write((uint16_t)vp.addr, Swap16((uint16_t)DGUS_Data::Status::ENABLED));
  #else
    dgus_display.DisableControl(DGUS_Screen::SETTINGS_MENU2,
                                DGUSDisplay::RETURN_KEY_CODE,
                                DGUS_Control::EXTRA2);

    dgus_display.Write((uint16_t)vp.addr, Swap16((uint16_t)DGUS_Data::Status::DISABLED));
  #endif
}

void DGUSTxHandler::PIDIcons(DGUS_VP &vp) {
  uint16_t icons = 0;

  switch (dgus_screen_handler.pid_heater) {
    default: return;
    case DGUS_Data::Heater::BED:
      icons |= (uint16_t)DGUS_Data::HeaterIcon::BED;
      break;
    case DGUS_Data::Heater::H0:
      icons |= (uint16_t)DGUS_Data::HeaterIcon::H0;
      break;
    case DGUS_Data::Heater::H1:
      icons |= (uint16_t)DGUS_Data::HeaterIcon::H1;
      break;
  }

  dgus_display.Write((uint16_t)vp.addr, Swap16(icons));
}

void DGUSTxHandler::PIDKp(DGUS_VP &vp) {
  float value;

  switch (dgus_screen_handler.pid_heater) {
    default: return;
    #if ENABLED(PIDTEMPBED)
      case DGUS_Data::Heater::BED:
        value = ExtUI::getBedPIDValues_Kp();
        break;
    #endif
    #if ENABLED(PIDTEMP)
      case DGUS_Data::Heater::H0:
        value = ExtUI::getPIDValues_Kp(ExtUI::E0);
        break;
      #if HOTENDS > 1
        case DGUS_Data::Heater::H1:
          value = ExtUI::getPIDValues_Kp(ExtUI::E1);
          break;
      #endif
    #endif
  }

  const int32_t data = dgus_display.ToFixedPoint<float, int32_t, 2>(value);
  dgus_display.Write((uint16_t)vp.addr, dgus_display.SwapBytes(data));
}

void DGUSTxHandler::PIDKi(DGUS_VP &vp) {
  float value;

  switch (dgus_screen_handler.pid_heater) {
    default: return;
    #if ENABLED(PIDTEMPBED)
      case DGUS_Data::Heater::BED:
        value = ExtUI::getBedPIDValues_Ki();
        break;
    #endif
    #if ENABLED(PIDTEMP)
      case DGUS_Data::Heater::H0:
        value = ExtUI::getPIDValues_Ki(ExtUI::E0);
        break;
      #if HOTENDS > 1
        case DGUS_Data::Heater::H1:
          value = ExtUI::getPIDValues_Ki(ExtUI::E1);
          break;
      #endif
    #endif
  }

  const int32_t data = dgus_display.ToFixedPoint<float, int32_t, 2>(value);
  dgus_display.Write((uint16_t)vp.addr, dgus_display.SwapBytes(data));
}

void DGUSTxHandler::PIDKd(DGUS_VP &vp) {
  float value;

  switch (dgus_screen_handler.pid_heater) {
    default: return;
    #if ENABLED(PIDTEMPBED)
      case DGUS_Data::Heater::BED:
        value = ExtUI::getBedPIDValues_Kd();
        break;
    #endif
    #if ENABLED(PIDTEMP)
      case DGUS_Data::Heater::H0:
        value = ExtUI::getPIDValues_Kd(ExtUI::E0);
        break;
      #if HOTENDS > 1
        case DGUS_Data::Heater::H1:
          value = ExtUI::getPIDValues_Kd(ExtUI::E1);
          break;
      #endif
    #endif
  }

  const int32_t data = dgus_display.ToFixedPoint<float, int32_t, 2>(value);
  dgus_display.Write((uint16_t)vp.addr, dgus_display.SwapBytes(data));
}

void DGUSTxHandler::BuildVolume(DGUS_VP &vp) {
  char buffer[vp.size];
  snprintf_P(buffer, vp.size, PSTR("%dx%dx%d"), X_BED_SIZE, Y_BED_SIZE, (Z_MAX_POS - Z_MIN_POS));

  dgus_display.WriteString((uint16_t)vp.addr, buffer, vp.size);
}

void DGUSTxHandler::TotalPrints(DGUS_VP &vp) {
  #if ENABLED(PRINTCOUNTER)
    dgus_display.Write((uint16_t)vp.addr, dgus_display.SwapBytes(print_job_timer.getStats().totalPrints));
  #else
    UNUSED(vp);
  #endif
}

void DGUSTxHandler::FinishedPrints(DGUS_VP &vp) {
  #if ENABLED(PRINTCOUNTER)
    dgus_display.Write((uint16_t)vp.addr, dgus_display.SwapBytes(print_job_timer.getStats().finishedPrints));
  #else
    UNUSED(vp);
  #endif
}

void DGUSTxHandler::PrintTime(DGUS_VP &vp) {
  #if ENABLED(PRINTCOUNTER)
    char buffer[21];
    ExtUI::getTotalPrintTime_str(buffer);

    dgus_display.WriteString((uint16_t)vp.addr, buffer, vp.size);
  #else
    dgus_display.WriteStringPGM((uint16_t)vp.addr, DGUS_MSG_UNDEF, vp.size);
  #endif
}

void DGUSTxHandler::LongestPrint(DGUS_VP &vp) {
  #if ENABLED(PRINTCOUNTER)
    char buffer[21];
    ExtUI::getLongestPrint_str(buffer);

    dgus_display.WriteString((uint16_t)vp.addr, buffer, vp.size);
  #else
    dgus_display.WriteStringPGM((uint16_t)vp.addr, DGUS_MSG_UNDEF, vp.size);
  #endif
}

void DGUSTxHandler::FilamentUsed(DGUS_VP &vp) {
  #if ENABLED(PRINTCOUNTER)
    char buffer[21];
    ExtUI::getFilamentUsed_str(buffer);

    dgus_display.WriteString((uint16_t)vp.addr, buffer, vp.size);
  #else
    dgus_display.WriteStringPGM((uint16_t)vp.addr, DGUS_MSG_UNDEF, vp.size);
  #endif
}

void DGUSTxHandler::WaitIcons(DGUS_VP &vp) {
  uint16_t icons = 0;

  if (ExtUI::isPrintingPaused()) {
    icons |= (uint16_t)DGUS_Data::WaitIcon::ABORT;

    dgus_display.EnableControl(DGUS_Screen::WAIT,
                               DGUSDisplay::POPUP_WINDOW,
                               DGUS_Control::ABORT);
  }
  else {
    dgus_display.DisableControl(DGUS_Screen::WAIT,
                                DGUSDisplay::POPUP_WINDOW,
                                DGUS_Control::ABORT);
  }

  if (dgus_screen_handler.wait_continue) {
    icons |= (uint16_t)DGUS_Data::WaitIcon::CONTINUE;

    dgus_display.EnableControl(DGUS_Screen::WAIT,
                               DGUSDisplay::RETURN_KEY_CODE,
                               DGUS_Control::CONTINUE);
  }
  else {
    dgus_display.DisableControl(DGUS_Screen::WAIT,
                                DGUSDisplay::RETURN_KEY_CODE,
                                DGUS_Control::CONTINUE);
  }

  dgus_display.Write((uint16_t)vp.addr, Swap16(icons));
}

void DGUSTxHandler::FanSpeed(DGUS_VP &vp) {
  uint16_t fan_speed;

  switch (vp.addr) {
    default: return;
    case DGUS_Addr::FAN0_Speed: fan_speed = ExtUI::getTargetFan_percent(ExtUI::FAN0); break;
  }

  dgus_display.Write((uint16_t)vp.addr, Swap16(fan_speed));
}

void DGUSTxHandler::Volume(DGUS_VP &vp) {
  const uint16_t volume = dgus_display.GetVolume();

  dgus_display.Write((uint16_t)vp.addr, Swap16(volume));
}

void DGUSTxHandler::Brightness(DGUS_VP &vp) {
  const uint16_t brightness = dgus_display.GetBrightness();

  dgus_display.Write((uint16_t)vp.addr, Swap16(brightness));
}

void DGUSTxHandler::ExtraToString(DGUS_VP &vp) {
  if (!vp.size || !vp.extra) return;

  dgus_display.WriteString((uint16_t)vp.addr, vp.extra, vp.size, true, false, false);
}

void DGUSTxHandler::ExtraPGMToString(DGUS_VP &vp) {
  if (!vp.size || !vp.extra) return;

  dgus_display.WriteStringPGM((uint16_t)vp.addr, vp.extra, vp.size, true, false, false);
}

#endif // DGUS_LCD_UI_RELOADED
