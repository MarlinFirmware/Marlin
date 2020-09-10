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
 * \file
 * \brief FAT file structures
 */

/**
 * Arduino SdFat Library
 * Copyright (c) 2009 by William Greiman
 *
 * This file is part of the Arduino Sd2Card Library
 */

#include <stdint.h>

#define PACKED __attribute__((__packed__))

/**
 * mostly from Microsoft document fatgen103.doc
 * https://www.microsoft.com/whdc/system/platform/firmware/fatgen.mspx
 */

uint8_t const BOOTSIG0 = 0x55,          // Value for byte 510 of boot block or MBR
              BOOTSIG1 = 0xAA,          // Value for byte 511 of boot block or MBR
              EXTENDED_BOOT_SIG = 0x29; // Value for bootSignature field int FAT/FAT32 boot sector

/**
 * \struct partitionTable
 * \brief MBR partition table entry
 *
 * A partition table entry for a MBR formatted storage device.
 * The MBR partition table has four entries.
 */
struct partitionTable {
  /**
   * Boot Indicator . Indicates whether the volume is the active
   * partition.  Legal values include: 0x00. Do not use for booting.
   * 0x80 Active partition.
   */
  uint8_t  boot;
  /**
   * Head part of Cylinder-head-sector address of the first block in
   * the partition. Legal values are 0-255. Only used in old PC BIOS.
   */
  uint8_t  beginHead;
  /**
   * Sector part of Cylinder-head-sector address of the first block in
   * the partition. Legal values are 1-63. Only used in old PC BIOS.
   */
  unsigned beginSector : 6;
  /** High bits cylinder for first block in partition. */
  unsigned beginCylinderHigh : 2;
  /**
   * Combine beginCylinderLow with beginCylinderHigh. Legal values
   * are 0-1023.  Only used in old PC BIOS.
   */
  uint8_t  beginCylinderLow;
  /**
   * Partition type. See defines that begin with PART_TYPE_ for
   * some Microsoft partition types.
   */
  uint8_t  type;
  /**
   * head part of cylinder-head-sector address of the last sector in the
   * partition.  Legal values are 0-255. Only used in old PC BIOS.
   */
  uint8_t  endHead;
  /**
   * Sector part of cylinder-head-sector address of the last sector in
   * the partition.  Legal values are 1-63. Only used in old PC BIOS.
   */
  unsigned endSector : 6;
  /** High bits of end cylinder */
  unsigned endCylinderHigh : 2;
  /**
   * Combine endCylinderLow with endCylinderHigh. Legal values
   * are 0-1023.  Only used in old PC BIOS.
   */
  uint8_t  endCylinderLow;

  uint32_t firstSector;   // Logical block address of the first block in the partition.
  uint32_t totalSectors;  // Length of the partition, in blocks.
} PACKED;

typedef struct partitionTable part_t; // Type name for partitionTable

/**
 * \struct masterBootRecord
 *
 * \brief Master Boot Record
 *
 * The first block of a storage device that is formatted with a MBR.
 */
struct masterBootRecord {
  uint8_t  codeArea[440]; // Code Area for master boot program.
  uint32_t diskSignature; // Optional Windows NT disk signature. May contain boot code.
  uint16_t usuallyZero;   // Usually zero but may be more boot code.
  part_t   part[4];       // Partition tables.
  uint8_t  mbrSig0;       // First MBR signature byte. Must be 0x55
  uint8_t  mbrSig1;       // Second MBR signature byte. Must be 0xAA
} PACKED;
/** Type name for masterBootRecord */
typedef struct masterBootRecord mbr_t;

/**
 * \struct fat_boot
 *
 * \brief Boot sector for a FAT12/FAT16 volume.
 *
 */
