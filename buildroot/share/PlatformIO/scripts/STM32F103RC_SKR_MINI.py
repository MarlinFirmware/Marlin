Import("env")

# Relocate firmware from 0x08000000 to 0x08007000
for define in env['CPPDEFINES']:
    if define[0] == "VECT_TAB_ADDR":
        env['CPPDEFINES'].remove(define)
env['CPPDEFINES'].append(("VECT_TAB_ADDR", "0x08007000"))
env.Replace(LDSCRIPT_PATH="buildroot/share/PlatformIO/ldscripts/STM32F103RC_SKR_MINI.ld")
