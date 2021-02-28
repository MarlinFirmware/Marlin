#
# SAMD51_grandcentral_m4.py
# Customizations for env:SAMD51_grandcentral_m4
#
from os.path import join, isfile
import shutil
from pprint import pprint

Import("env")

rxBuf = env["MARLIN_FEATURES"]["RX_BUFFER_SIZE"] if "RX_BUFFER_SIZE" in env["MARLIN_FEATURES"] else "0"
txBuf = env["MARLIN_FEATURES"]["TX_BUFFER_SIZE"] if "TX_BUFFER_SIZE" in env["MARLIN_FEATURES"] else "0"

serialBuf = str(max(int(rxBuf), int(txBuf), 350))

build_flags = env.get('BUILD_FLAGS')
build_flags.append("-DSERIAL_BUFFER_SIZE=" + serialBuf)
env.Replace(BUILD_FLAGS=build_flags)
