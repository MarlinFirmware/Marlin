# DWIN_ICO
# -  Dissect and create DWIN .ico files for their LCD displays.
#
#  Copyright (c) 2020 Brent Burton
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <https://www.gnu.org/licenses/>.
#----------------------------------------------------------------
#
# This is not a normal Microsoft .ICO file, but it has a similar
# structure for containing a number of icon images. Each icon is
# a small JPG file.
#
# The file has a directory header containing fixed-length
# records, and each record points to its data at an offset later
# in the file.
#
# The directory entries are 16 bytes each, and the entire
# directory is 4KB (0 - 0x1000). This supports 256 entries.
#
# Multibyte values are in Big Endian format.
#
# Header: (offset 0x0)
#              W    H     offset  ?? len  ?? ??
#   Entry 0:  xxxx xxxx 00001000 xx 10a2 00 00000000
#   Entry 1:  xxxx xxxx 000020a2 xx 0eac 00 00000000
#   Entry 2:  xxxx xxxx 00002f4e xx 0eaa 00 00000000
#   ...
# 0x00001000: ffd8 ffe1 0018 ... jpeg exif and data follow .. ffd9
# 0x000020a2: ffd8 ffe1 ...
# ...rest of ICO entries' data...
#
# Header structure:
# Offset Len  What
#      0   2  width
#      2   2  height
#      4   4  file byte position from SEEK_BEG
#      8   3  length of data
#     11   5  ??? all zeroes
#
# Other notes:
# * The index of each icon corresponds to the Icon number in dwin.h
# * One exception is number 39: that header entry is blank, and dwin.h
#   does not define a name for 39. This is specially handled to
#   prevent reordering stock icons.

import os
import struct
from PIL import Image

def getJpegResolution(jpegFile):
    """Returns a 2-tuple containing the jpegFile's (width, height).
    """
    img = Image.open(jpegFile)
    return img.size

