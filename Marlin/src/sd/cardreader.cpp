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

#include "../inc/MarlinConfig.h"

#if ENABLED(SDSUPPORT)

//#define DEBUG_CARDREADER

#include "cardreader.h"

#include "../MarlinCore.h"
#include "../lcd/marlinui.h"

#if ENABLED(DWIN_CREALITY_LCD)
  #include "../lcd/dwin/e3v2/dwin.h"
#endif

#include "../module/planner.h"        // for synchronize
#include "../module/printcounter.h"
#include "../gcode/queue.h"
#include "../module/settings.h"
#include "../module/stepper/indirection.h"

#if ENABLED(EMERGENCY_PARSER)
  #include "../feature/e_parser.h"
#endif

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../feature/powerloss.h"
#endif

#if ENABLED(ADVANCED_PAUSE_FEATURE)
  #include "../feature/pause.h"
#endif

#define DEBUG_OUT EITHER(DEBUG_CARDREADER, MARLIN_DEV_MODE)
#include "../core/debug_out.h"
#include "../libs/hex_print.h"

// extern

PGMSTR(M21_STR, "M21");
PGMSTR(M23_STR, "M23 %s");
PGMSTR(M24_STR, "M24");

// public:

card_flags_t CardReader::flag;
char CardReader::filename[FILENAME_LENGTH], CardReader::longFilename[LONG_FILENAME_LENGTH];

IF_DISABLED(NO_SD_AUTOSTART, uint8_t CardReader::autofile_index); // = 0

#if BOTH(HAS_MULTI_SERIAL, BINARY_FILE_TRANSFER)
  serial_index_t CardReader::transfer_port_index;
#endif

// private:

SdFile CardReader::root, CardReader::workDir, CardReader::workDirParents[MAX_DIR_DEPTH];
uint8_t CardReader::workDirDepth;

#if ENABLED(SDCARD_SORT_ALPHA)

  uint16_t CardReader::sort_count;
  #if ENABLED(SDSORT_GCODE)
    bool CardReader::sort_alpha;
    int CardReader::sort_folders;
    //bool CardReader::sort_reverse;
  #endif

  #if ENABLED(SDSORT_DYNAMIC_RAM)
    uint8_t *CardReader::sort_order;
  #else
    uint8_t CardReader::sort_order[SDSORT_LIMIT];
  #endif

  #if ENABLED(SDSORT_USES_RAM)

    #if ENABLED(SDSORT_CACHE_NAMES)
      uint16_t CardReader::nrFiles; // Cached total file count
      #if ENABLED(SDSORT_DYNAMIC_RAM)
        char **CardReader::sortshort, **CardReader::sortnames;
      #else
        char CardReader::sortshort[SDSORT_LIMIT][FILENAME_LENGTH];
        char CardReader::sortnames[SDSORT_LIMIT][SORTED_LONGNAME_STORAGE];
      #endif
    #elif DISABLED(SDSORT_USES_STACK)
      char CardReader::sortnames[SDSORT_LIMIT][SORTED_LONGNAME_STORAGE];
    #endif

    #if HAS_FOLDER_SORTING
      #if ENABLED(SDSORT_DYNAMIC_RAM)
        uint8_t *CardReader::isDir;
      #elif ENABLED(SDSORT_CACHE_NAMES) || DISABLED(SDSORT_USES_STACK)
        uint8_t CardReader::isDir[(SDSORT_LIMIT+7)>>3];
      #endif
      #define IS_DIR(n) TEST(isDir[(n) >> 3], (n) & 0x07)
    #endif

  #endif // SDSORT_USES_RAM

#endif // SDCARD_SORT_ALPHA

#if HAS_USB_FLASH_DRIVE
  DiskIODriver_USBFlash CardReader::media_driver_usbFlash;
#endif

#if NEED_SD2CARD_SDIO || NEED_SD2CARD_SPI
  CardReader::sdcard_driver_t CardReader::media_driver_sdcard;
#endif

DiskIODriver* CardReader::driver = nullptr;
SdVolume CardReader::volume;
SdFile CardReader::file;

#if HAS_MEDIA_SUBCALLS
  uint8_t CardReader::file_subcall_ctr;
  uint32_t CardReader::filespos[SD_PROCEDURE_DEPTH];
  char CardReader::proc_filenames[SD_PROCEDURE_DEPTH][MAXPATHNAMELENGTH];
#endif

uint32_t CardReader::filesize, CardReader::sdpos;

CardReader::CardReader() {
  changeMedia(&
    #if HAS_USB_FLASH_DRIVE && !SHARED_VOLUME_IS(SD_ONBOARD)
      media_driver_usbFlash
    #else
      media_driver_sdcard
    #endif
  );

  #if ENABLED(SDCARD_SORT_ALPHA)
    sort_count = 0;
    #if ENABLED(SDSORT_GCODE)
      sort_alpha = true;
      sort_folders = FOLDER_SORTING;
      //sort_reverse = false;
    #endif
  #endif

  flag.sdprinting = flag.sdprintdone = flag.mounted = flag.saving = flag.logging = false;
  filesize = sdpos = 0;

  TERN_(HAS_MEDIA_SUBCALLS, file_subcall_ctr = 0);

  IF_DISABLED(NO_SD_AUTOSTART, autofile_cancel());

  workDirDepth = 0;
  ZERO(workDirParents);

  #if ENABLED(SDSUPPORT) && PIN_EXISTS(SD_DETECT)
    SET_INPUT_PULLUP(SD_DETECT_PIN);
  #endif

  #if PIN_EXISTS(SDPOWER)
    OUT_WRITE(SDPOWER_PIN, HIGH); // Power the SD reader
  #endif
}

