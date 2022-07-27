#
# signature.py
#
import os,subprocess,re,json,hashlib

#
# Return all macro names in a header as an array, so we can take
# the intersection with the preprocessor output, giving a decent
# reflection of all enabled options that (probably) came from the
# configuration files. We end up with the actual configured state,
# better than what the config files say. You can then use the
# resulting config.ini to produce more exact configuration files.
#
def extract_defines(filepath):
	f = open(filepath, encoding="utf8").read().split("\n")
	a = []
	for line in f:
		sline = line.strip()
		if sline[:7] == "#define":
			# Extract the key here (we don't care about the value)
			kv = sline[8:].strip().split()
			a.append(kv[0])
	return a

# Compute the SHA256 hash of a file
def get_file_sha256sum(filepath):
	sha256_hash = hashlib.sha256()
	with open(filepath,"rb") as f:
		# Read and update hash string value in blocks of 4K
		for byte_block in iter(lambda: f.read(4096),b""):
			sha256_hash.update(byte_block)
	return sha256_hash.hexdigest()

#
# Compress a JSON file into a zip file
#
import zipfile
def compress_file(filepath, outputbase):
	with zipfile.ZipFile(outputbase + '.zip', 'w', compression=zipfile.ZIP_BZIP2, compresslevel=9) as zipf:
		zipf.write(filepath, compress_type=zipfile.ZIP_BZIP2, compresslevel=9)

#
# Compute the build signature. The idea is to extract all defines in the configuration headers
# to build a unique reversible signature from this build so it can be included in the binary
# We can reverse the signature to get a 1:1 equivalent configuration file
#
def compute_build_signature(env):
	if 'BUILD_SIGNATURE' in env:
		return

	# Definitions from these files will be kept
	files_to_keep = [ 'Marlin/Configuration.h', 'Marlin/Configuration_adv.h' ]

	build_dir = os.path.join(env['PROJECT_BUILD_DIR'], env['PIOENV'])

	# Check if we can skip processing
	hashes = ''
	for header in files_to_keep:
		hashes += get_file_sha256sum(header)[0:10]

	marlin_json = os.path.join(build_dir, 'marlin_config.json')
	marlin_zip = os.path.join(build_dir, 'mc')

	# Read existing config file
	try:
		with open(marlin_json, 'r') as infile:
			conf = json.load(infile)
			if conf['__INITIAL_HASH'] == hashes:
				# Same configuration, skip recomputing the building signature
				compress_file(marlin_json, marlin_zip)
				return
	except:
		pass

	# Get enabled config options based on preprocessor
	from preprocessor import run_preprocessor
	complete_cfg = run_preprocessor(env)

	# Dumb #define extraction from the configuration files
	conf_defines = {}
	all_defines = []
	for header in files_to_keep:
		defines = extract_defines(header)
		# To filter only the define we want
		all_defines += defines
		# To remember from which file it cames from
		conf_defines[header.split('/')[-1]] = defines

	r = re.compile(r"\(+(\s*-*\s*_.*)\)+")

	# First step is to collect all valid macros
	defines = {}
	for line in complete_cfg:

		# Split the define from the value
		key_val = line[8:].strip().decode().split(' ')
		key, value = key_val[0], ' '.join(key_val[1:])

		# Ignore values starting with two underscore, since it's low level
		if len(key) > 2 and key[0:2] == "__" :
			continue
		# Ignore values containing a parenthesis (likely a function macro)
		if '(' in key and ')' in key:
			continue

		# Then filter dumb values
		if r.match(value):
			continue

		defines[key] = value if len(value) else ""

	if not 'CONFIGURATION_EMBEDDING' in defines:
		return

	# Second step is to filter useless macro
	resolved_defines = {}
	for key in defines:
		# Remove all boards now
		if key.startswith("BOARD_") and key != "BOARD_INFO_NAME":
			continue
		# Remove all keys ending by "_NAME" as it does not make a difference to the configuration
		if key.endswith("_NAME") and key != "CUSTOM_MACHINE_NAME":
			continue
		# Remove all keys ending by "_T_DECLARED" as it's a copy of extraneous system stuff
		if key.endswith("_T_DECLARED"):
			continue
		# Remove keys that are not in the #define list in the Configuration list
		if key not in all_defines + [ 'DETAILED_BUILD_VERSION', 'STRING_DISTRIBUTION_DATE' ]:
			continue

		# Don't be that smart guy here
		resolved_defines[key] = defines[key]

	# Generate a build signature now
	# We are making an object that's a bit more complex than a basic dictionary here
	data = {}
	data['__INITIAL_HASH'] = hashes
	# First create a key for each header here
	for header in conf_defines:
		data[header] = {}

	# Then populate the object where each key is going to (that's a O(N^2) algorithm here...)
	for key in resolved_defines:
		for header in conf_defines:
			if key in conf_defines[header]:
				data[header][key] = resolved_defines[key]

	# Append the source code version and date
	data['VERSION'] = {}
	data['VERSION']['DETAILED_BUILD_VERSION'] = resolved_defines['DETAILED_BUILD_VERSION']
	data['VERSION']['STRING_DISTRIBUTION_DATE'] = resolved_defines['STRING_DISTRIBUTION_DATE']
	try:
		curver = subprocess.check_output(["git", "describe", "--match=NeVeRmAtCh", "--always"]).strip()
		data['VERSION']['GIT_REF'] = curver.decode()
	except:
		pass

	#
	# Produce a JSON file for CONFIGURATION_EMBEDDING or CONFIG_DUMP > 0
	#
	with open(marlin_json, 'w') as outfile:
		json.dump(data, outfile, separators=(',', ':'))

	# Compress the JSON file as much as we can
	compress_file(marlin_json, marlin_zip)

	# Generate a C source file for storing this array
	with open('Marlin/src/mczip.h','wb') as result_file:
		result_file.write(
			  b'#ifndef NO_CONFIGURATION_EMBEDDING_WARNING\n'
			+ b'  #warning "Generated file \'mc.zip\' is embedded (Define NO_CONFIGURATION_EMBEDDING_WARNING to suppress this warning.)"\n'
			+ b'#endif\n'
			+ b'const unsigned char mc_zip[] PROGMEM = {\n '
		)
		count = 0
		for b in open(os.path.join(build_dir, 'mc.zip'), 'rb').read():
			result_file.write(b' 0x%02X,' % b)
			count += 1
			if (count % 16 == 0):
			 	result_file.write(b'\n ')
		if (count % 16):
			result_file.write(b'\n')
		result_file.write(b'};\n')
