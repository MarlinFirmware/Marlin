Import("env")

my_flags = env.ParseFlags(env['BUILD_FLAGS'])
defines = {k: v for (k, v) in my_flags.get("CPPDEFINES")}

#env.Replace(PROGNAME="firmware_%s" % defines.get("VERSION"))
env.Replace(PROGNAME="%s" % defines.get("HEX_NAME"))