//
// Get a DOS 8.3 filename in its useful form
//
char *createFilename(char * const buffer, const dir_t &p) {
  char *pos = buffer;
  LOOP_L_N(i, 11) {
    if (p.name[i] == ' ') continue;
    if (i == 8) *pos++ = '.';
    *pos++ = p.name[i];
  }
  *pos++ = 0;
  return buffer;
}

//
// Return 'true' if the item is a folder or G-code file
//
bool CardReader::is_dir_or_gcode(const dir_t &p) {
  //uint8_t pn0 = p.name[0];

  if ( (p.attributes & DIR_ATT_HIDDEN)                  // Hidden by attribute
    // When readDir() > 0 these must be false:
    //|| pn0 == DIR_NAME_FREE || pn0 == DIR_NAME_DELETED  // Clear or Deleted entry
    //|| pn0 == '.' || longFilename[0] == '.'             // Hidden file
    //|| !DIR_IS_FILE_OR_SUBDIR(&p)                       // Not a File or Directory
  ) return false;

  flag.filenameIsDir = DIR_IS_SUBDIR(&p);               // We know it's a File or Folder

  return (
    flag.filenameIsDir                                  // All Directories are ok
    || (p.name[8] == 'G' && p.name[9] != '~')           // Non-backup *.G* files are accepted
  );
}

//
// Get the number of (compliant) items in the folder
//
int CardReader::countItems(SdFile dir) {
  dir_t p;
  int c = 0;
  while (dir.readDir(&p, longFilename) > 0)
    c += is_dir_or_gcode(p);

  #if ALL(SDCARD_SORT_ALPHA, SDSORT_USES_RAM, SDSORT_CACHE_NAMES)
    nrFiles = c;
  #endif

  return c;
}

//
// Get file/folder info for an item by index
//
void CardReader::selectByIndex(SdFile dir, const uint8_t index) {
  dir_t p;
  for (uint8_t cnt = 0; dir.readDir(&p, longFilename) > 0;) {
    if (is_dir_or_gcode(p)) {
      if (cnt == index) {
        createFilename(filename, p);
        return;  // 0 based index
      }
      cnt++;
    }
  }
}

//
// Get file/folder info for an item by name
//
void CardReader::selectByName(SdFile dir, const char * const match) {
  dir_t p;
  for (uint8_t cnt = 0; dir.readDir(&p, longFilename) > 0; cnt++) {
    if (is_dir_or_gcode(p)) {
      createFilename(filename, p);
      if (strcasecmp(match, filename) == 0) return;
    }
  }
}

/**
 * Recursive method to print all files within a folder in flat
 * DOS 8.3 format. This style of listing is the most compatible
 * with legacy hosts.
 *
 * This method recurses to unlimited depth and lists all G-code
 * files within the given parent. If the hierarchy is very deep
 * this can blow up the stack, so a 'depth' parameter would be a
 * good addition.
 */
void CardReader::printListing(
  SdFile parent
  OPTARG(LONG_FILENAME_HOST_SUPPORT, const bool includeLongNames/*=false*/)
  , const char * const prepend/*=nullptr*/
  OPTARG(LONG_FILENAME_HOST_SUPPORT, const char * const prependLong/*=nullptr*/)
) {
  dir_t p;
  while (parent.readDir(&p, longFilename) > 0) {
    if (DIR_IS_SUBDIR(&p)) {

      size_t lenPrepend = prepend ? strlen(prepend) + 1 : 0;
      // Allocate enough stack space for the full path including / separator
      char path[lenPrepend + FILENAME_LENGTH];
      if (prepend) {
        strcpy(path, prepend);
        path[lenPrepend - 1] = '/';
      }
      char* dosFilename = path + lenPrepend;
      createFilename(dosFilename, p);

      // Get a new directory object using the full path
      // and dive recursively into it.
      SdFile child; // child.close() in destructor
      if (child.open(&parent, dosFilename, O_READ))
        #if ENABLED(LONG_FILENAME_HOST_SUPPORT)
          if (includeLongNames) {
            size_t lenPrependLong = prependLong ? strlen(prependLong) + 1 : 0;
            // Allocate enough stack space for the full long path including / separator
            char pathLong[lenPrependLong + strlen(longFilename) + 1];
            if (prependLong) {
              strcpy(pathLong, prependLong);
              pathLong[lenPrependLong - 1] = '/';
            }
            strcpy(pathLong + lenPrependLong, longFilename);
            printListing(child, true, path, pathLong);
          }
          else
            printListing(child, false, path);
        #else
          printListing(child, path);
        #endif
      else {
        SERIAL_ECHO_MSG(STR_SD_CANT_OPEN_SUBDIR, dosFilename);
        return;
      }
    }
    else if (is_dir_or_gcode(p)) {
      if (prepend) {
        SERIAL_ECHO(prepend);
        SERIAL_CHAR('/');
      }
      SERIAL_ECHO(createFilename(filename, p));
      SERIAL_CHAR(' ');
      #if ENABLED(LONG_FILENAME_HOST_SUPPORT)
        if (!includeLongNames)
      #endif
          SERIAL_ECHOLN(p.fileSize);
      #if ENABLED(LONG_FILENAME_HOST_SUPPORT)
        else {
          SERIAL_ECHO(p.fileSize);
          SERIAL_CHAR(' ');
          if (prependLong) {
            SERIAL_ECHO(prependLong);
            SERIAL_CHAR('/');
          }
          SERIAL_ECHOLN(longFilename[0] ? longFilename : "???");
        }
      #endif
    }
  }
}

//
// List all files on the SD card
//
void CardReader::ls(TERN_(LONG_FILENAME_HOST_SUPPORT, bool includeLongNames/*=false*/)) {
  if (flag.mounted) {
    root.rewind();
    printListing(root OPTARG(LONG_FILENAME_HOST_SUPPORT, includeLongNames));
  }
}

