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

#include "DGUSRxHandler.h"

#include "DGUSScreenHandler.h"
#include "config/DGUS_Screen.h"

#include "../ui_api.h"
#include "../../../core/language.h"
#include "../../../module/temperature.h"
#include "../../../module/printcounter.h"
#include "../../../gcode/queue.h"
#if ENABLED(ADVANCED_PAUSE_FEATURE)
  #include "../../../feature/pause.h"
#endif
#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../../feature/powerloss.h"
#endif

void DGUSRxHandler::ScreenChange(DGUS_VP &vp, void *data_ptr) {
  const DGUS_Screen screen = (DGUS_Screen)((uint8_t*)data_ptr)[1];

  if (vp.addr == DGUS_Addr::SCREENCHANGE_SD) {
    #if ENABLED(SDSUPPORT)
      #if !PIN_EXISTS(SD_DETECT)
        card.mount();
      #endif

      if (!ExtUI::isMediaInserted()) {
        dgus_screen_handler.SetStatusMessagePGM(GET_TEXT(MSG_NO_MEDIA));
        return;
      }

      card.cdroot();
    #else
      dgus_screen_handler.SetStatusMessagePGM(GET_TEXT(MSG_NO_MEDIA));
      return;
    #endif
  }

  if (vp.addr == DGUS_Addr::SCREENCHANGE_Idle
      && (printingIsActive() || printingIsPaused())) {
    dgus_screen_handler.SetStatusMessagePGM(PSTR("Impossible while printing"));
    return;
  }

  if (vp.addr == DGUS_Addr::SCREENCHANGE_Printing
      && (!printingIsActive() && !printingIsPaused())) {
    dgus_screen_handler.SetStatusMessagePGM(PSTR("Impossible while idle"));
    return;
  }

  dgus_screen_handler.TriggerScreenChange(screen);
}

#if ENABLED(SDSUPPORT)
  void DGUSRxHandler::Scroll(DGUS_VP &vp, void *data_ptr) {
    UNUSED(vp);

    const DGUS_Data::Scroll scroll = (DGUS_Data::Scroll)((uint8_t*)data_ptr)[1];

    switch (scroll) {
      case DGUS_Data::Scroll::GO_BACK:
        if (dgus_screen_handler.filelist.isAtRootDir()) {
          return;
        }

        dgus_screen_handler.filelist_offset = 0;
        dgus_screen_handler.filelist_selected = -1;
        dgus_screen_handler.filelist.upDir();
        break;
      case DGUS_Data::Scroll::UP:
        if (dgus_screen_handler.filelist_offset < 1) {
          return;
        }

        --dgus_screen_handler.filelist_offset;
        break;
      case DGUS_Data::Scroll::DOWN:
        if (dgus_screen_handler.filelist_offset + 1 + DGUS_FILE_COUNT > dgus_screen_handler.filelist.count()) {
          return;
        }

        ++dgus_screen_handler.filelist_offset;
        break;
    }

    dgus_screen_handler.TriggerFullUpdate();
  }

  void DGUSRxHandler::SelectFile(DGUS_VP &vp, void *data_ptr) {
    UNUSED(vp);

    const uint8_t index = ((uint8_t*)data_ptr)[1];

    if (!dgus_screen_handler.filelist.seek(dgus_screen_handler.filelist_offset + index)) {
      return;
    }

    if (dgus_screen_handler.filelist.isDir()) {
      dgus_screen_handler.filelist_offset = 0;
      dgus_screen_handler.filelist_selected = -1;
      dgus_screen_handler.filelist.changeDir(dgus_screen_handler.filelist.filename());
    }
    else {
      dgus_screen_handler.filelist_selected = dgus_screen_handler.filelist_offset + index;
    }

    dgus_screen_handler.TriggerFullUpdate();
  }

  void DGUSRxHandler::PrintFile(DGUS_VP &vp, void *data_ptr) {
    UNUSED(vp);
    UNUSED(data_ptr);

    if (dgus_screen_handler.filelist_selected < 0) {
      dgus_screen_handler.SetStatusMessagePGM(PSTR("No file selected"));
      return;
    }

    if (!dgus_screen_handler.filelist.seek(dgus_screen_handler.filelist_selected)
        || dgus_screen_handler.filelist.isDir()) {
      return;
    }

    if (!dgus_screen_handler.IsPrinterIdle()) {
      dgus_screen_handler.SetStatusMessagePGM(DGUS_MSG_BUSY);
      return;
    }

    ExtUI::printFile(dgus_screen_handler.filelist.shortFilename());
    dgus_screen_handler.TriggerScreenChange(DGUS_Screen::PRINT_STATUS);
  }
#endif // SDSUPPORT

