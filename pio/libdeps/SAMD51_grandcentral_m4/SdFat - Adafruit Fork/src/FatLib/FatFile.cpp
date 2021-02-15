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
// Pointer to cwd directory.
FatFile* FatFile::m_cwd = 0;
// Callback function for date/time.
void (*FatFile::m_dateTime)(uint16_t* date, uint16_t* time) = 0;
//------------------------------------------------------------------------------
// Add a cluster to a file.
bool FatFile::addCluster() {
  m_flags |= F_FILE_DIR_DIRTY;
  return m_vol->allocateCluster(m_curCluster, &m_curCluster);
}
//------------------------------------------------------------------------------
// Add a cluster to a directory file and zero the cluster.
// Return with first block of cluster in the cache.
bool FatFile::addDirCluster() {
  uint32_t block;
  cache_t* pc;

  if (isRootFixed()) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  // max folder size
  if (m_curPosition >= 512UL*4095) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  if (!addCluster()) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  block = m_vol->clusterFirstBlock(m_curCluster);
  pc = m_vol->cacheFetchData(block, FatCache::CACHE_RESERVE_FOR_WRITE);
  if (!pc) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  memset(pc, 0, 512);
  // zero rest of clusters
  for (uint8_t i = 1; i < m_vol->blocksPerCluster(); i++) {
    if (!m_vol->writeBlock(block + i, pc->data)) {
      DBG_FAIL_MACRO;
      goto fail;
    }
  }
  // Set position to EOF to avoid inconsistent curCluster/curPosition.
  m_curPosition += 512UL*m_vol->blocksPerCluster();
  return true;

fail:
  return false;
}
//------------------------------------------------------------------------------
// cache a file's directory entry
// return pointer to cached entry or null for failure
dir_t* FatFile::cacheDirEntry(uint8_t action) {
  cache_t* pc;
  pc = m_vol->cacheFetchData(m_dirBlock, action);
  if (!pc) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  return pc->dir + (m_dirIndex & 0XF);

fail:
  return 0;
}
//------------------------------------------------------------------------------
bool FatFile::close() {
  bool rtn = sync();
  m_attr = FILE_ATTR_CLOSED;
  return rtn;
}
//------------------------------------------------------------------------------
bool FatFile::contiguousRange(uint32_t* bgnBlock, uint32_t* endBlock) {
  // error if no blocks
  if (m_firstCluster == 0) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  for (uint32_t c = m_firstCluster; ; c++) {
    uint32_t next;
    int8_t fg = m_vol->fatGet(c, &next);
    if (fg < 0) {
      DBG_FAIL_MACRO;
      goto fail;
    }
    // check for contiguous
    if (fg == 0 || next != (c + 1)) {
      // error if not end of chain
      if (fg) {
        DBG_FAIL_MACRO;
        goto fail;
      }
      *bgnBlock = m_vol->clusterFirstBlock(m_firstCluster);
      *endBlock = m_vol->clusterFirstBlock(c)
                  + m_vol->blocksPerCluster() - 1;
      return true;
    }
  }

fail:
  return false;
}
//------------------------------------------------------------------------------
bool FatFile::createContiguous(FatFile* dirFile, const char* path,
                               uint32_t size, uint32_t startCluster) {
  uint32_t count;

  // don't allow zero length file
  if (size == 0) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  if (!open(dirFile, path, O_RDWR | O_CREAT | O_EXCL)) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  // calculate number of clusters needed
  count = ((size - 1) >> (m_vol->clusterSizeShift() + 9)) + 1;

  // allocate clusters
  if (!m_vol->allocContiguous(count, &m_firstCluster, startCluster)) {
    remove();
    DBG_FAIL_MACRO;
    goto fail;
  }
  m_fileSize = size;

  // insure sync() will update dir entry
  m_flags |= F_FILE_DIR_DIRTY;
  return sync();

fail:
  return false;
}
//------------------------------------------------------------------------------
bool FatFile::dirEntry(dir_t* dst) {
  dir_t* dir;
  // Make sure fields on device are correct.
  if (!sync()) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  // read entry
  dir = cacheDirEntry(FatCache::CACHE_FOR_READ);
  if (!dir) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  // copy to caller's struct
  memcpy(dst, dir, sizeof(dir_t));
  return true;

fail:
  return false;
}
//------------------------------------------------------------------------------
uint8_t FatFile::dirName(const dir_t* dir, char* name) {
  uint8_t j = 0;
  uint8_t lcBit = DIR_NT_LC_BASE;
  for (uint8_t i = 0; i < 11; i++) {
    if (dir->name[i] == ' ') {
      continue;
    }
    if (i == 8) {
      // Position bit for extension.
      lcBit = DIR_NT_LC_EXT;
      name[j++] = '.';
    }
    char c = dir->name[i];
    if ('A' <= c && c <= 'Z' && (lcBit & dir->reservedNT)) {
      c += 'a' - 'A';
    }
    name[j++] = c;
  }
  name[j] = 0;
  return j;
}

