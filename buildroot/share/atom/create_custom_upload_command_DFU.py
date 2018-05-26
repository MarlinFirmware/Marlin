#
#  Builds custom upload command
#    1) Run platformio as a subprocess to find a COM port
#    2) Build the upload command
#    3) Exit and let upload tool do the work
#
#  This script runs between completion of the library/dependencies installation and compilation.
#
#  Will continue on if a COM port isn't found so that the compilation can be done.
#

import sys
from SCons.Script import DefaultEnvironment

import platform
current_OS = platform.system()

env = DefaultEnvironment()

if current_OS == 'Windows':
    avrdude_conf_path =  env.get("PIOHOME_DIR") + '\\packages\\toolchain-atmelavr\\etc\\avrdude.conf'

    source_path = env.get("PROJECTBUILD_DIR") + '\\' + env.get("PIOENV") + '\\firmware.hex'

    upload_string = 'avrdude -p usb1286 -c flip1 -C ' + avrdude_conf_path + ' -U flash:w:' + source_path + ':i'

else:
    source_path = env.get("PROJECTBUILD_DIR") + '/' + env.get("PIOENV") + '/firmware.hex'

    upload_string = 'avrdude -p usb1286 -c flip1 -U flash:w:' + source_path + ':i'


env.Replace(
     UPLOADCMD = upload_string,
     MAXIMUM_RAM_SIZE = 8192,
     MAXIMUM_SIZE = 130048
)