#if ENABLED(LONG_FILENAME_HOST_SUPPORT)

  //
  // Get a long pretty path based on a DOS 8.3 path
  //
  void CardReader::printLongPath(char * const path) {

    int i, pathLen = strlen(path);

    // SERIAL_ECHOPGM("Full Path: "); SERIAL_ECHOLN(path);

    // Zero out slashes to make segments
    for (i = 0; i < pathLen; i++) if (path[i] == '/') path[i] = '\0';

    SdFile diveDir = root; // start from the root for segment 1
    for (i = 0; i < pathLen;) {

      if (path[i] == '\0') i++; // move past a single nul

      char *segment = &path[i]; // The segment after most slashes

      // If a segment is empty (extra-slash) then exit
      if (!*segment) break;

      // Go to the next segment
      while (path[++i]) { }

      //SERIAL_ECHOLNPAIR("Looking for segment: ", segment);

      // Find the item, setting the long filename
      diveDir.rewind();
      selectByName(diveDir, segment);

      // Print /LongNamePart to serial output
      SERIAL_CHAR('/');
      SERIAL_ECHO(longFilename[0] ? longFilename : "???");

      // If the filename was printed then that's it
      if (!flag.filenameIsDir) break;

      // SERIAL_ECHOPGM("Opening dir: "); SERIAL_ECHOLN(segment);

      // Open the sub-item as the new dive parent
      SdFile dir;
      if (!dir.open(&diveDir, segment, O_READ)) {
        SERIAL_EOL();
        SERIAL_ECHO_START();
        SERIAL_ECHOPAIR(STR_SD_CANT_OPEN_SUBDIR, segment);
        break;
      }

      diveDir.close();
      diveDir = dir;

    } // while i<pathLen

    SERIAL_EOL();
  }

#endif // LONG_FILENAME_HOST_SUPPORT

//
// Echo the DOS 8.3 filename (and long filename, if any)
//
void CardReader::printSelectedFilename() {
  if (file.isOpen()) {
    char dosFilename[FILENAME_LENGTH];
    file.getDosName(dosFilename);
    SERIAL_ECHO(dosFilename);
    #if ENABLED(LONG_FILENAME_HOST_SUPPORT)
      selectFileByName(dosFilename);
      if (longFilename[0]) {
        SERIAL_CHAR(' ');
        SERIAL_ECHO(longFilename);
      }
    #endif
  }
  else
    SERIAL_ECHOPGM("(no file)");

  SERIAL_EOL();
}

void CardReader::mount() {
  flag.mounted = false;
  if (root.isOpen()) root.close();

  if (!driver->init(SD_SPI_SPEED, SDSS)
    #if defined(LCD_SDSS) && (LCD_SDSS != SDSS)
      && !driver->init(SD_SPI_SPEED, LCD_SDSS)
    #endif
  ) SERIAL_ECHO_MSG(STR_SD_INIT_FAIL);
  else if (!volume.init(driver))
    SERIAL_ERROR_MSG(STR_SD_VOL_INIT_FAIL);
  else if (!root.openRoot(&volume))
    SERIAL_ERROR_MSG(STR_SD_OPENROOT_FAIL);
  else {
    flag.mounted = true;
    SERIAL_ECHO_MSG(STR_SD_CARD_OK);
  }

  if (flag.mounted)
    cdroot();
  #if ENABLED(USB_FLASH_DRIVE_SUPPORT) || PIN_EXISTS(SD_DETECT)
    else if (marlin_state != MF_INITIALIZING)
      ui.set_status_P(GET_TEXT(MSG_SD_INIT_FAIL), -1);
  #endif

  ui.refresh();
}

/**
 * Handle SD card events
 */
#if MB(FYSETC_CHEETAH, FYSETC_AIO_II)
  #include "../module/stepper.h"
#endif

void CardReader::manage_media() {
  static uint8_t prev_stat = 2;       // First call, no prior state
  uint8_t stat = uint8_t(IS_SD_INSERTED());
  if (stat == prev_stat) return;

  DEBUG_ECHOLNPAIR("SD: Status changed from ", prev_stat, " to ", stat);

  flag.workDirIsRoot = true;          // Return to root on mount/release

  if (ui.detected()) {

    uint8_t old_stat = prev_stat;
    prev_stat = stat;                 // Change now to prevent re-entry

    if (stat) {                       // Media Inserted
      safe_delay(500);                // Some boards need a delay to get settled
      if (TERN1(SD_IGNORE_AT_STARTUP, old_stat != 2))
        mount();                      // Try to mount the media
      #if MB(FYSETC_CHEETAH, FYSETC_CHEETAH_V12, FYSETC_AIO_II)
        reset_stepper_drivers();      // Workaround for Cheetah bug
      #endif
      if (!isMounted()) stat = 0;     // Not mounted?
    }
    else {
      #if PIN_EXISTS(SD_DETECT)
        release();                    // Card is released
      #endif
    }

    ui.media_changed(old_stat, stat); // Update the UI

    if (stat) {
      TERN_(SDCARD_EEPROM_EMULATION, settings.first_load());
      if (old_stat == 2) {            // First mount?
        DEBUG_ECHOLNPGM("First mount.");
        #if ENABLED(POWER_LOSS_RECOVERY)
          recovery.check();           // Check for PLR file. (If not there then call autofile_begin)
        #elif DISABLED(NO_SD_AUTOSTART)
          autofile_begin();           // Look for auto0.g on the next loop
        #endif
      }
    }
  }
  else
    DEBUG_ECHOLNPGM("SD: No UI Detected.");
}

/**
 * "Release" the media by clearing the 'mounted' flag.
 * Used by M22, "Release Media", manage_media.
 */
void CardReader::release() {
  // Card removed while printing? Abort!
  if (IS_SD_PRINTING())
    abortFilePrintSoon();
  else
    endFilePrintNow();

  flag.mounted = false;
  flag.workDirIsRoot = true;
  #if ALL(SDCARD_SORT_ALPHA, SDSORT_USES_RAM, SDSORT_CACHE_NAMES)
    nrFiles = 0;
  #endif
}

