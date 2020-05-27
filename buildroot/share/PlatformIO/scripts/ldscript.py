#import os
Import("env")

LD_FLASH_OFFSET=""

for define in env['CPPDEFINES']:
    if define[0] == "VECT_TAB_OFFSET":
        LD_FLASH_OFFSET=define[1]
#        print(define)

from SCons.Script import DefaultEnvironment
board = DefaultEnvironment().BoardConfig()
maximum_ram_size = board.get("upload.maximum_ram_size")
#print(maximum_ram_size)

if LD_FLASH_OFFSET:
    for i, flag in enumerate(env["LINKFLAGS"]):
        if "-Wl,--defsym=LD_FLASH_OFFSET" in flag:
            env["LINKFLAGS"][i] = "-Wl,--defsym=LD_FLASH_OFFSET=" + LD_FLASH_OFFSET
#            print(env["LINKFLAGS"][i])
        if "-Wl,--defsym=LD_MAX_DATA_SIZE" in flag:
            env["LINKFLAGS"][i] = "-Wl,--defsym=LD_MAX_DATA_SIZE=" + str(maximum_ram_size - 40)
#            print(env["LINKFLAGS"][i])

#print("CPPDEFINES")
#for define in env["CPPDEFINES"]:
#    print(define)

#print("LINKFLAGS")
#for flag in env["LINKFLAGS"]:
#    print(flag)

#exit(0)
