import os
Import("env")

# Relocate firmware from 0x08000000 to 0x08007000
for define in env['CPPDEFINES']:
    if define[0] == "VECT_TAB_ADDR":
        env['CPPDEFINES'].remove(define)
env['CPPDEFINES'].append(("VECT_TAB_ADDR", "0x08007000"))

custom_ld_script = os.path.abspath("buildroot/share/PlatformIO/ldscripts/mks_robin_nano.ld")
for i, flag in enumerate(env["LINKFLAGS"]):
    if "-Wl,-T" in flag:
        env["LINKFLAGS"][i] = "-Wl,-T" + custom_ld_script
    elif flag == "-T":
        env["LINKFLAGS"][i + 1] = custom_ld_script


# Encrypt ${PROGNAME}.bin and save it as 'Robin_nano35.bin'
def encrypt(source, target, env):
    import sys

    key = [0xA3, 0xBD, 0xAD, 0x0D, 0x41, 0x11, 0xBB, 0x8D, 0xDC, 0x80, 0x2D, 0xD0, 0xD2, 0xC4, 0x9B, 0x1E, 0x26, 0xEB, 0xE3, 0x33, 0x4A, 0x15, 0xE4, 0x0A, 0xB3, 0xB1, 0x3C, 0x93, 0xBB, 0xAF, 0xF7, 0x3E]

    firmware = open(target[0].path, "rb")
    robin = open(target[0].dir.path +'/Robin_nano35.bin', "wb")
    length = os.path.getsize(target[0].path)
    position = 0
    try:
        while position < length:
            byte = firmware.read(1)
            if position >= 320 and position < 31040:
                byte = chr(ord(byte) ^ key[position & 31])
                if sys.version_info[0] > 2:
                    byte = bytes(byte, 'latin1')
            robin.write(byte)
            position += 1
    finally:
        firmware.close()
        robin.close()
env.AddPostAction("$BUILD_DIR/${PROGNAME}.bin", encrypt);
