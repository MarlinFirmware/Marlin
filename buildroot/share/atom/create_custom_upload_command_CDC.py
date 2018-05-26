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

import subprocess


import platform
current_OS = platform.system()

from SCons.Script import DefaultEnvironment

env = DefaultEnvironment()

com_first = ''
com_last = ''
com_CDC = ''
description_first = ''
description_last = ''
description_CDC = ''

#
# grab the first com port that pops up unless we find one we know for sure
# is a CDC device
#
def get_com_port(com_search_text, descr_search_text, start):

    global com_first
    global com_last
    global com_CDC
    global description_first
    global description_last
    global description_CDC


    print '\nLooking for Serial Port\n'

  # stream output from subprocess and split it into lines
    pio_subprocess = subprocess.Popen(['platformio', 'device', 'list'], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)

    looking_for_description = False
    for line in iter(pio_subprocess.stdout.readline, ''):
        if 0 <= line.find(com_search_text):
          looking_for_description = True
          com_last = line.replace('\n', '')
          if com_first == '':
            com_first = com_last
        if 0 <= line.find(descr_search_text) and looking_for_description:
          looking_for_description = False
          description_last = line[ start : ]
          if description_first == '':
            description_first = description_last
          if 0 <= description_last.find('CDC'):
            com_CDC = com_last
            description_CDC = description_last

    if  com_CDC == '' and not(com_first == ''):
        com_CDC = com_first
        description_CDC = description_first
    elif com_CDC == '':
          com_CDC = 'COM_PORT_NOT_FOUND'

    if com_CDC == 'COM_PORT_NOT_FOUND':
        print com_CDC, '\n'
    else:
        print 'FOUND: ' ,com_CDC
        print 'DESCRIPTION: ',  description_CDC , '\n'

if current_OS == 'Windows':

    get_com_port('COM', 'Hardware ID:', 13)

    avrdude_conf_path =  env.get("PIOHOME_DIR") + '\\packages\\toolchain-atmelavr\\etc\\avrdude.conf'

    source_path = env.get("PROJECTBUILD_DIR") + '\\' + env.get("PIOENV") + '\\firmware.hex'

    upload_string = 'avrdude -p usb1286 -c avr109 -P ' + com_CDC + ' -C ' + avrdude_conf_path + ' -U flash:w:' + source_path + ':i'


if current_OS == 'Darwin':  # MAC

    get_com_port('usbmodem', 'Description:', 13)

    avrdude_conf_path =  env.get("PIOHOME_DIR") + '/packages/toolchain-atmelavr/etc/avrdude.conf'

    source_path = env.get("PROJECTBUILD_DIR") + '/' + env.get("PIOENV") + '/firmware.hex'

    upload_string = 'avrdude -p usb1286 -c avr109 -P ' + com_CDC + ' -U flash:w:' + source_path + ':i'


if current_OS == 'Linux':

    get_com_port('/dev/tty', 'Description:', 13)

    avrdude_conf_path =  env.get("PIOHOME_DIR") + '/packages/toolchain-atmelavr/etc/avrdude.conf'

    source_path = env.get("PROJECTBUILD_DIR") + '/' + env.get("PIOENV") + '/firmware.hex'

    upload_string = 'avrdude -p usb1286 -c avr109 -P ' + com_CDC + ' -U flash:w:' + source_path + ':i'


env.Replace(
     UPLOADCMD = upload_string,
     MAXIMUM_RAM_SIZE = 8192,
     MAXIMUM_SIZE = 130048
)
