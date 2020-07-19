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

TMCSTEPPER        = 'TMCStepper@~0.7.1'
SAILFISHLCD       = 'SailfishLCD=https://github.com/mikeshub/SailfishLCD/archive/master.zip'
SLOWSOFTI2CMASTER = 'SlowSoftI2CMaster'
TMC26XSTEPPER     = 'TMC26XStepper=https://github.com/trinamic/TMC26XStepper/archive/master.zip'
L64XX             = 'Arduino-L6470@0.8.0'
NEOPIXEL          = 'Adafruit NeoPixel@1.5.0'
MAX31865          = 'Adafruit MAX31865 library@~1.1.0'
U8GLIB            = 'U8glib-HAL@0.4.1'
LIQUIDCRYSTAL     = 'LiquidCrystal@1.5.0'
LIQUIDTWI2        = 'LiquidTWI2@1.2.7'
LITTLEVGL         = 'MKS-LittlevGL=https://github.com/makerbase-mks/MKS-LittlevGL/archive/master.zip'

FEATURE_DEPENDENCIES = {
	'TFT_LVGL_UI': {
		'lib_deps': [LITTLEVGL],
		# not supported yet
		#'src_filter': '+<src/lcd/extui/lib/mks_ui>',
		# script in another PR
		#'extra_scripts': 'buildroot/share/PlatformIO/scripts/dowload_mks_assets.py',
	},
	'HAS_TRINAMIC': {
		'lib_deps': [TMCSTEPPER]
	},
	'SR_LCD_2W_NL': {
		'lib_deps': [SAILFISHLCD]
	},
	'SR_LCD_3W_NL': {
		'lib_deps': [SAILFISHLCD]
	},
	'DIGIPOT_MCP4018': {
		'lib_deps': [SLOWSOFTI2CMASTER]
	},
	'DIGIPOT_MCP4451': {
		'lib_deps': [SLOWSOFTI2CMASTER]
	},
	'HAS_TMC26X': {
		'lib_deps': [TMC26XSTEPPER]
	},
	'HAS_L64XX': {
		'lib_deps': [L64XX]
	},
	'NEOPIXEL_LED': {
		'lib_deps': [NEOPIXEL]
	},
	'MAX6675_IS_MAX31865': {
		'lib_deps': [MAX31865]
	},
	'HAS_GRAPHICAL_LCD': {
		'lib_deps': [U8GLIB]
	},
	'HAS_CHARACTER_LCD': {
		'lib_deps': [LIQUIDCRYSTAL, LIQUIDTWI2]
	}
}

def install_features_dependencies():
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
	cmd = ['-D' + s for s in build_flags['CPPDEFINES']]
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
