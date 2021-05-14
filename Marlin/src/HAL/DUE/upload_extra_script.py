#
# Set upload_command
#
#  Windows: bossac.exe
#  Other: leave unchanged
#

import platform
current_OS = platform.system()

if current_OS == 'Windows':

	Import("env")

	# Use bossac.exe on Windows
	env.Replace(
	    UPLOADCMD="bossac --info --unlock --write --verify --reset --erase -U false --boot $SOURCE"
	)
