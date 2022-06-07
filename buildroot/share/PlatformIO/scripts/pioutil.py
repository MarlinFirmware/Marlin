#
# pioutil.py
#

# Make sure 'vscode init' is not the current command
def is_pio_build():
	from SCons.Script import COMMAND_LINE_TARGETS
	return "idedata" not in COMMAND_LINE_TARGETS and "_idedata" not in COMMAND_LINE_TARGETS
