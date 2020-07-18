#
# common-check-deps.py
# Convenience script to check dependencies and add libs and sources for Marlin Enabled Features
#
import subprocess
import os
try:
    import configparser
except ImportError:
    import ConfigParser as configparser

Import("env")

FEATURE_DEPENDENCIES = {
	'TFT_LVGL_UI': {
		'lib_deps': ['MKS-LittlevGL=https://github.com/makerbase-mks/MKS-LittlevGL/archive/master.zip'],
		# not supported yet
		#'src_filter': '+<src/lcd/extui/lib/mks_ui>',
		# script in another PR
		#'extra_scripts': 'buildroot/share/PlatformIO/scripts/dowload_mks_assets.py', 
	}
}

def install_features_dependencies():
	for feature in FEATURE_DEPENDENCIES:
		if env.MarlinFeatureIsEnabled(feature) == False:
			continue

		print("Feature enabled: %s" % feature)
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
	define_list = subprocess.check_output(' '.join([env.get('CXX'), '-w -dM -E -x c++ Marlin/Configuration.h -includeMarlin/src/core/macros.h']), shell=True).splitlines()
	marlin_features = {}
	for define in define_list:
		feature = define[8:].strip().decode().split(' ')
		feature, definition = feature[0], ' '.join(feature[1:])
		marlin_features[feature] = definition
	env["MARLIN_FEATURES"] = marlin_features

def MarlinFeatureIsEnabled(env, feature):
	load_marlin_features()
	if feature in env["MARLIN_FEATURES"]:
		return True
	else:
		return False

# add a method for others scripts to check if a feature is enabled
env.AddMethod(MarlinFeatureIsEnabled)

# install all dependencies for features enabled in Configuration.h
install_features_dependencies()