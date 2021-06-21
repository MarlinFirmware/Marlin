#
# buildroot/share/PlatformIO/scripts/custom_board.py
#
import marlin
board = marlin.env.BoardConfig()

address = board.get("build.address", "")
if address:
	marlin.relocate_firmware(address)

ldscript = board.get("build.ldscript", "")
if ldscript:
	marlin.custom_ld_script(ldscript)
