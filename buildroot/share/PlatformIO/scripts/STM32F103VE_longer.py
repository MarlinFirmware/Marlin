#
# STM32F103VE_longer.py
# Customizations for env:STM32F103VE_longer
#
import os
Import("env")

# Relocate firmware from 0x08000000 to 0x08010000
for define in env['CPPDEFINES']:
    if define[0] == "VECT_TAB_ADDR":
        env['CPPDEFINES'].remove(define)
env['CPPDEFINES'].append(("VECT_TAB_ADDR", "0x08010000"))

custom_ld_script = os.path.abspath("buildroot/share/PlatformIO/ldscripts/STM32F103VE_longer.ld")
for i, flag in enumerate(env["LINKFLAGS"]):
    if "-Wl,-T" in flag:
        env["LINKFLAGS"][i] = "-Wl,-T" + custom_ld_script
    elif flag == "-T":
        env["LINKFLAGS"][i + 1] = custom_ld_script

# Rename ${PROGNAME}.bin and save it as 'project.bin' (No encryption on the Longer3D)
def encrypt(source, target, env):
    firmware = open(target[0].path, "rb")
    marlin_alfa = open(target[0].dir.path +'/project.bin', "wb")
    length = os.path.getsize(target[0].path)
    position = 0
    try:
        while position < length:
            byte = firmware.read(1)
            marlin_alfa.write(byte)
            position += 1
    finally:
        firmware.close()
        marlin_alfa.close()

env.AddPostAction("$BUILD_DIR/${PROGNAME}.bin", encrypt);
