from os.path import join
from os.path import expandvars
Import("env", "projenv")

# Relocate firmware from 0x08000000 to 0x08010000
# for define in env['CPPDEFINES']:
#    if define[0] == "VECT_TAB_ADDR":
#        env['CPPDEFINES'].remove(define)
#env['CPPDEFINES'].remove(("VECT_TAB_ADDR", 134217728))
#env['CPPDEFINES'].append(("VECT_TAB_ADDR", "0x08010000"))

# Custom HEX from ELF
env.AddPostAction(
	join("$BUILD_DIR","${PROGNAME}.elf"),
	env.VerboseAction(" ".join([
		"$OBJCOPY", "-O ihex", "$TARGET", # TARGET=.pio/build/fysetc_STM32F1/firmware.elf
		"\"" + join("$BUILD_DIR","${PROGNAME}.hex") + "\"", # Note: $BUILD_DIR is a full path
	]), "Building $TARGET"))

# In-line command with arguments
UPLOAD_TOOL="stm32flash"
platform = env.PioPlatform()
if platform.get_package_dir("tool-stm32duino") != None:
	UPLOAD_TOOL=expandvars("\"" + join(platform.get_package_dir("tool-stm32duino"),"stm32flash","stm32flash") + "\"")

env.Replace(
	UPLOADER=UPLOAD_TOOL,
	UPLOADCMD=expandvars(UPLOAD_TOOL + " -v -i rts,-dtr,dtr -R -b 115200 -g 0x8000000 -w \"" + join("$BUILD_DIR","${PROGNAME}.hex")+"\"" + " $UPLOAD_PORT")
)
