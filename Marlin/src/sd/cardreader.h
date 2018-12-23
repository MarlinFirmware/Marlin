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
#pragma once

#include "../inc/MarlinConfig.h"

#if ENABLED(SDSUPPORT)

#define SD_RESORT ENABLED(SDCARD_SORT_ALPHA) && ENABLED(SDSORT_DYNAMIC_RAM)

#define MAX_DIR_DEPTH     10       // Maximum folder depth
#define MAXDIRNAMELENGTH   8       // DOS folder name size
#define MAXPATHNAMELENGTH  (1 + (MAXDIRNAMELENGTH + 1) * (MAX_DIR_DEPTH) + 1 + FILENAME_LENGTH) // "/" + N * ("ADIRNAME/") + "filename.ext"

#include "SdFile.h"

enum LsAction : uint8_t { LS_SerialPrint, LS_Count, LS_GetFilename };

typedef struct {
  bool saving:1,
       logging:1,
       sdprinting:1,
       cardOK:1,
       filenameIsDir:1,
       abort_sd_printing:1
       #if ENABLED(FAST_FILE_TRANSFER)
         , binary_mode:1
       #endif
    ;
} card_flags_t;

class CardReader {
public:
  CardReader();

  static void initsd();
  static void write_command(char *buf);

  static void beginautostart();
  static void checkautostart();

  static void openFile(char * const path, const bool read, const bool subcall=false);
  static void openLogFile(char * const path);
  static void removeFile(const char * const name);
  static void closefile(const bool store_location=false);
  static void release();
  static void openAndPrintFile(const char *name);
  static void startFileprint();
  static void stopSDPrint(
    #if SD_RESORT
      const bool re_sort=false
    #endif
  );
  static void getStatus(
    #if NUM_SERIAL > 1
      const int8_t port = -1
    #endif
  );
  static void printingHasFinished();
  static void printFilename(
    #if NUM_SERIAL > 1
      const int8_t port = -1
    #endif
  );

  #if ENABLED(LONG_FILENAME_HOST_SUPPORT)
    static void printLongPath(char *path
      #if NUM_SERIAL > 1
        , const int8_t port = -1
      #endif
    );
  #endif

  static void getfilename(uint16_t nr, const char* const match=NULL);
  static uint16_t getnrfilenames();

  static void getAbsFilename(char *t);

  static void ls(
    #if NUM_SERIAL > 1
      const int8_t port = -1
    #endif
  );
  static void chdir(const char *relpath);
  static int8_t updir();
  static void setroot();

  static const char* diveToFile(SdFile*& curDir, const char * const path, const bool echo);

  static uint16_t get_num_Files();

  #if ENABLED(SDCARD_SORT_ALPHA)
    static void presort();
    static void getfilename_sorted(const uint16_t nr);
    #if ENABLED(SDSORT_GCODE)
      FORCE_INLINE static void setSortOn(bool b) { sort_alpha = b; presort(); }
      FORCE_INLINE static void setSortFolders(int i) { sort_folders = i; presort(); }
      //FORCE_INLINE static void setSortReverse(bool b) { sort_reverse = b; }
    #endif
  #else
    FORCE_INLINE static void getfilename_sorted(const uint16_t nr) { getfilename(nr); }
  #endif

  #if ENABLED(POWER_LOSS_RECOVERY)
    static bool jobRecoverFileExists();
    static void openJobRecoveryFile(const bool read);
    static void removeJobRecoveryFile();
  #endif

  static inline void pauseSDPrint() { flag.sdprinting = false; }
  static inline bool isFileOpen() { return file.isOpen(); }
  static inline bool eof() { return sdpos >= filesize; }
  static inline int16_t get() { sdpos = file.curPosition(); return (int16_t)file.read(); }
  static inline void setIndex(const uint32_t index) { sdpos = index; file.seekSet(index); }
  static inline uint32_t getIndex() { return sdpos; }
  static inline uint8_t percentDone() { return (isFileOpen() && filesize) ? sdpos / ((filesize + 99) / 100) : 0; }
  static inline char* getWorkDirName() { workDir.getFilename(filename); return filename; }
  static inline int16_t read(void* buf, uint16_t nbyte) { return file.isOpen() ? file.read(buf, nbyte) : -1; }
  static inline int16_t write(void* buf, uint16_t nbyte) { return file.isOpen() ? file.write(buf, nbyte) : -1; }

  static Sd2Card& getSd2Card() { return sd2card; }

