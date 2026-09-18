"""
ArduinoCore-AT32F4 PlatformIO build script.

This script lives inside the framework package so that SCons can find it
via an absolute path (the package installs to ~/.platformio/packages/).

Called by the AT32 platform's builder/frameworks/arduino.py.
"""
import sys
from os.path import isdir, isfile, join

from SCons.Script import DefaultEnvironment

env = DefaultEnvironment()
platform = env.PioPlatform()
board = env.BoardConfig()

FRAMEWORK_DIR = platform.get_package_dir("framework-arduino-at32f4")
CORE_DIR     = join(FRAMEWORK_DIR, "cores", "arduino")
LIBCORE_DIR  = join(CORE_DIR, "libcore")
VARIANT      = board.get("build.variant", "AT32F403ACGU7")
VARIANT_DIR  = join(FRAMEWORK_DIR, "variants", VARIANT)
SYSTEM_DIR   = join(FRAMEWORK_DIR, "system")

if not isdir(CORE_DIR):
    sys.stderr.write(f"Error: Arduino core dir not found: {CORE_DIR}\n")
    env.Exit(1)

if not isdir(VARIANT_DIR):
    sys.stderr.write(f"Error: Variant dir not found: {VARIANT_DIR}\n")
    env.Exit(1)

# Optional app_config.h force-include (same convention as HC32 platform)
app_config = board.get("build.app_config", "")
app_config_flag = f"-include{app_config}" if app_config and isfile(app_config) else ""

# Linker script from variant
ldscript = board.get("build.ldscript", "AT32F403AxG_FLASH.ld")
ldscript_path = join(VARIANT_DIR, "linker_scripts", ldscript)

env.Append(
    CFLAGS  = (["-Os", "-DNDEBUG"] + ([app_config_flag] if app_config_flag else [])),
    CXXFLAGS= (["-Os", "-DNDEBUG"] + ([app_config_flag] if app_config_flag else [])),
    # Hard-float ABI must reach the LINK step too, otherwise the linker
    # driver defaults to soft-float and rejects the hard-float objects.
    LINKFLAGS=[
        "-mcpu=cortex-m4",
        "-mthumb",
        "-mfloat-abi=hard",
        "-mfpu=fpv4-sp-d16",
    ],
    CPPDEFINES=[
        ("ARDUINO", 100),
        "ARDUINO_ARCH_AT32",
        "ARDUINO_ARCH_AT32F4",
        f"ARDUINO_{board.get('build.board', 'AT32F403ACGxx')}",
        "USE_STDPERIPH_DRIVER",
        ("F_CPU", board.get("build.f_cpu", "240000000L")),
    ],
    CPPPATH=[
        CORE_DIR,
        LIBCORE_DIR,
        join(LIBCORE_DIR, "config"),
        VARIANT_DIR,
        join(SYSTEM_DIR, "Drivers", "CMSIS", "Include"),
        join(SYSTEM_DIR, "Drivers", "Firmware", "inc"),
        SYSTEM_DIR,
    ],
    LIBSOURCE_DIRS=[
        join(FRAMEWORK_DIR, "libraries"),
    ],
    LIBPATH=[
        VARIANT_DIR,
        SYSTEM_DIR,
    ],
)

if isfile(ldscript_path):
    env.Replace(LDSCRIPT_PATH=ldscript_path)

# Build core, variant and libcore (driver) sources.
# NOTE: The driver library (gpio/timer/adc/uart/etc.) lives under
# cores/arduino/libcore/ and is compiled automatically because
# BuildSources(CORE_DIR) recurses into that subdirectory. The prebuilt
# FrameLib.a shipped with the package is an empty stub, so it must NOT
# be linked (doing so fails with "cannot find -lFrameLib").
env.BuildSources(join("$BUILD_DIR", "FrameworkArduino", "CORE"), CORE_DIR)
env.BuildSources(join("$BUILD_DIR", "FrameworkArduino", "VARIANT"), VARIANT_DIR)

# The AT32 Standard Peripheral Library (crm/gpio/tmr/spi/dma/...) provides the
# real implementations of the peripheral driver functions. These live in
# system/Drivers/Firmware/src/ and are NOT pulled in by the Arduino core, so
# they must be compiled explicitly or every crm_/gpio_/tmr_/spi_ call is an
# undefined reference at link time.
FIRMWARE_SRC_DIR = join(SYSTEM_DIR, "Drivers", "Firmware", "src")
if isdir(FIRMWARE_SRC_DIR):
    env.BuildSources(join("$BUILD_DIR", "FrameworkArduino", "FIRMWARE"), FIRMWARE_SRC_DIR)
