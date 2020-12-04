import os,sys
Import("env")

from SCons.Script import DefaultEnvironment
from os.path import join

custom_ld_script = os.path.abspath("buildroot/share/PlatformIO/ldscripts/anet_et4.ld")
for i, flag in enumerate(env["LINKFLAGS"]):
    if "-Wl,-T" in flag:
        env["LINKFLAGS"][i] = "-Wl,-T" + custom_ld_script
    elif flag == "-T":
        env["LINKFLAGS"][i + 1] = custom_ld_script

env.AddPostAction(
    "$BUILD_DIR/${PROGNAME}.elf",
    env.VerboseAction(" ".join([
        "$OBJCOPY", "-O", "srec",
        "\"$BUILD_DIR/${PROGNAME}.elf\"", "\"$BUILD_DIR/${PROGNAME}.srec\""
    ]), "Building " + join("$BUILD_DIR","${PROGNAME}.srec"))
)
