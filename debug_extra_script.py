Import("env")

env.AddPostAction(
    "$BUILD_DIR/firmware.hex",
    env.VerboseAction(" ".join([
        "sed", "-i.bak",
        "s/:10040000FFFFFFFFFFFFFFFFFFFFFFFFDEF9FFFF23/:10040000FFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFD/",
        "$BUILD_DIR/firmware.hex"
    ]), "Fixing $BUILD_DIR/firmware.hex secure flash flags"))
env.AddPreAction(
    "upload",
     env.VerboseAction(" ".join([
         "echo",
         "'h\\nloadfile $BUILD_DIR/firmware.hex\\nr\\nq\\n'",
         ">$BUILD_DIR/aux.jlink"
     ]), "Creating auxiliary files"))

env.Replace(
    UPLOADHEXCMD=
    'JLinkExe -device MK20DX256xxx7 -speed 4000 -if swd -autoconnect 1 -CommanderScript $BUILD_DIR/aux.jlink -SkipProgOnCRCMatch = 1 -VerifyDownload = 1'
)
