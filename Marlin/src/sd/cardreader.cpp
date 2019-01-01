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

#include "../inc/MarlinConfig.h"

#if ENABLED(SDSUPPORT)

#include "cardreader.h"

#include "../Marlin.h"
#include "../lcd/ultralcd.h"
#include "../module/planner.h"
#include "../module/printcounter.h"
#include "../core/language.h"
#include "../gcode/queue.h"

#if ENABLED(EMERGENCY_PARSER)
  #include "../feature/emergency_parser.h"
#endif

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../feature/power_loss_recovery.h"
#endif

#if ENABLED(ADVANCED_PAUSE_FEATURE)
  #include "../feature/pause.h"
#endif

// public:

card_flags_t CardReader::flag;
char CardReader::filename[FILENAME_LENGTH], CardReader::longFilename[LONG_FILENAME_LENGTH];
int8_t CardReader::autostart_index;

#if ENABLED(FAST_FILE_TRANSFER)
  #if NUM_SERIAL > 1
    uint8_t CardReader::transfer_port;
  #endif
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
      #if ENABLED(SDSORT_DYNAMIC_RAM)
        char **CardReader::sortshort, **CardReader::sortnames;
      #else
        char CardReader::sortshort[SDSORT_LIMIT][FILENAME_LENGTH];
        char CardReader::sortnames[SDSORT_LIMIT][SORTED_LONGNAME_MAXLEN];
      #endif
    #elif DISABLED(SDSORT_USES_STACK)
      char CardReader::sortnames[SDSORT_LIMIT][SORTED_LONGNAME_MAXLEN];
    #endif

    #if HAS_FOLDER_SORTING
      #if ENABLED(SDSORT_DYNAMIC_RAM)
        uint8_t *CardReader::isDir;
      #elif ENABLED(SDSORT_CACHE_NAMES) || DISABLED(SDSORT_USES_STACK)
        uint8_t CardReader::isDir[(SDSORT_LIMIT+7)>>3];
      #endif
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

LsAction CardReader::lsAction; //stored for recursion.
uint16_t CardReader::nrFiles; //counter for the files in the current directory and recycled as position counter for getting the nrFiles'th name in the directory.
char *CardReader::diveDirName;

CardReader::CardReader() {
  #if ENABLED(SDCARD_SORT_ALPHA)
    sort_count = 0;
    #if ENABLED(SDSORT_GCODE)
      sort_alpha = true;
      sort_folders = FOLDER_SORTING;
      //sort_reverse = false;
    #endif
  #endif
  flag.sdprinting = flag.cardOK = flag.saving = flag.logging = false;
  filesize = sdpos = 0;
  file_subcall_ctr = 0;

  workDirDepth = 0;
  ZERO(workDirParents);

  // Disable autostart until card is initialized
  autostart_index = -1;

  //power to SD reader
  #if SDPOWER > -1
    OUT_WRITE(SDPOWER, HIGH);
  #endif
}

char *createFilename(char *buffer, const dir_t &p) {
  char *pos = buffer;
  for (uint8_t i = 0; i < 11; i++) {
    if (p.name[i] == ' ') continue;
    if (i == 8) *pos++ = '.';
    *pos++ = p.name[i];
  }
  *pos++ = 0;
  return buffer;
}

/**
 * Dive into a folder and recurse depth-first to perform a pre-set operation lsAction:
 *   LS_Count       - Add +1 to nrFiles for every file within the parent
 *   LS_GetFilename - Get the filename of the file indexed by nrFile_index
 *   LS_SerialPrint - Print the full path and size of each file to serial output
 */

uint16_t nrFile_index;

