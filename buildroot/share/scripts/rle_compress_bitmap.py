#!/usr/bin/env python3
#
# Bitwise RLE compress a Marlin mono DOGM bitmap.
# Input: An existing Marlin Marlin mono DOGM bitmap .cpp or .h file.
# Output: A new file with the original and compressed data.
#
# Usage: rle_compress_bitmap.py INPUT_FILE OUTPUT_FILE
#
import sys,struct
import re

def addCompressedData(input_file, output_file):
    ofile = open(output_file, 'wt')

    datatype = "uint8_t"
    bytewidth = 16
    raw_data = []
    arrname = ''

    c_data_section = False ; c_skip_data = False ; c_footer = False
    while True:
        line = input_file.readline()
        if not line: break

        if not c_footer:
            if not c_skip_data: ofile.write(line)

        mat = re.match(r'.+CUSTOM_BOOTSCREEN_BMPWIDTH\s+(\d+)', line)
        if mat: bytewidth = (int(mat[1]) + 7) // 8

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

    input_file.close()

    #print("\nRaw Bitmap Data", raw_data)

    #
    # Bitwise RLE (run length) encoding
    # Convert data from raw mono bitmap to a bitwise run-length-encoded format.
    # - The first nybble is the starting bit state. Changing this nybble inverts the bitmap.
    # - The following bytes provide the runs for alternating on/off bits.
    #   - A value of 0-14 encodes a run of 1-15.
    #   - A value of 16 indicates a run of 16-270 calculated using the next two bytes.
    #
    def bitwise_rle_encode(data):
        warn = "This may take a while" if len(data) > 300000 else ""
        print("Compressing image data...", warn)

        def get_bit(data, n): return 1 if (data[n // 8] & (0x80 >> (n & 7))) else 0

        bitslen = len(data) * 8
        bitstate = get_bit(data, 0)
        rledata = [ bitstate ]

        i = 0
        runlen = -1
        while i <= bitslen:
            if i < bitslen: b = get_bit(data, i)
            runlen += 1
            if bitstate != b or i == bitslen:
                if i > 11 * 56 * 8: print(f'Bit change at index {i} with runlen={runlen}')
                if runlen >= 16:
                    rledata += [ 15, runlen // 16 - 1, runlen % 16 ]
                    if i > 11 * 56 * 8: print(f'Storing {[ 15, runlen // 16 - 1, runlen % 16 ]}')
                else:
                    rledata += [ runlen - 1 ]
                    if i > 11 * 56 * 8: print(f'Storing {[ runlen ]}')
                bitstate ^= 1
                runlen = 0
            i += 1

        print("\nrledata", rledata)

        encoded = []
        ri = 0
        rlen = len(rledata)
        while ri < rlen:
            v = rledata[ri] << 4
            if (ri < rlen - 1): v |= rledata[ri + 1]
            encoded += [ v ]
            ri += 2

        print("\nencoded", encoded)
        return encoded

    def bitwise_rle_decode(rledata, invert=0):
        expanded = []
        for n in rledata: expanded += [ n >> 4, n & 0xF ]

        decoded = []
        bitstate = 0 ; workbyte = 0 ; outindex = 0
        i = 0
        while i < len(expanded):
            c = expanded[i]
            i += 1

            if i == 1: bitstate = c ; continue

            if c == 15:
                c = 16 * expanded[i] + expanded[i + 1] + 15
                i += 2

            for _ in range(c, -1, -1):
                bitval = 0x80 >> (outindex & 7)
                if bitstate: workbyte |= bitval
                if bitval == 1:
                    decoded += [ workbyte ]
                    workbyte = 0
                outindex += 1

            bitstate ^= 1

        print("\nDecoded RLE data:")
        pretty = [ '{0:08b}'.format(v) for v in decoded ]
        rows = [pretty[i:i+bytewidth] for i in range(0, len(pretty), bytewidth)]
        for row in rows: print(f"{''.join(row)}")

        return decoded

    def rle_emit(ofile, arrname, rledata, rawsize):

        outstr = ''
        rows = [ rledata[i:i+16] for i in range(0, len(rledata), 16) ]
        for i in range(0, len(rows)):
            rows[i] = [ '0x{0:02X}'.format(v) for v in rows[i] ]
            outstr += f"  {', '.join(rows[i])},\n"

        outstr = outstr[:-2]
        size = len(rledata)
        ofile.write("\n// Saves %i bytes\n%s %s_rle[%d] PROGMEM = {\n%s\n};\n" % (rawsize - size, datatype, arrname, size, outstr))

    # Encode the data, write it out, close the file
    rledata = bitwise_rle_encode(raw_data)
    rle_emit(ofile, arrname, rledata, len(raw_data))
    ofile.close()

    # Validate that code properly compressed (and decompressed) the data
    checkdata = bitwise_rle_decode(rledata)
    badindex = -1
    for i in range(0, len(checkdata)):
        if raw_data[i] != checkdata[i]:
            badindex = i
            break
    if badindex >= 0: print(f'Data mismatch at byte {badindex}')

if len(sys.argv) <= 2:
    print('Usage: rle_compress_bitmap.py INPUT_FILE OUTPUT_FILE')
    exit(1)

output_cpp = sys.argv[2]
inname = sys.argv[1].replace('//', '/')
try:
    input_cpp = open(inname)
    print("Processing", inname, "...")
    addCompressedData(input_cpp, output_cpp)
except OSError:
    print("Can't find input file", inname)
