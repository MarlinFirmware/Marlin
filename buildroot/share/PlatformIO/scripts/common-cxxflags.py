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

# Useful for JTAG debugging
#
# It will separe release and debug build folders.
# It useful when we need keep two live versions: one debug, for debugging,
# other release, for flashing.
# Without this, PIO will recompile everything twice for any small change.
#
if env.GetBuildType() == "debug":
	env['BUILD_DIR'] = '$PROJECT_BUILD_DIR/$PIOENV/debug'