void CardReader::lsDive(const char *prepend, SdFile parent, const char * const match/*=NULL*/
  #if NUM_SERIAL > 1
    , const int8_t port/*= -1*/
  #endif
) {
  dir_t p;
  uint8_t cnt = 0;

  // Read the next entry from a directory
  while (parent.readDir(&p, longFilename) > 0) {

    // If the entry is a directory and the action is LS_SerialPrint
    if (DIR_IS_SUBDIR(&p) && lsAction != LS_Count && lsAction != LS_GetFilename) {

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
      SdFile dir;
      if (!dir.open(&parent, dosFilename, O_READ)) {
        if (lsAction == LS_SerialPrint) {
          SERIAL_ECHO_START_P(port);
          SERIAL_ECHOPGM_P(port, MSG_SD_CANT_OPEN_SUBDIR);
          SERIAL_ECHOLN_P(port, dosFilename);
        }
      }
      lsDive(path, dir
        #if NUM_SERIAL > 1
          , NULL, port
        #endif
      );
      // close() is done automatically by destructor of SdFile
    }
    else {
      uint8_t pn0 = p.name[0];
      if (pn0 == DIR_NAME_FREE) break;
      if (pn0 == DIR_NAME_DELETED || pn0 == '.') continue;
      if (longFilename[0] == '.') continue;

      if (!DIR_IS_FILE_OR_SUBDIR(&p) || (p.attributes & DIR_ATT_HIDDEN)) continue;

      flag.filenameIsDir = DIR_IS_SUBDIR(&p);

      if (!flag.filenameIsDir && (p.name[8] != 'G' || p.name[9] == '~')) continue;

      switch (lsAction) {  // 1 based file count
        case LS_Count:
          nrFiles++;
          break;

        case LS_SerialPrint:
          createFilename(filename, p);
          if (prepend) SERIAL_ECHO_P(port, prepend);
          SERIAL_ECHO_P(port, filename);
          SERIAL_CHAR_P(port, ' ');
          SERIAL_ECHOLN_P(port, p.fileSize);
          break;

        case LS_GetFilename:
          createFilename(filename, p);
          if (match != NULL) {
            if (strcasecmp(match, filename) == 0) return;
          }
          else if (cnt == nrFile_index) return;  // 0 based index
          cnt++;
          break;
      }

    }
  } // while readDir
}

void CardReader::ls(
  #if NUM_SERIAL > 1
    const int8_t port
  #endif
) {
  lsAction = LS_SerialPrint;
  root.rewind();
  lsDive(NULL, root
    #if NUM_SERIAL > 1
      , NULL, port
    #endif
  );
}

#if ENABLED(LONG_FILENAME_HOST_SUPPORT)

  /**
   * Get a long pretty path based on a DOS 8.3 path
   */
  void CardReader::printLongPath(char *path
    #if NUM_SERIAL > 1
      , const int8_t port/*= -1*/
    #endif
  ) {
    lsAction = LS_GetFilename;

    int i, pathLen = strlen(path);

    // SERIAL_ECHOPGM_P(port, "Full Path: "); SERIAL_ECHOLN_P(port, path);

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

      // SERIAL_ECHOPGM_P(port, "Looking for segment: "); SERIAL_ECHOLN_P(port, segment);

      // Find the item, setting the long filename
      diveDir.rewind();
      lsDive(NULL, diveDir, segment
        #if NUM_SERIAL > 1
          , port
        #endif
      );

      // Print /LongNamePart to serial output
      SERIAL_CHAR_P(port, '/');
      SERIAL_ECHO_P(port, longFilename[0] ? longFilename : "???");

      // If the filename was printed then that's it
      if (!flag.filenameIsDir) break;

      // SERIAL_ECHOPGM_P(port, "Opening dir: "); SERIAL_ECHOLN_P(port, segment);

      // Open the sub-item as the new dive parent
      SdFile dir;
      if (!dir.open(&diveDir, segment, O_READ)) {
        SERIAL_EOL_P(port);
        SERIAL_ECHO_START_P(port);
        SERIAL_ECHOPGM_P(port, MSG_SD_CANT_OPEN_SUBDIR);
        SERIAL_ECHO_P(port, segment);
        break;
      }

      diveDir.close();
      diveDir = dir;

    } // while i<pathLen

    SERIAL_EOL_P(port);
  }

#endif // LONG_FILENAME_HOST_SUPPORT

/**
 * Echo the DOS 8.3 filename (and long filename, if any)
 */
void CardReader::printFilename(
  #if NUM_SERIAL > 1
    const int8_t port/*= -1*/
  #endif
) {
  if (file.isOpen()) {
    char dosFilename[FILENAME_LENGTH];
    file.getFilename(dosFilename);
    SERIAL_ECHO_P(port, dosFilename);
    #if ENABLED(LONG_FILENAME_HOST_SUPPORT)
      getfilename(0, dosFilename);
      if (longFilename[0]) {
        SERIAL_ECHO_P(port, ' ');
        SERIAL_ECHO_P(port, longFilename);
      }
    #endif
  }
  else
    SERIAL_ECHOPGM_P(port, "(no file)");

  SERIAL_EOL_P(port);
}

