"""
lerdge.py

Customizations for Lerdge build environments:
  env:LERDGEX  env:LERDGEX_usb_flash_drive
  env:LERDGES  env:LERDGES_usb_flash_drive
  env:LERDGEK  env:LERDGEK_usb_flash_drive
"""

import pioutil
if pioutil.is_pio_build():
    import os, marlin

    board = marlin.env.BoardConfig()

    def encryptByte(byte):
        byte = 0xFF & ((byte << 6) | (byte >> 2))
        i = 0x58 + byte
        j = 0x05 + byte + (i >> 8)
        byte = (0xF8 & i) | (0x07 & j)
        return byte

    def encrypt_file(input_file, output_file, file_length):
        input = bytearray(input_file.read())
        for i, byte in enumerate(input):
            input[i] = encryptByte(byte)
        output_file.write(input)

    def encrypt(source, target, env):
        """
        Encrypt ${PROGNAME}.bin and save it with the name given in build.crypt_lerdge
        """
        fwpath = target[0].path
        enname = board.get("build.crypt_lerdge")
        print("Encrypting %s to %s" % (fwpath, enname))
        fwfile = open(fwpath, "rb")
        enfile = open(target[0].dir.path + "/" + enname, "wb")
        length = os.path.getsize(fwpath)

        encrypt_file(fwfile, enfile, length)

        fwfile.close()
        enfile.close()
        os.remove(fwpath)

    if 'crypt_lerdge' in board.get("build").keys():
        if board.get("build.crypt_lerdge") != "":
            marlin.add_post_action(encrypt)
    else:
        print("LERDGE builds require output file via board_build.crypt_lerdge = 'filename' parameter")
        exit(1)
