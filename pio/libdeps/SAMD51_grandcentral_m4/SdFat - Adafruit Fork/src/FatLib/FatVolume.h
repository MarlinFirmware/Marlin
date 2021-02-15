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
#ifndef FatVolume_h
#define FatVolume_h
/**
 * \file
 * \brief FatVolume class
 */
#include <stddef.h>
#include "FatLibConfig.h"
#include "FatStructs.h"
#include "BlockDriver.h"
//------------------------------------------------------------------------------
#ifndef DOXYGEN_SHOULD_SKIP_THIS
/** Macro for debug. */
#define DEBUG_MODE 0
#if DEBUG_MODE
#define DBG_FAIL_MACRO Serial.print(F(__FILE__)); Serial.println(__LINE__);
#define DBG_PRINT_IF(b) if (b) {Serial.println(F(#b)); DBG_FAIL_MACRO;}
#define DBG_HALT_IF(b) if (b) {Serial.println(F(#b));\
                               DBG_FAIL_MACRO; while (1);}
#else  // DEBUG_MODE
#define DBG_FAIL_MACRO
#define DBG_PRINT_IF(b)
#define DBG_HALT_IF(b)
#endif  // DEBUG_MODE
#endif  // DOXYGEN_SHOULD_SKIP_THIS
//------------------------------------------------------------------------------
#if ENABLE_ARDUINO_FEATURES
/** Use Print for Arduino */
typedef Print print_t;
#else  // ENABLE_ARDUINO_FEATURES
/**
 * \class CharWriter
 * \brief Character output - often serial port.
 */
class CharWriter {
 public:
  virtual size_t write(char c) = 0;
  virtual size_t write(const char* s) = 0;
};
typedef CharWriter print_t;
#endif  // ENABLE_ARDUINO_FEATURES
//------------------------------------------------------------------------------
// Forward declaration of FatVolume.
class FatVolume;
//------------------------------------------------------------------------------
/**
 * \brief Cache for an raw data block.
 */
union cache_t {
  /** Used to access cached file data blocks. */
  uint8_t  data[512];
  /** Used to access cached FAT16 entries. */
  uint16_t fat16[256];
  /** Used to access cached FAT32 entries. */
  uint32_t fat32[128];
  /** Used to access cached directory entries. */
  dir_t    dir[16];
  /** Used to access a cached Master Boot Record. */
  mbr_t    mbr;
  /** Used to access to a cached FAT boot sector. */
  fat_boot_t fbs;
  /** Used to access to a cached FAT32 boot sector. */
  fat32_boot_t fbs32;
  /** Used to access to a cached FAT32 FSINFO sector. */
  fat32_fsinfo_t fsinfo;
};
//==============================================================================
/**
 * \class FatCache
 * \brief Block cache.
 */
class FatCache {
 public:
  /** Cached block is dirty */
  static const uint8_t CACHE_STATUS_DIRTY = 1;
  /** Cashed block is FAT entry and must be mirrored in second FAT. */
  static const uint8_t CACHE_STATUS_MIRROR_FAT = 2;
  /** Cache block status bits */
  static const uint8_t CACHE_STATUS_MASK
    = CACHE_STATUS_DIRTY | CACHE_STATUS_MIRROR_FAT;
  /** Sync existing block but do not read new block. */
  static const uint8_t CACHE_OPTION_NO_READ = 4;
  /** Cache block for read. */
  static const uint8_t CACHE_FOR_READ = 0;
  /** Cache block for write. */
  static const uint8_t CACHE_FOR_WRITE = CACHE_STATUS_DIRTY;
  /** Reserve cache block for write - do not read from block device. */
  static const uint8_t CACHE_RESERVE_FOR_WRITE
    = CACHE_STATUS_DIRTY | CACHE_OPTION_NO_READ;
  /** \return Cache block address. */
  cache_t* block() {
    return &m_block;
  }
  /** Set current block dirty. */
  void dirty() {
    m_status |= CACHE_STATUS_DIRTY;
  }
  /** Initialize the cache.
   * \param[in] vol FatVolume that owns this FatCache.
   */
  void init(FatVolume *vol) {
    m_vol = vol;
    invalidate();
  }
  /** Invalidate current cache block. */
  void invalidate() {
    m_status = 0;
    m_lbn = 0XFFFFFFFF;
  }
  /** \return dirty status */
  bool isDirty() {
    return m_status & CACHE_STATUS_DIRTY;
  }
  /** \return Logical block number for cached block. */
  uint32_t lbn() {
    return m_lbn;
  }
  /** Read a block into the cache.
   * \param[in] lbn Block to read.
   * \param[in] option mode for cached block.
   * \return Address of cached block. */
  cache_t* read(uint32_t lbn, uint8_t option);
  /** Write current block if dirty.
   * \return true for success else false.
   */
  bool sync();

