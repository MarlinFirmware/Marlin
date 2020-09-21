/********************
 * FileNavigator.cpp *
 *******************/

/****************************************************************************
 *   Written By Nick Wells 2020 [https://github.com/SwiftNick]              * 
 *   I am not affiliated with Anycubic Ltd.                                 * 
 *                                                                          *
 *   This is an open source interface for the factory TFT panel on the      *
 *   Anycubic Chiron using the Extensible_UI API                            *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   To view a copy of the GNU General Public License, go to the following  *
 *   location: <http://www.gnu.org/licenses/>.                              *
 ***************************************************************************/
/*                                                                          *
 *   The AC panel want files in block of 4 and can only display a flat list *
 *   This library allows full folder traversal.                             *
 *                                                                          *
 ***************************************************************************/
#include "FileNavigator.h"
#include "chiron_tft.h"
#if BOTH(ANYCUBIC_LCD_CHIRON, EXTENSIBLE_UI)

using namespace ExtUI;
namespace Anycubic {
  FileList  FileNavigator::filelist;                          // Instance of the Marlin file API
  char      FileNavigator::currentfoldername[MAX_PATH_LEN];   // Current folder path
  uint16_t  FileNavigator::lastindex;
  uint8_t   FileNavigator::folderdepth;
  uint16_t  FileNavigator::currentindex;                      // override the panel request
  FileNavigator::FileNavigator() {
    reset();
  }
  void FileNavigator::reset() {
    currentfoldername[0] = '\0';
    folderdepth  = 0;
    currentindex = 0;
    lastindex    = 0;
    // Start at root folder
    while(!filelist.isAtRootDir()) {
      filelist.upDir();
    }
    refresh();
  }
  void FileNavigator::refresh() {
    filelist.refresh();
  }
  void FileNavigator::getFiles(uint16_t index, bool command_menu) {
    uint8_t files = 4;
    if(index == 0) {
      currentindex = 0; 
    }

    // Each time we change folder we reset the file index to 0 and keep track of the current position
    // as the TFT panel isnt aware of folders trees.
    if(index > 0){
      currentindex -=1; // go back a file to take account off the .. we added to the root.
      if(index > lastindex) currentindex += files;
      else {
        if(currentindex < 4) {
          currentindex = 0;
        }
        else {
          currentindex -= files;
        }
      } 
    }
    lastindex=index;
    #if ACDEBUG(4) 
      _SELP_4_P(PSTR("index="), index, PSTR(" currentindex="),currentindex);
    #endif
      if( (currentindex == 0) && ( folderdepth > 0) ) { // Add a link to go up a folder
      TFTSer.println("<<");
      TFTSer.println("..");
      files--;
    }
    for(uint16_t seek = currentindex; seek < currentindex+files; seek ++) {
      if(filelist.seek(seek)) {
        sendFile();
        #if ACDEBUG(4) 
          _SELP_9_P(PSTR("-"),seek,PSTR(" '"),filelist.longFilename(),PSTR("' '"),currentfoldername,PSTR(""),filelist.shortFilename(),PSTR("'\n"));
        #endif
      }
    }
  }
  void FileNavigator::sendFile() {
    // send the file and folder info to the panel
    // this info will be returned when the file is selected
    // Permitted special characters in file name -_*#~
    // Panel can display 22 characters per line
    if(filelist.isDir()) {
      //TFTSer.print(currentfoldername);
      TFTSer.println(filelist.shortFilename());
      TFTSer.print(filelist.shortFilename());
      TFTSer.println("/");
    }
    else {
      // Logical Name
      TFTSer.print("/");
      if(folderdepth > 0) {
        TFTSer.print(currentfoldername);
      }
      
      TFTSer.println(filelist.shortFilename());

      // Display Name
      TFTSer.println(filelist.longFilename());
    }
  }
  void FileNavigator::changeDIR(char * folder) {
    #if ACDEBUG(4) 
      _SELP_4_P(PSTR("currentfolder: "),currentfoldername,PSTR("  New: "),folder);
    #endif
    if(folderdepth == MAX_FOLDER_DEPTH) { //limit the folder depth
      return;
    }
    strcat(currentfoldername,folder);
    strcat(currentfoldername,"/");

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
    // remove the last child folder from the stored path
    if(folderdepth == 0) {
      currentfoldername[0] = '\0';
      reset();
    }
    else {
      char * pos = NULL;
      for(uint8_t f = 0; f < folderdepth; f++) {
        pos = strchr(currentfoldername, '/');
      }  
      *(pos+1) = '\0';
    }
    #if ACDEBUG(4) 
      _SELP_4_P(PSTR("depth: "),folderdepth,PSTR(" currentfoldername: "),currentfoldername);
    #endif
  }
  char * FileNavigator::getCurrentFolderName() {
    return currentfoldername;
  }
}
#endif