#!/usr/bin/env python3
#
# RLE compress a Marlin mono DOGM bitmap.
# Input: An existing Marlin Marlin mono DOGM bitmap .cpp or .h file.
# Output: A new file with the original and compressed data.
#
# Usage: rle_compress_bitmap.py INPUT_FILE OUTPUT_FILE
#
import sys,struct
import re

def addCompressedData(input_file, output_file):
    ofile = open(output_file, 'wt')

    c_data_section = False
    c_skip_data = False
    c_footer = False
    datatype = "uint8_t"
    bytewidth = 16
    raw_data = []
    rle_value = []
    rle_count = []
    arrname = ''

    line = input_file.readline()
    while line:
        if not c_footer:
            if not c_skip_data: ofile.write(line)

        mat = re.match(r'CUSTOM_BOOTSCREEN_BMPWIDTH\s+(\d+)', line)
        if mat: bytewidth = int(mat[1]) / 16

        if "};" in line:
            c_skip_data = False
            c_data_section = False
            c_footer = True

        if c_data_section:
            cleaned = re.sub(r"\s|,|\n", "", line)
            mat = re.match(r'(0b|B)[01]{8}', cleaned)
            if mat:
                as_list = cleaned.split(mat[1])
                as_list.pop(0)
                raw_data += [int(x, 2) for x in as_list]
            else:
                as_list = cleaned.split("0x")
                as_list.pop(0)
                raw_data += [int(x, 16) for x in as_list]

        mat = re.match(r'const (uint\d+_t|unsigned char)', line)
        if mat:
            # e.g.: const unsigned char custom_start_bmp[] PROGMEM = {
            datatype = mat[0]
            if "_rle" in line:
                c_skip_data = True
            else:
                c_data_section = True
                arrname = line.split('[')[0].split(' ')[-1]
                print("Found data array", arrname)

        line = input_file.readline()

    input_file.close()

    #
    # RLE (run length) encoding
    # Convert data from raw mono bitmap to a simple run-length-encoded format.
    # - Each sequence begins with a count byte N.
    #   - If the high bit is set in N the run contains N & 0x7F + 1 unique bytes.
    #   - Otherwise it repeats the following byte N + 1 times.
    #
    def rle_encode(data):
        warn = "This may take a while" if len(data) > 300000 else ""
        print("Compressing image data...", warn)
        rledata = []
        distinct = []
        i = 0
        while i < len(data):
            v = data[i]
            i += 1
            rsize = 1
            for j in range(i, len(data)):
                if v != data[j]: break
                i += 1
                rsize += 1
                if rsize >= 128: break

            # If the run is one, add to the distinct values
            if rsize == 1: distinct.append(v)

            # If distinct length >= 127, or the repeat run is 2 or more,
            # store the distinct run.
            nr = len(distinct)
            if nr and (nr >= 128 or rsize > 1 or i >= len(data)):
                rledata += [(nr - 1) | 0x80] + distinct
                distinct = []

            # If the repeat run is 2 or more, store the repeat run.
            if rsize > 1: rledata += [rsize - 1, v]

        return rledata

    def append_byte(data, byte, cols=bytewidth):
        if data == '': data = '  '
        data += ('0x{0:02X}, '.format(byte)) # 6 characters
        if len(data) % (cols * 6 + 2) == 0: data = data.rstrip() + "\n  "
        return data

    def rle_emit(ofile, arrname, rledata, rawsize):
        i = 0
        outstr = ''
        size = 0
        while i < len(rledata):
            rval = rledata[i]
            i += 1
            if rval & 0x80:
                count = (rval & 0x7F) + 1
                outstr = append_byte(outstr, rval)
                size += 1
                for j in range(count):
                    outstr = append_byte(outstr, rledata[i + j])
                    size += 1
                i += count
            else:
                outstr = append_byte(outstr, rval)
                outstr = append_byte(outstr, rledata[i])
                i += 1
                size += 2

        outstr = outstr.rstrip()[:-1]
        ofile.write("\n// Saves %i bytes\n%s %s_rle[%d] PROGMEM = {\n%s\n};\n" % (rawsize - size, datatype, arrname, size, outstr))

    # Encode the data, write it out, close the file
    rledata = rle_encode(raw_data)
    rle_emit(ofile, arrname, rledata, len(raw_data))
    ofile.close()

if len(sys.argv) <= 2:
    print('Usage: rle_compress_bitmap.py INPUT_FILE OUTPUT_FILE')
    exit(1)

output_cpp = sys.argv[2]
inname = sys.argv[1].replace('//', '/')
input_cpp = open(inname)
print("Processing", inname, "...")
addCompressedData(input_cpp, output_cpp)
