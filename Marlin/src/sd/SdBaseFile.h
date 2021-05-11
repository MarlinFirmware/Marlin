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

/**
 * sd/SdBaseFile.h
 *
 * Arduino SdFat Library
 * Copyright (c) 2009 by William Greiman
 *
 * This file is part of the Arduino Sd2Card Library
 */

#include "SdFatConfig.h"
#include "SdVolume.h"

#include <stdint.h>

/**
 * \struct filepos_t
 * \brief internal type for istream
 * do not use in user apps
 */
struct filepos_t {
  uint32_t position;  // stream byte position
  uint32_t cluster;   // cluster of position
  filepos_t() : position(0), cluster(0) {}
};

// use the gnu style oflag in open()
uint8_t const O_READ = 0x01,                    // open() oflag for reading
              O_RDONLY = O_READ,                // open() oflag - same as O_IN
              O_WRITE = 0x02,                   // open() oflag for write
              O_WRONLY = O_WRITE,               // open() oflag - same as O_WRITE
              O_RDWR = (O_READ | O_WRITE),      // open() oflag for reading and writing
              O_ACCMODE = (O_READ | O_WRITE),   // open() oflag mask for access modes
              O_APPEND = 0x04,                  // The file offset shall be set to the end of the file prior to each write.
              O_SYNC = 0x08,                    // Synchronous writes - call sync() after each write
              O_TRUNC = 0x10,                   // Truncate the file to zero length
              O_AT_END = 0x20,                  // Set the initial position at the end of the file
              O_CREAT = 0x40,                   // Create the file if nonexistent
              O_EXCL = 0x80;                    // If O_CREAT and O_EXCL are set, open() shall fail if the file exists

// SdBaseFile class static and const definitions

// flags for ls()
uint8_t const LS_DATE = 1,    // ls() flag to print modify date
              LS_SIZE = 2,    // ls() flag to print file size
              LS_R = 4;       // ls() flag for recursive list of subdirectories


// flags for timestamp
uint8_t const T_ACCESS = 1,   // Set the file's last access date
              T_CREATE = 2,   // Set the file's creation date and time
              T_WRITE = 4;    // Set the file's write date and time

// values for type_
uint8_t const FAT_FILE_TYPE_CLOSED = 0,                           // This file has not been opened.
              FAT_FILE_TYPE_NORMAL = 1,                           // A normal file
              FAT_FILE_TYPE_ROOT_FIXED = 2,                       // A FAT12 or FAT16 root directory
              FAT_FILE_TYPE_ROOT32 = 3,                           // A FAT32 root directory
              FAT_FILE_TYPE_SUBDIR = 4,                           // A subdirectory file
              FAT_FILE_TYPE_MIN_DIR = FAT_FILE_TYPE_ROOT_FIXED;   // Test value for directory type

/**
 * date field for FAT directory entry
 * \param[in] year [1980,2107]
 * \param[in] month [1,12]
 * \param[in] day [1,31]
 *
 * \return Packed date for dir_t entry.
 */
static inline uint16_t FAT_DATE(uint16_t year, uint8_t month, uint8_t day) { return (year - 1980) << 9 | month << 5 | day; }

/**
 * year part of FAT directory date field
 * \param[in] fatDate Date in packed dir format.
 *
 * \return Extracted year [1980,2107]
 */
static inline uint16_t FAT_YEAR(uint16_t fatDate) { return 1980 + (fatDate >> 9); }

/**
 * month part of FAT directory date field
 * \param[in] fatDate Date in packed dir format.
 *
 * \return Extracted month [1,12]
 */
static inline uint8_t FAT_MONTH(uint16_t fatDate) { return (fatDate >> 5) & 0xF; }

/**
 * day part of FAT directory date field
 * \param[in] fatDate Date in packed dir format.
 *
 * \return Extracted day [1,31]
 */
static inline uint8_t FAT_DAY(uint16_t fatDate) { return fatDate & 0x1F; }

/**
 * time field for FAT directory entry
 * \param[in] hour [0,23]
 * \param[in] minute [0,59]
 * \param[in] second [0,59]
 *
 * \return Packed time for dir_t entry.
 */
static inline uint16_t FAT_TIME(uint8_t hour, uint8_t minute, uint8_t second) { return hour << 11 | minute << 5 | second >> 1; }

/**
 * hour part of FAT directory time field
 * \param[in] fatTime Time in packed dir format.
 *
 * \return Extracted hour [0,23]
 */
