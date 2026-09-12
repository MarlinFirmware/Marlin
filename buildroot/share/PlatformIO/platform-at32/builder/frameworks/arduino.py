"""
AT32F4 framework build script entry point.
Delegates to the build script bundled inside the framework package,
so that SCons can resolve it via an absolute installed path.
(Same pattern as HC32 platform.)
"""
import sys
from os.path import join, isfile

from SCons.Script import DefaultEnvironment, SConscript

env = DefaultEnvironment()
build_script = join(
    env.PioPlatform().get_package_dir("framework-arduino-at32f4"),
    "tools", "platformio", "platformio-build-arduino.py"
)

if not isfile(build_script):
    sys.stderr.write(f"Error: Missing AT32 PlatformIO build script: {build_script}\n")
    env.Exit(1)

SConscript(build_script)
