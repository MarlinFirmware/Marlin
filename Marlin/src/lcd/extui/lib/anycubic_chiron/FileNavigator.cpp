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

/**
 * lcd/extui/lib/FileNavigator.cpp
 *
 * Extensible_UI implementation for Anycubic Chiron
 * Written By Nick Wells, 2020 [https://github.com/SwiftNick]
 *  (not affiliated with Anycubic, Ltd.)
 */

/***************************************************************************
 * The AC panel wants files in block of 4 and can only display a flat list *
 * This library allows full folder traversal.                              *
 ***************************************************************************/

#include "../../../../inc/MarlinConfigPre.h"

#if ENABLED(ANYCUBIC_LCD_CHIRON)

#include "FileNavigator.h"
#include "chiron_tft.h"

using namespace ExtUI;

namespace Anycubic {

  FileNavigator filenavigator;

  FileList  FileNavigator::filelist;                          // Instance of the Marlin file API
  char      FileNavigator::currentfoldername[MAX_PATH_LEN];   // Current folder path
  uint16_t  FileNavigator::lastindex;
  uint8_t   FileNavigator::folderdepth;
  uint16_t  FileNavigator::currentindex;                      // override the panel request

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
    uint8_t files = 4;
    if (index == 0) currentindex = 0;

    // Each time we change folder we reset the file index to 0 and keep track
    // of the current position as the TFT panel isnt aware of folders trees.
    if (index > 0) {
      --currentindex; // go back a file to take account off the .. we added to the root.
      if (index > lastindex)
        currentindex += files;
      else
        currentindex = currentindex < 4 ? 0 : currentindex - files;
    }
    lastindex = index;

    #if ACDEBUG(AC_FILE)
      SERIAL_ECHOLNPAIR("index=", index, " currentindex=", currentindex);
    #endif

    if (currentindex == 0 && folderdepth > 0) { // Add a link to go up a folder
      TFTSer.println("<<");
      TFTSer.println("..");
      files--;
    }

    for (uint16_t seek = currentindex; seek < currentindex + files; seek++) {
      if (filelist.seek(seek)) {
        sendFile();
        #if ACDEBUG(AC_FILE)
          SERIAL_ECHOLNPAIR("-", seek, " '", filelist.longFilename(), "' '", currentfoldername, "", filelist.shortFilename(), "'\n");
        #endif
      }
    }
  }

  void FileNavigator::sendFile() {
    // send the file and folder info to the panel
    // this info will be returned when the file is selected
    // Permitted special characters in file name -_*#~
    // Panel can display 22 characters per line
    if (filelist.isDir()) {
      //TFTSer.print(currentfoldername);
      TFTSer.println(filelist.shortFilename());
      TFTSer.print(filelist.shortFilename());
      TFTSer.println("/");
    }
    else {
      // Logical Name
      TFTSer.write('/');
      if (folderdepth > 0) TFTSer.print(currentfoldername);

      TFTSer.println(filelist.shortFilename());

      // Display Name
      TFTSer.println(filelist.longFilename());
    }
  }
  void FileNavigator::changeDIR(char *folder) {
    #if ACDEBUG(AC_FILE)
      SERIAL_ECHOLNPAIR("currentfolder: ", currentfoldername, "  New: ", folder);
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

      *(pos + 1) = '\0';
    }
    #if ACDEBUG(AC_FILE)
      SERIAL_ECHOLNPAIR("depth: ", folderdepth, " currentfoldername: ", currentfoldername);
    #endif
  }

  char* FileNavigator::getCurrentFolderName() { return currentfoldername; }
}

#endif // ANYCUBIC_LCD_CHIRON