void DGUSRxHandler::PrintAbort(DGUS_VP &vp, void *data_ptr) {
  UNUSED(vp);

  const DGUS_Data::Popup result = (DGUS_Data::Popup)((uint8_t*)data_ptr)[1];

  if (result != DGUS_Data::Popup::CONFIRMED) {
    return;
  }

  if (!printingIsActive() && !printingIsPaused()) {
    dgus_screen_handler.TriggerFullUpdate();
    return;
  }

  ExtUI::stopPrint();
}

void DGUSRxHandler::PrintPause(DGUS_VP &vp, void *data_ptr) {
  UNUSED(vp);

  const DGUS_Data::Popup result = (DGUS_Data::Popup)((uint8_t*)data_ptr)[1];

  if (result != DGUS_Data::Popup::CONFIRMED) {
    return;
  }

  if (!printingIsActive()) {
    dgus_screen_handler.TriggerFullUpdate();
    return;
  }

  ExtUI::pausePrint();
}

void DGUSRxHandler::PrintResume(DGUS_VP &vp, void *data_ptr) {
  UNUSED(vp);

  const DGUS_Data::Popup result = (DGUS_Data::Popup)((uint8_t*)data_ptr)[1];

  if (result != DGUS_Data::Popup::CONFIRMED) {
    return;
  }

  if (!printingIsPaused()) {
    dgus_screen_handler.TriggerFullUpdate();
    return;
  }

  if (!dgus_screen_handler.IsPrinterIdle()) {
    dgus_screen_handler.SetStatusMessagePGM(DGUS_MSG_BUSY);
    return;
  }

  ExtUI::resumePrint();
}

void DGUSRxHandler::Feedrate(DGUS_VP &vp, void *data_ptr) {
  UNUSED(vp);

  const int16_t feedrate = Swap16(*(int16_t*)data_ptr);

  ExtUI::setFeedrate_percent(feedrate);

  dgus_screen_handler.TriggerFullUpdate();
}

void DGUSRxHandler::Flowrate(DGUS_VP &vp, void *data_ptr) {
  const int16_t flowrate = Swap16(*(int16_t*)data_ptr);

  switch (vp.addr) {
    default: return;
    case DGUS_Addr::ADJUST_SetFlowrate_CUR:
      #if EXTRUDERS > 1
        ExtUI::setFlow_percent(flowrate, ExtUI::getActiveTool());
      #else
        ExtUI::setFlow_percent(flowrate, ExtUI::E0);
      #endif
      break;
    #if EXTRUDERS > 1
      case DGUS_Addr::ADJUST_SetFlowrate_E0:
        ExtUI::setFlow_percent(flowrate, ExtUI::E0);
        break;
      case DGUS_Addr::ADJUST_SetFlowrate_E1:
        ExtUI::setFlow_percent(flowrate, ExtUI::E1);
        break;
    #endif
  }

  dgus_screen_handler.TriggerFullUpdate();
}

void DGUSRxHandler::BabystepSet(DGUS_VP &vp, void *data_ptr) {
  UNUSED(vp);

  const int16_t data = Swap16(*(int16_t*)data_ptr);
  const float offset = dgus_display.FromFixedPoint<int16_t, float, 2>(data);

  const int16_t steps = ExtUI::mmToWholeSteps(offset - ExtUI::getZOffset_mm(), ExtUI::Z);

  ExtUI::smartAdjustAxis_steps(steps, ExtUI::Z, true);

  dgus_screen_handler.TriggerEEPROMSave();
  dgus_screen_handler.TriggerFullUpdate();
}

void DGUSRxHandler::Babystep(DGUS_VP &vp, void *data_ptr) {
  UNUSED(vp);

  const DGUS_Data::Adjust adjust = (DGUS_Data::Adjust)((uint8_t*)data_ptr)[1];
  int16_t steps;

  switch (adjust) {
    default: return;
    case DGUS_Data::Adjust::INCREMENT:
      steps = ExtUI::mmToWholeSteps(DGUS_PRINT_BABYSTEP, ExtUI::Z);
      break;
    case DGUS_Data::Adjust::DECREMENT:
      steps = ExtUI::mmToWholeSteps(-DGUS_PRINT_BABYSTEP, ExtUI::Z);
      break;
  }

  ExtUI::smartAdjustAxis_steps(steps, ExtUI::Z, true);

  dgus_screen_handler.TriggerEEPROMSave();
  dgus_screen_handler.TriggerFullUpdate();
}