static inline uint8_t FAT_HOUR(uint16_t fatTime) { return fatTime >> 11; }

/**
 * minute part of FAT directory time field
 * \param[in] fatTime Time in packed dir format.
 *
 * \return Extracted minute [0,59]
 */
static inline uint8_t FAT_MINUTE(uint16_t fatTime) { return (fatTime >> 5) & 0x3F; }

/**
 * second part of FAT directory time field
 * Note second/2 is stored in packed time.
 *
 * \param[in] fatTime Time in packed dir format.
 *
 * \return Extracted second [0,58]
 */
static inline uint8_t FAT_SECOND(uint16_t fatTime) { return 2 * (fatTime & 0x1F); }

// Default date for file timestamps is 1 Jan 2000
uint16_t const FAT_DEFAULT_DATE = ((2000 - 1980) << 9) | (1 << 5) | 1;
// Default time for file timestamp is 1 am
uint16_t const FAT_DEFAULT_TIME = (1 << 11);

/**
 * \class SdBaseFile
 * \brief Base class for SdFile with Print and C++ streams.
 */
class SdBaseFile {
 public:
  SdBaseFile() : writeError(false), type_(FAT_FILE_TYPE_CLOSED) {}
  SdBaseFile(const char *path, uint8_t oflag);
  ~SdBaseFile() { if (isOpen()) close(); }

  /**
   * writeError is set to true if an error occurs during a write().
   * Set writeError to false before calling print() and/or write() and check
   * for true after calls to print() and/or write().
   */
  bool writeError;

  // helpers for stream classes

  /**
   * get position for streams
   * \param[out] pos struct to receive position
   */
  void getpos(filepos_t *pos);

  /**
   * set position for streams
   * \param[out] pos struct with value for new position
   */
  void setpos(filepos_t *pos);

  bool close();
  bool contiguousRange(uint32_t *bgnBlock, uint32_t *endBlock);
  bool createContiguous(SdBaseFile *dirFile,
                        const char *path, uint32_t size);
  /**
   * \return The current cluster number for a file or directory.
   */
  uint32_t curCluster() const { return curCluster_; }

  /**
   * \return The current position for a file or directory.
   */
  uint32_t curPosition() const { return curPosition_; }

  /**
   * \return Current working directory
   */
  static SdBaseFile *cwd() { return cwd_; }

  /**
   * Set the date/time callback function
   *
   * \param[in] dateTime The user's call back function.  The callback
   * function is of the form:
   *
   * \code
   * void dateTime(uint16_t *date, uint16_t *time) {
   *   uint16_t year;
   *   uint8_t month, day, hour, minute, second;
   *
   *   // User gets date and time from GPS or real-time clock here
   *
   *   // return date using FAT_DATE macro to format fields
   *   *date = FAT_DATE(year, month, day);
   *
   *   // return time using FAT_TIME macro to format fields
   *   *time = FAT_TIME(hour, minute, second);
   * }
   * \endcode
   *
   * Sets the function that is called when a file is created or when
   * a file's directory entry is modified by sync(). All timestamps,
   * access, creation, and modify, are set when a file is created.
   * sync() maintains the last access date and last modify date/time.
   *
   * See the timestamp() function.
   */
  static void dateTimeCallback(
    void (*dateTime)(uint16_t *date, uint16_t *time)) {
    dateTime_ = dateTime;
  }

  /**
   * Cancel the date/time callback function.
   */
  static void dateTimeCallbackCancel() { dateTime_ = 0; }
  bool dirEntry(dir_t *dir);
  static void dirName(const dir_t& dir, char *name);
  bool exists(const char *name);
  int16_t fgets(char *str, int16_t num, char *delim = 0);

  /**
   * \return The total number of bytes in a file or directory.
   */
  uint32_t fileSize() const { return fileSize_; }

  /**
   * \return The first cluster number for a file or directory.
   */
  uint32_t firstCluster() const { return firstCluster_; }

  /**
   * \return True if this is a directory else false.
   */
  bool isDir() const { return type_ >= FAT_FILE_TYPE_MIN_DIR; }

  /**
   * \return True if this is a normal file else false.
   */
  bool isFile() const { return type_ == FAT_FILE_TYPE_NORMAL; }

  /**
   * \return True if this is an open file/directory else false.
   */
  bool isOpen() const { return type_ != FAT_FILE_TYPE_CLOSED; }