/**
 * Open a G-code file and set Marlin to start processing it.
 * Enqueues M23 and M24 commands to initiate a media print.
 */
void CardReader::openAndPrintFile(const char *name) {
  char cmd[4 + strlen(name) + 1 + 3 + 1]; // Room for "M23 ", filename, "\n", "M24", and null
  sprintf_P(cmd, M23_STR, name);
  for (char *c = &cmd[4]; *c; c++) *c = tolower(*c);
  strcat_P(cmd, PSTR("\nM24"));
  queue.inject(cmd);
}

/**
 * Start or resume a media print by setting the sdprinting flag.
 * The file browser pre-sort is also purged to free up memory,
 * since you cannot browse files during active printing.
 * Used by M24 and anywhere Start / Resume applies.
 */
void CardReader::startOrResumeFilePrinting() {
  if (isMounted()) {
    flag.sdprinting = true;
    flag.sdprintdone = false;
    TERN_(SD_RESORT, flush_presort());
  }
}

//
// Run tasks upon finishing or aborting a file print.
//
void CardReader::endFilePrintNow(TERN_(SD_RESORT, const bool re_sort/*=false*/)) {
  TERN_(ADVANCED_PAUSE_FEATURE, did_pause_print = 0);
  TERN_(DWIN_CREALITY_LCD, HMI_flag.print_finish = flag.sdprinting);
  flag.abort_sd_printing = false;
  if (isFileOpen()) file.close();
  TERN_(SD_RESORT, if (re_sort) presort());
}

void CardReader::abortFilePrintNow(TERN_(SD_RESORT, const bool re_sort/*=false*/)) {
  flag.sdprinting = flag.sdprintdone = false;
  endFilePrintNow(TERN_(SD_RESORT, re_sort));
}

void CardReader::openLogFile(const char * const path) {
  flag.logging = DISABLED(SDCARD_READONLY);
  IF_DISABLED(SDCARD_READONLY, openFileWrite(path));
}

//
// Get the root-relative DOS path of the selected file
//
void CardReader::getAbsFilenameInCWD(char *dst) {
  *dst++ = '/';
  uint8_t cnt = 1;

  auto appendAtom = [&](SdFile &file) {
    file.getDosName(dst);
    while (*dst && cnt < MAXPATHNAMELENGTH) { dst++; cnt++; }
    if (cnt < MAXPATHNAMELENGTH) { *dst = '/'; dst++; cnt++; }
  };

  LOOP_L_N(i, workDirDepth)                // Loop down to current work dir
    appendAtom(workDirParents[i]);

  if (cnt < MAXPATHNAMELENGTH - (FILENAME_LENGTH) - 1) {    // Leave room for filename and nul
    appendAtom(file);
    --dst;
  }
  *dst = '\0';
}

void openFailed(const char * const fname) {
  SERIAL_ECHOLNPAIR(STR_SD_OPEN_FILE_FAIL, fname, ".");
}

void announceOpen(const uint8_t doing, const char * const path) {
  if (doing) {
    PORT_REDIRECT(SerialMask::All);
    SERIAL_ECHO_START();
    SERIAL_ECHOPGM("Now ");
    SERIAL_ECHOPGM_P(doing == 1 ? PSTR("doing") : PSTR("fresh"));
    SERIAL_ECHOLNPAIR(" file: ", path);
  }
}

//
// Open a file by DOS path for read
// The 'subcall_type' flag indicates...
//   - 0 : Standard open from host or user interface.
//   - 1 : (file open) Opening a new sub-procedure.
//   - 1 : (no file open) Opening a macro (M98).
//   - 2 : Resuming from a sub-procedure
//
void CardReader::openFileRead(const char * const path, const uint8_t subcall_type/*=0*/) {
  if (!isMounted()) return;

  switch (subcall_type) {
    case 0:      // Starting a new print. "Now fresh file: ..."
      announceOpen(2, path);
      TERN_(HAS_MEDIA_SUBCALLS, file_subcall_ctr = 0);
      break;

    #if HAS_MEDIA_SUBCALLS

      case 1:      // Starting a sub-procedure

        // With no file is open it's a simple macro. "Now doing file: ..."
        if (!isFileOpen()) { announceOpen(1, path); break; }

        // Too deep? The firmware has to bail.
        if (file_subcall_ctr > SD_PROCEDURE_DEPTH - 1) {
          SERIAL_ERROR_MSG("Exceeded max SUBROUTINE depth:", SD_PROCEDURE_DEPTH);
          kill(GET_TEXT(MSG_KILL_SUBCALL_OVERFLOW));
          return;
        }

        // Store current filename (based on workDirParents) and position
        getAbsFilenameInCWD(proc_filenames[file_subcall_ctr]);
        filespos[file_subcall_ctr] = sdpos;

        // For sub-procedures say 'SUBROUTINE CALL target: "..." parent: "..." pos12345'
        SERIAL_ECHO_MSG("SUBROUTINE CALL target:\"", path, "\" parent:\"", proc_filenames[file_subcall_ctr], "\" pos", sdpos);
        file_subcall_ctr++;
        break;

      case 2:      // Resuming previous file after sub-procedure
        SERIAL_ECHO_MSG("END SUBROUTINE");
        break;

    #endif
  }

  abortFilePrintNow();

  SdFile *diveDir;
  const char * const fname = diveToFile(true, diveDir, path);
  if (!fname) return;

  if (file.open(diveDir, fname, O_READ)) {
    filesize = file.fileSize();
    sdpos = 0;

    { // Don't remove this block, as the PORT_REDIRECT is a RAII
      PORT_REDIRECT(SerialMask::All);
      SERIAL_ECHOLNPAIR(STR_SD_FILE_OPENED, fname, STR_SD_SIZE, filesize);
      SERIAL_ECHOLNPGM(STR_SD_FILE_SELECTED);
    }

    selectFileByName(fname);
    ui.set_status(longFilename[0] ? longFilename : fname);
  }
  else
    openFailed(fname);
}