void DGUSRxHandler::TempPreset(DGUS_VP &vp, void *data_ptr) {
  UNUSED(vp);

  const DGUS_Data::TempPreset preset = (DGUS_Data::TempPreset)((uint8_t*)data_ptr)[1];

  switch (preset) {
    case DGUS_Data::TempPreset::PLA:
      #if HOTENDS < 2
        ExtUI::setTargetTemp_celsius(DGUS_PLA_TEMP_HOTEND, ExtUI::H0);
      #else
        ExtUI::setTargetTemp_celsius(DGUS_PLA_TEMP_HOTEND, ExtUI::getActiveTool());
      #endif
      ExtUI::setTargetTemp_celsius(DGUS_PLA_TEMP_BED, ExtUI::BED);
      break;
    case DGUS_Data::TempPreset::ABS:
      #if HOTENDS < 2
        ExtUI::setTargetTemp_celsius(DGUS_ABS_TEMP_HOTEND, ExtUI::H0);
      #else
        ExtUI::setTargetTemp_celsius(DGUS_ABS_TEMP_HOTEND, ExtUI::getActiveTool());
      #endif
      ExtUI::setTargetTemp_celsius(DGUS_ABS_TEMP_BED, ExtUI::BED);
      break;
    case DGUS_Data::TempPreset::PETG:
      #if HOTENDS < 2
        ExtUI::setTargetTemp_celsius(DGUS_PETG_TEMP_HOTEND, ExtUI::H0);
      #else
        ExtUI::setTargetTemp_celsius(DGUS_PETG_TEMP_HOTEND, ExtUI::getActiveTool());
      #endif
      ExtUI::setTargetTemp_celsius(DGUS_PETG_TEMP_BED, ExtUI::BED);
      break;
  }

  dgus_screen_handler.TriggerFullUpdate();
}

void DGUSRxHandler::TempTarget(DGUS_VP &vp, void *data_ptr) {
  const int16_t temp = Swap16(*(int16_t*)data_ptr);

  switch (vp.addr) {
    default: return;
    case DGUS_Addr::TEMP_SetTarget_Bed:
      ExtUI::setTargetTemp_celsius(temp, ExtUI::BED);
      break;
    case DGUS_Addr::TEMP_SetTarget_H0:
      ExtUI::setTargetTemp_celsius(temp, ExtUI::H0);
      break;
    #if HOTENDS > 1
      case DGUS_Addr::TEMP_SetTarget_H1:
        ExtUI::setTargetTemp_celsius(temp, ExtUI::H1);
        break;
    #endif
  }

  dgus_screen_handler.TriggerFullUpdate();
}

void DGUSRxHandler::TempCool(DGUS_VP &vp, void *data_ptr) {
  UNUSED(vp);

  const DGUS_Data::Heater heater = (DGUS_Data::Heater)Swap16(*(uint16_t*)data_ptr);

  switch (heater) {
    default: return;
    case DGUS_Data::Heater::ALL:
      ExtUI::setTargetTemp_celsius(0, ExtUI::BED);
      ExtUI::setTargetTemp_celsius(0, ExtUI::H0);
      #if HOTENDS > 1
        ExtUI::setTargetTemp_celsius(0, ExtUI::H1);
      #endif
      break;
    case DGUS_Data::Heater::BED:
      ExtUI::setTargetTemp_celsius(0, ExtUI::BED);
      break;
    case DGUS_Data::Heater::H0:
      ExtUI::setTargetTemp_celsius(0, ExtUI::H0);
      break;
    #if HOTENDS > 1
      case DGUS_Data::Heater::H1:
        ExtUI::setTargetTemp_celsius(0, ExtUI::H1);
        break;
    #endif
  }

  dgus_screen_handler.SetStatusMessagePGM(PSTR("Cooling..."));

  dgus_screen_handler.TriggerFullUpdate();
}

void DGUSRxHandler::Steppers(DGUS_VP &vp, void *data_ptr) {
  UNUSED(vp);

  const DGUS_Data::Control control = (DGUS_Data::Control)((uint8_t*)data_ptr)[1];

  switch (control) {
    case DGUS_Data::Control::ENABLE:
      enable_all_steppers();
      break;
    case DGUS_Data::Control::DISABLE:
      disable_all_steppers();
      break;
  }

  dgus_screen_handler.TriggerFullUpdate();
}

void DGUSRxHandler::ZOffset(DGUS_VP &vp, void *data_ptr) {
  UNUSED(vp);

  if (!ExtUI::isAxisPositionKnown(ExtUI::Z)) {
    dgus_screen_handler.SetStatusMessagePGM(DGUS_MSG_HOMING_REQUIRED);
    return;
  }

  if (!dgus_screen_handler.IsPrinterIdle()) {
    dgus_screen_handler.SetStatusMessagePGM(DGUS_MSG_BUSY);
    return;
  }

  const int16_t data = Swap16(*(int16_t*)data_ptr);
  const float offset = dgus_display.FromFixedPoint<int16_t, float, 2>(data);

  const int16_t steps = ExtUI::mmToWholeSteps(offset - ExtUI::getZOffset_mm(), ExtUI::Z);

  ExtUI::smartAdjustAxis_steps(steps, ExtUI::Z, true);

  dgus_screen_handler.TriggerEEPROMSave();
  dgus_screen_handler.TriggerFullUpdate();
}

