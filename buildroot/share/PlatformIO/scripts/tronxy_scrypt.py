
import marlin

from SCons.Script import DefaultEnvironment

env = DefaultEnvironment()
board = env.BoardConfig()
if 'offset' in board.get("build").keys():
    marlin.relocate_vtab(board.get('build.offset'))
def output_target():
    tar_bin = "update/fmw_tronxy.bin"
    env.AddPostAction(
        "${BUILD_DIR}/${PROGNAME}.elf",
        env.VerboseAction(" ".join([
            "${OBJCOPY}", "-O", "binary", "-R", ".eeprom",
            "${BUILD_DIR}/${PROGNAME}.elf", tar_bin
        ]), "Building %s" % tar_bin)
    )
output_target()