inline void echo_write_to_file(const char * const fname) {
  SERIAL_ECHOLNPAIR(STR_SD_WRITE_TO_FILE, fname);
}

//
// Open a file by DOS path for write
//
void CardReader::openFileWrite(const char * const path) {
  if (!isMounted()) return;

  announceOpen(2, path);
  TERN_(HAS_MEDIA_SUBCALLS, file_subcall_ctr = 0);

  abortFilePrintNow();

  SdFile *diveDir;
  const char * const fname = diveToFile(false, diveDir, path);
  if (!fname) return;

  #if ENABLED(SDCARD_READONLY)
    openFailed(fname);
  #else
    if (file.open(diveDir, fname, O_CREAT | O_APPEND | O_WRITE | O_TRUNC)) {
      flag.saving = true;
      selectFileByName(fname);
      TERN_(EMERGENCY_PARSER, emergency_parser.disable());
      echo_write_to_file(fname);
      ui.set_status(fname);
    }
    else
      openFailed(fname);
  #endif
}

//
// Check if a file exists by absolute or workDir-relative path
// If the file exists, the long name can also be fetched.
//
bool CardReader::fileExists(const char * const path) {
  if (!isMounted()) return false;

  DEBUG_ECHOLNPAIR("fileExists: ", path);

  // Dive to the file's directory and get the base name
  SdFile *diveDir = nullptr;
  const char * const fname = diveToFile(false, diveDir, path);
  if (!fname) return false;

  // Get the longname of the checked file
  //diveDir->rewind();
  //selectByName(*diveDir, fname);
  //diveDir->close();

  // Try to open the file and return the result
  SdFile tmpFile;
  const bool success = tmpFile.open(diveDir, fname, O_READ);
  if (success) tmpFile.close();
  return success;
}

//
// Delete a file by name in the working directory
//
void CardReader::removeFile(const char * const name) {
  if (!isMounted()) return;

  //abortFilePrintNow();

  SdFile *itsDirPtr;
  const char * const fname = diveToFile(false, itsDirPtr, name);
  if (!fname) return;

  #if ENABLED(SDCARD_READONLY)
    SERIAL_ECHOLNPAIR("Deletion failed (read-only), File: ", fname, ".");
  #else
    if (file.remove(itsDirPtr, fname)) {
      SERIAL_ECHOLNPAIR("File deleted:", fname);
      sdpos = 0;
      TERN_(SDCARD_SORT_ALPHA, presort());
    }
    else
      SERIAL_ECHOLNPAIR("Deletion failed, File: ", fname, ".");
  #endif
}

void CardReader::report_status() {
  if (isPrinting()) {
    SERIAL_ECHOPAIR(STR_SD_PRINTING_BYTE, sdpos);
    SERIAL_CHAR('/');
    SERIAL_ECHOLN(filesize);
  }
  else
    SERIAL_ECHOLNPGM(STR_SD_NOT_PRINTING);
}

void CardReader::write_command(char * const buf) {
  char *begin = buf,
       *npos = nullptr,
       *end = buf + strlen(buf) - 1;

  file.writeError = false;
  if ((npos = strchr(buf, 'N'))) {
    begin = strchr(npos, ' ') + 1;
    end = strchr(npos, '*') - 1;
  }
  end[1] = '\r';
  end[2] = '\n';
  end[3] = '\0';
  file.write(begin);

  if (file.writeError) SERIAL_ERROR_MSG(STR_SD_ERR_WRITE_TO_FILE);
}

#if DISABLED(NO_SD_AUTOSTART)
  /**
   * Run all the auto#.g files. Called:
   * - On boot after successful card init.
   * - From the LCD command to Run Auto Files
   */
  void CardReader::autofile_begin() {
    autofile_index = 1;
    (void)autofile_check();
  }

  /**
   * Run the next auto#.g file. Called:
   *   - On boot after successful card init
   *   - After finishing the previous auto#.g file
   *   - From the LCD command to begin the auto#.g files
   *
   * Return 'true' if an auto file was started
   */
  bool CardReader::autofile_check() {
    if (!autofile_index) return false;

    if (!isMounted())
      mount();
    else if (ENABLED(SDCARD_EEPROM_EMULATION))
      settings.first_load();

    // Don't run auto#.g when a PLR file exists
    if (isMounted() && TERN1(POWER_LOSS_RECOVERY, !recovery.valid())) {
      char autoname[10];
      sprintf_P(autoname, PSTR("/auto%c.g"), '0' + autofile_index - 1);
      if (fileExists(autoname)) {
        cdroot();
        openAndPrintFile(autoname);
        autofile_index++;
        return true;
      }
    }
    autofile_cancel();
    return false;
  }
#endif

void CardReader::closefile(const bool store_location/*=false*/) {
  file.sync();
  file.close();
  flag.saving = flag.logging = false;
  sdpos = 0;
  TERN_(EMERGENCY_PARSER, emergency_parser.enable());

  if (store_location) {
    //future: store printer state, filename and position for continuing a stopped print
    // so one can unplug the printer and continue printing the next day.
  }
}

//
// Get info for a file in the working directory by index
//
void CardReader::selectFileByIndex(const uint16_t nr) {
  #if ENABLED(SDSORT_CACHE_NAMES)
    if (nr < sort_count) {
      strcpy(filename, sortshort[nr]);
      strcpy(longFilename, sortnames[nr]);
      flag.filenameIsDir = IS_DIR(nr);
      return;
    }
  #endif
  workDir.rewind();
  selectByIndex(workDir, nr);
}