void DGUSRxHandler::ZOffsetStep(DGUS_VP &vp, void *data_ptr) {
  UNUSED(vp);

  if (!ExtUI::isAxisPositionKnown(ExtUI::Z)) {
    dgus_screen_handler.SetStatusMessagePGM(DGUS_MSG_HOMING_REQUIRED);
    return;
  }

  if (!dgus_screen_handler.IsPrinterIdle()) {
    dgus_screen_handler.SetStatusMessagePGM(DGUS_MSG_BUSY);
    return;
  }

  const DGUS_Data::Adjust adjust = (DGUS_Data::Adjust)((uint8_t*)data_ptr)[1];
  int16_t steps;

  switch (dgus_screen_handler.offset_steps) {
    default: return;
    case DGUS_Data::StepSize::MMP1:
      steps = ExtUI::mmToWholeSteps((adjust == DGUS_Data::Adjust::INCREMENT ? 0.1f : -0.1f), ExtUI::Z);
      break;
    case DGUS_Data::StepSize::MMP01:
      steps = ExtUI::mmToWholeSteps((adjust == DGUS_Data::Adjust::INCREMENT ? 0.01f : -0.01f), ExtUI::Z);
      break;
  }

  ExtUI::smartAdjustAxis_steps(steps, ExtUI::Z, true);

  dgus_screen_handler.TriggerEEPROMSave();
  dgus_screen_handler.TriggerFullUpdate();
}

void DGUSRxHandler::ZOffsetSetStep(DGUS_VP &vp, void *data_ptr) {
  UNUSED(vp);

  const DGUS_Data::StepSize size = (DGUS_Data::StepSize)((uint8_t*)data_ptr)[1];

  dgus_screen_handler.offset_steps = size;

  dgus_screen_handler.TriggerFullUpdate();
}

void DGUSRxHandler::MoveToPoint(DGUS_VP &vp, void *data_ptr) {
  UNUSED(vp);

  if (!ExtUI::isPositionKnown()) {
    dgus_screen_handler.SetStatusMessagePGM(DGUS_MSG_HOMING_REQUIRED);
    return;
  }

  if (!dgus_screen_handler.IsPrinterIdle()) {
    dgus_screen_handler.SetStatusMessagePGM(DGUS_MSG_BUSY);
    return;
  }

  const uint8_t point = ((uint8_t*)data_ptr)[1];
  constexpr float lfrb[4] = LEVEL_CORNERS_INSET_LFRB;
  float x, y;

  switch (point) {
    default: return;
    case 1:
      x = DGUS_LEVEL_CENTER_X;
      y = DGUS_LEVEL_CENTER_Y;
      break;
    case 2:
      x = X_MIN_POS + lfrb[0];
      y = Y_MIN_POS + lfrb[1];
      break;
    case 3:
      x = X_MAX_POS - lfrb[2];
      y = Y_MIN_POS + lfrb[1];
      break;
    case 4:
      x = X_MAX_POS - lfrb[2];
      y = Y_MAX_POS - lfrb[3];
      break;
    case 5:
      x = X_MIN_POS + lfrb[0];
      y = Y_MAX_POS - lfrb[3];
      break;
  }

  if (ExtUI::getAxisPosition_mm(ExtUI::Z) < Z_MIN_POS + LEVEL_CORNERS_Z_HOP) {
    ExtUI::setAxisPosition_mm(Z_MIN_POS + LEVEL_CORNERS_Z_HOP, ExtUI::Z);
  }
  ExtUI::setAxisPosition_mm(x, ExtUI::X);
  ExtUI::setAxisPosition_mm(y, ExtUI::Y);
  ExtUI::setAxisPosition_mm(Z_MIN_POS + LEVEL_CORNERS_HEIGHT, ExtUI::Z);
}

void DGUSRxHandler::Probe(DGUS_VP &vp, void *data_ptr) {
  UNUSED(vp);
  UNUSED(data_ptr);

  #if ENABLED(MESH_BED_LEVELING)
    dgus_screen_handler.SetStatusMessagePGM(DGUS_MSG_ABL_REQUIRED);
    return;
  #endif

  if (!ExtUI::isPositionKnown()) {
    dgus_screen_handler.SetStatusMessagePGM(DGUS_MSG_HOMING_REQUIRED);
    return;
  }

  if (!dgus_screen_handler.IsPrinterIdle()) {
    dgus_screen_handler.SetStatusMessagePGM(DGUS_MSG_BUSY);
    return;
  }

  dgus_screen_handler.TriggerScreenChange(DGUS_Screen::LEVELING_PROBING);

  #if ENABLED(AUTO_BED_LEVELING_UBL)
    queue.enqueue_now_P(PSTR("G29P1\nG29P3\nG29P5C"));
  #else
    queue.enqueue_now_P(PSTR("G29"));
  #endif
  queue.enqueue_now_P(DGUS_CMD_EEPROM_SAVE);
}

