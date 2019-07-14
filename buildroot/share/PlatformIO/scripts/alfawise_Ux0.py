Import("env")

# Relocate firmware from 0x08000000 to 0x08010000
for define in env['CPPDEFINES']:
    if define[0] == "VECT_TAB_ADDR":
        env['CPPDEFINES'].remove(define)
env['CPPDEFINES'].append(("VECT_TAB_ADDR", "0x08010000"))
env.Replace(LDSCRIPT_PATH="buildroot/share/PlatformIO/ldscripts/alfawise_Ux0.ld")

# Encrypt ${PROGNAME}.bin and save it as 'project.bin'
def encrypt(source, target, env):
    import os

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
