#
# add_nanolib.py
#
Import("env")

env.Append(LINKFLAGS=["--specs=nano.specs"])