//------------------------------------------------------------------------------
uint32_t FatFile::dirSize() {
  int8_t fg;
  if (!isDir()) {
    return 0;
  }
  if (isRootFixed()) {
    return 32*m_vol->rootDirEntryCount();
  }
  uint16_t n = 0;
  uint32_t c = isRoot32() ? m_vol->rootDirStart() : m_firstCluster;
  do {
    fg = m_vol->fatGet(c, &c);
    if (fg < 0 || n > 4095) {
      return 0;
    }
    n += m_vol->blocksPerCluster();
  } while (fg);
  return 512UL*n;
}
//------------------------------------------------------------------------------
int16_t FatFile::fgets(char* str, int16_t num, char* delim) {
  char ch;
  int16_t n = 0;
  int16_t r = -1;
  while ((n + 1) < num && (r = read(&ch, 1)) == 1) {
    // delete CR
    if (ch == '\r') {
      continue;
    }
    str[n++] = ch;
    if (!delim) {
      if (ch == '\n') {
        break;
      }
    } else {
      if (strchr(delim, ch)) {
        break;
      }
    }
  }
  if (r < 0) {
    // read error
    return -1;
  }
  str[n] = '\0';
  return n;
}
//------------------------------------------------------------------------------
void FatFile::getpos(FatPos_t* pos) {
  pos->position = m_curPosition;
  pos->cluster = m_curCluster;
}
//------------------------------------------------------------------------------
bool FatFile::mkdir(FatFile* parent, const char* path, bool pFlag) {
  fname_t fname;
  FatFile tmpDir;

  if (isOpen() || !parent->isDir()) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  if (isDirSeparator(*path)) {
    while (isDirSeparator(*path)) {
      path++;
    }
    if (!tmpDir.openRoot(parent->m_vol)) {
      DBG_FAIL_MACRO;
      goto fail;
    }
    parent = &tmpDir;
  }
  while (1) {
    if (!parsePathName(path, &fname, &path)) {
      DBG_FAIL_MACRO;
      goto fail;
    }
    if (!*path) {
      break;
    }
    if (!open(parent, &fname, O_RDONLY)) {
      if (!pFlag || !mkdir(parent, &fname)) {
        DBG_FAIL_MACRO;
        goto fail;
      }
    }
    tmpDir = *this;
    parent = &tmpDir;
    close();
  }
  return mkdir(parent, &fname);

fail:
  return false;
}
//------------------------------------------------------------------------------
bool FatFile::mkdir(FatFile* parent, fname_t* fname) {
  uint32_t block;
  dir_t dot;
  dir_t* dir;
  cache_t* pc;

  if (!parent->isDir()) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  // create a normal file
  if (!open(parent, fname, O_RDWR | O_CREAT | O_EXCL)) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  // convert file to directory
  m_flags = F_READ;
  m_attr = FILE_ATTR_SUBDIR;

  // allocate and zero first cluster
  if (!addDirCluster()) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  m_firstCluster = m_curCluster;
  // Set to start of dir
  rewind();
  // force entry to device
  if (!sync()) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  // cache entry - should already be in cache due to sync() call
  dir = cacheDirEntry(FatCache::CACHE_FOR_WRITE);
  if (!dir) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  // change directory entry  attribute
  dir->attributes = DIR_ATT_DIRECTORY;

  // make entry for '.'
  memcpy(&dot, dir, sizeof(dot));
  dot.name[0] = '.';
  for (uint8_t i = 1; i < 11; i++) {
    dot.name[i] = ' ';
  }

  // cache block for '.'  and '..'
  block = m_vol->clusterFirstBlock(m_firstCluster);
  pc = m_vol->cacheFetchData(block, FatCache::CACHE_FOR_WRITE);
  if (!pc) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  // copy '.' to block
  memcpy(&pc->dir[0], &dot, sizeof(dot));
  // make entry for '..'
  dot.name[1] = '.';
  dot.firstClusterLow = parent->m_firstCluster & 0XFFFF;
  dot.firstClusterHigh = parent->m_firstCluster >> 16;
  // copy '..' to block
  memcpy(&pc->dir[1], &dot, sizeof(dot));
  // write first block
  return m_vol->cacheSync();

fail:
  return false;
}
//------------------------------------------------------------------------------
bool FatFile::open(FatFileSystem* fs, const char* path, oflag_t oflag) {
  return open(fs->vwd(), path, oflag);
}
//------------------------------------------------------------------------------
bool FatFile::open(FatFile* dirFile, const char* path, oflag_t oflag) {
  FatFile tmpDir;
  fname_t fname;

  // error if already open
  if (isOpen() || !dirFile->isDir()) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  if (isDirSeparator(*path)) {
    while (isDirSeparator(*path)) {
      path++;
    }
    if (*path == 0) {
      return openRoot(dirFile->m_vol);
    }
    if (!tmpDir.openRoot(dirFile->m_vol)) {
      DBG_FAIL_MACRO;
      goto fail;
    }
    dirFile = &tmpDir;
  }
  while (1) {
    if (!parsePathName(path, &fname, &path)) {
      DBG_FAIL_MACRO;
      goto fail;
    }
    if (*path == 0) {
      break;
    }
    if (!open(dirFile, &fname, O_RDONLY)) {
      DBG_FAIL_MACRO;
      goto fail;
    }
    tmpDir = *this;
    dirFile = &tmpDir;
    close();
  }
  return open(dirFile, &fname, oflag);

fail:
  return false;
}
//------------------------------------------------------------------------------
bool FatFile::open(FatFile* dirFile, uint16_t index, oflag_t oflag) {
  uint8_t chksum = 0;
  uint8_t lfnOrd = 0;
  dir_t* dir;
  ldir_t*ldir;

  // Error if already open.
  if (isOpen() || !dirFile->isDir()) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  // Don't open existing file if O_EXCL - user call error.
  if (oflag & O_EXCL) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  if (index) {
    // Check for LFN.
    if (!dirFile->seekSet(32UL*(index -1))) {
      DBG_FAIL_MACRO;
      goto fail;
    }
    ldir = reinterpret_cast<ldir_t*>(dirFile->readDirCache());
    if (!ldir) {
      DBG_FAIL_MACRO;
      goto fail;
    }
    if (ldir->attr == DIR_ATT_LONG_NAME) {
      if (1 == (ldir->ord & 0X1F)) {
        chksum = ldir->chksum;
        // Use largest possible number.
        lfnOrd = index > 20 ? 20 : index;
      }
    }
  } else {
    dirFile->rewind();
  }
  // read entry into cache
  dir = dirFile->readDirCache();
  if (!dir) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  // error if empty slot or '.' or '..'
  if (dir->name[0] == DIR_NAME_DELETED ||
      dir->name[0] == DIR_NAME_FREE ||
      dir->name[0] == '.') {
    DBG_FAIL_MACRO;
    goto fail;
  }
  if (lfnOrd && chksum != lfnChecksum(dir->name)) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  // open cached entry
  if (!openCachedEntry(dirFile, index, oflag, lfnOrd)) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  return true;

fail:
  return false;
}
//------------------------------------------------------------------------------
// open a cached directory entry.

