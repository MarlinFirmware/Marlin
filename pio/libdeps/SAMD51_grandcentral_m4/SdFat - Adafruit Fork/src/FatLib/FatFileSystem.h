/**
 * Copyright (c) 2011-2018 Bill Greiman
 * This file is part of the SdFat library for SD memory cards.
 *
 * MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
#ifndef FatFileSystem_h
#define FatFileSystem_h
#include "FatVolume.h"
#include "FatFile.h"
#include "ArduinoFiles.h"
/**
 * \file
 * \brief FatFileSystem class
 */
//------------------------------------------------------------------------------
/**
 * \class FatFileSystem
 * \brief Integration class for the FatLib library.
 */
class FatFileSystem : public  FatVolume {
 public:
  /**
   * Initialize an FatFileSystem object.
   * \param[in] blockDev Device block driver.
   * \param[in] part partition to initialize.
   * \return The value true is returned for success and
   * the value false is returned for failure.
   */
  bool begin(BlockDriver* blockDev, uint8_t part = 0) {
    m_blockDev = blockDev;
    vwd()->close();
    return (part ? init(part) : init(1) || init(0))
            && vwd()->openRoot(this) && FatFile::setCwd(vwd());
  }
#if ENABLE_ARDUINO_FEATURES
   /** List the directory contents of the volume working directory to Serial.
   *
   * \param[in] flags The inclusive OR of
   *
   * LS_DATE - %Print file modification date
   *
   * LS_SIZE - %Print file size.
   *
   * LS_R - Recursive list of subdirectories.
   *
   * \return true for success or false if an error occurred.
   */
  bool ls(uint8_t flags = 0) {
    return ls(&Serial, flags);
  }
  /** List the directory contents of a directory to Serial.
   *
   * \param[in] path directory to list.
   *
   * \param[in] flags The inclusive OR of
   *
   * LS_DATE - %Print file modification date
   *
   * LS_SIZE - %Print file size.
   *
   * LS_R - Recursive list of subdirectories.
   *
   * \return true for success or false if an error occurred.
   */
  bool ls(const char* path, uint8_t flags = 0) {
    return ls(&Serial, path, flags);
  }
  /** open a file
   *
   * \param[in] path location of file to be opened.
   * \param[in] oflag open flags.
   * \return a File object.
   */
  File open(const char *path, oflag_t oflag = FILE_READ) {
    File tmpFile;
    tmpFile.open(vwd(), path, oflag);
    return tmpFile;
  }
  /** open a file
   *
   * \param[in] path location of file to be opened.
   * \param[in] oflag open flags.
   * \return a File object.
   */
  File open(const String &path, oflag_t oflag = FILE_READ) {
    return open(path.c_str(), oflag );
  }
#endif  // ENABLE_ARDUINO_FEATURES
  /** Change a volume's working directory to root
   *
   * Changes the volume's working directory to the SD's root directory.
   * Optionally set the current working directory to the volume's
   * working directory.
   *
   * \param[in] set_cwd Set the current working directory to this volume's
   *  working directory if true.
   *
   * \return The value true is returned for success and
   * the value false is returned for failure.
   */
  bool chdir(bool set_cwd = false) {
    vwd()->close();
    return vwd()->openRoot(this) && (set_cwd ? FatFile::setCwd(vwd()) : true);
  }
  /** Change a volume's working directory
   *
   * Changes the volume working directory to the \a path subdirectory.
   * Optionally set the current working directory to the volume's
   * working directory.
   *
   * Example: If the volume's working directory is "/DIR", chdir("SUB")
   * will change the volume's working directory from "/DIR" to "/DIR/SUB".
   *
   * If path is "/", the volume's working directory will be changed to the
   * root directory
   *
   * \param[in] path The name of the subdirectory.
   *
   * \param[in] set_cwd Set the current working directory to this volume's
   *  working directory if true.
   *
   * \return The value true is returned for success and
   * the value false is returned for failure.
   */
  //----------------------------------------------------------------------------
  bool chdir(const char *path, bool set_cwd = false) {
    FatFile dir;
    if (path[0] == '/' && path[1] == '\0') {
      return chdir(set_cwd);
    }
    if (!dir.open(vwd(), path, O_RDONLY)) {
      goto fail;
    }
    if (!dir.isDir()) {
      goto fail;
    }
    m_vwd = dir;
    if (set_cwd) {
      FatFile::setCwd(vwd());
    }
    return true;

fail:
    return false;
  }
  //----------------------------------------------------------------------------
  /** Set the current working directory to a volume's working directory.
   *
   * This is useful with multiple SD cards.
   *
   * The current working directory is changed to this
   * volume's working directory.
   *
   * This is like the Windows/DOS \<drive letter>: command.
   */
  void chvol() {
    FatFile::setCwd(vwd());
  }
  //----------------------------------------------------------------------------
  /**
   * Test for the existence of a file.
   *
   * \param[in] path Path of the file to be tested for.
   *
   * \return true if the file exists else false.
   */
  bool exists(const char* path) {
    return vwd()->exists(path);
  }
  //----------------------------------------------------------------------------
  /** List the directory contents of the volume working directory.
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
   * \return true for success or false if an error occurred.
   */
  bool ls(print_t* pr, uint8_t flags = 0) {
    return vwd()->ls(pr, flags);
  }
  //----------------------------------------------------------------------------
  /** List the directory contents of a directory.
   *
   * \param[in] pr Print stream for list.
   *
   * \param[in] path directory to list.
   *
   * \param[in] flags The inclusive OR of
   *
   * LS_DATE - %Print file modification date
   *
   * LS_SIZE - %Print file size.
   *
   * LS_R - Recursive list of subdirectories.
   *
   * \return true for success or false if an error occurred.
   */
  bool ls(print_t* pr, const char* path, uint8_t flags) {
    FatFile dir;
    return dir.open(vwd(), path, O_RDONLY) && dir.ls(pr, flags);
  }
  //----------------------------------------------------------------------------
  /** Make a subdirectory in the volume working directory.
   *
   * \param[in] path A path with a valid 8.3 DOS name for the subdirectory.
   *
   * \param[in] pFlag Create missing parent directories if true.
   *
   * \return The value true is returned for success and
   * the value false is returned for failure.
   */
  bool mkdir(const char* path, bool pFlag = true) {
    FatFile sub;
    return sub.mkdir(vwd(), path, pFlag);
  }
  //----------------------------------------------------------------------------
  /** Remove a file from the volume working directory.
  *
  * \param[in] path A path with a valid 8.3 DOS name for the file.
  *
  * \return The value true is returned for success and
  * the value false is returned for failure.
  */
  bool remove(const char* path) {
    return FatFile::remove(vwd(), path);
  }
  //----------------------------------------------------------------------------
  /** Rename a file or subdirectory.
   *
   * \param[in] oldPath Path name to the file or subdirectory to be renamed.
   *
   * \param[in] newPath New path name of the file or subdirectory.
   *
   * The \a newPath object must not exist before the rename call.
   *
   * The file to be renamed must not be open.  The directory entry may be
   * moved and file system corruption could occur if the file is accessed by
   * a file object that was opened before the rename() call.
   *
   * \return The value true is returned for success and
   * the value false is returned for failure.
   */
  bool rename(const char *oldPath, const char *newPath) {
    FatFile file;
    if (!file.open(vwd(), oldPath, O_RDONLY)) {
      return false;
    }
    return file.rename(vwd(), newPath);
  }
  //----------------------------------------------------------------------------
  /** Remove a subdirectory from the volume's working directory.
   *
   * \param[in] path A path with a valid 8.3 DOS name for the subdirectory.
   *
   * The subdirectory file will be removed only if it is empty.
   *
   * \return The value true is returned for success and
   * the value false is returned for failure.
   */
  bool rmdir(const char* path) {
    FatFile sub;
    if (!sub.open(vwd(), path, O_RDONLY)) {
      return false;
    }
    return sub.rmdir();
  }
  //----------------------------------------------------------------------------
  /** Truncate a file to a specified length.  The current file position
   * will be maintained if it is less than or equal to \a length otherwise
   * it will be set to end of file.
   *
   * \param[in] path A path with a valid 8.3 DOS name for the file.
   * \param[in] length The desired length for the file.
   *
   * \return The value true is returned for success and
   * the value false is returned for failure.
   */
  bool truncate(const char* path, uint32_t length) {
    FatFile file;
    if (!file.open(vwd(), path, O_WRONLY)) {
      return false;
    }
    return file.truncate(length);
  }
  /** \return a pointer to the FatVolume object. */
  FatVolume* vol() {
    return this;
  }
  /** \return a pointer to the volume working directory. */
  FatFile* vwd() {
    return &m_vwd;
  }
  /** Wipe all data from the volume. You must reinitialize the volume before
   *  accessing it again.
   * \param[in] pr print stream for status dots.
   * \return true for success else false.
   */
  bool wipe(print_t* pr = 0) {
    vwd()->close();
    return FatVolume::wipe(pr);
  }

 private:
  FatFile m_vwd;
};
#endif  // FatFileSystem_h
