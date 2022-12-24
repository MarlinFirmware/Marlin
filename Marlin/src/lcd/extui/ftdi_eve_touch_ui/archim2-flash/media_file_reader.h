/**********************
 * media_filereader.h *
 **********************/

/****************************************************************************
 *   Written By Mark Pelletier  2017 - Aleph Objects, Inc.                  *
 *   Written By Marcio Teixeira 2018 - Aleph Objects, Inc.                  *
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
 *   location: <https://www.gnu.org/licenses/>.                             *
 ****************************************************************************/

#pragma once

#include "../../../../inc/MarlinConfigPre.h"

#if ENABLED(SDSUPPORT)
  #include "../../../../sd/SdFile.h"
  #include "../../../../sd/cardreader.h"
#endif

class MediaFileReader {
  private:
    #if ENABLED(SDSUPPORT)
      SdFile root, file;
    #endif

  public:
    bool open(const char *filename);
    int16_t read(void *buff, size_t bytes);
    uint32_t size();
    void rewind();
    void close();

    static int16_t read(void *obj, void *buff, size_t bytes);
};
