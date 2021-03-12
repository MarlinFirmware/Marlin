import json
import sys


if len(sys.argv) == 2 and sys.argv[1] == '--no-dry-run':
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
			print('Output configuration written to: ' + 'Marlin/' + key + output_suffix)
except:
	print('No marlin_config.json found.')

