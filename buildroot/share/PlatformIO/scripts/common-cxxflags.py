#
# common-cxxflags.py
# Convenience script to apply customizations to CPP flags
#
Import("env")
env.Append(CXXFLAGS=[
  "-Wno-register"
  #"-Wno-incompatible-pointer-types",
  #"-Wno-unused-const-variable",
  #"-Wno-maybe-uninitialized",
  #"-Wno-sign-compare"
])

#
# Add CPU frequency as a compile time constant instead of a runtime variable
#
def add_cpu_freq():
	if 'BOARD_F_CPU' in env:
		env['BUILD_FLAGS'].append('-DBOARD_F_CPU=' + env['BOARD_F_CPU'])

# Useful for JTAG debugging
#
# It will separate release and debug build folders.
# It useful to keep two live versions: a debug version for debugging and another for
# release, for flashing when upload is not done automatically by jlink/stlink.
# Without this, PIO needs to recompile everything twice for any small change.
if env.GetBuildType() == "debug" and env.get('UPLOAD_PROTOCOL') not in ['jlink', 'stlink']:
	env['BUILD_DIR'] = '$PROJECT_BUILD_DIR/$PIOENV/debug'

# On some platform, F_CPU is a runtime variable. Since it's used to convert from ns
# to CPU cycles, this adds overhead preventing small delay (in the order of less than
# 30 cycles) to be generated correctly. By using a compile time constant instead
# the compiler will perform the computation and this overhead will be avoided
add_cpu_freq()