class DWIN_ICO_File():
    def __init__(self):
        self.entries = []    # list of header entries

    def splitFile(self, filename, outDir):
        if not filename[-4:].lower() == '.ico':
            raise RuntimeError('Input file must end in .ico')

        with open(filename, 'rb') as infile:
            self._parseHeader(infile)
            self._splitEntryData(infile, outDir)

        return

    def _parseHeader(self, infile):
        maxEntries = 256
        count = 0
        validEntries = 0
        while count < maxEntries:
            rawBytes = infile.read(16)
            entry = Entry()
            entry.parseRawData(rawBytes)
            # check that it is valid: is offset nonzero?
            # Special case: treat 39 as valid
            if (entry.offset > 0) or (count == 39):
                validEntries += 1
                self.entries.append(entry)
            count += 1
        return

    def _splitEntryData(self, infile, outDir):
        print('Splitting Entry Data...')
        if 0 == len(self.entries):
            raise RuntimeError('.ico file is not loaded yet')

        # check for output dir:
        if not os.path.exists(outDir):
            os.mkdir(outDir)

        # keep a count
        count = 0
        for entry in self.entries:
            # Skip any empty entries. (Special handling of 39.)
            if entry.length == 0:
                count += 1
                continue
            # Seek file position, read length bytes, and write to new output file.
            print('%02d: offset: 0x%06x len: 0x%04x width: %d height: %d' %
                  (count, entry.offset, entry.length, entry.width, entry.height))
            outfilename = os.path.join(outDir,
                                       '%03d-%s.jpg' % (count, _iconNames[count]))
            with open(outfilename, 'wb') as outfile:
                infile.seek(entry.offset)
                blob = infile.read(entry.length)
                outfile.write(blob)
                print('Wrote %d bytes to %s' % (entry.length, outfilename))

            count += 1
        return

    def createFile(self, iconDir, filename):
        '''Create a new .ico file from the contents of iconDir.

        The contents of iconDir are processed to get image
        resolution, and a new entry is created for each.

        Each filename must have a leading number followed by a
        dash, which is the icon index. E.g., "071-ICON_StepX.jpg".
        '''
        self.entries = [Entry() for i in range(0,256)]
        # 1. Scan icon directory and record all valid files
        print('Scanning icon directory', iconDir)
        count = 0
        for dirEntry in os.scandir(iconDir):
            if not dirEntry.is_file():
                print('...Ignoring', dirEntry.path)
                continue
            # process each file:
            try:
                index = int(dirEntry.name[0:3])
                if (index < 0) or (index > 255):
                    print('...Ignoring invalid index on', dirEntry.path)
                    continue
                #dirEntry.path is iconDir/name
                w,h = getJpegResolution(dirEntry.path)
                length = dirEntry.stat().st_size
                e = self.entries[index]
                e.width = w
                e.height = h
                e.length = length
                e.filename = dirEntry.path
                count += 1
            except Exception as e:
                print('Whoops: ', e)
                pass
        print('...Scanned %d icon files' % (count))

        # 2. Scan over valid header entries and update offsets
        self._updateHeaderOffsets()

        # 3. Write out header to .ico file, the append each icon file
        self._combineAndWriteIcoFile(filename)
        print('Scanning done. %d icons included.' % (count))

    def _updateHeaderOffsets(self):
        """Iterate over all header entries and update their offsets.
        """
        offset = 256 * 16
        for i in range(0,256):
            e = self.entries[i]
            if e.length == 0:
                continue
            e.offset = offset
            offset += e.length
            #print('%03d: (%d x %d) len=%d off=%d' %
            #      (i, e.width, e.height, e.length, e.offset))
        return

    def _combineAndWriteIcoFile(self, filename):
        """Write out final .ico file.
        All header entries are updated, so write out
        the final header contents, and concat each icon
        file to the .ico.
        """
        with open(filename, 'wb') as outfile:
            # 1. Write header directory
            for e in self.entries:
                outfile.write( e.serialize() )
            if outfile.tell() != 4096:
                raise RuntimeError('Header directory write failed. Not 4096 bytes')
            # 2. For each entry, concat the icon file data
            for e in self.entries:
                if 0 == e.length: continue
                guts = self._getFileContents(e.filename, e.length)
                outfile.write(guts)
        return

    def _getFileContents(self, filename, length):
        """Read contents of filename, and return bytes"""
        with open(filename, 'rb') as infile:
            contents = infile.read(length)
            if len(contents) != length:
                raise RuntimeError('Failed to read contents of', filename)
            return contents

class Entry():
    '''Entry objects record resolution and size information
    about each icon stored in an ICO file.
    '''
    __slots__ = ('width', 'height', 'offset', 'length', 'filename')

    def __init__(self, w=0, h=0, length=0, offset=0):
        self.width = w
        self.height = h
        self.offset = offset
        self.length = length
        self.filename = None

    def parseRawData(self, rawEntryBytes):
        if len(rawEntryBytes) != 16:
            raise RuntimeError('Entry: data must be 16 bytes long')

        # Split data into bigendian fields
        (w, h, off, len3, len21, b1,b2,b3,b4,b5) = \
                struct.unpack('>HHLBHBBBBB', rawEntryBytes)
        self.width = w
        self.height = h
        self.offset = off
        self.length = len3 * 65536 + len21
        return

    def serialize(self):
        """Convert this Entry's information into a 16-byte
        .ico directory entry record. Return bytes object.
        """
        len21 = self.length % 65536
        len3  = self.length // 65536
        rawdata = struct.pack('>HHLBHBBBBB', self.width, self.height,
                              self.offset, len3, len21,
                              0, 0, 0, 0, 0)
        return rawdata

