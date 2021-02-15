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
#include "FatFile.h"
#include "FatFileSystem.h"
//------------------------------------------------------------------------------
bool FatFile::getSFN(char* name) {
  dir_t* dir;
  if (!isOpen()) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  if (isRoot()) {
    name[0] = '/';
    name[1] = '\0';
    return true;
  }
  // cache entry
  dir = cacheDirEntry(FatCache::CACHE_FOR_READ);
  if (!dir) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  // format name
  dirName(dir, name);
  return true;

fail:
  return false;
}
//------------------------------------------------------------------------------
size_t FatFile::printSFN(print_t* pr) {
  char name[13];
  if (!getSFN(name)) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  return pr->write(name);

fail:
  return 0;
}
#if !USE_LONG_FILE_NAMES
//------------------------------------------------------------------------------
bool FatFile::getName(char* name, size_t size) {
  return size < 13 ? 0 : getSFN(name);
}
//------------------------------------------------------------------------------
// format directory name field from a 8.3 name string
bool FatFile::parsePathName(const char* path, fname_t* fname,
                            const char** ptr) {
  uint8_t uc = 0;
  uint8_t lc = 0;
  uint8_t bit = FNAME_FLAG_LC_BASE;
  // blank fill name and extension
  for (uint8_t i = 0; i < 11; i++) {
    fname->sfn[i] = ' ';
  }

  for (uint8_t i = 0, n = 7;; path++) {
    uint8_t c = *path;
    if (c == 0 || isDirSeparator(c)) {
      // Done.
      break;
    }
    if (c == '.' && n == 7) {
      n = 10;  // max index for full 8.3 name
      i = 8;   // place for extension

      // bit for extension.
      bit = FNAME_FLAG_LC_EXT;
    } else {
      if (!legal83Char(c) || i > n) {
        DBG_FAIL_MACRO;
        goto fail;
      }
      if ('a' <= c && c <= 'z') {
        c += 'A' - 'a';
        lc |= bit;
      } else if ('A' <= c && c <= 'Z') {
        uc |= bit;
      }
      fname->sfn[i++] = c;
    }
  }
  // must have a file name, extension is optional
  if (fname->sfn[0] == ' ') {
    DBG_FAIL_MACRO;
    goto fail;
  }
  // Set base-name and extension bits.
  fname->flags = lc & uc ? 0 : lc;
  while (isDirSeparator(*path)) {
    path++;
  }
  *ptr = path;
  return true;

fail:
  return false;
}
//------------------------------------------------------------------------------
// open with filename in fname
#define SFN_OPEN_USES_CHKSUM 0
bool FatFile::open(FatFile* dirFile, fname_t* fname, oflag_t oflag) {
  bool emptyFound = false;
#if SFN_OPEN_USES_CHKSUM
  uint8_t chksum;
#endif
  uint8_t lfnOrd = 0;
  uint16_t emptyIndex;
  uint16_t index = 0;
  dir_t* dir;
  ldir_t* ldir;

  dirFile->rewind();
  while (1) {
    if (!emptyFound) {
      emptyIndex = index;
    }
    dir = dirFile->readDirCache(true);
    if (!dir) {
      if (dirFile->getError())  {
        DBG_FAIL_MACRO;
        goto fail;
      }
      // At EOF if no error.
      break;
    }
    if (dir->name[0] == DIR_NAME_FREE) {
      emptyFound = true;
      break;
    }
    if (dir->name[0] == DIR_NAME_DELETED) {
      lfnOrd = 0;
      emptyFound = true;
    } else if (DIR_IS_FILE_OR_SUBDIR(dir)) {
      if (!memcmp(fname->sfn, dir->name, 11)) {
        // don't open existing file if O_EXCL
        if (oflag & O_EXCL) {
          DBG_FAIL_MACRO;
          goto fail;
        }
#if SFN_OPEN_USES_CHKSUM
        if (lfnOrd && chksum != lfnChecksum(dir->name)) {
          DBG_FAIL_MACRO;
          goto fail;
        }
#endif  // SFN_OPEN_USES_CHKSUM
        if (!openCachedEntry(dirFile, index, oflag, lfnOrd)) {
          DBG_FAIL_MACRO;
          goto fail;
        }
        return true;
      } else {
        lfnOrd = 0;
      }
    } else if (DIR_IS_LONG_NAME(dir)) {
      ldir = reinterpret_cast<ldir_t*>(dir);
      if (ldir->ord & LDIR_ORD_LAST_LONG_ENTRY) {
        lfnOrd = ldir->ord & 0X1F;
#if SFN_OPEN_USES_CHKSUM
        chksum = ldir->chksum;
#endif  // SFN_OPEN_USES_CHKSUM
      }
    } else {
      lfnOrd = 0;
    }
    index++;
  }
  // don't create unless O_CREAT and write mode
  if (!(oflag & O_CREAT) || !isWriteMode(oflag)) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  if (emptyFound) {
    index = emptyIndex;
  } else {
    if (!dirFile->addDirCluster()) {
      DBG_FAIL_MACRO;
      goto fail;
    }
  }
  if (!dirFile->seekSet(32UL*index)) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  dir = dirFile->readDirCache();
  if (!dir) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  // initialize as empty file
  memset(dir, 0, sizeof(dir_t));
  memcpy(dir->name, fname->sfn, 11);

  // Set base-name and extension lower case bits.
  dir->reservedNT =  (DIR_NT_LC_BASE | DIR_NT_LC_EXT) & fname->flags;

  // set timestamps
  if (m_dateTime) {
    // call user date/time function
    m_dateTime(&dir->creationDate, &dir->creationTime);
  } else {
    // use default date/time
    dir->creationDate = FAT_DEFAULT_DATE;
    dir->creationTime = FAT_DEFAULT_TIME;
  }
  dir->lastAccessDate = dir->creationDate;
  dir->lastWriteDate = dir->creationDate;
  dir->lastWriteTime = dir->creationTime;

  // Force write of entry to device.
  dirFile->m_vol->cacheDirty();

  // open entry in cache.
  return openCachedEntry(dirFile, index, oflag, 0);

fail:
  return false;
}
//------------------------------------------------------------------------------
size_t FatFile::printName(print_t* pr) {
  return printSFN(pr);
}
//------------------------------------------------------------------------------
bool FatFile::remove() {
  dir_t* dir;
  // Can't remove if LFN or not open for write.
  if (!isFile() || isLFN() || !(m_flags & F_WRITE)) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  // Free any clusters.
  if (m_firstCluster && !m_vol->freeChain(m_firstCluster)) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  // Cache directory entry.
  dir = cacheDirEntry(FatCache::CACHE_FOR_WRITE);
  if (!dir) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  // Mark entry deleted.
  dir->name[0] = DIR_NAME_DELETED;

  // Set this file closed.
  m_attr = FILE_ATTR_CLOSED;

  // Write entry to device.
  return m_vol->cacheSync();

fail:
  return false;
}
#endif  // !USE_LONG_FILE_NAMES
