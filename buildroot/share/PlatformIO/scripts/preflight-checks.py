#
# preflight-checks.py
# Check for common issues prior to compiling
#
import pioutil
if pioutil.is_pio_build():

    import re, sys
    from pathlib import Path
    env = pioutil.env

    def get_envs_for_board(board):
        ppath = Path("Marlin/src/pins/pins.h")
        with ppath.open() as file:

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

        # Require PlatformIO 6.1.1 or later
        vers = pioutil.get_pio_version()
        if vers < [6, 1, 1]:
            raise SystemExit("Error: Marlin requires PlatformIO >= 6.1.1. Use 'pio upgrade' to get a newer version.")

        if 'MARLIN_FEATURES' not in env:
            raise SystemExit("Error: this script should be used after common Marlin scripts.")

        # Useful values
        project_dir = Path(env['PROJECT_DIR'])
        config_files = ("Configuration.h", "Configuration_adv.h")
        mpath = project_dir / "Marlin"

        #
        # Update old macros BOTH and EITHER in configuration files
        #
        conf_modified = False
        for f in config_files:
            conf_path = mpath / f
            if conf_path.is_file():
                with open(conf_path, 'r', encoding="utf8") as file:
                    text = file.read()
                    modified_text = text.replace("BOTH(", "ALL(").replace("EITHER(", "ANY(")
                    if text != modified_text:
                        conf_modified = True
                        with open(conf_path, 'w', encoding="utf8", newline='') as file:
                            file.write(modified_text)

        if conf_modified:
            raise SystemExit('WARNING: Configuration files needed an update to remove incompatible items. Try the build again to use the updated files.')

        if len(env['MARLIN_FEATURES']) == 0:
            raise SystemExit("Error: Failed to parse Marlin features. See previous error messages.")

        #
        # Alert user for config files in 'project' or 'project/config'
        # NOTE: Some issues could prevent reaching this check.
        #
        has_cfgs = (mpath / "Config.h").is_file() or ((mpath / config_files[0]).is_file() and (mpath / config_files[1]).is_file())
        for p in (project_dir, project_dir / "config"):
            for f in config_files:
                if (p / f).is_file():
                    desc = "Redundant" if has_cfgs else "Your"
                    err = f"ERROR: {desc} config files were found in {p}."
                    err += " Put the configs you want to use into the 'Marlin' subfolder."
                    raise SystemExit(err)

        if not has_cfgs:
            raise SystemExit("Error: No configuration files found! Put your config files into the 'Marlin' subfolder.")

        build_env = env['PIOENV']
        motherboard = env['MARLIN_FEATURES']['MOTHERBOARD']
        board_envs = get_envs_for_board(motherboard)
        config = env.GetProjectConfig()
        result = check_envs("env:"+build_env, board_envs, config)

        # Make sure board is compatible with the build environment. Skip for _test,
        # since the board is manipulated as each unit test is executed.
        if not result and not build_env.endswith("_native_test"):
            err = "Error: Build environment '%s' is incompatible with %s. Use one of these environments: %s" % \
                  ( build_env, motherboard, ", ".join([ e[4:] for e in board_envs if e.startswith("env:") ]) )
            raise SystemExit(err)

        #
        # Find the name.cpp.o or name.o and remove it
        #
        def rm_ofile(subdir, name):
            build_dir = Path(env['PROJECT_BUILD_DIR'], build_env)
            for outdir in (build_dir, build_dir / "debug"):
                for ext in (".cpp.o", ".o"):
                    fpath = outdir / "src/src" / subdir / (name + ext)
                    if fpath.exists():
                        fpath.unlink()

        #
        # Give warnings on every build
        #
        rm_ofile("inc", "Warnings")

        #
        # Renew date/time
        #
        rm_ofile("gcode/host", "M115")
        rm_ofile("lcd/menu", "menu_info")

        #
        # Rebuild 'settings.cpp' for EEPROM_INIT_NOW
        #
        if 'EEPROM_INIT_NOW' in env['MARLIN_FEATURES']:
            rm_ofile("module", "settings")

        #
        # Check for old files indicating an entangled Marlin (mixing old and new code)
        #
        mixedin = []
        p = project_dir / "Marlin/src/lcd/dogm"
        for f in [ "ultralcd_DOGM.cpp", "ultralcd_DOGM.h", "u8g_dev_ssd1306_sh1106_128x64_I2C.cpp", "u8g_dev_ssd1309_12864.cpp", "u8g_dev_st7565_64128n_HAL.cpp", "u8g_dev_st7920_128x64_HAL.cpp", "u8g_dev_tft_upscale_from_128x64.cpp", "u8g_dev_uc1701_mini12864_HAL.cpp", "ultralcd_st7920_u8glib_rrd_AVR.cpp" ]:
            if (p / f).is_file():
                mixedin += [ f ]
        p = project_dir / "Marlin/src/feature/bedlevel/abl"
        for f in [ "abl.cpp", "abl.h" ]:
            if (p / f).is_file():
                mixedin += [ f ]
        if mixedin:
            err = "ERROR: Old files fell into your Marlin folder. Remove %s and try again" % ", ".join(mixedin)
            raise SystemExit(err)

        #
        # Check FILAMENT_RUNOUT_SCRIPT has a %c parammeter when required
        #
        if 'FILAMENT_RUNOUT_SENSOR' in env['MARLIN_FEATURES'] and 'NUM_RUNOUT_SENSORS' in env['MARLIN_FEATURES']:
            if env['MARLIN_FEATURES']['NUM_RUNOUT_SENSORS'].isdigit() and int(env['MARLIN_FEATURES']['NUM_RUNOUT_SENSORS']) > 1:
                if 'FILAMENT_RUNOUT_SCRIPT' in env['MARLIN_FEATURES']:
                    frs = env['MARLIN_FEATURES']['FILAMENT_RUNOUT_SCRIPT']
                    if "M600" in frs and "%c" not in frs:
                        err = "ERROR: FILAMENT_RUNOUT_SCRIPT needs a %c parameter (e.g., \"M600 T%c\") when NUM_RUNOUT_SENSORS is > 1"
                        raise SystemExit(err)


    sanity_check_target()
