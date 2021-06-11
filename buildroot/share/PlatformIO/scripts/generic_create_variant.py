#
# generic_create_variant.py
#
# Copy one of the variants from buildroot/platformio/variants into
# the appropriate framework variants folder, so that its contents
# will be picked up by PlatformIO just like any other variant.
#
import os,shutil,marlin
from SCons.Script import DefaultEnvironment
from platformio import util

env = DefaultEnvironment()

#
# Get the platform name from the 'platform_packages' option,
# or look it up by the platform.class.name.
#
platform = env.PioPlatform()

from platformio.package.meta import PackageSpec
platform_packages = env.GetProjectOption('platform_packages')
if len(platform_packages) == 0:
    framewords = {
        "Ststm32Platform": "framework-arduinoststm32",
        "AtmelavrPlatform": "framework-arduino-avr"
    }
    platform_name = framewords[platform.__class__.__name__]
else:
    platform_name = PackageSpec(platform_packages[0]).name

if platform_name in [ "usb-host-msc", "usb-host-msc-cdc-msc", "usb-host-msc-cdc-msc-2", "tool-stm32duino" ]:
    platform_name = "framework-arduinoststm32"

FRAMEWORK_DIR = platform.get_package_dir(platform_name)
assert os.path.isdir(FRAMEWORK_DIR)

board = env.BoardConfig()

#mcu_type = board.get("build.mcu")[:-2]
variant = board.get("build.variant")
#series = mcu_type[:7].upper() + "xx"

# Prepare a new empty folder at the destination
variant_dir = os.path.join(FRAMEWORK_DIR, "variants", variant)
if os.path.isdir(variant_dir):
    shutil.rmtree(variant_dir)
if not os.path.isdir(variant_dir):
    os.mkdir(variant_dir)

# Source dir is a local variant sub-folder
source_dir = os.path.join("buildroot/share/PlatformIO/variants", variant)
assert os.path.isdir(source_dir)

marlin.copytree(source_dir, variant_dir)
