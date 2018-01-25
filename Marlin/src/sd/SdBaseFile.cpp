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

/**
 * Arduino SdFat Library
 * Copyright (C) 2009 by William Greiman
 *
 * This file is part of the Arduino Sd2Card Library
 */

#include "../inc/MarlinConfig.h"

#if ENABLED(SDSUPPORT)

#include "SdBaseFile.h"

#include "../Marlin.h"
SdBaseFile* SdBaseFile::cwd_ = 0;   // Pointer to Current Working Directory

// callback function for date/time
void (*SdBaseFile::dateTime_)(uint16_t* date, uint16_t* time) = 0;

// add a cluster to a file
bool SdBaseFile::addCluster() {
  if (!vol_->allocContiguous(1, &curCluster_)) return false;

  // if first cluster of file link to directory entry
  if (firstCluster_ == 0) {
    firstCluster_ = curCluster_;
    flags_ |= F_FILE_DIR_DIRTY;
  }
  return true;
}

// Add a cluster to a directory file and zero the cluster.
// return with first block of cluster in the cache
bool SdBaseFile::addDirCluster() {
  uint32_t block;
  // max folder size
  if (fileSize_ / sizeof(dir_t) >= 0xFFFF) return false;

  if (!addCluster()) return false;
  if (!vol_->cacheFlush()) return false;

  block = vol_->clusterStartBlock(curCluster_);

  // set cache to first block of cluster
  vol_->cacheSetBlockNumber(block, true);

  // zero first block of cluster
  memset(vol_->cacheBuffer_.data, 0, 512);

  // zero rest of cluster
  for (uint8_t i = 1; i < vol_->blocksPerCluster_; i++) {
    if (!vol_->writeBlock(block + i, vol_->cacheBuffer_.data)) return false;
  }
  // Increase directory file size by cluster size
  fileSize_ += 512UL << vol_->clusterSizeShift_;
  return true;
}

// cache a file's directory entry
// return pointer to cached entry or null for failure
dir_t* SdBaseFile::cacheDirEntry(uint8_t action) {
  if (!vol_->cacheRawBlock(dirBlock_, action)) return NULL;
  return vol_->cache()->dir + dirIndex_;
}

/**
 * Close a file and force cached data and directory information
 *  to be written to the storage device.
 *
 * \return true for success, false for failure.
 * Reasons for failure include no file is open or an I/O error.
 */
bool SdBaseFile::close() {
  bool rtn = sync();
  type_ = FAT_FILE_TYPE_CLOSED;
  return rtn;
}

/**
 * Check for contiguous file and return its raw block range.
 *
 * \param[out] bgnBlock the first block address for the file.
 * \param[out] endBlock the last  block address for the file.
 *
 * \return true for success, false for failure.
 * Reasons for failure include file is not contiguous, file has zero length
 * or an I/O error occurred.
 */
bool SdBaseFile::contiguousRange(uint32_t* bgnBlock, uint32_t* endBlock) {
  // error if no blocks
  if (firstCluster_ == 0) return false;

  for (uint32_t c = firstCluster_; ; c++) {
    uint32_t next;
    if (!vol_->fatGet(c, &next)) return false;

    // check for contiguous
    if (next != (c + 1)) {
      // error if not end of chain
      if (!vol_->isEOC(next)) return false;
      *bgnBlock = vol_->clusterStartBlock(firstCluster_);
      *endBlock = vol_->clusterStartBlock(c)
                  + vol_->blocksPerCluster_ - 1;
      return true;
    }
  }
  return false;
}

/**
 * Create and open a new contiguous file of a specified size.
 *
 * \note This function only supports short DOS 8.3 names.
 * See open() for more information.
 *
 * \param[in] dirFile The directory where the file will be created.
 * \param[in] path A path with a valid DOS 8.3 file name.
 * \param[in] size The desired file size.
 *
 * \return true for success, false for failure.
 * Reasons for failure include \a path contains
 * an invalid DOS 8.3 file name, the FAT volume has not been initialized,
 * a file is already open, the file already exists, the root
 * directory is full or an I/O error.
 *
 */
bool SdBaseFile::createContiguous(SdBaseFile* dirFile, const char* path, uint32_t size) {
  uint32_t count;
  // don't allow zero length file
  if (size == 0) return false;
  if (!open(dirFile, path, O_CREAT | O_EXCL | O_RDWR)) return false;

  // calculate number of clusters needed
  count = ((size - 1) >> (vol_->clusterSizeShift_ + 9)) + 1;

  // allocate clusters
  if (!vol_->allocContiguous(count, &firstCluster_)) {
    remove();
    return false;
  }
  fileSize_ = size;

  // insure sync() will update dir entry
  flags_ |= F_FILE_DIR_DIRTY;

  return sync();
}

/**
 * Return a file's directory entry.
 *
 * \param[out] dir Location for return of the file's directory entry.
 *
 * \return true for success, false for failure.
 */
bool SdBaseFile::dirEntry(dir_t* dir) {
  dir_t* p;
  // make sure fields on SD are correct
  if (!sync()) return false;

  // read entry
  p = cacheDirEntry(SdVolume::CACHE_FOR_READ);
  if (!p) return false;

  // copy to caller's struct
  memcpy(dir, p, sizeof(dir_t));
  return true;
}

/**
 * Format the name field of \a dir into the 13 byte array
 * \a name in standard 8.3 short name format.
 *
 * \param[in] dir The directory structure containing the name.
 * \param[out] name A 13 byte char array for the formatted name.
 */
void SdBaseFile::dirName(const dir_t& dir, char* name) {
  uint8_t j = 0;
  for (uint8_t i = 0; i < 11; i++) {
    if (dir.name[i] == ' ')continue;
    if (i == 8) name[j++] = '.';
    name[j++] = dir.name[i];
  }
  name[j] = 0;
}

/**
 * Test for the existence of a file in a directory
 *
 * \param[in] name Name of the file to be tested for.
 *
 * The calling instance must be an open directory file.
 *
 * dirFile.exists("TOFIND.TXT") searches for "TOFIND.TXT" in  the directory
 * dirFile.
 *
 * \return true if the file exists else false.
 */
bool SdBaseFile::exists(const char* name) {
  SdBaseFile file;
  return file.open(this, name, O_READ);
}

/**
 * Get a string from a file.
 *
 * fgets() reads bytes from a file into the array pointed to by \a str, until
 * \a num - 1 bytes are read, or a delimiter is read and transferred to \a str,
 * or end-of-file is encountered. The string is then terminated
 * with a null byte.
 *
 * fgets() deletes CR, '\\r', from the string.  This insures only a '\\n'
 * terminates the string for Windows text files which use CRLF for newline.
 *
 * \param[out] str Pointer to the array where the string is stored.
 * \param[in] num Maximum number of characters to be read
 * (including the final null byte). Usually the length
 * of the array \a str is used.
 * \param[in] delim Optional set of delimiters. The default is "\n".
 *
 * \return For success fgets() returns the length of the string in \a str.
 * If no data is read, fgets() returns zero for EOF or -1 if an error occurred.
 **/