struct fat_boot {
  /**
   * The first three bytes of the boot sector must be valid,
   * executable x 86-based CPU instructions. This includes a
   * jump instruction that skips the next nonexecutable bytes.
   */
  uint8_t jump[3];
  /**
   * This is typically a string of characters that identifies
   * the operating system that formatted the volume.
   */
  char    oemId[8];
  /**
   * The size of a hardware sector. Valid decimal values for this
   * field are 512, 1024, 2048, and 4096. For most disks used in
   * the United States, the value of this field is 512.
   */
  uint16_t bytesPerSector;
  /**
   * Number of sectors per allocation unit. This value must be a
   * power of 2 that is greater than 0. The legal values are
   * 1, 2, 4, 8, 16, 32, 64, and 128.  128 should be avoided.
   */
  uint8_t  sectorsPerCluster;
  /**
   * The number of sectors preceding the start of the first FAT,
   * including the boot sector. The value of this field is always 1.
   */
  uint16_t reservedSectorCount;
  /**
   * The number of copies of the FAT on the volume.
   * The value of this field is always 2.
   */
  uint8_t  fatCount;
  /**
   * For FAT12 and FAT16 volumes, this field contains the count of
   * 32-byte directory entries in the root directory. For FAT32 volumes,
   * this field must be set to 0. For FAT12 and FAT16 volumes, this
   * value should always specify a count that when multiplied by 32
   * results in a multiple of bytesPerSector.  FAT16 volumes should
   * use the value 512.
   */
  uint16_t rootDirEntryCount;
  /**
   * This field is the old 16-bit total count of sectors on the volume.
   * This count includes the count of all sectors in all four regions
   * of the volume. This field can be 0; if it is 0, then totalSectors32
   * must be nonzero.  For FAT32 volumes, this field must be 0. For
   * FAT12 and FAT16 volumes, this field contains the sector count, and
   * totalSectors32 is 0 if the total sector count fits
   * (is less than 0x10000).
   */
  uint16_t totalSectors16;
  /**
   * This dates back to the old MS-DOS 1.x media determination and is
   * no longer usually used for anything.  0xF8 is the standard value
   * for fixed (nonremovable) media. For removable media, 0xF0 is
   * frequently used. Legal values are 0xF0 or 0xF8-0xFF.
   */
  uint8_t  mediaType;
  /**
   * Count of sectors occupied by one FAT on FAT12/FAT16 volumes.
   * On FAT32 volumes this field must be 0, and sectorsPerFat32
   * contains the FAT size count.
   */
  uint16_t sectorsPerFat16;

  uint16_t sectorsPerTrack; // Sectors per track for interrupt 0x13. Not used otherwise.
  uint16_t headCount;       // Number of heads for interrupt 0x13. Not used otherwise.

  /**
   * Count of hidden sectors preceding the partition that contains this
   * FAT volume. This field is generally only relevant for media
   * visible on interrupt 0x13.
   */
  uint32_t hidddenSectors;
  /**
   * This field is the new 32-bit total count of sectors on the volume.
   * This count includes the count of all sectors in all four regions
   * of the volume.  This field can be 0; if it is 0, then
   * totalSectors16 must be nonzero.
   */
  uint32_t totalSectors32;
  /**
   * Related to the BIOS physical drive number. Floppy drives are
   * identified as 0x00 and physical hard disks are identified as
   * 0x80, regardless of the number of physical disk drives.
   * Typically, this value is set prior to issuing an INT 13h BIOS
   * call to specify the device to access. The value is only
   * relevant if the device is a boot device.
   */
  uint8_t  driveNumber;

  uint8_t  reserved1;       // used by Windows NT - should be zero for FAT
  uint8_t  bootSignature;   // 0x29 if next three fields are valid

  /**
   * A random serial number created when formatting a disk,
   * which helps to distinguish between disks.
   * Usually generated by combining date and time.
   */
  uint32_t volumeSerialNumber;
  /**
   * A field once used to store the volume label. The volume label
   * is now stored as a special file in the root directory.
   */
  char     volumeLabel[11];
  /**
   * A field with a value of either FAT, FAT12 or FAT16,
   * depending on the disk format.
   */
  char     fileSystemType[8];

  uint8_t  bootCode[448];   // X86 boot code
  uint8_t  bootSectorSig0;  // must be 0x55
  uint8_t  bootSectorSig1;  // must be 0xAA
} PACKED;

typedef struct fat_boot fat_boot_t;   // Type name for FAT Boot Sector

/**
 * \struct fat32_boot
 *
 * \brief Boot sector for a FAT32 volume.
 */
struct fat32_boot {
  /**
   * The first three bytes of the boot sector must be valid,
   * executable x 86-based CPU instructions. This includes a
   * jump instruction that skips the next nonexecutable bytes.
   */
  uint8_t jump[3];
  /**
   * This is typically a string of characters that identifies
   * the operating system that formatted the volume.
   */
  char    oemId[8];
  /**
   * The size of a hardware sector. Valid decimal values for this
   * field are 512, 1024, 2048, and 4096. For most disks used in
   * the United States, the value of this field is 512.
   */
  uint16_t bytesPerSector;
  /**
   * Number of sectors per allocation unit. This value must be a
   * power of 2 that is greater than 0. The legal values are
   * 1, 2, 4, 8, 16, 32, 64, and 128.  128 should be avoided.
   */
  uint8_t  sectorsPerCluster;
  /**
   * The number of sectors preceding the start of the first FAT,
   * including the boot sector. Must not be zero
   */
  uint16_t reservedSectorCount;
  /**
   * The number of copies of the FAT on the volume.
   * The value of this field is always 2.
   */
  uint8_t  fatCount;
  /**
   * FAT12/FAT16 only. For FAT32 volumes, this field must be set to 0.
   */
  uint16_t rootDirEntryCount;
  /**
   * For FAT32 volumes, this field must be 0.
   */
  uint16_t totalSectors16;
  /**
   * This dates back to the old MS-DOS 1.x media determination and is
   * no longer usually used for anything.  0xF8 is the standard value
   * for fixed (nonremovable) media. For removable media, 0xF0 is
   * frequently used. Legal values are 0xF0 or 0xF8-0xFF.
   */
  uint8_t  mediaType;
  /**
   * On FAT32 volumes this field must be 0, and sectorsPerFat32
   * contains the FAT size count.
   */
  uint16_t sectorsPerFat16;

