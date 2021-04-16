#
# stm32_bootloader.py
#
import os,sys,shutil
Import("env")

from SCons.Script import DefaultEnvironment
board = DefaultEnvironment().BoardConfig()

#
# Copy the firmware.bin file to build.firmware, no encryption
#
def noencrypt(source, target, env):
  firmware = os.path.join(target[0].dir.path, board.get("build.firmware"))
  shutil.copy(target[0].path, firmware)

#
# For build.offset define LD_FLASH_OFFSET, used by ldscript.ld
#
if 'offset' in board.get("build").keys():
  LD_FLASH_OFFSET = board.get("build.offset")

  # Remove an existing VECT_TAB_OFFSET from CPPDEFINES
  for define in env['CPPDEFINES']:
    if define[0] == "VECT_TAB_OFFSET":
      env['CPPDEFINES'].remove(define)

  # Replace VECT_TAB_OFFSET with our LD_FLASH_OFFSET
  env['CPPDEFINES'].append(("VECT_TAB_OFFSET", LD_FLASH_OFFSET))

  # Get upload.maximum_ram_size (defined by /buildroot/share/PlatformIO/boards/VARIOUS.json)
  maximum_ram_size = board.get("upload.maximum_ram_size")

  for i, flag in enumerate(env["LINKFLAGS"]):
    if "-Wl,--defsym=LD_FLASH_OFFSET" in flag:
      env["LINKFLAGS"][i] = "-Wl,--defsym=LD_FLASH_OFFSET=" + LD_FLASH_OFFSET
    if "-Wl,--defsym=LD_MAX_DATA_SIZE" in flag:
      env["LINKFLAGS"][i] = "-Wl,--defsym=LD_MAX_DATA_SIZE=" + str(maximum_ram_size - 40)

#
# Only copy the file if there's no encrypt
#
board_keys = board.get("build").keys()
if 'firmware' in board_keys and not 'encrypt' in board_keys:
  env.AddPostAction("$BUILD_DIR/${PROGNAME}.bin", noencrypt)
