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

# to avoid ordering errors in linker
env.Prepend(_LIBFLAGS="-Wl,--start-group ")
env.Append(_LIBFLAGS=" -Wl,--end-group")
