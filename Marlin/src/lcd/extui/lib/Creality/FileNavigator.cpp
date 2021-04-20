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



#include "FileNavigator.h"

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


bool FileNavigator::getIndexisDir(uint16_t index){
  filelist.seek(index);
  return filelist.isDir();
}

const char *FileNavigator::getIndexName(uint16_t index){
  filelist.seek(index);
  return filelist.shortFilename();
}

void FileNavigator::getFiles(uint16_t index) {
  uint16_t files = DISPLAY_FILES, fseek = 0, fcnt  = 0;
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

  DEBUG_ECHOLNPAIR("index=", index, " currentindex=", currentindex);

  if (currentindex == 0 && folderdepth > 0) { // Add a link to go up a folder
    files--;
    rtscheck.RTS_SndData(filelist.shortFilename(), SDFILE_ADDR);
    fcnt++;
  }

  for (uint16_t seek = currentindex; seek < currentindex + files; seek++) {
    if (filelist.seek(seek)) {
      rtscheck.RTS_SndData(filelist.shortFilename(), (SDFILE_ADDR + (fcnt * 10)));
      if (filelist.isDir())
        rtscheck.RTS_SndData((unsigned long)0x1A2B, (FilenameNature + (fcnt * 16))); // Change BG of selected line to Light Green
      else
        rtscheck.RTS_SndData((unsigned long)0xFFFF, FilenameNature + fcnt * 16); // white
      rtscheck.RTS_SndData(1, FilenameIcon + ++fcnt);
      fcnt++;
      fseek = seek;
      DEBUG_ECHOLNPAIR("-", seek, " '", filelist.longFilename(), "' '", currentfoldername, "", filelist.shortFilename(), "'\n");
    }
  }
}

void FileNavigator::changeDIR(char *folder) {
  DEBUG_ECHOLNPAIR("currentfolder: ", currentfoldername, "  New: ", folder);
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
  DEBUG_ECHOLNPAIR("depth: ", folderdepth, " currentfoldername: ", currentfoldername);
}

char* FileNavigator::getCurrentFolderName() { return currentfoldername; }
