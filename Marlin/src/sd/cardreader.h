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

#if ENABLED(SDSUPPORT)

extern const char M23_STR[], M24_STR[];

#if ENABLED(SDCARD_SORT_ALPHA)
  #if ENABLED(SDSORT_DYNAMIC_RAM)
    #define SD_RESORT 1
  #endif
  #if FOLDER_SORTING || ENABLED(SDSORT_GCODE)
    #define HAS_FOLDER_SORTING 1
  #endif
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
#include "disk_io_driver.h"

#if ENABLED(USB_FLASH_DRIVE_SUPPORT)
  #include "usb_flashdrive/Sd2Card_FlashDrive.h"
#endif

#if NEED_SD2CARD_SDIO
  #include "Sd2Card_sdio.h"
#elif NEED_SD2CARD_SPI
  #include "Sd2Card.h"
#endif

#if ENABLED(MULTI_VOLUME)
  #define SV_SD_ONBOARD      1
  #define SV_USB_FLASH_DRIVE 2
  #define _VOLUME_ID(N) _CAT(SV_, N)
  #define SHARED_VOLUME_IS(N) (DEFAULT_SHARED_VOLUME == _VOLUME_ID(N))
  #if !SHARED_VOLUME_IS(SD_ONBOARD) && !SHARED_VOLUME_IS(USB_FLASH_DRIVE)
    #error "DEFAULT_SHARED_VOLUME must be either SD_ONBOARD or USB_FLASH_DRIVE."
  #endif
#else
  #define SHARED_VOLUME_IS(...) 0
#endif

typedef struct {
  bool saving:1,
       logging:1,
       sdprinting:1,
       sdprintdone:1,
       mounted:1,
       filenameIsDir:1,
       workDirIsRoot:1,
       abort_sd_printing:1
       #if DO_LIST_BIN_FILES
         , filenameIsBin:1
       #endif
       #if ENABLED(BINARY_FILE_TRANSFER)
         , binary_mode:1
       #endif
    ;
} card_flags_t;

#if ENABLED(AUTO_REPORT_SD_STATUS)
  #include "../libs/autoreport.h"
#endif

class CardReader {
public:
  static card_flags_t flag;                         // Flags (above)
  static char filename[FILENAME_LENGTH],            // DOS 8.3 filename of the selected item
              longFilename[LONG_FILENAME_LENGTH];   // Long name of the selected item

  // Fast! binary file transfer
  #if ENABLED(BINARY_FILE_TRANSFER)
    #if HAS_MULTI_SERIAL
      static serial_index_t transfer_port_index;
    #else
      static constexpr serial_index_t transfer_port_index = 0;
    #endif
  #endif

  // // // Methods // // //

  CardReader();

  static void changeMedia(DiskIODriver *_driver) { driver = _driver; }

  static SdFile getroot() { return root; }

  static void mount();
  static void release();
  static bool isMounted() { return flag.mounted; }

  // Handle media insert/remove
  static void manage_media();

  // SD Card Logging
  static void openLogFile(const char * const path);
  static void write_command(char * const buf);

  #if DISABLED(NO_SD_AUTOSTART)     // Auto-Start auto#.g file handling
    static uint8_t autofile_index;  // Next auto#.g index to run, plus one. Ignored by autofile_check when zero.
    static void autofile_begin();   // Begin check. Called automatically after boot-up.
    static bool autofile_check();   // Check for the next auto-start file and run it.
    static void autofile_cancel() { autofile_index = 0; }
  #endif

  // Basic file ops
  static void openFileRead(const char * const path, const uint8_t subcall=0);
  static void openFileWrite(const char * const path);
  static void closefile(const bool store_location=false);
  static bool fileExists(const char * const name);
  static void removeFile(const char * const name);

  static char* longest_filename() { return longFilename[0] ? longFilename : filename; }
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
  static void selectFileByName(const char * const match);  // (working directory only)

