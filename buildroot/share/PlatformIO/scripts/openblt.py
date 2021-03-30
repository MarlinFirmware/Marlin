#
# Convert the ELF to an SREC file suitable for some bootloaders
#
import os,sys
from os.path import join

Import("env")

env.AddPostAction(
    "$BUILD_DIR/${PROGNAME}.elf",
    env.VerboseAction(" ".join([
        "$OBJCOPY", "-O", "srec",
        "\"$BUILD_DIR/${PROGNAME}.elf\"", "\"$BUILD_DIR/${PROGNAME}.srec\""
    ]), "Building " + join("$BUILD_DIR", "${PROGNAME}.srec"))
)
