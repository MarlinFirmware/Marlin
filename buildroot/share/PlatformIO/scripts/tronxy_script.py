
import marlin
import os

from SCons.Script import DefaultEnvironment

env = DefaultEnvironment()
board = env.BoardConfig()
if 'offset' in board.get("build").keys():
    marlin.relocate_vtab(board.get('build.offset'))

path = "update"
# Check whether the "update" folder exists
isExist = os.path.exists(path)
if not isExist:
   # Create the "update" folder
   os.makedirs(path)

#build "fmw_tronxy.hex" and place in "update" folder
def output_target_hex():
    tar_hex = "update/fmw_tronxy.hex"
    env.AddPostAction(
        "$BUILD_DIR/${PROGNAME}.elf",
        env.VerboseAction(" ".join([
            "$OBJCOPY", "-O", "ihex", "-R", ".eeprom",
            "$BUILD_DIR/${PROGNAME}.elf", tar_hex
        ]), "Building %s" % tar_hex)
    )

#build "fmw_tronxy.bin" and place in "update" folder
def output_target_bin():
    tar_bin = "update/fmw_tronxy.bin"
    env.AddPostAction(
        "$BUILD_DIR/${PROGNAME}.elf",
        env.VerboseAction(" ".join([
            "$OBJCOPY", "-O", "binary", "-R", ".eeprom",
            "$BUILD_DIR/${PROGNAME}.elf", tar_bin
        ]), "Building %s" % tar_bin)
    )

output_target_hex()
output_target_bin()
