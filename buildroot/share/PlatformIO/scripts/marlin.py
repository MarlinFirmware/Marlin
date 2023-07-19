#
# buildroot/share/PlatformIO/scripts/marlin.py
# Helper module with some commonly-used functions
#
from SCons.Script import DefaultEnvironment
env = DefaultEnvironment()

def replace_define(field, value):
	envdefs = env['CPPDEFINES'].copy()
	for define in envdefs:
		if define[0] == field:
			env['CPPDEFINES'].remove(define)
	env['CPPDEFINES'].append((field, value))

# Relocate the firmware to a new address, such as "0x08005000"
def relocate_firmware(address):
	replace_define("VECT_TAB_ADDR", address)
