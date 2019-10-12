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