//
// Get info for a file in the working directory by DOS name
//
void CardReader::selectFileByName(const char * const match) {
  #if ENABLED(SDSORT_CACHE_NAMES)
    for (uint16_t nr = 0; nr < sort_count; nr++)
      if (strcasecmp(match, sortshort[nr]) == 0) {
        strcpy(filename, sortshort[nr]);
        strcpy(longFilename, sortnames[nr]);
        flag.filenameIsDir = IS_DIR(nr);
        return;
      }
  #endif
  workDir.rewind();
  selectByName(workDir, match);
}

uint16_t CardReader::countFilesInWorkDir() {
  workDir.rewind();
  return countItems(workDir);
}

/**
 * Dive to the given DOS 8.3 file path, with optional echo of the dive paths.
 *
 * On entry:
 *  - The workDir points to the last-set navigation target by cd, cdup, cdroot, or diveToFile(true, ...)
 *
 * On exit:
 *  - Your curDir pointer contains an SdFile reference to the file's directory.
 *  - If update_cwd was 'true' the workDir now points to the file's directory.
 *
 * Returns a pointer to the last segment (filename) of the given DOS 8.3 path.
 * On exit, inDirPtr contains an SdFile reference to the file's directory.
 *
 * A nullptr result indicates an unrecoverable error.
 *
 * NOTE: End the path with a slash to dive to a folder. In this case the
 *       returned filename will be blank (points to the end of the path).
 */
const char* CardReader::diveToFile(const bool update_cwd, SdFile* &inDirPtr, const char * const path, const bool echo/*=false*/) {
  DEBUG_SECTION(est, "diveToFile", true);

  // Track both parent and subfolder
  static SdFile newDir1, newDir2;
  SdFile *sub = &newDir1, *startDirPtr;

  // Parsing the path string
  const char *atom_ptr = path;

  DEBUG_ECHOLNPAIR(" path = '", path, "'");

  if (path[0] == '/') {               // Starting at the root directory?
    inDirPtr = &root;
    atom_ptr++;
    DEBUG_ECHOLNPAIR(" CWD to root: ", hex_address((void*)inDirPtr));
    if (update_cwd) workDirDepth = 0; // The cwd can be updated for the benefit of sub-programs
  }
  else
    inDirPtr = &workDir;              // Dive from workDir (as set by the UI)

  startDirPtr = inDirPtr;

  DEBUG_ECHOLNPAIR(" startDirPtr = ", hex_address((void*)startDirPtr));

  while (atom_ptr) {
    // Find next subdirectory delimiter
    char * const name_end = strchr(atom_ptr, '/');

    // Last atom in the path? Item found.
    if (name_end <= atom_ptr) break;

    // Isolate the next subitem name
    const uint8_t len = name_end - atom_ptr;
    char dosSubdirname[len + 1];
    strncpy(dosSubdirname, atom_ptr, len);
    dosSubdirname[len] = 0;

    if (echo) SERIAL_ECHOLN(dosSubdirname);

    DEBUG_ECHOLNPAIR(" sub = ", hex_address((void*)sub));

    // Open inDirPtr (closing first)
    sub->close();
    if (!sub->open(inDirPtr, dosSubdirname, O_READ)) {
      openFailed(dosSubdirname);
      atom_ptr = nullptr;
      break;
    }

    // Close inDirPtr if not at starting-point
    if (inDirPtr != startDirPtr) {
      DEBUG_ECHOLNPAIR(" closing inDirPtr: ", hex_address((void*)inDirPtr));
      inDirPtr->close();
    }

    // inDirPtr now subDir
    inDirPtr = sub;
    DEBUG_ECHOLNPAIR(" inDirPtr = sub: ", hex_address((void*)inDirPtr));

    // Update workDirParents and workDirDepth
    if (update_cwd) {
      DEBUG_ECHOLNPAIR(" update_cwd");
      if (workDirDepth < MAX_DIR_DEPTH)
        workDirParents[workDirDepth++] = *inDirPtr;
    }

    // Point sub at the other scratch object
    sub = (inDirPtr != &newDir1) ? &newDir1 : &newDir2;
    DEBUG_ECHOLNPAIR(" swapping sub = ", hex_address((void*)sub));

    // Next path atom address
    atom_ptr = name_end + 1;
  }

  if (update_cwd) {
    workDir = *inDirPtr;
    DEBUG_ECHOLNPAIR(" final workDir = ", hex_address((void*)inDirPtr));
    flag.workDirIsRoot = (workDirDepth == 0);
    TERN_(SDCARD_SORT_ALPHA, presort());
  }

  DEBUG_ECHOLNPAIR(" returning string ", atom_ptr ?: "nullptr");
  return atom_ptr;
}

void CardReader::cd(const char * relpath) {
  SdFile newDir, *parent = &getWorkDir();

  if (newDir.open(parent, relpath, O_READ)) {
    workDir = newDir;
    flag.workDirIsRoot = false;
    if (workDirDepth < MAX_DIR_DEPTH)
      workDirParents[workDirDepth++] = workDir;
    TERN_(SDCARD_SORT_ALPHA, presort());
  }
  else
    SERIAL_ECHO_MSG(STR_SD_CANT_ENTER_SUBDIR, relpath);
}

int8_t CardReader::cdup() {
  if (workDirDepth > 0) {                                               // At least 1 dir has been saved
    workDir = --workDirDepth ? workDirParents[workDirDepth - 1] : root; // Use parent, or root if none
    TERN_(SDCARD_SORT_ALPHA, presort());
  }
  if (!workDirDepth) flag.workDirIsRoot = true;
  return workDirDepth;
}

void CardReader::cdroot() {
  workDir = root;
  flag.workDirIsRoot = true;
  TERN_(SDCARD_SORT_ALPHA, presort());
}

