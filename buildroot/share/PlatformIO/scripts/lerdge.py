Import("env")
import os

FIRMWARE=""

# Specify output file name with -DFIRMWARE=<filename.bin> build parameter
for define in env['CPPDEFINES']:
    if define[0] == "FIRMWARE":
        FIRMWARE=define[1]

if FIRMWARE == "":
  print("You need to define output file via '-DFIRMWARE' build parameter", file=sys.stderr)
  exit(1);

# Relocate firmware from 0x08000000 to 0x08008800
#env['CPPDEFINES'].remove(("VECT_TAB_ADDR", "0x8000000"))
env['CPPDEFINES'].append(("VECT_TAB_ADDR", "0x8010000"))

custom_ld_script = os.path.abspath("buildroot/share/PlatformIO/ldscripts/lerdge.ld")
for i, flag in enumerate(env["LINKFLAGS"]):
    if "-Wl,-T" in flag:
        env["LINKFLAGS"][i] = "-Wl,-T" + custom_ld_script
    elif flag == "-T":
        env["LINKFLAGS"][i + 1] = custom_ld_script

def encryptByte(byte):
    byte = 0xFF & ((byte << 6) | (byte >> 2))
    i = 0x58 + byte
    j = 0x05 + byte + (i >> 8)
    byte = (0xF8 & i) | (0x07 & j)
    return byte

def encrypt_file(input, output_file, file_length):
    input_file = bytearray(input.read())
    for i in range(len(input_file)):
        result = encryptByte(input_file[i])
        input_file[i] = result

    output_file.write(input_file)
    return


# Encrypt ${PROGNAME}.bin and save it as 'update.cbd'
def encrypt(source, target, env):
    print("Encrypting to:", FIRMWARE)
    firmware = open(target[0].path, "rb")
    result = open(target[0].dir.path + "/" + FIRMWARE, "wb")
    length = os.path.getsize(target[0].path)

    encrypt_file(firmware, result, length)

    firmware.close()
    result.close()

env.AddPostAction("$BUILD_DIR/${PROGNAME}.bin", encrypt);