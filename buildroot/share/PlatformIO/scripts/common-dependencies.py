#
# common-dependencies.py
# Convenience script to check dependencies and add libs and sources for Marlin Enabled Features
#
import subprocess
import os
import re
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

	feat = FEATURE_CONFIG[feature]
	atoms = re.sub(',\\s*', '\n', flines).strip().split('\n')
	for dep in atoms:
		parts = dep.split('=')
		name = parts.pop(0)
		rest = '='.join(parts)
		if name in ['extra_scripts', 'src_filter', 'lib_ignore']:
			feat[name] = rest
		else:
			feat['lib_deps'] += [dep]

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
	if verbose:
		print("Ignore libraries:", lib_ignore)
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

		if 'extra_scripts' in feat:
			blab("Running extra_scripts for %s... " % feature)
			env.SConscript(feat['extra_scripts'], exports="env")

		if 'src_filter' in feat:
			blab("Adding src_filter for %s... " % feature)
			src_filter = ' '.join(env.GetProjectOption('src_filter'))
			# first we need to remove the references to the same folder
			my_srcs = re.findall( r'[+-](<.*?>)', feat['src_filter'])
			cur_srcs = re.findall( r'[+-](<.*?>)', src_filter)
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
		blab('Getting compiler from env')
		return filepath
	except:
		pass

	if os.path.exists(GCC_PATH_CACHE):
		blab('Getting g++ path from cache')
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
				blab('Caching g++ for current env')
				with open(GCC_PATH_CACHE, 'w+') as f:
					f.write(filepath)

			return filepath

	filepath = env.get('CXX')
	blab("Couldn't find a compiler! Fallback to %s" % filepath)
	return filepath

#
# Use the compiler to get a list of all enabled features
#
def load_marlin_features():
	if 'MARLIN_FEATURES' in env:
		return

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

	cmd += ['-D__MARLIN_DEPS__ -w -dM -E -x c++ buildroot/share/PlatformIO/scripts/common-dependencies.h']
	cmd = ' '.join(cmd)
	blab(cmd)
	define_list = subprocess.check_output(cmd, shell=True).splitlines()
	marlin_features = {}
	for define in define_list:
		feature = define[8:].strip().decode().split(' ')
		feature, definition = feature[0], ' '.join(feature[1:])
		marlin_features[feature] = definition
	env['MARLIN_FEATURES'] = marlin_features

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
