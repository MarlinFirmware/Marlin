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
//------------------------------------------------------------------------------
//
uint8_t FatFile::lfnChecksum(uint8_t* name) {
  uint8_t sum = 0;
  for (uint8_t i = 0; i < 11; i++) {
    sum = (((sum & 1) << 7) | ((sum & 0xfe) >> 1)) + name[i];
  }
  return sum;
}
#if USE_LONG_FILE_NAMES
//------------------------------------------------------------------------------
// Saves about 90 bytes of flash on 328 over tolower().
inline char lfnToLower(char c) {
  return 'A' <= c && c <= 'Z' ? c + 'a' - 'A' : c;
}
//------------------------------------------------------------------------------
// Daniel Bernstein University of Illinois at Chicago.
// Original had + instead of ^
static uint16_t Bernstein(uint16_t hash, const char *str, size_t len) {
  for (size_t i = 0; i < len; i++) {
    // hash = hash * 33 ^ str[i];
    hash = ((hash << 5) + hash) ^ str[i];
  }
  return hash;
}
//------------------------------------------------------------------------------
/**
 * Fetch a 16-bit long file name character.
 *
 * \param[in] ldir Pointer to long file name directory entry.
 * \param[in] i Index of character.
 * \return The 16-bit character.
 */
static uint16_t lfnGetChar(ldir_t *ldir, uint8_t i) {
  if (i < LDIR_NAME1_DIM) {
    return ldir->name1[i];
  } else if (i < (LDIR_NAME1_DIM + LDIR_NAME2_DIM)) {
    return ldir->name2[i - LDIR_NAME1_DIM];
  } else if (i < (LDIR_NAME1_DIM + LDIR_NAME2_DIM + LDIR_NAME2_DIM)) {
    return ldir->name3[i - LDIR_NAME1_DIM - LDIR_NAME2_DIM];
  }
  return 0;
}
//------------------------------------------------------------------------------
static bool lfnGetName(ldir_t *ldir, char* name, size_t n) {
  uint8_t i;
  size_t k = 13*((ldir->ord & 0X1F) - 1);
  for (i = 0; i < 13; i++) {
    uint16_t c = lfnGetChar(ldir, i);
    if (c == 0 || k >= n) {
      break;
    }
    name[k++] = c >= 0X7F ? '?' : c;
  }
  // Terminate with zero byte if name fits.
  if (k < n && (ldir->ord & LDIR_ORD_LAST_LONG_ENTRY)) {
    name[k] = 0;
  }
  // Truncate if name is too long.
  name[n - 1] = 0;
  return true;
}
//------------------------------------------------------------------------------
inline bool lfnLegalChar(char c) {
  if (c == '/' || c == '\\' || c == '"' || c == '*' ||
      c == ':' || c == '<' || c == '>' || c == '?' || c == '|') {
    return false;
  }
  return 0X1F < c && c < 0X7F;
}
//------------------------------------------------------------------------------
/**
 * Store a 16-bit long file name character.
 *
 * \param[in] ldir Pointer to long file name directory entry.
 * \param[in] i Index of character.
 * \param[in] c  The 16-bit character.
 */
