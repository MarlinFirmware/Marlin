#
# buildroot/share/PlatformIO/scripts/pioutil.py
#

# Detect that 'vscode init' is running
def is_vscode_init():
	from SCons.Script import COMMAND_LINE_TARGETS
	return "idedata" in COMMAND_LINE_TARGETS or "_idedata" in COMMAND_LINE_TARGETS
