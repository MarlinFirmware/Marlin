import os, marlin
Import("env")

# Relocate firmware from 0x08000000 to 0x08005000
marlin.relocate_firmware("0x08005000")

custom_ld_script = os.path.abspath("buildroot/share/PlatformIO/ldscripts/fly_mini.ld")
for i, flag in enumerate(env["LINKFLAGS"]):
    if "-Wl,-T" in flag:
        env["LINKFLAGS"][i] = "-Wl,-T" + custom_ld_script
    elif flag == "-T":
        env["LINKFLAGS"][i + 1] = custom_ld_script