_iconNames = {
    0 : 'ICON_LOGO',
    1 : 'ICON_Print_0',
    2 : 'ICON_Print_1',
    3 : 'ICON_Prepare_0',
    4 : 'ICON_Prepare_1',
    5 : 'ICON_Control_0',
    6 : 'ICON_Control_1',
    7 : 'ICON_Leveling_0',
    8 : 'ICON_Leveling_1',
    9 : 'ICON_HotendTemp',
    10 : 'ICON_BedTemp',
    11 : 'ICON_Speed',
    12 : 'ICON_Zoffset',
    13 : 'ICON_Back',
    14 : 'ICON_File',
    15 : 'ICON_PrintTime',
    16 : 'ICON_RemainTime',
    17 : 'ICON_Setup_0',
    18 : 'ICON_Setup_1',
    19 : 'ICON_Pause_0',
    20 : 'ICON_Pause_1',
    21 : 'ICON_Continue_0',
    22 : 'ICON_Continue_1',
    23 : 'ICON_Stop_0',
    24 : 'ICON_Stop_1',
    25 : 'ICON_Bar',
    26 : 'ICON_More',
    27 : 'ICON_Axis',
    28 : 'ICON_CloseMotor',
    29 : 'ICON_Homing',
    30 : 'ICON_SetHome',
    31 : 'ICON_PLAPreheat',
    32 : 'ICON_ABSPreheat',
    33 : 'ICON_Cool',
    34 : 'ICON_Language',
    35 : 'ICON_MoveX',
    36 : 'ICON_MoveY',
    37 : 'ICON_MoveZ',
    38 : 'ICON_Extruder',
    # no 39
    40 : 'ICON_Temperature',
    41 : 'ICON_Motion',
    42 : 'ICON_WriteEEPROM',
    43 : 'ICON_ReadEEPROM',
    44 : 'ICON_ResumeEEPROM',
    45 : 'ICON_Info',
    46 : 'ICON_SetEndTemp',
    47 : 'ICON_SetBedTemp',
    48 : 'ICON_FanSpeed',
    49 : 'ICON_SetPLAPreheat',
    50 : 'ICON_SetABSPreheat',
    51 : 'ICON_MaxSpeed',
    52 : 'ICON_MaxAccelerated',
    53 : 'ICON_MaxJerk',
    54 : 'ICON_Step',
    55 : 'ICON_PrintSize',
    56 : 'ICON_Version',
    57 : 'ICON_Contact',
    58 : 'ICON_StockConfiguraton',
    59 : 'ICON_MaxSpeedX',
    60 : 'ICON_MaxSpeedY',
    61 : 'ICON_MaxSpeedZ',
    62 : 'ICON_MaxSpeedE',
    63 : 'ICON_MaxAccX',
    64 : 'ICON_MaxAccY',
    65 : 'ICON_MaxAccZ',
    66 : 'ICON_MaxAccE',
    67 : 'ICON_MaxSpeedJerkX',
    68 : 'ICON_MaxSpeedJerkY',
    69 : 'ICON_MaxSpeedJerkZ',
    70 : 'ICON_MaxSpeedJerkE',
    71 : 'ICON_StepX',
    72 : 'ICON_StepY',
    73 : 'ICON_StepZ',
    74 : 'ICON_StepE',
    75 : 'ICON_Setspeed',
    76 : 'ICON_SetZOffset',
    77 : 'ICON_Rectangle',
    78 : 'ICON_BLTouch',
    79 : 'ICON_TempTooLow',
    80 : 'ICON_AutoLeveling',
    81 : 'ICON_TempTooHigh',
    82 : 'ICON_NoTips_C',
    83 : 'ICON_NoTips_E',
    84 : 'ICON_Continue_C',
    85 : 'ICON_Continue_E',
    86 : 'ICON_Cancel_C',
    87 : 'ICON_Cancel_E',
    88 : 'ICON_Confirm_C',
    89 : 'ICON_Confirm_E',
    90 : 'ICON_Info_0',
    91 : 'ICON_Info_1'
    }