int16_t SdBaseFile::fgets(char* str, int16_t num, char* delim) {
  char ch;
  int16_t n = 0;
  int16_t r = -1;
  while ((n + 1) < num && (r = read(&ch, 1)) == 1) {
    // delete CR
    if (ch == '\r') continue;
    str[n++] = ch;
    if (!delim) {
      if (ch == '\n') break;
    }
    else {
      if (strchr(delim, ch)) break;
    }
  }
  if (r < 0) {
    // read error
    return -1;
  }
  str[n] = '\0';
  return n;
}

/**
 * Get a file's name
 *
 * \param[out] name An array of 13 characters for the file's name.
 *
 * \return true for success, false for failure.
 */
bool SdBaseFile::getFilename(char * const name) {
  if (!isOpen()) return false;

  if (isRoot()) {
    name[0] = '/';
    name[1] = '\0';
    return true;
  }
  // cache entry
  dir_t* p = cacheDirEntry(SdVolume::CACHE_FOR_READ);
  if (!p) return false;

  // format name
  dirName(*p, name);
  return true;
}

void SdBaseFile::getpos(filepos_t* pos) {
  pos->position = curPosition_;
  pos->cluster = curCluster_;
}

/**
 * List directory contents.
 *
 * \param[in] pr Print stream for list.
 *
 * \param[in] flags The inclusive OR of
 *
 * LS_DATE - %Print file modification date
 *
 * LS_SIZE - %Print file size.
 *
 * LS_R - Recursive list of subdirectories.
 *
 * \param[in] indent Amount of space before file name. Used for recursive
 * list to indicate subdirectory level.
 */
void SdBaseFile::ls(uint8_t flags, uint8_t indent) {
  rewind();
  int8_t status;
  while ((status = lsPrintNext(flags, indent))) {
    if (status > 1 && (flags & LS_R)) {
      uint16_t index = curPosition() / 32 - 1;
      SdBaseFile s;
      if (s.open(this, index, O_READ)) s.ls(flags, indent + 2);
      seekSet(32 * (index + 1));
    }
  }
}

// saves 32 bytes on stack for ls recursion
// return 0 - EOF, 1 - normal file, or 2 - directory
int8_t SdBaseFile::lsPrintNext(uint8_t flags, uint8_t indent) {
  dir_t dir;
  uint8_t w = 0;

  while (1) {
    if (read(&dir, sizeof(dir)) != sizeof(dir)) return 0;
    if (dir.name[0] == DIR_NAME_FREE) return 0;

    // skip deleted entry and entries for . and  ..
    if (dir.name[0] != DIR_NAME_DELETED && dir.name[0] != '.'
        && DIR_IS_FILE_OR_SUBDIR(&dir)) break;
  }
  // indent for dir level
  for (uint8_t i = 0; i < indent; i++) SERIAL_CHAR(' ');

  // print name
  for (uint8_t i = 0; i < 11; i++) {
    if (dir.name[i] == ' ')continue;
    if (i == 8) {
      SERIAL_CHAR('.');
      w++;
    }
    SERIAL_CHAR(dir.name[i]);
    w++;
  }
  if (DIR_IS_SUBDIR(&dir)) {
    SERIAL_CHAR('/');
    w++;
  }
  if (flags & (LS_DATE | LS_SIZE)) {
    while (w++ < 14) SERIAL_CHAR(' ');
  }
  // print modify date/time if requested
  if (flags & LS_DATE) {
    SERIAL_CHAR(' ');
    printFatDate(dir.lastWriteDate);
    SERIAL_CHAR(' ');
    printFatTime(dir.lastWriteTime);
  }
  // print size if requested
  if (!DIR_IS_SUBDIR(&dir) && (flags & LS_SIZE)) {
    SERIAL_CHAR(' ');
    SERIAL_ECHO(dir.fileSize);
  }
  SERIAL_EOL();
  return DIR_IS_FILE(&dir) ? 1 : 2;
}

// Format directory name field from a 8.3 name string
bool SdBaseFile::make83Name(const char* str, uint8_t* name, const char** ptr) {
  uint8_t n = 7,                      // Max index until a dot is found
          i = 11;
  while (i) name[--i] = ' ';          // Set whole FILENAME.EXT to spaces
  while (*str && *str != '/') {       // For each character, until nul or '/'
    uint8_t c = *str++;               // Get char and advance
    if (c == '.') {                   // For a dot...
      if (n == 10) return false;      // Already moved the max index? fail!
      n = 10;                         // Move the max index for full 8.3 name
      i = 8;                          // Move up to the extension place
    }
    else {
      // Fail for illegal characters
      PGM_P p = PSTR("|<>^+=?/[];,*\"\\");
      while (uint8_t b = pgm_read_byte(p++)) if (b == c) return false;
      if (i > n || c < 0x21 || c == 0x7F) return false;           // Check size, non-printable characters
      name[i++] = (c < 'a' || c > 'z') ? (c) : (c + ('A' - 'a')); // Uppercase required for 8.3 name
    }
  }
  *ptr = str;                         // Set passed pointer to the end
  return name[0] != ' ';              // Return true if any name was set
}

/**
 * Make a new directory.
 *
 * \param[in] parent An open SdFat instance for the directory that will contain
 * the new directory.
 *
 * \param[in] path A path with a valid 8.3 DOS name for the new directory.
 *
 * \param[in] pFlag Create missing parent directories if true.
 *
 * \return true for success, false for failure.
 * Reasons for failure include this file is already open, \a parent is not a
 * directory, \a path is invalid or already exists in \a parent.
 */
bool SdBaseFile::mkdir(SdBaseFile* parent, const char* path, bool pFlag) {
  uint8_t dname[11];
  SdBaseFile dir1, dir2;
  SdBaseFile* sub = &dir1;
  SdBaseFile* start = parent;

  if (!parent || isOpen()) return false;

  if (*path == '/') {
    while (*path == '/') path++;
    if (!parent->isRoot()) {
      if (!dir2.openRoot(parent->vol_)) return false;
      parent = &dir2;
    }
  }
  while (1) {
    if (!make83Name(path, dname, &path)) return false;
    while (*path == '/') path++;
    if (!*path) break;
    if (!sub->open(parent, dname, O_READ)) {
      if (!pFlag || !sub->mkdir(parent, dname))
        return false;
    }
    if (parent != start) parent->close();
    parent = sub;
    sub = parent != &dir1 ? &dir1 : &dir2;
  }
  return mkdir(parent, dname);
}

