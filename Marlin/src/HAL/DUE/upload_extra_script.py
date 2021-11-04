#
# Set upload_command
#
#  Windows: bossac.exe
#  Other: leave unchanged
#
print("Executing upload_extra_script.py")

import pioutil
if not pioutil.is_vscode_init():
	import platform
	current_OS = platform.system()

	if current_OS == 'Windows':

		Import("env")

		# Use bossac.exe on Windows
		env.Replace(
			UPLOADCMD="bossac --info --unlock --write --verify --reset --erase -U false --boot $SOURCE"
		)
