#
# list-firmware-files.py
# List the firmware file(s) at the end of the build, since some envs rename or encrypt them
#
import pioutil
if pioutil.is_pio_build():
    from pathlib import Path
    env = pioutil.env

    FIRMWARE_EXTS = ('.bin', '.hex', '.uf2', '.cur', '.cbd', '.bbf', '.srec')
    SKIP_NAMES = ('bootloader.bin', 'partitions.bin')

    def show_firmware(source, target, env):
        build_dir = Path(env.subst("$BUILD_DIR"))
        elfs = list(build_dir.glob('*.elf'))
        if not elfs: return
        # Files made from the latest .elf, skipping leftovers from older builds
        newest = max(f.stat().st_mtime for f in elfs) - 1
        files = sorted(f for f in build_dir.iterdir()
                       if f.suffix.lower() in FIRMWARE_EXTS and f.name not in SKIP_NAMES and f.stat().st_mtime >= newest)
        if not files: return
        project_dir = Path(env.subst("$PROJECT_DIR"))
        names = [ str(f.relative_to(project_dir)) for f in files ]
        if len(names) == 1:
            print("Firmware: " + names[0])
        else:
            print("Firmware files:")
            for n in names: print("  " + n)

    if env["PIOPLATFORM"] != "native":
        # Pass cmdstr=None so SCons doesn't echo the action's Python signature
        env.AddPostAction("buildprog", env.Action(show_firmware, None))
