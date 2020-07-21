#
# common-features-dependencies.py
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
			if name == 'extra_scripts':
				FEATURE_DEPENDENCIES[ukey]['extra_scripts'] = rest
			elif name == 'src_filter':
				FEATURE_DEPENDENCIES[ukey]['src_filter'] = rest
			elif name == 'lib_ignore':
				FEATURE_DEPENDENCIES[ukey]['lib_ignore'] = rest
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
	lib_deps = env.GetProjectOption("lib_deps")
	for dep in lib_deps:
		name, _, _ = PackageManager.parse_pkg_uri(dep)
		env_libs.append(name)
	return env_libs

# We need to ignore all non-used libs,
# so if a lib folder lay forgotten in .pio/lib_deps, it
# will not break compiling
def force_ignore_unused_libs():
	env_libs = get_all_env_libs()
	known_libs = get_all_known_libs()
	diff = (list(set(known_libs) - set(env_libs)))
	lib_ignore = env.GetProjectOption("lib_ignore") + diff
	print("Ignoring libs: ", lib_ignore)
	proj = env.GetProjectConfig()
	proj.set("env:" + env["PIOENV"], "lib_ignore", lib_ignore)

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

			# first check if the env already have the dep
			deps = env.GetProjectOption("lib_deps")
			for dep in deps:
				name, _, _ = PackageManager.parse_pkg_uri(dep)
				if name in deps_to_add:
					del deps_to_add[name]

			# check if we need ignore any lib
			lib_ignore = env.GetProjectOption("lib_ignore")
			for dep in deps:
				name, _, _ = PackageManager.parse_pkg_uri(dep)
				if name in deps_to_add:
					del deps_to_add[name]

			# any left?
			if len(deps_to_add) > 0:
				# add only the missing deps
				proj = env.GetProjectConfig()
				proj.set("env:" + env["PIOENV"], "lib_deps", deps + list(deps_to_add.values()))

		if 'extra_scripts' in FEATURE_DEPENDENCIES[feature]:
			print("Executing extra_scripts for %s... " % feature)
			env.SConscript(FEATURE_DEPENDENCIES[feature]['extra_scripts'], exports="env")

		if 'src_filter' in FEATURE_DEPENDENCIES[feature]:
			print("Adding src_filter for %s... " % feature)
			proj = env.GetProjectConfig()
			src_filter = ' '.join(env.GetProjectOption("src_filter"))
			# first we need to remove the references to the same folder
			my_srcs = re.findall( r'[+-](<.*?>)', FEATURE_DEPENDENCIES[feature]['src_filter'])
			cur_srcs = re.findall( r'[+-](<.*?>)', src_filter)
			for d in my_srcs:
				if d in cur_srcs:
					src_filter = re.sub(r'[+-]' + d, '', src_filter)

			src_filter = FEATURE_DEPENDENCIES[feature]['src_filter'] + ' ' + src_filter
			proj.set("env:" + env["PIOENV"], "src_filter", [src_filter])
			env.Replace(SRC_FILTER=src_filter)

# search the current compiler, considering the OS
def search_compiler():
	if env['PLATFORM'] == 'win32':
		# the first path have the compiler
		for path in env['ENV']['PATH'].split(';'):
			if not re.search(r'platformio\\packages.*\\bin', path):
				continue
			#print(path)
			for file in os.listdir(path):
				if file.endswith("g++.exe"):
					return file
		print("Could not find the g++")
		return None
	else:
		return env.get('CXX')


# load marlin features
def load_marlin_features():
	if "MARLIN_FEATURES" in env:
		return

	# procces defines
	# print(env.Dump())
	build_flags = env.get('BUILD_FLAGS')
	build_flags = env.ParseFlagsExtended(build_flags)

	cxx = search_compiler()
	cmd = [cxx]

	# build flags from board.json
	# if 'BOARD' in env:
	# 	cmd += [env.BoardConfig().get("build.extra_flags")]
	for s in build_flags['CPPDEFINES']:
		if isinstance(s, tuple):
			cmd += ['-D' + s[0] + '=' + str(s[1])]
		else:
			cmd += ['-D' + s]
	# cmd += ['-w -dM -E -x c++ Marlin/src/inc/MarlinConfigPre.h']
	cmd += ['-w -dM -E -x c++ buildroot/share/PlatformIO/scripts/common-features-dependencies.h']
	cmd = ' '.join(cmd)
	print(cmd)
	define_list = subprocess.check_output(cmd, shell=True).splitlines()
	marlin_features = {}
	for define in define_list:
		feature = define[8:].strip().decode().split(' ')
		feature, definition = feature[0], ' '.join(feature[1:])
		marlin_features[feature] = definition
	env["MARLIN_FEATURES"] = marlin_features

def MarlinFeatureIsEnabled(env, feature):
	load_marlin_features()
	r = re.compile(feature)
	matches = list(filter(r.match, env["MARLIN_FEATURES"]))
	return len(matches) > 0

# add a method for others scripts to check if a feature is enabled
env.AddMethod(MarlinFeatureIsEnabled)

# install all dependencies for features enabled in Configuration.h
install_features_dependencies()
force_ignore_unused_libs()