bool SdBaseFile::mkdir(SdBaseFile* parent, const uint8_t dname[11]) {
  uint32_t block;
  dir_t d;
  dir_t* p;

  if (!parent->isDir()) return false;

  // create a normal file
  if (!open(parent, dname, O_CREAT | O_EXCL | O_RDWR)) return false;

  // convert file to directory
  flags_ = O_READ;
  type_ = FAT_FILE_TYPE_SUBDIR;

  // allocate and zero first cluster
  if (!addDirCluster()) return false;

  // force entry to SD
  if (!sync()) return false;

  // cache entry - should already be in cache due to sync() call
  p = cacheDirEntry(SdVolume::CACHE_FOR_WRITE);
  if (!p) return false;

  // change directory entry  attribute
  p->attributes = DIR_ATT_DIRECTORY;

  // make entry for '.'
  memcpy(&d, p, sizeof(d));
  d.name[0] = '.';
  for (uint8_t i = 1; i < 11; i++) d.name[i] = ' ';

  // cache block for '.'  and '..'
  block = vol_->clusterStartBlock(firstCluster_);
  if (!vol_->cacheRawBlock(block, SdVolume::CACHE_FOR_WRITE)) return false;

  // copy '.' to block
  memcpy(&vol_->cache()->dir[0], &d, sizeof(d));

  // make entry for '..'
  d.name[1] = '.';
  if (parent->isRoot()) {
    d.firstClusterLow = 0;
    d.firstClusterHigh = 0;
  }
  else {
    d.firstClusterLow = parent->firstCluster_ & 0xFFFF;
    d.firstClusterHigh = parent->firstCluster_ >> 16;
  }
  // copy '..' to block
  memcpy(&vol_->cache()->dir[1], &d, sizeof(d));

  // write first block
  return vol_->cacheFlush();
}

/**
 * Open a file in the current working directory.
 *
 * \param[in] path A path with a valid 8.3 DOS name for a file to be opened.
 *
 * \param[in] oflag Values for \a oflag are constructed by a bitwise-inclusive
 * OR of open flags. see SdBaseFile::open(SdBaseFile*, const char*, uint8_t).
 *
 * \return true for success, false for failure.
 */
bool SdBaseFile::open(const char* path, uint8_t oflag) {
  return open(cwd_, path, oflag);
}

/**
 * Open a file or directory by name.
 *
 * \param[in] dirFile An open SdFat instance for the directory containing the
 * file to be opened.
 *
 * \param[in] path A path with a valid 8.3 DOS name for a file to be opened.
 *
 * \param[in] oflag Values for \a oflag are constructed by a bitwise-inclusive
 * OR of flags from the following list
 *
 * O_READ - Open for reading.
 *
 * O_RDONLY - Same as O_READ.
 *
 * O_WRITE - Open for writing.
 *
 * O_WRONLY - Same as O_WRITE.
 *
 * O_RDWR - Open for reading and writing.
 *
 * O_APPEND - If set, the file offset shall be set to the end of the
 * file prior to each write.
 *
 * O_AT_END - Set the initial position at the end of the file.
 *
 * O_CREAT - If the file exists, this flag has no effect except as noted
 * under O_EXCL below. Otherwise, the file shall be created
 *
 * O_EXCL - If O_CREAT and O_EXCL are set, open() shall fail if the file exists.
 *
 * O_SYNC - Call sync() after each write.  This flag should not be used with
 * write(uint8_t), write_P(PGM_P), writeln_P(PGM_P), or the Arduino Print class.
 * These functions do character at a time writes so sync() will be called
 * after each byte.
 *
 * O_TRUNC - If the file exists and is a regular file, and the file is
 * successfully opened and is not read only, its length shall be truncated to 0.
 *
 * WARNING: A given file must not be opened by more than one SdBaseFile object
 * of file corruption may occur.
 *
 * \note Directory files must be opened read only.  Write and truncation is
 * not allowed for directory files.
 *
 * \return true for success, false for failure.
 * Reasons for failure include this file is already open, \a dirFile is not
 * a directory, \a path is invalid, the file does not exist
 * or can't be opened in the access mode specified by oflag.
 */
bool SdBaseFile::open(SdBaseFile* dirFile, const char* path, uint8_t oflag) {
  uint8_t dname[11];
  SdBaseFile dir1, dir2;
  SdBaseFile *parent = dirFile, *sub = &dir1;

  if (!dirFile || isOpen()) return false;

  if (*path == '/') {                                         // Path starts with '/'
    if (!dirFile->isRoot()) {                                 // Is the passed dirFile the root?
      if (!dir2.openRoot(dirFile->vol_)) return false;        // Get the root in dir2, if possible
      parent = &dir2;                                         // Change 'parent' to point at the root dir
    }
    while (*path == '/') path++;                              // Skip all leading slashes
  }

  for (;;) {
    if (!make83Name(path, dname, &path)) return false;
    while (*path == '/') path++;
    if (!*path) break;
    if (!sub->open(parent, dname, O_READ)) return false;
    if (parent != dirFile) parent->close();
    parent = sub;
    sub = parent != &dir1 ? &dir1 : &dir2;
  }
  return open(parent, dname, oflag);
}

// open with filename in dname
bool SdBaseFile::open(SdBaseFile* dirFile, const uint8_t dname[11], uint8_t oflag) {
  bool emptyFound = false, fileFound = false;
  uint8_t index;
  dir_t* p;

  vol_ = dirFile->vol_;

  dirFile->rewind();
  // search for file

  while (dirFile->curPosition_ < dirFile->fileSize_) {
    index = 0xF & (dirFile->curPosition_ >> 5);
    p = dirFile->readDirCache();
    if (!p) return false;

    if (p->name[0] == DIR_NAME_FREE || p->name[0] == DIR_NAME_DELETED) {
      // remember first empty slot
      if (!emptyFound) {
        dirBlock_ = dirFile->vol_->cacheBlockNumber();
        dirIndex_ = index;
        emptyFound = true;
      }
      // done if no entries follow
      if (p->name[0] == DIR_NAME_FREE) break;
    }
    else if (!memcmp(dname, p->name, 11)) {
      fileFound = true;
      break;
    }
  }
  if (fileFound) {
    // don't open existing file if O_EXCL
    if (oflag & O_EXCL) return false;
  }
  else {
    // don't create unless O_CREAT and O_WRITE
    if (!(oflag & O_CREAT) || !(oflag & O_WRITE)) return false;
    if (emptyFound) {
      index = dirIndex_;
      p = cacheDirEntry(SdVolume::CACHE_FOR_WRITE);
      if (!p) return false;
    }
    else {
      if (dirFile->type_ == FAT_FILE_TYPE_ROOT_FIXED) return false;

      // add and zero cluster for dirFile - first cluster is in cache for write
      if (!dirFile->addDirCluster()) return false;

      // use first entry in cluster
      p = dirFile->vol_->cache()->dir;
      index = 0;
    }
    // initialize as empty file
    memset(p, 0, sizeof(*p));
    memcpy(p->name, dname, 11);

    // set timestamps
    if (dateTime_) {
      // call user date/time function
      dateTime_(&p->creationDate, &p->creationTime);
    }
    else {
      // use default date/time
      p->creationDate = FAT_DEFAULT_DATE;
      p->creationTime = FAT_DEFAULT_TIME;
    }
    p->lastAccessDate = p->creationDate;
    p->lastWriteDate = p->creationDate;
    p->lastWriteTime = p->creationTime;

    // write entry to SD
    if (!dirFile->vol_->cacheFlush()) return false;
  }
  // open entry in cache
  return openCachedEntry(index, oflag);
}

