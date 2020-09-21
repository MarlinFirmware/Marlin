/********************
 * FileNavigator.h  *
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

#pragma once 
#include "../../../../inc/MarlinConfigPre.h"
#include "../../../../MarlinCore.h"
#include "../../ui_api.h"
#include "chiron_tft_defs.h"
using namespace ExtUI;
namespace Anycubic {
  class FileNavigator {
    public:
      FileNavigator();
      void   reset();
      void   getFiles(uint16_t);  
      void   upDIR();
      void   changeDIR(char *);
      void   sendFile();
      void   refresh();
      char * getCurrentFolderName();   
    private:
      static FileList  filelist;
      static char      currentfoldername[MAX_PATH_LEN];
      static uint16_t  lastindex;
      static uint8_t   folderdepth;
      static uint16_t  currentindex;
  };
  extern FileNavigator filenavigator;
}