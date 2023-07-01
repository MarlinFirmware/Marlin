/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#if ALL(DGUS_LCD_UI_E3S1PRO, DGUS_ADVANCED_SDCARD)

#include "DGUSSDCardHandler.h"
#include "DGUSScreenHandler.h"
#include "DGUSDisplay.h"

static DGUS_SDCardHandler::page_t currentPage = DGUS_SDCardHandler::page_t::PAGE_1;
static uint8_t currentVirtualPage = 0;
static size_t fileCount = 0;
static ExtUI::FileList fileList = ExtUI::FileList();

void DGUS_SDCardHandler::Reset() {
  currentPage = DGUS_SDCardHandler::page_t::PAGE_1;
  currentVirtualPage = 0;

  while (!fileList.isAtRootDir())
    fileList.upDir();

  fileList.refresh();
  fileCount = fileList.count();
}

void DGUS_SDCardHandler::onPageLoad(DGUS_SDCardHandler::page_t page) {
  setFilename(file_t::FILE_1, NULL);
  setFilename(file_t::FILE_2, NULL);
  setFilename(file_t::FILE_3, NULL);
  setFilename(file_t::FILE_4, NULL);
  setFilename(file_t::FILE_5, NULL);

  uint8_t pageIndex = currentVirtualPage;
  uint16_t currentSeekPos = 0;

  currentSeekPos += pageIndex * DGUS_E3S1PRO_BASIC_SDCARD_FILES_PER_PAGE;

  file_t currentFile = file_t::FILE_1;
  if (!fileList.isAtRootDir()) {
    if (currentPage == page_t::PAGE_1) {
      setFilename(currentFile, "-- dir up --");
      currentFile = DGUS_FILE_FROM_INDEX(DGUS_FILE_TO_INDEX(currentFile) + 1);
    }
    else
      currentSeekPos -= 1; // account for the updir entry on page 1
  }

  while (currentFile <= file_t::FILE_5
    && fileList.seek(currentSeekPos, true)
    && currentSeekPos < fileCount) {
    ++currentSeekPos;
    setFilename(currentFile, fileList.filename(), fileList.isDir());
    currentFile = DGUS_FILE_FROM_INDEX(DGUS_FILE_TO_INDEX(currentFile) + 1);
  }
}

bool DGUS_SDCardHandler::onFileSelect(DGUS_SDCardHandler::file_t file) {
  uint8_t fileIndex = DGUS_FILE_TO_INDEX(file);
  if (!filenames[fileIndex][0])
    return false;

  bool changeDir = false;
  if (!fileList.isAtRootDir()
    && currentPage == page_t::PAGE_1
    && file == file_t::FILE_1) {
    fileList.upDir();
    changeDir = true;
  }
  else if (filenames[fileIndex][strlen(filenames[fileIndex])-1] == '/') {
    filenames[fileIndex][strlen(filenames[fileIndex])-1] = '\0';
    fileList.changeDir(filenames[fileIndex]);
    changeDir = true;
  }

  if (changeDir) {
    fileList.refresh();
    currentPage = DGUS_SDCardHandler::page_t::PAGE_1;
    currentVirtualPage = 0;
    fileCount = fileList.count();

    if (currentPage == page_t::PAGE_1)
      onPageLoad(page_t::PAGE_1);
    else
      screen.triggerScreenChange(DGUS_ScreenID::FILE1);
    return true;
  }

  screen.startPrintFromSD(filenames[fileIndex]);
  return true;
}

DGUS_SDCardHandler::page_t DGUS_SDCardHandler::onFirstPage() {
  currentPage = DGUS_SDCardHandler::page_t::PAGE_1;
  currentVirtualPage = 0;
  return currentPage;
}

DGUS_SDCardHandler::page_t DGUS_SDCardHandler::onLastPage() {
  currentVirtualPage = (fileCount - 1 + (fileList.isAtRootDir() ? 0 : 1)) / DGUS_E3S1PRO_BASIC_SDCARD_FILES_PER_PAGE;

  if (currentVirtualPage >= 4) {
    currentPage = page_t::PAGE_4;
    onPageLoad(page_t::PAGE_4);
  }
  else
    currentPage = DGUS_PAGE_FROM_INDEX(currentVirtualPage);

  return currentPage;
}

DGUS_SDCardHandler::page_t DGUS_SDCardHandler::onPreviousPage() {
  if (currentVirtualPage > 0) {
    currentVirtualPage -= 1;

    if (currentVirtualPage >= 4)
      currentPage = page_t::PAGE_4;
    else
      currentPage = DGUS_PAGE_FROM_INDEX(currentVirtualPage);
  }
  return currentPage;
}

DGUS_SDCardHandler::page_t DGUS_SDCardHandler::onNextPage() {
  if (currentVirtualPage < (fileCount - 1 + (fileList.isAtRootDir() ? 0 : 1)) / DGUS_E3S1PRO_BASIC_SDCARD_FILES_PER_PAGE) {
    if (++currentVirtualPage >= 4) {
      currentPage = page_t::PAGE_4;
      onPageLoad(page_t::PAGE_4);
    }
    else
      currentPage = DGUS_PAGE_FROM_INDEX(currentVirtualPage);
  }

  return currentPage;
}

#endif // DGUS_LCD_UI_E3S1PRO && DGUS_ADVANCED_SDCARD
