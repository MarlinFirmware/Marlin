#
# STM32F103RC_MEEB_3DP.py
#
import pioutil
if pioutil.is_pio_build():

	try:
		import configparser
	except ImportError:
		import ConfigParser as configparser

	import os
	Import("env", "projenv")

	config = configparser.ConfigParser()
	config.read("platformio.ini")

	#
	# Upload actions
	#
	def before_upload(source, target, env):
		env.Execute("pwd")

	def after_upload(source, target, env):
		env.Execute("pwd")

	env.AddPreAction("upload", before_upload)
	env.AddPostAction("upload", after_upload)

	flash_size = 0
	vect_tab_addr = 0

	for define in env['CPPDEFINES']:
		if define[0] == "VECT_TAB_ADDR":
			vect_tab_addr = define[1]
		if define[0] == "STM32_FLASH_SIZE":
			flash_size = define[1]

	print('Use the {0:s} address as the marlin app entry point.'.format(vect_tab_addr))
	print('Use the {0:d}KB flash version of stm32f103rct6 chip.'.format(flash_size))

	import marlin
	marlin.custom_ld_script("STM32F103RC_MEEB_3DP.ld")
