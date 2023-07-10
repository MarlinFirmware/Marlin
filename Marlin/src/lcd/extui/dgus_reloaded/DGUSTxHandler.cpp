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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "../../../inc/MarlinConfigPre.h"

#if DGUS_LCD_UI_RELOADED

#include "DGUSTxHandler.h"

#include "DGUSScreenHandler.h"
#include "config/DGUS_Data.h"

#include "../ui_api.h"
#include "../../../module/stepper.h"
#include "../../../module/printcounter.h"
#if ENABLED(ADVANCED_PAUSE_FEATURE)
  #include "../../../feature/pause.h"
#endif

#if HAS_MEDIA
  void DGUSTxHandler::setFileControlState(int16_t file, bool state) {
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
      dgus.enableControl(DGUS_ScreenID::PRINT,
                                 DGUSDisplay::RETURN_KEY_CODE,
                                 control);
    }
    else {
      dgus.disableControl(DGUS_ScreenID::PRINT,
                                  DGUSDisplay::RETURN_KEY_CODE,
                                  control);
    }
  }

  void DGUSTxHandler::fileType(DGUS_VP &vp) {
    // Batch send
    uint16_t data[DGUS_FILE_COUNT];

    for (int16_t i = 0; i < DGUS_FILE_COUNT; i++) {
      if (!screen.filelist.seek(screen.filelist_offset + i)) {
        data[i] = Swap16(DGUS_Data::SDType::NONE);

        setFileControlState(i, false);
        continue;
      }

      data[i] = screen.filelist.isDir() ?
                  Swap16(DGUS_Data::SDType::DIRECTORY)
                : Swap16(DGUS_Data::SDType::FILE);

      setFileControlState(i, true);
    }

    dgus.write((uint16_t)vp.addr, data, sizeof(*data) * DGUS_FILE_COUNT);
  }

  void DGUSTxHandler::fileName(DGUS_VP &vp) {
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

    if (screen.filelist.seek(screen.filelist_offset + offset)) {
      dgus.writeString((uint16_t)vp.addr, screen.filelist.filename(), vp.size);
    }
    else {
      dgus.writeStringPGM((uint16_t)vp.addr, NUL_STR, vp.size);
    }
  }

  void DGUSTxHandler::scrollIcons(DGUS_VP &vp) {
    uint16_t icons = 0;

    if (!screen.filelist.isAtRootDir()) {
      icons |= (uint16_t)DGUS_Data::ScrollIcon::GO_BACK;

      dgus.enableControl(DGUS_ScreenID::PRINT,
                                 DGUSDisplay::RETURN_KEY_CODE,
                                 DGUS_Control::GO_BACK);
    }
    else {
      dgus.disableControl(DGUS_ScreenID::PRINT,
                                  DGUSDisplay::RETURN_KEY_CODE,
                                  DGUS_Control::GO_BACK);
    }

    if (screen.filelist_offset > 0) {
      icons |= (uint16_t)DGUS_Data::ScrollIcon::UP;

      dgus.enableControl(DGUS_ScreenID::PRINT,
                                 DGUSDisplay::RETURN_KEY_CODE,
                                 DGUS_Control::SCROLL_UP);
    }
    else {
      dgus.disableControl(DGUS_ScreenID::PRINT,
                                  DGUSDisplay::RETURN_KEY_CODE,
                                  DGUS_Control::SCROLL_UP);
    }

    if (screen.filelist_offset + DGUS_FILE_COUNT < screen.filelist.count()) {
      icons |= (uint16_t)DGUS_Data::ScrollIcon::DOWN;

      dgus.enableControl(DGUS_ScreenID::PRINT,
                                 DGUSDisplay::RETURN_KEY_CODE,
                                 DGUS_Control::SCROLL_DOWN);
    }
    else {
      dgus.disableControl(DGUS_ScreenID::PRINT,
                                  DGUSDisplay::RETURN_KEY_CODE,
                                  DGUS_Control::SCROLL_DOWN);
    }

    dgus.write((uint16_t)vp.addr, Swap16(icons));
  }

  void DGUSTxHandler::selectedFileName(DGUS_VP &vp) {
    if (screen.filelist_selected < 0
        || !screen.filelist.seek(screen.filelist_selected)) {
      dgus.writeStringPGM((uint16_t)vp.addr, NUL_STR, vp.size);
      return;
    }

    dgus.writeString((uint16_t)vp.addr, screen.filelist.filename(), vp.size);
  }
#endif // HAS_MEDIA

