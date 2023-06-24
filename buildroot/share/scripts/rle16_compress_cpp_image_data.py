#!/usr/bin/env python3
#
# Utility to compress Marlin RGB565 TFT data to RLE16 format.
# Reads the existing Marlin RGB565 cpp file and generates a new file with the additional RLE16 data.
#
# Usage: rle16_compress_cpp_image_data.py INPUT_FILE.cpp OUTPUT_FILE.cpp
#
import sys,struct
import re

def addCompressedData(input_file, output_file):
    ofile = open(output_file, 'wt')

    line = input_file.readline()
    c_data_section = False
    c_footer = False
    raw_data = []
    rle_value = []
    rle_count = []
    footer = ''
    while line:
        if c_footer:
            footer += line
        else:
            ofile.write(line)
            currentline = re.sub(r"\s|,|\n", "", line)
        if "};" in currentline:
            c_data_section = False
            c_footer = True
        if c_data_section:
            as_list = currentline.split("0x")
            as_list.pop(0)
            raw_data += [int(x, 16) for x in as_list]

        if "extern" in currentline:
            # e.g.: extern const uint16_t marlin_logo_480x320x16[153600] = {
            c_data_section = True
            name = line.split(' ')[3].split('[')[0]

        line = input_file.readline()

    input_file.close()

    # RLE16 (run length 16) encoding

    # Convert data from from raw RGB565 to a simple run-length-encoded format for each word of data.
    #
    # Data is stored as 3 byte 'tuples'.
    # Byte 1 is the run count.
    #   If the run value is > 127 then the run contains unique words.
    #   If the run is < 128 then it repeats the following word.
    # Byte 2 is the high byte of the uint16_t RGB565 value
    # Byte 3 is the low byte of the uint16_t RGB565 value

    def rle_encode(mydata):
        rledata = []
        distinct = []
        i = 0
        while i < len(mydata):
            v = mydata[i]
            i += 1
            rsize = 1
            for j in range(i, len(mydata)):
                if v != mydata[j]: break;
                i += 1
                rsize += 1
                if rsize >= 128: break;

            # If the run is one, add to the distinct values
            if rsize == 1: distinct.append(v)

            # If distinct length >= 127, or the repeat run is 2 or more,
            # store the distinct run.
            if len(distinct) and (len(distinct) >= 128 or rsize > 1 or i >= len(mydata)):
                # Store the distinct run
                rledata += [(len(distinct) - 1) | 0x80] + distinct
                distinct = []

            # If the repeat run is 2 or more, store the repeat run.
            if rsize > 1: rledata += [rsize - 1, v]

        return rledata

    def extend_data(data, byte, cols=240):
        if data == '': data = '  '
        data += ('0x{0:02X}, '.format(byte)) # 6 characters
        if len(data) % (cols * 6 + 2) == 0: data = data.rstrip() + "\n  "
        return data

    def rle_emit(ofile, name, rledata):
        col = 0
        i = 0
        outstr = ''
        bcount = 0
        while i < len(rledata):
            count = rledata[i]
            i += 1
            if count & 0x80:
                count &= 0x7F
                outstr = extend_data(outstr, count)
                count += 1
                bcount += 1
                for j in range(count):
                    outstr = extend_data(outstr, rledata[i + j] >> 8)
                    outstr = extend_data(outstr, rledata[i + j] & 0xFF)
                    bcount += 2
                i += count
            else:
                outstr = extend_data(outstr, count)
                outstr = extend_data(outstr, rledata[i] >> 8)
                outstr = extend_data(outstr, rledata[i] & 0xFF)
                i += 1
                bcount += 3

        outstr = outstr.rstrip()[:-1]
        ofile.write("\nextern const uint8_t %s_rle16[%d] = {\n%s\n};\n" % (name, bcount, outstr))

    # Encode the data, write it out, close the file
    rledata = rle_encode(raw_data)
    rle_emit(ofile, name, rledata)
    ofile.write(footer)
    ofile.close()

if len(sys.argv) <= 2:
    print("Utility to compress Marlin RGB565 TFT data to RLE16 format.")
    print("Reads the existing Marlin RGB565 cpp file and generates a new file with the additional RLE16 data.")
    print("Usage: rle16_compress_cpp_image_data.py INPUT_FILE.cpp OUTPUT_FILE.cpp")
    exit(1)

output_cpp = sys.argv[2]
input_cpp = open(sys.argv[1])
addCompressedData(input_cpp, output_cpp)
