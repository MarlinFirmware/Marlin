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
variant = board.get("build.variant")
variant_dir = ' +<buildroot/share/PlatformIO/variants/' + variant + '>';
src_filter = env.get("SRC_FILTER")
print("Starting SRC Filter:", env.get("SRC_FILTER"))
src_filter_value = src_filter[0];

src_filter_value = src_filter_value + variant_dir
src_filter[0] = src_filter_value;
env["SRC_FILTER"] = src_filter

print("Modified SRC Filter:", env.get("SRC_FILTER"))

cxx_flags = env['CXXFLAGS']
print("CXXFLAGS", cxx_flags)

FRAMEWORK_DIR = platform.get_package_dir("framework-arduinoststm32")
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