void CardReader::initsd() {
  flag.cardOK = false;
  if (root.isOpen()) root.close();

  #ifndef SPI_SPEED
    #define SPI_SPEED SPI_FULL_SPEED
  #endif

  if (!sd2card.init(SPI_SPEED, SDSS)
    #if defined(LCD_SDSS) && (LCD_SDSS != SDSS)
      && !sd2card.init(SPI_SPEED, LCD_SDSS)
    #endif
  ) {
    //if (!sd2card.init(SPI_HALF_SPEED,SDSS))
    SERIAL_ECHO_MSG(MSG_SD_INIT_FAIL);
  }
  else if (!volume.init(&sd2card))
    SERIAL_ERROR_MSG(MSG_SD_VOL_INIT_FAIL);
  else if (!root.openRoot(&volume))
    SERIAL_ERROR_MSG(MSG_SD_OPENROOT_FAIL);
  else {
    flag.cardOK = true;
    SERIAL_ECHO_MSG(MSG_SD_CARD_OK);
  }
  setroot();

  ui.refresh();
}

void CardReader::release() {
  stopSDPrint();
  flag.cardOK = false;
}

void CardReader::openAndPrintFile(const char *name) {
  char cmd[4 + strlen(name) + 1]; // Room for "M23 ", filename, and null
  sprintf_P(cmd, PSTR("M23 %s"), name);
  for (char *c = &cmd[4]; *c; c++) *c = tolower(*c);
  enqueue_and_echo_command_now(cmd);
  enqueue_and_echo_commands_P(PSTR("M24"));
}

void CardReader::startFileprint() {
  if (flag.cardOK) {
    flag.sdprinting = true;
    #if SD_RESORT
      flush_presort();
    #endif
  }
}

void CardReader::stopSDPrint(
  #if SD_RESORT
    const bool re_sort/*=false*/
  #endif
) {
  #if ENABLED(ADVANCED_PAUSE_FEATURE)
    did_pause_print = 0;
  #endif
  flag.sdprinting = flag.abort_sd_printing = false;
  if (isFileOpen()) file.close();
  #if SD_RESORT
    if (re_sort) presort();
  #endif
}

void CardReader::openLogFile(char * const path) {
  flag.logging = true;
  openFile(path, false);
}

void appendAtom(SdFile &file, char *& dst, uint8_t &cnt) {
  file.getFilename(dst);
  while (*dst && cnt < MAXPATHNAMELENGTH) { dst++; cnt++; }
  if (cnt < MAXPATHNAMELENGTH) { *dst = '/'; dst++; cnt++; }
}

void CardReader::getAbsFilename(char *t) {
  *t++ = '/';                                               // Root folder
  uint8_t cnt = 1;

  for (uint8_t i = 0; i < workDirDepth; i++)                // Loop to current work dir
    appendAtom(workDirParents[i], t, cnt);

  if (cnt < MAXPATHNAMELENGTH - (FILENAME_LENGTH) - 1) {    // Leave room for filename and nul
    appendAtom(file, t, cnt);
    --t;
  }
  *t = '\0';
}

