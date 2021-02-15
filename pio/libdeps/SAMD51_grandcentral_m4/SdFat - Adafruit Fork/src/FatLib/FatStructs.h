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
#ifndef FatStructs_h
#define FatStructs_h
/**
 * \file
 * \brief FAT file structures
 */
/*
 * mostly from Microsoft document fatgen103.doc
 * http://www.microsoft.com/whdc/system/platform/firmware/fatgen.mspx
 */
//------------------------------------------------------------------------------
/** Value for byte 510 of boot block or MBR */
const uint8_t BOOTSIG0 = 0X55;
/** Value for byte 511 of boot block or MBR */
const uint8_t BOOTSIG1 = 0XAA;
/** Value for bootSignature field int FAT/FAT32 boot sector */
const uint8_t EXTENDED_BOOT_SIG = 0X29;
//------------------------------------------------------------------------------
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
   * partition.  Legal values include: 0X00. Do not use for booting.
   * 0X80 Active partition.
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
  /** Logical block address of the first block in the partition. */
  uint32_t firstSector;
  /** Length of the partition, in blocks. */
  uint32_t totalSectors;
} __attribute__((packed));
/** Type name for partitionTable */
typedef struct partitionTable part_t;
//------------------------------------------------------------------------------
/**
 * \struct masterBootRecord
 *
 * \brief Master Boot Record
 *
 * The first block of a storage device that is formatted with a MBR.
 */
struct masterBootRecord {
  /** Code Area for master boot program. */
  uint8_t  codeArea[440];
  /** Optional Windows NT disk signature. May contain boot code. */
  uint32_t diskSignature;
  /** Usually zero but may be more boot code. */
  uint16_t usuallyZero;
  /** Partition tables. */
  part_t   part[4];
  /** First MBR signature byte. Must be 0X55 */
  uint8_t  mbrSig0;
  /** Second MBR signature byte. Must be 0XAA */
  uint8_t  mbrSig1;
} __attribute__((packed));
/** Type name for masterBootRecord */
typedef struct masterBootRecord mbr_t;
//------------------------------------------------------------------------------
/**
 * \struct biosParmBlock
 *
 * \brief BIOS parameter block
 *
 *  The BIOS parameter block describes the physical layout of a FAT volume.
 */
struct biosParmBlock {
          /**
           * Count of bytes per sector. This value may take on only the
           * following values: 512, 1024, 2048 or 4096
           */
  uint16_t bytesPerSector;
          /**
           * Number of sectors per allocation unit. This value must be a
           * power of 2 that is greater than 0. The legal values are
           * 1, 2, 4, 8, 16, 32, 64, and 128.
           */
  uint8_t  sectorsPerCluster;
          /**
           * Number of sectors before the first FAT.
           * This value must not be zero.
           */
  uint16_t reservedSectorCount;
          /** The count of FAT data structures on the volume. This field should
           *  always contain the value 2 for any FAT volume of any type.
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
           /** Sectors per track for interrupt 0x13. Not used otherwise. */
  uint16_t sectorsPerTrtack;
           /** Number of heads for interrupt 0x13.  Not used otherwise. */
  uint16_t headCount;
          /**
           * Count of hidden sectors preceding the partition that contains this
           * FAT volume. This field is generally only relevant for media
           *  visible on interrupt 0x13.
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
           * Count of sectors occupied by one FAT on FAT32 volumes.
           */
  uint32_t sectorsPerFat32;
          /**
           * This field is only defined for FAT32 media and does not exist on
           * FAT12 and FAT16 media.
           * Bits 0-3 -- Zero-based number of active FAT.
           *             Only valid if mirroring is disabled.
           * Bits 4-6 -- Reserved.
           * Bit 7	-- 0 means the FAT is mirrored at runtime into all FATs.
	         *        -- 1 means only one FAT is active; it is the one referenced in bits 0-3.
           * Bits 8-15 	-- Reserved.
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
} __attribute__((packed));
/** Type name for biosParmBlock */
typedef struct biosParmBlock bpb_t;
//------------------------------------------------------------------------------
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
   * jump instruction that skips the next non-executable bytes.
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
   * must be non-zero.  For FAT32 volumes, this field must be 0. For
   * FAT12 and FAT16 volumes, this field contains the sector count, and
   * totalSectors32 is 0 if the total sector count fits
   * (is less than 0x10000).
   */
  uint16_t totalSectors16;
  /**
   * This dates back to the old MS-DOS 1.x media determination and is
   * no longer usually used for anything.  0xF8 is the standard value
   * for fixed (non-removable) media. For removable media, 0xF0 is
   * frequently used. Legal values are 0xF0 or 0xF8-0xFF.
   */
  uint8_t  mediaType;
  /**
   * Count of sectors occupied by one FAT on FAT12/FAT16 volumes.
   * On FAT32 volumes this field must be 0, and sectorsPerFat32
   * contains the FAT size count.
   */
  uint16_t sectorsPerFat16;
  /** Sectors per track for interrupt 0x13. Not used otherwise. */
  uint16_t sectorsPerTrack;
  /** Number of heads for interrupt 0x13.  Not used otherwise. */
  uint16_t headCount;
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
   * totalSectors16 must be non-zero.
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
  /** used by Windows NT - should be zero for FAT */
  uint8_t  reserved1;
  /** 0X29 if next three fields are valid */
  uint8_t  bootSignature;
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
  /** X86 boot code */
  uint8_t  bootCode[448];
  /** must be 0X55 */
  uint8_t  bootSectorSig0;
  /** must be 0XAA */
  uint8_t  bootSectorSig1;
} __attribute__((packed));
/** Type name for FAT Boot Sector */
typedef struct fat_boot fat_boot_t;
//------------------------------------------------------------------------------
/**
 * \struct fat32_boot
 *
 * \brief Boot sector for a FAT32 volume.
 *
 */
