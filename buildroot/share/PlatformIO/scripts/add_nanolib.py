#
# add_nanolib.py
#
Import("env")

print("Executing add_nanolib.py")

env.Append(LINKFLAGS=["--specs=nano.specs"])