void DGUSTxHandler::zPosition(DGUS_VP &vp) {
  const float position = ExtUI::isAxisPositionKnown(ExtUI::Z) ? planner.get_axis_position_mm(Z_AXIS) : 0;
  const int32_t data = dgus.toFixedPoint<float, int32_t, 2>(int32_t(position * 50.0f) / 50.0f); // Round to 0.02
  dgus.write((uint16_t)vp.addr, dgus.swapBytes(data));
}

void DGUSTxHandler::elapsed(DGUS_VP &vp) {
  char buffer[21];
  duration_t(print_job_timer.duration()).toString(buffer);

  dgus.writeString((uint16_t)vp.addr, buffer, vp.size);
}

void DGUSTxHandler::percent(DGUS_VP &vp) {
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

  dgus.write((uint16_t)DGUS_Addr::STATUS_Percent, Swap16(progress));
}

void DGUSTxHandler::statusIcons(DGUS_VP &vp) {
  uint16_t icons = 0;

  if (ExtUI::isPrinting()) {
    icons |= (uint16_t)DGUS_Data::StatusIcon::PAUSE;

    dgus.enableControl(DGUS_ScreenID::PRINT_STATUS,
                               DGUSDisplay::POPUP_WINDOW,
                               DGUS_Control::PAUSE);
  }
  else {
    dgus.disableControl(DGUS_ScreenID::PRINT_STATUS,
                                DGUSDisplay::POPUP_WINDOW,
                                DGUS_Control::PAUSE);
  }

  if (ExtUI::isPrintingPaused()) {
    icons |= (uint16_t)DGUS_Data::StatusIcon::RESUME;

    dgus.enableControl(DGUS_ScreenID::PRINT_STATUS,
                               DGUSDisplay::POPUP_WINDOW,
                               DGUS_Control::RESUME);
  }
  else {
    dgus.disableControl(DGUS_ScreenID::PRINT_STATUS,
                                DGUSDisplay::POPUP_WINDOW,
                                DGUS_Control::RESUME);
  }

  dgus.write((uint16_t)vp.addr, Swap16(icons));
}

void DGUSTxHandler::flowrate(DGUS_VP &vp) {
  int16_t flowrate;

  switch (vp.addr) {
    default: return;
    case DGUS_Addr::ADJUST_Flowrate_CUR:
      flowrate = ExtUI::getFlow_percent(TERN(HAS_MULTI_EXTRUDER, ExtUI::getActiveTool(), ExtUI::E0));
      break;
    #if HAS_MULTI_EXTRUDER
      case DGUS_Addr::ADJUST_Flowrate_E0:
        flowrate = ExtUI::getFlow_percent(ExtUI::E0);
        break;
      case DGUS_Addr::ADJUST_Flowrate_E1:
        flowrate = ExtUI::getFlow_percent(ExtUI::E1);
        break;
    #endif
  }

  dgus.write((uint16_t)vp.addr, Swap16(flowrate));
}

void DGUSTxHandler::tempMax(DGUS_VP &vp) {
  uint16_t temp;

  switch (vp.addr) {
    default: return;
    case DGUS_Addr::TEMP_Max_Bed:
      temp = BED_MAX_TARGET;
      break;
    case DGUS_Addr::TEMP_Max_H0:
      temp = HEATER_0_MAXTEMP - HOTEND_OVERSHOOT;
      break;
    #if HAS_MULTI_HOTEND
      case DGUS_Addr::TEMP_Max_H1:
        temp = HEATER_1_MAXTEMP - HOTEND_OVERSHOOT;
        break;
    #endif
  }

  dgus.write((uint16_t)vp.addr, Swap16(temp));
}

void DGUSTxHandler::stepperStatus(DGUS_VP &vp) {
  const bool motor_on = stepper.axis_enabled.bits & (_BV(NUM_AXES) - 1);
  dgus.write((uint16_t)vp.addr, Swap16(motor_on ? DGUS_Data::Status::ENABLED : DGUS_Data::Status::DISABLED));
}

void DGUSTxHandler::stepIcons(DGUS_VP &vp) {
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

  dgus.write((uint16_t)vp.addr, Swap16(icons));
}

void DGUSTxHandler::ablDisableIcon(DGUS_VP &vp) {
  uint16_t data;

  if (ExtUI::getLevelingActive()) {
    data = (uint16_t)DGUS_Data::Status::ENABLED;

    dgus.enableControl(DGUS_ScreenID::LEVELING_AUTOMATIC,
                               DGUSDisplay::RETURN_KEY_CODE,
                               DGUS_Control::DISABLE);
  }
  else {
    data = (uint16_t)DGUS_Data::Status::DISABLED;

    dgus.disableControl(DGUS_ScreenID::LEVELING_AUTOMATIC,
                                DGUSDisplay::RETURN_KEY_CODE,
                                DGUS_Control::DISABLE);
  }

  dgus.write((uint16_t)vp.addr, Swap16(data));
}

