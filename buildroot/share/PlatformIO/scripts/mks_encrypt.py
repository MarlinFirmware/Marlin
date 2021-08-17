#
# buildroot/share/PlatformIO/scripts/mks_encrypt.py
#
# Apply encryption and save as 'build.firmware' for these environments:
#  - env:mks_robin
#  - env:mks_robin_e3
#  - env:flsun_hispeedv1
#  - env:mks_robin_nano35
#
Import("env")

from SCons.Script import DefaultEnvironment
board = DefaultEnvironment().BoardConfig()

if 'encrypt' in board.get("build").keys():

	import marlin

	# Encrypt ${PROGNAME}.bin and save it with the name given in build.encrypt
	def encrypt(source, target, env):
		marlin.encrypt_mks(source, target, env, board.get("build.encrypt"))

	marlin.add_post_action(encrypt);

else:

	import sys
	print("You need to define output file via board_build.encrypt = 'filename' parameter", file=sys.stderr)
	env.Exit(1);