  uint16_t sectorsPerTrack; // Sectors per track for interrupt 0x13. Not used otherwise.
  uint16_t headCount;       // Number of heads for interrupt 0x13. Not used otherwise.

  /**
   * Count of hidden sectors preceding the partition that contains this
   * FAT volume. This field is generally only relevant for media
   * visible on interrupt 0x13.
   */
  uint32_t hidddenSectors;
  /**
   * Contains the total number of sectors in the FAT32 volume.
   */
  uint32_t totalSectors32;
  /**
   * Count of sectors occupied by one FAT on FAT32 volumes.
   */
  uint32_t sectorsPerFat32;
  /**
   * This field is only defined for FAT32 media and does not exist on
   * FAT12 and FAT16 media.
   * Bits 0-3 -- Zero-based number of active FAT.
   *             Only valid if mirroring is disabled.
   * Bits 4-6 -- Reserved.
   * Bit 7  -- 0 means the FAT is mirrored at runtime into all FATs.
   *        -- 1 means only one FAT is active; it is the one referenced
   *             in bits 0-3.
   * Bits 8-15  -- Reserved.
   */
  uint16_t fat32Flags;
  /**
   * FAT32 version. High byte is major revision number.
   * Low byte is minor revision number. Only 0.0 define.
   */
  uint16_t fat32Version;
  /**
   * Cluster number of the first cluster of the root directory for FAT32.
   * This usually 2 but not required to be 2.
   */
  uint32_t fat32RootCluster;
  /**
   * Sector number of FSINFO structure in the reserved area of the
   * FAT32 volume. Usually 1.
   */
  uint16_t fat32FSInfo;
  /**
   * If nonzero, indicates the sector number in the reserved area
   * of the volume of a copy of the boot record. Usually 6.
   * No value other than 6 is recommended.
   */
  uint16_t fat32BackBootBlock;
  /**
   * Reserved for future expansion. Code that formats FAT32 volumes
   * should always set all of the bytes of this field to 0.
   */
  uint8_t  fat32Reserved[12];
  /**
   * Related to the BIOS physical drive number. Floppy drives are
   * identified as 0x00 and physical hard disks are identified as
   * 0x80, regardless of the number of physical disk drives.
   * Typically, this value is set prior to issuing an INT 13h BIOS
   * call to specify the device to access. The value is only
   * relevant if the device is a boot device.
   */
  uint8_t  driveNumber;

  uint8_t  reserved1;       // Used by Windows NT - should be zero for FAT
  uint8_t  bootSignature;   // 0x29 if next three fields are valid

  /**
   * A random serial number created when formatting a disk,
   * which helps to distinguish between disks.
   * Usually generated by combining date and time.
   */
  uint32_t volumeSerialNumber;
  /**
   * A field once used to store the volume label. The volume label
   * is now stored as a special file in the root directory.
   */
  char     volumeLabel[11];
  /**
   * A text field with a value of FAT32.
   */
  char     fileSystemType[8];

  uint8_t  bootCode[420];   // X86 boot code
  uint8_t  bootSectorSig0;  // must be 0x55
  uint8_t  bootSectorSig1;  // must be 0xAA

} PACKED;

typedef struct fat32_boot fat32_boot_t; // Type name for FAT32 Boot Sector

uint32_t const FSINFO_LEAD_SIG   = 0x41615252,  // 'AaRR' Lead signature for a FSINFO sector
               FSINFO_STRUCT_SIG = 0x61417272;  // 'aArr' Struct signature for a FSINFO sector

/**
 * \struct fat32_fsinfo
 *
 * \brief FSINFO sector for a FAT32 volume.
 *
 */
