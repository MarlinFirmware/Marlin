#
# simulator.py
# PlatformIO pre: script for simulator builds
#
import pioutil
if pioutil.is_pio_build():
	# Get the environment thus far for the build
	Import("env")

	#print(env.Dump())

	#
	# Give the binary a distinctive name
	#

	env['PROGNAME'] = "MarlinSimulator"

	#
	# If Xcode is installed add the path to its Frameworks folder,
	# or if Mesa is installed try to use its GL/gl.h.
	#

	import sys
	if sys.platform == 'darwin':

		#
		# Silence half of the ranlib warnings. (No equivalent for 'ARFLAGS')
		#
		env['RANLIBFLAGS'] += [ "-no_warning_for_no_symbols" ]

		# Default paths for Xcode and a lucky GL/gl.h dropped by Mesa
		xcode_path = "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/Frameworks"
		mesa_path = "/opt/local/include/GL/gl.h"

		import os.path

		if os.path.exists(xcode_path):

			env['BUILD_FLAGS'] += [ "-F" + xcode_path ]
			print("Using OpenGL framework headers from Xcode.app")

		elif os.path.exists(mesa_path):

			env['BUILD_FLAGS'] += [ '-D__MESA__' ]
			print("Using OpenGL header from", mesa_path)

		else:

			print("\n\nNo OpenGL headers found. Install Xcode for matching headers, or use 'sudo port install mesa' to get a GL/gl.h.\n\n")

			# Break out of the PIO build immediately
			sys.exit(1)
