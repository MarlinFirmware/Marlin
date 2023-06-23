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
    f = open(output_file, 'wt')

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
            f.write(line)
            currentline = re.sub(r"\s|,|\n", "", line)
        if "};" in currentline:
            c_data_section = False
            c_footer = True
        if c_data_section:
            as_list = currentline.split("0x")
            as_list.pop(0)
            raw_data += as_list

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
    # Byte 1 is the run count. Limit is 255, if > 255 create multiple tuples of the same RGB565 value.
    # Byte 2 is the high byte of the uint16_t RGB565 value
    # Byte 3 is the low byte of the uint16_t RGB565 value

    i = 0
    while (i < len(raw_data)):
        count = 1
        word = raw_data[i]
        j = i
        while (j < len(raw_data) - 1):
            if (raw_data[j] != raw_data[j + 1]): break
            count += 1
            j += 1
            if count == 255: break  # <= 255 of the same word in a single run
        rle_count.append('0x{0:02X}, '.format(count))
        rle_value.append(word)

        i = j + 1

    x_size = int(480 / 2) # used for line formatting

    rows = int(len(rle_count) / x_size)
    remainder = int(len(rle_count) % x_size)

    f.write("extern const uint8_t %s_rle16[%d] = {\n" % (name, len(rle_count * 3)))

    for i in range(rows):
        line = '  '
        for j in range(x_size):
            count = rle_count[j + (i * x_size)]
            value = rle_value[j + (i * x_size)]
            line += count + '0x' + value[:2] + ', 0x' + value[2:] + ', '
        line = line.rstrip() + "\n"
        f.write(line)

    i += 1
    line = '  '
    for j in range(remainder):
        count = rle_count[j + (i * x_size)]
        value = rle_value[j + (i * x_size)]
        line += count + '0x' + value[:2] + ', 0x' + value[2:] + ', '
    line = line.rstrip(', ') + "\n"
    f.write(line + "};\n" + footer)
    f.close()

if len(sys.argv) <= 2:
    print("Utility to compress Marlin RGB565 TFT data to RLE16 format.")
    print("Reads the existing Marlin RGB565 cpp file and generates a new file with the additional RLE16 data.")
    print("Usage: rle16_compress_cpp_image_data.py INPUT_FILE.cpp OUTPUT_FILE.cpp")
    exit(1)

output_cpp = sys.argv[2]
input_cpp = open(sys.argv[1])
addCompressedData(input_cpp, output_cpp)