struct fat32_fsinfo {
  uint32_t  leadSignature;    // must be 0x52, 0x52, 0x61, 0x41 'RRaA'
  uint8_t  reserved1[480];    // must be zero
  uint32_t  structSignature;  // must be 0x72, 0x72, 0x41, 0x61 'rrAa'
          /**
           * Contains the last known free cluster count on the volume.
           * If the value is 0xFFFFFFFF, then the free count is unknown
           * and must be computed. Any other value can be used, but is
           * not necessarily correct. It should be range checked at least
           * to make sure it is <= volume cluster count.
           */
  uint32_t freeCount;
          /**
           * This is a hint for the FAT driver. It indicates the cluster
           * number at which the driver should start looking for free clusters.
           * If the value is 0xFFFFFFFF, then there is no hint and the driver
           * should start looking at cluster 2.
           */
  uint32_t nextFree;

  uint8_t  reserved2[12];     // must be zero
  uint8_t  tailSignature[4];  // must be 0x00, 0x00, 0x55, 0xAA
} PACKED;

typedef struct fat32_fsinfo fat32_fsinfo_t; // Type name for FAT32 FSINFO Sector

// End Of Chain values for FAT entries
uint16_t const FAT12EOC = 0xFFF,          // FAT12 end of chain value used by Microsoft.
               FAT12EOC_MIN = 0xFF8,      // Minimum value for FAT12 EOC.  Use to test for EOC.
               FAT16EOC = 0xFFFF,         // FAT16 end of chain value used by Microsoft.
               FAT16EOC_MIN = 0xFFF8;     // Minimum value for FAT16 EOC.  Use to test for EOC.
uint32_t const FAT32EOC = 0x0FFFFFFF,     // FAT32 end of chain value used by Microsoft.
               FAT32EOC_MIN = 0x0FFFFFF8, // Minimum value for FAT32 EOC.  Use to test for EOC.
               FAT32MASK = 0x0FFFFFFF;    // Mask a for FAT32 entry. Entries are 28 bits.

/**
 * \struct directoryEntry
 * \brief FAT short directory entry
 *
 * Short means short 8.3 name, not the entry size.
 *
 * Date Format. A FAT directory entry date stamp is a 16-bit field that is
 * basically a date relative to the MS-DOS epoch of 01/01/1980. Here is the
 * format (bit 0 is the LSB of the 16-bit word, bit 15 is the MSB of the
 * 16-bit word):
 *
 * Bits 9-15: Count of years from 1980, valid value range 0-127
 * inclusive (1980-2107).
 *
 * Bits 5-8: Month of year, 1 = January, valid value range 1-12 inclusive.
 *
 * Bits 0-4: Day of month, valid value range 1-31 inclusive.
 *
 * Time Format. A FAT directory entry time stamp is a 16-bit field that has
 * a granularity of 2 seconds. Here is the format (bit 0 is the LSB of the
 * 16-bit word, bit 15 is the MSB of the 16-bit word).
 *
 * Bits 11-15: Hours, valid value range 0-23 inclusive.
 *
 * Bits 5-10: Minutes, valid value range 0-59 inclusive.
 *
 * Bits 0-4: 2-second count, valid value range 0-29 inclusive (0 - 58 seconds).
 *
 * The valid time range is from Midnight 00:00:00 to 23:59:58.
 */
struct directoryEntry {
  /**
   * Short 8.3 name.
   *
   * The first eight bytes contain the file name with blank fill.
   * The last three bytes contain the file extension with blank fill.
   */
  uint8_t  name[11];
  /**
   * Entry attributes.
   *
   * The upper two bits of the attribute byte are reserved and should
   * always be set to 0 when a file is created and never modified or
   * looked at after that.  See defines that begin with DIR_ATT_.
   */
  uint8_t  attributes;
  /**
   * Reserved for use by Windows NT. Set value to 0 when a file is
   * created and never modify or look at it after that.
   */
  uint8_t  reservedNT;
  /**
   * The granularity of the seconds part of creationTime is 2 seconds
   * so this field is a count of tenths of a second and it's valid
   * value range is 0-199 inclusive. (WHG note - seems to be hundredths)
   */
  uint8_t  creationTimeTenths;

  uint16_t creationTime;    // Time file was created.
  uint16_t creationDate;    // Date file was created.

  /**
   * Last access date. Note that there is no last access time, only
   * a date.  This is the date of last read or write. In the case of
   * a write, this should be set to the same date as lastWriteDate.
   */
  uint16_t lastAccessDate;
  /**
   * High word of this entry's first cluster number (always 0 for a
   * FAT12 or FAT16 volume).
   */
  uint16_t firstClusterHigh;

