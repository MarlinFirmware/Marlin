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
# * The index of each icon corresponds to the Icon number in dwin_set.h
#   (7.ICO) or marlinui/dwin_lcd.h (3.ICO, 4.ICO).
# * Entries with zero length are empty slots (e.g., 39 in 7.ICO).
#   All 256 entries are kept so that indexes never shift.

import os, re, struct
from PIL import Image

class DWIN_ICO_File():
    def __init__(self):
        self.entries = []    # list of header entries

    def splitFile(self, filename, outDir, names=None):
        if not filename[-4:].lower() == '.ico':
            raise RuntimeError('Input file must end in .ico')

        if names is None: names = iconNamesFor(filename)

        with open(filename, 'rb') as infile:
            self._parseHeader(infile)
            self._splitEntryData(infile, outDir, names)

    def _parseHeader(self, infile):
        # Keep all 256 entries so each list index is the icon index
        self.entries = []
        for count in range(256):
            rawBytes = infile.read(16)
            entry = Entry()
            entry.parseRawData(rawBytes)
            self.entries.append(entry)

    def _splitEntryData(self, infile, outDir, names):
        print('Splitting Entry Data...')
        if 0 == len(self.entries):
            raise RuntimeError('.ico file is not loaded yet')

        # check for output dir:
        if not os.path.exists(outDir):
            os.mkdir(outDir)

        for count, entry in enumerate(self.entries):
            # Skip empty entries
            if entry.length == 0: continue
            outfilename = os.path.join(outDir, '%03d-ICON_%s.jpg' % (count, names.get(count, "UNKNOWN")))
            with open(outfilename, 'wb') as outfile:
                # Seek file position, read length bytes, and write to new output file.
                infile.seek(entry.offset)
                blob = infile.read(entry.length)
                if len(blob) != entry.length:
                    raise RuntimeError('Icon %d data runs past the end of the file' % count)
                outfile.write(blob)
                print('(%3d: width=%3d height=%3d offset=%6d len=%4d) ... %s' %
                      (count, entry.width, entry.height, entry.offset, entry.length, os.path.basename(outfilename)))

    def createFile(self, iconDir, filename):
        """
        Create a new .ico file from the contents of iconDir.

        The contents of iconDir are processed to get image
        resolution, and a new entry is created for each.

        Each filename must have a leading number followed by a
        dash, which is the icon index. E.g., "071-ICON_StepX.jpg".
        """
        self.entries = [Entry() for i in range(0,256)]
        # 1. Scan icon directory and record all valid files
        print('Scanning icon directory', iconDir)
        count = 0
        for dirEntry in sorted(os.scandir(iconDir), key=lambda d: d.name):
            # Only consider files named like "071-ICON_StepX.jpg"
            m = re.match(r'^(\d{3})-.*\.jpe?g$', dirEntry.name, re.IGNORECASE)
            if not dirEntry.is_file() or not m:
                continue
            index = int(m.group(1))
            if not (0 <= index <= 255):
                raise RuntimeError('Invalid icon index in %s' % dirEntry.path)
            e = self.entries[index]
            if e.filename:
                raise RuntimeError('Duplicate icon index %03d: %s and %s' % (index, os.path.basename(e.filename), dirEntry.name))
            # dirEntry.path is iconDir/name
            img = Image.open(dirEntry.path)
            if img.format != 'JPEG':
                raise RuntimeError('%s is not a JPEG file' % dirEntry.path)
            if img.info.get('progressive') or img.info.get('progression'):
                raise RuntimeError('%s is a progressive JPEG (use baseline)' % dirEntry.path)
            e.width, e.height = img.size
            e.length = dirEntry.stat().st_size
            e.filename = dirEntry.path
            count += 1
        print('...Scanned %d icon files' % (count))
        if count == 0:
            raise RuntimeError('No icon files found in %s' % iconDir)

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
            # print('%03d: (%d x %d) len=%d off=%d' %
            #      (i, e.width, e.height, e.length, e.offset))

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

    def _getFileContents(self, filename, length):
        """Read contents of filename, and return bytes"""
        with open(filename, 'rb') as infile:
            contents = infile.read(length)
            if len(contents) != length:
                raise RuntimeError('Failed to read contents of %s' % filename)
            return contents

