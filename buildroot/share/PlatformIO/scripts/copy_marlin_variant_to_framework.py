#
# copy_marlin_variant_to_framework.py
#
import os,shutil
from SCons.Script import DefaultEnvironment
from platformio import util
from platformio.package.meta import PackageSpec

def copytree(src, dst, symlinks=False, ignore=None):
    for item in os.listdir(src):
        s = os.path.join(src, item)
        d = os.path.join(dst, item)
        if os.path.isdir(s):
            shutil.copytree(s, d, symlinks, ignore)
        else:
            shutil.copy2(s, d)

env = DefaultEnvironment()
platform = env.PioPlatform()
board = env.BoardConfig()
variant = board.get("build.variant")

platform_packages = env.GetProjectOption('platform_packages')
# if there's no framework defined, take it from the class name of platform
framewords = {
    "Ststm32Platform": "framework-arduinoststm32",
    "AtmelavrPlatform": "framework-arduino-avr"
}
if len(platform_packages) == 0:
    platform_name = framewords[platform.__class__.__name__]
else:
    platform_name = PackageSpec(platform_packages[0]).name

FRAMEWORK_DIR = platform.get_package_dir(platform_name)
assert os.path.isdir(FRAMEWORK_DIR)
assert os.path.isdir("buildroot/share/PlatformIO/variants")

variant_dir = os.path.join(FRAMEWORK_DIR, "variants", variant)

source_dir = os.path.join("buildroot/share/PlatformIO/variants", variant)
assert os.path.isdir(source_dir)

if os.path.isdir(variant_dir):
    shutil.rmtree(variant_dir)

if not os.path.isdir(variant_dir):
    os.mkdir(variant_dir)

copytree(source_dir, variant_dir)
