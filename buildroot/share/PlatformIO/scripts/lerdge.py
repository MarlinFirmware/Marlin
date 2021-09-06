#
# buildroot/share/PlatformIO/scripts/lerdge.py
# Customizations for Lerdge build environments:
#   env:LERDGEX  env:LERDGEX_usb_flash_drive
#   env:LERDGES  env:LERDGES_usb_flash_drive
#   env:LERDGEK  env:LERDGEK_usb_flash_drive
#
import os,marlin
Import("env")

from SCons.Script import DefaultEnvironment
board = DefaultEnvironment().BoardConfig()

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

# Encrypt ${PROGNAME}.bin and save it with the name given in build.encrypt
def encrypt(source, target, env):
    fwname = board.get("build.encrypt")
    print("Encrypting %s to %s" % (target[0].path, fwname))
    firmware = open(target[0].path, "rb")
    renamed = open(target[0].dir.path + "/" + fwname, "wb")
    length = os.path.getsize(target[0].path)

    encrypt_file(firmware, renamed, length)

    firmware.close()
    renamed.close()

if 'encrypt' in board.get("build").keys():
    if board.get("build.encrypt") != "":
        marlin.add_post_action(encrypt)
else:
    print("LERDGE builds require output file via board_build.encrypt = 'filename' parameter")
    exit(1)
