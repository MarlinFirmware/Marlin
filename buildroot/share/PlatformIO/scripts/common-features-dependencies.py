#
# common-features-dependencies.py
# Convenience script to check dependencies and add libs and sources for Marlin Enabled Features
#
import subprocess
import os
try:
    import configparser
except ImportError:
    import ConfigParser as configparser

Import("env")

FEATURE_DEPENDENCIES = {}

def load_config():
	config = configparser.ConfigParser()
	config.read("platformio.ini")
	items = config.items('features')
	for key in items:
		FEATURE_DEPENDENCIES[key[0].upper()] = {
			'lib_deps': key[1].split('\n')
		}

def install_features_dependencies():
	load_config()
	for feature in FEATURE_DEPENDENCIES:
		if not env.MarlinFeatureIsEnabled(feature):
			continue

		#print("Feature enabled: %s" % feature)
		if 'lib_deps' in FEATURE_DEPENDENCIES[feature]:
			print("Adding lib_deps for %s... " % feature)
			proj = env.GetProjectConfig()
			deps = env.GetProjectOption("lib_deps")
			proj.set("env:" + env["PIOENV"], "lib_deps", deps + FEATURE_DEPENDENCIES[feature]['lib_deps'])
		if 'extra_scripts' in FEATURE_DEPENDENCIES[feature]:
			print("Executing extra_scripts for %s... " % feature)
			env.SConscript(FEATURE_DEPENDENCIES[feature]['extra_scripts'], exports="env")
		# if 'src_filter' in deps[d]:
		# 	env.Replace(SRC_FILTER=''.join(env.get("SRC_FILTER")) + ' ' + deps[d]['src_filter'])

# load marlin features
def load_marlin_features():
	if "MARLIN_FEATURES" in env:
		return

	# procces defines
	build_flags = env.get('BUILD_FLAGS')
	build_flags = env.ParseFlagsExtended(build_flags)
	cmd = []
	for s in build_flags['CPPDEFINES']:
		if isinstance(s, tuple):
			cmd += ['-D' + s[0] + '=' + str(s[1])]
		else:
			cmd += ['-D' + s]
	cmd += ['-w -dM -E -x c++ Marlin/src/inc/MarlinConfigPre.h']
	cmd = [env.get('CXX')] + cmd
	cmd = ' '.join(cmd)
	define_list = subprocess.check_output(cmd, shell=True).splitlines()
	marlin_features = {}
	for define in define_list:
		feature = define[8:].strip().decode().split(' ')
		feature, definition = feature[0], ' '.join(feature[1:])
		marlin_features[feature] = definition
	env["MARLIN_FEATURES"] = marlin_features

def MarlinFeatureIsEnabled(env, feature):
	load_marlin_features()
	return feature in env["MARLIN_FEATURES"]

# add a method for others scripts to check if a feature is enabled
env.AddMethod(MarlinFeatureIsEnabled)

# install all dependencies for features enabled in Configuration.h
install_features_dependencies()