struct fat32_boot {
  /**
   * The first three bytes of the boot sector must be valid,
   * executable x 86-based CPU instructions. This includes a
   * jump instruction that skips the next non-executable bytes.
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
   * for fixed (non-removable) media. For removable media, 0xF0 is
   * frequently used. Legal values are 0xF0 or 0xF8-0xFF.
   */
  uint8_t  mediaType;
  /**
   * On FAT32 volumes this field must be 0, and sectorsPerFat32
   * contains the FAT size count.
   */
  uint16_t sectorsPerFat16;
  /** Sectors per track for interrupt 0x13. Not used otherwise. */
  uint16_t sectorsPerTrack;
  /** Number of heads for interrupt 0x13.  Not used otherwise. */
  uint16_t headCount;
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
   * Bit 7	-- 0 means the FAT is mirrored at runtime into all FATs.
   *        -- 1 means only one FAT is active; it is the one referenced
   *             in bits 0-3.
   * Bits 8-15 	-- Reserved.
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
   * If non-zero, indicates the sector number in the reserved area
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
  /** used by Windows NT - should be zero for FAT */
  uint8_t  reserved1;
  /** 0X29 if next three fields are valid */
  uint8_t  bootSignature;
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
  /** X86 boot code */
  uint8_t  bootCode[420];
  /** must be 0X55 */
  uint8_t  bootSectorSig0;
  /** must be 0XAA */
  uint8_t  bootSectorSig1;
} __attribute__((packed));
/** Type name for FAT32 Boot Sector */
typedef struct fat32_boot fat32_boot_t;
//------------------------------------------------------------------------------
/** Lead signature for a FSINFO sector */
const uint32_t FSINFO_LEAD_SIG = 0x41615252;
/** Struct signature for a FSINFO sector */
const uint32_t FSINFO_STRUCT_SIG = 0x61417272;
/**
 * \struct fat32_fsinfo
 *
 * \brief FSINFO sector for a FAT32 volume.
 *
 */
