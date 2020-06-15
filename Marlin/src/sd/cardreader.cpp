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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "../inc/MarlinConfig.h"

#if ENABLED(SDSUPPORT)

#include "cardreader.h"

#include "../MarlinCore.h"
#include "../lcd/ultralcd.h"
#include "../module/planner.h"        // for synchronize
#include "../module/printcounter.h"
#include "../gcode/queue.h"
#include "../module/configuration_store.h"

#if ENABLED(EMERGENCY_PARSER)
  #include "../feature/e_parser.h"
#endif

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../feature/powerloss.h"
#endif

#if ENABLED(ADVANCED_PAUSE_FEATURE)
  #include "../feature/pause.h"
#endif

// public:

card_flags_t CardReader::flag;
char CardReader::filename[FILENAME_LENGTH], CardReader::longFilename[LONG_FILENAME_LENGTH];
int8_t CardReader::autostart_index;

#if BOTH(HAS_MULTI_SERIAL, BINARY_FILE_TRANSFER)
  int8_t CardReader::transfer_port_index;
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

Sd2Card CardReader::sd2card;
SdVolume CardReader::volume;
SdFile CardReader::file;

uint8_t CardReader::file_subcall_ctr;
uint32_t CardReader::filespos[SD_PROCEDURE_DEPTH];
char CardReader::proc_filenames[SD_PROCEDURE_DEPTH][MAXPATHNAMELENGTH];

uint32_t CardReader::filesize, CardReader::sdpos;

