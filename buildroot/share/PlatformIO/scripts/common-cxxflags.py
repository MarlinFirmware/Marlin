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
# It will separe release and debug build folders.
# It useful when we need keep two live versions: one debug, for debugging,
# other release, for flashing.
# Without this, PIO will recompile everything twice for any small change.
#
if env.GetBuildType() == "debug":
	env['BUILD_DIR'] = '$PROJECT_BUILD_DIR/$PIOENV/debug'

# On some platform, F_CPU is a runtime variable. Since it's used to convert from ns
# to CPU cycles, this adds overhead preventing small delay (in the order of less than
# 30 cycles) to be generated correctly. By using a compile time constant instead
# the compiler will perform the computation and this overhead will be avoided
add_cpu_freq()
