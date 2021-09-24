#
# buildroot/share/PlatformIO/scripts/STM32F103VE_longer.py
# Customizations for env:STM32F103VE_longer
#
import os,marlin

# Rename ${PROGNAME}.bin and save it as 'project.bin' (No encryption on the Longer3D)
def encrypt(source, target, env):
    firmware = open(target[0].path, "rb")
    renamed = open(target[0].dir.path + '/project.bin', "wb")
    length = os.path.getsize(target[0].path)
    position = 0
    try:
        while position < length:
            byte = firmware.read(1)
            renamed.write(byte)
            position += 1
    finally:
        firmware.close()
        renamed.close()

marlin.add_post_action(encrypt);
