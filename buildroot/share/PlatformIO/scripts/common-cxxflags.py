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

env_name = str(env["PIOENV"])
env.Replace(PROGNAME="%s_DW7" % (env_name))
print("Environment: %s" % (env_name))
