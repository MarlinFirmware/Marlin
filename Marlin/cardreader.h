/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef CARDREADER_H
#define CARDREADER_H

#include "MarlinConfig.h"

#if ENABLED(SDSUPPORT)

#define MAX_DIR_DEPTH 10          // Maximum folder depth

#include "SdFile.h"

#include "types.h"
#include "enum.h"

class CardReader {
public:
  CardReader();

  void initsd();
  void write_command(char *buf);
  //files auto[0-9].g on the sd card are performed in a row
  //this is to delay autostart and hence the initialisaiton of the sd card to some seconds after the normal init, so the device is available quick after a reset

  void checkautostart(bool x);
  void openFile(char* name, bool read, bool push_current=false);
  void openLogFile(char* name);
  void removeFile(char* name);
  void closefile(bool store_location=false);
  void release();
  void openAndPrintFile(const char *name);
  void startFileprint();
  void stopSDPrint();
  void getStatus();
  void printingHasFinished();

  #if ENABLED(LONG_FILENAME_HOST_SUPPORT)
    void printLongPath(char *path);
  #endif

  void getfilename(uint16_t nr, const char* const match=NULL);
  uint16_t getnrfilenames();

  void getAbsFilename(char *t);

  void ls();
  void chdir(const char *relpath);
  void updir();
  void setroot();

  #if ENABLED(SDCARD_SORT_ALPHA)
    void presort();
    void getfilename_sorted(const uint16_t nr);
    #if ENABLED(SDSORT_GCODE)
      FORCE_INLINE void setSortOn(bool b) { sort_alpha = b; presort(); }
      FORCE_INLINE void setSortFolders(int i) { sort_folders = i; presort(); }
      //FORCE_INLINE void setSortReverse(bool b) { sort_reverse = b; }
    #endif
  #endif

  FORCE_INLINE void pauseSDPrint() { sdprinting = false; }
  FORCE_INLINE bool isFileOpen() { return file.isOpen(); }
  FORCE_INLINE bool eof() { return sdpos >= filesize; }
  FORCE_INLINE int16_t get() { sdpos = file.curPosition(); return (int16_t)file.read(); }
  FORCE_INLINE void setIndex(long index) { sdpos = index; file.seekSet(index); }
  FORCE_INLINE uint8_t percentDone() { return (isFileOpen() && filesize) ? sdpos / ((filesize + 99) / 100) : 0; }
  FORCE_INLINE char* getWorkDirName() { workDir.getFilename(filename); return filename; }

public:
  bool saving, logging, sdprinting, cardOK, filenameIsDir;
  char filename[FILENAME_LENGTH], longFilename[LONG_FILENAME_LENGTH];
  int autostart_index;
private:
  SdFile root, *curDir, workDir, workDirParents[MAX_DIR_DEPTH];
  uint8_t workDirDepth;

  // Sort files and folders alphabetically.
  #if ENABLED(SDCARD_SORT_ALPHA)
    uint16_t sort_count;        // Count of sorted items in the current directory
    #if ENABLED(SDSORT_GCODE)
      bool sort_alpha;          // Flag to enable / disable the feature
      int sort_folders;         // Flag to enable / disable folder sorting
      //bool sort_reverse;      // Flag to enable / disable reverse sorting
    #endif

    // By default the sort index is static
    #if ENABLED(SDSORT_DYNAMIC_RAM)
      uint8_t *sort_order;
    #else
      uint8_t sort_order[SDSORT_LIMIT];
    #endif

    // Cache filenames to speed up SD menus.
    #if ENABLED(SDSORT_USES_RAM)

      // If using dynamic ram for names, allocate on the heap.
      #if ENABLED(SDSORT_CACHE_NAMES)
        #if ENABLED(SDSORT_DYNAMIC_RAM)
          char **sortshort, **sortnames;
        #else
          char sortshort[SDSORT_LIMIT][FILENAME_LENGTH];
          char sortnames[SDSORT_LIMIT][FILENAME_LENGTH];
        #endif
      #elif DISABLED(SDSORT_USES_STACK)
        char sortnames[SDSORT_LIMIT][FILENAME_LENGTH];
      #endif

      // Folder sorting uses an isDir array when caching items.
      #if HAS_FOLDER_SORTING
        #if ENABLED(SDSORT_DYNAMIC_RAM)
          uint8_t *isDir;
        #elif ENABLED(SDSORT_CACHE_NAMES) || DISABLED(SDSORT_USES_STACK)
          uint8_t isDir[(SDSORT_LIMIT+7)>>3];
        #endif
      #endif

    #endif // SDSORT_USES_RAM

  #endif // SDCARD_SORT_ALPHA

  Sd2Card card;
  SdVolume volume;
  SdFile file;

  #define SD_PROCEDURE_DEPTH 1
  #define MAXPATHNAMELENGTH (FILENAME_LENGTH*MAX_DIR_DEPTH + MAX_DIR_DEPTH + 1)
  uint8_t file_subcall_ctr;
  uint32_t filespos[SD_PROCEDURE_DEPTH];
  char proc_filenames[SD_PROCEDURE_DEPTH][MAXPATHNAMELENGTH];
  uint32_t filesize;
  uint32_t sdpos;

  millis_t next_autostart_ms;
  bool autostart_stilltocheck; //the sd start is delayed, because otherwise the serial cannot answer fast enought to make contact with the hostsoftware.

  LsAction lsAction; //stored for recursion.
  uint16_t nrFiles; //counter for the files in the current directory and recycled as position counter for getting the nrFiles'th name in the directory.
  char* diveDirName;
  void lsDive(const char *prepend, SdFile parent, const char * const match=NULL);

  #if ENABLED(SDCARD_SORT_ALPHA)
    void flush_presort();
  #endif
};

extern CardReader card;

#define IS_SD_PRINTING (card.sdprinting)

#if PIN_EXISTS(SD_DETECT)
  #if ENABLED(SD_DETECT_INVERTED)
    #define IS_SD_INSERTED (READ(SD_DETECT_PIN) != 0)
  #else
    #define IS_SD_INSERTED (READ(SD_DETECT_PIN) == 0)
  #endif
#else
  //No card detect line? Assume the card is inserted.
  #define IS_SD_INSERTED true
#endif

#else

#define IS_SD_PRINTING (false)

#endif // SDSUPPORT

#endif // __CARDREADER_H
