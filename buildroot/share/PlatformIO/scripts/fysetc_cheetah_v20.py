#
# STM32F401VE_STEVAL.py
# Customizations for env:STM32F401VE_STEVAL
#
import os
Import("env")

custom_ld_script = os.path.abspath("buildroot/share/PlatformIO/variants/FYSETC_CHEETAH_V20/ldscript.ld")
for i, flag in enumerate(env["LINKFLAGS"]):
    if "-Wl,-T" in flag:
        env["LINKFLAGS"][i] = "-Wl,-T" + custom_ld_script
    elif flag == "-T":
        env["LINKFLAGS"][i + 1] = custom_ld_script