/**
 * Open a file by index.
 *
 * \param[in] dirFile An open SdFat instance for the directory.
 *
 * \param[in] index The \a index of the directory entry for the file to be
 * opened.  The value for \a index is (directory file position)/32.
 *
 * \param[in] oflag Values for \a oflag are constructed by a bitwise-inclusive
 * OR of flags O_READ, O_WRITE, O_TRUNC, and O_SYNC.
 *
 * See open() by path for definition of flags.
 * \return true for success or false for failure.
 */
bool SdBaseFile::open(SdBaseFile* dirFile, uint16_t index, uint8_t oflag) {
  dir_t* p;

  vol_ = dirFile->vol_;

  // error if already open
  if (isOpen() || !dirFile) return false;

  // don't open existing file if O_EXCL - user call error
  if (oflag & O_EXCL) return false;

  // seek to location of entry
  if (!dirFile->seekSet(32 * index)) return false;

  // read entry into cache
  p = dirFile->readDirCache();
  if (!p) return false;

  // error if empty slot or '.' or '..'
  if (p->name[0] == DIR_NAME_FREE ||
      p->name[0] == DIR_NAME_DELETED || p->name[0] == '.') {
    return false;
  }
  // open cached entry
  return openCachedEntry(index & 0xF, oflag);
}

// open a cached directory entry. Assumes vol_ is initialized
bool SdBaseFile::openCachedEntry(uint8_t dirIndex, uint8_t oflag) {
  // location of entry in cache
  dir_t* p = &vol_->cache()->dir[dirIndex];

  // write or truncate is an error for a directory or read-only file
  if (p->attributes & (DIR_ATT_READ_ONLY | DIR_ATT_DIRECTORY)) {
    if (oflag & (O_WRITE | O_TRUNC)) goto FAIL;
  }
  // remember location of directory entry on SD
  dirBlock_ = vol_->cacheBlockNumber();
  dirIndex_ = dirIndex;

  // copy first cluster number for directory fields
  firstCluster_ = (uint32_t)p->firstClusterHigh << 16;
  firstCluster_ |= p->firstClusterLow;

  // make sure it is a normal file or subdirectory
  if (DIR_IS_FILE(p)) {
    fileSize_ = p->fileSize;
    type_ = FAT_FILE_TYPE_NORMAL;
  }
  else if (DIR_IS_SUBDIR(p)) {
    if (!vol_->chainSize(firstCluster_, &fileSize_)) goto FAIL;
    type_ = FAT_FILE_TYPE_SUBDIR;
  }
  else
    goto FAIL;

  // save open flags for read/write
  flags_ = oflag & F_OFLAG;

  // set to start of file
  curCluster_ = 0;
  curPosition_ = 0;
  if ((oflag & O_TRUNC) && !truncate(0)) return false;
  return oflag & O_AT_END ? seekEnd(0) : true;

  FAIL:
  type_ = FAT_FILE_TYPE_CLOSED;
  return false;
}

/**
 * Open the next file or subdirectory in a directory.
 *
 * \param[in] dirFile An open SdFat instance for the directory containing the
 * file to be opened.
 *
 * \param[in] oflag Values for \a oflag are constructed by a bitwise-inclusive
 * OR of flags O_READ, O_WRITE, O_TRUNC, and O_SYNC.
 *
 * See open() by path for definition of flags.
 * \return true for success or false for failure.
 */
bool SdBaseFile::openNext(SdBaseFile* dirFile, uint8_t oflag) {
  dir_t* p;
  uint8_t index;

  if (!dirFile) return false;

  // error if already open
  if (isOpen()) return false;

  vol_ = dirFile->vol_;

  while (1) {
    index = 0xF & (dirFile->curPosition_ >> 5);

    // read entry into cache
    p = dirFile->readDirCache();
    if (!p) return false;

    // done if last entry
    if (p->name[0] == DIR_NAME_FREE) return false;

    // skip empty slot or '.' or '..'
    if (p->name[0] == DIR_NAME_DELETED || p->name[0] == '.') {
      continue;
    }
    // must be file or dir
    if (DIR_IS_FILE_OR_SUBDIR(p)) {
      return openCachedEntry(index, oflag);
    }
  }
  return false;
}

#if 0
/**
 * Open a directory's parent directory.
 *
 * \param[in] dir Parent of this directory will be opened.  Must not be root.
 *
 * \return true for success, false for failure.
 */
bool SdBaseFile::openParent(SdBaseFile* dir) {
  dir_t entry;
  dir_t* p;
  SdBaseFile file;
  uint32_t c;
  uint32_t cluster;
  uint32_t lbn;
  // error if already open or dir is root or dir is not a directory
  if (isOpen() || !dir || dir->isRoot() || !dir->isDir()) return false;
  vol_ = dir->vol_;
  // position to '..'
  if (!dir->seekSet(32)) return false;
  // read '..' entry
  if (dir->read(&entry, sizeof(entry)) != 32) return false;
  // verify it is '..'
  if (entry.name[0] != '.' || entry.name[1] != '.') return false;
  // start cluster for '..'
  cluster = entry.firstClusterLow;
  cluster |= (uint32_t)entry.firstClusterHigh << 16;
  if (cluster == 0) return openRoot(vol_);
  // start block for '..'
  lbn = vol_->clusterStartBlock(cluster);
  // first block of parent dir
  if (!vol_->cacheRawBlock(lbn, SdVolume::CACHE_FOR_READ)) return false;

  p = &vol_->cacheBuffer_.dir[1];
  // verify name for '../..'
  if (p->name[0] != '.' || p->name[1] != '.') return false;
  // '..' is pointer to first cluster of parent. open '../..' to find parent
  if (p->firstClusterHigh == 0 && p->firstClusterLow == 0) {
    if (!file.openRoot(dir->volume())) return false;
  }
  else if (!file.openCachedEntry(1, O_READ))
    return false;

  // search for parent in '../..'
  do {
    if (file.readDir(&entry, NULL) != 32) return false;
    c = entry.firstClusterLow;
    c |= (uint32_t)entry.firstClusterHigh << 16;
  } while (c != cluster);

  // open parent
  return open(&file, file.curPosition() / 32 - 1, O_READ);
}
#endif

/**
 * Open a volume's root directory.
 *
 * \param[in] vol The FAT volume containing the root directory to be opened.
 *
 * \return true for success, false for failure.
 * Reasons for failure include the file is already open, the FAT volume has
 * not been initialized or it a FAT12 volume.
 */
