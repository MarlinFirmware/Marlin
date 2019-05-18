Import("env")

# Relocate firmware from 0x08000000 to 0x08007000
env['CPPDEFINES'].remove(("VECT_TAB_ADDR", 134217728))
env['CPPDEFINES'].append(("VECT_TAB_ADDR", "0x08007000"))
env.Replace(LDSCRIPT_PATH="buildroot/share/PlatformIO/ldscripts/STM32F1.ld")


