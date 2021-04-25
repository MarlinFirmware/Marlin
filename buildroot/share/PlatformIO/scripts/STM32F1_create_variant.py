#
# STM32F1_create_variant.py
#
import os,shutil
from SCons.Script import DefaultEnvironment
from platformio import util

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

FRAMEWORK_DIR = platform.get_package_dir("framework-arduinoststm32-maple")
assert os.path.isdir(FRAMEWORK_DIR)

source_root = os.path.join("buildroot", "share", "PlatformIO", "variants")
assert os.path.isdir(source_root)

variant = board.get("build.variant")
variant_dir = os.path.join(FRAMEWORK_DIR, "STM32F1", "variants", variant)

source_dir = os.path.join(source_root, variant)
assert os.path.isdir(source_dir)

if os.path.isdir(variant_dir):
    shutil.rmtree(variant_dir)

if not os.path.isdir(variant_dir):
    os.mkdir(variant_dir)

copytree(source_dir, variant_dir)