void DGUSRxHandler::DisableABL(DGUS_VP &vp, void *data_ptr) {
  UNUSED(vp);
  UNUSED(data_ptr);

  if (!dgus_screen_handler.IsPrinterIdle()) {
    dgus_screen_handler.SetStatusMessagePGM(DGUS_MSG_BUSY);
    return;
  }

  ExtUI::setLevelingActive(false);

  dgus_screen_handler.TriggerEEPROMSave();
  dgus_screen_handler.TriggerFullUpdate();
}

void DGUSRxHandler::FilamentSelect(DGUS_VP &vp, void *data_ptr) {
  UNUSED(vp);

  const DGUS_Data::Extruder extruder = (DGUS_Data::Extruder)Swap16(*(uint16_t*)data_ptr);

  switch (extruder) {
    default: return;
    case DGUS_Data::Extruder::CURRENT:
    case DGUS_Data::Extruder::E0:
    #if EXTRUDERS > 1
      case DGUS_Data::Extruder::E1:
    #endif
      dgus_screen_handler.filament_extruder = extruder;
      break;
  }

  dgus_screen_handler.TriggerFullUpdate();
}

void DGUSRxHandler::FilamentLength(DGUS_VP &vp, void *data_ptr) {
  UNUSED(vp);

  const uint16_t length = Swap16(*(uint16_t*)data_ptr);

  dgus_screen_handler.filament_length = constrain(length, 0, EXTRUDE_MAXLENGTH);

  dgus_screen_handler.TriggerFullUpdate();
}

void DGUSRxHandler::FilamentMove(DGUS_VP &vp, void *data_ptr) {
  UNUSED(vp);

  if (!dgus_screen_handler.IsPrinterIdle()) {
    dgus_screen_handler.SetStatusMessagePGM(DGUS_MSG_BUSY);
    return;
  }

  ExtUI::extruder_t extruder;

  switch (dgus_screen_handler.filament_extruder) {
    default: return;
    case DGUS_Data::Extruder::CURRENT:
      #if EXTRUDERS > 1
        extruder = ExtUI::getActiveTool();
        break;
      #endif
    case DGUS_Data::Extruder::E0:
      extruder = ExtUI::E0;
      break;
    #if EXTRUDERS > 1
      case DGUS_Data::Extruder::E1:
        extruder = ExtUI::E1;
        break;
    #endif
  }

  if (ExtUI::getActualTemp_celsius(extruder) < (float)EXTRUDE_MINTEMP) {
    dgus_screen_handler.SetStatusMessagePGM(PSTR("Temperature too low"));
    return;
  }

  const DGUS_Data::FilamentMove move = (DGUS_Data::FilamentMove)((uint8_t*)data_ptr)[1];

  switch (move) {
    case DGUS_Data::FilamentMove::RETRACT:
      UI_DECREMENT_BY(AxisPosition_mm, (float)dgus_screen_handler.filament_length, extruder);
      break;
    case DGUS_Data::FilamentMove::EXTRUDE:
      UI_INCREMENT_BY(AxisPosition_mm, (float)dgus_screen_handler.filament_length, extruder);
      break;
  }
}

void DGUSRxHandler::Home(DGUS_VP &vp, void *data_ptr) {
  UNUSED(vp);

  if (!dgus_screen_handler.IsPrinterIdle()) {
    dgus_screen_handler.SetStatusMessagePGM(DGUS_MSG_BUSY);
    return;
  }

  DGUS_Data::Axis axis = (DGUS_Data::Axis)((uint8_t*)data_ptr)[1];

  dgus_screen_handler.SetMessageLinePGM(NUL_STR, 1);
  dgus_screen_handler.SetMessageLinePGM(DGUS_MSG_HOMING, 2);
  dgus_screen_handler.SetMessageLinePGM(NUL_STR, 3);
  dgus_screen_handler.SetMessageLinePGM(NUL_STR, 4);
  dgus_screen_handler.ShowWaitScreen(dgus_screen_handler.GetCurrentScreen());

  switch (axis) {
    case DGUS_Data::Axis::X_Y_Z:
      queue.enqueue_now_P(PSTR("G28XYZ"));
      break;
    case DGUS_Data::Axis::X_Y:
      queue.enqueue_now_P(PSTR("G28XY"));
      break;
    case DGUS_Data::Axis::Z:
      queue.enqueue_now_P(PSTR("G28Z"));
      break;
  }
}

