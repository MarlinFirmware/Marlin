#
# common-dependencies.py
# Convenience script to check dependencies and add libs and sources for Marlin Enabled Features
#
import subprocess,os,re
# For building the signature and caching the result
import json,hashlib
try:
	import configparser
except ImportError:
	import ConfigParser as configparser
try:
	# PIO < 4.4
	from platformio.managers.package import PackageManager
except ImportError:
	# PIO >= 4.4
	from platformio.package.meta import PackageSpec as PackageManager

PIO_VERSION_MIN = (5, 0, 3)
try:
	from platformio import VERSION as PIO_VERSION
	weights = (1000, 100, 1)
	version_min = sum([x[0] * float(re.sub(r'[^0-9]', '.', str(x[1]))) for x in zip(weights, PIO_VERSION_MIN)])
	version_cur = sum([x[0] * float(re.sub(r'[^0-9]', '.', str(x[1]))) for x in zip(weights, PIO_VERSION)])
	if version_cur < version_min:
		print()
		print("**************************************************")
		print("******      An update to PlatformIO is      ******")
		print("******  required to build Marlin Firmware.  ******")
		print("******                                      ******")
		print("******      Minimum version: ", PIO_VERSION_MIN, "    ******")
		print("******      Current Version: ", PIO_VERSION, "    ******")
		print("******                                      ******")
		print("******   Update PlatformIO and try again.   ******")
		print("**************************************************")
		print()
		exit(1)
except SystemExit:
	exit(1)
except:
	print("Can't detect PlatformIO Version")

Import("env")

#print(env.Dump())

try:
	verbose = int(env.GetProjectOption('custom_verbose'))
except:
	verbose = 0


def blab(str):
	if verbose:
		print(str)

def parse_pkg_uri(spec):
	if PackageManager.__name__ == 'PackageSpec':
		return PackageManager(spec).name
	else:
		name, _, _ = PackageManager.parse_pkg_uri(spec)
		return name

FEATURE_CONFIG = {}

def add_to_feat_cnf(feature, flines):

	try:
		feat = FEATURE_CONFIG[feature]
	except:
		FEATURE_CONFIG[feature] = {}

	# Get a reference to the FEATURE_CONFIG under construction
	feat = FEATURE_CONFIG[feature]

	# Split up passed lines on commas or newlines and iterate
	# Add common options to the features config under construction
	# For lib_deps replace a previous instance of the same library
	atoms = re.sub(r',\\s*', '\n', flines).strip().split('\n')
	for line in atoms:
		parts = line.split('=')
		name = parts.pop(0)
		if name in ['build_flags', 'extra_scripts', 'src_filter', 'lib_ignore']:
			feat[name] = '='.join(parts)
		else:
			for dep in line.split(','):
				lib_name = re.sub(r'@([~^]|[<>]=?)?[\d.]+', '', dep.strip()).split('=').pop(0)
				lib_re = re.compile('(?!^' + lib_name + '\\b)')
				feat['lib_deps'] = list(filter(lib_re.match, feat['lib_deps'])) + [dep]

def load_config():
	config = configparser.ConfigParser()
	config.read("platformio.ini")
	items = config.items('features')
	for key in items:
		feature = key[0].upper()
		if not feature in FEATURE_CONFIG:
			FEATURE_CONFIG[feature] = { 'lib_deps': [] }
		add_to_feat_cnf(feature, key[1])

	# Add options matching custom_marlin.MY_OPTION to the pile
	all_opts = env.GetProjectOptions()
	for n in all_opts:
		mat = re.match(r'custom_marlin\.(.+)', n[0])
		if mat:
			try:
				val = env.GetProjectOption(n[0])
			except:
				val = None
			if val:
				add_to_feat_cnf(mat.group(1).upper(), val)

def get_all_known_libs():
	known_libs = []
	for feature in FEATURE_CONFIG:
		feat = FEATURE_CONFIG[feature]
		if not 'lib_deps' in feat:
			continue
		for dep in feat['lib_deps']:
			name = parse_pkg_uri(dep)
			known_libs.append(name)
	return known_libs

def get_all_env_libs():
	env_libs = []
	lib_deps = env.GetProjectOption('lib_deps')
	for dep in lib_deps:
		name = parse_pkg_uri(dep)
		env_libs.append(name)
	return env_libs

def set_env_field(field, value):
	proj = env.GetProjectConfig()
	proj.set("env:" + env['PIOENV'], field, value)

# All unused libs should be ignored so that if a library
# exists in .pio/lib_deps it will not break compilation.
def force_ignore_unused_libs():
	env_libs = get_all_env_libs()
	known_libs = get_all_known_libs()
	diff = (list(set(known_libs) - set(env_libs)))
	lib_ignore = env.GetProjectOption('lib_ignore') + diff
	blab("Ignore libraries: %s" % lib_ignore)
	set_env_field('lib_ignore', lib_ignore)