bool SdBaseFile::openRoot(SdVolume* vol) {
  // error if file is already open
  if (isOpen()) return false;

  if (vol->fatType() == 16 || (FAT12_SUPPORT && vol->fatType() == 12)) {
    type_ = FAT_FILE_TYPE_ROOT_FIXED;
    firstCluster_ = 0;
    fileSize_ = 32 * vol->rootDirEntryCount();
  }
  else if (vol->fatType() == 32) {
    type_ = FAT_FILE_TYPE_ROOT32;
    firstCluster_ = vol->rootDirStart();
    if (!vol->chainSize(firstCluster_, &fileSize_)) return false;
  }
  else // volume is not initialized, invalid, or FAT12 without support
    return false;

  vol_ = vol;
  // read only
  flags_ = O_READ;

  // set to start of file
  curCluster_ = curPosition_ = 0;

  // root has no directory entry
  dirBlock_ = dirIndex_ = 0;
  return true;
}

/**
 * Return the next available byte without consuming it.
 *
 * \return The byte if no error and not at eof else -1;
 */
int SdBaseFile::peek() {
  filepos_t pos;
  getpos(&pos);
  int c = read();
  if (c >= 0) setpos(&pos);
  return c;
}

// print uint8_t with width 2
static void print2u(const uint8_t v) {
  if (v < 10) SERIAL_CHAR('0');
  SERIAL_ECHO_F(v, DEC);
}

/**
 * %Print a directory date field to Serial.
 *
 *  Format is yyyy-mm-dd.
 *
 * \param[in] fatDate The date field from a directory entry.
 */


/**
 * %Print a directory date field.
 *
 *  Format is yyyy-mm-dd.
 *
 * \param[in] pr Print stream for output.
 * \param[in] fatDate The date field from a directory entry.
 */
void SdBaseFile::printFatDate(uint16_t fatDate) {
  SERIAL_ECHO(FAT_YEAR(fatDate));
  SERIAL_CHAR('-');
  print2u(FAT_MONTH(fatDate));
  SERIAL_CHAR('-');
  print2u(FAT_DAY(fatDate));
}


/**
 * %Print a directory time field.
 *
 * Format is hh:mm:ss.
 *
 * \param[in] pr Print stream for output.
 * \param[in] fatTime The time field from a directory entry.
 */
void SdBaseFile::printFatTime(uint16_t fatTime) {
  print2u(FAT_HOUR(fatTime));
  SERIAL_CHAR(':');
  print2u(FAT_MINUTE(fatTime));
  SERIAL_CHAR(':');
  print2u(FAT_SECOND(fatTime));
}

/**
 * Print a file's name to Serial
 *
 * \return true for success, false for failure.
 */
bool SdBaseFile::printName() {
  char name[FILENAME_LENGTH];
  if (!getFilename(name)) return false;
  SERIAL_ECHO(name);
  return true;
}

/**
 * Read the next byte from a file.
 *
 * \return For success read returns the next byte in the file as an int.
 * If an error occurs or end of file is reached -1 is returned.
 */
int16_t SdBaseFile::read() {
  uint8_t b;
  return read(&b, 1) == 1 ? b : -1;
}

/**
 * Read data from a file starting at the current position.
 *
 * \param[out] buf Pointer to the location that will receive the data.
 *
 * \param[in] nbyte Maximum number of bytes to read.
 *
 * \return For success read() returns the number of bytes read.
 * A value less than \a nbyte, including zero, will be returned
 * if end of file is reached.
 * If an error occurs, read() returns -1.  Possible errors include
 * read() called before a file has been opened, corrupt file system
 * or an I/O error occurred.
 */
int16_t SdBaseFile::read(void* buf, uint16_t nbyte) {
  uint8_t* dst = reinterpret_cast<uint8_t*>(buf);
  uint16_t offset, toRead;
  uint32_t block;  // raw device block number

  // error if not open or write only
  if (!isOpen() || !(flags_ & O_READ)) return -1;

  // max bytes left in file
  NOMORE(nbyte, fileSize_ - curPosition_);

  // amount left to read
  toRead = nbyte;
  while (toRead > 0) {
    offset = curPosition_ & 0x1FF;  // offset in block
    if (type_ == FAT_FILE_TYPE_ROOT_FIXED) {
      block = vol_->rootDirStart() + (curPosition_ >> 9);
    }
    else {
      uint8_t blockOfCluster = vol_->blockOfCluster(curPosition_);
      if (offset == 0 && blockOfCluster == 0) {
        // start of new cluster
        if (curPosition_ == 0)
          curCluster_ = firstCluster_;                      // use first cluster in file
        else if (!vol_->fatGet(curCluster_, &curCluster_))  // get next cluster from FAT
          return -1;
      }
      block = vol_->clusterStartBlock(curCluster_) + blockOfCluster;
    }
    uint16_t n = toRead;

    // amount to be read from current block
    NOMORE(n, 512 - offset);

    // no buffering needed if n == 512
    if (n == 512 && block != vol_->cacheBlockNumber()) {
      if (!vol_->readBlock(block, dst)) return -1;
    }
    else {
      // read block to cache and copy data to caller
      if (!vol_->cacheRawBlock(block, SdVolume::CACHE_FOR_READ)) return -1;
      uint8_t* src = vol_->cache()->data + offset;
      memcpy(dst, src, n);
    }
    dst += n;
    curPosition_ += n;
    toRead -= n;
  }
  return nbyte;
}

/**
 * Read the next entry in a directory.
 *
 * \param[out] dir The dir_t struct that will receive the data.
 *
 * \return For success readDir() returns the number of bytes read.
 * A value of zero will be returned if end of file is reached.
 * If an error occurs, readDir() returns -1.  Possible errors include
 * readDir() called before a directory has been opened, this is not
 * a directory file or an I/O error occurred.
 */
int8_t SdBaseFile::readDir(dir_t* dir, char* longFilename) {
  int16_t n;
  // if not a directory file or miss-positioned return an error
  if (!isDir() || (0x1F & curPosition_)) return -1;

  // If we have a longFilename buffer, mark it as invalid.
  // If long filename is found it will be filled automatically.
  if (longFilename) longFilename[0] = '\0';

  while (1) {

    n = read(dir, sizeof(dir_t));
    if (n != sizeof(dir_t)) return n ? -1 : 0;

    // last entry if DIR_NAME_FREE
    if (dir->name[0] == DIR_NAME_FREE) return 0;

    // skip deleted entry and entry for .  and ..
    if (dir->name[0] == DIR_NAME_DELETED || dir->name[0] == '.') {
      if (longFilename) longFilename[0] = '\0';     // Invalidate erased file long name, if any
      continue;
    }

    // Fill the long filename if we have a long filename entry.
    // Long filename entries are stored before the short filename.
    if (longFilename && DIR_IS_LONG_NAME(dir)) {
      vfat_t* VFAT = (vfat_t*)dir;
      // Sanity-check the VFAT entry. The first cluster is always set to zero. And the sequence number should be higher than 0
      if (VFAT->firstClusterLow == 0) {
        const uint8_t seq = VFAT->sequenceNumber & 0x1F;
        if (WITHIN(seq, 1, MAX_VFAT_ENTRIES)) {
          // TODO: Store the filename checksum to verify if a long-filename-unaware system modified the file table.
          n = (seq - 1) * (FILENAME_LENGTH);
          for (uint8_t i = 0; i < FILENAME_LENGTH; i++)
            longFilename[n + i] = (i < 5) ? VFAT->name1[i] : (i < 11) ? VFAT->name2[i - 5] : VFAT->name3[i - 11];
          // If this VFAT entry is the last one, add a NUL terminator at the end of the string
          if (VFAT->sequenceNumber & 0x40) longFilename[n + FILENAME_LENGTH] = '\0';
        }
      }
    }
    // Return if normal file or subdirectory
    if (DIR_IS_FILE_OR_SUBDIR(dir)) return n;
  }
}


