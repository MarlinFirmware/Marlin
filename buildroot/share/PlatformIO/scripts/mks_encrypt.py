#
# buildroot/share/PlatformIO/scripts/mks_encrypt.py
#
# Apply encryption and save as 'build.firmware' for these environments:
#  - env:mks_robin_stm32
#  - env:flsun_hispeedv1
#  - env:mks_robin_nano35_stm32
#
Import("env")

from SCons.Script import DefaultEnvironment
board = DefaultEnvironment().BoardConfig()

if 'firmware' in board.get("build").keys():

	import marlin

	# Encrypt ${PROGNAME}.bin and save it as build.firmware
	def encrypt(source, target, env):
		marlin.encrypt_mks(source, target, env, "build.firmware")

	marlin.add_post_action(encrypt);

else:

	import sys
	print("You need to define output file via board_build.firmware = 'filename' parameter", file=sys.stderr)
	env.Exit(1);
