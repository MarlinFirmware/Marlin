import json
import sys


if len(sys.argv) == 2:
	output_suffix = sys.argv[1]
else:
	output_suffix = '.gen'

try:
	with open('marlin_config.json', 'r') as infile:
		outfile = open('Marlin/Configuration' + output_suffix, 'w')
		conf = json.load(infile)
		for key in conf:
			v = conf[key]
			# Make define line now
			define = '#define ' + key + ' ' + v + '\n'
			outfile.write(define)
		outfile.close()
		print('Output configuration written to: ' + 'Marlin/Configuration' + output_suffix)
except:
	print('No marlin_config.json found.')