#if ENABLED(SDCARD_SORT_ALPHA)

  /**
   * Get the name of a file in the working directory by sort-index
   */
  void CardReader::getfilename_sorted(const uint16_t nr) {
    selectFileByIndex(TERN1(SDSORT_GCODE, sort_alpha) && (nr < sort_count)
      ? sort_order[nr] : nr);
  }

  #if ENABLED(SDSORT_USES_RAM)
    #if ENABLED(SDSORT_DYNAMIC_RAM)
      // Use dynamic method to copy long filename
      #define SET_SORTNAME(I) (sortnames[I] = strdup(longest_filename()))
      #if ENABLED(SDSORT_CACHE_NAMES)
        // When caching also store the short name, since
        // we're replacing the selectFileByIndex() behavior.
        #define SET_SORTSHORT(I) (sortshort[I] = strdup(filename))
      #else
        #define SET_SORTSHORT(I) NOOP
      #endif
    #else
      // Copy filenames into the static array
      #define _SET_SORTNAME(I) strncpy(sortnames[I], longest_filename(), SORTED_LONGNAME_MAXLEN)
      #if SORTED_LONGNAME_MAXLEN == LONG_FILENAME_LENGTH
        // Short name sorting always use LONG_FILENAME_LENGTH with no trailing nul
        #define SET_SORTNAME(I) _SET_SORTNAME(I)
      #else
        // Copy multiple name blocks. Add a nul for the longest case.
        #define SET_SORTNAME(I) do{ _SET_SORTNAME(I); sortnames[I][SORTED_LONGNAME_MAXLEN] = '\0'; }while(0)
      #endif
      #if ENABLED(SDSORT_CACHE_NAMES)
        #define SET_SORTSHORT(I) strcpy(sortshort[I], filename)
      #else
        #define SET_SORTSHORT(I) NOOP
      #endif
    #endif
  #endif

  /**
   * Read all the files and produce a sort key
   *
   * We can do this in 3 ways...
   *  - Minimal RAM: Read two filenames at a time sorting along...
   *  - Some RAM: Buffer the directory just for this sort
   *  - Most RAM: Buffer the directory and return filenames from RAM
   */
  void CardReader::presort() {

    // Throw away old sort index
    flush_presort();

    // Sorting may be turned off
    if (TERN0(SDSORT_GCODE, !sort_alpha)) return;

    // If there are files, sort up to the limit
    uint16_t fileCnt = countFilesInWorkDir();
    if (fileCnt > 0) {

      // Never sort more than the max allowed
      // If you use folders to organize, 20 may be enough
      NOMORE(fileCnt, uint16_t(SDSORT_LIMIT));

      // Sort order is always needed. May be static or dynamic.
      TERN_(SDSORT_DYNAMIC_RAM, sort_order = new uint8_t[fileCnt]);

      // Use RAM to store the entire directory during pre-sort.
      // SDSORT_LIMIT should be set to prevent over-allocation.
      #if ENABLED(SDSORT_USES_RAM)

        // If using dynamic ram for names, allocate on the heap.
        #if ENABLED(SDSORT_CACHE_NAMES)
          #if ENABLED(SDSORT_DYNAMIC_RAM)
            sortshort = new char*[fileCnt];
            sortnames = new char*[fileCnt];
          #endif
        #elif ENABLED(SDSORT_USES_STACK)
          char sortnames[fileCnt][SORTED_LONGNAME_STORAGE];
        #endif

        // Folder sorting needs 1 bit per entry for flags.
        #if HAS_FOLDER_SORTING
          #if ENABLED(SDSORT_DYNAMIC_RAM)
            isDir = new uint8_t[(fileCnt + 7) >> 3];
          #elif ENABLED(SDSORT_USES_STACK)
            uint8_t isDir[(fileCnt + 7) >> 3];
          #endif
        #endif

      #else // !SDSORT_USES_RAM

        // By default re-read the names from SD for every compare
        // retaining only two filenames at a time. This is very
        // slow but is safest and uses minimal RAM.
        char name1[LONG_FILENAME_LENGTH];

      #endif

      if (fileCnt > 1) {

        // Init sort order.
        for (uint16_t i = 0; i < fileCnt; i++) {
          sort_order[i] = i;
          // If using RAM then read all filenames now.
          #if ENABLED(SDSORT_USES_RAM)
            selectFileByIndex(i);
            SET_SORTNAME(i);
            SET_SORTSHORT(i);
            // char out[30];
            // sprintf_P(out, PSTR("---- %i %s %s"), i, flag.filenameIsDir ? "D" : " ", sortnames[i]);
            // SERIAL_ECHOLN(out);
            #if HAS_FOLDER_SORTING
              const uint16_t bit = i & 0x07, ind = i >> 3;
              if (bit == 0) isDir[ind] = 0x00;
              if (flag.filenameIsDir) SBI(isDir[ind], bit);
            #endif
          #endif
        }

        // Bubble Sort
        for (uint16_t i = fileCnt; --i;) {
          bool didSwap = false;
          uint8_t o1 = sort_order[0];
          #if DISABLED(SDSORT_USES_RAM)
            selectFileByIndex(o1);              // Pre-fetch the first entry and save it
            strcpy(name1, longest_filename());  // so the loop only needs one fetch
            #if ENABLED(HAS_FOLDER_SORTING)
              bool dir1 = flag.filenameIsDir;
            #endif
          #endif

          for (uint16_t j = 0; j < i; ++j) {
            const uint16_t o2 = sort_order[j + 1];

            // Compare names from the array or just the two buffered names
            #if ENABLED(SDSORT_USES_RAM)
              #define _SORT_CMP_NODIR() (strcasecmp(sortnames[o1], sortnames[o2]) > 0)
            #else
              #define _SORT_CMP_NODIR() (strcasecmp(name1, name2) > 0)
            #endif

            #if HAS_FOLDER_SORTING
              #if ENABLED(SDSORT_USES_RAM)
                // Folder sorting needs an index and bit to test for folder-ness.
                #define _SORT_CMP_DIR(fs) (IS_DIR(o1) == IS_DIR(o2) ? _SORT_CMP_NODIR() : IS_DIR(fs > 0 ? o1 : o2))
              #else
                #define _SORT_CMP_DIR(fs) ((dir1 == flag.filenameIsDir) ? _SORT_CMP_NODIR() : (fs > 0 ? dir1 : !dir1))
              #endif
            #endif

            // The most economical method reads names as-needed
            // throughout the loop. Slow if there are many.
            #if DISABLED(SDSORT_USES_RAM)
              selectFileByIndex(o2);
              const bool dir2 = flag.filenameIsDir;
              char * const name2 = longest_filename(); // use the string in-place
            #endif // !SDSORT_USES_RAM

            // Sort the current pair according to settings.
            if (
              #if HAS_FOLDER_SORTING
                #if ENABLED(SDSORT_GCODE)
                  sort_folders ? _SORT_CMP_DIR(sort_folders) : _SORT_CMP_NODIR()
                #else
                  _SORT_CMP_DIR(FOLDER_SORTING)
                #endif
              #else
                _SORT_CMP_NODIR()
              #endif
            ) {
              // Reorder the index, indicate that sorting happened
              // Note that the next o1 will be the current o1. No new fetch needed.
              sort_order[j] = o2;
              sort_order[j + 1] = o1;
              didSwap = true;
            }
            else {
              // The next o1 is the current o2. No new fetch needed.
              o1 = o2;
              #if DISABLED(SDSORT_USES_RAM)
                TERN_(HAS_FOLDER_SORTING, dir1 = dir2);
                strcpy(name1, name2);
              #endif
            }
          }
          if (!didSwap) break;
        }
        // Using RAM but not keeping names around
        #if ENABLED(SDSORT_USES_RAM) && DISABLED(SDSORT_CACHE_NAMES)
          #if ENABLED(SDSORT_DYNAMIC_RAM)
            for (uint16_t i = 0; i < fileCnt; ++i) free(sortnames[i]);
            TERN_(HAS_FOLDER_SORTING, free(isDir));
          #endif
        #endif
      }
      else {
        sort_order[0] = 0;
        #if BOTH(SDSORT_USES_RAM, SDSORT_CACHE_NAMES)
          #if ENABLED(SDSORT_DYNAMIC_RAM)
            sortnames = new char*[1];
            sortshort = new char*[1];
            isDir = new uint8_t[1];
          #endif
          selectFileByIndex(0);
          SET_SORTNAME(0);
          SET_SORTSHORT(0);
          isDir[0] = flag.filenameIsDir;
        #endif
      }

      sort_count = fileCnt;
    }
  }

  void CardReader::flush_presort() {
    if (sort_count > 0) {
      #if ENABLED(SDSORT_DYNAMIC_RAM)
        delete sort_order;
        #if ENABLED(SDSORT_CACHE_NAMES)
          LOOP_L_N(i, sort_count) {
            free(sortshort[i]); // strdup
            free(sortnames[i]); // strdup
          }
          delete sortshort;
          delete sortnames;
        #endif
      #endif
      sort_count = 0;
    }
  }