void DGUSTxHandler::ablGrid(DGUS_VP &vp) {
  // Batch send
  int16_t data[DGUS_LEVEL_GRID_SIZE];
  xy_uint8_t point;
  int16_t fixed;

  for (int16_t i = 0; i < DGUS_LEVEL_GRID_SIZE; i++) {
    point.x = i % (GRID_MAX_POINTS_X);
    point.y = i / (GRID_MAX_POINTS_X);
    fixed = dgus.toFixedPoint<float, int16_t, 3>(ExtUI::getMeshPoint(point));
    data[i] = Swap16(fixed);
  }

  dgus.write((uint16_t)vp.addr, data, sizeof(*data) * DGUS_LEVEL_GRID_SIZE);
}

void DGUSTxHandler::filamentIcons(DGUS_VP &vp) {
  uint16_t icons = 0;

  switch (screen.filament_extruder) {
    default: return;
    case DGUS_Data::Extruder::CURRENT:
      #if HAS_MULTI_EXTRUDER
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

  dgus.write((uint16_t)vp.addr, Swap16(icons));
}

void DGUSTxHandler::blTouch(DGUS_VP &vp) {
  #if ENABLED(BLTOUCH)
    dgus.enableControl(DGUS_ScreenID::SETTINGS_MENU2,
                               DGUSDisplay::RETURN_KEY_CODE,
                               DGUS_Control::EXTRA2);

    dgus.write((uint16_t)vp.addr, Swap16(DGUS_Data::Status::ENABLED));
  #else
    dgus.disableControl(DGUS_ScreenID::SETTINGS_MENU2,
                                DGUSDisplay::RETURN_KEY_CODE,
                                DGUS_Control::EXTRA2);

    dgus.write((uint16_t)vp.addr, Swap16(DGUS_Data::Status::DISABLED));
  #endif
}

void DGUSTxHandler::pidIcons(DGUS_VP &vp) {
  uint16_t icons = 0;

  switch (screen.pid_heater) {
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

  dgus.write((uint16_t)vp.addr, Swap16(icons));
}

void DGUSTxHandler::pidKp(DGUS_VP &vp) {
  float value;

  switch (screen.pid_heater) {
    default: return;
    #if ENABLED(PIDTEMPBED)
      case DGUS_Data::Heater::BED:
        value = ExtUI::getBedPID_Kp();
        break;
    #endif
    #if ENABLED(PIDTEMP)
      case DGUS_Data::Heater::H0:
        value = ExtUI::getPID_Kp(ExtUI::E0);
        break;
      #if HAS_MULTI_HOTEND
        case DGUS_Data::Heater::H1:
          value = ExtUI::getPID_Kp(ExtUI::E1);
          break;
      #endif
    #endif
  }

  const int32_t data = dgus.toFixedPoint<float, int32_t, 2>(value);
  dgus.write((uint16_t)vp.addr, dgus.swapBytes(data));
}

void DGUSTxHandler::pidKi(DGUS_VP &vp) {
  float value;

  switch (screen.pid_heater) {
    default: return;
    #if ENABLED(PIDTEMPBED)
      case DGUS_Data::Heater::BED:
        value = ExtUI::getBedPID_Ki();
        break;
    #endif
    #if ENABLED(PIDTEMP)
      case DGUS_Data::Heater::H0:
        value = ExtUI::getPID_Ki(ExtUI::E0);
        break;
      #if HAS_MULTI_HOTEND
        case DGUS_Data::Heater::H1:
          value = ExtUI::getPID_Ki(ExtUI::E1);
          break;
      #endif
    #endif
  }

  const int32_t data = dgus.toFixedPoint<float, int32_t, 2>(value);
  dgus.write((uint16_t)vp.addr, dgus.swapBytes(data));
}

void DGUSTxHandler::pidKd(DGUS_VP &vp) {
  float value;

  switch (screen.pid_heater) {
    default: return;
    #if ENABLED(PIDTEMPBED)
      case DGUS_Data::Heater::BED:
        value = ExtUI::getBedPID_Kd();
        break;
    #endif
    #if ENABLED(PIDTEMP)
      case DGUS_Data::Heater::H0:
        value = ExtUI::getPID_Kd(ExtUI::E0);
        break;
      #if HAS_MULTI_HOTEND
        case DGUS_Data::Heater::H1:
          value = ExtUI::getPID_Kd(ExtUI::E1);
          break;
      #endif
    #endif
  }

  const int32_t data = dgus.toFixedPoint<float, int32_t, 2>(value);
  dgus.write((uint16_t)vp.addr, dgus.swapBytes(data));
}

void DGUSTxHandler::buildVolume(DGUS_VP &vp) {
  char buffer[vp.size];
  snprintf_P(buffer, vp.size, PSTR("%dx%dx%d"), X_BED_SIZE, Y_BED_SIZE, (Z_MAX_POS - Z_MIN_POS));

  dgus.writeString((uint16_t)vp.addr, buffer, vp.size);
}

void DGUSTxHandler::totalPrints(DGUS_VP &vp) {
  #if ENABLED(PRINTCOUNTER)
    dgus.write((uint16_t)vp.addr, dgus.swapBytes(print_job_timer.getStats().totalPrints));
  #else
    UNUSED(vp);
  #endif
}

void DGUSTxHandler::finishedPrints(DGUS_VP &vp) {
  #if ENABLED(PRINTCOUNTER)
    dgus.write((uint16_t)vp.addr, dgus.swapBytes(print_job_timer.getStats().finishedPrints));
  #else
    UNUSED(vp);
  #endif
}

void DGUSTxHandler::printTime(DGUS_VP &vp) {
  #if ENABLED(PRINTCOUNTER)
    char buffer[21];
    ExtUI::getTotalPrintTime_str(buffer);

    dgus.writeString((uint16_t)vp.addr, buffer, vp.size);
  #else
    dgus.writeString((uint16_t)vp.addr, F("-"), vp.size);
  #endif
}

void DGUSTxHandler::longestPrint(DGUS_VP &vp) {
  #if ENABLED(PRINTCOUNTER)
    char buffer[21];
    ExtUI::getLongestPrint_str(buffer);

    dgus.writeString((uint16_t)vp.addr, buffer, vp.size);
  #else
    dgus.writeString((uint16_t)vp.addr, F("-"), vp.size);
  #endif
}

void DGUSTxHandler::filamentUsed(DGUS_VP &vp) {
  #if ENABLED(PRINTCOUNTER)
    char buffer[21];
    ExtUI::getFilamentUsed_str(buffer);

    dgus.writeString((uint16_t)vp.addr, buffer, vp.size);
  #else
    dgus.writeString((uint16_t)vp.addr, F("-"), vp.size);
  #endif
}

void DGUSTxHandler::waitIcons(DGUS_VP &vp) {
  uint16_t icons = 0;

  if (ExtUI::isPrintingPaused()) {
    icons |= (uint16_t)DGUS_Data::WaitIcon::ABORT;

    dgus.enableControl(DGUS_ScreenID::WAIT,
                               DGUSDisplay::POPUP_WINDOW,
                               DGUS_Control::ABORT);
  }
  else {
    dgus.disableControl(DGUS_ScreenID::WAIT,
                                DGUSDisplay::POPUP_WINDOW,
                                DGUS_Control::ABORT);
  }

  if (screen.wait_continue) {
    icons |= (uint16_t)DGUS_Data::WaitIcon::CONTINUE;

    dgus.enableControl(DGUS_ScreenID::WAIT,
                               DGUSDisplay::RETURN_KEY_CODE,
                               DGUS_Control::CONTINUE);
  }
  else {
    dgus.disableControl(DGUS_ScreenID::WAIT,
                                DGUSDisplay::RETURN_KEY_CODE,
                                DGUS_Control::CONTINUE);
  }

  dgus.write((uint16_t)vp.addr, Swap16(icons));
}

void DGUSTxHandler::fanSpeed(DGUS_VP &vp) {
  uint16_t fan_speed;

  switch (vp.addr) {
    default: return;
    case DGUS_Addr::FAN0_Speed: fan_speed = ExtUI::getTargetFan_percent(ExtUI::FAN0); break;
  }

  dgus.write((uint16_t)vp.addr, Swap16(fan_speed));
}

void DGUSTxHandler::volume(DGUS_VP &vp) {
  const uint16_t volume = dgus.getVolume();

  dgus.write((uint16_t)vp.addr, Swap16(volume));
}

void DGUSTxHandler::brightness(DGUS_VP &vp) {
  const uint16_t brightness = dgus.getBrightness();

  dgus.write((uint16_t)vp.addr, Swap16(brightness));
}

void DGUSTxHandler::extraToString(DGUS_VP &vp) {
  if (!vp.size || !vp.extra) return;

  dgus.writeString((uint16_t)vp.addr, vp.extra, vp.size, true, false, false);
}

void DGUSTxHandler::extraPGMToString(DGUS_VP &vp) {
  if (!vp.size || !vp.extra) return;

  dgus.writeStringPGM((uint16_t)vp.addr, vp.extra, vp.size, true, false, false);
}

#endif // DGUS_LCD_UI_RELOADED