  #if ENABLED(AUTO_REPORT_SD_STATUS)
    static void auto_report_sd_status(void);
    static inline void set_auto_report_interval(uint8_t v
      #if NUM_SERIAL > 1
        , int8_t port
      #endif
    ) {
      #if NUM_SERIAL > 1
        serialport = port;
      #endif
      NOMORE(v, 60);
      auto_report_sd_interval = v;
      next_sd_report_ms = millis() + 1000UL * v;
    }
  #endif

  static inline char* longest_filename() { return longFilename[0] ? longFilename : filename; }

public:
  static card_flags_t flag;
  static char filename[FILENAME_LENGTH], longFilename[LONG_FILENAME_LENGTH];
  static int8_t autostart_index;

  #if ENABLED(FAST_FILE_TRANSFER)
    #if NUM_SERIAL > 1
      static uint8_t transfer_port;
    #else
      static constexpr uint8_t transfer_port = 0;
    #endif
  #endif

private:
  static SdFile root, workDir, workDirParents[MAX_DIR_DEPTH];
  static uint8_t workDirDepth;

  // Sort files and folders alphabetically.
  #if ENABLED(SDCARD_SORT_ALPHA)
    static uint16_t sort_count;   // Count of sorted items in the current directory
    #if ENABLED(SDSORT_GCODE)
      static bool sort_alpha;     // Flag to enable / disable the feature
      static int sort_folders;    // Folder sorting before/none/after
      //static bool sort_reverse; // Flag to enable / disable reverse sorting
    #endif

    // By default the sort index is static
    #if ENABLED(SDSORT_DYNAMIC_RAM)
      static uint8_t *sort_order;
    #else
      static uint8_t sort_order[SDSORT_LIMIT];
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
          static char **sortshort, **sortnames;
        #else
          static char sortshort[SDSORT_LIMIT][FILENAME_LENGTH];
          static char sortnames[SDSORT_LIMIT][SORTED_LONGNAME_MAXLEN];
        #endif
      #elif DISABLED(SDSORT_USES_STACK)
        static char sortnames[SDSORT_LIMIT][SORTED_LONGNAME_MAXLEN];
      #endif

      // Folder sorting uses an isDir array when caching items.
      #if HAS_FOLDER_SORTING
        #if ENABLED(SDSORT_DYNAMIC_RAM)
          static uint8_t *isDir;
        #elif ENABLED(SDSORT_CACHE_NAMES) || DISABLED(SDSORT_USES_STACK)
          static uint8_t isDir[(SDSORT_LIMIT+7)>>3];
        #endif
      #endif

    #endif // SDSORT_USES_RAM

  #endif // SDCARD_SORT_ALPHA

  static Sd2Card sd2card;
  static SdVolume volume;
  static SdFile file;

  #ifndef SD_PROCEDURE_DEPTH
    #define SD_PROCEDURE_DEPTH 1
  #endif

  static uint8_t file_subcall_ctr;
  static uint32_t filespos[SD_PROCEDURE_DEPTH];
  static char proc_filenames[SD_PROCEDURE_DEPTH][MAXPATHNAMELENGTH];

  static uint32_t filesize, sdpos;

  static LsAction lsAction; //stored for recursion.
  static uint16_t nrFiles; //counter for the files in the current directory and recycled as position counter for getting the nrFiles'th name in the directory.
  static char *diveDirName;
  static void lsDive(const char *prepend, SdFile parent, const char * const match=NULL
    #if NUM_SERIAL > 1
      , const int8_t port = -1
    #endif
  );

  #if ENABLED(SDCARD_SORT_ALPHA)
    static void flush_presort();
  #endif

  #if ENABLED(AUTO_REPORT_SD_STATUS)
    static uint8_t auto_report_sd_interval;
    static millis_t next_sd_report_ms;
    #if NUM_SERIAL > 1
      static int8_t serialport;
    #endif
  #endif
};

#if ENABLED(USB_FLASH_DRIVE_SUPPORT)
  #define IS_SD_INSERTED() Sd2Card::isInserted()
#elif PIN_EXISTS(SD_DETECT)
  #if ENABLED(SD_DETECT_INVERTED)
    #define IS_SD_INSERTED()  READ(SD_DETECT_PIN)
  #else
    #define IS_SD_INSERTED() !READ(SD_DETECT_PIN)
  #endif
#else
  // No card detect line? Assume the card is inserted.
  #define IS_SD_INSERTED() true
#endif

#define IS_SD_PRINTING()  card.flag.sdprinting
#define IS_SD_FILE_OPEN() card.isFileOpen()

extern CardReader card;

#else // !SDSUPPORT

#define IS_SD_PRINTING()  false
#define IS_SD_FILE_OPEN() false

#endif // !SDSUPPORT
