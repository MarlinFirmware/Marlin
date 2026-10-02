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

#include "../../../../inc/MarlinConfigPre.h"

#if ENABLED(DGUS_LCD_UI_E3V2_TOUCH)

#include "DGUS_VPList.h"

#include "../config/DGUS_Addr.h"
#include "../DGUSScreenHandler.h"
#include "../DGUSReturnKeyCodeHandler.h"
#include "../DGUSRxHandler.h"
#include "../DGUSTxHandler.h"

#include "../../ui_api.h"

const char DGUS_MARLINVERSION[] PROGMEM = SHORT_BUILD_VERSION;
const char DGUS_BEDSIZE[] PROGMEM = DGUS_BED_SIZE_STR;
const char DGUS_WEBSITE[] PROGMEM = "marlinfw.org";

#define VP_HELPER(ADDR, SIZE, FLAGS, EXTRA, RXHANDLER, TXHANDLER) \
  { .addr = ADDR, \
  .size = SIZE, \
  .flags = FLAGS, \
  .extra = EXTRA, \
  .rx_handler = RXHANDLER, \
  .tx_handler = TXHANDLER }

#define VP_HELPER_WORD(ADDR, FLAGS, EXTRA, RXHANDLER, TXHANDLER) \
  VP_HELPER(ADDR, 2, FLAGS, EXTRA, RXHANDLER, TXHANDLER)

#define VP_HELPER_RX(ADDR, RXHANDLER) \
  VP_HELPER_WORD(ADDR, VPFLAG_NONE, nullptr, RXHANDLER, nullptr)

#define VP_HELPER_TX(ADDR, TXHANDLER) \
  VP_HELPER_WORD(ADDR, VPFLAG_NONE, nullptr, nullptr, TXHANDLER)

#define VP_HELPER_TX_AUTO(ADDR, TXHANDLER) \
  VP_HELPER_WORD(ADDR, VPFLAG_AUTOUPLOAD, nullptr, nullptr, TXHANDLER)

#define VP_HELPER_RW_AUTO(ADDR, HANDLER) \
  VP_HELPER_WORD(ADDR, VPFLAG_AUTOUPLOAD, nullptr, &DGUSRxHandler::HANDLER, &DGUSTxHandler::HANDLER)

#define VP_HELPER_PRESET(ADDR, FIELD) \
  VP_HELPER_WORD(ADDR, VPFLAG_NONE, &screen.config.FIELD, \
    &DGUSRxHandler::integerToExtra<uint16_t>, &DGUSTxHandler::extraToInteger<uint16_t>)

#define VP_HELPER_PGM_STRING(ADDR, STR) \
  VP_HELPER(ADDR, DGUS_INFOSTRING_LEN, VPFLAG_NONE, VP_EXTRA_TO_STR(STR), nullptr, &DGUSTxHandler::extraPGMToString)

#define VP_HELPER_LANG_ICON(ADDR) \
  VP_HELPER_WORD(ADDR, VPFLAG_NONE, &screen.config.language, nullptr, &DGUSTxHandler::extraToInteger<uint16_t>)

#define VP_HELPER_FILENAME(ADDR, INDEX) \
  VP_HELPER(ADDR, DGUS_FILENAME_LEN, VPFLAG_NONE, VP_EXTRA_TO_STR(DGUS_SDCardHandler::filenames[INDEX]), \
    nullptr, &DGUSTxHandler::extraToString)