  uint16_t lastWriteTime;   // Time of last write. File creation is considered a write.
  uint16_t lastWriteDate;   // Date of last write. File creation is considered a write.
  uint16_t firstClusterLow; // Low word of this entry's first cluster number.
  uint32_t fileSize;        // 32-bit unsigned holding this file's size in bytes.
} PACKED;

/**
 * \struct directoryVFATEntry
 * \brief VFAT long filename directory entry
 *
 * directoryVFATEntries are found in the same list as normal directoryEntry.
 * But have the attribute field set to DIR_ATT_LONG_NAME.
 *
 * Long filenames are saved in multiple directoryVFATEntries.
 * Each entry containing 13 UTF-16 characters.
 */
struct directoryVFATEntry {
  /**
   * Sequence number. Consists of 2 parts:
   *  bit 6:   indicates first long filename block for the next file
   *  bit 0-4: the position of this long filename block (first block is 1)
   */
  uint8_t  sequenceNumber;

  uint16_t name1[5];        // First set of UTF-16 characters
  uint8_t  attributes;      // attributes (at the same location as in directoryEntry), always 0x0F
  uint8_t  reservedNT;      // Reserved for use by Windows NT. Always 0.
  uint8_t  checksum;        // Checksum of the short 8.3 filename, can be used to checked if the file system as modified by a not-long-filename aware implementation.
  uint16_t name2[6];        // Second set of UTF-16 characters
  uint16_t firstClusterLow; // firstClusterLow is always zero for longFilenames
  uint16_t name3[2];        // Third set of UTF-16 characters
} PACKED;

// Definitions for directory entries
//
typedef struct directoryEntry dir_t;          // Type name for directoryEntry
typedef struct directoryVFATEntry vfat_t;     // Type name for directoryVFATEntry

uint8_t const DIR_NAME_0xE5     = 0x05,       // escape for name[0] = 0xE5
              DIR_NAME_DELETED  = 0xE5,       // name[0] value for entry that is free after being "deleted"
              DIR_NAME_FREE     = 0x00,       // name[0] value for entry that is free and no allocated entries follow
              DIR_ATT_READ_ONLY = 0x01,       // file is read-only
              DIR_ATT_HIDDEN    = 0x02,       // File should hidden in directory listings
              DIR_ATT_SYSTEM    = 0x04,       // Entry is for a system file
              DIR_ATT_VOLUME_ID = 0x08,       // Directory entry contains the volume label
              DIR_ATT_DIRECTORY = 0x10,       // Entry is for a directory
              DIR_ATT_ARCHIVE   = 0x20,       // Old DOS archive bit for backup support
              DIR_ATT_LONG_NAME = 0x0F,       // Test value for long name entry.  Test is (d->attributes & DIR_ATT_LONG_NAME_MASK) == DIR_ATT_LONG_NAME.
              DIR_ATT_LONG_NAME_MASK = 0x3F,  // Test mask for long name entry
              DIR_ATT_DEFINED_BITS = 0x3F;    // defined attribute bits

/**
 * Directory entry is part of a long name
 * \param[in] dir Pointer to a directory entry.
 *
 * \return true if the entry is for part of a long name else false.
 */
static inline uint8_t DIR_IS_LONG_NAME(const dir_t* dir) {
  return (dir->attributes & DIR_ATT_LONG_NAME_MASK) == DIR_ATT_LONG_NAME;
}

/** Mask for file/subdirectory tests */
uint8_t const DIR_ATT_FILE_TYPE_MASK = (DIR_ATT_VOLUME_ID | DIR_ATT_DIRECTORY);

/**
 * Directory entry is for a file
 * \param[in] dir Pointer to a directory entry.
 *
 * \return true if the entry is for a normal file else false.
 */
static inline uint8_t DIR_IS_FILE(const dir_t* dir) {
  return (dir->attributes & DIR_ATT_FILE_TYPE_MASK) == 0;
}

/**
 * Directory entry is for a subdirectory
 * \param[in] dir Pointer to a directory entry.
 *
 * \return true if the entry is for a subdirectory else false.
 */
static inline uint8_t DIR_IS_SUBDIR(const dir_t* dir) {
  return (dir->attributes & DIR_ATT_FILE_TYPE_MASK) == DIR_ATT_DIRECTORY;
}

/**
 * Directory entry is for a file or subdirectory
 * \param[in] dir Pointer to a directory entry.
 *
 * \return true if the entry is for a normal file or subdirectory else false.
 */
static inline uint8_t DIR_IS_FILE_OR_SUBDIR(const dir_t* dir) {
  return (dir->attributes & DIR_ATT_VOLUME_ID) == 0;
}
