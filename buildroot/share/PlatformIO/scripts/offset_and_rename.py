#
# offset_and_rename.py
#
# - If 'board_build.offset' is provided, either by JSON or by the environment...
#   - Set linker flag LD_FLASH_OFFSET and relocate the VTAB based on 'build.offset'.
#   - Set linker flag LD_MAX_DATA_SIZE based on 'build.maximum_ram_size'.
#   - Define STM32_FLASH_SIZE from 'upload.maximum_size' for use by Flash-based EEPROM emulation.
#
# - For 'board_build.rename' add a post-action to rename the firmware file.
#
import pioutil
if pioutil.is_pio_build():
    import marlin

    env = marlin.env
    board = env.BoardConfig()
    board_keys = board.get("build").keys()

    #
    # For build.offset define LD_FLASH_OFFSET, used by ldscript.ld
    #
    if 'offset' in board_keys:
        LD_FLASH_OFFSET = board.get("build.offset")
        marlin.relocate_vtab(LD_FLASH_OFFSET)

        # Chip total flash (bytes) from board JSON
        _max_flash_bytes = int(board.get("upload.maximum_size"))

        # Keep STM32_FLASH_SIZE as the chip total (KiB)
        maximum_flash_size = _max_flash_bytes // 1024
        marlin.replace_define('STM32_FLASH_SIZE', maximum_flash_size)

        # Also compute available flash after bootloader for Project Inspect
        try:
            _offset_int = int(str(LD_FLASH_OFFSET), 0)
        except Exception:
            _offset_int = 0

        if _max_flash_bytes and _offset_int:
            _avail = _max_flash_bytes - _offset_int
            if _avail > 0:
                # Update in-memory manifest so Advanced Memory Usage shows the correct total
                try:
                    board.manifest.setdefault("upload", {})
                    board.manifest["upload"]["maximum_size"] = _avail
                except Exception:
                    pass

        # Get upload.maximum_ram_size (defined by /buildroot/share/PlatformIO/boards/VARIOUS.json)
        maximum_ram_size = board.get("upload.maximum_ram_size")

        # Helper: replace existing -Wl,--defsym=NAME=... or append if missing
        def _upsert_defsym(name, value):
            key = "-Wl,--defsym=" + name
            if isinstance(value, int):
                val = key + "=" + (hex(value) if value >= 10 else str(value))
            else:
                val = key + "=" + str(value)
            found = False
            for i, flag in enumerate(env["LINKFLAGS"]):
                if key in flag:
                    env["LINKFLAGS"][i] = val
                    found = True
                    break
            if not found:
                env.Append(LINKFLAGS=[val])

        # Provide the symbols the linker script expects:
        #   ORIGIN = 0x08000000 + LD_FLASH_OFFSET
        #   LENGTH = LD_MAX_SIZE - LD_FLASH_OFFSET
        _upsert_defsym("LD_FLASH_OFFSET", _offset_int)
        _upsert_defsym("LD_MAX_SIZE", _max_flash_bytes)

        if maximum_ram_size:
            _upsert_defsym("LD_MAX_DATA_SIZE", int(maximum_ram_size) - 40)

    #
    # For build.encrypt_mks rename and encode the firmware file.
    #
    if 'encrypt_mks' in board_keys:

        # Encrypt ${PROGNAME}.bin and save it with the name given in build.encrypt_mks
        def encrypt(source, target, env):
            marlin.encrypt_mks(source, target, env, board.get("build.encrypt_mks"))

        if board.get("build.encrypt_mks") != "":
            marlin.add_post_action(encrypt)

    #
    # For build.rename simply rename the firmware file.
    #
    if 'rename' in board_keys:

        # If FIRMWARE_BIN is defined by config, override all
        mf = env["MARLIN_FEATURES"]
        if "FIRMWARE_BIN" in mf: new_name = mf["FIRMWARE_BIN"]
        else: new_name = board.get("build.rename")

        def rename_target(source, target, env):
            from pathlib import Path
            from datetime import datetime
            from os import path

            # Build the timestamped base name from your template (may already include ".bin")
            base = datetime.now().strftime(new_name.replace('{date}', '%Y%m%d').replace('{time}', '%H%M%S'))

            # Ensure correct extensions for both outputs
            if base.lower().endswith('.bin'):
                stem = base[:-4]            # strip ".bin" for the ELF stem
                bin_name = base
            else:
                stem = base
                bin_name = base + '.bin'

            elf_name = stem + '.elf'

            # Current files produced by PlatformIO
            bin_old = Path(target[0].path)           # e.g. .pio/build/<env>/firmware.bin
            elf_old = Path(source[0].path)           # e.g. .pio/build/<env>/firmware.elf

            # New paths in the same directories
            bin_new = Path(target[0].dir.path, bin_name)
            elf_new = Path(source[0].dir.path, elf_name)

            # Rename both
            bin_old.replace(bin_new)
            elf_old.replace(elf_new)

            # Update PROGNAME (base without extension) for any later steps that read it
            env['PROGNAME'] = path.splitext(str(bin_new))[0]

            # Optional: log the results
            print(f"FIRMWARE BIN: {bin_new}")
            print(f"FIRMWARE ELF: {elf_new}")

        marlin.add_post_action(rename_target)
