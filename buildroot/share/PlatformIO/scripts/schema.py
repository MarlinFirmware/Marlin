#!/usr/bin/env python3
#
# schema.py
#
# Used by signature.py via common-dependencies.py to generate a schema file during the PlatformIO build.
# This script can also be run standalone from within the Marlin repo to generate all schema files.
#
import re,json
from pathlib import Path

def extend_dict(d:dict, k:tuple):
	if len(k) >= 1 and k[0] not in d:
		d[k[0]] = {}
	if len(k) >= 2 and k[1] not in d[k[0]]:
		d[k[0]][k[1]] = {}
	if len(k) >= 3 and k[2] not in d[k[0]][k[1]]:
		d[k[0]][k[1]][k[2]] = {}

grouping_patterns = [
	re.compile(r'^([XYZIJKUVW]|[XYZ]2|Z[34]|E[0-7])$'),
	re.compile(r'^AXIS\d$'),
	re.compile(r'^(MIN|MAX)$'),
	re.compile(r'^[0-8]$'),
	re.compile(r'^HOTEND[0-7]$'),
	re.compile(r'^(HOTENDS|BED|PROBE|COOLER)$'),
	re.compile(r'^[XYZIJKUVW]M(IN|AX)$')
]
# If the indexed part of the option name matches a pattern
# then add it to the dictionary.
def find_grouping(gdict, filekey, sectkey, optkey, pindex):
	optparts = optkey.split('_')
	if 1 < len(optparts) > pindex:
		for patt in grouping_patterns:
			if patt.match(optparts[pindex]):
				subkey = optparts[pindex]
				modkey = '_'.join(optparts)
				optparts[pindex] = '*'
				wildkey = '_'.join(optparts)
				kkey = f'{filekey}|{sectkey}|{wildkey}'
				if kkey not in gdict: gdict[kkey] = []
				gdict[kkey].append((subkey, modkey))

# Build a list of potential groups. Only those with multiple items will be grouped.
def group_options(schema):
	for pindex in range(10, -1, -1):
		found_groups = {}
		for filekey, f in schema.items():
			for sectkey, s in f.items():
				for optkey in s:
					find_grouping(found_groups, filekey, sectkey, optkey, pindex)

		fkeys = [ k for k in found_groups.keys() ]
		for kkey in fkeys:
			items = found_groups[kkey]
			if len(items) > 1:
				f, s, w = kkey.split('|')
				extend_dict(schema, (f, s, w))						# Add wildcard group to schema
				for subkey, optkey in items:						# Add all items to wildcard group
					schema[f][s][w][subkey] = schema[f][s][optkey]	# Move non-wildcard item to wildcard group
					del schema[f][s][optkey]
			del found_groups[kkey]

# Extract all board names from boards.h
def load_boards():
	bpath = Path("Marlin/src/core/boards.h")
	if bpath.is_file():
		with bpath.open() as bfile:
			boards = []
			for line in bfile:
				if line.startswith("#define BOARD_"):
					bname = line.split()[1]
					if bname != "BOARD_UNKNOWN": boards.append(bname)
			return "['" + "','".join(boards) + "']"
	return ''

