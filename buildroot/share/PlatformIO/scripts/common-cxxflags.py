#
# common-cxxflags.py
# Convenience script to apply customizations to CPP flags
#

import re

Import("env")
env.Append(CXXFLAGS=[
  "-Wno-register"
  #"-Wno-incompatible-pointer-types",
  #"-Wno-unused-const-variable",
  #"-Wno-maybe-uninitialized",
  #"-Wno-sign-compare"
])


# We can use it to apply custom compilation flags for individual folders or files
def ignore_lcd_warnings(node):
	"""
	`node.name` - a name of File System Node
	`node.get_path()` - a relative path
	`node.get_abspath()` - an absolute path
	"""
	mat = re.match(r'.*\/LiquidCrystal\/.*', node.get_path())
	if not mat:
		return node

	#print(node.get_path())
	return env.Object(
        node,
        CCFLAGS=env["CCFLAGS"] + ["-Wno-comment"]
    )

env.AddBuildMiddleware(ignore_lcd_warnings)