// Read next directory entry into the cache
// Assumes file is correctly positioned
dir_t* SdBaseFile::readDirCache() {
  uint8_t i;
  // error if not directory
  if (!isDir()) return 0;

  // index of entry in cache
  i = (curPosition_ >> 5) & 0xF;

  // use read to locate and cache block
  if (read() < 0) return 0;

  // advance to next entry
  curPosition_ += 31;

  // return pointer to entry
  return vol_->cache()->dir + i;
}

/**
 * Remove a file.
 *
 * The directory entry and all data for the file are deleted.
 *
 * \note This function should not be used to delete the 8.3 version of a
 * file that has a long name. For example if a file has the long name
 * "New Text Document.txt" you should not delete the 8.3 name "NEWTEX~1.TXT".
 *
 * \return true for success, false for failure.
 * Reasons for failure include the file read-only, is a directory,
 * or an I/O error occurred.
 */
bool SdBaseFile::remove() {
  dir_t* d;
  // free any clusters - will fail if read-only or directory
  if (!truncate(0)) return false;

  // cache directory entry
  d = cacheDirEntry(SdVolume::CACHE_FOR_WRITE);
  if (!d) return false;

  // mark entry deleted
  d->name[0] = DIR_NAME_DELETED;

  // set this file closed
  type_ = FAT_FILE_TYPE_CLOSED;

  // write entry to SD
  return vol_->cacheFlush();
  return true;
}

/**
 * Remove a file.
 *
 * The directory entry and all data for the file are deleted.
 *
 * \param[in] dirFile The directory that contains the file.
 * \param[in] path Path for the file to be removed.
 *
 * \note This function should not be used to delete the 8.3 version of a
 * file that has a long name. For example if a file has the long name
 * "New Text Document.txt" you should not delete the 8.3 name "NEWTEX~1.TXT".
 *
 * \return true for success, false for failure.
 * Reasons for failure include the file is a directory, is read only,
 * \a dirFile is not a directory, \a path is not found
 * or an I/O error occurred.
 */
bool SdBaseFile::remove(SdBaseFile* dirFile, const char* path) {
  SdBaseFile file;
  return file.open(dirFile, path, O_WRITE) ? file.remove() : false;
}

/**
 * Rename a file or subdirectory.
 *
 * \param[in] dirFile Directory for the new path.
 * \param[in] newPath New path name for the file/directory.
 *
 * \return true for success, false for failure.
 * Reasons for failure include \a dirFile is not open or is not a directory
 * file, newPath is invalid or already exists, or an I/O error occurs.
 */
bool SdBaseFile::rename(SdBaseFile* dirFile, const char* newPath) {
  dir_t entry;
  uint32_t dirCluster = 0;
  SdBaseFile file;
  dir_t* d;

  // must be an open file or subdirectory
  if (!(isFile() || isSubDir())) return false;

  // can't move file
  if (vol_ != dirFile->vol_) return false;

  // sync() and cache directory entry
  sync();
  d = cacheDirEntry(SdVolume::CACHE_FOR_WRITE);
  if (!d) return false;

  // save directory entry
  memcpy(&entry, d, sizeof(entry));

  // mark entry deleted
  d->name[0] = DIR_NAME_DELETED;

  // make directory entry for new path
  if (isFile()) {
    if (!file.open(dirFile, newPath, O_CREAT | O_EXCL | O_WRITE)) {
      goto restore;
    }
  }
  else {
    // don't create missing path prefix components
    if (!file.mkdir(dirFile, newPath, false)) {
      goto restore;
    }
    // save cluster containing new dot dot
    dirCluster = file.firstCluster_;
  }
  // change to new directory entry
  dirBlock_ = file.dirBlock_;
  dirIndex_ = file.dirIndex_;

  // mark closed to avoid possible destructor close call
  file.type_ = FAT_FILE_TYPE_CLOSED;

  // cache new directory entry
  d = cacheDirEntry(SdVolume::CACHE_FOR_WRITE);
  if (!d) return false;

  // copy all but name field to new directory entry
  memcpy(&d->attributes, &entry.attributes, sizeof(entry) - sizeof(d->name));

  // update dot dot if directory
  if (dirCluster) {
    // get new dot dot
    uint32_t block = vol_->clusterStartBlock(dirCluster);
    if (!vol_->cacheRawBlock(block, SdVolume::CACHE_FOR_READ)) return false;
    memcpy(&entry, &vol_->cache()->dir[1], sizeof(entry));

    // free unused cluster
    if (!vol_->freeChain(dirCluster)) return false;

    // store new dot dot
    block = vol_->clusterStartBlock(firstCluster_);
    if (!vol_->cacheRawBlock(block, SdVolume::CACHE_FOR_WRITE)) return false;
    memcpy(&vol_->cache()->dir[1], &entry, sizeof(entry));
  }
  return vol_->cacheFlush();

restore:
  if ((d = cacheDirEntry(SdVolume::CACHE_FOR_WRITE))) {
    // restore entry
    d->name[0] = entry.name[0];
    vol_->cacheFlush();
  }
  return false;
}

/**
 * Remove a directory file.
 *
 * The directory file will be removed only if it is empty and is not the
 * root directory.  rmdir() follows DOS and Windows and ignores the
 * read-only attribute for the directory.
 *
 * \note This function should not be used to delete the 8.3 version of a
 * directory that has a long name. For example if a directory has the
 * long name "New folder" you should not delete the 8.3 name "NEWFOL~1".
 *
 * \return true for success, false for failure.
 * Reasons for failure include the file is not a directory, is the root
 * directory, is not empty, or an I/O error occurred.
 */
