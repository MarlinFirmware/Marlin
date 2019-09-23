#!/usr/bin/python

# Written By Marcio Teixeira 2018 - Aleph Objects, Inc.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# To view a copy of the GNU General Public License, go to the following
# location: <http://www.gnu.org/licenses/>.

from __future__ import print_function
from PIL import Image
import argparse
import textwrap

def pack_rle(data):
  """Use run-length encoding to pack the bytes"""
  rle = []
  value = data[0]
  count = 0
  for i in data:
    if i != value or count == 255:
      rle.append(count)
      rle.append(value)
      value = i
      count = 1
    else:
      count += 1
  rle.append(count)
  rle.append(value)
  return rle

class WriteSource:
  def __init__(self):
    self.values = []

  def add_pixel(self, value):
    self.values.append(value)

  def convert_to_4bpp(self, data):
    # Invert the image
    data = map(lambda i: 255 - i, data)
    # Quanitize 8-bit values into 4-bits
    data = map(lambda i: i >> 4, data)
    # Make sure there is an even number of elements
    if (len(data) & 1) == 1:
      result.append(0)
    # Combine each two adjacent values into one
    i = iter(data)
    data = map(lambda a, b: a << 4 | b, i ,i)
    # Pack the data
    data = pack_rle(data)
    # Convert values into hex strings
    return map(lambda a: "0x" + format(a, '02x'), data)

  def end_row(self):
    # Pad each row into even number of values
    if len(self.values) & 1:
      self.values.append(0)

  def write(self):
    data = self.convert_to_4bpp(self.values)
    data = ', '.join(data)
    data = textwrap.fill(data, 75, initial_indent = '  ', subsequent_indent = '  ')

    print("const unsigned char font[] PROGMEM = {")
    print(data);
    print("};")

if __name__ == "__main__":
  parser = argparse.ArgumentParser(description='Converts a grayscale bitmap into a 16-level RLE packed C array for use as font data')
  parser.add_argument("input")
  args = parser.parse_args()

  img = Image.open(args.input).convert('L')

  writer = WriteSource()
  for y in range(img.height):
    for x in range(img.width):
      writer.add_pixel(img.getpixel((x,y)))
    writer.end_row()
  writer.write()