 private:
  uint8_t m_status;
  FatVolume* m_vol;
  uint32_t m_lbn;
  cache_t m_block;
};
//==============================================================================
/**
 * \class FatVolume
 * \brief Access FAT16 and FAT32 volumes on raw file devices.
 */
class FatVolume {
 public:
  /** Create an instance of FatVolume
   */
  FatVolume() : m_fatType(0) {}

  /** \return The volume's cluster size in blocks. */
  uint8_t blocksPerCluster() const {
    return m_blocksPerCluster;
  }
  /** \return The number of blocks in one FAT. */
  uint32_t blocksPerFat()  const {
    return m_blocksPerFat;
  }
  /** Clear the cache and returns a pointer to the cache.  Not for normal apps.
   * \return A pointer to the cache buffer or zero if an error occurs.
   */
  cache_t* cacheClear() {
    if (!cacheSync()) {
      return 0;
    }
    m_cache.invalidate();
    return m_cache.block();
  }
  /** \return The total number of clusters in the volume. */
  uint32_t clusterCount() const {
    return m_lastCluster - 1;
  }
  /** \return The shift count required to multiply by blocksPerCluster. */
  uint8_t clusterSizeShift() const {
    return m_clusterSizeShift;
  }
  /** \return The logical block number for the start of file data. */
  uint32_t dataStartBlock() const {
    return m_dataStartBlock;
  }
  /** \return The sector number for the start of file data. */
  uint32_t dataStartSector() const {
    return m_dataStartBlock;
  }
  /** \return The number of File Allocation Tables. */
  uint8_t fatCount() {
    return 2;
  }
  /** \return The logical block number for the start of the first FAT. */
  uint32_t fatStartBlock() const {
    return m_fatStartBlock;
  }
  /** \return The sector number for the start of the first FAT. */
  uint32_t fatStartSector() const {
    return m_fatStartBlock;
  }
  /** \return The FAT type of the volume. Values are 12, 16 or 32. */
  uint8_t fatType() const {
    return m_fatType;
  }
  /** Volume free space in clusters.
   *
   * \return Count of free clusters for success or -1 if an error occurs.
   */
  int32_t freeClusterCount();
  /** Initialize a FAT volume.  Try partition one first then try super
   * floppy format.
   *
   * \return The value true is returned for success and
   * the value false is returned for failure.
   */
  bool init() {
    return init(1) || init(0);
  }
  /** Initialize a FAT volume.

   * \param[in] part The partition to be used.  Legal values for \a part are
   * 1-4 to use the corresponding partition on a device formatted with
   * a MBR, Master Boot Record, or zero if the device is formatted as
   * a super floppy with the FAT boot sector in block zero.
   *
   * \return The value true is returned for success and
   * the value false is returned for failure.
   */
  bool init(uint8_t part);
  /** \return The cluster number of last cluster in the volume. */
  uint32_t lastCluster() const {
    return m_lastCluster;
  }
  /** \return The number of entries in the root directory for FAT16 volumes. */
  uint16_t rootDirEntryCount() const {
    return m_rootDirEntryCount;
  }
  /** \return The logical block number for the start of the root directory
       on FAT16 volumes or the first cluster number on FAT32 volumes. */
  uint32_t rootDirStart() const {
    return m_rootDirStart;
  }
  /** \return The volume's cluster size in sectors. */
  uint8_t sectorsPerCluster() const {
    return m_blocksPerCluster;
  }
  /** \return The number of blocks in the volume */
  uint32_t volumeBlockCount() const {
    return blocksPerCluster()*clusterCount();
  }
  /** \return The number of sectors in the volume */
  uint32_t volumeSectorCount() const {
    return sectorsPerCluster()*clusterCount();
  }
  /** Wipe all data from the volume.
   * \param[in] pr print stream for status dots.
   * \return true for success else false.
   */
  bool wipe(print_t* pr = 0);
  /** Debug access to FAT table
   *
   * \param[in] n cluster number.
   * \param[out] v value of entry
   * \return -1 error, 0 EOC, else 1.
   */
  int8_t dbgFat(uint32_t n, uint32_t* v) {
    return fatGet(n, v);
  }
//------------------------------------------------------------------------------
 private:
  // Allow FatFile and FatCache access to FatVolume private functions.
  friend class FatCache;       ///< Allow access to FatVolume.
  friend class FatFile;        ///< Allow access to FatVolume.
  friend class FatFileSystem;  ///< Allow access to FatVolume.
//------------------------------------------------------------------------------
  BlockDriver* m_blockDev;      // block device
  uint8_t  m_blocksPerCluster;     // Cluster size in blocks.
  uint8_t  m_clusterBlockMask;     // Mask to extract block of cluster.
  uint8_t  m_clusterSizeShift;     // Cluster count to block count shift.
  uint8_t  m_fatType;              // Volume type (12, 16, OR 32).
  uint8_t  m_fatCount;             // Number of FAT (1 or 2)
  uint16_t m_rootDirEntryCount;    // Number of entries in FAT16 root dir.
  uint32_t m_allocSearchStart;     // Start cluster for alloc search.
  uint32_t m_blocksPerFat;         // FAT size in blocks
  uint32_t m_dataStartBlock;       // First data block number.
  uint32_t m_fatStartBlock;        // Start block for first FAT.
  uint32_t m_lastCluster;          // Last cluster number in FAT.
  uint32_t m_rootDirStart;         // Start block for FAT16, cluster for FAT32.
//------------------------------------------------------------------------------
  // block I/O functions.
  bool readBlock(uint32_t block, uint8_t* dst) {
    return m_blockDev->readBlock(block, dst);
  }
  bool syncBlocks() {
    return m_blockDev->syncBlocks();
  }
  bool writeBlock(uint32_t block, const uint8_t* src) {
    return m_blockDev->writeBlock(block, src);
  }
#if USE_MULTI_BLOCK_IO
  bool readBlocks(uint32_t block, uint8_t* dst, size_t nb) {
    return m_blockDev->readBlocks(block, dst, nb);
  }
  bool writeBlocks(uint32_t block, const uint8_t* src, size_t nb) {
    return m_blockDev->writeBlocks(block, src, nb);
  }
#endif  // USE_MULTI_BLOCK_IO
#if MAINTAIN_FREE_CLUSTER_COUNT
  int32_t  m_freeClusterCount;     // Count of free clusters in volume.
  void setFreeClusterCount(int32_t value) {
    m_freeClusterCount = value;
  }
  void updateFreeClusterCount(int32_t change) {
    if (m_freeClusterCount >= 0) {
      m_freeClusterCount += change;
    }
  }
#else  // MAINTAIN_FREE_CLUSTER_COUNT
  void setFreeClusterCount(int32_t value) {
    (void)value;
  }
  void updateFreeClusterCount(int32_t change) {
    (void)change;
  }
#endif  // MAINTAIN_FREE_CLUSTER_COUNT

// block caches
  FatCache m_cache;
#if USE_SEPARATE_FAT_CACHE
  FatCache m_fatCache;
  cache_t* cacheFetchFat(uint32_t blockNumber, uint8_t options) {
    return m_fatCache.read(blockNumber,
                           options | FatCache::CACHE_STATUS_MIRROR_FAT);
  }
  bool cacheSync() {
    return m_cache.sync() && m_fatCache.sync() && syncBlocks();
  }
#else  //
  cache_t* cacheFetchFat(uint32_t blockNumber, uint8_t options) {
    return cacheFetchData(blockNumber,
                          options | FatCache::CACHE_STATUS_MIRROR_FAT);
  }
  bool cacheSync() {
    return m_cache.sync() && syncBlocks();
  }
#endif  // USE_SEPARATE_FAT_CACHE
  cache_t* cacheFetchData(uint32_t blockNumber, uint8_t options) {
    return m_cache.read(blockNumber, options);
  }
  void cacheInvalidate() {
    m_cache.invalidate();
  }
  bool cacheSyncData() {
    return m_cache.sync();
  }
  cache_t *cacheAddress() {
    return m_cache.block();
  }
  uint32_t cacheBlockNumber() {
    return m_cache.lbn();
  }
  void cacheDirty() {
    m_cache.dirty();
  }
//------------------------------------------------------------------------------
  bool allocateCluster(uint32_t current, uint32_t* next);
  bool allocContiguous(uint32_t count,
                       uint32_t* firstCluster, uint32_t startCluster = 0);
  uint8_t blockOfCluster(uint32_t position) const {
    return (position >> 9) & m_clusterBlockMask;
  }
  uint32_t clusterFirstBlock(uint32_t cluster) const;
  int8_t fatGet(uint32_t cluster, uint32_t* value);
  bool fatPut(uint32_t cluster, uint32_t value);
  bool fatPutEOC(uint32_t cluster) {
    return fatPut(cluster, 0x0FFFFFFF);
  }
  bool freeChain(uint32_t cluster);
  bool isEOC(uint32_t cluster) const {
    return cluster > m_lastCluster;
  }
};
#endif  // FatVolume
