#
# tronxy_cxy_446_v10.py
# Build customizations for env:TRONXY_CXY_446_V10
#
import pioutil
if pioutil.is_pio_build():

    import marlin, os

    from SCons.Script import DefaultEnvironment

    env = DefaultEnvironment()

    # Check whether the "update" folder exists
    outpath = "update"
    if not os.path.exists(outpath): os.makedirs(outpath)

    # Build "fmw_tronxy.hex" and place in "update" folder
    def output_target_hex():
        tar_hex = f"{outpath}/fmw_tronxy.hex"
        env.AddPostAction(
            "$BUILD_DIR/${PROGNAME}.elf",
            env.VerboseAction(" ".join([
                "$OBJCOPY", "-O", "ihex", "-R", ".eeprom",
                "$BUILD_DIR/${PROGNAME}.elf", tar_hex
            ]), "Building %s" % tar_hex)
        )

    # Build "fmw_tronxy.bin" and place in "update" folder
    def output_target_bin():
        tar_bin = f"{outpath}/fmw_tronxy.bin"
        env.AddPostAction(
            "$BUILD_DIR/${PROGNAME}.elf",
            env.VerboseAction(" ".join([
                "$OBJCOPY", "-O", "binary", "-R", ".eeprom",
                "$BUILD_DIR/${PROGNAME}.elf", tar_bin
            ]), "Building %s" % tar_bin)
        )

    output_target_hex()
    output_target_bin()