class Entry():
    """Entry objects record resolution and size information
    about each icon stored in an ICO file.
    """
    __slots__ = ('width', 'height', 'offset', 'length', 'filename')

    def __init__(self, w=0, h=0, length=0, offset=0, filename=None):
        self.width = w
        self.height = h
        self.offset = offset
        self.length = length
        self.filename = filename

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
      0 : "LOGO_Creality",
      1 : "Print_0",
      2 : "Print_1",
      3 : "Prepare_0",
      4 : "Prepare_1",
      5 : "Control_0",
      6 : "Control_1",
      7 : "Leveling_0",
      8 : "Leveling_1",
      9 : "HotendTemp",
     10 : "BedTemp",
     11 : "Speed",
     12 : "Zoffset",
     13 : "Back",
     14 : "File",
     15 : "PrintTime",
     16 : "RemainTime",
     17 : "Setup_0",
     18 : "Setup_1",
     19 : "Pause_0",
     20 : "Pause_1",
     21 : "Continue_0",
     22 : "Continue_1",
     23 : "Stop_0",
     24 : "Stop_1",
     25 : "Bar",
     26 : "More",
     27 : "Axis",
     28 : "CloseMotor",
     29 : "Homing",
     30 : "SetHome",
     31 : "PLAPreheat",
     32 : "ABSPreheat",
     33 : "Cool",
     34 : "Language",
     35 : "MoveX",
     36 : "MoveY",
     37 : "MoveZ",
     38 : "Extruder",
     # Skip 39
     40 : "Temperature",
     41 : "Motion",
     42 : "WriteEEPROM",
     43 : "ReadEEPROM",
     44 : "ResetEEPROM",
     45 : "Info",
     46 : "SetEndTemp",
     47 : "SetBedTemp",
     48 : "FanSpeed",
     49 : "SetPLAPreheat",
     50 : "SetABSPreheat",
     51 : "MaxSpeed",
     52 : "MaxAccelerated",
     53 : "MaxJerk",
     54 : "Step",
     55 : "PrintSize",
     56 : "Version",
     57 : "Contact",
     58 : "StockConfiguraton",
     59 : "MaxSpeedX",
     60 : "MaxSpeedY",
     61 : "MaxSpeedZ",
     62 : "MaxSpeedE",
     63 : "MaxAccX",
     64 : "MaxAccY",
     65 : "MaxAccZ",
     66 : "MaxAccE",
     67 : "MaxSpeedJerkX",
     68 : "MaxSpeedJerkY",
     69 : "MaxSpeedJerkZ",
     70 : "MaxSpeedJerkE",
     71 : "StepX",
     72 : "StepY",
     73 : "StepZ",
     74 : "StepE",
     75 : "Setspeed",
     76 : "SetZOffset",
     77 : "Rectangle",
     78 : "BLTouch",
     79 : "TempTooLow",
     80 : "AutoLeveling",
     81 : "TempTooHigh",
     82 : "NoTips_C",
     83 : "NoTips_E",
     84 : "Continue_C",
     85 : "Continue_E",
     86 : "Cancel_C",
     87 : "Cancel_E",
     88 : "Confirm_C",
     89 : "Confirm_E",
     90 : "Info_0",
     91 : "Info_1",
     92 : "DegreesC",
     93 : "Printer_0",
    200 : "Checkbox_F",
    201 : "Checkbox_T",
    202 : "Fade",
    203 : "Mesh",
    204 : "Tilt",
    205 : "Brightness",
    206 : "Probe",
    249 : "AxisD",
    250 : "AxisBR",
    251 : "AxisTR",
    252 : "AxisBL",
    253 : "AxisTL",
    254 : "AxisC"
}

# Names for MarlinUI's boot icons (3.ICO) and status icons (4.ICO), from marlinui/dwin_lcd.h
_iconNames3 = {
      0 : "MarlinBoot",
      1 : "OpenSource",
      2 : "GitHubURL",
      3 : "MarlinURL",
      4 : "Copyright"
}

_iconNames4 = {
      0 : "LOGO_Marlin",
      1 : "HotendOff",
      2 : "HotendOn",
      3 : "BedOff",
      4 : "BedOn",
      5 : "Fan0",
      6 : "Fan1",
      7 : "Fan2",
      8 : "Fan3",
      9 : "Halted",
     10 : "Question",
     11 : "Alert",
     12 : "RotateCW",
     13 : "RotateCCW",
     14 : "UpArrow",
     15 : "DownArrow",
     16 : "Bedline",
     17 : "BedLeveledOff",
     18 : "BedLeveledOn"
}

def iconNamesFor(filename):
    """Return the icon name map for an ICO file, by its leading set number.
    Unknown sets get the 7.ICO (dwin_set.h) names.
    """
    m = re.match(r'^(\d+)', os.path.basename(filename))
    num = int(m.group(1)) if m else 7
    return { 3: _iconNames3, 4: _iconNames4 }.get(num, _iconNames)

