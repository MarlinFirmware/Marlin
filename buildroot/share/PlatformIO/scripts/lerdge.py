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

# Encrypt ${PROGNAME}.bin and save it as build.firmware
def encrypt(source, target, env):
    print("Encrypting to:", board.get("build.firmware"))
    firmware = open(target[0].path, "rb")
    renamed = open(target[0].dir.path + "/" + board.get("build.firmware"), "wb")
    length = os.path.getsize(target[0].path)

    encrypt_file(firmware, renamed, length)

    firmware.close()
    renamed.close()

if 'firmware' in board.get("build").keys():
  marlin.add_post_action(encrypt);
else:
  print("You need to define output file via board_build.firmware = 'filename' parameter")
  exit(1);
