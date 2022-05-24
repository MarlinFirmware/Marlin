#
# Convert the ELF to an SREC file suitable for some bootloaders
#
import pioutil
if pioutil.is_pio_build():
	import os,sys
	from os.path import join

	Import("env")

	board = env.BoardConfig()
	board_keys = board.get("build").keys()
	if 'encrypt' in board_keys:
		env.AddPostAction(
			join("$BUILD_DIR", "${PROGNAME}.bin"),
			env.VerboseAction(" ".join([
				"$OBJCOPY", "-O", "srec",
				"\"$BUILD_DIR/${PROGNAME}.elf\"", "\"" + join("$BUILD_DIR", board.get("build.encrypt")) + "\""
			]), "Building $TARGET")
		)