#endif // SDCARD_SORT_ALPHA

uint16_t CardReader::get_num_Files() {
  if (!isMounted()) return 0;
  return (
    #if ALL(SDCARD_SORT_ALPHA, SDSORT_USES_RAM, SDSORT_CACHE_NAMES)
      nrFiles // no need to access the SD card for filenames
    #else
      countFilesInWorkDir()
    #endif
  );
}

//
// Return from procedure or close out the Print Job
//
void CardReader::fileHasFinished() {
  file.close();
  #if HAS_MEDIA_SUBCALLS
    if (file_subcall_ctr > 0) { // Resume calling file after closing procedure
      file_subcall_ctr--;
      openFileRead(proc_filenames[file_subcall_ctr], 2); // 2 = Returning from sub-procedure
      setIndex(filespos[file_subcall_ctr]);
      startOrResumeFilePrinting();
      return;
    }
  #endif

  endFilePrintNow(TERN_(SD_RESORT, true));

  flag.sdprintdone = true;        // Stop getting bytes from the SD card
  marlin_state = MF_SD_COMPLETE;  // Tell Marlin to enqueue M1001 soon
}

#if ENABLED(AUTO_REPORT_SD_STATUS)
  AutoReporter<CardReader::AutoReportSD> CardReader::auto_reporter;
#endif

#if ENABLED(POWER_LOSS_RECOVERY)

  bool CardReader::jobRecoverFileExists() {
    const bool exists = recovery.file.open(&root, recovery.filename, O_READ);
    if (exists) recovery.file.close();
    return exists;
  }

  void CardReader::openJobRecoveryFile(const bool read) {
    if (!isMounted()) return;
    if (recovery.file.isOpen()) return;
    if (!recovery.file.open(&root, recovery.filename, read ? O_READ : O_CREAT | O_WRITE | O_TRUNC | O_SYNC))
      openFailed(recovery.filename);
    else if (!read)
      echo_write_to_file(recovery.filename);
  }

  // Removing the job recovery file currently requires closing
  // the file being printed, so during SD printing the file should
  // be zeroed and written instead of deleted.
  void CardReader::removeJobRecoveryFile() {
    if (jobRecoverFileExists()) {
      recovery.init();
      removeFile(recovery.filename);
      #if ENABLED(DEBUG_POWER_LOSS_RECOVERY)
        SERIAL_ECHOPGM("Power-loss file delete");
        SERIAL_ECHOPGM_P(jobRecoverFileExists() ? PSTR(" failed.\n") : PSTR("d.\n"));
      #endif
    }
  }

#endif // POWER_LOSS_RECOVERY

#endif // SDSUPPORT
