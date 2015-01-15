/* Arduino SdFat Library
 * Copyright (C) 2009 by William Greiman
 *
 * This file is part of the Arduino SdFat Library
 *
 * This Library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the Arduino SdFat Library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */
#include "Marlin.h"
#ifdef SDSUPPORT

#include "SdVolume.h"
//------------------------------------------------------------------------------
#if !USE_MULTIPLE_CARDS
// raw block cache
uint32_t SdVolume::cacheBlockNumber_;  // current block number
cache_t  SdVolume::cacheBuffer_;       // 512 byte cache for Sd2Card
Sd2Card* SdVolume::sdCard_;            // pointer to SD card object
bool     SdVolume::cacheDirty_;        // cacheFlush() will write block if true
uint32_t SdVolume::cacheMirrorBlock_;  // mirror  block for second FAT
#endif  // USE_MULTIPLE_CARDS
//------------------------------------------------------------------------------
// find a contiguous group of clusters
bool SdVolume::allocContiguous(uint32_t count, uint32_t* curCluster) {
  // start of group
  uint32_t bgnCluster;
  // end of group
  uint32_t endCluster;
  // last cluster of FAT
  uint32_t fatEnd = clusterCount_ + 1;

  // flag to save place to start next search
  bool setStart;

  // set search start cluster
  if (*curCluster) {
    // try to make file contiguous
    bgnCluster = *curCluster + 1;

    // don't save new start location
    setStart = false;
  } else {
    // start at likely place for free cluster
    bgnCluster = allocSearchStart_;

    // save next search start if one cluster
    setStart = count == 1;
  }
  // end of group
  endCluster = bgnCluster;

  // search the FAT for free clusters
  for (uint32_t n = 0;; n++, endCluster++) {
    // can't find space checked all clusters
    if (n >= clusterCount_) goto fail;

    // past end - start from beginning of FAT
    if (endCluster > fatEnd) {
      bgnCluster = endCluster = 2;
    }
    uint32_t f;
    if (!fatGet(endCluster, &f)) goto fail;

    if (f != 0) {
      // cluster in use try next cluster as bgnCluster
      bgnCluster = endCluster + 1;
    } else if ((endCluster - bgnCluster + 1) == count) {
      // done - found space
      break;
    }
  }
  // mark end of chain
  if (!fatPutEOC(endCluster)) goto fail;

  // link clusters
  while (endCluster > bgnCluster) {
    if (!fatPut(endCluster - 1, endCluster)) goto fail;
    endCluster--;
  }
  if (*curCluster != 0) {
    // connect chains
    if (!fatPut(*curCluster, bgnCluster)) goto fail;
  }
  // return first cluster number to caller
  *curCluster = bgnCluster;

  // remember possible next free cluster
  if (setStart) allocSearchStart_ = bgnCluster + 1;

  return true;

 fail:
  return false;
}
//------------------------------------------------------------------------------
bool SdVolume::cacheFlush() {
  if (cacheDirty_) {
    if (!sdCard_->writeBlock(cacheBlockNumber_, cacheBuffer_.data)) {
      goto fail;
    }
    // mirror FAT tables
    if (cacheMirrorBlock_) {
      if (!sdCard_->writeBlock(cacheMirrorBlock_, cacheBuffer_.data)) {
        goto fail;
      }
      cacheMirrorBlock_ = 0;
    }
    cacheDirty_ = 0;
  }
  return true;

 fail:
  return false;
}
//------------------------------------------------------------------------------
bool SdVolume::cacheRawBlock(uint32_t blockNumber, bool dirty) {
  if (cacheBlockNumber_ != blockNumber) {
    if (!cacheFlush()) goto fail;
    if (!sdCard_->readBlock(blockNumber, cacheBuffer_.data)) goto fail;
    cacheBlockNumber_ = blockNumber;
  }
  if (dirty) cacheDirty_ = true;
  return true;

 fail:
  return false;
}
//------------------------------------------------------------------------------
// return the size in bytes of a cluster chain
bool SdVolume::chainSize(uint32_t cluster, uint32_t* size) {
  uint32_t s = 0;
  do {
    if (!fatGet(cluster, &cluster)) goto fail;
    s += 512UL << clusterSizeShift_;
  } while (!isEOC(cluster));
  *size = s;
  return true;

 fail:
  return false;
}
//------------------------------------------------------------------------------
// Fetch a FAT entry
bool SdVolume::fatGet(uint32_t cluster, uint32_t* value) {
  uint32_t lba;
  if (cluster > (clusterCount_ + 1)) goto fail;
  if (FAT12_SUPPORT && fatType_ == 12) {
    uint16_t index = cluster;
    index += index >> 1;
    lba = fatStartBlock_ + (index >> 9);
    if (!cacheRawBlock(lba, CACHE_FOR_READ)) goto fail;
    index &= 0X1FF;
    uint16_t tmp = cacheBuffer_.data[index];
    index++;
    if (index == 512) {
      if (!cacheRawBlock(lba + 1, CACHE_FOR_READ)) goto fail;
      index = 0;
    }
    tmp |= cacheBuffer_.data[index] << 8;
    *value = cluster & 1 ? tmp >> 4 : tmp & 0XFFF;
    return true;
  }
  if (fatType_ == 16) {
    lba = fatStartBlock_ + (cluster >> 8);
  } else if (fatType_ == 32) {
    lba = fatStartBlock_ + (cluster >> 7);
  } else {
    goto fail;
  }
  if (lba != cacheBlockNumber_) {
    if (!cacheRawBlock(lba, CACHE_FOR_READ)) goto fail;
  }
  if (fatType_ == 16) {
    *value = cacheBuffer_.fat16[cluster & 0XFF];
  } else {
    *value = cacheBuffer_.fat32[cluster & 0X7F] & FAT32MASK;
  }
  return true;

 fail:
  return false;
}
//------------------------------------------------------------------------------
// Store a FAT entry
bool SdVolume::fatPut(uint32_t cluster, uint32_t value) {
  uint32_t lba;
  // error if reserved cluster
  if (cluster < 2) goto fail;

  // error if not in FAT
  if (cluster > (clusterCount_ + 1)) goto fail;

  if (FAT12_SUPPORT && fatType_ == 12) {
    uint16_t index = cluster;
    index += index >> 1;
    lba = fatStartBlock_ + (index >> 9);
    if (!cacheRawBlock(lba, CACHE_FOR_WRITE)) goto fail;
    // mirror second FAT
    if (fatCount_ > 1) cacheMirrorBlock_ = lba + blocksPerFat_;
    index &= 0X1FF;
    uint8_t tmp = value;
    if (cluster & 1) {
      tmp = (cacheBuffer_.data[index] & 0XF) | tmp << 4;
    }
    cacheBuffer_.data[index] = tmp;
    index++;
    if (index == 512) {
      lba++;
      index = 0;
      if (!cacheRawBlock(lba, CACHE_FOR_WRITE)) goto fail;
      // mirror second FAT
      if (fatCount_ > 1) cacheMirrorBlock_ = lba + blocksPerFat_;
    }
    tmp = value >> 4;
    if (!(cluster & 1)) {
      tmp = ((cacheBuffer_.data[index] & 0XF0)) | tmp >> 4;
    }
    cacheBuffer_.data[index] = tmp;
    return true;
  }
  if (fatType_ == 16) {
    lba = fatStartBlock_ + (cluster >> 8);
  } else if (fatType_ == 32) {
    lba = fatStartBlock_ + (cluster >> 7);
  } else {
    goto fail;
  }
  if (!cacheRawBlock(lba, CACHE_FOR_WRITE)) goto fail;
  // store entry
  if (fatType_ == 16) {
    cacheBuffer_.fat16[cluster & 0XFF] = value;
  } else {
    cacheBuffer_.fat32[cluster & 0X7F] = value;
  }
  // mirror second FAT
  if (fatCount_ > 1) cacheMirrorBlock_ = lba + blocksPerFat_;
  return true;

 fail:
  return false;
}
//------------------------------------------------------------------------------
// free a cluster chain
bool SdVolume::freeChain(uint32_t cluster) {
  uint32_t next;

  // clear free cluster location
  allocSearchStart_ = 2;

  do {
    if (!fatGet(cluster, &next)) goto fail;

    // free cluster
    if (!fatPut(cluster, 0)) goto fail;

    cluster = next;
  } while (!isEOC(cluster));

  return true;

 fail:
  return false;
}
//------------------------------------------------------------------------------
/** Volume free space in clusters.
 *
 * \return Count of free clusters for success or -1 if an error occurs.
 */