bool FatFile::openCachedEntry(FatFile* dirFile, uint16_t dirIndex,
                              oflag_t oflag, uint8_t lfnOrd) {
  uint32_t firstCluster;
  memset(this, 0, sizeof(FatFile));
  // location of entry in cache
  m_vol = dirFile->m_vol;
  m_dirIndex = dirIndex;
  m_dirCluster = dirFile->m_firstCluster;
  dir_t* dir = &m_vol->cacheAddress()->dir[0XF & dirIndex];

  // Must be file or subdirectory.
  if (!DIR_IS_FILE_OR_SUBDIR(dir)) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  m_attr = dir->attributes & FILE_ATTR_COPY;
  if (DIR_IS_FILE(dir)) {
    m_attr |= FILE_ATTR_FILE;
  }
  m_lfnOrd = lfnOrd;

  switch (oflag & O_ACCMODE) {
    case O_RDONLY:
      if (oflag & O_TRUNC) {
        DBG_FAIL_MACRO;
        goto fail;
      }
      m_flags = F_READ;
      break;

    case O_RDWR:
      m_flags = F_READ | F_WRITE;
      break;

    case O_WRONLY:
      m_flags = F_WRITE;
      break;

    default:
      DBG_FAIL_MACRO;
      goto fail;
  }

  if (m_flags & F_WRITE) {
    if (isSubDir() || isReadOnly()) {
      DBG_FAIL_MACRO;
      goto fail;
    }
  }

  m_flags |= (oflag & O_APPEND ? F_APPEND : 0) | (oflag & O_SYNC ? F_SYNC : 0);

  m_dirBlock = m_vol->cacheBlockNumber();

  // copy first cluster number for directory fields
  firstCluster = ((uint32_t)dir->firstClusterHigh << 16)
                 | dir->firstClusterLow;

  if (oflag & O_TRUNC) {
    if (firstCluster && !m_vol->freeChain(firstCluster)) {
      DBG_FAIL_MACRO;
      goto fail;
    }
    // need to update directory entry
    m_flags |= F_FILE_DIR_DIRTY;
  } else {
    m_firstCluster = firstCluster;
    m_fileSize = dir->fileSize;
  }
  if ((oflag & O_AT_END) && !seekSet(m_fileSize)) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  return true;

fail:
  m_attr = FILE_ATTR_CLOSED;
  return false;
}
//------------------------------------------------------------------------------
bool FatFile::openCwd() {
  if (!cwd()) {
    DBG_FAIL_MACRO;
    return false;
  }
  *this = *cwd();
  rewind();
  return true;
}
//------------------------------------------------------------------------------
bool FatFile::openNext(FatFile* dirFile, oflag_t oflag) {
  uint8_t chksum = 0;
  ldir_t* ldir;
  uint8_t lfnOrd = 0;
  uint16_t index;

  // Check for not open and valid directory..
  if (isOpen() || !dirFile->isDir() || (dirFile->curPosition() & 0X1F)) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  while (1) {
    // read entry into cache
    index = dirFile->curPosition()/32;
    dir_t* dir = dirFile->readDirCache();
    if (!dir) {
      if (dirFile->getError()) {
        DBG_FAIL_MACRO;
      }
      goto fail;
    }
    // done if last entry
    if (dir->name[0] == DIR_NAME_FREE) {
      goto fail;
    }
    // skip empty slot or '.' or '..'
    if (dir->name[0] == '.' || dir->name[0] == DIR_NAME_DELETED) {
      lfnOrd = 0;
    } else if (DIR_IS_FILE_OR_SUBDIR(dir)) {
      if (lfnOrd && chksum != lfnChecksum(dir->name)) {
        DBG_FAIL_MACRO;
        goto fail;
      }
      if (!openCachedEntry(dirFile, index, oflag, lfnOrd)) {
        DBG_FAIL_MACRO;
        goto fail;
      }
      return true;
    } else if (DIR_IS_LONG_NAME(dir)) {
      ldir = reinterpret_cast<ldir_t*>(dir);
      if (ldir->ord & LDIR_ORD_LAST_LONG_ENTRY) {
        lfnOrd = ldir->ord & 0X1F;
        chksum = ldir->chksum;
      }
    } else {
      lfnOrd = 0;
    }
  }

fail:
  return false;
}
#ifndef DOXYGEN_SHOULD_SKIP_THIS
//------------------------------------------------------------------------------
/** Open a file's parent directory.
 *
 * \param[in] file Parent of this directory will be opened.  Must not be root.
 *
 * \return The value true is returned for success and
 * the value false is returned for failure.
 */
