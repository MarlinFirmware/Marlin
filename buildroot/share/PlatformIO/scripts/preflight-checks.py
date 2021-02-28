#
# preflight-checks.py
# Check for common issues prior to compiling
#
import os,re
Import("env")

def get_envs_for_board(board):
    if board.startswith("BOARD_"):
        board = board[6:]
    with open(os.path.join("Marlin", "src", "pins", "pins.h"),"r") as f:
        board_found = ""
        r=re.compile(r"if\s+MB\((.+)\)")
        for line in f.readlines():
            mbs = r.findall(line)
            if mbs:
                board_found = board if board in re.split(r",\s*", mbs[0]) else ""
            if board_found and "#include " in line and "env:" in line:
                return re.findall(r"env:\w+", line)
    return []

def check_envs(build_env, base_envs, config):
    if build_env in base_envs:
        return True
    ext = config.get(build_env, 'extends', default=None)
    if ext:
        if isinstance(ext, str):
            return check_envs(ext, base_envs, config)
        elif isinstance(ext, list):
            for ext_env in ext:
                if check_envs(ext_env, base_envs, config):
                    return True
    return False

# Sanity checks:
if 'PIOENV' not in env:
    raise SystemExit("Error: PIOENV is not defined. This script is intended to be used with PlatformIO")

if 'MARLIN_FEATURES' not in env:
    raise SystemExit("Error: this script should be used after common Marlin scripts")

if 'MOTHERBOARD' not in env['MARLIN_FEATURES']:
    raise SystemExit("Error: MOTHERBOARD is not defined in Configuration.h")

build_env = env['PIOENV']
motherboard = env['MARLIN_FEATURES']['MOTHERBOARD']
base_envs = get_envs_for_board(motherboard)
config = env.GetProjectConfig()
result = check_envs("env:"+build_env, base_envs, config)

if not result:
    err = "Error: your selected build environment '%s' is not compatible with MOTHERBOARD=%s in Configuration.h. " \
          "Please use one of compatible build environments for this board: %s" % \
          (build_env, motherboard, ",".join([e[4:] for e in base_envs if e.startswith("env:")]))
    raise SystemExit(err)

#
# Check for Config files in two common incorrect places
#
for p in [ env['PROJECT_DIR'], os.path.join(env['PROJECT_DIR'], "config") ]:
    for f in [ "Configuration.h", "Configuration_adv.h" ]:
        if os.path.isfile(os.path.join(p, f)):
            err = "ERROR: Config files found in directory %s. Please move them into the Marlin subfolder." % p
            raise SystemExit(err)