int32_t SdVolume::freeClusterCount() {
  uint32_t free = 0;
  uint16_t n;
  uint32_t todo = clusterCount_ + 2;

  if (fatType_ == 16) {
    n = 256;
  } else if (fatType_ == 32) {
    n = 128;
  } else {
    // put FAT12 here
    return -1;
  }

  for (uint32_t lba = fatStartBlock_; todo; todo -= n, lba++) {
    if (!cacheRawBlock(lba, CACHE_FOR_READ)) return -1;
    if (todo < n) n = todo;
    if (fatType_ == 16) {
      for (uint16_t i = 0; i < n; i++) {
        if (cacheBuffer_.fat16[i] == 0) free++;
      }
    } else {
      for (uint16_t i = 0; i < n; i++) {
        if (cacheBuffer_.fat32[i] == 0) free++;
      }
    }
  }
  return free;
}
//------------------------------------------------------------------------------
/** Initialize a FAT volume.
 *
 * \param[in] dev The SD card where the volume is located.
 *
 * \param[in] part The partition to be used.  Legal values for \a part are
 * 1-4 to use the corresponding partition on a device formatted with
 * a MBR, Master Boot Record, or zero if the device is formatted as
 * a super floppy with the FAT boot sector in block zero.
 *
 * \return The value one, true, is returned for success and
 * the value zero, false, is returned for failure.  Reasons for
 * failure include not finding a valid partition, not finding a valid
 * FAT file system in the specified partition or an I/O error.
 */
