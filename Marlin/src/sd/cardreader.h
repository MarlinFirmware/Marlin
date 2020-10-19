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
#pragma once

#include "../inc/MarlinConfig.h"

#define IFSD(A,B) TERN(SDSUPPORT,A,B)

#if ENABLED(SDSUPPORT)

#if BOTH(SDCARD_SORT_ALPHA, SDSORT_DYNAMIC_RAM)
  #define SD_RESORT 1
#endif

#if ENABLED(SDCARD_RATHERRECENTFIRST) && DISABLED(SDCARD_SORT_ALPHA)
  #define SD_ORDER(N,C) ((C) - 1 - (N))
#else
  #define SD_ORDER(N,C) N
#endif

#define MAX_DIR_DEPTH     10       // Maximum folder depth
#define MAXDIRNAMELENGTH   8       // DOS folder name size
#define MAXPATHNAMELENGTH  (1 + (MAXDIRNAMELENGTH + 1) * (MAX_DIR_DEPTH) + 1 + FILENAME_LENGTH) // "/" + N * ("ADIRNAME/") + "filename.ext"

#include "SdFile.h"

typedef struct {
  bool saving:1,
       logging:1,
       sdprinting:1,
       mounted:1,
       filenameIsDir:1,
       workDirIsRoot:1,
       abort_sd_printing:1
       #if ENABLED(BINARY_FILE_TRANSFER)
         , binary_mode:1
       #endif
    ;
} card_flags_t;

class CardReader {
public:
  static card_flags_t flag;                         // Flags (above)
  static char filename[FILENAME_LENGTH],            // DOS 8.3 filename of the selected item
              longFilename[LONG_FILENAME_LENGTH];   // Long name of the selected item

  // Fast! binary file transfer
  #if ENABLED(BINARY_FILE_TRANSFER)
    #if HAS_MULTI_SERIAL
      static int8_t transfer_port_index;
    #else
      static constexpr int8_t transfer_port_index = 0;
    #endif
  #endif

  // // // Methods // // //

  CardReader();

  static SdFile getroot() { return root; }

  static void mount();
  static void release();
  static inline bool isMounted() { return flag.mounted; }
  static void ls();

  // Handle media insert/remove
  static void manage_media();

  // SD Card Logging
  static void openLogFile(char * const path);
  static void write_command(char * const buf);

  // Auto-Start files
  static int8_t autostart_index;                    // Index of autoX.g files
  static void beginautostart();
  static void checkautostart();

  // Basic file ops
  static void openFileRead(char * const path, const uint8_t subcall=0);
  static void openFileWrite(char * const path);
  static void closefile(const bool store_location=false);
  static bool fileExists(const char * const name);
  static void removeFile(const char * const name);

  static inline char* longest_filename() { return longFilename[0] ? longFilename : filename; }
  #if ENABLED(LONG_FILENAME_HOST_SUPPORT)
    static void printLongPath(char * const path);   // Used by M33
  #endif

  // Working Directory for SD card menu
  static void cdroot();
  static void cd(const char *relpath);
  static int8_t cdup();
  static uint16_t countFilesInWorkDir();
  static uint16_t get_num_Files();

  // Select a file
  static void selectFileByIndex(const uint16_t nr);
  static void selectFileByName(const char* const match);

  // Print job
  static void openAndPrintFile(const char *name);   // (working directory)
  static void fileHasFinished();
  static void getAbsFilename(char *dst);
  static void printFilename();
  static void startFileprint();
  static void endFilePrint(TERN_(SD_RESORT, const bool re_sort=false));
  static void report_status();
  static inline void pauseSDPrint() { flag.sdprinting = false; }
  static inline bool isPaused() { return isFileOpen() && !flag.sdprinting; }
  static inline bool isPrinting() { return flag.sdprinting; }
  #if HAS_PRINT_PROGRESS_PERMYRIAD
    static inline uint16_t permyriadDone() { return (isFileOpen() && filesize) ? sdpos / ((filesize + 9999) / 10000) : 0; }
  #endif
  static inline uint8_t percentDone() { return (isFileOpen() && filesize) ? sdpos / ((filesize + 99) / 100) : 0; }

  // Helper for open and remove
  static const char* diveToFile(const bool update_cwd, SdFile*& curDir, const char * const path, const bool echo=false);

  #if ENABLED(SDCARD_SORT_ALPHA)
    static void presort();
    static void getfilename_sorted(const uint16_t nr);
    #if ENABLED(SDSORT_GCODE)
      FORCE_INLINE static void setSortOn(bool b) { sort_alpha = b; presort(); }
      FORCE_INLINE static void setSortFolders(int i) { sort_folders = i; presort(); }
      //FORCE_INLINE static void setSortReverse(bool b) { sort_reverse = b; }
    #endif
  #else
    FORCE_INLINE static void getfilename_sorted(const uint16_t nr) { selectFileByIndex(nr); }
  #endif

  #if ENABLED(POWER_LOSS_RECOVERY)
    static bool jobRecoverFileExists();
    static void openJobRecoveryFile(const bool read);
    static void removeJobRecoveryFile();
  #endif

