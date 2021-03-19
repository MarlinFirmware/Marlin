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

/* ****************************************
 * lcd/extui/lib/nextion/FileNavigator.cpp
 * ****************************************
 * Extensible_UI implementation for Nextion
 * https://github.com/Skorpi08
 * ***************************************/

#include "../../../../inc/MarlinConfigPre.h"

#if ENABLED(NEXTION_TFT)

#include "FileNavigator.h"
#include "nextion_tft.h"

using namespace ExtUI;

#define DEBUG_OUT NEXDEBUGLEVEL
#include "../../../../core/debug_out.h"

FileList  FileNavigator::filelist;                          // Instance of the Marlin file API
char      FileNavigator::currentfoldername[MAX_PATH_LEN];   // Current folder path
uint16_t  FileNavigator::lastindex;
uint8_t   FileNavigator::folderdepth;
uint16_t  FileNavigator::currentindex;                      // override the panel request

FileNavigator filenavigator;

FileNavigator::FileNavigator() { reset(); }

void FileNavigator::reset() {
  currentfoldername[0] = '\0';
  folderdepth  = 0;
  currentindex = 0;
  lastindex    = 0;
  // Start at root folder
  while (!filelist.isAtRootDir()) filelist.upDir();
  refresh();
}

void FileNavigator::refresh() { filelist.refresh(); }

void FileNavigator::getFiles(uint16_t index) {
  uint16_t files = 7, fseek = 0, fcnt  = 0;
  if (index == 0)
    currentindex = 0;
  else {
    // Each time we change folder we reset the file index to 0 and keep track
    // of the current position as the TFT panel isn't aware of folder trees.
    --currentindex; // go back a file to take account of the .. added to the root.
    if (index > lastindex)
      currentindex += files + 1;
    else if (currentindex >= files)
      currentindex -= files - 1;
    else
      currentindex = 0;
  }
  lastindex = index;

  #if NEXDEBUG(AC_FILE)
    DEBUG_ECHOLNPAIR("index=", index, " currentindex=", currentindex);
  #endif

  if (currentindex == 0 && folderdepth > 0) { // Add a link to go up a folder
    nextion.SendtoTFT(PSTR("vis p0,1"));
    nextion.SendtoTFT(PSTR("\xFF\xFF\xFF"));
    SEND_VAL("tmpUP", "0");
    files--;
  }
  else {
    nextion.SendtoTFT(PSTR("vis p0,0"));
    nextion.SendtoTFT(PSTR("\xFF\xFF\xFF"));
  }

  for (uint16_t seek = currentindex; seek < currentindex + files; seek++) {
    if (filelist.seek(seek)) {
      nextion.SendtoTFT(PSTR("s"));
      LCD_SERIAL.print(fcnt);
      nextion.SendtoTFT(PSTR(".txt=\""));
      if (filelist.isDir()) {
        LCD_SERIAL.print(filelist.shortFilename());
        nextion.SendtoTFT(PSTR("/\""));
        nextion.SendtoTFT(PSTR("\xFF\xFF\xFF"));

        nextion.SendtoTFT(PSTR("l"));
        LCD_SERIAL.print(fcnt);
        nextion.SendtoTFT(PSTR(".txt=\""));
        LCD_SERIAL.print(filelist.filename());
        nextion.SendtoTFT(PSTR("\""));
        nextion.SendtoTFT(PSTR("\xFF\xFF\xFF"));
        SEND_PCO2("l", fcnt, "1055");
      }
      else {
        LCD_SERIAL.print(currentfoldername);
        LCD_SERIAL.print(filelist.shortFilename());
        nextion.SendtoTFT(PSTR("\""));
        nextion.SendtoTFT(PSTR("\xFF\xFF\xFF"));

        nextion.SendtoTFT(PSTR("l"));
        LCD_SERIAL.print(fcnt);
        nextion.SendtoTFT(PSTR(".txt=\""));
        LCD_SERIAL.print(filelist.longFilename());
        nextion.SendtoTFT(PSTR("\""));
        nextion.SendtoTFT(PSTR("\xFF\xFF\xFF"));
      }
      fcnt++;
      fseek = seek;
      #if NEXDEBUG(AC_FILE)
        DEBUG_ECHOLNPAIR("-", seek, " '", filelist.longFilename(), "' '", currentfoldername, "", filelist.shortFilename(), "'\n");
      #endif
    }
  }
  SEND_VAL("n0", filelist.count());
  SEND_VAL("n1", fseek + 1);
}

void FileNavigator::changeDIR(char *folder) {
  #if NEXDEBUG(AC_FILE)
    DEBUG_ECHOLNPAIR("currentfolder: ", currentfoldername, "  New: ", folder);
  #endif
  if (folderdepth >= MAX_FOLDER_DEPTH) return; // limit the folder depth
  strcat(currentfoldername, folder);
  strcat(currentfoldername, "/");
  filelist.changeDir(folder);
  refresh();
  folderdepth++;
  currentindex = 0;
}

void FileNavigator::upDIR() {
  filelist.upDir();
  refresh();
  folderdepth--;
  currentindex = 0;
  // Remove the last child folder from the stored path
  if (folderdepth == 0) {
    currentfoldername[0] = '\0';
    reset();
  }
  else {
    char *pos = nullptr;
    for (uint8_t f = 0; f < folderdepth; f++)
      pos = strchr(currentfoldername, '/');
    pos[1] = '\0';
  }
  #if NEXDEBUG(AC_FILE)
    DEBUG_ECHOLNPAIR("depth: ", folderdepth, " currentfoldername: ", currentfoldername);
  #endif
}

char* FileNavigator::getCurrentFolderName() { return currentfoldername; }

#endif // NEXTION_TFT