  // Print job
  static void report_status();
  static void getAbsFilenameInCWD(char *dst);
  static void printSelectedFilename();
  static void openAndPrintFile(const char *name);   // (working directory or full path)
  static void startOrResumeFilePrinting();
  static void endFilePrintNow(TERN_(SD_RESORT, const bool re_sort=false));
  static void abortFilePrintNow(TERN_(SD_RESORT, const bool re_sort=false));
  static void fileHasFinished();
  static void abortFilePrintSoon() { flag.abort_sd_printing = isFileOpen(); }
  static void pauseSDPrint()       { flag.sdprinting = false; }
  static bool isPrinting()         { return flag.sdprinting; }
  static bool isPaused()           { return isFileOpen() && !isPrinting(); }
  #if HAS_PRINT_PROGRESS_PERMYRIAD
    static uint16_t permyriadDone() {
      if (flag.sdprintdone) return 10000;
      if (isFileOpen() && filesize) return sdpos / ((filesize + 9999) / 10000);
      return 0;
    }
  #endif
  static uint8_t percentDone() {
    if (flag.sdprintdone) return 100;
    if (isFileOpen() && filesize) return sdpos / ((filesize + 99) / 100);
    return 0;
  }

  /**
   * Dive down to a relative or absolute path.
   * Relative paths apply to the workDir.
   *
   * update_cwd: Pass 'true' to update the workDir on success.
   *   inDirPtr: On exit your pointer points to the target SdFile.
   *             A nullptr indicates failure.
   *       path: Start with '/' for abs path. End with '/' to get a folder ref.
   *       echo: Set 'true' to print the path throughout the loop.
   */
  static const char* diveToFile(const bool update_cwd, SdFile* &inDirPtr, const char * const path, const bool echo=false);

  #if ENABLED(SDCARD_SORT_ALPHA)
    static void presort();
    static void getfilename_sorted(const uint16_t nr);
    #if ENABLED(SDSORT_GCODE)
      FORCE_INLINE static void setSortOn(bool b)        { sort_alpha   = b; presort(); }
      FORCE_INLINE static void setSortFolders(int i)    { sort_folders = i; presort(); }
      //FORCE_INLINE static void setSortReverse(bool b) { sort_reverse = b; }
    #endif
  #else
    FORCE_INLINE static void getfilename_sorted(const uint16_t nr) { selectFileByIndex(nr); }
  #endif

  static void ls(
    TERN_(CUSTOM_FIRMWARE_UPLOAD, const bool onlyBin=false)
    #if BOTH(CUSTOM_FIRMWARE_UPLOAD, LONG_FILENAME_HOST_SUPPORT)
      ,
    #endif
    TERN_(LONG_FILENAME_HOST_SUPPORT, const bool includeLongNames=false)
  );

  #if ENABLED(POWER_LOSS_RECOVERY)
    static bool jobRecoverFileExists();
    static void openJobRecoveryFile(const bool read);
    static void removeJobRecoveryFile();
  #endif

  // Binary flag for the current file
  static bool fileIsBinary() { return TERN0(DO_LIST_BIN_FILES, flag.filenameIsBin); }
  static void setBinFlag(const bool bin) { TERN(DO_LIST_BIN_FILES, flag.filenameIsBin = bin, UNUSED(bin)); }

  // Current Working Dir - Set by cd, cdup, cdroot, and diveToFile(true, ...)
  static char* getWorkDirName()  { workDir.getDosName(filename); return filename; }
  static SdFile& getWorkDir()    { return workDir.isOpen() ? workDir : root; }

  // Print File stats
  static uint32_t getFileSize()  { return filesize; }
  static uint32_t getIndex()     { return sdpos; }
  static bool isFileOpen()       { return isMounted() && file.isOpen(); }
  static bool eof()              { return getIndex() >= getFileSize(); }