  static inline bool isFileOpen() { return isMounted() && file.isOpen(); }
  static inline uint32_t getIndex() { return sdpos; }
  static inline uint32_t getFileSize() { return filesize; }
  static inline bool eof() { return sdpos >= filesize; }
  static inline void setIndex(const uint32_t index) { sdpos = index; file.seekSet(index); }
  static inline char* getWorkDirName() { workDir.getDosName(filename); return filename; }
  static inline int16_t get() { sdpos = file.curPosition(); return (int16_t)file.read(); }
  static inline int16_t read(void* buf, uint16_t nbyte) { return file.isOpen() ? file.read(buf, nbyte) : -1; }
  static inline int16_t write(void* buf, uint16_t nbyte) { return file.isOpen() ? file.write(buf, nbyte) : -1; }

  static Sd2Card& getSd2Card() { return sd2card; }

  #if ENABLED(AUTO_REPORT_SD_STATUS)
    static void auto_report_sd_status();
    static inline void set_auto_report_interval(uint8_t v) {
      TERN_(HAS_MULTI_SERIAL, auto_report_port = serial_port_index);
      NOMORE(v, 60);
      auto_report_sd_interval = v;
      next_sd_report_ms = millis() + 1000UL * v;
    }
  #endif

private:
  //
  // Working directory and parents
  //
  static SdFile root, workDir, workDirParents[MAX_DIR_DEPTH];
  static uint8_t workDirDepth;

  //
  // Alphabetical file and folder sorting
  //
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

    #if BOTH(SDSORT_USES_RAM, SDSORT_CACHE_NAMES) && DISABLED(SDSORT_DYNAMIC_RAM)
      #define SORTED_LONGNAME_MAXLEN (SDSORT_CACHE_VFATS) * (FILENAME_LENGTH)
      #define SORTED_LONGNAME_STORAGE (SORTED_LONGNAME_MAXLEN + 1)
    #else
      #define SORTED_LONGNAME_MAXLEN LONG_FILENAME_LENGTH
      #define SORTED_LONGNAME_STORAGE SORTED_LONGNAME_MAXLEN
    #endif

    // Cache filenames to speed up SD menus.
    #if ENABLED(SDSORT_USES_RAM)

      // If using dynamic ram for names, allocate on the heap.
      #if ENABLED(SDSORT_CACHE_NAMES)
        static uint16_t nrFiles; // Cache the total count
        #if ENABLED(SDSORT_DYNAMIC_RAM)
          static char **sortshort, **sortnames;
        #else
          static char sortshort[SDSORT_LIMIT][FILENAME_LENGTH];
        #endif
      #endif

      #if (ENABLED(SDSORT_CACHE_NAMES) && DISABLED(SDSORT_DYNAMIC_RAM)) || NONE(SDSORT_CACHE_NAMES, SDSORT_USES_STACK)
        static char sortnames[SDSORT_LIMIT][SORTED_LONGNAME_STORAGE];
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

  static uint32_t filesize, sdpos;

  //
  // Procedure calls to other files
  //
  #ifndef SD_PROCEDURE_DEPTH
    #define SD_PROCEDURE_DEPTH 1
  #endif
  static uint8_t file_subcall_ctr;
  static uint32_t filespos[SD_PROCEDURE_DEPTH];
  static char proc_filenames[SD_PROCEDURE_DEPTH][MAXPATHNAMELENGTH];

  //
  // SD Auto Reporting
  //
  #if ENABLED(AUTO_REPORT_SD_STATUS)
    static uint8_t auto_report_sd_interval;
    static millis_t next_sd_report_ms;
    #if HAS_MULTI_SERIAL
      static int8_t auto_report_port;
    #endif
  #endif

  //
  // Directory items
  //
  static bool is_dir_or_gcode(const dir_t &p);
  static int countItems(SdFile dir);
  static void selectByIndex(SdFile dir, const uint8_t index);
  static void selectByName(SdFile dir, const char * const match);
  static void printListing(SdFile parent, const char * const prepend=nullptr);

  #if ENABLED(SDCARD_SORT_ALPHA)
    static void flush_presort();
  #endif
};

#if ENABLED(USB_FLASH_DRIVE_SUPPORT)
  #define IS_SD_INSERTED() Sd2Card::isInserted()
#elif PIN_EXISTS(SD_DETECT)
  #define IS_SD_INSERTED() (READ(SD_DETECT_PIN) == SD_DETECT_STATE)
#else
  // No card detect line? Assume the card is inserted.
  #define IS_SD_INSERTED() true
#endif

#define IS_SD_PRINTING()  card.flag.sdprinting
#define IS_SD_PAUSED()    card.isPaused()
#define IS_SD_FILE_OPEN() card.isFileOpen()

extern CardReader card;

#else // !SDSUPPORT

#define IS_SD_PRINTING()  false
#define IS_SD_PAUSED()    false
#define IS_SD_FILE_OPEN() false

#define LONG_FILENAME_LENGTH 0

#endif // !SDSUPPORT
