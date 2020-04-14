from os.path import join
Import("env", "projenv")

import os,shutil
from SCons.Script import DefaultEnvironment
from platformio import util

env = DefaultEnvironment()
platform = env.PioPlatform()
board = env.BoardConfig()

FRAMEWORK_DIR = platform.get_package_dir("framework-arduinoststm32")
#FRAMEWORK_DIR = platform.get_package_dir("framework-arduinoststm32@3.10500.190327")
CMSIS_DIR = os.path.join(FRAMEWORK_DIR, "CMSIS", "CMSIS")
assert os.path.isdir(FRAMEWORK_DIR)
assert os.path.isdir(CMSIS_DIR)
assert os.path.isdir("buildroot/share/PlatformIO/variants")

mcu_type = board.get("build.mcu")[:-2]
variant = board.get("build.variant")
series = mcu_type[:7].upper() + "xx"
variant_dir = os.path.join(FRAMEWORK_DIR, "variants", variant)

source_dir = os.path.join("buildroot/share/PlatformIO/variants", variant)
assert os.path.isdir(source_dir)

if not os.path.isdir(variant_dir):
    os.mkdir(variant_dir)

for file_name in os.listdir(source_dir):
    full_file_name = os.path.join(source_dir, file_name)
    if os.path.isfile(full_file_name):
        shutil.copy(full_file_name, variant_dir)

# Relocate firmware from 0x08000000 to 0x08002000
#env['CPPDEFINES'].remove(("VECT_TAB_ADDR", 134217728))
#env['CPPDEFINES'].append(("VECT_TAB_ADDR", "0x08010000"))
#env.Replace(LDSCRIPT_PATH="buildroot/share/PlatformIO/ldscripts/fysetc_aio_ii.ld")

# Custom HEX from ELF
env.AddPostAction(
	"$BUILD_DIR/${PROGNAME}.elf",
	env.VerboseAction(" ".join([
				"$OBJCOPY",
				"-O",
				"ihex",
				"$BUILD_DIR/${PROGNAME}.elf",
				"$BUILD_DIR/${PROGNAME}.hex"
			]), "Building $TARGET"))

# In-line command with arguments
env.Replace(
	UPLOADER=platform.get_package_dir("tool-stm32duino") + '/stm32flash/stm32flash',
	UPLOADCMD='"${UPLOADER}" -v -i rts,-dtr,dtr,-rts -R -b 115200 -g 0x8000000 -w "${BUILD_DIR}/${PROGNAME}.hex" ${UPLOAD_PORT}'
)