void DGUSRxHandler::Move(DGUS_VP &vp, void *data_ptr) {
  const int16_t data = Swap16(*(int16_t*)data_ptr);
  const float position = dgus_display.FromFixedPoint<int16_t, float, 1>(data);
  ExtUI::axis_t axis;

  switch (vp.addr) {
    default: return;
    case DGUS_Addr::MOVE_SetX:
      axis = ExtUI::X;
      break;
    case DGUS_Addr::MOVE_SetY:
      axis = ExtUI::Y;
      break;
    case DGUS_Addr::MOVE_SetZ:
      axis = ExtUI::Z;
      break;
  }

  if (!ExtUI::isAxisPositionKnown(axis)) {
    dgus_screen_handler.SetStatusMessagePGM(DGUS_MSG_HOMING_REQUIRED);
    return;
  }

  ExtUI::setAxisPosition_mm(position, axis);

  dgus_screen_handler.TriggerFullUpdate();
}

void DGUSRxHandler::MoveStep(DGUS_VP &vp, void *data_ptr) {
  UNUSED(vp);

  float offset;

  switch (dgus_screen_handler.move_steps) {
    default: return;
    case DGUS_Data::StepSize::MM10:
      offset = 10.0f;
      break;
    case DGUS_Data::StepSize::MM1:
      offset = 1.0f;
      break;
    case DGUS_Data::StepSize::MMP1:
      offset = 0.1f;
      break;
  }

  const DGUS_Data::MoveDirection direction = (DGUS_Data::MoveDirection)((uint8_t*)data_ptr)[1];
  ExtUI::axis_t axis;

  switch (direction) {
    default: return;
    case DGUS_Data::MoveDirection::XP:
      axis = ExtUI::X;
      break;
    case DGUS_Data::MoveDirection::XM:
      axis = ExtUI::X;
      offset = -offset;
      break;
    case DGUS_Data::MoveDirection::YP:
      axis = ExtUI::Y;
      break;
    case DGUS_Data::MoveDirection::YM:
      axis = ExtUI::Y;
      offset = -offset;
      break;
    case DGUS_Data::MoveDirection::ZP:
      axis = ExtUI::Z;
      break;
    case DGUS_Data::MoveDirection::ZM:
      axis = ExtUI::Z;
      offset = -offset;
      break;
  }

  if (!ExtUI::isAxisPositionKnown(axis)) {
    dgus_screen_handler.SetStatusMessagePGM(DGUS_MSG_HOMING_REQUIRED);
    return;
  }

  UI_INCREMENT_BY(AxisPosition_mm, offset, axis);

  dgus_screen_handler.TriggerFullUpdate();
}

void DGUSRxHandler::MoveSetStep(DGUS_VP &vp, void *data_ptr) {
  UNUSED(vp);

  const DGUS_Data::StepSize size = (DGUS_Data::StepSize)((uint8_t*)data_ptr)[1];

  dgus_screen_handler.move_steps = size;

  dgus_screen_handler.TriggerFullUpdate();
}

void DGUSRxHandler::GcodeClear(DGUS_VP &vp, void *data_ptr) {
  UNUSED(vp);
  UNUSED(data_ptr);

  ZERO(dgus_screen_handler.gcode);

  dgus_screen_handler.TriggerFullUpdate();
}

void DGUSRxHandler::GcodeExecute(DGUS_VP &vp, void *data_ptr) {
  UNUSED(vp);
  UNUSED(data_ptr);

  if (!strlen(dgus_screen_handler.gcode)) {
    return;
  }

  if (!dgus_screen_handler.IsPrinterIdle()) {
    dgus_screen_handler.SetStatusMessagePGM(DGUS_MSG_BUSY);
    return;
  }

  dgus_screen_handler.SetMessageLinePGM(NUL_STR, 1);
  dgus_screen_handler.SetMessageLinePGM(PSTR("Executing command..."), 2);
  dgus_screen_handler.SetMessageLinePGM(NUL_STR, 3);
  dgus_screen_handler.SetMessageLinePGM(NUL_STR, 4);
  dgus_screen_handler.ShowWaitScreen(DGUS_Screen::GCODE);

  queue.enqueue_one_now(dgus_screen_handler.gcode);
}

void DGUSRxHandler::ResetEEPROM(DGUS_VP &vp, void *data_ptr) {
  UNUSED(vp);

  const DGUS_Data::Popup result = (DGUS_Data::Popup)((uint8_t*)data_ptr)[1];

  if (result != DGUS_Data::Popup::CONFIRMED) {
    return;
  }

  if (!dgus_screen_handler.IsPrinterIdle()) {
    dgus_screen_handler.SetStatusMessagePGM(DGUS_MSG_BUSY);
    return;
  }

  queue.enqueue_now_P(PSTR("M502"));
  queue.enqueue_now_P(DGUS_CMD_EEPROM_SAVE);
}