const struct DGUS_VP vp_list[] PROGMEM = {
  VP_HELPER_TX_AUTO(DGUS_Addr::BOOT_Logo, &DGUSTxHandler::bootAnimation),
  VP_HELPER_RX(DGUS_Addr::CMD_MenuSelect, &DGUSReturnKeyCodeHandler::Command_MenuSelect),
  VP_HELPER_RX(DGUS_Addr::CMD_Adjust, &DGUSReturnKeyCodeHandler::Command_Adjust),
  VP_HELPER_RW_AUTO(DGUS_Addr::MAIN_PrintSpeedPercentage, printSpeedPercentage),
  VP_HELPER_RX(DGUS_Addr::CMD_Stop, &DGUSReturnKeyCodeHandler::Command_Stop),
  VP_HELPER_RX(DGUS_Addr::CMD_Pause, &DGUSReturnKeyCodeHandler::Command_Pause),
  VP_HELPER_RX(DGUS_Addr::CMD_Resume, &DGUSReturnKeyCodeHandler::Command_Resume),
  VP_HELPER_TX_AUTO(DGUS_Addr::MAIN_Icon_Percentage, &DGUSTxHandler::printPercentageIcon),
  VP_HELPER_TX_AUTO(DGUS_Addr::MAIN_ElapsedHours, &DGUSTxHandler::elapsedHours),
  VP_HELPER_TX_AUTO(DGUS_Addr::MAIN_ElapsedMinutes, &DGUSTxHandler::elapsedMinutes),
  VP_HELPER_TX_AUTO(DGUS_Addr::MAIN_PrintPercentage, &DGUSTxHandler::printPercentage),
  VP_HELPER_TX_AUTO(DGUS_Addr::ADJUST_Icon_Fan, &DGUSTxHandler::fanIcon),
  VP_HELPER_RW_AUTO(DGUS_Addr::MAIN_ZOffset, zOffset),
  VP_HELPER_RX(DGUS_Addr::CMD_TemperatureMenu, &DGUSReturnKeyCodeHandler::Command_TemperatureMenu),
  VP_HELPER_RX(DGUS_Addr::CMD_Cooldown, &DGUSReturnKeyCodeHandler::Command_Cooldown),
  VP_HELPER_RW_AUTO(DGUS_Addr::MAIN_ExtruderTargetTemp, extruderTargetTemp),
  VP_HELPER_TX_AUTO(DGUS_Addr::MAIN_ExtruderCurrentTemp, &DGUSTxHandler::extruderCurrentTemp),
  VP_HELPER_RW_AUTO(DGUS_Addr::MAIN_BedTargetTemp, bedTargetTemp),
  VP_HELPER_TX_AUTO(DGUS_Addr::MAIN_BedCurrentTemp, &DGUSTxHandler::bedCurrentTemp),
  VP_HELPER_RX(DGUS_Addr::CMD_ControlMenu, &DGUSReturnKeyCodeHandler::Command_ControlMenu),
  VP_HELPER_RX(DGUS_Addr::CMD_Leveling, &DGUSReturnKeyCodeHandler::Command_Leveling),
  VP_HELPER_RX(DGUS_Addr::CMD_AxisControl, &DGUSReturnKeyCodeHandler::Command_AxisControl),
  VP_HELPER_RW_AUTO(DGUS_Addr::AXIS_X, axis_X),
  VP_HELPER_RW_AUTO(DGUS_Addr::AXIS_Y, axis_Y),
  VP_HELPER_RW_AUTO(DGUS_Addr::AXIS_Z, axis_Z),
  VP_HELPER_WORD(DGUS_Addr::IO_FilamentLength, VPFLAG_NONE, nullptr, &DGUSRxHandler::filamentLength, &DGUSTxHandler::filamentLength),
  VP_HELPER_RX(DGUS_Addr::CMD_FilamentIO, &DGUSReturnKeyCodeHandler::Command_FilamentIO),
  VP_HELPER_RX(DGUS_Addr::CMD_LanguageMenu, &DGUSReturnKeyCodeHandler::Command_LanguageMenu),
  VP_HELPER_RX(DGUS_Addr::CMD_PowerLoss, &DGUSReturnKeyCodeHandler::Command_PowerLoss),
  VP_HELPER_PGM_STRING(DGUS_Addr::INFO_FW_Version, DGUS_MARLINVERSION),
  VP_HELPER_PGM_STRING(DGUS_Addr::INFO_Print_Size, DGUS_BEDSIZE),
  VP_HELPER_PGM_STRING(DGUS_Addr::INFO_Website, DGUS_WEBSITE),
  VP_HELPER_TX_AUTO(DGUS_Addr::LEVELING_Progress_Icon, &DGUSTxHandler::levelingProgressIcon),
  VP_HELPER_TX_AUTO(DGUS_Addr::FILAMENT_Icon_Missing, &DGUSTxHandler::filamentMissingIcon),
  VP_HELPER_PRESET(DGUS_Addr::TEMP_PLA_ExtruderTemp, plaExtruderTemp),
  VP_HELPER_PRESET(DGUS_Addr::TEMP_PLA_BedTemp, plaBedTemp),
  VP_HELPER_PRESET(DGUS_Addr::TEMP_PLA_FanSpeed, plaFanSpeed),
  VP_HELPER_PRESET(DGUS_Addr::TEMP_ABS_ExtruderTemp, absExtruderTemp),
  VP_HELPER_PRESET(DGUS_Addr::TEMP_ABS_BedTemp, absBedTemp),
  VP_HELPER_PRESET(DGUS_Addr::TEMP_ABS_FanSpeed, absFanSpeed),
  VP_HELPER_RX(DGUS_Addr::CMD_Refresh, &DGUSRxHandler::refresh),
  VP_HELPER(DGUS_Addr::ABNORMAL_StatusMessage, DGUS_ERRORSTRING_LEN, VPFLAG_NONE, VP_EXTRA_TO_STR(screen.statusMessage), nullptr, &DGUSTxHandler::extraToString),
  VP_HELPER_RX(DGUS_Addr::CMD_Acknowledge, &DGUSReturnKeyCodeHandler::Command_Acknowledge),
  VP_HELPER_RX(DGUS_Addr::CMD_SetLanguage, &DGUSRxHandler::setLanguage),
  VP_HELPER_TX_AUTO(DGUS_Addr::AXIS_StepperStatus, &DGUSTxHandler::stepperStatus),
  VP_HELPER_TX(DGUS_Addr::SDCARD_Selection_File1, &DGUSTxHandler::fileSelectionIcon),
  VP_HELPER_TX(DGUS_Addr::SDCARD_Selection_File2, &DGUSTxHandler::fileSelectionIcon),
  VP_HELPER_TX(DGUS_Addr::SDCARD_Selection_File3, &DGUSTxHandler::fileSelectionIcon),
  VP_HELPER_TX(DGUS_Addr::SDCARD_Selection_File4, &DGUSTxHandler::fileSelectionIcon),
  VP_HELPER_TX(DGUS_Addr::SDCARD_Selection_File5, &DGUSTxHandler::fileSelectionIcon),
  VP_HELPER_TX(DGUS_Addr::SDCARD_Selection_File6, &DGUSTxHandler::fileSelectionIcon),
  VP_HELPER_TX(DGUS_Addr::SDCARD_Selection_File7, &DGUSTxHandler::fileSelectionIcon),
  VP_HELPER_TX(DGUS_Addr::SDCARD_Selection_File8, &DGUSTxHandler::fileSelectionIcon),
  VP_HELPER_TX(DGUS_Addr::SDCARD_Selection_File9, &DGUSTxHandler::fileSelectionIcon),
  VP_HELPER_TX(DGUS_Addr::SDCARD_Selection_File10, &DGUSTxHandler::fileSelectionIcon),
  VP_HELPER_TX(DGUS_Addr::SDCARD_Selection_File11, &DGUSTxHandler::fileSelectionIcon),
  VP_HELPER_TX(DGUS_Addr::SDCARD_Selection_File12, &DGUSTxHandler::fileSelectionIcon),
  VP_HELPER_TX(DGUS_Addr::SDCARD_Selection_File13, &DGUSTxHandler::fileSelectionIcon),
  VP_HELPER_TX(DGUS_Addr::SDCARD_Selection_File14, &DGUSTxHandler::fileSelectionIcon),
  VP_HELPER_TX(DGUS_Addr::SDCARD_Selection_File15, &DGUSTxHandler::fileSelectionIcon),
  VP_HELPER_TX(DGUS_Addr::SDCARD_Selection_File16, &DGUSTxHandler::fileSelectionIcon),
  VP_HELPER_TX(DGUS_Addr::SDCARD_Selection_File17, &DGUSTxHandler::fileSelectionIcon),
  VP_HELPER_TX(DGUS_Addr::SDCARD_Selection_File18, &DGUSTxHandler::fileSelectionIcon),
  VP_HELPER_TX(DGUS_Addr::SDCARD_Selection_File19, &DGUSTxHandler::fileSelectionIcon),
  VP_HELPER_TX(DGUS_Addr::SDCARD_Selection_File20, &DGUSTxHandler::fileSelectionIcon),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_1300),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_1301),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_1302),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_1303),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_1304),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_1305),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_1306),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_1307),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_1308),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_1309),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_130A),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_130B),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_130C),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_130D),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_130E),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_130F),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_1310),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_1311),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_1312),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_1314),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_1315),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_1316),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_1317),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_1318),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_1319),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_131A),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_131B),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_131C),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_131D),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_131E),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_131F),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_1320),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_1321),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_1322),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_1323),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_1324),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_1326),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_1327),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_1328),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_1329),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_132A),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_132B),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_132C),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_132D),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_132E),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_132F),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_1330),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_1331),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_1332),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_1333),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_1334),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_1335),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_1336),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_1337),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_1338),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_1339),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_133A),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_133B),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_133C),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_133D),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_133E),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_133F),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_1340),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_1341),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_1342),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_1343),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_1344),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_1345),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_1346),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_1347),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_1348),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_1349),
  VP_HELPER_LANG_ICON(DGUS_Addr::LANG_Icon_134C),
  VP_HELPER(DGUS_Addr::PRINT_Filename, DGUS_FILENAME_LEN, VPFLAG_NONE, nullptr, nullptr, &DGUSTxHandler::printFilename),
  VP_HELPER_FILENAME(DGUS_Addr::SDCARD_Filename1, 0),
  VP_HELPER_FILENAME(DGUS_Addr::SDCARD_Filename2, 1),
  VP_HELPER_FILENAME(DGUS_Addr::SDCARD_Filename3, 2),
  VP_HELPER_FILENAME(DGUS_Addr::SDCARD_Filename4, 3),
  VP_HELPER_FILENAME(DGUS_Addr::SDCARD_Filename5, 4),
  VP_HELPER_FILENAME(DGUS_Addr::SDCARD_Filename6, 5),
  VP_HELPER_FILENAME(DGUS_Addr::SDCARD_Filename7, 6),
  VP_HELPER_FILENAME(DGUS_Addr::SDCARD_Filename8, 7),
  VP_HELPER_FILENAME(DGUS_Addr::SDCARD_Filename9, 8),
  VP_HELPER_FILENAME(DGUS_Addr::SDCARD_Filename10, 9),
  VP_HELPER_FILENAME(DGUS_Addr::SDCARD_Filename11, 10),
  VP_HELPER_FILENAME(DGUS_Addr::SDCARD_Filename12, 11),
  VP_HELPER_FILENAME(DGUS_Addr::SDCARD_Filename13, 12),
  VP_HELPER_FILENAME(DGUS_Addr::SDCARD_Filename14, 13),
  VP_HELPER_FILENAME(DGUS_Addr::SDCARD_Filename15, 14),
  VP_HELPER_FILENAME(DGUS_Addr::SDCARD_Filename16, 15),
  VP_HELPER_FILENAME(DGUS_Addr::SDCARD_Filename17, 16),
  VP_HELPER_FILENAME(DGUS_Addr::SDCARD_Filename18, 17),
  VP_HELPER_FILENAME(DGUS_Addr::SDCARD_Filename19, 18),
  VP_HELPER_FILENAME(DGUS_Addr::SDCARD_Filename20, 19),
  VP_HELPER_RX(DGUS_Addr::CMD_FilelistControl, &DGUSReturnKeyCodeHandler::Command_FilelistControl),
  VP_HELPER_RX(DGUS_Addr::SDCARD_FileSelection, &DGUSReturnKeyCodeHandler::Command_FileSelect),
  VP_HELPER_TX(DGUS_Addr::SDCARD_Color_File1, &DGUSTxHandler::fileSelectionColor),
  VP_HELPER_TX(DGUS_Addr::SDCARD_Color_File2, &DGUSTxHandler::fileSelectionColor),
  VP_HELPER_TX(DGUS_Addr::SDCARD_Color_File3, &DGUSTxHandler::fileSelectionColor),
  VP_HELPER_TX(DGUS_Addr::SDCARD_Color_File4, &DGUSTxHandler::fileSelectionColor),
  VP_HELPER_TX(DGUS_Addr::SDCARD_Color_File5, &DGUSTxHandler::fileSelectionColor),
  VP_HELPER_TX(DGUS_Addr::SDCARD_Color_File6, &DGUSTxHandler::fileSelectionColor),
  VP_HELPER_TX(DGUS_Addr::SDCARD_Color_File7, &DGUSTxHandler::fileSelectionColor),
  VP_HELPER_TX(DGUS_Addr::SDCARD_Color_File8, &DGUSTxHandler::fileSelectionColor),
  VP_HELPER_TX(DGUS_Addr::SDCARD_Color_File9, &DGUSTxHandler::fileSelectionColor),
  VP_HELPER_TX(DGUS_Addr::SDCARD_Color_File10, &DGUSTxHandler::fileSelectionColor),
  VP_HELPER_TX(DGUS_Addr::SDCARD_Color_File11, &DGUSTxHandler::fileSelectionColor),
  VP_HELPER_TX(DGUS_Addr::SDCARD_Color_File12, &DGUSTxHandler::fileSelectionColor),
  VP_HELPER_TX(DGUS_Addr::SDCARD_Color_File13, &DGUSTxHandler::fileSelectionColor),
  VP_HELPER_TX(DGUS_Addr::SDCARD_Color_File14, &DGUSTxHandler::fileSelectionColor),
  VP_HELPER_TX(DGUS_Addr::SDCARD_Color_File15, &DGUSTxHandler::fileSelectionColor),
  VP_HELPER_TX(DGUS_Addr::SDCARD_Color_File16, &DGUSTxHandler::fileSelectionColor),
  VP_HELPER_TX(DGUS_Addr::SDCARD_Color_File17, &DGUSTxHandler::fileSelectionColor),
  VP_HELPER_TX(DGUS_Addr::SDCARD_Color_File18, &DGUSTxHandler::fileSelectionColor),
  VP_HELPER_TX(DGUS_Addr::SDCARD_Color_File19, &DGUSTxHandler::fileSelectionColor),
  VP_HELPER_TX(DGUS_Addr::SDCARD_Color_File20, &DGUSTxHandler::fileSelectionColor),
  VP_HELPER((DGUS_Addr)0, 0, VPFLAG_NONE, nullptr, nullptr, nullptr)
};

#endif // DGUS_LCD_UI_E3V2_TOUCH
