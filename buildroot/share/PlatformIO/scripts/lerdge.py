#
# lerdge.py
# Customizations for Lerdge build environments
#
import os,sys
Import("env")

from SCons.Script import DefaultEnvironment
board = DefaultEnvironment().BoardConfig()

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

# Encrypt ${PROGNAME}.bin and save it as build.firmware
def encrypt(source, target, env):
    print("Encrypting to:", board.get("build.firmware"))
    firmware = open(target[0].path, "rb")
    result = open(target[0].dir.path + "/" + board.get("build.firmware"), "wb")
    length = os.path.getsize(target[0].path)

    encrypt_file(firmware, result, length)

    firmware.close()
    result.close()

if 'firmware' in board.get("build").keys():
  env.AddPostAction("$BUILD_DIR/${PROGNAME}.bin", encrypt);
else:
  print("You need to define output file via board_build.firmware = 'filename' parameter")
  exit(1);