def apply_features_config():
	load_config()
	for feature in FEATURE_CONFIG:
		if not env.MarlinFeatureIsEnabled(feature):
			continue

		feat = FEATURE_CONFIG[feature]

		if 'lib_deps' in feat and len(feat['lib_deps']):
			blab("Adding lib_deps for %s... " % feature)

			# feat to add
			deps_to_add = {}
			for dep in feat['lib_deps']:
				name = parse_pkg_uri(dep)
				deps_to_add[name] = dep

			# Does the env already have the dependency?
			deps = env.GetProjectOption('lib_deps')
			for dep in deps:
				name = parse_pkg_uri(dep)
				if name in deps_to_add:
					del deps_to_add[name]

			# Are there any libraries that should be ignored?
			lib_ignore = env.GetProjectOption('lib_ignore')
			for dep in deps:
				name = parse_pkg_uri(dep)
				if name in deps_to_add:
					del deps_to_add[name]

			# Is there anything left?
			if len(deps_to_add) > 0:
				# Only add the missing dependencies
				set_env_field('lib_deps', deps + list(deps_to_add.values()))

		if 'build_flags' in feat:
			f = feat['build_flags']
			blab("Adding build_flags for %s: %s" % (feature, f))
			new_flags = env.GetProjectOption('build_flags') + [ f ]
			env.Replace(BUILD_FLAGS=new_flags)

		if 'extra_scripts' in feat:
			blab("Running extra_scripts for %s... " % feature)
			env.SConscript(feat['extra_scripts'], exports="env")

		if 'src_filter' in feat:
			blab("Adding src_filter for %s... " % feature)
			src_filter = ' '.join(env.GetProjectOption('src_filter'))
			# first we need to remove the references to the same folder
			my_srcs = re.findall(r'[+-](<.*?>)', feat['src_filter'])
			cur_srcs = re.findall(r'[+-](<.*?>)', src_filter)
			for d in my_srcs:
				if d in cur_srcs:
					src_filter = re.sub(r'[+-]' + d, '', src_filter)

			src_filter = feat['src_filter'] + ' ' + src_filter
			set_env_field('src_filter', [src_filter])
			env.Replace(SRC_FILTER=src_filter)

		if 'lib_ignore' in feat:
			blab("Adding lib_ignore for %s... " % feature)
			lib_ignore = env.GetProjectOption('lib_ignore') + [feat['lib_ignore']]
			set_env_field('lib_ignore', lib_ignore)

#
# Find a compiler, considering the OS
#
ENV_BUILD_PATH = os.path.join(env.Dictionary('PROJECT_BUILD_DIR'), env['PIOENV'])
GCC_PATH_CACHE = os.path.join(ENV_BUILD_PATH, ".gcc_path")
def search_compiler():
	try:
		filepath = env.GetProjectOption('custom_gcc')
		blab("Getting compiler from env")
		return filepath
	except:
		pass

	if os.path.exists(GCC_PATH_CACHE):
		blab("Getting g++ path from cache")
		with open(GCC_PATH_CACHE, 'r') as f:
			return f.read()

	# Find the current platform compiler by searching the $PATH
	# which will be in a platformio toolchain bin folder
	path_regex = re.escape(env['PROJECT_PACKAGES_DIR'])
	gcc = "g++"
	if env['PLATFORM'] == 'win32':
		path_separator = ';'
		path_regex += r'.*\\bin'
		gcc += ".exe"
	else:
		path_separator = ':'
		path_regex += r'/.+/bin'

	# Search for the compiler
	for pathdir in env['ENV']['PATH'].split(path_separator):
		if not re.search(path_regex, pathdir, re.IGNORECASE):
			continue
		for filepath in os.listdir(pathdir):
			if not filepath.endswith(gcc):
				continue
			# Use entire path to not rely on env PATH
			filepath = os.path.sep.join([pathdir, filepath])
			# Cache the g++ path to no search always
			if os.path.exists(ENV_BUILD_PATH):
				blab("Caching g++ for current env")
				with open(GCC_PATH_CACHE, 'w+') as f:
					f.write(filepath)

			return filepath

	filepath = env.get('CXX')
	blab("Couldn't find a compiler! Fallback to %s" % filepath)
	return filepath

#
# Run the preprocessor
#
preprocessor_cache = {}
def run_preprocessor(filename):
	if filename in preprocessor_cache:
		return preprocessor_cache[filename]

	# Process defines
	build_flags = env.get('BUILD_FLAGS')
	build_flags = env.ParseFlagsExtended(build_flags)

	cxx = search_compiler()
	cmd = ['"' + cxx + '"']

	# Build flags from board.json
	#if 'BOARD' in env:
	#	cmd += [env.BoardConfig().get("build.extra_flags")]
	for s in build_flags['CPPDEFINES']:
		if isinstance(s, tuple):
			cmd += ['-D' + s[0] + '=' + str(s[1])]
		else:
			cmd += ['-D' + s]

	cmd += ['-D__MARLIN_DEPS__ -w -dM -E -x c++']
	depcmd = cmd + [ filename ]
	cmd = ' '.join(depcmd)
	blab(cmd)
	define_list = subprocess.check_output(cmd, shell=True).splitlines()
	preprocessor_cache[filename] = define_list
	return define_list