bool SdVolume::init(Sd2Card* dev, uint8_t part) {
  uint32_t totalBlocks;
  uint32_t volumeStartBlock = 0;
  fat32_boot_t* fbs;

  sdCard_ = dev;
  fatType_ = 0;
  allocSearchStart_ = 2;
  cacheDirty_ = 0;  // cacheFlush() will write block if true
  cacheMirrorBlock_ = 0;
  cacheBlockNumber_ = 0XFFFFFFFF;

  // if part == 0 assume super floppy with FAT boot sector in block zero
  // if part > 0 assume mbr volume with partition table
  if (part) {
    if (part > 4)goto fail;
    if (!cacheRawBlock(volumeStartBlock, CACHE_FOR_READ)) goto fail;
    part_t* p = &cacheBuffer_.mbr.part[part-1];
    if ((p->boot & 0X7F) !=0  ||
      p->totalSectors < 100 ||
      p->firstSector == 0) {
      // not a valid partition
      goto fail;
    }
    volumeStartBlock = p->firstSector;
  }
  if (!cacheRawBlock(volumeStartBlock, CACHE_FOR_READ)) goto fail;
  fbs = &cacheBuffer_.fbs32;
  if (fbs->bytesPerSector != 512 ||
    fbs->fatCount == 0 ||
    fbs->reservedSectorCount == 0 ||
    fbs->sectorsPerCluster == 0) {
       // not valid FAT volume
      goto fail;
  }
  fatCount_ = fbs->fatCount;
  blocksPerCluster_ = fbs->sectorsPerCluster;
  // determine shift that is same as multiply by blocksPerCluster_
  clusterSizeShift_ = 0;
  while (blocksPerCluster_ != (1 << clusterSizeShift_)) {
    // error if not power of 2
    if (clusterSizeShift_++ > 7) goto fail;
  }
  blocksPerFat_ = fbs->sectorsPerFat16 ?
                    fbs->sectorsPerFat16 : fbs->sectorsPerFat32;

  fatStartBlock_ = volumeStartBlock + fbs->reservedSectorCount;

  // count for FAT16 zero for FAT32
  rootDirEntryCount_ = fbs->rootDirEntryCount;

  // directory start for FAT16 dataStart for FAT32
  rootDirStart_ = fatStartBlock_ + fbs->fatCount * blocksPerFat_;

  // data start for FAT16 and FAT32
  dataStartBlock_ = rootDirStart_ + ((32 * fbs->rootDirEntryCount + 511)/512);

  // total blocks for FAT16 or FAT32
  totalBlocks = fbs->totalSectors16 ?
                           fbs->totalSectors16 : fbs->totalSectors32;
  // total data blocks
  clusterCount_ = totalBlocks - (dataStartBlock_ - volumeStartBlock);

  // divide by cluster size to get cluster count
  clusterCount_ >>= clusterSizeShift_;

  // FAT type is determined by cluster count
  if (clusterCount_ < 4085) {
    fatType_ = 12;
    if (!FAT12_SUPPORT) goto fail;
  } else if (clusterCount_ < 65525) {
    fatType_ = 16;
  } else {
    rootDirStart_ = fbs->fat32RootCluster;
    fatType_ = 32;
  }
  return true;

 fail:
  return false;
}
#endif