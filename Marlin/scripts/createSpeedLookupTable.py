#!/usr/bin/env python

""" Generate the stepper delay lookup table for Marlin firmware. """

import argparse

__author__ = "Ben Gamari <bgamari@gmail.com>"
__copyright__ = "Copyright 2012, Ben Gamari"
__license__ = "GPL"

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('-f', '--cpu-freq', type=int, default=16, help='CPU clockrate in MHz (default=16)')
parser.add_argument('-d', '--divider', type=int, default=8, help='Timer/counter pre-scale divider (default=8)')
args = parser.parse_args()

cpu_freq = args.cpu_freq * 1000000
timer_freq = cpu_freq / args.divider

print "#ifndef SPEED_LOOKUPTABLE_H"
print "#define SPEED_LOOKUPTABLE_H"
print
print '#include "Marlin.h"'
print

print "const uint16_t speed_lookuptable[1024] PROGMEM = {"
for step_rate in range(1024):
	if step_rate < 32:
		count = 65535
	else:
		count = (timer_freq / step_rate)
	print "\t%d," % count,

	if step_rate % 8 == 7:
		print
print "};"
print

print "#endif"