#
# Use the compiler to get a list of all enabled features
#
def load_marlin_features():
	if 'MARLIN_FEATURES' in env:
		return

	# Process defines
	define_list = run_preprocessor('buildroot/share/PlatformIO/scripts/common-dependencies.h')
	marlin_features = {}
	for define in define_list:
		feature = define[8:].strip().decode().split(' ')
		feature, definition = feature[0], ' '.join(feature[1:])
		marlin_features[feature] = definition
	env['MARLIN_FEATURES'] = marlin_features

#
# The dumbest preprocessor in the world
# Extract macro name from an header file and store them in an array
# No processing is done here, so they are raw values here and it does not match what actually enabled
# in the file (since you can have #if SOMETHING_UNDEFINED / #define BOB / #endif)
# But it's useful to filter the useful macro spit out by the preprocessor from noise from the system
# headers.
#
def extract_defines(filepath):
	f = open(filepath).read().split("\n")
	a = []
	for line in f:
		sline = line.strip(" \t\n\r")
		if sline[:7] == "#define":
			# Extract the key here (we don't care about the value)
			kv = sline[8:].strip().split(' ')
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
def compute_build_signature():
	if 'BUILD_SIGNATURE' in env:
		return

	# Definition from these files will be kept
	files_to_keep = [ 'Marlin/Configuration.h',
					  'Marlin/Configuration_adv.h',
	]

	# Check if we can skip processing
	hashes = ''
	for header in files_to_keep:
		hashes = hashes + get_file_sha256sum(header)

	marlin_json = 'marlin_config.json'

	# Read existing config file
	try:
		with open(marlin_json, 'r') as infile:
			conf = json.load(infile)
			if conf['__INITIAL_HASH'] == hashes:
				# Same configuration, skip recomputing the building signature
				compress_file(marlin_json, '.pio/build/mc')
				return
	except:
		pass

	# Need to parse all valid defines in the configuration files
	complete_cfg = run_preprocessor('buildroot/share/PlatformIO/scripts/common-dependencies.h')

	# Dumb #define extraction from the configuration files
	real_defines = {}
	all_defines = []
	for header in files_to_keep:
		defines = extract_defines(header)
		# To filter only the define we want
		all_defines = all_defines + defines
		# To remember from which file it cames from
		real_defines[header.split('/')[-1]] = defines


	r = re.compile("\(+(\s*-*\s*_.*)\)+")

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


	# Second step is to filter useless macro
	resolved_defines = {}
	for key in defines:
		# Remove all boards now
		if key[0:6] == "BOARD_" and key != "BOARD_INFO_NAME":
			continue
		# Remove all keys ending by "_NAME" as it does not make a difference to the configuration
		if key[-5:] == "_NAME" and key != "CUSTOM_MACHINE_NAME":
			continue
		# Remove all keys ending by "_T_DECLARED" as it's a copy of not important system stuff
		if key[-11:] == "_T_DECLARED":
			continue
		# Remove keys that are not in the #define list in the Configuration list
		if not(key in all_defines) and key != "DETAILED_BUILD_VERSION" and key != "STRING_DISTRIBUTION_DATE":
			continue

		# Don't be that smart guy here
		resolved_defines[key] = defines[key]

	# Generate a build signature now
	# We are making an object that's a bit more complex than a basic dictionary here
	data = {}
	data['__INITIAL_HASH'] = hashes
	# First create a key for each header here
	for header in real_defines:
		data[header] = {}

	# Then populate the object where each key is going to (that's a O(N^2) algorithm here...)
	for key in resolved_defines:
		for header in real_defines:
			if key in real_defines[header]:
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

	with open(marlin_json, 'w') as outfile:
		json.dump(data, outfile, separators=(',', ':'))

	# Now compress the JSON file to as much as we can
	compress_file(marlin_json, '.pio/build/mc')

#
# Return True if a matching feature is enabled
#
def MarlinFeatureIsEnabled(env, feature):
	load_marlin_features()
	r = re.compile('^' + feature + '$')
	found = list(filter(r.match, env['MARLIN_FEATURES']))

	# Defines could still be 'false' or '0', so check
	some_on = False
	if len(found):
		for f in found:
			val = env['MARLIN_FEATURES'][f]
			if val in [ '', '1', 'true' ]:
				some_on = True
			elif val in env['MARLIN_FEATURES']:
				some_on = env.MarlinFeatureIsEnabled(val)

	return some_on

#
# Add a method for other PIO scripts to query enabled features
#
env.AddMethod(MarlinFeatureIsEnabled)

#
# Add dependencies for enabled Marlin features
#
apply_features_config()
force_ignore_unused_libs()

# print(env.Dump())
compute_build_signature()
