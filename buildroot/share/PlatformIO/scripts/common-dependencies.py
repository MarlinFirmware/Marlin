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
from platformio.managers.package import PackageManager

Import("env")

FEATURE_DEPENDENCIES = {}

def load_config():
	config = configparser.ConfigParser()
	config.read("platformio.ini")
	items = config.items('features')
	for key in items:
		ukey = key[0].upper()
		if not ukey in FEATURE_DEPENDENCIES:
			FEATURE_DEPENDENCIES[ukey] = {
				'lib_deps': []
			}
		deps = re.sub(',\\s*', '\n', key[1]).strip().split('\n')
		for dep in deps:
			parts = dep.split('=')
			name = parts.pop(0)
			rest = '='.join(parts)
			if name in ['extra_scripts', 'src_filter', 'lib_ignore']:
				FEATURE_DEPENDENCIES[ukey][name] = rest
			else:
				FEATURE_DEPENDENCIES[ukey]['lib_deps'] += [dep]

def get_all_known_libs():
	known_libs = []
	for feature in FEATURE_DEPENDENCIES:
		if not 'lib_deps' in FEATURE_DEPENDENCIES[feature]:
			continue
		for dep in FEATURE_DEPENDENCIES[feature]['lib_deps']:
			name, _, _ = PackageManager.parse_pkg_uri(dep)
			known_libs.append(name)
	return known_libs

def get_all_env_libs():
	env_libs = []
	lib_deps = env.GetProjectOption('lib_deps')
	for dep in lib_deps:
		name, _, _ = PackageManager.parse_pkg_uri(dep)
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
	print("Ignoring libs:", lib_ignore)
	set_env_field('lib_ignore', lib_ignore)

def install_features_dependencies():
	load_config()
	for feature in FEATURE_DEPENDENCIES:
		if not env.MarlinFeatureIsEnabled(feature):
			continue

		if 'lib_deps' in FEATURE_DEPENDENCIES[feature]:
			print("Adding lib_deps for %s... " % feature)

			# deps to add
			deps_to_add = {}
			for dep in FEATURE_DEPENDENCIES[feature]['lib_deps']:
				name, _, _ = PackageManager.parse_pkg_uri(dep)
				deps_to_add[name] = dep

			# Does the env already have the dependency?
			deps = env.GetProjectOption('lib_deps')
			for dep in deps:
				name, _, _ = PackageManager.parse_pkg_uri(dep)
				if name in deps_to_add:
					del deps_to_add[name]

			# Are there any libraries that should be ignored?
			lib_ignore = env.GetProjectOption('lib_ignore')
			for dep in deps:
				name, _, _ = PackageManager.parse_pkg_uri(dep)
				if name in deps_to_add:
					del deps_to_add[name]

			# Is there anything left?
			if len(deps_to_add) > 0:
				# Only add the missing dependencies
				set_env_field('lib_deps', deps + list(deps_to_add.values()))

		if 'extra_scripts' in FEATURE_DEPENDENCIES[feature]:
			print("Executing extra_scripts for %s... " % feature)
			env.SConscript(FEATURE_DEPENDENCIES[feature]['extra_scripts'], exports="env")

		if 'src_filter' in FEATURE_DEPENDENCIES[feature]:
			print("Adding src_filter for %s... " % feature)
			src_filter = ' '.join(env.GetProjectOption('src_filter'))
			# first we need to remove the references to the same folder
			my_srcs = re.findall( r'[+-](<.*?>)', FEATURE_DEPENDENCIES[feature]['src_filter'])
			cur_srcs = re.findall( r'[+-](<.*?>)', src_filter)
			for d in my_srcs:
				if d in cur_srcs:
					src_filter = re.sub(r'[+-]' + d, '', src_filter)

			src_filter = FEATURE_DEPENDENCIES[feature]['src_filter'] + ' ' + src_filter
			set_env_field('src_filter', [src_filter])
			env.Replace(SRC_FILTER=src_filter)

		if 'lib_ignore' in FEATURE_DEPENDENCIES[feature]:
			print("Ignoring libs for %s... " % feature)
			lib_ignore = env.GetProjectOption('lib_ignore') + [FEATURE_DEPENDENCIES[feature]['lib_ignore']]
			set_env_field('lib_ignore', lib_ignore)

#
# Find a compiler, considering the OS
#
ENV_BUILD_PATH = os.path.join(env.Dictionary('PROJECT_BUILD_DIR'), env['PIOENV'])
GCC_PATH_CACHE = os.path.join(ENV_BUILD_PATH, ".gcc_path")
def search_compiler():
	if os.path.exists(GCC_PATH_CACHE):
		print('Getting g++ path from cache')
		with open(GCC_PATH_CACHE, 'r') as f:
			return f.read()

	# PlatformIO inserts the toolchain bin folder on the front of the $PATH
	# Find the current platform compiler by searching the $PATH
	if env['PLATFORM'] == 'win32':
		path_separator = ';'
		path_regex = r'platformio\\packages.*\\bin'
		gcc = "g++.exe"
	else:
		path_separator = ':'
		path_regex = r'platformio/packages.*/bin'
		gcc = "g++"

	# Search for the compiler
	for path in env['ENV']['PATH'].split(path_separator):
		if not re.search(path_regex, path):
			continue
		for file in os.listdir(path):
			if not file.endswith(gcc):
				continue

			# Cache the g++ path to no search always
			if os.path.exists(ENV_BUILD_PATH):
				print('Caching g++ for current env')
				with open(GCC_PATH_CACHE, 'w+') as f:
					f.write(file)

			return file

	file = env.get('CXX')
	print("Couldn't find a compiler! Fallback to", file)
	return file

#
# Use the compiler to get a list of all enabled features
#
def load_marlin_features():
	if "MARLIN_FEATURES" in env:
		return

	# Process defines
	#print(env.Dump())
	build_flags = env.get('BUILD_FLAGS')
	build_flags = env.ParseFlagsExtended(build_flags)

	cxx = search_compiler()
	cmd = [cxx]

	# Build flags from board.json
	#if 'BOARD' in env:
	#	cmd += [env.BoardConfig().get("build.extra_flags")]
	for s in build_flags['CPPDEFINES']:
		if isinstance(s, tuple):
			cmd += ['-D' + s[0] + '=' + str(s[1])]
		else:
			cmd += ['-D' + s]

	cmd += ['-w -dM -E -x c++ buildroot/share/PlatformIO/scripts/common-dependencies.h']
	cmd = ' '.join(cmd)
	print(cmd)
	define_list = subprocess.check_output(cmd, shell=True).splitlines()
	marlin_features = {}
	for define in define_list:
		feature = define[8:].strip().decode().split(' ')
		feature, definition = feature[0], ' '.join(feature[1:])
		marlin_features[feature] = definition
	env["MARLIN_FEATURES"] = marlin_features

#
# Return True if a matching feature is enabled
#
def MarlinFeatureIsEnabled(env, feature):
	load_marlin_features()
	r = re.compile(feature)
	matches = list(filter(r.match, env["MARLIN_FEATURES"]))
	return len(matches) > 0

#
# Add a method for other PIO scripts to query enabled features
#
env.AddMethod(MarlinFeatureIsEnabled)

#
# Add dependencies for enabled Marlin features
#
install_features_dependencies()
force_ignore_unused_libs()