bool FatFile::openParent(FatFile* dirFile) {
  FatFile dotdot;
  uint32_t lbn;
  dir_t* dir;
  uint32_t ddc;
  cache_t* cb;

  if (isOpen() || !dirFile->isOpen()) {
    goto fail;
  }
  if (dirFile->m_dirCluster == 0) {
    return openRoot(dirFile->m_vol);
  }
  lbn = dirFile->m_vol->clusterFirstBlock(dirFile->m_dirCluster);
  cb = dirFile->m_vol->cacheFetchData(lbn, FatCache::CACHE_FOR_READ);
  if (!cb) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  // Point to dir entery for ..
  dir = cb->dir + 1;
  ddc = dir->firstClusterLow | ((uint32_t)dir->firstClusterHigh << 16);
  if (ddc == 0) {
    if (!dotdot.openRoot(dirFile->m_vol)) {
      DBG_FAIL_MACRO;
      goto fail;
    }
  } else {
    memset(&dotdot, 0, sizeof(FatFile));
    dotdot.m_attr = FILE_ATTR_SUBDIR;
    dotdot.m_flags = F_READ;
    dotdot.m_vol = dirFile->m_vol;
    dotdot.m_firstCluster = ddc;
  }
  uint32_t di;
  do {
    di = dotdot.curPosition()/32;
    dir = dotdot.readDirCache();
    if (!dir) {
      DBG_FAIL_MACRO;
      goto fail;
    }
    ddc = dir->firstClusterLow | ((uint32_t)dir->firstClusterHigh << 16);
  } while (ddc != dirFile->m_dirCluster);
  return open(&dotdot, di, O_RDONLY);

fail:
  return false;
}
#endif  // DOXYGEN_SHOULD_SKIP_THIS
//------------------------------------------------------------------------------
bool FatFile::openRoot(FatVolume* vol) {
  // error if file is already open
  if (isOpen()) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  memset(this, 0, sizeof(FatFile));

  m_vol = vol;
  switch (vol->fatType()) {
#if FAT12_SUPPORT
  case 12:
#endif  // FAT12_SUPPORT
  case 16:
    m_attr = FILE_ATTR_ROOT_FIXED;
    break;

  case 32:
    m_attr = FILE_ATTR_ROOT32;
    break;

  default:
    DBG_FAIL_MACRO;
    goto fail;
  }
  // read only
  m_flags = F_READ;
  return true;

fail:
  return false;
}
//------------------------------------------------------------------------------
int FatFile::peek() {
  FatPos_t pos;
  getpos(&pos);
  int c = read();
  if (c >= 0) {
    setpos(&pos);
  }
  return c;
}
//------------------------------------------------------------------------------
int FatFile::read(void* buf, size_t nbyte) {
  int8_t fg;
  uint8_t blockOfCluster = 0;
  uint8_t* dst = reinterpret_cast<uint8_t*>(buf);
  uint16_t offset;
  size_t toRead;
  uint32_t block;  // raw device block number
  cache_t* pc;

  // error if not open for read
  if (!isOpen() || !(m_flags & F_READ)) {
    DBG_FAIL_MACRO;
    goto fail;
  }

  if (isFile()) {
    uint32_t tmp32 = m_fileSize - m_curPosition;
    if (nbyte >= tmp32) {
      nbyte = tmp32;
    }
  } else if (isRootFixed()) {
    uint16_t tmp16 = 32*m_vol->m_rootDirEntryCount - (uint16_t)m_curPosition;
    if (nbyte > tmp16) {
      nbyte = tmp16;
    }
  }
  toRead = nbyte;
  while (toRead) {
    size_t n;
    offset = m_curPosition & 0X1FF;  // offset in block
    if (isRootFixed()) {
      block = m_vol->rootDirStart() + (m_curPosition >> 9);
    } else {
      blockOfCluster = m_vol->blockOfCluster(m_curPosition);
      if (offset == 0 && blockOfCluster == 0) {
        // start of new cluster
        if (m_curPosition == 0) {
          // use first cluster in file
          m_curCluster = isRoot32() ? m_vol->rootDirStart() : m_firstCluster;
        } else {
          // get next cluster from FAT
          fg = m_vol->fatGet(m_curCluster, &m_curCluster);
          if (fg < 0) {
            DBG_FAIL_MACRO;
            goto fail;
          }
          if (fg == 0) {
            if (isDir()) {
              break;
            }
            DBG_FAIL_MACRO;
            goto fail;
          }
        }
      }
      block = m_vol->clusterFirstBlock(m_curCluster) + blockOfCluster;
    }
    if (offset != 0 || toRead < 512 || block == m_vol->cacheBlockNumber()) {
      // amount to be read from current block
      n = 512 - offset;
      if (n > toRead) {
        n = toRead;
      }
      // read block to cache and copy data to caller
      pc = m_vol->cacheFetchData(block, FatCache::CACHE_FOR_READ);
      if (!pc) {
        DBG_FAIL_MACRO;
        goto fail;
      }
      uint8_t* src = pc->data + offset;
      memcpy(dst, src, n);
#if USE_MULTI_BLOCK_IO
    } else if (toRead >= 1024) {
      size_t nb = toRead >> 9;
      if (!isRootFixed()) {
        uint8_t mb = m_vol->blocksPerCluster() - blockOfCluster;
        if (mb < nb) {
          nb = mb;
        }
      }
      n = 512*nb;
      if (m_vol->cacheBlockNumber() <= block
          && block < (m_vol->cacheBlockNumber() + nb)) {
        // flush cache if a block is in the cache
        if (!m_vol->cacheSyncData()) {
          DBG_FAIL_MACRO;
          goto fail;
        }
      }
      if (!m_vol->readBlocks(block, dst, nb)) {
        DBG_FAIL_MACRO;
        goto fail;
      }
#endif  // USE_MULTI_BLOCK_IO
    } else {
      // read single block
      n = 512;
      if (!m_vol->readBlock(block, dst)) {
        DBG_FAIL_MACRO;
        goto fail;
      }
    }
    dst += n;
    m_curPosition += n;
    toRead -= n;
  }
  return nbyte - toRead;

fail:
  m_error |= READ_ERROR;
  return -1;
}
//------------------------------------------------------------------------------
int8_t FatFile::readDir(dir_t* dir) {
  int16_t n;
  // if not a directory file or miss-positioned return an error
  if (!isDir() || (0X1F & m_curPosition)) {
    return -1;
  }

  while (1) {
    n = read(dir, sizeof(dir_t));
    if (n != sizeof(dir_t)) {
      return n == 0 ? 0 : -1;
    }
    // last entry if DIR_NAME_FREE
    if (dir->name[0] == DIR_NAME_FREE) {
      return 0;
    }
    // skip empty entries and entry for .  and ..
    if (dir->name[0] == DIR_NAME_DELETED || dir->name[0] == '.') {
      continue;
    }
    // return if normal file or subdirectory
    if (DIR_IS_FILE_OR_SUBDIR(dir)) {
      return n;
    }
  }
}
//------------------------------------------------------------------------------
// Read next directory entry into the cache
// Assumes file is correctly positioned
dir_t* FatFile::readDirCache(bool skipReadOk) {
//  uint8_t b;
  uint8_t i = (m_curPosition >> 5) & 0XF;

  if (i == 0 || !skipReadOk) {
    int8_t n = read(&n, 1);
    if  (n != 1) {
      if (n != 0) {
        DBG_FAIL_MACRO;
      }
      goto fail;
    }
    m_curPosition += 31;
  } else {
    m_curPosition += 32;
  }
  // return pointer to entry
  return m_vol->cacheAddress()->dir + i;

fail:
  return 0;
}
//------------------------------------------------------------------------------
bool FatFile::remove(FatFile* dirFile, const char* path) {
  FatFile file;
  if (!file.open(dirFile, path, O_WRONLY)) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  return file.remove();

fail:
  return false;
}
//------------------------------------------------------------------------------
bool FatFile::rename(FatFile* dirFile, const char* newPath) {
  dir_t entry;
  uint32_t dirCluster = 0;
  FatFile file;
  FatFile oldFile;
  cache_t* pc;
  dir_t* dir;

  // Must be an open file or subdirectory.
  if (!(isFile() || isSubDir())) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  // Can't rename LFN in 8.3 mode.
  if (!USE_LONG_FILE_NAMES && isLFN()) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  // Can't move file to new volume.
  if (m_vol != dirFile->m_vol) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  // sync() and cache directory entry
  sync();
  oldFile = *this;
  dir = cacheDirEntry(FatCache::CACHE_FOR_READ);
  if (!dir) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  // save directory entry
  memcpy(&entry, dir, sizeof(entry));
  // make directory entry for new path
  if (isFile()) {
    if (!file.open(dirFile, newPath, O_WRONLY | O_CREAT | O_EXCL)) {
      DBG_FAIL_MACRO;
      goto fail;
    }
  } else {
    // don't create missing path prefix components
    if (!file.mkdir(dirFile, newPath, false)) {
      DBG_FAIL_MACRO;
      goto fail;
    }
    // save cluster containing new dot dot
    dirCluster = file.m_firstCluster;
  }
  // change to new directory entry

  m_dirBlock = file.m_dirBlock;
  m_dirIndex = file.m_dirIndex;
  m_lfnOrd = file.m_lfnOrd;
  m_dirCluster = file.m_dirCluster;
  // mark closed to avoid possible destructor close call
  file.m_attr = FILE_ATTR_CLOSED;

  // cache new directory entry
  dir = cacheDirEntry(FatCache::CACHE_FOR_WRITE);
  if (!dir) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  // copy all but name and name flags to new directory entry
  memcpy(&dir->creationTimeTenths, &entry.creationTimeTenths,
         sizeof(entry) - sizeof(dir->name) - 2);
  dir->attributes = entry.attributes;

  // update dot dot if directory
  if (dirCluster) {
    // get new dot dot
    uint32_t block = m_vol->clusterFirstBlock(dirCluster);
    pc = m_vol->cacheFetchData(block, FatCache::CACHE_FOR_READ);
    if (!pc) {
      DBG_FAIL_MACRO;
      goto fail;
    }
    memcpy(&entry, &pc->dir[1], sizeof(entry));

    // free unused cluster
    if (!m_vol->freeChain(dirCluster)) {
      DBG_FAIL_MACRO;
      goto fail;
    }
    // store new dot dot
    block = m_vol->clusterFirstBlock(m_firstCluster);
    pc = m_vol->cacheFetchData(block, FatCache::CACHE_FOR_WRITE);
    if (!pc) {
      DBG_FAIL_MACRO;
      goto fail;
    }
    memcpy(&pc->dir[1], &entry, sizeof(entry));
  }
  // Remove old directory entry;
  oldFile.m_firstCluster = 0;
  oldFile.m_flags = F_WRITE;
  oldFile.m_attr = FILE_ATTR_FILE;
  if (!oldFile.remove()) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  return m_vol->cacheSync();

fail:
  return false;
}
//------------------------------------------------------------------------------
bool FatFile::rmdir() {
  // must be open subdirectory
  if (!isSubDir() || (!USE_LONG_FILE_NAMES && isLFN())) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  rewind();

  // make sure directory is empty
  while (1) {
    dir_t* dir = readDirCache(true);
    if (!dir) {
      // EOF if no error.
      if (!getError()) {
        break;
      }
      DBG_FAIL_MACRO;
      goto fail;
    }
    // done if past last used entry
    if (dir->name[0] == DIR_NAME_FREE) {
      break;
    }
    // skip empty slot, '.' or '..'
    if (dir->name[0] == DIR_NAME_DELETED || dir->name[0] == '.') {
      continue;
    }
    // error not empty
    if (DIR_IS_FILE_OR_SUBDIR(dir)) {
      DBG_FAIL_MACRO;
      goto fail;
    }
  }
  // convert empty directory to normal file for remove
  m_attr = FILE_ATTR_FILE;
  m_flags |= F_WRITE;
  return remove();

fail:
  return false;
}
//------------------------------------------------------------------------------
bool FatFile::rmRfStar() {
  uint16_t index;
  FatFile f;
  if (!isDir()) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  rewind();
  while (1) {
    // remember position
    index = m_curPosition/32;

    dir_t* dir = readDirCache();
    if (!dir) {
      // At EOF if no error.
      if (!getError()) {
        break;
      }
      DBG_FAIL_MACRO;
      goto fail;
    }
    // done if past last entry
    if (dir->name[0] == DIR_NAME_FREE) {
      break;
    }

    // skip empty slot or '.' or '..'
    if (dir->name[0] == DIR_NAME_DELETED || dir->name[0] == '.') {
      continue;
    }

    // skip if part of long file name or volume label in root
    if (!DIR_IS_FILE_OR_SUBDIR(dir)) {
      continue;
    }

    if (!f.open(this, index, O_RDONLY)) {
      DBG_FAIL_MACRO;
      goto fail;
    }
    if (f.isSubDir()) {
      // recursively delete
      if (!f.rmRfStar()) {
        DBG_FAIL_MACRO;
        goto fail;
      }
    } else {
      // ignore read-only
      f.m_flags |= F_WRITE;
      if (!f.remove()) {
        DBG_FAIL_MACRO;
        goto fail;
      }
    }
    // position to next entry if required
    if (m_curPosition != (32UL*(index + 1))) {
      if (!seekSet(32UL*(index + 1))) {
        DBG_FAIL_MACRO;
        goto fail;
      }
    }
  }
  // don't try to delete root
  if (!isRoot()) {
    if (!rmdir()) {
      DBG_FAIL_MACRO;
      goto fail;
    }
  }
  return true;

fail:
  return false;
}
//------------------------------------------------------------------------------
bool FatFile::seekSet(uint32_t pos) {
  uint32_t nCur;
  uint32_t nNew;
  uint32_t tmp = m_curCluster;
  // error if file not open
  if (!isOpen()) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  // Optimize O_APPEND writes.
  if (pos == m_curPosition) {
    return true;
  }
  if (pos == 0) {
    // set position to start of file
    m_curCluster = 0;
    goto done;
  }
  if (isFile()) {
    if (pos > m_fileSize) {
      DBG_FAIL_MACRO;
      goto fail;
    }
  } else if (isRootFixed()) {
    if (pos <= 32*m_vol->rootDirEntryCount()) {
      goto done;
    }
    DBG_FAIL_MACRO;
    goto fail;
  }
  // calculate cluster index for cur and new position
  nCur = (m_curPosition - 1) >> (m_vol->clusterSizeShift() + 9);
  nNew = (pos - 1) >> (m_vol->clusterSizeShift() + 9);

  if (nNew < nCur || m_curPosition == 0) {
    // must follow chain from first cluster
    m_curCluster = isRoot32() ? m_vol->rootDirStart() : m_firstCluster;
  } else {
    // advance from curPosition
    nNew -= nCur;
  }
  while (nNew--) {
    if (m_vol->fatGet(m_curCluster, &m_curCluster) <= 0) {
      DBG_FAIL_MACRO;
      goto fail;
    }
  }

done:
  m_curPosition = pos;
  return true;

fail:
  m_curCluster = tmp;
  return false;
}
//------------------------------------------------------------------------------
void FatFile::setpos(FatPos_t* pos) {
  m_curPosition = pos->position;
  m_curCluster = pos->cluster;
}
//------------------------------------------------------------------------------
bool FatFile::sync() {
  if (!isOpen()) {
    return true;
  }
  if (m_flags & F_FILE_DIR_DIRTY) {
    dir_t* dir = cacheDirEntry(FatCache::CACHE_FOR_WRITE);
    // check for deleted by another open file object
    if (!dir || dir->name[0] == DIR_NAME_DELETED) {
      DBG_FAIL_MACRO;
      goto fail;
    }
    // do not set filesize for dir files
    if (isFile()) {
      dir->fileSize = m_fileSize;
    }

    // update first cluster fields
    dir->firstClusterLow = m_firstCluster & 0XFFFF;
    dir->firstClusterHigh = m_firstCluster >> 16;

    // set modify time if user supplied a callback date/time function
    if (m_dateTime) {
      m_dateTime(&dir->lastWriteDate, &dir->lastWriteTime);
      dir->lastAccessDate = dir->lastWriteDate;
    }
    // clear directory dirty
    m_flags &= ~F_FILE_DIR_DIRTY;
  }
  if (m_vol->cacheSync()) {
    return true;
  }
  DBG_FAIL_MACRO;

fail:
  m_error |= WRITE_ERROR;
  return false;
}
//------------------------------------------------------------------------------
bool FatFile::timestamp(FatFile* file) {
  dir_t* dir;
  dir_t srcDir;

  // most be files get timestamps
  if (!isFile() || !file->isFile() || !file->dirEntry(&srcDir)) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  // update directory fields
  if (!sync()) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  dir = cacheDirEntry(FatCache::CACHE_FOR_WRITE);
  if (!dir) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  // copy timestamps
  dir->lastAccessDate = srcDir.lastAccessDate;
  dir->creationDate = srcDir.creationDate;
  dir->creationTime = srcDir.creationTime;
  dir->creationTimeTenths = srcDir.creationTimeTenths;
  dir->lastWriteDate = srcDir.lastWriteDate;
  dir->lastWriteTime = srcDir.lastWriteTime;

  // write back entry
  return m_vol->cacheSync();

fail:
  return false;
}
//------------------------------------------------------------------------------
bool FatFile::timestamp(uint8_t flags, uint16_t year, uint8_t month,
                   uint8_t day, uint8_t hour, uint8_t minute, uint8_t second) {
  uint16_t dirDate;
  uint16_t dirTime;
  dir_t* dir;

  if (!isFile()
      || year < 1980
      || year > 2107
      || month < 1
      || month > 12
      || day < 1
      || day > 31
      || hour > 23
      || minute > 59
      || second > 59) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  // update directory entry
  if (!sync()) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  dir = cacheDirEntry(FatCache::CACHE_FOR_WRITE);
  if (!dir) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  dirDate = FAT_DATE(year, month, day);
  dirTime = FAT_TIME(hour, minute, second);
  if (flags & T_ACCESS) {
    dir->lastAccessDate = dirDate;
  }
  if (flags & T_CREATE) {
    dir->creationDate = dirDate;
    dir->creationTime = dirTime;
    // seems to be units of 1/100 second not 1/10 as Microsoft states
    dir->creationTimeTenths = second & 1 ? 100 : 0;
  }
  if (flags & T_WRITE) {
    dir->lastWriteDate = dirDate;
    dir->lastWriteTime = dirTime;
  }
  return m_vol->cacheSync();

fail:
  return false;
}
//------------------------------------------------------------------------------
bool FatFile::truncate(uint32_t length) {
  uint32_t newPos;
  // error if not a normal file or read-only
  if (!isFile() || !(m_flags & F_WRITE)) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  // error if length is greater than current size
  if (length > m_fileSize) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  // fileSize and length are zero - nothing to do
  if (m_fileSize == 0) {
    return true;
  }

  // remember position for seek after truncation
  newPos = m_curPosition > length ? length : m_curPosition;

  // position to last cluster in truncated file
  if (!seekSet(length)) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  if (length == 0) {
    // free all clusters
    if (!m_vol->freeChain(m_firstCluster)) {
      DBG_FAIL_MACRO;
      goto fail;
    }
    m_firstCluster = 0;
  } else {
    uint32_t toFree;
    int8_t fg = m_vol->fatGet(m_curCluster, &toFree);
    if (fg < 0) {
      DBG_FAIL_MACRO;
      goto fail;
    }
    if (fg) {
      // free extra clusters
      if (!m_vol->freeChain(toFree)) {
        DBG_FAIL_MACRO;
        goto fail;
      }
      // current cluster is end of chain
      if (!m_vol->fatPutEOC(m_curCluster)) {
        DBG_FAIL_MACRO;
        goto fail;
      }
    }
  }
  m_fileSize = length;

  // need to update directory entry
  m_flags |= F_FILE_DIR_DIRTY;

  if (!sync()) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  // set file to correct position
  return seekSet(newPos);

fail:
  return false;
}
//------------------------------------------------------------------------------
int FatFile::write(const void* buf, size_t nbyte) {
  // convert void* to uint8_t*  -  must be before goto statements
  const uint8_t* src = reinterpret_cast<const uint8_t*>(buf);
  cache_t* pc;
  uint8_t cacheOption;
  // number of bytes left to write  -  must be before goto statements
  size_t nToWrite = nbyte;
  size_t n;
  // error if not a normal file or is read-only
  if (!isFile() || !(m_flags & F_WRITE)) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  // seek to end of file if append flag
  if ((m_flags & F_APPEND)) {
    if (!seekSet(m_fileSize)) {
      DBG_FAIL_MACRO;
      goto fail;
    }
  }
  // Don't exceed max fileSize.
  if (nbyte > (0XFFFFFFFF - m_curPosition)) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  while (nToWrite) {
    uint8_t blockOfCluster = m_vol->blockOfCluster(m_curPosition);
    uint16_t blockOffset = m_curPosition & 0X1FF;
    if (blockOfCluster == 0 && blockOffset == 0) {
      // start of new cluster
      if (m_curCluster != 0) {
        int8_t fg = m_vol->fatGet(m_curCluster, &m_curCluster);
        if (fg < 0) {
          DBG_FAIL_MACRO;
          goto fail;
        }
        if (fg == 0) {
          // add cluster if at end of chain
          if (!addCluster()) {
            DBG_FAIL_MACRO;
            goto fail;
          }
        }
      } else {
        if (m_firstCluster == 0) {
          // allocate first cluster of file
          if (!addCluster()) {
            DBG_FAIL_MACRO;
            goto fail;
          }
          m_firstCluster = m_curCluster;
        } else {
          m_curCluster = m_firstCluster;
        }
      }
    }
    // block for data write
    uint32_t block = m_vol->clusterFirstBlock(m_curCluster) + blockOfCluster;

    if (blockOffset != 0 || nToWrite < 512) {
      // partial block - must use cache
      // max space in block
      n = 512 - blockOffset;
      // lesser of space and amount to write
      if (n > nToWrite) {
        n = nToWrite;
      }

      if (blockOffset == 0 && m_curPosition >= m_fileSize) {
        // start of new block don't need to read into cache
        cacheOption = FatCache::CACHE_RESERVE_FOR_WRITE;
      } else {
        // rewrite part of block
        cacheOption = FatCache::CACHE_FOR_WRITE;
      }
      pc = m_vol->cacheFetchData(block, cacheOption);
      if (!pc) {
        DBG_FAIL_MACRO;
        goto fail;
      }
      uint8_t* dst = pc->data + blockOffset;
      memcpy(dst, src, n);
      if (512 == (n + blockOffset)) {
        // Force write if block is full - improves large writes.
        if (!m_vol->cacheSyncData()) {
          DBG_FAIL_MACRO;
          goto fail;
        }
      }
#if USE_MULTI_BLOCK_IO
    } else if (nToWrite >= 1024) {
      // use multiple block write command
      uint8_t maxBlocks = m_vol->blocksPerCluster() - blockOfCluster;
      size_t nb = nToWrite >> 9;
      if (nb > maxBlocks) {
        nb = maxBlocks;
      }
      n = 512*nb;
      if (m_vol->cacheBlockNumber() <= block
          && block < (m_vol->cacheBlockNumber() + nb)) {
        // invalidate cache if block is in cache
        m_vol->cacheInvalidate();
      }
      if (!m_vol->writeBlocks(block, src, nb)) {
        DBG_FAIL_MACRO;
        goto fail;
      }
#endif  // USE_MULTI_BLOCK_IO
    } else {
      // use single block write command
      n = 512;
      if (m_vol->cacheBlockNumber() == block) {
        m_vol->cacheInvalidate();
      }
      if (!m_vol->writeBlock(block, src)) {
        DBG_FAIL_MACRO;
        goto fail;
      }
    }
    m_curPosition += n;
    src += n;
    nToWrite -= n;
  }
  if (m_curPosition > m_fileSize) {
    // update fileSize and insure sync will update dir entry
    m_fileSize = m_curPosition;
    m_flags |= F_FILE_DIR_DIRTY;
  } else if (m_dateTime) {
    // insure sync will update modified date and time
    m_flags |= F_FILE_DIR_DIRTY;
  }

  if (m_flags & F_SYNC) {
    if (!sync()) {
      DBG_FAIL_MACRO;
      goto fail;
    }
  }
  return nbyte;

fail:
  // return for write error
  m_error |= WRITE_ERROR;
  return -1;
}