struct fat32_fsinfo {
  /** must be 0X52, 0X52, 0X61, 0X41 */
  uint32_t  leadSignature;
  /** must be zero */
  uint8_t  reserved1[480];
  /** must be 0X72, 0X72, 0X41, 0X61 */
  uint32_t  structSignature;
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
  /** must be zero */
  uint8_t  reserved2[12];
  /** must be 0X00, 0X00, 0X55, 0XAA */
  uint8_t  tailSignature[4];
} __attribute__((packed));
/** Type name for FAT32 FSINFO Sector */
typedef struct fat32_fsinfo fat32_fsinfo_t;
//------------------------------------------------------------------------------
// End Of Chain values for FAT entries
/** FAT12 end of chain value used by Microsoft. */
const uint16_t FAT12EOC = 0XFFF;
/** Minimum value for FAT12 EOC.  Use to test for EOC. */
const uint16_t FAT12EOC_MIN = 0XFF8;
/** FAT16 end of chain value used by Microsoft. */
const uint16_t FAT16EOC = 0XFFFF;
/** Minimum value for FAT16 EOC.  Use to test for EOC. */
const uint16_t FAT16EOC_MIN = 0XFFF8;
/** FAT32 end of chain value used by Microsoft. */
const uint32_t FAT32EOC = 0X0FFFFFFF;
/** Minimum value for FAT32 EOC.  Use to test for EOC. */
const uint32_t FAT32EOC_MIN = 0X0FFFFFF8;
/** Mask a for FAT32 entry. Entries are 28 bits. */
const uint32_t FAT32MASK = 0X0FFFFFFF;
//------------------------------------------------------------------------------
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
  /** Short 8.3 name.
   *
   * The first eight bytes contain the file name with blank fill.
   * The last three bytes contain the file extension with blank fill.
   */
  uint8_t  name[11];
  /** Entry attributes.
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
   * so this field is a count of tenths of a second and its valid
   * value range is 0-199 inclusive. (WHG note - seems to be hundredths)
   */
  uint8_t  creationTimeTenths;
  /** Time file was created. */
  uint16_t creationTime;
  /** Date file was created. */
  uint16_t creationDate;
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
  /** Time of last write. File creation is considered a write. */
  uint16_t lastWriteTime;
  /** Date of last write. File creation is considered a write. */
  uint16_t lastWriteDate;
  /** Low word of this entry's first cluster number. */
  uint16_t firstClusterLow;
  /** 32-bit unsigned holding this file's size in bytes. */
  uint32_t fileSize;
} __attribute__((packed));
/** Type name for directoryEntry */
typedef struct directoryEntry dir_t;
//------------------------------------------------------------------------------
// Definitions for directory entries
//
/** escape for name[0] = 0XE5 */
const uint8_t DIR_NAME_0XE5 = 0X05;
/** name[0] value for entry that is free after being "deleted" */
const uint8_t DIR_NAME_DELETED = 0XE5;
/** name[0] value for entry that is free and no allocated entries follow */
const uint8_t DIR_NAME_FREE = 0X00;
/** file is read-only */
const uint8_t DIR_ATT_READ_ONLY = 0X01;
/** File should e hidden in directory listings */
const uint8_t DIR_ATT_HIDDEN = 0X02;
/** Entry is for a system file */
const uint8_t DIR_ATT_SYSTEM = 0X04;
/** Directory entry contains the volume label */
const uint8_t DIR_ATT_VOLUME_ID = 0X08;
/** Entry is for a directory */
const uint8_t DIR_ATT_DIRECTORY = 0X10;
/** Old DOS archive bit for backup support */
const uint8_t DIR_ATT_ARCHIVE = 0X20;
/** Test value for long name entry.  Test is
  (d->attributes & DIR_ATT_LONG_NAME_MASK) == DIR_ATT_LONG_NAME. */
const uint8_t DIR_ATT_LONG_NAME = 0X0F;
/** Test mask for long name entry */
const uint8_t DIR_ATT_LONG_NAME_MASK = 0X3F;
/** defined attribute bits */
const uint8_t DIR_ATT_DEFINED_BITS = 0X3F;

/** Mask for file/subdirectory tests */
const uint8_t DIR_ATT_FILE_TYPE_MASK = (DIR_ATT_VOLUME_ID | DIR_ATT_DIRECTORY);

/** Filename base-name is all lower case */
const uint8_t DIR_NT_LC_BASE = 0X08;
/** Filename extension is all lower case.*/
const uint8_t DIR_NT_LC_EXT = 0X10;


/** Directory entry is for a file
 * \param[in] dir Pointer to a directory entry.
 *
 * \return true if the entry is for a normal file else false.
 */
static inline uint8_t DIR_IS_FILE(const dir_t* dir) {
  return (dir->attributes & DIR_ATT_FILE_TYPE_MASK) == 0;
}
/** Directory entry is for a file or subdirectory
 * \param[in] dir Pointer to a directory entry.
 *
 * \return true if the entry is for a normal file or subdirectory else false.
 */
static inline uint8_t DIR_IS_FILE_OR_SUBDIR(const dir_t* dir) {
  return (dir->attributes & DIR_ATT_VOLUME_ID) == 0;
}
/** Directory entry is part of a long name
 * \param[in] dir Pointer to a directory entry.
 *
 * \return true if the entry is for part of a long name else false.
 */
static inline uint8_t DIR_IS_LONG_NAME(const dir_t* dir) {
  return dir->attributes == DIR_ATT_LONG_NAME;
}
/** Directory entry is hidden
 * \param[in] dir Pointer to a directory entry.
 *
 * \return true if the entry is hidden else false.
 */
static inline uint8_t DIR_IS_HIDDEN(const dir_t* dir) {
  return dir->attributes & DIR_ATT_HIDDEN;
}
/** Directory entry is for a subdirectory
 * \param[in] dir Pointer to a directory entry.
 *
 * \return true if the entry is for a subdirectory else false.
 */
static inline uint8_t DIR_IS_SUBDIR(const dir_t* dir) {
  return (dir->attributes & DIR_ATT_FILE_TYPE_MASK) == DIR_ATT_DIRECTORY;
}
/** Directory entry is system type
 * \param[in] dir Pointer to a directory entry.
 *
 * \return true if the entry is system else false.
 */
static inline uint8_t DIR_IS_SYSTEM(const dir_t* dir) {
  return dir->attributes & DIR_ATT_SYSTEM;
}
/** date field for FAT directory entry
 * \param[in] year [1980,2107]
 * \param[in] month [1,12]
 * \param[in] day [1,31]
 *
 * \return Packed date for dir_t entry.
 */
