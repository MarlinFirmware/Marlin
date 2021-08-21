#
# common-dependencies-post.py
# Convenience script to add build flags for Marlin Enabled Features
#

Import("env")
Import("projenv")

def apply_board_build_flags():
	if not 'BOARD_CUSTOM_BUILD_FLAGS' in env['MARLIN_FEATURES']:
		return
	projenv.Append(CCFLAGS=env['MARLIN_FEATURES']['BOARD_CUSTOM_BUILD_FLAGS'].split())

# We need to add the board build flags in a post script
# so the platform build script doesn't overwrite the custom CCFLAGS
apply_board_build_flags()