#
# Extract a schema from the current configuration files
#
def extract():
	# Load board names from boards.h
	boards = load_boards()

	# Parsing states
	class Parse:
		NORMAL			= 0 # No condition yet
		BLOCK_COMMENT	= 1 # Looking for the end of the block comment
		EOL_COMMENT		= 2 # EOL comment started, maybe add the next comment?
		GET_SENSORS		= 3 # Gathering temperature sensor options
		ERROR			= 9 # Syntax error

	# List of files to process, with shorthand
	filekey = { 'Configuration.h':'basic', 'Configuration_adv.h':'advanced' }
	# A JSON object to store the data
	sch_out = { 'basic':{}, 'advanced':{} }
	# Regex for #define NAME [VALUE] [COMMENT] with sanitized line
	defgrep = re.compile(r'^(//)?\s*(#define)\s+([A-Za-z0-9_]+)\s*(.*?)\s*(//.+)?$')
	# Defines to ignore
	ignore = ('CONFIGURATION_H_VERSION', 'CONFIGURATION_ADV_H_VERSION', 'CONFIG_EXPORT')
	# Start with unknown state
	state = Parse.NORMAL
	# Serial ID
	sid = 0
	# Loop through files and parse them line by line
	for fn, fk in filekey.items():
		with Path("Marlin", fn).open() as fileobj:
			section = 'none'		# Current Settings section
			line_number = 0			# Counter for the line number of the file
			conditions = []			# Create a condition stack for the current file
			comment_buff = []		# A temporary buffer for comments
			options_json = ''		# A buffer for the most recent options JSON found
			eol_options = False		# The options came from end of line, so only apply once
			join_line = False		# A flag that the line should be joined with the previous one
			line = ''				# A line buffer to handle \ continuation
			last_added_ref = None	# Reference to the last added item
			# Loop through the lines in the file
			for the_line in fileobj.readlines():
				line_number += 1

				# Clean the line for easier parsing
				the_line = the_line.strip()

				if join_line:	# A previous line is being made longer
					line += (' ' if line else '') + the_line
				else:			# Otherwise, start the line anew
					line, line_start = the_line, line_number

				# If the resulting line ends with a \, don't process now.
				# Strip the end off. The next line will be joined with it.
				join_line = line.endswith("\\")
				if join_line:
					line = line[:-1].strip()
					continue
				else:
					line_end = line_number

				defmatch = defgrep.match(line)

				# Special handling for EOL comments after a #define.
				# At this point the #define is already digested and inserted,
				# so we have to extend it
				if state == Parse.EOL_COMMENT:
					# If the line is not a comment, we're done with the EOL comment
					if not defmatch and the_line.startswith('//'):
						comment_buff.append(the_line[2:].strip())
					else:
						last_added_ref['comment'] = ' '.join(comment_buff)
						comment_buff = []
						state = Parse.NORMAL

				def use_comment(c, opt, sec, bufref):
					if c.startswith(':'):				# If the comment starts with : then it has magic JSON
						d = c[1:].strip()				# Strip the leading :
						cbr = c.rindex('}') if d.startswith('{') else c.rindex(']') if d.startswith('[') else 0
						if cbr:
							opt, cmt = c[1:cbr+1].strip(), c[cbr+1:].strip()
							if cmt != '': bufref.append(cmt)
						else:
							opt = c[1:].strip()
					elif c.startswith('@section'):		# Start a new section
						sec = c[8:].strip()
					elif not c.startswith('========'):
						bufref.append(c)
					return opt, sec

				# In a block comment, capture lines up to the end of the comment.
				# Assume nothing follows the comment closure.
				if state in (Parse.BLOCK_COMMENT, Parse.GET_SENSORS):
					endpos = line.find('*/')
					if endpos < 0:
						cline = line
					else:
						cline, line = line[:endpos].strip(), line[endpos+2:].strip()

						# Temperature sensors are done
						if state == Parse.GET_SENSORS:
							options_json = f'[ {options_json[:-2]} ]'

						state = Parse.NORMAL

					# Strip the leading '*' from block comments
					if cline.startswith('*'): cline = cline[1:].strip()

					# Collect temperature sensors
					if state == Parse.GET_SENSORS:
						sens = re.match(r'^(-?\d+)\s*:\s*(.+)$', cline)
						if sens:
							s2 = sens[2].replace("'","''")
							options_json += f"{sens[1]}:'{s2}', "

					elif state == Parse.BLOCK_COMMENT:

						# Look for temperature sensors
						if cline == "Temperature sensors available:":
							state, cline = Parse.GET_SENSORS, "Temperature Sensors"

						options_json, section = use_comment(cline, options_json, section, comment_buff)

				# For the normal state we're looking for any non-blank line
				elif state == Parse.NORMAL:
					# Skip a commented define when evaluating comment opening
					st = 2 if re.match(r'^//\s*#define', line) else 0
					cpos1 = line.find('/*')		# Start a block comment on the line?
					cpos2 = line.find('//', st)	# Start an end of line comment on the line?

					# Only the first comment starter gets evaluated
					cpos = -1
					if cpos1 != -1 and (cpos1 < cpos2 or cpos2 == -1):
						cpos = cpos1
						comment_buff = []
						state = Parse.BLOCK_COMMENT
						eol_options = False

					elif cpos2 != -1 and (cpos2 < cpos1 or cpos1 == -1):
						cpos = cpos2

						# Expire end-of-line options after first use
						if cline.startswith(':'): eol_options = True

						# Comment after a define may be continued on the following lines
						if state == Parse.NORMAL and defmatch != None and cpos > 10:
							state = Parse.EOL_COMMENT
							comment_buff = []

					# Process the start of a new comment
					if cpos != -1:
						cline, line = line[cpos+2:].strip(), line[:cpos].strip()

						# Strip leading '*' from block comments
						if state == Parse.BLOCK_COMMENT:
							if cline.startswith('*'): cline = cline[1:].strip()

						# Buffer a non-empty comment start
						if cline != '':
							options_json, section = use_comment(cline, options_json, section, comment_buff)

					# If the line has nothing before the comment, go to the next line
					if line == '':
						options_json = ''
						continue

					# Parenthesize the given expression if needed
					def atomize(s):
						if s == '' \
						or re.match(r'^[A-Za-z0-9_]*(\([^)]+\))?$', s) \
						or re.match(r'^[A-Za-z0-9_]+ == \d+?$', s):
							return s
						return f'({s})'

					#
					# The conditions stack is an array containing condition-arrays.
					# Each condition-array lists the conditions for the current block.
					# IF/N/DEF adds a new condition-array to the stack.
					# ELSE/ELIF/ENDIF pop the condition-array.
					# ELSE/ELIF negate the last item in the popped condition-array.
					# ELIF adds a new condition to the end of the array.
					# ELSE/ELIF re-push the condition-array.
					#
					cparts = line.split()
					iselif, iselse = cparts[0] == '#elif', cparts[0] == '#else'
					if iselif or iselse or cparts[0] == '#endif':
						if len(conditions) == 0:
							raise Exception(f'no #if block at line {line_number}')

						# Pop the last condition-array from the stack
						prev = conditions.pop()

						if iselif or iselse:
							prev[-1] = '!' + prev[-1] # Invert the last condition
							if iselif: prev.append(atomize(line[5:].strip()))
							conditions.append(prev)

					elif cparts[0] == '#if':
						conditions.append([ atomize(line[3:].strip()) ])
					elif cparts[0] == '#ifdef':
						conditions.append([ f'defined({line[6:].strip()})' ])
					elif cparts[0] == '#ifndef':
						conditions.append([ f'!defined({line[7:].strip()})' ])

					# Handle a complete #define line
					elif defmatch != None:

						# Get the match groups into vars
						enabled, define_name, val = defmatch[1] == None, defmatch[3], defmatch[4]

						# Increment the serial ID
						sid += 1

						# Create a new dictionary for the current #define
						define_info = {
							'section': section,
							'name': define_name,
							'enabled': enabled,
							'line': line_start,
							'sid': sid
						}

						# Type is based on the value
						if val == '':
							value_type = 'switch'
						elif re.match(r'^(true|false)$', val):
							value_type = 'bool'
							val = val == 'true'
						elif re.match(r'^[-+]?\s*\d+$', val):
							value_type = 'int'
							val = int(val)
						elif re.match(r'[-+]?\s*(\d+\.|\d*\.\d+)([eE][-+]?\d+)?[fF]?', val):
							value_type = 'float'
							val = float(val.replace('f',''))
						else:
							value_type = 'string'	if val[0] == '"' \
									else 'char'		if val[0] == "'" \
									else 'state'	if re.match(r'^(LOW|HIGH)$', val) \
									else 'enum'		if re.match(r'^[A-Za-z0-9_]{3,}$', val) \
									else 'int[]'	if re.match(r'^{(\s*[-+]?\s*\d+\s*(,\s*)?)+}$', val) \
									else 'float[]'	if re.match(r'^{(\s*[-+]?\s*(\d+\.|\d*\.\d+)([eE][-+]?\d+)?[fF]?\s*(,\s*)?)+}$', val) \
									else 'array'	if val[0] == '{' \
									else ''

						if val != '': define_info['value'] = val
						if value_type != '': define_info['type'] = value_type

						# Join up accumulated conditions with &&
						if conditions: define_info['requires'] = ' && '.join(sum(conditions, []))

						# If the comment_buff is not empty, add the comment to the info
						if comment_buff:
							full_comment = '\n'.join(comment_buff)

							# An EOL comment will be added later
							# The handling could go here instead of above
							if state == Parse.EOL_COMMENT:
								define_info['comment'] = ''
							else:
								define_info['comment'] = full_comment
								comment_buff = []

							# If the comment specifies units, add that to the info
							units = re.match(r'^\(([^)]+)\)', full_comment)
							if units:
								units = units[1]
								if units == 's' or units == 'sec': units = 'seconds'
								define_info['units'] = units

						# Set the options for the current #define
						if define_name == "MOTHERBOARD" and boards != '':
							define_info['options'] = boards
						elif options_json != '':
							define_info['options'] = options_json
							if eol_options: options_json = ''

						# Create section dict if it doesn't exist yet
						if section not in sch_out[fk]: sch_out[fk][section] = {}

						# If define has already been seen...
						if define_name in sch_out[fk][section]:
							info = sch_out[fk][section][define_name]
							if isinstance(info, dict): info = [ info ]	# Convert a single dict into a list
							info.append(define_info)					# Add to the list
						else:
							# Add the define dict with name as key
							sch_out[fk][section][define_name] = define_info

						if state == Parse.EOL_COMMENT:
							last_added_ref = define_info

	return sch_out

def dump_json(schema:dict, jpath:Path):
	with jpath.open('w') as jfile:
		json.dump(schema, jfile, ensure_ascii=False, indent=2)

def dump_yaml(schema:dict, ypath:Path):
	import yaml
	with ypath.open('w') as yfile:
		yaml.dump(schema, yfile, default_flow_style=False, width=120, indent=2)

def main():
	try:
		schema = extract()
	except Exception as exc:
		print("Error: " + str(exc))
		schema = None

	if schema:
		print("Generating JSON ...")
		dump_json(schema, Path('schema.json'))
		group_options(schema)
		dump_json(schema, Path('schema_grouped.json'))

		try:
			import yaml
		except ImportError:
			print("Installing YAML module ...")
			import subprocess
			try:
				subprocess.run(['python3', '-m', 'pip', 'install', 'pyyaml'])
				import yaml
			except:
				print("Failed to install YAML module")
				return

		print("Generating YML ...")
		dump_yaml(schema, Path('schema.yml'))

if __name__ == '__main__':
	main()
