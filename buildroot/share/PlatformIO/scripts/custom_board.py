#
# buildroot/share/PlatformIO/scripts/custom_board.py
#
# - For build.address replace VECT_TAB_ADDR to relocate the firmware
# - For build.ldscript use one of the linker scripts in buildroot/share/PlatformIO/ldscripts
# - For build.rename simply rename the firmware file.
#
import marlin
board = marlin.env.BoardConfig()

address = board.get("build.address", "")
if address:
	marlin.relocate_firmware(address)

ldscript = board.get("build.ldscript", "")
if ldscript:
	marlin.custom_ld_script(ldscript)

rename_to = board.get("build.rename", "")
if rename_to:
	def rename_target(source, target, env):
		import os, shutil
		firmware = os.path.join(target[0].dir.path, rename_to)
		shutil.copy(target[0].path, firmware)
	marlin.add_post_action(rename_target)

