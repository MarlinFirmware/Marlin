#
# common-dependencies.py
# Convenience script to check dependencies and add libs and sources for Marlin Enabled Features
#
import subprocess,os,re

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

################################################################################
#
# Get the 'name' field from a PackageManager URI
#
def parse_pkg_uri(spec):
	if PackageManager.__name__ == 'PackageSpec':
		return PackageManager(spec).name
	else:
		name, _, _ = PackageManager.parse_pkg_uri(spec)
		return name

################################################################################
#
# Container for all the parsed [feature] info
#
FEATURE_CONFIG = {}

################################################################################
#
# Add an entry to FEATURE_CONFIG for the given feature
#
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

################################################################################
#
# Populate FEATURE_CONFIG from the [features] group in platformio.ini
#
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

################################################################################
#
# Return a list of all libraries that a feature could enable
#
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

################################################################################
#
# Return a list of all libraries enabled for the PlatformIO build in progress
#
def get_all_env_libs():
	env_libs = []
	lib_deps = env.GetProjectOption('lib_deps')
	for dep in lib_deps:
		name = parse_pkg_uri(dep)
		env_libs.append(name)
	return env_libs

################################################################################
#
# Add or override a field's value in the current PlatformIO environment
#
def set_env_field(field, value):
	proj = env.GetProjectConfig()
	proj.set("env:" + env['PIOENV'], field, value)

################################################################################
#
# Add to lib_ignore all the unused libraries mentioned in [features]
# (so if a library exists in .pio/lib_deps it won't break compilation)
#
def force_ignore_unused_libs():
	env_libs = get_all_env_libs()
	known_libs = get_all_known_libs()
	diff = (list(set(known_libs) - set(env_libs)))
	lib_ignore = env.GetProjectOption('lib_ignore') + diff
	blab("Ignore libraries: %s" % lib_ignore)
	set_env_field('lib_ignore', lib_ignore)


################################################################################
#
# Apply enabled [features] to the settings for the PlatformIO build in progress
#
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


################################################################################
#
# Use the compiler to get a list of all enabled features
#
def load_marlin_features():
	if 'MARLIN_FEATURES' in env:
		return

	# Process defines
	from preprocessor import run_preprocessor
	define_list = run_preprocessor(env)
	marlin_features = {}
	for define in define_list:
		feature = define[8:].strip().decode().split(' ')
		feature, definition = feature[0], ' '.join(feature[1:])
		marlin_features[feature] = definition
	env['MARLIN_FEATURES'] = marlin_features


################################################################################
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


################################################################################
################################################################################

#
# Add a method for other PIO scripts to query enabled features
#
env.AddMethod(MarlinFeatureIsEnabled)

#
# Add dependencies for enabled Marlin features
#
apply_features_config()
force_ignore_unused_libs()

#print(env.Dump())

from signature import compute_build_signature
compute_build_signature(env)
