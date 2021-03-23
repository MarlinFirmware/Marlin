import json
import sys
import shutil
import re


if '--bare-output' in sys.argv:
	output_suffix = ''
else:
	output_suffix = '.gen'

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
				define = '#define ' + k + ' ' + v + '\n'
				outfile.write(define)
			outfile.close()

			# Try to apply the configuration to the actual configuration file (in order to keep useful comments)
			if output_suffix != '':
				# Move the existing configuration so it does not interfer
				shutil.move('Marlin/' + key, 'Marlin/' + key + '.origin')
				infile_lines = open('Marlin/' + key + '.origin', 'r').read().split('\n')
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

			
			# Parse the current configuration and th
			print('Output configuration written to: ' + 'Marlin/' + key + output_suffix)
except:
	print('No marlin_config.json found.')

