import os
Import("env")

for define in env['CPPDEFINES']:
    if define[0] == "VECT_TAB_ADDR":
        env['CPPDEFINES'].remove(define)

# Relocate firmware from 0x08000000 to 0x08005000
env['CPPDEFINES'].append(("VECT_TAB_ADDR", "0x08005000"))

custom_ld_script = os.path.abspath("buildroot/share/PlatformIO/ldscripts/ZONESTAR_ZM3E2.ld")
for i, flag in enumerate(env["LINKFLAGS"]):
    if "-Wl,-T" in flag:
        env["LINKFLAGS"][i] = "-Wl,-T" + custom_ld_script
    elif flag == "-T":
        env["LINKFLAGS"][i + 1] = custom_ld_script