bool SdBaseFile::rmdir() {
  // must be open subdirectory
  if (!isSubDir()) return false;

  rewind();

  // make sure directory is empty
  while (curPosition_ < fileSize_) {
    dir_t* p = readDirCache();
    if (!p) return false;
    // done if past last used entry
    if (p->name[0] == DIR_NAME_FREE) break;
    // skip empty slot, '.' or '..'
    if (p->name[0] == DIR_NAME_DELETED || p->name[0] == '.') continue;
    // error not empty
    if (DIR_IS_FILE_OR_SUBDIR(p)) return false;
  }
  // convert empty directory to normal file for remove
  type_ = FAT_FILE_TYPE_NORMAL;
  flags_ |= O_WRITE;
  return remove();
}

/**
 * Recursively delete a directory and all contained files.
 *
 * This is like the Unix/Linux 'rm -rf *' if called with the root directory
 * hence the name.
 *
 * Warning - This will remove all contents of the directory including
 * subdirectories.  The directory will then be removed if it is not root.
 * The read-only attribute for files will be ignored.
 *
 * \note This function should not be used to delete the 8.3 version of
 * a directory that has a long name.  See remove() and rmdir().
 *
 * \return true for success, false for failure.
 */
bool SdBaseFile::rmRfStar() {
  uint32_t index;
  SdBaseFile f;
  rewind();
  while (curPosition_ < fileSize_) {
    // remember position
    index = curPosition_ / 32;

    dir_t* p = readDirCache();
    if (!p) return false;

    // done if past last entry
    if (p->name[0] == DIR_NAME_FREE) break;

    // skip empty slot or '.' or '..'
    if (p->name[0] == DIR_NAME_DELETED || p->name[0] == '.') continue;

    // skip if part of long file name or volume label in root
    if (!DIR_IS_FILE_OR_SUBDIR(p)) continue;

    if (!f.open(this, index, O_READ)) return false;
    if (f.isSubDir()) {
      // recursively delete
      if (!f.rmRfStar()) return false;
    }
    else {
      // ignore read-only
      f.flags_ |= O_WRITE;
      if (!f.remove()) return false;
    }
    // position to next entry if required
    if (curPosition_ != (32 * (index + 1))) {
      if (!seekSet(32 * (index + 1))) return false;
    }
  }
  // don't try to delete root
  if (!isRoot()) {
    if (!rmdir()) return false;
  }
  return true;
}

/**
 * Create a file object and open it in the current working directory.
 *
 * \param[in] path A path with a valid 8.3 DOS name for a file to be opened.
 *
 * \param[in] oflag Values for \a oflag are constructed by a bitwise-inclusive
 * OR of open flags. see SdBaseFile::open(SdBaseFile*, const char*, uint8_t).
 */
SdBaseFile::SdBaseFile(const char* path, uint8_t oflag) {
  type_ = FAT_FILE_TYPE_CLOSED;
  writeError = false;
  open(path, oflag);
}

/**
 * Sets a file's position.
 *
 * \param[in] pos The new position in bytes from the beginning of the file.
 *
 * \return true for success, false for failure.
 */
bool SdBaseFile::seekSet(const uint32_t pos) {
  uint32_t nCur, nNew;
  // error if file not open or seek past end of file
  if (!isOpen() || pos > fileSize_) return false;

  if (type_ == FAT_FILE_TYPE_ROOT_FIXED) {
    curPosition_ = pos;
    return true;
  }
  if (pos == 0) {
    curCluster_ = curPosition_ = 0;   // set position to start of file
    return true;
  }

  // calculate cluster index for cur and new position
  nCur = (curPosition_ - 1) >> (vol_->clusterSizeShift_ + 9);
  nNew = (pos - 1) >> (vol_->clusterSizeShift_ + 9);

  if (nNew < nCur || curPosition_ == 0)
    curCluster_ = firstCluster_;      // must follow chain from first cluster
  else
    nNew -= nCur;                     // advance from curPosition

  while (nNew--)
    if (!vol_->fatGet(curCluster_, &curCluster_)) return false;

  curPosition_ = pos;
  return true;
}

void SdBaseFile::setpos(filepos_t* pos) {
  curPosition_ = pos->position;
  curCluster_ = pos->cluster;
}

/**
 * The sync() call causes all modified data and directory fields
 * to be written to the storage device.
 *
 * \return true for success, false for failure.
 * Reasons for failure include a call to sync() before a file has been
 * opened or an I/O error.
 */
bool SdBaseFile::sync() {
  // only allow open files and directories
  if (!isOpen()) goto FAIL;

  if (flags_ & F_FILE_DIR_DIRTY) {
    dir_t* d = cacheDirEntry(SdVolume::CACHE_FOR_WRITE);
    // check for deleted by another open file object
    if (!d || d->name[0] == DIR_NAME_DELETED) goto FAIL;

    // do not set filesize for dir files
    if (!isDir()) d->fileSize = fileSize_;

    // update first cluster fields
    d->firstClusterLow = firstCluster_ & 0xFFFF;
    d->firstClusterHigh = firstCluster_ >> 16;

    // set modify time if user supplied a callback date/time function
    if (dateTime_) {
      dateTime_(&d->lastWriteDate, &d->lastWriteTime);
      d->lastAccessDate = d->lastWriteDate;
    }
    // clear directory dirty
    flags_ &= ~F_FILE_DIR_DIRTY;
  }
  return vol_->cacheFlush();

  FAIL:
  writeError = true;
  return false;
}

/**
 * Copy a file's timestamps
 *
 * \param[in] file File to copy timestamps from.
 *
 * \note
 * Modify and access timestamps may be overwritten if a date time callback
 * function has been set by dateTimeCallback().
 *
 * \return true for success, false for failure.
 */
bool SdBaseFile::timestamp(SdBaseFile* file) {
  dir_t* d;
  dir_t dir;

  // get timestamps
  if (!file->dirEntry(&dir)) return false;

  // update directory fields
  if (!sync()) return false;

  d = cacheDirEntry(SdVolume::CACHE_FOR_WRITE);
  if (!d) return false;

  // copy timestamps
  d->lastAccessDate = dir.lastAccessDate;
  d->creationDate = dir.creationDate;
  d->creationTime = dir.creationTime;
  d->creationTimeTenths = dir.creationTimeTenths;
  d->lastWriteDate = dir.lastWriteDate;
  d->lastWriteTime = dir.lastWriteTime;

  // write back entry
  return vol_->cacheFlush();
}

/**
 * Set a file's timestamps in its directory entry.
 *
 * \param[in] flags Values for \a flags are constructed by a bitwise-inclusive
 * OR of flags from the following list
 *
 * T_ACCESS - Set the file's last access date.
 *
 * T_CREATE - Set the file's creation date and time.
 *
 * T_WRITE - Set the file's last write/modification date and time.
 *
 * \param[in] year Valid range 1980 - 2107 inclusive.
 *
 * \param[in] month Valid range 1 - 12 inclusive.
 *
 * \param[in] day Valid range 1 - 31 inclusive.
 *
 * \param[in] hour Valid range 0 - 23 inclusive.
 *
 * \param[in] minute Valid range 0 - 59 inclusive.
 *
 * \param[in] second Valid range 0 - 59 inclusive
 *
 * \note It is possible to set an invalid date since there is no check for
 * the number of days in a month.
 *
 * \note
 * Modify and access timestamps may be overwritten if a date time callback
 * function has been set by dateTimeCallback().
 *
 * \return true for success, false for failure.
 */