void CardReader::openFile(char * const path, const bool read, const bool subcall/*=false*/) {

  if (!flag.cardOK) return;

  uint8_t doing = 0;
  if (isFileOpen()) {                     // Replacing current file or doing a subroutine
    if (subcall) {
      if (file_subcall_ctr > SD_PROCEDURE_DEPTH - 1) {
        SERIAL_ERROR_MSG("trying to call sub-gcode files with too many levels. MAX level is:" STRINGIFY(SD_PROCEDURE_DEPTH));
        kill();
        return;
      }

      // Store current filename (based on workDirParents) and position
      getAbsFilename(proc_filenames[file_subcall_ctr]);
      filespos[file_subcall_ctr] = sdpos;

      SERIAL_ECHO_START();
      SERIAL_ECHOPAIR("SUBROUTINE CALL target:\"", path);
      SERIAL_ECHOPAIR("\" parent:\"", proc_filenames[file_subcall_ctr]);
      SERIAL_ECHOLNPAIR("\" pos", sdpos);
      file_subcall_ctr++;
    }
    else
      doing = 1;
  }
  else if (subcall)       // Returning from a subcall?
    SERIAL_ECHO_MSG("END SUBROUTINE");
  else {                  // Opening fresh file
    doing = 2;
    file_subcall_ctr = 0; // Reset procedure depth in case user cancels print while in procedure
  }

  if (doing) {
    SERIAL_ECHO_START();
    SERIAL_ECHOPGM("Now ");
    serialprintPGM(doing == 1 ? PSTR("doing") : PSTR("fresh"));
    SERIAL_ECHOLNPAIR(" file: ", path);
  }

  stopSDPrint();

  SdFile *curDir;
  const char * const fname = diveToFile(curDir, path, false);
  if (!fname) return;

  if (read) {
    if (file.open(curDir, fname, O_READ)) {
      filesize = file.fileSize();
      sdpos = 0;
      SERIAL_ECHOPAIR(MSG_SD_FILE_OPENED, fname);
      SERIAL_ECHOLNPAIR(MSG_SD_SIZE, filesize);
      SERIAL_ECHOLNPGM(MSG_SD_FILE_SELECTED);

      getfilename(0, fname);
      ui.set_status(longFilename[0] ? longFilename : fname);
      //if (longFilename[0]) {
      //  SERIAL_ECHOPAIR(MSG_SD_FILE_LONG_NAME, longFilename);
      //}
    }
    else {
      SERIAL_ECHOPAIR(MSG_SD_OPEN_FILE_FAIL, fname);
      SERIAL_CHAR('.');
      SERIAL_EOL();
    }
  }
  else { //write
    if (!file.open(curDir, fname, O_CREAT | O_APPEND | O_WRITE | O_TRUNC)) {
      SERIAL_ECHOPAIR(MSG_SD_OPEN_FILE_FAIL, fname);
      SERIAL_CHAR('.');
      SERIAL_EOL();
    }
    else {
      flag.saving = true;
      getfilename(0, fname);
      #if ENABLED(EMERGENCY_PARSER)
        emergency_parser.disable();
      #endif
      SERIAL_ECHOLNPAIR(MSG_SD_WRITE_TO_FILE, fname);
      ui.set_status(fname);
    }
  }
}

void CardReader::removeFile(const char * const name) {
  if (!flag.cardOK) return;

  //stopSDPrint();

  SdFile *curDir;
  const char * const fname = diveToFile(curDir, name, false);
  if (!fname) return;

  if (file.remove(curDir, fname)) {
    SERIAL_ECHOLNPAIR("File deleted:", fname);
    sdpos = 0;
    #if ENABLED(SDCARD_SORT_ALPHA)
      presort();
    #endif
  }
  else {
    SERIAL_ECHOPAIR("Deletion failed, File: ", fname);
    SERIAL_CHAR('.');
  }
}

void CardReader::getStatus(
  #if NUM_SERIAL > 1
    const int8_t port/*= -1*/
  #endif
) {
  if (flag.cardOK && flag.sdprinting) {
    SERIAL_ECHOPGM_P(port, MSG_SD_PRINTING_BYTE);
    SERIAL_ECHO_P(port, sdpos);
    SERIAL_CHAR_P(port, '/');
    SERIAL_ECHOLN_P(port, filesize);
  }
  else
    SERIAL_ECHOLNPGM_P(port, MSG_SD_NOT_PRINTING);
}

void CardReader::write_command(char *buf) {
  char* begin = buf;
  char* npos = NULL;
  char* end = buf + strlen(buf) - 1;

  file.writeError = false;
  if ((npos = strchr(buf, 'N')) != NULL) {
    begin = strchr(npos, ' ') + 1;
    end = strchr(npos, '*') - 1;
  }
  end[1] = '\r';
  end[2] = '\n';
  end[3] = '\0';
  file.write(begin);

  if (file.writeError) SERIAL_ERROR_MSG(MSG_SD_ERR_WRITE_TO_FILE);
}

//
// Run the next autostart file. Called:
// - On boot after successful card init
// - After finishing the previous autostart file
// - From the LCD command to run the autostart file
//