CardReader::CardReader() {
  #if ENABLED(SDCARD_SORT_ALPHA)
    sort_count = 0;
    #if ENABLED(SDSORT_GCODE)
      sort_alpha = true;
      sort_folders = FOLDER_SORTING;
      //sort_reverse = false;
    #endif
  #endif
  flag.sdprinting = flag.mounted = flag.saving = flag.logging = false;
  filesize = sdpos = 0;
  file_subcall_ctr = 0;

  workDirDepth = 0;
  ZERO(workDirParents);

  // Disable autostart until card is initialized
  autostart_index = -1;

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

  #if ENABLED(SDCARD_SORT_ALPHA) && SDSORT_USES_RAM && SDSORT_CACHE_NAMES
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

//
// Recursive method to list all files within a folder
//
void CardReader::printListing(SdFile parent, const char * const prepend/*=nullptr*/) {
  dir_t p;
  while (parent.readDir(&p, longFilename) > 0) {
    if (DIR_IS_SUBDIR(&p)) {

      // Get the short name for the item, which we know is a folder
      char dosFilename[FILENAME_LENGTH];
      createFilename(dosFilename, p);

      // Allocate enough stack space for the full path to a folder, trailing slash, and nul
      const bool prepend_is_empty = (!prepend || prepend[0] == '\0');
      const int len = (prepend_is_empty ? 1 : strlen(prepend)) + strlen(dosFilename) + 1 + 1;
      char path[len];

      // Append the FOLDERNAME12/ to the passed string.
      // It contains the full path to the "parent" argument.
      // We now have the full path to the item in this folder.
      strcpy(path, prepend_is_empty ? "/" : prepend); // root slash if prepend is empty
      strcat(path, dosFilename);                      // FILENAME_LENGTH characters maximum
      strcat(path, "/");                              // 1 character

      // Serial.print(path);

      // Get a new directory object using the full path
      // and dive recursively into it.
      SdFile child;
      if (!child.open(&parent, dosFilename, O_READ)) {
        SERIAL_ECHO_START();
        SERIAL_ECHOLNPAIR(STR_SD_CANT_OPEN_SUBDIR, dosFilename);
      }
      printListing(child, path);
      // close() is done automatically by destructor of SdFile
    }
    else if (is_dir_or_gcode(p)) {
      createFilename(filename, p);
      if (prepend) SERIAL_ECHO(prepend);
      SERIAL_ECHO(filename);
      SERIAL_CHAR(' ');
      SERIAL_ECHOLN(p.fileSize);
    }
  }
}

//
// List all files on the SD card
//
void CardReader::ls() {
  root.rewind();
  printListing(root);
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

      // SERIAL_ECHOPGM("Looking for segment: "); SERIAL_ECHOLN(segment);

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
void CardReader::printFilename() {
  if (file.isOpen()) {
    char dosFilename[FILENAME_LENGTH];
    file.getDosName(dosFilename);
    SERIAL_ECHO(dosFilename);
    #if ENABLED(LONG_FILENAME_HOST_SUPPORT)
      selectFileByName(dosFilename);
      if (longFilename[0]) {
        SERIAL_ECHO(' ');
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

  if (!sd2card.init(SPI_SPEED, SDSS)
    #if defined(LCD_SDSS) && (LCD_SDSS != SDSS)
      && !sd2card.init(SPI_SPEED, LCD_SDSS)
    #endif
  ) SERIAL_ECHO_MSG(STR_SD_INIT_FAIL);
  else if (!volume.init(&sd2card))
    SERIAL_ERROR_MSG(STR_SD_VOL_INIT_FAIL);
  else if (!root.openRoot(&volume))
    SERIAL_ERROR_MSG(STR_SD_OPENROOT_FAIL);
  else {
    flag.mounted = true;
    SERIAL_ECHO_MSG(STR_SD_CARD_OK);
  }
  cdroot();

  ui.refresh();
}

/**
 * Handle SD card events
 */
#if MB(FYSETC_CHEETAH, FYSETC_AIO_II)
  #include "../module/stepper.h"
#endif

void CardReader::manage_media() {
  static uint8_t prev_stat = TERN(INIT_SDCARD_ON_BOOT, 2, 0);
  uint8_t stat = uint8_t(IS_SD_INSERTED());
  if (stat != prev_stat && ui.detected()) {

    uint8_t old_stat = prev_stat;
    prev_stat = stat;                 // Change now to prevent re-entry

    if (stat) {                       // Media Inserted
      safe_delay(500);                // Some boards need a delay to get settled
      mount();                        // Try to mount the media
      #if MB(FYSETC_CHEETAH, FYSETC_AIO_II)
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
      if (old_stat == 2)              // First mount?
        TERN(POWER_LOSS_RECOVERY,
          recovery.check(),           // Check for PLR file. (If not there it will beginautostart)
          beginautostart()            // Look for auto0.g on the next loop
        );
    }
  }
}

void CardReader::release() {
  endFilePrint();
  flag.mounted = false;
}

void CardReader::openAndPrintFile(const char *name) {
  char cmd[4 + strlen(name) + 1]; // Room for "M23 ", filename, and null
  extern const char M23_STR[];
  sprintf_P(cmd, M23_STR, name);
  for (char *c = &cmd[4]; *c; c++) *c = tolower(*c);
  queue.enqueue_one_now(cmd);
  queue.enqueue_now_P(M24_STR);
}

void CardReader::startFileprint() {
  if (isMounted()) {
    flag.sdprinting = true;
    TERN_(SD_RESORT, flush_presort());
  }
}

void CardReader::endFilePrint(TERN_(SD_RESORT, const bool re_sort/*=false*/)) {
  TERN_(ADVANCED_PAUSE_FEATURE, did_pause_print = 0);
  flag.sdprinting = flag.abort_sd_printing = false;
  if (isFileOpen()) file.close();
  TERN_(SD_RESORT, if (re_sort) presort());
}

void CardReader::openLogFile(char * const path) {
  flag.logging = DISABLED(SDCARD_READONLY);
  TERN(SDCARD_READONLY,,openFileWrite(path));
}

//
// Get the root-relative DOS path of the selected file
//
void CardReader::getAbsFilename(char *dst) {
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
    PORT_REDIRECT(SERIAL_BOTH);
    SERIAL_ECHO_START();
    SERIAL_ECHOPGM("Now ");
    serialprintPGM(doing == 1 ? PSTR("doing") : PSTR("fresh"));
    SERIAL_ECHOLNPAIR(" file: ", path);
    PORT_RESTORE();
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
void CardReader::openFileRead(char * const path, const uint8_t subcall_type/*=0*/) {
  if (!isMounted()) return;

  switch (subcall_type) {
    case 0:      // Starting a new print. "Now fresh file: ..."
      announceOpen(2, path);
      file_subcall_ctr = 0;
      break;

    case 1:      // Starting a sub-procedure

      // With no file is open it's a simple macro. "Now doing file: ..."
      if (!isFileOpen()) { announceOpen(1, path); break; }

      // Too deep? The firmware has to bail.
      if (file_subcall_ctr > SD_PROCEDURE_DEPTH - 1) {
        SERIAL_ERROR_MSG("Exceeded max SUBROUTINE depth:" STRINGIFY(SD_PROCEDURE_DEPTH));
        kill(GET_TEXT(MSG_KILL_SUBCALL_OVERFLOW));
        return;
      }

      // Store current filename (based on workDirParents) and position
      getAbsFilename(proc_filenames[file_subcall_ctr]);
      filespos[file_subcall_ctr] = sdpos;

      // For sub-procedures say 'SUBROUTINE CALL target: "..." parent: "..." pos12345'
      SERIAL_ECHO_START();
      SERIAL_ECHOLNPAIR("SUBROUTINE CALL target:\"", path, "\" parent:\"", proc_filenames[file_subcall_ctr], "\" pos", sdpos);
      file_subcall_ctr++;
      break;

    case 2:      // Resuming previous file after sub-procedure
      SERIAL_ECHO_MSG("END SUBROUTINE");
      break;
  }

  endFilePrint();

  SdFile *curDir;
  const char * const fname = diveToFile(true, curDir, path);
  if (!fname) return;

  if (file.open(curDir, fname, O_READ)) {
    filesize = file.fileSize();
    sdpos = 0;

    PORT_REDIRECT(SERIAL_BOTH);
    SERIAL_ECHOLNPAIR(STR_SD_FILE_OPENED, fname, STR_SD_SIZE, filesize);
    SERIAL_ECHOLNPGM(STR_SD_FILE_SELECTED);
    PORT_RESTORE();

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
void CardReader::openFileWrite(char * const path) {
  if (!isMounted()) return;

  announceOpen(2, path);
  file_subcall_ctr = 0;

  endFilePrint();

  SdFile *curDir;
  const char * const fname = diveToFile(false, curDir, path);
  if (!fname) return;

  #if ENABLED(SDCARD_READONLY)
    openFailed(fname);
  #else
    if (file.open(curDir, fname, O_CREAT | O_APPEND | O_WRITE | O_TRUNC)) {
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
// Delete a file by name in the working directory
//
void CardReader::removeFile(const char * const name) {
  if (!isMounted()) return;

  //endFilePrint();

  SdFile *curDir;
  const char * const fname = diveToFile(false, curDir, name);
  if (!fname) return;

  #if ENABLED(SDCARD_READONLY)
    SERIAL_ECHOLNPAIR("Deletion failed (read-only), File: ", fname, ".");
  #else
    if (file.remove(curDir, fname)) {
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
    SERIAL_ECHOPGM(STR_SD_PRINTING_BYTE);
    SERIAL_ECHO(sdpos);
    SERIAL_CHAR('/');
    SERIAL_ECHOLN(filesize);
  }
  else
    SERIAL_ECHOLNPGM(STR_SD_NOT_PRINTING);
}

void CardReader::write_command(char * const buf) {
  char* begin = buf;
  char* npos = nullptr;
  char* end = buf + strlen(buf) - 1;

  file.writeError = false;
  if ((npos = strchr(buf, 'N')) != nullptr) {
    begin = strchr(npos, ' ') + 1;
    end = strchr(npos, '*') - 1;
  }
  end[1] = '\r';
  end[2] = '\n';
  end[3] = '\0';
  file.write(begin);

  if (file.writeError) SERIAL_ERROR_MSG(STR_SD_ERR_WRITE_TO_FILE);
}

//
// Run the next autostart file. Called:
// - On boot after successful card init
// - After finishing the previous autostart file
// - From the LCD command to run the autostart file
//

void CardReader::checkautostart() {

  if (autostart_index < 0 || flag.sdprinting) return;

  if (!isMounted()) mount();
  TERN_(SDCARD_EEPROM_EMULATION, else settings.first_load());

  // Don't run auto#.g when a PLR file exists
  if (isMounted() && TERN1(POWER_LOSS_RECOVERY, !recovery.valid())) {
    char autoname[8];
    sprintf_P(autoname, PSTR("auto%c.g"), autostart_index + '0');
    dir_t p;
    root.rewind();
    while (root.readDir(&p, nullptr) > 0) {
      for (int8_t i = (int8_t)strlen((char*)p.name); i--;) p.name[i] = tolower(p.name[i]);
      if (p.name[9] != '~' && strncmp((char*)p.name, autoname, 5) == 0) {
        openAndPrintFile(autoname);
        autostart_index++;
        return;
      }
    }
  }
  autostart_index = -1;
}

void CardReader::beginautostart() {
  autostart_index = 0;
  cdroot();
}

void CardReader::closefile(const bool store_location) {
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
 * On exit, curDir contains an SdFile reference to the file's directory.
 *
 * Returns a pointer to the last segment (filename) of the given DOS 8.3 path.
 *
 * A nullptr result indicates an unrecoverable error.
 */
const char* CardReader::diveToFile(const bool update_cwd, SdFile*& curDir, const char * const path, const bool echo/*=false*/) {
  // Track both parent and subfolder
  static SdFile newDir1, newDir2;
  SdFile *sub = &newDir1, *startDir;

  // Parsing the path string
  const char *item_name_adr = path;

  if (path[0] == '/') {               // Starting at the root directory?
    curDir = &root;
    if (update_cwd) workDirDepth = 0; // The cwd can be updated for the benefit of sub-programs
    item_name_adr++;
  }
  else
    curDir = &workDir;                // Dive from workDir (as set by the UI)

  startDir = curDir;
  while (item_name_adr) {
    // Find next subdirectory delimiter
    char * const name_end = strchr(item_name_adr, '/');

    // Last atom in the path? Item found.
    if (name_end <= item_name_adr) break;

    // Set subDirName
    const uint8_t len = name_end - item_name_adr;
    char dosSubdirname[len + 1];
    strncpy(dosSubdirname, item_name_adr, len);
    dosSubdirname[len] = 0;

    if (echo) SERIAL_ECHOLN(dosSubdirname);

    // Open curDir
    if (!sub->open(curDir, dosSubdirname, O_READ)) {
      SERIAL_ECHOLNPAIR(STR_SD_OPEN_FILE_FAIL, dosSubdirname, ".");
      return nullptr;
    }

    // Close curDir if not at starting-point
    if (curDir != startDir) curDir->close();

    // curDir now subDir
    curDir = sub;

    // Update workDirParents, workDirDepth, and workDir
    if (update_cwd) {
      if (workDirDepth < MAX_DIR_DEPTH) workDirParents[workDirDepth++] = *curDir;
      workDir = *curDir;
    }

    // Point sub at the other scratch object
    sub = (curDir != &newDir1) ? &newDir1 : &newDir2;

    // Next path atom address
    item_name_adr = name_end + 1;
  }
  return item_name_adr;
}

void CardReader::cd(const char * relpath) {
  SdFile newDir;
  SdFile *parent = workDir.isOpen() ? &workDir : &root;

  if (newDir.open(parent, relpath, O_READ)) {
    workDir = newDir;
    flag.workDirIsRoot = false;
    if (workDirDepth < MAX_DIR_DEPTH)
      workDirParents[workDirDepth++] = workDir;
    TERN_(SDCARD_SORT_ALPHA, presort());
  }
  else {
    SERIAL_ECHO_START();
    SERIAL_ECHOLNPAIR(STR_SD_CANT_ENTER_SUBDIR, relpath);
  }
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
          sort_order[i] = SD_ORDER(i, fileCnt);
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
              if (flag.filenameIsDir) isDir[ind] |= _BV(bit);
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
            TERN_(HAS_FOLDER_SORTING, bool dir1 = flag.filenameIsDir);
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
                #define _SORT_CMP_DIR(fs) IS_DIR(o1) == IS_DIR(o2) ? _SORT_CMP_NODIR() : IS_DIR(fs > 0 ? o1 : o2)
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
  return
    #if ENABLED(SDCARD_SORT_ALPHA) && SDSORT_USES_RAM && SDSORT_CACHE_NAMES
      nrFiles // no need to access the SD card for filenames
    #else
      countFilesInWorkDir()
    #endif
  ;
}

//
// Return from procedure or close out the Print Job
//
void CardReader::fileHasFinished() {
  planner.synchronize();
  file.close();
  if (file_subcall_ctr > 0) { // Resume calling file after closing procedure
    file_subcall_ctr--;
    openFileRead(proc_filenames[file_subcall_ctr], 2); // 2 = Returning from sub-procedure
    setIndex(filespos[file_subcall_ctr]);
    startFileprint();
  }
  else {
    endFilePrint();

    TERN_(SDCARD_SORT_ALPHA, presort());

    marlin_state = MF_SD_COMPLETE;
  }
}

#if ENABLED(AUTO_REPORT_SD_STATUS)
  uint8_t CardReader::auto_report_sd_interval = 0;
  millis_t CardReader::next_sd_report_ms;
  #if HAS_MULTI_SERIAL
    int8_t CardReader::auto_report_port;
  #endif

  void CardReader::auto_report_sd_status() {
    millis_t current_ms = millis();
    if (auto_report_sd_interval && ELAPSED(current_ms, next_sd_report_ms)) {
      next_sd_report_ms = current_ms + 1000UL * auto_report_sd_interval;
      PORT_REDIRECT(auto_report_port);
      report_status();
    }
  }
#endif // AUTO_REPORT_SD_STATUS

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
      SERIAL_ECHOLNPAIR(STR_SD_OPEN_FILE_FAIL, recovery.filename, ".");
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
        serialprintPGM(jobRecoverFileExists() ? PSTR(" failed.\n") : PSTR("d.\n"));
      #endif
    }
  }

#endif // POWER_LOSS_RECOVERY

#endif // SDSUPPORT
