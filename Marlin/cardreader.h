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

#ifndef _CARDREADER_H_
#define _CARDREADER_H_

#include "MarlinConfig.h"

#if ENABLED(SDSUPPORT)

#define SD_RESORT ENABLED(SDCARD_SORT_ALPHA) && ENABLED(SDSORT_DYNAMIC_RAM)

#define MAX_DIR_DEPTH 10          // Maximum folder depth

#include "SdFile.h"

class CardReader {
public:
  CardReader();

  void initsd();
  void write_command(char *buf);

  void beginautostart();
  void checkautostart();

  void openFile(char * const path, const bool read, const bool subcall=false);
  void openLogFile(char * const path);
  void removeFile(const char * const name);
  void closefile(const bool store_location=false);
  void release();
  void openAndPrintFile(const char *name);
  void startFileprint();
  void stopSDPrint(
    #if SD_RESORT
      const bool re_sort=false
    #endif
  );
  void getStatus();
  void printingHasFinished();
  void printFilename();

  #if ENABLED(LONG_FILENAME_HOST_SUPPORT)
    void printLongPath(char *path);
  #endif

  void getfilename(uint16_t nr, const char* const match=NULL);
  uint16_t getnrfilenames();

  void getAbsFilename(char *t);

  void ls();
  void chdir(const char *relpath);
  int8_t updir();
  void setroot();

  const char* diveToFile(SdFile*& curDir, const char * const path, const bool echo);

  uint16_t get_num_Files();

  #if ENABLED(SDCARD_SORT_ALPHA)
    void presort();
    void getfilename_sorted(const uint16_t nr);
    #if ENABLED(SDSORT_GCODE)
      FORCE_INLINE void setSortOn(bool b) { sort_alpha = b; presort(); }
      FORCE_INLINE void setSortFolders(int i) { sort_folders = i; presort(); }
      //FORCE_INLINE void setSortReverse(bool b) { sort_reverse = b; }
    #endif
  #endif

  #if ENABLED(POWER_LOSS_RECOVERY)
    void openJobRecoveryFile(const bool read);
    void closeJobRecoveryFile();
    bool jobRecoverFileExists();
    int16_t saveJobRecoveryInfo();
    int16_t loadJobRecoveryInfo();
    void removeJobRecoveryFile();
  #endif

  FORCE_INLINE void pauseSDPrint() { sdprinting = false; }
  FORCE_INLINE bool isFileOpen() { return file.isOpen(); }
  FORCE_INLINE bool eof() { return sdpos >= filesize; }
  FORCE_INLINE int16_t get() { sdpos = file.curPosition(); return (int16_t)file.read(); }
  FORCE_INLINE void setIndex(const uint32_t index) { sdpos = index; file.seekSet(index); }
  FORCE_INLINE uint32_t getIndex() { return sdpos; }
  FORCE_INLINE uint8_t percentDone() { return (isFileOpen() && filesize) ? sdpos / ((filesize + 99) / 100) : 0; }
  FORCE_INLINE char* getWorkDirName() { workDir.getFilename(filename); return filename; }

  #if ENABLED(AUTO_REPORT_SD_STATUS)
    void auto_report_sd_status(void);
    FORCE_INLINE void set_auto_report_interval(uint8_t v) {
      NOMORE(v, 60);
      auto_report_sd_interval = v;
      next_sd_report_ms = millis() + 1000UL * v;
    }
  #endif

  FORCE_INLINE char* longest_filename() { return longFilename[0] ? longFilename : filename; }

public:
  bool saving, logging, sdprinting, cardOK, filenameIsDir;
  char filename[FILENAME_LENGTH], longFilename[LONG_FILENAME_LENGTH];
  int8_t autostart_index;
private:
  SdFile root, workDir, workDirParents[MAX_DIR_DEPTH];
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

    #if ENABLED(SDSORT_USES_RAM) && ENABLED(SDSORT_CACHE_NAMES) && DISABLED(SDSORT_DYNAMIC_RAM)
      #define SORTED_LONGNAME_MAXLEN ((SDSORT_CACHE_VFATS) * (FILENAME_LENGTH) + 1)
    #else
      #define SORTED_LONGNAME_MAXLEN LONG_FILENAME_LENGTH
    #endif

    // Cache filenames to speed up SD menus.
    #if ENABLED(SDSORT_USES_RAM)

      // If using dynamic ram for names, allocate on the heap.
      #if ENABLED(SDSORT_CACHE_NAMES)
        #if ENABLED(SDSORT_DYNAMIC_RAM)
          char **sortshort, **sortnames;
        #else
          char sortshort[SDSORT_LIMIT][FILENAME_LENGTH];
          char sortnames[SDSORT_LIMIT][SORTED_LONGNAME_MAXLEN];
        #endif
      #elif DISABLED(SDSORT_USES_STACK)
        char sortnames[SDSORT_LIMIT][SORTED_LONGNAME_MAXLEN];
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

  Sd2Card sd2card;
  SdVolume volume;
  SdFile file;

  #if ENABLED(POWER_LOSS_RECOVERY)
    SdFile jobRecoveryFile;
  #endif

  #define SD_PROCEDURE_DEPTH 1
  #define MAXPATHNAMELENGTH (FILENAME_LENGTH*MAX_DIR_DEPTH + MAX_DIR_DEPTH + 1)
  uint8_t file_subcall_ctr;
  uint32_t filespos[SD_PROCEDURE_DEPTH];
  char proc_filenames[SD_PROCEDURE_DEPTH][MAXPATHNAMELENGTH];
  uint32_t filesize, sdpos;

  LsAction lsAction; //stored for recursion.
  uint16_t nrFiles; //counter for the files in the current directory and recycled as position counter for getting the nrFiles'th name in the directory.
  char* diveDirName;
  void lsDive(const char *prepend, SdFile parent, const char * const match=NULL);

  #if ENABLED(SDCARD_SORT_ALPHA)
    void flush_presort();
  #endif

  #if ENABLED(AUTO_REPORT_SD_STATUS)
    static uint8_t auto_report_sd_interval;
    static millis_t next_sd_report_ms;
  #endif
};

#if PIN_EXISTS(SD_DETECT)
  #if ENABLED(SD_DETECT_INVERTED)
    #define IS_SD_INSERTED (READ(SD_DETECT_PIN) == HIGH)
  #else
    #define IS_SD_INSERTED (READ(SD_DETECT_PIN) == LOW)
  #endif
#else
  // No card detect line? Assume the card is inserted.
  #define IS_SD_INSERTED true
#endif

extern CardReader card;

#endif // SDSUPPORT

#if ENABLED(SDSUPPORT)
  #define IS_SD_PRINTING (card.sdprinting)
  #define IS_SD_FILE_OPEN (card.isFileOpen())
#else
  #define IS_SD_PRINTING (false)
  #define IS_SD_FILE_OPEN (false)
#endif

#endif // _CARDREADER_H_