void CardReader::checkautostart() {

  if (autostart_index < 0 || flag.sdprinting) return;

  if (!flag.cardOK) initsd();

  if (flag.cardOK
    #if ENABLED(POWER_LOSS_RECOVERY)
      && !recovery.valid() // Don't run auto#.g when a resume file exists
    #endif
  ) {
    char autoname[8];
    sprintf_P(autoname, PSTR("auto%c.g"), autostart_index + '0');
    dir_t p;
    root.rewind();
    while (root.readDir(&p, NULL) > 0) {
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
  setroot();
}

void CardReader::closefile(const bool store_location) {
  file.sync();
  file.close();
  flag.saving = flag.logging = false;
  sdpos = 0;
  #if ENABLED(EMERGENCY_PARSER)
    emergency_parser.enable();
  #endif

  if (store_location) {
    //future: store printer state, filename and position for continuing a stopped print
    // so one can unplug the printer and continue printing the next day.
  }
}

/**
 * Get the name of a file in the current directory by index
 * with optional name to match.
 */
void CardReader::getfilename(uint16_t nr, const char * const match/*=NULL*/) {
  #if ENABLED(SDSORT_CACHE_NAMES)
    if (match != NULL) {
      while (nr < sort_count) {
        if (strcasecmp(match, sortshort[nr]) == 0) break;
        nr++;
      }
    }
    if (nr < sort_count) {
      strcpy(filename, sortshort[nr]);
      strcpy(longFilename, sortnames[nr]);
      flag.filenameIsDir = TEST(isDir[nr>>3], nr & 0x07);
      return;
    }
  #endif // SDSORT_CACHE_NAMES
  lsAction = LS_GetFilename;
  nrFile_index = nr;
  workDir.rewind();
  lsDive(NULL, workDir, match);
}

uint16_t CardReader::getnrfilenames() {
  lsAction = LS_Count;
  nrFiles = 0;
  workDir.rewind();
  lsDive(NULL, workDir);
  //SERIAL_ECHOLN(nrFiles);
  return nrFiles;
}

/**
 * Dive to the given DOS 8.3 file path, with optional echo of the dive paths.
 *
 * On exit, curDir contains an SdFile reference to the file's directory.
 *
 * Returns a pointer to the last segment (filename) of the given DOS 8.3 path.
 *
 * A NULL result indicates an unrecoverable error.
 */
const char* CardReader::diveToFile(SdFile*& curDir, const char * const path, const bool echo) {
  SdFile myDir;
  if (path[0] != '/') { curDir = &workDir; return path; }

  curDir = &root;
  const char *dirname_start = &path[1];
  while (dirname_start) {
    char * const dirname_end = strchr(dirname_start, '/');
    if (dirname_end <= dirname_start) break;
    const uint8_t len = dirname_end - dirname_start;
    char dosSubdirname[len + 1];
    strncpy(dosSubdirname, dirname_start, len);
    dosSubdirname[len] = 0;

    if (echo) SERIAL_ECHOLN(dosSubdirname);

    if (!myDir.open(curDir, dosSubdirname, O_READ)) {
      SERIAL_ECHOPAIR(MSG_SD_OPEN_FILE_FAIL, dosSubdirname);
      SERIAL_CHAR('.');
      SERIAL_EOL();
      return NULL;
    }
    curDir = &myDir;
    dirname_start = dirname_end + 1;
  }
  return dirname_start;
}

void CardReader::chdir(const char * relpath) {
  SdFile newDir;
  SdFile *parent = workDir.isOpen() ? &workDir : &root;

  if (newDir.open(parent, relpath, O_READ)) {
    workDir = newDir;
    if (workDirDepth < MAX_DIR_DEPTH)
      workDirParents[workDirDepth++] = workDir;
    #if ENABLED(SDCARD_SORT_ALPHA)
      presort();
    #endif
  }
  else {
    SERIAL_ECHO_START();
    SERIAL_ECHOLNPAIR(MSG_SD_CANT_ENTER_SUBDIR, relpath);
  }
}

int8_t CardReader::updir() {
  if (workDirDepth > 0) {                                               // At least 1 dir has been saved
    workDir = --workDirDepth ? workDirParents[workDirDepth - 1] : root; // Use parent, or root if none
    #if ENABLED(SDCARD_SORT_ALPHA)
      presort();
    #endif
  }
  return workDirDepth;
}

void CardReader::setroot() {
  /*if (!workDir.openRoot(&volume)) {
    SERIAL_ECHOLNPGM(MSG_SD_WORKDIR_FAIL);
  }*/
  workDir = root;
  #if ENABLED(SDCARD_SORT_ALPHA)
    presort();
  #endif
}

#if ENABLED(SDCARD_SORT_ALPHA)

  /**
   * Get the name of a file in the current directory by sort-index
   */
  void CardReader::getfilename_sorted(const uint16_t nr) {
    getfilename(
      #if ENABLED(SDSORT_GCODE)
        sort_alpha &&
      #endif
      (nr < sort_count) ? sort_order[nr] : nr
    );
  }

  #if ENABLED(SDSORT_USES_RAM)
    #if ENABLED(SDSORT_DYNAMIC_RAM)
      // Use dynamic method to copy long filename
      #define SET_SORTNAME(I) (sortnames[I] = strdup(longest_filename()))
      #if ENABLED(SDSORT_CACHE_NAMES)
        // When caching also store the short name, since
        // we're replacing the getfilename() behavior.
        #define SET_SORTSHORT(I) (sortshort[I] = strdup(filename))
      #else
        #define SET_SORTSHORT(I) NOOP
      #endif
    #else
      // Copy filenames into the static array
      #if SORTED_LONGNAME_MAXLEN != LONG_FILENAME_LENGTH
        #define SET_SORTNAME(I) do{ strncpy(sortnames[I], longest_filename(), SORTED_LONGNAME_MAXLEN); \
                                    sortnames[I][SORTED_LONGNAME_MAXLEN] = '\0'; }while(0)
      #else
        #define SET_SORTNAME(I) strncpy(sortnames[I], longest_filename(), SORTED_LONGNAME_MAXLEN)
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
    #if ENABLED(SDSORT_GCODE)
      if (!sort_alpha) return;
    #endif

    // If there are files, sort up to the limit
    uint16_t fileCnt = getnrfilenames();
    if (fileCnt > 0) {

      // Never sort more than the max allowed
      // If you use folders to organize, 20 may be enough
      if (fileCnt > SDSORT_LIMIT) fileCnt = SDSORT_LIMIT;

      // Sort order is always needed. May be static or dynamic.
      #if ENABLED(SDSORT_DYNAMIC_RAM)
        sort_order = new uint8_t[fileCnt];
      #endif

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
          char sortnames[fileCnt][SORTED_LONGNAME_MAXLEN];
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
            getfilename(i);
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
          for (uint16_t j = 0; j < i; ++j) {
            const uint16_t o1 = sort_order[j], o2 = sort_order[j + 1];

            // Compare names from the array or just the two buffered names
            #if ENABLED(SDSORT_USES_RAM)
              #define _SORT_CMP_NODIR() (strcasecmp(sortnames[o1], sortnames[o2]) > 0)
            #else
              #define _SORT_CMP_NODIR() (strcasecmp(name1, name2) > 0)
            #endif

            #if HAS_FOLDER_SORTING
              #if ENABLED(SDSORT_USES_RAM)
                // Folder sorting needs an index and bit to test for folder-ness.
                const uint8_t ind1 = o1 >> 3, bit1 = o1 & 0x07,
                              ind2 = o2 >> 3, bit2 = o2 & 0x07;
                #define _SORT_CMP_DIR(fs) \
                  (((isDir[ind1] & _BV(bit1)) != 0) == ((isDir[ind2] & _BV(bit2)) != 0) \
                    ? _SORT_CMP_NODIR() \
                    : (isDir[fs > 0 ? ind1 : ind2] & (fs > 0 ? _BV(bit1) : _BV(bit2))) != 0)
              #else
                #define _SORT_CMP_DIR(fs) ((dir1 == flag.filenameIsDir) ? _SORT_CMP_NODIR() : (fs > 0 ? dir1 : !dir1))
              #endif
            #endif

            // The most economical method reads names as-needed
            // throughout the loop. Slow if there are many.
            #if DISABLED(SDSORT_USES_RAM)
              getfilename(o1);
              strcpy(name1, longest_filename()); // save (or getfilename below will trounce it)
              #if HAS_FOLDER_SORTING
                bool dir1 = flag.filenameIsDir;
              #endif
              getfilename(o2);
              char *name2 = longest_filename(); // use the string in-place
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
              sort_order[j] = o2;
              sort_order[j + 1] = o1;
              didSwap = true;
            }
          }
          if (!didSwap) break;
        }
        // Using RAM but not keeping names around
        #if ENABLED(SDSORT_USES_RAM) && DISABLED(SDSORT_CACHE_NAMES)
          #if ENABLED(SDSORT_DYNAMIC_RAM)
            for (uint16_t i = 0; i < fileCnt; ++i) free(sortnames[i]);
            #if HAS_FOLDER_SORTING
              free(isDir);
            #endif
          #endif
        #endif
      }
      else {
        sort_order[0] = 0;
        #if ENABLED(SDSORT_USES_RAM) && ENABLED(SDSORT_CACHE_NAMES)
          #if ENABLED(SDSORT_DYNAMIC_RAM)
            sortnames = new char*[1];
            #if ENABLED(SDSORT_CACHE_NAMES)
              sortshort = new char*[1];
            #endif
            isDir = new uint8_t[1];
          #endif
          getfilename(0);
          SET_SORTNAME(0);
          SET_SORTSHORT(0);
          isDir[0] = flag.filenameIsDir ? 0x01 : 0x00;
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
          for (uint8_t i = 0; i < sort_count; ++i) {
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
      getnrfilenames()
    #endif
  ;
}

void CardReader::printingHasFinished() {
  planner.synchronize();
  file.close();
  if (file_subcall_ctr > 0) { // Heading up to a parent file that called current as a procedure.
    file_subcall_ctr--;
    openFile(proc_filenames[file_subcall_ctr], true, true);
    setIndex(filespos[file_subcall_ctr]);
    startFileprint();
  }
  else {
    stopSDPrint();

    #if ENABLED(POWER_LOSS_RECOVERY)
      removeJobRecoveryFile();
    #endif

    #if ENABLED(SD_FINISHED_STEPPERRELEASE) && defined(SD_FINISHED_RELEASECOMMAND)
      planner.finish_and_disable();
    #endif

    print_job_timer.stop();
    if (print_job_timer.duration() > 60) enqueue_and_echo_commands_P(PSTR("M31"));

    #if ENABLED(SDCARD_SORT_ALPHA)
      presort();
    #endif

    #if (ENABLED(ULTRA_LCD) || ENABLED(EXTENSIBLE_UI)) && ENABLED(LCD_SET_PROGRESS_MANUALLY)
      ui.progress_bar_percent = 0;
    #endif

    ui.reset_status();

    #if ENABLED(SD_REPRINT_LAST_SELECTED_FILE)
      ui.reselect_last_file();
    #endif
  }
}

#if ENABLED(AUTO_REPORT_SD_STATUS)
  uint8_t CardReader::auto_report_sd_interval = 0;
  millis_t CardReader::next_sd_report_ms;
  #if NUM_SERIAL > 1
    int8_t CardReader::serialport;
  #endif

  void CardReader::auto_report_sd_status() {
    millis_t current_ms = millis();
    if (auto_report_sd_interval && ELAPSED(current_ms, next_sd_report_ms)) {
      next_sd_report_ms = current_ms + 1000UL * auto_report_sd_interval;
      getStatus(
        #if NUM_SERIAL > 1
          serialport
        #endif
      );
    }
  }
#endif // AUTO_REPORT_SD_STATUS

#if ENABLED(POWER_LOSS_RECOVERY)

  constexpr char job_recovery_file_name[4] = "BIN";

  bool CardReader::jobRecoverFileExists() {
    const bool exists = recovery.file.open(&root, job_recovery_file_name, O_READ);
    if (exists) recovery.file.close();
    return exists;
  }

  void CardReader::openJobRecoveryFile(const bool read) {
    if (!flag.cardOK) return;
    if (recovery.file.isOpen()) return;
    if (!recovery.file.open(&root, job_recovery_file_name, read ? O_READ : O_CREAT | O_WRITE | O_TRUNC | O_SYNC)) {
      SERIAL_ECHOPAIR(MSG_SD_OPEN_FILE_FAIL, job_recovery_file_name);
      SERIAL_CHAR('.');
      SERIAL_EOL();
    }
    else if (!read)
      SERIAL_ECHOLNPAIR(MSG_SD_WRITE_TO_FILE, job_recovery_file_name);
  }

  // Removing the job recovery file currently requires closing
  // the file being printed, so during SD printing the file should
  // be zeroed and written instead of deleted.
  void CardReader::removeJobRecoveryFile() {
    if (jobRecoverFileExists()) {
      //closefile();
      removeFile(job_recovery_file_name);
      #if ENABLED(DEBUG_POWER_LOSS_RECOVERY)
        SERIAL_ECHOPGM("Power-loss file delete");
        serialprintPGM(jobRecoverFileExists() ? PSTR(" failed.\n") : PSTR("d.\n"));
      #endif
    }
  }

#endif // POWER_LOSS_RECOVERY

#endif // SDSUPPORT
