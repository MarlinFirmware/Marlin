#!/usr/bin/python3

# This file is for preprocessing gcode and the new G29 Autobedleveling from Marlin
# It will analyse the first 2 Layer and return the maximum size for this part
# After this it will replace with g29_keyword = ';MarlinG29Script' with the new G29 LRFB
# the new file will be created in the same folder.

# your gcode-file/folder
folder = './'
my_file = 'test.gcode'

# this is the minimum of G1 instructions which should be between 2 different heights
min_g1 = 3

# maximum number of lines to parse, I don't want to parse the complete file
# only the first plane is we are interested in
max_g1 = 100000000

# g29 keyword
g29_keyword = 'g29'
g29_keyword = g29_keyword.upper()

# output filename
output_file = folder + 'g29_' + my_file
# input filename
input_file = folder + my_file

# minimum scan size
min_size = 40
probing_points = 3  # points x points

# other stuff
min_x = 500
min_y = min_x
max_x = -500
max_y = max_x
last_z = 0.001

layer = 0
lines_of_g1 = 0

gcode = []


# return only g1-lines
def has_g1(line):
    return line[:2].upper() == "G1"


# find position in g1 (x,y,z)
def find_axis(line, axis):
    found = False
    number = ""
    for char in line:
        if found:
            if char == ".":
                number += char
            elif char == "-":
                number += char
            else:
                try:
                    int(char)
                    number += char
                except ValueError:
                    break
        else:
            found = char.upper() == axis.upper()
    try:
        return float(number)
    except ValueError:
        return None


# save the min or max-values for each axis
def set_mima(line):
    global min_x, max_x, min_y, max_y, last_z

    current_x = find_axis(line, 'x')
    current_y = find_axis(line, 'y')

    if current_x is not None:
        min_x = min(current_x, min_x)
        max_x = max(current_x, max_x)
    if current_y is not None:
        min_y = min(current_y, min_y)
        max_y = max(current_y, max_y)

    return min_x, max_x, min_y, max_y


# find z in the code and return it
def find_z(gcode, start_at_line=0):
    for i in range(start_at_line, len(gcode)):
        my_z = find_axis(gcode[i], 'Z')
        if my_z is not None:
            return my_z, i


def z_parse(gcode, start_at_line=0, end_at_line=0):
    i = start_at_line
    all_z = []
    line_between_z = []
    z_at_line = []
    # last_z = 0
    last_i = -1

    while len(gcode) > i:
        try:
            z, i = find_z(gcode, i + 1)
        except TypeError:
            break

        all_z.append(z)
        z_at_line.append(i)
        temp_line = i - last_i -1
        line_between_z.append(i - last_i - 1)
        # last_z = z
        last_i = i
        if 0 < end_at_line <= i or temp_line >= min_g1:
            # print('break at line {} at heigth {}'.format(i, z))
            break

    line_between_z = line_between_z[1:]
    return all_z, line_between_z, z_at_line


# get the lines which should be the first layer
def get_lines(gcode, minimum):
    i = 0
    all_z, line_between_z, z_at_line = z_parse(gcode, end_at_line=max_g1)
    for count in line_between_z:
        i += 1
        if count > minimum:
            # print('layer: {}:{}'.format(z_at_line[i-1], z_at_line[i]))
            return z_at_line[i - 1], z_at_line[i]


with open(input_file, 'r') as file:
    lines = 0
    for line in file:
        lines += 1
        if lines > 1000:
            break
        if has_g1(line):
            gcode.append(line)
file.close()

start, end = get_lines(gcode, min_g1)
for i in range(start, end):
    set_mima(gcode[i])

print('x_min:{} x_max:{}\ny_min:{} y_max:{}'.format(min_x, max_x, min_y, max_y))

# resize min/max - values for minimum scan
if max_x - min_x < min_size:
    offset_x = int((min_size - (max_x - min_x)) / 2 + 0.5)  # int round up
    # print('min_x! with {}'.format(int(max_x - min_x)))
    min_x = int(min_x) - offset_x
    max_x = int(max_x) + offset_x
if max_y - min_y < min_size:
    offset_y = int((min_size - (max_y - min_y)) / 2 + 0.5)  # int round up
    # print('min_y! with {}'.format(int(max_y - min_y)))
    min_y = int(min_y) - offset_y
    max_y = int(max_y) + offset_y


new_command = 'G29 L{0} R{1} F{2} B{3} P{4}\n'.format(min_x,
                                                      max_x,
                                                      min_y,
                                                      max_y,
                                                      probing_points)

out_file = open(output_file, 'w')
in_file = open(input_file, 'r')

for line in in_file:
    if line[:len(g29_keyword)].upper() == g29_keyword:
        out_file.write(new_command)
        print('write G29')
    else:
        out_file.write(line)

file.close()
out_file.close()

print('auto G29 finished')