void DGUSRxHandler::SettingsExtra(DGUS_VP &vp, void *data_ptr) {
  UNUSED(vp);

  const DGUS_Data::Extra extra = (DGUS_Data::Extra)((uint8_t*)data_ptr)[1];

  switch (extra) {
    default: return;
    case DGUS_Data::Extra::BUTTON1:
      #if ENABLED(BLTOUCH)
        if (!dgus_screen_handler.IsPrinterIdle()) {
          dgus_screen_handler.SetStatusMessagePGM(DGUS_MSG_BUSY);
          return;
        }

        queue.enqueue_now_P(PSTR(DGUS_RESET_BLTOUCH));
      #else
        dgus_screen_handler.TriggerScreenChange(DGUS_Screen::INFOS);
      #endif
      break;
    #if ENABLED(BLTOUCH)
      case DGUS_Data::Extra::BUTTON2:
        dgus_screen_handler.TriggerScreenChange(DGUS_Screen::INFOS);
        break;
    #endif
  }
}

void DGUSRxHandler::PIDSelect(DGUS_VP &vp, void *data_ptr) {
  UNUSED(vp);

  const DGUS_Data::Heater heater = (DGUS_Data::Heater)Swap16(*(uint16_t*)data_ptr);

  switch (heater) {
    default: return;
    case DGUS_Data::Heater::BED:
      dgus_screen_handler.pid_temp = DGUS_PLA_TEMP_BED;
      dgus_screen_handler.pid_heater = heater;
      break;
    case DGUS_Data::Heater::H0:
    #if HOTENDS > 1
      case DGUS_Data::Heater::H1:
    #endif
      dgus_screen_handler.pid_temp = DGUS_PLA_TEMP_HOTEND;
      dgus_screen_handler.pid_heater = heater;
      break;
  }

  dgus_screen_handler.pid_cycles = 5;

  dgus_screen_handler.TriggerFullUpdate();
}

void DGUSRxHandler::PIDSetTemp(DGUS_VP &vp, void *data_ptr) {
  UNUSED(vp);

  if (!dgus_screen_handler.IsPrinterIdle()) {
    dgus_screen_handler.SetStatusMessagePGM(DGUS_MSG_BUSY);
    return;
  }

  uint16_t temp = Swap16(*(uint16_t*)data_ptr);

  switch (dgus_screen_handler.pid_heater) {
    default: return;
    case DGUS_Data::Heater::BED:
      temp = constrain(temp, BED_MINTEMP, BED_MAX_TARGET);
      break;
    case DGUS_Data::Heater::H0:
      temp = constrain(temp, HEATER_0_MINTEMP, (HEATER_0_MAXTEMP - HOTEND_OVERSHOOT));
      break;
    #if HOTENDS > 1
      case DGUS_Data::Heater::H1:
        temp = constrain(temp, HEATER_1_MINTEMP, (HEATER_1_MAXTEMP - HOTEND_OVERSHOOT));
        break;
    #endif
  }

  dgus_screen_handler.pid_temp = temp;

  dgus_screen_handler.TriggerFullUpdate();
}

void DGUSRxHandler::PIDRun(DGUS_VP &vp, void *data_ptr) {
  UNUSED(vp);
  UNUSED(data_ptr);

  if (!dgus_screen_handler.IsPrinterIdle()) {
    dgus_screen_handler.SetStatusMessagePGM(DGUS_MSG_BUSY);
    return;
  }

  heater_id_t heater;
  uint8_t cycles = constrain(dgus_screen_handler.pid_cycles, 3, 10);

  switch (dgus_screen_handler.pid_heater) {
    default: return;
    case DGUS_Data::Heater::BED:
      #if ENABLED(PIDTEMPBED)
        heater = H_BED;
        break;
      #else
        dgus_screen_handler.SetStatusMessagePGM(PSTR("Bed PID disabled"));
        return;
      #endif
    case DGUS_Data::Heater::H0:
      #if ENABLED(PIDTEMP)
        heater = H_E0;
        break;
      #else
        dgus_screen_handler.SetStatusMessagePGM(PSTR("PID disabled"));
        return;
      #endif
    #if HOTENDS > 1
      case DGUS_Data::Heater::H1:
        #if ENABLED(PIDTEMP)
          heater = H_E1;
          break;
        #else
          dgus_screen_handler.SetStatusMessagePGM(PSTR("PID disabled"));
          return;
        #endif
    #endif
  }

  char buffer[24];
  snprintf_P(buffer, sizeof(buffer), PSTR("M303C%dE%dS%dU1"), cycles, heater, dgus_screen_handler.pid_temp);

  dgus_screen_handler.SetMessageLinePGM(NUL_STR, 1);
  dgus_screen_handler.SetMessageLinePGM(PSTR("PID autotuning..."), 2);
  dgus_screen_handler.SetMessageLinePGM(NUL_STR, 3);
  dgus_screen_handler.SetMessageLinePGM(NUL_STR, 4);
  dgus_screen_handler.ShowWaitScreen(DGUS_Screen::PID);

  queue.enqueue_one_now(buffer);
  queue.enqueue_now_P(DGUS_CMD_EEPROM_SAVE);
}

