#!/usr/bin/env python
#
# Create a Configuration from marlin_config.json
#
import json
import sys
import shutil
import re

opt_output = '--opt' in sys.argv
output_suffix = '.sh' if opt_output else '' if '--bare-output' in sys.argv else '.gen'

try:
	with open('marlin_config.json', 'r') as infile:
		conf = json.load(infile)
		for key in conf:
			# We don't care about the hash when restoring here
			if key == '__INITIAL_HASH':
				continue
			if key == 'VERSION':
				for k, v in sorted(conf[key].items()):
					print(k + ': ' + v)
				continue
			# The key is the file name, so let's build it now
			outfile = open('Marlin/' + key + output_suffix, 'w')
			for k, v in sorted(conf[key].items()):
				# Make define line now
				if opt_output:
					if v != '':
						if '"' in v:
							v = "'%s'" % v
						elif ' ' in v:
							v = '"%s"' % v
						define = 'opt_set ' + k + ' ' + v + '\n'
					else:
						define = 'opt_enable ' + k + '\n'
				else:
					define = '#define ' + k + ' ' + v + '\n'
				outfile.write(define)
			outfile.close()

			# Try to apply changes to the actual configuration file (in order to keep useful comments)
			if output_suffix != '':
				# Move the existing configuration so it doesn't interfere
				shutil.move('Marlin/' + key, 'Marlin/' + key + '.orig')
				infile_lines = open('Marlin/' + key + '.orig', 'r').read().split('\n')
				outfile = open('Marlin/' + key, 'w')
				for line in infile_lines:
					sline = line.strip(" \t\n\r")
					if sline[:7] == "#define":
						# Extract the key here (we don't care about the value)
						kv = sline[8:].strip().split(' ')
						if kv[0] in conf[key]:
							outfile.write('#define ' + kv[0] + ' ' + conf[key][kv[0]] + '\n')
							# Remove the key from the dict, so we can still write all missing keys at the end of the file
							del conf[key][kv[0]]
						else:
							outfile.write(line + '\n')
					else:
						outfile.write(line + '\n')
				# Process any remaining defines here
				for k, v in sorted(conf[key].items()):
					define = '#define ' + k + ' ' + v + '\n'
					outfile.write(define)
				outfile.close()

			print('Output configuration written to: ' + 'Marlin/' + key + output_suffix)
except:
	print('No marlin_config.json found.')