  /**
   * \return True if this is a subdirectory else false.
   */
  bool isSubDir() const { return type_ == FAT_FILE_TYPE_SUBDIR; }

  /**
   * \return True if this is the root directory.
   */
  bool isRoot() const { return type_ == FAT_FILE_TYPE_ROOT_FIXED || type_ == FAT_FILE_TYPE_ROOT32; }

  bool getDosName(char * const name);
  void ls(uint8_t flags = 0, uint8_t indent = 0);

  bool mkdir(SdBaseFile *dir, const char *path, bool pFlag = true);
  bool open(SdBaseFile *dirFile, uint16_t index, uint8_t oflag);
  bool open(SdBaseFile *dirFile, const char *path, uint8_t oflag);
  bool open(const char *path, uint8_t oflag = O_READ);
  bool openNext(SdBaseFile *dirFile, uint8_t oflag);
  bool openRoot(SdVolume *vol);
  int peek();
  static void printFatDate(uint16_t fatDate);
  static void printFatTime(uint16_t fatTime);
  bool printName();
  int16_t read();
  int16_t read(void *buf, uint16_t nbyte);
  int8_t readDir(dir_t *dir, char *longFilename);
  static bool remove(SdBaseFile *dirFile, const char *path);
  bool remove();

  /**
   * Set the file's current position to zero.
   */
  void rewind() { seekSet(0); }
  bool rename(SdBaseFile *dirFile, const char *newPath);
  bool rmdir();
  bool rmRfStar();

  /**
   * Set the files position to current position + \a pos. See seekSet().
   * \param[in] offset The new position in bytes from the current position.
   * \return true for success or false for failure.
   */
  bool seekCur(const int32_t offset) { return seekSet(curPosition_ + offset); }

  /**
   * Set the files position to end-of-file + \a offset. See seekSet().
   * \param[in] offset The new position in bytes from end-of-file.
   * \return true for success or false for failure.
   */
  bool seekEnd(const int32_t offset = 0) { return seekSet(fileSize_ + offset); }
  bool seekSet(const uint32_t pos);
  bool sync();
  bool timestamp(SdBaseFile *file);
  bool timestamp(uint8_t flag, uint16_t year, uint8_t month, uint8_t day,
                 uint8_t hour, uint8_t minute, uint8_t second);

  /**
   * Type of file. Use isFile() or isDir() instead of type() if possible.
   *
   * \return The file or directory type.
   */
  uint8_t type() const { return type_; }
  bool truncate(uint32_t size);

  /**
   * \return SdVolume that contains this file.
   */
  SdVolume* volume() const { return vol_; }
  int16_t write(const void *buf, uint16_t nbyte);

 private:
  friend class SdFat;           // allow SdFat to set cwd_
  static SdBaseFile *cwd_;      // global pointer to cwd dir

  // data time callback function
  static void (*dateTime_)(uint16_t *date, uint16_t *time);

  // bits defined in flags_
  static uint8_t const F_OFLAG = (O_ACCMODE | O_APPEND | O_SYNC),   // should be 0x0F
                       F_FILE_DIR_DIRTY = 0x80;                     // sync of directory entry required

  // private data
  uint8_t   flags_;         // See above for definition of flags_ bits
  uint8_t   fstate_;        // error and eof indicator
  uint8_t   type_;          // type of file see above for values
  uint32_t  curCluster_;    // cluster for current file position
  uint32_t  curPosition_;   // current file position in bytes from beginning
  uint32_t  dirBlock_;      // block for this files directory entry
  uint8_t   dirIndex_;      // index of directory entry in dirBlock
  uint32_t  fileSize_;      // file size in bytes
  uint32_t  firstCluster_;  // first cluster of file
  SdVolume  *vol_;          // volume where file is located

  /**
   * EXPERIMENTAL - Don't use!
   */
  //bool openParent(SdBaseFile *dir);

  // private functions
  bool addCluster();
  bool addDirCluster();
  dir_t* cacheDirEntry(uint8_t action);
  int8_t lsPrintNext(uint8_t flags, uint8_t indent);
  static bool make83Name(const char *str, uint8_t *name, const char **ptr);
  bool mkdir(SdBaseFile *parent, const uint8_t dname[11]);
  bool open(SdBaseFile *dirFile, const uint8_t dname[11], uint8_t oflag);
  bool openCachedEntry(uint8_t cacheIndex, uint8_t oflags);
  dir_t* readDirCache();
};