#if ENABLED(POWER_LOSS_RECOVERY)
  void DGUSRxHandler::PowerLossAbort(DGUS_VP &vp, void *data_ptr) {
    UNUSED(vp);

    const DGUS_Data::Popup result = (DGUS_Data::Popup)((uint8_t*)data_ptr)[1];

    if (result != DGUS_Data::Popup::CONFIRMED) {
      return;
    }

    if (!dgus_screen_handler.IsPrinterIdle()) {
      dgus_screen_handler.SetStatusMessagePGM(DGUS_MSG_BUSY);
      return;
    }

    dgus_screen_handler.TriggerScreenChange(DGUS_Screen::HOME);

    queue.enqueue_now_P(PSTR("M1000C"));
  }

  void DGUSRxHandler::PowerLossResume(DGUS_VP &vp, void *data_ptr) {
    UNUSED(vp);

    const DGUS_Data::Popup result = (DGUS_Data::Popup)((uint8_t*)data_ptr)[1];

    if (result != DGUS_Data::Popup::CONFIRMED) {
      return;
    }

    if (!dgus_screen_handler.IsPrinterIdle()) {
      dgus_screen_handler.SetStatusMessagePGM(DGUS_MSG_BUSY);
      return;
    }

    if (!recovery.valid()) {
      dgus_screen_handler.SetStatusMessagePGM(PSTR("Invalid recovery data"));
      return;
    }

    dgus_screen_handler.TriggerScreenChange(DGUS_Screen::PRINT_STATUS);

    queue.enqueue_now_P(PSTR("M1000"));
  }
#endif // POWER_LOSS_RECOVERY

void DGUSRxHandler::WaitAbort(DGUS_VP &vp, void *data_ptr) {
  UNUSED(vp);

  const DGUS_Data::Popup result = (DGUS_Data::Popup)((uint8_t*)data_ptr)[1];

  if (result != DGUS_Data::Popup::CONFIRMED) {
    return;
  }

  if (!printingIsPaused()
      #if ENABLED(ADVANCED_PAUSE_FEATURE)
        || !did_pause_print
      #endif
  ) {
    dgus_screen_handler.TriggerFullUpdate();
    return;
  }

  #if ENABLED(ADVANCED_PAUSE_FEATURE)
    did_pause_print = 0;
  #endif

  ExtUI::setUserConfirmed();
  ExtUI::stopPrint();

  dgus_screen_handler.TriggerFullUpdate();
}

void DGUSRxHandler::WaitContinue(DGUS_VP &vp, void *data_ptr) {
  UNUSED(vp);
  UNUSED(data_ptr);

  ExtUI::setUserConfirmed();

  dgus_screen_handler.TriggerFullUpdate();
}

void DGUSRxHandler::FanSpeed(DGUS_VP &vp, void *data_ptr) {
  uint8_t speed = ((uint8_t*)data_ptr)[1];
  switch (vp.addr) {
    default: return;
    case DGUS_Addr::FAN0_Speed:
      ExtUI::setTargetFan_percent(speed, ExtUI::FAN0);
      break;
  }
}

void DGUSRxHandler::Volume(DGUS_VP &vp, void *data_ptr) {
  UNUSED(vp);

  uint8_t volume = ((uint8_t*)data_ptr)[1];
  dgus_display.SetVolume(volume);

  dgus_screen_handler.TriggerEEPROMSave();
}

void DGUSRxHandler::Brightness(DGUS_VP &vp, void *data_ptr) {
  UNUSED(vp);

  uint8_t brightness = ((uint8_t*)data_ptr)[1];
  dgus_display.SetBrightness(brightness);

  dgus_screen_handler.TriggerEEPROMSave();
}

void DGUSRxHandler::Debug(DGUS_VP &vp, void *data_ptr) {
  UNUSED(vp);
  UNUSED(data_ptr);

  ++dgus_screen_handler.debug_count;

  if (dgus_screen_handler.debug_count >= 10) {
    dgus_screen_handler.TriggerScreenChange(DGUS_Screen::DEBUG);
  }
}

void DGUSRxHandler::StringToExtra(DGUS_VP &vp, void *data_ptr) {
  if (!vp.size || !vp.extra) return;
  memcpy(vp.extra, data_ptr, vp.size);
}

#endif // DGUS_LCD_UI_RELOADED