  // File data operations
  static int16_t get()                            { int16_t out = (int16_t)file.read(); sdpos = file.curPosition(); return out; }
  static int16_t read(void *buf, uint16_t nbyte)  { return file.isOpen() ? file.read(buf, nbyte) : -1; }
  static int16_t write(void *buf, uint16_t nbyte) { return file.isOpen() ? file.write(buf, nbyte) : -1; }
  static void setIndex(const uint32_t index)      { file.seekSet((sdpos = index)); }

  // TODO: rename to diskIODriver()
  static DiskIODriver* diskIODriver() { return driver; }

  #if ENABLED(AUTO_REPORT_SD_STATUS)
    //
    // SD Auto Reporting
    //
    struct AutoReportSD { static void report() { report_status(); } };
    static AutoReporter<AutoReportSD> auto_reporter;
  #endif

  #if SHARED_VOLUME_IS(USB_FLASH_DRIVE) || ENABLED(USB_FLASH_DRIVE_SUPPORT)
    #define HAS_USB_FLASH_DRIVE 1
    static DiskIODriver_USBFlash media_driver_usbFlash;
  #endif

  #if NEED_SD2CARD_SDIO || NEED_SD2CARD_SPI
    typedef TERN(NEED_SD2CARD_SDIO, DiskIODriver_SDIO, DiskIODriver_SPI_SD) sdcard_driver_t;
    static sdcard_driver_t media_driver_sdcard;
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
          static uint8_t isDir[(SDSORT_LIMIT + 7) >> 3];
        #endif
      #endif

    #endif // SDSORT_USES_RAM

  #endif // SDCARD_SORT_ALPHA

  static DiskIODriver *driver;
  static SdVolume volume;
  static SdFile file;

  static uint32_t filesize, // Total size of the current file, in bytes
                  sdpos;    // Index most recently read (one behind file.getPos)

  //
  // Procedure calls to other files
  //
  #if HAS_MEDIA_SUBCALLS
    static uint8_t file_subcall_ctr;
    static uint32_t filespos[SD_PROCEDURE_DEPTH];
    static char proc_filenames[SD_PROCEDURE_DEPTH][MAXPATHNAMELENGTH];
  #endif

  //
  // Directory items
  //
  static bool is_visible_entity(const dir_t &p OPTARG(CUSTOM_FIRMWARE_UPLOAD, const bool onlyBin=false));
  static int countItems(SdFile dir);
  static void selectByIndex(SdFile dir, const uint8_t index);
  static void selectByName(SdFile dir, const char * const match);
  static void printListing(
    SdFile parent, const char * const prepend
    OPTARG(CUSTOM_FIRMWARE_UPLOAD, const bool onlyBin=false)
    OPTARG(LONG_FILENAME_HOST_SUPPORT, const bool includeLongNames=false)
    OPTARG(LONG_FILENAME_HOST_SUPPORT, const char * const prependLong=nullptr)
  );

  #if ENABLED(SDCARD_SORT_ALPHA)
    static void flush_presort();
  #endif
};

#if ENABLED(USB_FLASH_DRIVE_SUPPORT)
  #define IS_SD_INSERTED() DiskIODriver_USBFlash::isInserted()
#elif PIN_EXISTS(SD_DETECT)
  #define IS_SD_INSERTED() (READ(SD_DETECT_PIN) == SD_DETECT_STATE)
#else
  // No card detect line? Assume the card is inserted.
  #define IS_SD_INSERTED() true
#endif

#define IS_SD_PRINTING()  (card.flag.sdprinting && !card.flag.abort_sd_printing)
#define IS_SD_FETCHING()  (!card.flag.sdprintdone && IS_SD_PRINTING())
#define IS_SD_PAUSED()    card.isPaused()
#define IS_SD_FILE_OPEN() card.isFileOpen()

extern CardReader card;

#else // !SDSUPPORT

#define IS_SD_PRINTING()  false
#define IS_SD_FETCHING()  false
#define IS_SD_PAUSED()    false
#define IS_SD_FILE_OPEN() false

#define LONG_FILENAME_LENGTH 0

#endif // !SDSUPPORT
