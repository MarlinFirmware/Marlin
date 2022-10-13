#
# Set upload_command
#
#  Windows: bossac.exe
#  Other: leave unchanged
#
import pioutil
if pioutil.is_pio_build():
    import platform
    current_OS = platform.system()

    if current_OS == 'Windows':

        Import("env")

        # Use bossac.exe on Windows
        env.Replace(
            UPLOADCMD="bossac --info --unlock --write --verify --reset --erase -U false --boot $SOURCE"
        )
