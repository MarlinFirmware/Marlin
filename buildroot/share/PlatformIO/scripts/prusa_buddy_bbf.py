#
# prusa_buddy_bbf.py
#
# Wrap the firmware binary in an unsigned Prusa BBF container so the
# Prusa Buddy bootloader can flash it from a USB drive. The bootloader
# only accepts unsigned firmware once the appendix has been broken.
#
# BBF version 1 layout, as produced by Prusa-Firmware-Buddy/utils/pack_fw.py:
#
#   ECDSA signature       64 bytes  (zeros when unsigned)
#   SHA-256               32 bytes  (over everything that follows)
#   Firmware length        4 bytes  (uint32, little-endian)
#   Version               10 bytes  (major, minor, patch, build uint16, prerelease[5])
#   Board major version    1 byte
#   Printer type           1 byte   (2 = MINI)
#   Printer version        1 byte
#   Printer subversion     1 byte
#   Reserved               1 byte
#   Padding              461 bytes  (header + hash total 512 bytes)
#   Firmware image
#
import pioutil
if pioutil.is_pio_build():
    import re
    from hashlib import sha256
    from pathlib import Path

    import marlin

    PRINTER_TYPE_MINI = 2
    PRINTER_VERSION = 1
    PRINTER_SUBVERSION = 0
    BOARD_MAJOR_VERSION = 0

    def marlin_version():
        pattern = re.compile(r'^\s*#define\s+SHORT_BUILD_VERSION\s+"([^"]*)"', re.M)
        for header in ("Marlin/Version.h", "Marlin/src/inc/Version.h"):
            text = Path(header).read_text(encoding="utf-8", errors="ignore")
            found = pattern.search(text)
            if found:
                numbers = re.search(r'(\d+)\.(\d+)(?:\.(\d+))?', found.group(1))
                if numbers:
                    return tuple(int(n or 0) for n in numbers.groups())
        return (0, 0, 0)

    def pack_bbf(source, target, env):
        binpath = Path(target[0].path)
        firmware = binpath.read_bytes()
        major, minor, patch = marlin_version()

        header = bytearray()
        header += len(firmware).to_bytes(4, "little")
        header += bytes((major & 0xFF, minor & 0xFF, patch & 0xFF))
        header += (0).to_bytes(2, "little")            # build number
        header += bytes(5)                              # prerelease
        header += bytes((BOARD_MAJOR_VERSION, PRINTER_TYPE_MINI, PRINTER_VERSION, PRINTER_SUBVERSION, 0))
        header += bytes(461)

        payload = bytes(header) + firmware
        bbfpath = binpath.with_suffix(".bbf")
        bbfpath.write_bytes(bytes(64) + sha256(payload).digest() + payload)
        print(f"FIRMWARE BBF: {bbfpath} (unsigned, {major}.{minor}.{patch})")

    marlin.add_post_action(pack_bbf)
