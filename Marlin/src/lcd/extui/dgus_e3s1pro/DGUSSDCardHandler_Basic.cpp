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

#define DGUS_E3S1PRO_BASIC_SDCARD_MAX_FILES      20
#define DGUS_E3S1PRO_BASIC_SDCARD_FILES_PER_PAGE  5

#if ENABLED(DGUS_LCD_UI_E3S1PRO) && DISABLED(DGUS_ADVANCED_SDCARD)

#include "DGUSSDCardHandler.h"
#include "DGUSScreenHandler.h"
#include "DGUSDisplay.h"

static DGUS_SDCardHandler::page_t currentPage = DGUS_SDCardHandler::page_t::PAGE_1;
static size_t fileCount = 0;
static ExtUI::FileList fileList = ExtUI::FileList();

void DGUS_SDCardHandler::Reset() {
  currentPage = DGUS_SDCardHandler::page_t::PAGE_1;

  while (!fileList.isAtRootDir())
    fileList.upDir();

  fileCount = 0;
  uint16_t currentSeekPos = 0;
  uint16_t entriesCount = fileList.count();
  while (currentSeekPos < entriesCount
    && fileCount < DGUS_E3S1PRO_BASIC_SDCARD_MAX_FILES
    && fileList.seek(currentSeekPos, true)
  ) {
    if (!fileList.isDir()) fileCount++;
    currentSeekPos++;
  }

  #ifdef DEBUG_DGUSLCD
    DEBUG_ECHOPGM("Reset() :", fileCount);
    DEBUG_CHAR('/');
    DEBUG_ECHOLN(currentSeekPos);
  #endif
}

void DGUS_SDCardHandler::onPageLoad(DGUS_SDCardHandler::page_t page) {
  setFilename(file_t::FILE_1, NULL);
  setFilename(file_t::FILE_2, NULL);
  setFilename(file_t::FILE_3, NULL);
  setFilename(file_t::FILE_4, NULL);
  setFilename(file_t::FILE_5, NULL);

  uint8_t pageIndex = DGUS_PAGE_TO_INDEX(page);
  uint16_t currentFilePos = 0;
  uint16_t currentSeekPos = 0;
  uint16_t entriesCount = fileList.count();

  #ifdef DEBUG_DGUSLCD
    DEBUG_ECHOLNPGM("onPageLoad(): seek page ", pageIndex);
  #endif
  while (currentFilePos < pageIndex * DGUS_E3S1PRO_BASIC_SDCARD_FILES_PER_PAGE
    && fileList.seek(currentSeekPos, true)
    && currentSeekPos < entriesCount
  ) {
    ++currentSeekPos;
    if (!fileList.isDir())
      currentFilePos++;
  }

  #ifdef DEBUG_DGUSLCD
    DEBUG_ECHOPGM("onPageLoad() :", currentFilePos);
    DEBUG_CHAR('/');
    DEBUG_ECHOLN(currentSeekPos - 1);
  #endif

  file_t currentFile = file_t::FILE_1;
  while (currentFilePos < (pageIndex + 1) * DGUS_E3S1PRO_BASIC_SDCARD_FILES_PER_PAGE
    && fileList.seek(currentSeekPos, true)
    && currentSeekPos < entriesCount
  ) {
    ++currentSeekPos;
    if (!fileList.isDir()) {
      ++currentFilePos;
      setFilename(currentFile, fileList.longFilename());
      currentFile = DGUS_FILE_FROM_INDEX(DGUS_FILE_TO_INDEX(currentFile) + 1);
    }
  }
}

bool DGUS_SDCardHandler::onFileSelect(DGUS_SDCardHandler::file_t file) {
  uint8_t fileIndex = DGUS_FILE_TO_INDEX(file);
  if (!filenames[fileIndex][0])
    return false;

  screen.startPrintFromSD(filenames[fileIndex]);
  return true;
}

DGUS_SDCardHandler::page_t DGUS_SDCardHandler::onFirstPage() {
  currentPage = DGUS_SDCardHandler::page_t::PAGE_1;
  return currentPage;
}

DGUS_SDCardHandler::page_t DGUS_SDCardHandler::onLastPage() {
  currentPage = DGUS_PAGE_FROM_INDEX(fileCount / DGUS_E3S1PRO_BASIC_SDCARD_FILES_PER_PAGE);
  return currentPage;
}

DGUS_SDCardHandler::page_t DGUS_SDCardHandler::onPreviousPage() {
  if (currentPage > page_t::PAGE_1)
    currentPage = DGUS_PAGE_FROM_INDEX(DGUS_PAGE_TO_INDEX(currentPage) - 1);
  return currentPage;
}

DGUS_SDCardHandler::page_t DGUS_SDCardHandler::onNextPage() {
  if (DGUS_PAGE_TO_INDEX(currentPage) < (fileCount / DGUS_E3S1PRO_BASIC_SDCARD_FILES_PER_PAGE) - 1)
    currentPage = DGUS_PAGE_FROM_INDEX(DGUS_PAGE_TO_INDEX(currentPage) + 1);
  return currentPage;
}

#endif // DGUS_LCD_UI_E3S1PRO && !DGUS_ADVANCED_SDCARD