bool SdBaseFile::timestamp(uint8_t flags, uint16_t year, uint8_t month,
                           uint8_t day, uint8_t hour, uint8_t minute, uint8_t second) {
  uint16_t dirDate, dirTime;
  dir_t* d;

  if (!isOpen()
      || year < 1980
      || year > 2107
      || month < 1
      || month > 12
      || day < 1
      || day > 31
      || hour > 23
      || minute > 59
      || second > 59) {
    return false;
  }
  // update directory entry
  if (!sync()) return false;

  d = cacheDirEntry(SdVolume::CACHE_FOR_WRITE);
  if (!d) return false;

  dirDate = FAT_DATE(year, month, day);
  dirTime = FAT_TIME(hour, minute, second);
  if (flags & T_ACCESS) {
    d->lastAccessDate = dirDate;
  }
  if (flags & T_CREATE) {
    d->creationDate = dirDate;
    d->creationTime = dirTime;
    // seems to be units of 1/100 second not 1/10 as Microsoft states
    d->creationTimeTenths = second & 1 ? 100 : 0;
  }
  if (flags & T_WRITE) {
    d->lastWriteDate = dirDate;
    d->lastWriteTime = dirTime;
  }
  return vol_->cacheFlush();
}

/**
 * Truncate a file to a specified length.  The current file position
 * will be maintained if it is less than or equal to \a length otherwise
 * it will be set to end of file.
 *
 * \param[in] length The desired length for the file.
 *
 * \return true for success, false for failure.
 * Reasons for failure include file is read only, file is a directory,
 * \a length is greater than the current file size or an I/O error occurs.
 */
bool SdBaseFile::truncate(uint32_t length) {
  uint32_t newPos;
  // error if not a normal file or read-only
  if (!isFile() || !(flags_ & O_WRITE)) return false;

  // error if length is greater than current size
  if (length > fileSize_) return false;

  // fileSize and length are zero - nothing to do
  if (fileSize_ == 0) return true;

  // remember position for seek after truncation
  newPos = curPosition_ > length ? length : curPosition_;

  // position to last cluster in truncated file
  if (!seekSet(length)) return false;

  if (length == 0) {
    // free all clusters
    if (!vol_->freeChain(firstCluster_)) return false;
    firstCluster_ = 0;
  }
  else {
    uint32_t toFree;
    if (!vol_->fatGet(curCluster_, &toFree)) return false;

    if (!vol_->isEOC(toFree)) {
      // free extra clusters
      if (!vol_->freeChain(toFree)) return false;

      // current cluster is end of chain
      if (!vol_->fatPutEOC(curCluster_)) return false;
    }
  }
  fileSize_ = length;

  // need to update directory entry
  flags_ |= F_FILE_DIR_DIRTY;

  if (!sync()) return false;

  // set file to correct position
  return seekSet(newPos);
}

/**
 * Write data to an open file.
 *
 * \note Data is moved to the cache but may not be written to the
 * storage device until sync() is called.
 *
 * \param[in] buf Pointer to the location of the data to be written.
 *
 * \param[in] nbyte Number of bytes to write.
 *
 * \return For success write() returns the number of bytes written, always
 * \a nbyte.  If an error occurs, write() returns -1.  Possible errors
 * include write() is called before a file has been opened, write is called
 * for a read-only file, device is full, a corrupt file system or an I/O error.
 *
 */
int16_t SdBaseFile::write(const void* buf, uint16_t nbyte) {
  // convert void* to uint8_t*  -  must be before goto statements
  const uint8_t* src = reinterpret_cast<const uint8_t*>(buf);

  // number of bytes left to write  -  must be before goto statements
  uint16_t nToWrite = nbyte;

  // error if not a normal file or is read-only
  if (!isFile() || !(flags_ & O_WRITE)) goto FAIL;

  // seek to end of file if append flag
  if ((flags_ & O_APPEND) && curPosition_ != fileSize_) {
    if (!seekEnd()) goto FAIL;
  }

  while (nToWrite > 0) {
    uint8_t blockOfCluster = vol_->blockOfCluster(curPosition_);
    uint16_t blockOffset = curPosition_ & 0x1FF;
    if (blockOfCluster == 0 && blockOffset == 0) {
      // start of new cluster
      if (curCluster_ == 0) {
        if (firstCluster_ == 0) {
          // allocate first cluster of file
          if (!addCluster()) goto FAIL;
        }
        else {
          curCluster_ = firstCluster_;
        }
      }
      else {
        uint32_t next;
        if (!vol_->fatGet(curCluster_, &next)) goto FAIL;
        if (vol_->isEOC(next)) {
          // add cluster if at end of chain
          if (!addCluster()) goto FAIL;
        }
        else {
          curCluster_ = next;
        }
      }
    }
    // max space in block
    uint16_t n = 512 - blockOffset;

    // lesser of space and amount to write
    NOMORE(n, nToWrite);

    // block for data write
    uint32_t block = vol_->clusterStartBlock(curCluster_) + blockOfCluster;
    if (n == 512) {
      // full block - don't need to use cache
      if (vol_->cacheBlockNumber() == block) {
        // invalidate cache if block is in cache
        vol_->cacheSetBlockNumber(0xFFFFFFFF, false);
      }
      if (!vol_->writeBlock(block, src)) goto FAIL;
    }
    else {
      if (blockOffset == 0 && curPosition_ >= fileSize_) {
        // start of new block don't need to read into cache
        if (!vol_->cacheFlush()) goto FAIL;
        // set cache dirty and SD address of block
        vol_->cacheSetBlockNumber(block, true);
      }
      else {
        // rewrite part of block
        if (!vol_->cacheRawBlock(block, SdVolume::CACHE_FOR_WRITE)) goto FAIL;
      }
      uint8_t* dst = vol_->cache()->data + blockOffset;
      memcpy(dst, src, n);
    }
    curPosition_ += n;
    src += n;
    nToWrite -= n;
  }
  if (curPosition_ > fileSize_) {
    // update fileSize and insure sync will update dir entry
    fileSize_ = curPosition_;
    flags_ |= F_FILE_DIR_DIRTY;
  }
  else if (dateTime_ && nbyte) {
    // insure sync will update modified date and time
    flags_ |= F_FILE_DIR_DIRTY;
  }

  if (flags_ & O_SYNC) {
    if (!sync()) goto FAIL;
  }
  return nbyte;

  FAIL:
  // return for write error
  writeError = true;
  return -1;
}

#if ALLOW_DEPRECATED_FUNCTIONS
  void (*SdBaseFile::oldDateTime_)(uint16_t &date, uint16_t &time) = 0;
#endif

#endif // SDSUPPORT