static inline uint16_t FAT_DATE(uint16_t year, uint8_t month, uint8_t day) {
  return (year - 1980) << 9 | month << 5 | day;
}
/** year part of FAT directory date field
 * \param[in] fatDate Date in packed dir format.
 *
 * \return Extracted year [1980,2107]
 */
static inline uint16_t FAT_YEAR(uint16_t fatDate) {
  return 1980 + (fatDate >> 9);
}
/** month part of FAT directory date field
 * \param[in] fatDate Date in packed dir format.
 *
 * \return Extracted month [1,12]
 */
static inline uint8_t FAT_MONTH(uint16_t fatDate) {
  return (fatDate >> 5) & 0XF;
}
/** day part of FAT directory date field
 * \param[in] fatDate Date in packed dir format.
 *
 * \return Extracted day [1,31]
 */
static inline uint8_t FAT_DAY(uint16_t fatDate) {
  return fatDate & 0X1F;
}
/** time field for FAT directory entry
 * \param[in] hour [0,23]
 * \param[in] minute [0,59]
 * \param[in] second [0,59]
 *
 * \return Packed time for dir_t entry.
 */
static inline uint16_t FAT_TIME(uint8_t hour, uint8_t minute, uint8_t second) {
  return hour << 11 | minute << 5 | second >> 1;
}
/** hour part of FAT directory time field
 * \param[in] fatTime Time in packed dir format.
 *
 * \return Extracted hour [0,23]
 */
static inline uint8_t FAT_HOUR(uint16_t fatTime) {
  return fatTime >> 11;
}
/** minute part of FAT directory time field
 * \param[in] fatTime Time in packed dir format.
 *
 * \return Extracted minute [0,59]
 */
static inline uint8_t FAT_MINUTE(uint16_t fatTime) {
  return (fatTime >> 5) & 0X3F;
}
/** second part of FAT directory time field
 * \note second/2 is stored in packed time.
 *
 * \param[in] fatTime Time in packed dir format.
 *
 * \return Extracted second [0,58]
 */
static inline uint8_t FAT_SECOND(uint16_t fatTime) {
  return 2*(fatTime & 0X1F);
}
/** Default date for file timestamps is 1 Jan 2000 */
const uint16_t FAT_DEFAULT_DATE = ((2000 - 1980) << 9) | (1 << 5) | 1;
/** Default time for file timestamp is 1 am */
const uint16_t FAT_DEFAULT_TIME = (1 << 11);
//------------------------------------------------------------------------------
/** Dimension of first name field in long directory entry */
const uint8_t LDIR_NAME1_DIM = 5;
/** Dimension of first name field in long directory entry */
const uint8_t LDIR_NAME2_DIM = 6;
/** Dimension of first name field in long directory entry */
const uint8_t LDIR_NAME3_DIM = 2;
/**
 * \struct longDirectoryEntry
 * \brief FAT long directory entry
 */
struct longDirectoryEntry {
  /**
   * The order of this entry in the sequence of long dir entries
   * associated with the short dir entry at the end of the long dir set.
   *
   * If masked with 0X40 (LAST_LONG_ENTRY), this indicates the
   * entry is the last long dir entry in a set of long dir entries.
   * All valid sets of long dir entries must begin with an entry having
   * this mask.
   */
  uint8_t ord;
  /** Characters 1-5 of the long-name sub-component in this entry. */
  uint16_t name1[LDIR_NAME1_DIM];
  /** Attributes - must be ATTR_LONG_NAME */
  uint8_t attr;
  /**
   * If zero, indicates a directory entry that is a sub-component of a
   * long name. NOTE: Other values reserved for future extensions.
   *
   * Non-zero implies other directory entry types.
   */
  uint8_t type;
  /**
   * Checksum of name in the short dir entry at the end of the
   * long dir set.
   */
  uint8_t chksum;
  /** Characters 6-11 of the long-name sub-component in this entry. */
  uint16_t name2[LDIR_NAME2_DIM];
  /** Must be ZERO. This is an artifact of the FAT "first cluster" */
  uint16_t mustBeZero;
  /** Characters 12 and 13 of the long-name sub-component in this entry. */
  uint16_t name3[LDIR_NAME3_DIM];
} __attribute__((packed));
/** Type name for longDirectoryEntry */
typedef struct longDirectoryEntry ldir_t;
/**
 * Ord mast that indicates the entry is the last long dir entry in a
 * set of long dir entries. All valid sets of long dir entries must
 * begin with an entry having this mask.
 */
const uint8_t LDIR_ORD_LAST_LONG_ENTRY = 0X40;
#endif  // FatStructs_h
