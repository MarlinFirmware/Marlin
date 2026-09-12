#
# sam3u_build.py
# Build support for the Atmel SAM3U (AT91SAM3U4E) / 4pi environment.
#
# The SAM3U has no Arduino core, so this environment builds bare-metal and
# PlatformIO's framework builders never run. That leaves three things for us
# to supply that a framework would normally have handled:
#
#   1. The CMSIS include paths - the ARM core headers (core_cm3.h) and Atmel's
#      SAM3U device headers (sam3u4e.h and its component/ and pio/ trees).
#   2. libc/libm/libgcc, because the bare builder links with -nostdlib.
#
# The startup code (vector table + Reset_Handler) is NOT taken from the
# package: its startup_sam3u.c does not compile against the package's own
# sam3u4e.h. Marlin/src/HAL/SAM3U/startup.c replaces it and is built with the
# rest of the HAL.
#
# The headers come out of the framework-cmsis and framework-cmsis-atmel
# packages, which ini/sam3u.ini pulls in via platform_packages.
#
import os
Import("env")

platform = env.PioPlatform()

cmsis_dir = platform.get_package_dir("framework-cmsis")
cmsis_atmel_dir = platform.get_package_dir("framework-cmsis-atmel")

if not cmsis_dir or not os.path.isdir(cmsis_dir):
    raise Exception("framework-cmsis is not installed. Check platform_packages in ini/sam3u.ini.")
if not cmsis_atmel_dir or not os.path.isdir(cmsis_atmel_dir):
    raise Exception("framework-cmsis-atmel is not installed. Check platform_packages in ini/sam3u.ini.")

sam3u_dir = os.path.join(cmsis_atmel_dir, "CMSIS", "Device", "ATMEL", "sam3u")
if not os.path.isdir(sam3u_dir):
    raise Exception("SAM3U device headers not found at %s" % sam3u_dir)

# The ARM core headers moved between CMSIS package versions
core_include = os.path.join(cmsis_dir, "CMSIS", "Core", "Include")
if not os.path.isdir(core_include):
    core_include = os.path.join(cmsis_dir, "CMSIS", "Include")

# Marlin's own SAM3U include dir carries the Arduino.h shim and the generated
# CMSIS instance/ headers that the framework package is missing. It has to be on
# the *global* CPPPATH, not just build_src_flags, because the CMSIS startup code
# below is compiled outside src/ and still includes sam3u4e.h.
hal_include = os.path.join(env.subst("$PROJECT_DIR"), "Marlin", "src", "HAL", "SAM3U", "include")

env.Append(
    CPPPATH=[
        core_include,
        os.path.join(sam3u_dir, "include"),
        os.path.join(cmsis_atmel_dir, "CMSIS", "Device", "ATMEL"),
        hal_include
    ]
)

#
# -nostdlib drops the default libraries, but we still want the C library (for
# memcpy, sprintf and friends), libm and libgcc. Use the nano/nosys specs so
# printf stays small and the syscall stubs resolve.
#
env.Append(
    LINKFLAGS=["--specs=nano.specs", "--specs=nosys.specs"],
    LIBS=["c", "m", "gcc", "nosys"]
)
