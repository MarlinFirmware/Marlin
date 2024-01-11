/**
 * Print Stats page for PRO UI
 * Author: Miguel A. Risco-Castillo (MRISCOC)
 * Version: 2.1.0
 * Date: 2023/07/12
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "../../../inc/MarlinConfigPre.h"

#if ALL(DWIN_LCD_PROUI, PRINTCOUNTER)

#include "printstats.h"

#include "../../../core/types.h"
#include "../../../MarlinCore.h"
#include "../../marlinui.h"
#include "../../../module/printcounter.h"
#include "dwin.h"
#include "dwin_popup.h"

PrintStats printStats;

void PrintStats::draw() {
  char str[30] = "";
  constexpr int8_t MRG = 30;

  title.draw(GET_TEXT_F(MSG_INFO_STATS_MENU));
  DWINUI::clearMainArea();
  drawPopupBkgd();
  DWINUI::drawButton(BTN_Continue, 86, 250);
  printStatistics ps = print_job_timer.getStats();

  DWINUI::drawString(MRG,  80, TS(GET_TEXT_F(MSG_INFO_PRINT_COUNT), F(": "), ps.totalPrints));
  DWINUI::drawString(MRG, 100, TS(GET_TEXT_F(MSG_INFO_COMPLETED_PRINTS), F(": "), ps.finishedPrints));
  duration_t(print_job_timer.getStats().printTime).toDigital(str, true);
  DWINUI::drawString(MRG, 120, MString<50>(GET_TEXT_F(MSG_INFO_PRINT_TIME), F(": "), str));
  duration_t(print_job_timer.getStats().longestPrint).toDigital(str, true);
  DWINUI::drawString(MRG, 140, MString<50>(GET_TEXT(MSG_INFO_PRINT_LONGEST), F(": "), str));
  DWINUI::drawString(MRG, 160, TS(GET_TEXT_F(MSG_INFO_PRINT_FILAMENT), F(": "), p_float_t(ps.filamentUsed / 1000, 2), F(" m")));
}

void PrintStats::reset() {
  print_job_timer.initStats();
  DONE_BUZZ(true);
}

void gotoPrintStats() {
  printStats.draw();
  hmiSaveProcessID(ID_WaitResponse);
}

// Print Stats Reset popup
void popupResetStats() { dwinPopupConfirmCancel(ICON_Info_0, GET_TEXT_F(MSG_RESET_STATS)); }
void onClickResetStats() {
  if (hmiFlag.select_flag) printStats.reset();
  hmiReturnScreen();
}
void printStatsReset() { gotoPopup(popupResetStats, onClickResetStats); }

#endif // DWIN_LCD_PROUI && PRINTCOUNTER
