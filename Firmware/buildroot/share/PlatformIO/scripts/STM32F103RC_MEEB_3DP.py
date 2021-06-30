#
# buildroot/share/PlatformIO/scripts/STM32F103RC_MEEB_3DP.py
#
try:
    import configparser
except ImportError:
    import ConfigParser as configparser

import os
Import("env", "projenv")
# access to global build environment
print(env)
# access to project build environment (is used source files in "src" folder)
print(projenv)

config = configparser.ConfigParser()
config.read("platformio.ini")

#com_port = config.get("env:STM32F103RC_meeb", "upload_port")
#print('Use the {0:s} to reboot the board to dfu mode.'.format(com_port))

#
# Upload actions
#

def before_upload(source, target, env):
    print("before_upload")
    # do some actions
    # use com_port
    #
    env.Execute("pwd")

def after_upload(source, target, env):
    print("after_upload")
    # do some actions
    #
    #
    env.Execute("pwd")

print("Current build targets", map(str, BUILD_TARGETS))

env.AddPreAction("upload", before_upload)
env.AddPostAction("upload", after_upload)

flash_size = 0
vect_tab_addr = 0

for define in env['CPPDEFINES']:
    if define[0] == "VECT_TAB_ADDR":
        vect_tab_addr = define[1]
    if define[0] == "STM32_FLASH_SIZE":
        flash_size = define[1]

print('Use the {0:s} address as the marlin app entry point.'.format(vect_tab_addr))
print('Use the {0:d}KB flash version of stm32f103rct6 chip.'.format(flash_size))

import marlin
marlin.custom_ld_script("STM32F103RC_MEEB_3DP.ld")
