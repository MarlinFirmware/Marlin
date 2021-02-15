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
#include <string.h>
#include "FatVolume.h"
//------------------------------------------------------------------------------
cache_t* FatCache::read(uint32_t lbn, uint8_t option) {
  if (m_lbn != lbn) {
    if (!sync()) {
      DBG_FAIL_MACRO;
      goto fail;
    }
    if (!(option & CACHE_OPTION_NO_READ)) {
      if (!m_vol->readBlock(lbn, m_block.data)) {
        DBG_FAIL_MACRO;
        goto fail;
      }
    }
    m_status = 0;
    m_lbn = lbn;
  }
  m_status |= option & CACHE_STATUS_MASK;
  return &m_block;

fail:

  return 0;
}
//------------------------------------------------------------------------------
bool FatCache::sync() {
  if (m_status & CACHE_STATUS_DIRTY) {
    if (!m_vol->writeBlock(m_lbn, m_block.data)) {
      DBG_FAIL_MACRO;
      goto fail;
    }
    // mirror second FAT
    if ( (m_vol->m_fatCount == 2) && (m_status & CACHE_STATUS_MIRROR_FAT) ) {
      uint32_t lbn = m_lbn + m_vol->blocksPerFat();
      if (!m_vol->writeBlock(lbn, m_block.data)) {
        DBG_FAIL_MACRO;
        goto fail;
      }
    }
    m_status &= ~CACHE_STATUS_DIRTY;
  }
  return true;

fail:
  return false;
}
//------------------------------------------------------------------------------
bool FatVolume::allocateCluster(uint32_t current, uint32_t* next) {
  uint32_t find;
  bool setStart;
  if (m_allocSearchStart < current) {
    // Try to keep file contiguous. Start just after current cluster.
    find = current;
    setStart = false;
  } else {
    find = m_allocSearchStart;
    setStart = true;
  }
  while (1) {
    find++;
    if (find > m_lastCluster) {
      if (setStart) {
        // Can't find space, checked all clusters.
        DBG_FAIL_MACRO;
        goto fail;
      }
      find = m_allocSearchStart;
      setStart = true;
      continue;
    }
    if (find == current) {
      // Can't find space, already searched clusters after current.
      DBG_FAIL_MACRO;
      goto fail;
    }
    uint32_t f;
    int8_t fg = fatGet(find, &f);
    if (fg < 0) {
      DBG_FAIL_MACRO;
      goto fail;
    }
    if (fg && f == 0) {
      break;
    }
  }
  if (setStart) {
    m_allocSearchStart = find;
  }
  // Mark end of chain.
  if (!fatPutEOC(find)) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  if (current) {
    // Link clusters.
    if (!fatPut(current, find)) {
      DBG_FAIL_MACRO;
      goto fail;
    }
  }
  updateFreeClusterCount(-1);
  *next = find;
  return true;

fail:
  return false;
}
//------------------------------------------------------------------------------
// find a contiguous group of clusters
bool FatVolume::allocContiguous(uint32_t count,
                                uint32_t* firstCluster, uint32_t startCluster) {
  // flag to save place to start next search
  bool setStart;
  // start of group
  uint32_t bgnCluster;
  // end of group
  uint32_t endCluster;
  if (startCluster != 0) {
    bgnCluster = startCluster;
    setStart = false;
  } else {
    // Start at cluster after last allocated cluster.
    bgnCluster = m_allocSearchStart + 1;
    setStart = true;
  }
  endCluster = bgnCluster;
  // search the FAT for free clusters
  while (1) {
    if (endCluster > m_lastCluster) {
      // Can't find space.
      DBG_FAIL_MACRO;
      goto fail;
    }
    uint32_t f;
    int8_t fg = fatGet(endCluster, &f);
    if (fg < 0) {
      DBG_FAIL_MACRO;
      goto fail;
    }
    if (f || fg == 0) {
      if (startCluster) {
        DBG_FAIL_MACRO;
        goto fail;
      }
      // don't update search start if unallocated clusters before endCluster.
      if (bgnCluster != endCluster) {
        setStart = false;
      }
      // cluster in use try next cluster as bgnCluster
      bgnCluster = endCluster + 1;
    } else if ((endCluster - bgnCluster + 1) == count) {
      // done - found space
      break;
    }
    endCluster++;
  }
  // Remember possible next free cluster.
  if (setStart) {
    m_allocSearchStart = endCluster;
  }
  // mark end of chain
  if (!fatPutEOC(endCluster)) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  // link clusters
  while (endCluster > bgnCluster) {
    if (!fatPut(endCluster - 1, endCluster)) {
      DBG_FAIL_MACRO;
      goto fail;
    }
    endCluster--;
  }
  // Maintain count of free clusters.
  updateFreeClusterCount(-count);

  // return first cluster number to caller
  *firstCluster = bgnCluster;
  return true;

fail:
  return false;
}
//------------------------------------------------------------------------------
uint32_t FatVolume::clusterFirstBlock(uint32_t cluster) const {
  return m_dataStartBlock + ((cluster - 2) << m_clusterSizeShift);
}
//------------------------------------------------------------------------------
// Fetch a FAT entry - return -1 error, 0 EOC, else 1.
int8_t FatVolume::fatGet(uint32_t cluster, uint32_t* value) {
  uint32_t lba;
  uint32_t next;
  cache_t* pc;

  // error if reserved cluster of beyond FAT
  if (cluster < 2 || cluster > m_lastCluster) {
    DBG_FAIL_MACRO;
    goto fail;
  }

  if (fatType() == 32) {
    lba = m_fatStartBlock + (cluster >> 7);
    pc = cacheFetchFat(lba, FatCache::CACHE_FOR_READ);
    if (!pc) {
      DBG_FAIL_MACRO;
      goto fail;
    }
    next = pc->fat32[cluster & 0X7F] & FAT32MASK;
    goto done;
  }
  if (fatType() == 16) {
    lba = m_fatStartBlock + ((cluster >> 8) & 0XFF);
    pc = cacheFetchFat(lba, FatCache::CACHE_FOR_READ);
    if (!pc) {
      DBG_FAIL_MACRO;
      goto fail;
    }
    next = pc->fat16[cluster & 0XFF];
    goto done;
  }
  if (FAT12_SUPPORT && fatType() == 12) {
    uint16_t index = cluster;
    index += index >> 1;
    lba = m_fatStartBlock + (index >> 9);
    pc = cacheFetchFat(lba, FatCache::CACHE_FOR_READ);
    if (!pc) {
      DBG_FAIL_MACRO;
      goto fail;
    }
    index &= 0X1FF;
    uint16_t tmp = pc->data[index];
    index++;
    if (index == 512) {
      pc = cacheFetchFat(lba + 1, FatCache::CACHE_FOR_READ);
      if (!pc) {
        DBG_FAIL_MACRO;
        goto fail;
      }
      index = 0;
    }
    tmp |= pc->data[index] << 8;
    next = cluster & 1 ? tmp >> 4 : tmp & 0XFFF;
    goto done;
  } else {
    DBG_FAIL_MACRO;
    goto fail;
  }
done:
  if (isEOC(next)) {
    return 0;
  }
  *value = next;
  return 1;

fail:
  return -1;
}
//------------------------------------------------------------------------------
// Store a FAT entry
bool FatVolume::fatPut(uint32_t cluster, uint32_t value) {
  uint32_t lba;
  cache_t* pc;

  // error if reserved cluster of beyond FAT
  if (cluster < 2 || cluster > m_lastCluster) {
    DBG_FAIL_MACRO;
    goto fail;
  }

  if (fatType() == 32) {
    lba = m_fatStartBlock + (cluster >> 7);
    pc = cacheFetchFat(lba, FatCache::CACHE_FOR_WRITE);
    if (!pc) {
      DBG_FAIL_MACRO;
      goto fail;
    }
    pc->fat32[cluster & 0X7F] = value;
    return true;
  }

  if (fatType() == 16) {
    lba = m_fatStartBlock + ((cluster >> 8) & 0XFF);
    pc = cacheFetchFat(lba, FatCache::CACHE_FOR_WRITE);
    if (!pc) {
      DBG_FAIL_MACRO;
      goto fail;
    }
    pc->fat16[cluster & 0XFF] = value;
    return true;
  }

  if (FAT12_SUPPORT && fatType() == 12) {
    uint16_t index = cluster;
    index += index >> 1;
    lba = m_fatStartBlock + (index >> 9);
    pc = cacheFetchFat(lba, FatCache::CACHE_FOR_WRITE);
    if (!pc) {
      DBG_FAIL_MACRO;
      goto fail;
    }
    index &= 0X1FF;
    uint8_t tmp = value;
    if (cluster & 1) {
      tmp = (pc->data[index] & 0XF) | tmp << 4;
    }
    pc->data[index] = tmp;

    index++;
    if (index == 512) {
      lba++;
      index = 0;
      pc = cacheFetchFat(lba, FatCache::CACHE_FOR_WRITE);
      if (!pc) {
        DBG_FAIL_MACRO;
        goto fail;
      }
    }
    tmp = value >> 4;
    if (!(cluster & 1)) {
      tmp = ((pc->data[index] & 0XF0)) | tmp >> 4;
    }
    pc->data[index] = tmp;
    return true;
  } else {
    DBG_FAIL_MACRO;
    goto fail;
  }

fail:
  return false;
}
//------------------------------------------------------------------------------
// free a cluster chain
bool FatVolume::freeChain(uint32_t cluster) {
  uint32_t next;
  int8_t fg;
  do {
    fg = fatGet(cluster, &next);
    if (fg < 0) {
      DBG_FAIL_MACRO;
      goto fail;
    }
    // free cluster
    if (!fatPut(cluster, 0)) {
      DBG_FAIL_MACRO;
      goto fail;
    }
    // Add one to count of free clusters.
    updateFreeClusterCount(1);

    if (cluster <= m_allocSearchStart) {
      m_allocSearchStart = cluster - 1;
    }
    cluster = next;
  } while (fg);

  return true;

fail:
  return false;
}
//------------------------------------------------------------------------------
int32_t FatVolume::freeClusterCount() {
#if MAINTAIN_FREE_CLUSTER_COUNT
  if (m_freeClusterCount >= 0) {
    return m_freeClusterCount;
  }
#endif  // MAINTAIN_FREE_CLUSTER_COUNT
  uint32_t free = 0;
  uint32_t lba;
  uint32_t todo = m_lastCluster + 1;
  uint16_t n;

  if (FAT12_SUPPORT && fatType() == 12) {
    for (unsigned i = 2; i < todo; i++) {
      uint32_t c;
      int8_t fg = fatGet(i, &c);
      if (fg < 0) {
        DBG_FAIL_MACRO;
        goto fail;
      }
      if (fg && c == 0) {
        free++;
      }
    }
  } else if (fatType() == 16 || fatType() == 32) {
    lba = m_fatStartBlock;
    while (todo) {
      cache_t* pc = cacheFetchFat(lba++, FatCache::CACHE_FOR_READ);
      if (!pc) {
        DBG_FAIL_MACRO;
        goto fail;
      }
      n = fatType() == 16 ? 256 : 128;
      if (todo < n) {
        n = todo;
      }
      if (fatType() == 16) {
        for (uint16_t i = 0; i < n; i++) {
          if (pc->fat16[i] == 0) {
            free++;
          }
        }
      } else {
        for (uint16_t i = 0; i < n; i++) {
          if (pc->fat32[i] == 0) {
            free++;
          }
        }
      }
      todo -= n;
    }
  } else {
    // invalid FAT type
    DBG_FAIL_MACRO;
    goto fail;
  }
  setFreeClusterCount(free);
  return free;

fail:
  return -1;
}
//------------------------------------------------------------------------------
bool FatVolume::init(uint8_t part) {
  uint32_t clusterCount;
  uint32_t totalBlocks;
  uint32_t volumeStartBlock = 0;
  fat32_boot_t* fbs;
  cache_t* pc;
  uint8_t tmp;
  m_fatType = 0;
  m_allocSearchStart = 1;
  m_cache.init(this);
#if USE_SEPARATE_FAT_CACHE
  m_fatCache.init(this);
#endif  // USE_SEPARATE_FAT_CACHE
  // if part == 0 assume super floppy with FAT boot sector in block zero
  // if part > 0 assume mbr volume with partition table
  if (part) {
    if (part > 4) {
      DBG_FAIL_MACRO;
      goto fail;
    }
    pc = cacheFetchData(0, FatCache::CACHE_FOR_READ);
    if (!pc) {
      DBG_FAIL_MACRO;
      goto fail;
    }
    part_t* p = &pc->mbr.part[part - 1];
    if ((p->boot & 0X7F) != 0 || p->firstSector == 0) {
      // not a valid partition
      DBG_FAIL_MACRO;
      goto fail;
    }
    volumeStartBlock = p->firstSector;
  }
  pc = cacheFetchData(volumeStartBlock, FatCache::CACHE_FOR_READ);
  if (!pc) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  fbs = &(pc->fbs32);
  if (fbs->bytesPerSector != 512 ||
      fbs->fatCount < 1 ||
      fbs->fatCount > 2 ||
      fbs->reservedSectorCount == 0) {
    // not valid FAT volume
    DBG_FAIL_MACRO;
    goto fail;
  }
  m_fatCount = fbs->fatCount;
  m_blocksPerCluster = fbs->sectorsPerCluster;
  m_clusterBlockMask = m_blocksPerCluster - 1;
  // determine shift that is same as multiply by m_blocksPerCluster
  m_clusterSizeShift = 0;
  for (tmp = 1; m_blocksPerCluster != tmp; tmp <<= 1, m_clusterSizeShift++) {
    if (tmp == 0) {
      DBG_FAIL_MACRO;
      goto fail;
    }
  }
  m_blocksPerFat = fbs->sectorsPerFat16 ?
                   fbs->sectorsPerFat16 : fbs->sectorsPerFat32;

  m_fatStartBlock = volumeStartBlock + fbs->reservedSectorCount;

  // count for FAT16 zero for FAT32
  m_rootDirEntryCount = fbs->rootDirEntryCount;

  // directory start for FAT16 dataStart for FAT32
  m_rootDirStart = m_fatStartBlock + m_fatCount * m_blocksPerFat;
  // data start for FAT16 and FAT32
  m_dataStartBlock = m_rootDirStart + ((32 * fbs->rootDirEntryCount + 511)/512);

  // total blocks for FAT16 or FAT32
  totalBlocks = fbs->totalSectors16 ?
                fbs->totalSectors16 : fbs->totalSectors32;
  // total data blocks
  clusterCount = totalBlocks - (m_dataStartBlock - volumeStartBlock);

  // divide by cluster size to get cluster count
  clusterCount >>= m_clusterSizeShift;
  m_lastCluster = clusterCount + 1;

  // Indicate unknown number of free clusters.
  setFreeClusterCount(-1);
  // FAT type is determined by cluster count
  if (clusterCount < 4085) {
    m_fatType = 12;
    if (!FAT12_SUPPORT) {
      DBG_FAIL_MACRO;
      goto fail;
    }
  } else if (clusterCount < 65525) {
    m_fatType = 16;
  } else {
    m_rootDirStart = fbs->fat32RootCluster;
    m_fatType = 32;
  }
  return true;

fail:
  return false;
}
//------------------------------------------------------------------------------
bool FatVolume::wipe(print_t* pr) {
  cache_t* cache;
  uint16_t count;
  uint32_t lbn;
  if (!fatType()) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  cache = cacheClear();
  if (!cache) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  memset(cache->data, 0, 512);
  // Zero root.
  if (fatType() == 32) {
    lbn = clusterFirstBlock(m_rootDirStart);
    count = m_blocksPerCluster;
  } else {
    lbn = m_rootDirStart;
    count = m_rootDirEntryCount/16;
  }
  for (uint32_t n = 0; n < count; n++) {
    if (!writeBlock(lbn + n, cache->data)) {
      DBG_FAIL_MACRO;
      goto fail;
    }
  }
  // Clear FATs.
  count = m_fatCount * m_blocksPerFat;
  lbn = m_fatStartBlock;
  for (uint32_t nb = 0; nb < count; nb++) {
    if (pr && (nb & 0XFF) == 0) {
      pr->write('.');
    }
    if (!writeBlock(lbn + nb, cache->data)) {
      DBG_FAIL_MACRO;
      goto fail;
    }
  }
  // Reserve first two clusters.
  if (fatType() == 32) {
    cache->fat32[0] = 0x0FFFFFF8;
    cache->fat32[1] = 0x0FFFFFFF;
  } else if (fatType() == 16) {
    cache->fat16[0] = 0XFFF8;
    cache->fat16[1] = 0XFFFF;
  } else if (FAT12_SUPPORT && fatType() == 12) {
    cache->fat32[0] = 0XFFFFF8;
  } else {
    DBG_FAIL_MACRO;
    goto fail;
  }
  if (!writeBlock(m_fatStartBlock, cache->data) ||
      (m_fatCount == 2 && !writeBlock(m_fatStartBlock + m_blocksPerFat, cache->data))) {
    DBG_FAIL_MACRO;
    goto fail;
  }
  if (fatType() == 32) {
    // Reserve root cluster.
    if (!fatPutEOC(m_rootDirStart) || !cacheSync()) {
      DBG_FAIL_MACRO;
      goto fail;
    }
  }
  if (pr) {
    pr->write('\r');
    pr->write('\n');
  }
  m_fatType = 0;
  return true;

fail:
  m_fatType = 0;
  return false;
}
