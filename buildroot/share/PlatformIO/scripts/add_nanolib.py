#
# add_nanolib.py
#
from SCons.Script import DefaultEnvironment
env = DefaultEnvironment()
Import("env")

env.Append(LINKFLAGS=["--specs=nano.specs"])
