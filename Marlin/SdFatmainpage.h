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

/**
\mainpage Arduino SdFat Library
<CENTER>Copyright &copy; 2009 by William Greiman
</CENTER>

\section Intro Introduction
The Arduino SdFat Library is a minimal implementation of FAT16 and FAT32
file systems on SD flash memory cards.  Standard SD and high capacity
SDHC cards are supported.

The SdFat only supports short 8.3 names.

The main classes in SdFat are Sd2Card, SdVolume, and SdFile.

The Sd2Card class supports access to standard SD cards and SDHC cards.  Most
applications will only need to call the Sd2Card::init() member function.

The SdVolume class supports FAT16 and FAT32 partitions.  Most applications
will only need to call the SdVolume::init() member function.

The SdFile class provides file access functions such as open(), read(),
remove(), write(), close() and sync(). This class supports access to the root
directory and subdirectories.

A number of example are provided in the SdFat/examples folder.  These were
developed to test SdFat and illustrate its use.

SdFat was developed for high speed data recording.  SdFat was used to implement
an audio record/play class, WaveRP, for the Adafruit Wave Shield.  This
application uses special Sd2Card calls to write to contiguous files in raw mode.
These functions reduce write latency so that audio can be recorded with the
small amount of RAM in the Arduino.

\section SDcard SD\SDHC Cards

Arduinos access SD cards using the cards SPI protocol.  PCs, Macs, and
most consumer devices use the 4-bit parallel SD protocol.  A card that
functions well on A PC or Mac may not work well on the Arduino.

Most cards have good SPI read performance but cards vary widely in SPI
write performance.  Write performance is limited by how efficiently the
card manages internal erase/remapping operations.  The Arduino cannot
optimize writes to reduce erase operations because of its limit RAM.

SanDisk cards generally have good write performance.  They seem to have
more internal RAM buffering than other cards and therefore can limit
the number of flash erase operations that the Arduino forces due to its
limited RAM.

\section Hardware Hardware Configuration

SdFat was developed using an
<A HREF = "http://www.adafruit.com/"> Adafruit Industries</A> 
<A HREF = "http://www.ladyada.net/make/waveshield/"> Wave Shield</A>.

The hardware interface to the SD card should not use a resistor based level
shifter.  SdFat sets the SPI bus frequency to 8 MHz which results in signal
rise times that are too slow for the edge detectors in many newer SD card
controllers when resistor voltage dividers are used.

The 5 to 3.3 V level shifter for 5 V Arduinos should be IC based like the
74HC4050N based circuit shown in the file SdLevel.png.  The Adafruit Wave Shield
uses a 74AHC125N.  Gravitech sells SD and MicroSD Card Adapters based on the
74LCX245.

If you are using a resistor based level shifter and are having problems try
setting the SPI bus frequency to 4 MHz.  This can be done by using 
card.init(SPI_HALF_SPEED) to initialize the SD card.

\section comment Bugs and Comments

If you wish to report bugs or have comments, send email to fat16lib@sbcglobal.net.

\section SdFatClass SdFat Usage

SdFat uses a slightly restricted form of short names.
Only printable ASCII characters are supported. No characters with code point
values greater than 127 are allowed.  Space is not allowed even though space
was allowed in the API of early versions of DOS.

Short names are limited to 8 characters followed by an optional period (.)
and extension of up to 3 characters.  The characters may be any combination
of letters and digits.  The following special characters are also allowed:

$ % ' - _ @ ~ ` ! ( ) { } ^ # &

Short names are always converted to upper case and their original case
value is lost.

\note
  The Arduino Print class uses character
at a time writes so it was necessary to use a \link SdFile::sync() sync() \endlink
function to control when data is written to the SD card.

\par
An application which writes to a file using \link Print::print() print()\endlink,
\link Print::println() println() \endlink
or \link SdFile::write write() \endlink must call \link SdFile::sync() sync() \endlink
at the appropriate time to force data and directory information to be written
to the SD Card.  Data and directory information are also written to the SD card
when \link SdFile::close() close() \endlink is called.

\par
Applications must use care calling \link SdFile::sync() sync() \endlink
since 2048 bytes of I/O is required to update file and
directory information.  This includes writing the current data block, reading
the block that contains the directory entry for update, writing the directory
block back and reading back the current data block.

It is possible to open a file with two or more instances of SdFile.  A file may
be corrupted if data is written to the file by more than one instance of SdFile.

\section HowTo How to format SD Cards as FAT Volumes

You should use a freshly formatted SD card for best performance.  FAT
file systems become slower if many files have been created and deleted.
This is because the directory entry for a deleted file is marked as deleted,
but is not deleted.  When a new file is created, these entries must be scanned
before creating the file, a flaw in the FAT design.  Also files can become
fragmented which causes reads and writes to be slower.

Microsoft operating systems support removable media formatted with a
Master Boot Record, MBR, or formatted as a super floppy with a FAT Boot Sector
in block zero.

Microsoft operating systems expect MBR formatted removable media
to have only one partition. The first partition should be used.

Microsoft operating systems do not support partitioning SD flash cards.
If you erase an SD card with a program like KillDisk, Most versions of
Windows will format the card as a super floppy.

The best way to restore an SD card's format is to use SDFormatter
which can be downloaded from:

http://www.sdcard.org/consumers/formatter/

SDFormatter aligns flash erase boundaries with file
system structures which reduces write latency and file system overhead.

SDFormatter does not have an option for FAT type so it may format
small cards as FAT12.

After the MBR is restored by SDFormatter you may need to reformat small
cards that have been formatted FAT12 to force the volume type to be FAT16.

If you reformat the SD card with an OS utility, choose a cluster size that
will result in:

4084 < CountOfClusters && CountOfClusters < 65525

The volume will then be FAT16.

If you are formatting an SD card on OS X or Linux, be sure to use the first
partition. Format this partition with a cluster count in above range.

\section  References References

Adafruit Industries:

http://www.adafruit.com/

http://www.ladyada.net/make/waveshield/

The Arduino site:

http://www.arduino.cc/

For more information about FAT file systems see:

http://www.microsoft.com/whdc/system/platform/firmware/fatgen.mspx

For information about using SD cards as SPI devices see:

http://www.sdcard.org/developers/tech/sdcard/pls/Simplified_Physical_Layer_Spec.pdf

The ATmega328 datasheet:

http://www.atmel.com/dyn/resources/prod_documents/doc8161.pdf
 

 */  