static void lfnPutChar(ldir_t *ldir, uint8_t i, uint16_t c) {
  if (i < LDIR_NAME1_DIM) {
    ldir->name1[i] = c;
  } else if (i < (LDIR_NAME1_DIM + LDIR_NAME2_DIM)) {
    ldir->name2[i - LDIR_NAME1_DIM] = c;
  } else if (i < (LDIR_NAME1_DIM + LDIR_NAME2_DIM + LDIR_NAME2_DIM)) {
    ldir->name3[i - LDIR_NAME1_DIM - LDIR_NAME2_DIM] = c;
  }
}
//------------------------------------------------------------------------------
static void lfnPutName(ldir_t *ldir, const char* name, size_t n) {
  size_t k = 13*((ldir->ord & 0X1F) - 1);
  for (uint8_t i = 0; i < 13; i++, k++) {
    uint16_t c = k < n ? name[k] : k == n ? 0 : 0XFFFF;
    lfnPutChar(ldir, i, c);
  }
}
//==============================================================================
bool FatFile::getName(char* name, size_t size) {
  FatFile dirFile;
  ldir_t* ldir;
  if (!isOpen() || size < 13) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  if (!isLFN()) {
    return getSFN(name);
  }
  if (!dirFile.openCluster(this)) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  for (uint8_t ord = 1; ord <= m_lfnOrd; ord++) {
    if (!dirFile.seekSet(32UL*(m_dirIndex - ord))) {
      DBG_FAIL_MACRO;
      goto fail;
    }
    ldir = reinterpret_cast<ldir_t*>(dirFile.readDirCache());
    if (!ldir) {
      DBG_FAIL_MACRO;
      goto fail;
    }
    if (ldir->attr != DIR_ATT_LONG_NAME) {
      DBG_FAIL_MACRO;
      goto fail;
    }
    if (ord != (ldir->ord & 0X1F)) {
      DBG_FAIL_MACRO;
      goto fail;
    }
    if (!lfnGetName(ldir, name, size)) {
      DBG_FAIL_MACRO;
      goto fail;
    }
    if (ldir->ord & LDIR_ORD_LAST_LONG_ENTRY) {
      return true;
    }
  }
  // Fall into fail.
  DBG_FAIL_MACRO;

fail:
  name[0] = 0;
  return false;
}
//------------------------------------------------------------------------------
bool FatFile::openCluster(FatFile* file) {
  if (file->m_dirCluster == 0) {
    return openRoot(file->m_vol);
  }
  memset(this, 0, sizeof(FatFile));
  m_attr = FILE_ATTR_SUBDIR;
  m_flags = F_READ;
  m_vol = file->m_vol;
  m_firstCluster = file->m_dirCluster;
  return true;
}
//------------------------------------------------------------------------------
bool FatFile::parsePathName(const char* path,
                            fname_t* fname, const char** ptr) {
  char c;
  bool is83;
  uint8_t bit = DIR_NT_LC_BASE;
  uint8_t lc = 0;
  uint8_t uc = 0;
  uint8_t i = 0;
  uint8_t in = 7;
  int end;
  int len = 0;
  int si;
  int dot;

  // Skip leading spaces.
  while (*path == ' ') {
    path++;
  }
  fname->lfn = path;

  for (len = 0; ; len++) {
    c = path[len];
    if (c == 0 || isDirSeparator(c)) {
      break;
    }
    if (!lfnLegalChar(c)) {
      return false;
    }
  }
  // Advance to next path component.
  for (end = len; path[end] ==  ' ' || isDirSeparator(path[end]); end++) {}
  *ptr = &path[end];

  // Back over spaces and dots.
  while (len) {
    c = path[len - 1];
    if (c != '.' && c != ' ') {
      break;
    }
    len--;
  }
  // Max length of LFN is 255.
  if (len > 255) {
    return false;
  }
  fname->len = len;
  // Blank file short name.
  for (uint8_t k = 0; k < 11; k++) {
    fname->sfn[k] = ' ';
  }
  // skip leading spaces and dots.
  for (si = 0; path[si] == '.' || path[si] == ' '; si++) {}
  // Not 8.3 if leading dot or space.
  is83 = !si;

  // find last dot.
  for (dot = len - 1; dot >= 0 && path[dot] != '.'; dot--) {}
  for (; si < len; si++) {
    c = path[si];
    if (c == ' ' || (c == '.' && dot != si)) {
      is83 = false;
      continue;
    }
    if (!legal83Char(c) && si != dot) {
      is83 = false;
      c = '_';
    }
    if (si == dot || i > in) {
      if (in == 10) {
        // Done - extension longer than three characters.
        is83 = false;
        break;
      }
      if (si != dot) {
        is83 = false;
      }
      // Break if no dot and base-name is longer than eight characters.
      if (si > dot) {
        break;
      }
      si = dot;
      in = 10;  // Max index for full 8.3 name.
      i = 8;    // Place for extension.
      bit = DIR_NT_LC_EXT;  // bit for extension.
    } else {
      if ('a' <= c && c <= 'z') {
        c += 'A' - 'a';
        lc |= bit;
      } else if ('A' <= c && c <= 'Z') {
        uc |= bit;
      }
      fname->sfn[i++] = c;
      if (i < 7) {
        fname->seqPos = i;
      }
    }
  }
  if (fname->sfn[0] == ' ') {
    return false;
  }

  if (is83) {
    fname->flags = lc & uc ? FNAME_FLAG_MIXED_CASE : lc;
  } else {
    fname->flags = FNAME_FLAG_LOST_CHARS;
    fname->sfn[fname->seqPos] = '~';
    fname->sfn[fname->seqPos + 1] = '1';
  }
  return true;
}
//------------------------------------------------------------------------------
bool FatFile::open(FatFile* dirFile, fname_t* fname, oflag_t oflag) {
  bool fnameFound = false;
  uint8_t lfnOrd = 0;
  uint8_t freeNeed;
  uint8_t freeFound = 0;
  uint8_t ord = 0;
  uint8_t chksum = 0;
  uint16_t freeIndex = 0;
  uint16_t curIndex;
  dir_t* dir;
  ldir_t* ldir;
  size_t len = fname->len;

  if (!dirFile->isDir() || isOpen()) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  // Number of directory entries needed.
  freeNeed = fname->flags & FNAME_FLAG_NEED_LFN ? 1 + (len + 12)/13 : 1;

  dirFile->rewind();
  while (1) {
    curIndex = dirFile->m_curPosition/32;
    dir = dirFile->readDirCache(true);
    if (!dir) {
      if (dirFile->getError()) {
        DBG_FAIL_MACRO;
        goto fail;
      }
      // At EOF
      goto create;
    }
    if (dir->name[0] == DIR_NAME_DELETED || dir->name[0] == DIR_NAME_FREE) {
      if (freeFound == 0) {
        freeIndex = curIndex;
      }
      if (freeFound < freeNeed) {
        freeFound++;
      }
      if (dir->name[0] == DIR_NAME_FREE) {
        goto create;
      }
    } else {
      if (freeFound < freeNeed) {
        freeFound = 0;
      }
    }
    // skip empty slot or '.' or '..'
    if (dir->name[0] == DIR_NAME_DELETED || dir->name[0] == '.') {
      lfnOrd = 0;
    } else if (DIR_IS_LONG_NAME(dir)) {
      ldir_t *ldir = reinterpret_cast<ldir_t*>(dir);
      if (!lfnOrd) {
        if ((ldir->ord & LDIR_ORD_LAST_LONG_ENTRY) == 0) {
          continue;
        }
        lfnOrd = ord = ldir->ord & 0X1F;
        chksum = ldir->chksum;
      } else if (ldir->ord != --ord || chksum != ldir->chksum) {
        lfnOrd = 0;
        continue;
      }
      size_t k = 13*(ord - 1);
      if (k >= len) {
        // Not found.
        lfnOrd = 0;
        continue;
      }
      for (uint8_t i = 0; i < 13; i++) {
        uint16_t u = lfnGetChar(ldir, i);
        if (k == len) {
          if (u != 0) {
            // Not found.
            lfnOrd = 0;
          }
          break;
        }
        if (u > 255 || lfnToLower(u) != lfnToLower(fname->lfn[k++])) {
          // Not found.
          lfnOrd = 0;
          break;
        }
      }
    } else if (DIR_IS_FILE_OR_SUBDIR(dir)) {
      if (lfnOrd) {
        if (1 == ord && lfnChecksum(dir->name) == chksum) {
          goto found;
        }
        DBG_FAIL_MACRO;
        goto fail;
      }
      if (!memcmp(dir->name, fname->sfn, sizeof(fname->sfn))) {
        if (!(fname->flags & FNAME_FLAG_LOST_CHARS)) {
          goto found;
        }
        fnameFound = true;
      }
    } else {
      lfnOrd = 0;
    }
  }

found:
  // Don't open if create only.
  if (oflag & O_EXCL) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  goto open;

create:
  // don't create unless O_CREAT and write mode.
  if (!(oflag & O_CREAT) || !isWriteMode(oflag)) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  // If at EOF start in next cluster.
  if (freeFound == 0) {
    freeIndex = curIndex;
  }

  while (freeFound < freeNeed) {
    dir = dirFile->readDirCache();
    if (!dir) {
      if (dirFile->getError()) {
        DBG_FAIL_MACRO;
        goto fail;
      }
      // EOF if no error.
      break;
    }
    freeFound++;
  }
  while (freeFound < freeNeed) {
    // Will fail if FAT16 root.
    if (!dirFile->addDirCluster()) {
      DBG_FAIL_MACRO;
      goto fail;
    }
    // Done if more than one block per cluster.  Max freeNeed is 21.
    if (dirFile->m_vol->blocksPerCluster() > 1) {
      break;
    }
    freeFound += 16;
  }
  if (fnameFound) {
    if (!dirFile->lfnUniqueSfn(fname)) {
      goto fail;
    }
  }
  if (!dirFile->seekSet(32UL*freeIndex)) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  lfnOrd = freeNeed - 1;
  for (uint8_t ord = lfnOrd ; ord ; ord--) {
    ldir = reinterpret_cast<ldir_t*>(dirFile->readDirCache());
    if (!ldir) {
      DBG_FAIL_MACRO;
      goto fail;
    }
    dirFile->m_vol->cacheDirty();
    ldir->ord = ord == lfnOrd ? LDIR_ORD_LAST_LONG_ENTRY | ord : ord;
    ldir->attr = DIR_ATT_LONG_NAME;
    ldir->type = 0;
    ldir->chksum = lfnChecksum(fname->sfn);
    ldir->mustBeZero = 0;
    lfnPutName(ldir, fname->lfn, len);
  }
  curIndex = dirFile->m_curPosition/32;
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

open:
  // open entry in cache.
  if (!openCachedEntry(dirFile, curIndex, oflag, lfnOrd)) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  return true;

fail:
  return false;
}
//------------------------------------------------------------------------------
size_t FatFile::printName(print_t* pr) {
  FatFile dirFile;
  uint16_t u;
  size_t n = 0;
  ldir_t* ldir;

  if (!isLFN()) {
    return printSFN(pr);
  }
  if (!dirFile.openCluster(this)) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  for (uint8_t ord = 1; ord <= m_lfnOrd; ord++) {
    if (!dirFile.seekSet(32UL*(m_dirIndex - ord))) {
      DBG_FAIL_MACRO;
      goto fail;
    }
    ldir = reinterpret_cast<ldir_t*>(dirFile.readDirCache());
    if (!ldir) {
      DBG_FAIL_MACRO;
      goto fail;
    }
    if (ldir->attr != DIR_ATT_LONG_NAME ||
        ord != (ldir->ord & 0X1F)) {
      DBG_FAIL_MACRO;
      goto fail;
    }
    for (uint8_t i = 0; i < 13; i++) {
      u = lfnGetChar(ldir, i);
      if (u == 0) {
        // End of name.
        break;
      }
      if (u > 0X7E) {
        u = '?';
      }
      pr->write(static_cast<char>(u));
      n++;
    }
    if (ldir->ord & LDIR_ORD_LAST_LONG_ENTRY) {
      return n;
    }
  }
  // Fall into fail;
  DBG_FAIL_MACRO;

fail:
  return 0;
}
//------------------------------------------------------------------------------
bool FatFile::remove() {
  bool last;
  uint8_t chksum;
  uint8_t ord;
  FatFile dirFile;
  dir_t* dir;
  ldir_t* ldir;

  // Cant' remove not open for write.
  if (!isFile() || !(m_flags & F_WRITE)) {
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
  chksum = lfnChecksum(dir->name);

  // Mark entry deleted.
  dir->name[0] = DIR_NAME_DELETED;

  // Set this file closed.
  m_attr = FILE_ATTR_CLOSED;

  // Write entry to device.
  if (!m_vol->cacheSync()) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  if (!isLFN()) {
    // Done, no LFN entries.
    return true;
  }
  if (!dirFile.openCluster(this)) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  for (ord = 1; ord <= m_lfnOrd; ord++) {
    if (!dirFile.seekSet(32UL*(m_dirIndex - ord))) {
      DBG_FAIL_MACRO;
      goto fail;
    }
    ldir = reinterpret_cast<ldir_t*>(dirFile.readDirCache());
    if (!ldir) {
      DBG_FAIL_MACRO;
      goto fail;
    }
    if (ldir->attr != DIR_ATT_LONG_NAME ||
        ord != (ldir->ord & 0X1F) ||
        chksum != ldir->chksum) {
      DBG_FAIL_MACRO;
      goto fail;
    }
    last = ldir->ord & LDIR_ORD_LAST_LONG_ENTRY;
    ldir->ord = DIR_NAME_DELETED;
    m_vol->cacheDirty();
    if (last) {
      if (!m_vol->cacheSync()) {
        DBG_FAIL_MACRO;
        goto fail;
      }
      return true;
    }
  }
  // Fall into fail.
  DBG_FAIL_MACRO;

fail:
  return false;
}
//------------------------------------------------------------------------------
bool FatFile::lfnUniqueSfn(fname_t* fname) {
  const uint8_t FIRST_HASH_SEQ = 2;  // min value is 2
  uint8_t pos = fname->seqPos;;
  dir_t *dir;
  uint16_t hex;

  DBG_HALT_IF(!(fname->flags & FNAME_FLAG_LOST_CHARS));
  DBG_HALT_IF(fname->sfn[pos] != '~' && fname->sfn[pos + 1] != '1');

  for (uint8_t seq = 2; seq < 100; seq++) {
    if (seq < FIRST_HASH_SEQ) {
      fname->sfn[pos + 1] = '0' + seq;
    } else {
      DBG_PRINT_IF(seq > FIRST_HASH_SEQ);
      hex = Bernstein(seq + fname->len, fname->lfn, fname->len);
      if (pos > 3) {
        // Make space in name for ~HHHH.
        pos = 3;
      }
      for (uint8_t i = pos + 4 ; i > pos; i--) {
        uint8_t h = hex & 0XF;
        fname->sfn[i] = h < 10 ? h + '0' : h + 'A' - 10;
        hex >>= 4;
      }
    }
    fname->sfn[pos] = '~';
    rewind();
    while (1) {
      dir = readDirCache(true);
      if (!dir) {
        if (!getError()) {
          // At EOF and name not found if no error.
          goto done;
        }
        DBG_FAIL_MACRO;
        goto fail;
      }
      if (dir->name[0] == DIR_NAME_FREE) {
        goto done;
      }
      if (DIR_IS_FILE_OR_SUBDIR(dir) && !memcmp(fname->sfn, dir->name, 11)) {
        // Name found - try another.
        break;
      }
    }
  }
  // fall inti fail - too many tries.
  DBG_FAIL_MACRO;

fail:
  return false;

done:
  return true;
}
#endif  // #if USE_LONG_FILE_NAMES
