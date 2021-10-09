#
# preflight-checks.py
# Check for common issues prior to compiling
#
import os,re,sys
Import("env")

def get_envs_for_board(board):
	with open(os.path.join("Marlin", "src", "pins", "pins.h"), "r") as file:

		if sys.platform == 'win32':
			envregex = r"(?:env|win):"
		elif sys.platform == 'darwin':
			envregex = r"(?:env|mac|uni):"
		elif sys.platform == 'linux':
			envregex = r"(?:env|lin|uni):"
		else:
			envregex = r"(?:env):"

		r = re.compile(r"if\s+MB\((.+)\)")
		if board.startswith("BOARD_"):
			board = board[6:]

		for line in file:
			mbs = r.findall(line)
			if mbs and board in re.split(r",\s*", mbs[0]):
				line = file.readline()
				found_envs = re.match(r"\s*#include .+" + envregex, line)
				if found_envs:
					envlist = re.findall(envregex + r"(\w+)", line)
					return [ "env:"+s for s in envlist ]
	return []

def check_envs(build_env, board_envs, config):
	if build_env in board_envs:
		return True
	ext = config.get(build_env, 'extends', default=None)
	if ext:
		if isinstance(ext, str):
			return check_envs(ext, board_envs, config)
		elif isinstance(ext, list):
			for ext_env in ext:
				if check_envs(ext_env, board_envs, config):
					return True
	return False

def sanity_check_target():
	# Sanity checks:
	if 'PIOENV' not in env:
		raise SystemExit("Error: PIOENV is not defined. This script is intended to be used with PlatformIO")

	if 'MARLIN_FEATURES' not in env:
		raise SystemExit("Error: this script should be used after common Marlin scripts")

	if 'MOTHERBOARD' not in env['MARLIN_FEATURES']:
		raise SystemExit("Error: MOTHERBOARD is not defined in Configuration.h")

	build_env = env['PIOENV']
	motherboard = env['MARLIN_FEATURES']['MOTHERBOARD']
	board_envs = get_envs_for_board(motherboard)
	config = env.GetProjectConfig()
	result = check_envs("env:"+build_env, board_envs, config)

	if not result:
		err = "Error: Build environment '%s' is incompatible with %s. Use one of these: %s" % \
			  ( build_env, motherboard, ", ".join([ e[4:] for e in board_envs if e.startswith("env:") ]) )
		raise SystemExit(err)

	#
	# Check for Config files in two common incorrect places
	#
	for p in [ env['PROJECT_DIR'], os.path.join(env['PROJECT_DIR'], "config") ]:
		for f in [ "Configuration.h", "Configuration_adv.h" ]:
			if os.path.isfile(os.path.join(p, f)):
				err = "ERROR: Config files found in directory %s. Please move them into the Marlin subfolder." % p
				raise SystemExit(err)

	#
	# Give warnings on every build
	#
	warnfile = os.path.join(env['PROJECT_BUILD_DIR'], build_env, "src", "src", "inc", "Warnings.cpp.o")
	if os.path.exists(warnfile):
		os.remove(warnfile)

	#
	# Check for old files indicating an entangled Marlin (mixing old and new code)
	#
	mixedin = []
	p = os.path.join(env['PROJECT_DIR'], "Marlin", "src", "lcd", "dogm")
	for f in [ "ultralcd_DOGM.cpp", "ultralcd_DOGM.h" ]:
		if os.path.isfile(os.path.join(p, f)):
			mixedin += [ f ]
	if mixedin:
		err = "ERROR: Old files fell into your Marlin folder. Remove %s and try again" % ", ".join(mixedin)
		raise SystemExit(err)

# Detect that 'vscode init' is running
from SCons.Script import COMMAND_LINE_TARGETS
if "idedata" not in COMMAND_LINE_TARGETS:
	sanity_check_target()
