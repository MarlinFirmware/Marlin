#
# preflight-checks.py
# Check for common issues prior to compiling
#
import pioutil
if pioutil.is_pio_build():

    import json, re, sys
    from pathlib import Path
    env = pioutil.env

    def exit_with_error(title, *body):
        '''Exit with a formatted error. Body items are paragraphs, or lists of preformatted lines.'''
        out = ['', 'Error: ' + title]
        for item in body:
            out.append('')
            out += [ '  ' + line for line in ([item] if isinstance(item, str) else item) ]
        raise SystemExit('\n'.join(out) + '\n')

    def get_env_notes(envs, config):
        '''
        Describe each env by its MCU and what sets it apart, taken from its [env:] comment.
        Comments usually read "Board (MCU) with <variant>", sometimes with a variant line below.
        '''
        comments = {}
        for ini in [ Path('platformio.ini') ] + sorted(Path('ini').glob('*.ini')):
            lines = ini.read_text(encoding='utf8').splitlines()
            for i, line in enumerate(lines):
                m = re.match(r'\[env:(\w+)\]', line)
                if not m or m[1] not in envs: continue
                block, j = [], i - 1
                while j >= 0 and lines[j].startswith('#'):
                    text = lines[j].lstrip('#').strip()
                    if text: block.insert(0, text)
                    j -= 1
                comments[m[1]] = block

        def get_variant(block):
            for n, text in enumerate(block):
                if re.search(r'\.{3}', text): continue                         # Skip env tables
                text = re.sub(r'\s*\((see above|https?://[^)]*)\)', '', text)  # Drop asides
                if '(' in text:                                                # "Board (MCU) with <variant>"
                    tail = text[text.rindex(')') + 1:].strip()
                elif n == 0 and ' with ' in text:                              # "Board with <variant>"
                    tail = text[text.index(' with ') + 1:]
                else:
                    tail = '' if n == 0 else text                              # A variant line
                tail = re.split(r'(?<=\.)\s', tail)[0]                         # First sentence only
                tail = tail.rstrip('.')
                if tail: return tail[0].upper() + tail[1:]
            return ''

        boards_dir = Path(config.get('platformio', 'boards_dir', 'buildroot/share/PlatformIO/boards'))
        def get_mcu(e):
            board = config.get('env:' + e, 'board', '')
            try: return json.loads((boards_dir / (board + '.json')).read_text())['build']['mcu'].upper()
            except: pass
            try:
                from platformio.platform.factory import PlatformFactory
                return PlatformFactory.new(config.get('env:' + e, 'platform')).board_config(board).get('build.mcu', '').upper()
            except: return ''

        return { e: (get_mcu(e), get_variant(comments.get(e, []))) for e in envs }

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
            exit_with_error("PIOENV is not defined.", "This script is intended to be used with PlatformIO.")

        # Require PlatformIO 6.1.1 or later
        vers = pioutil.get_pio_version()
        if vers < [6, 1, 1]:
            exit_with_error("Marlin requires PlatformIO >= 6.1.1.", "Use 'pio upgrade' to get a newer version.")

        if 'MARLIN_FEATURES' not in env:
            exit_with_error("This script should always follow the common Marlin scripts.")

        if len(env['MARLIN_FEATURES']) == 0:
            exit_with_error("Failed to parse Marlin features.", "See previous error messages.")

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
            raise SystemExit('WARNING: Configuration files updated to remove incompatible items. Build again to use the updated files.')

        #
        # Alert user for config files in 'project' or 'project/config'
        # NOTE: Some issues could prevent reaching this check.
        #
        has_cfgs = (mpath / "Config.h").is_file() or ((mpath / config_files[0]).is_file() and (mpath / config_files[1]).is_file())
        for p in (project_dir, project_dir / "config"):
            for f in config_files:
                if (p / f).is_file():
                    desc = "Redundant" if has_cfgs else "Your"
                    exit_with_error(f"{desc} config files were found in {p}.", "Put the configs you want to use into the 'Marlin' subfolder.")

        if not has_cfgs:
            exit_with_error("No configuration files found!", "Put your config files into the 'Marlin' subfolder.")

        # Check for common errors in MOTHERBOARD setting
        motherboard = env['MARLIN_FEATURES']['MOTHERBOARD']
        if motherboard.startswith("MOTHERBOARD "):
            exit_with_error('MOTHERBOARD setting mangled by an extra instance of "MOTHERBOARD."')
        if not motherboard.startswith("BOARD_"):
            exit_with_error("MOTHERBOARD setting missing BOARD_ prefix.", f"Found '{motherboard}'. Did you mean 'BOARD_{motherboard}'?")

        build_env = env['PIOENV']
        board_envs = get_envs_for_board(motherboard)
        config = env.GetProjectConfig()
        result = check_envs("env:"+build_env, board_envs, config)

        # Make sure board is compatible with the build environment. Skip for _test,
        # since the board is manipulated as each unit test is executed.
        if not result and not build_env.endswith("_native_test"):
            envs = [ e[4:] for e in board_envs if e.startswith("env:") ]
            if not envs:
                exit_with_error("No build environment found for %s." % motherboard,
                                "Check the MOTHERBOARD setting in Configuration.h. Valid board names are listed in Marlin/src/core/boards.h.")
            notes = get_env_notes(envs, config)
            pad = max(len(e) for e in envs)
            mpad = max(len(m) for m, _ in notes.values())
            env_list = [ ("* %-*s  %-*s  %s" % (pad, e, mpad, *notes[e])).rstrip() for e in envs ]
            if build_env in config.default_envs():
                fix, cmd = "Set default_envs in platformio.ini to", "default_envs = " + envs[0]
            else:
                fix, cmd = "Build with", "pio run -e " + envs[0]
            if len(envs) == 1:
                howto = [ fix + ":", [ cmd ] ]
            else:
                howto = [ fix + " one of these environments:", env_list, [ "e.g., " + cmd ] ]
            howto.append("We recommend using the 'Auto Build Marlin' extension in VSCode.")
            exit_with_error("Build environment '%s' is incompatible with %s." % (build_env, motherboard), *howto)

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
        # Build Warnings.cpp first. It includes SanityCheck.h, so a config error
        # stops the build there instead of repeating for every source file.
        #
        warnings_cpp = mpath / "src/inc/Warnings.cpp"
        if warnings_cpp.is_file():
            marlin_src = str(mpath / "src")
            def build_warnings_first(env, node):
                src = node.srcnode().get_abspath()
                if not src.startswith(marlin_src): return node
                obj = env.Object(node)
                if Path(src) != warnings_cpp:
                    env.Requires(obj, env.File("$BUILD_DIR/src/src/inc/Warnings.cpp.o"))
                return obj
            env.AddBuildMiddleware(build_warnings_first)

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
        p = mpath / "src/lcd/dogm"
        for f in [ "ultralcd_DOGM.cpp", "ultralcd_DOGM.h", "u8g_dev_ssd1306_sh1106_128x64_I2C.cpp", "u8g_dev_ssd1309_12864.cpp", "u8g_dev_st7565_64128n_HAL.cpp", "u8g_dev_st7920_128x64_HAL.cpp", "u8g_dev_tft_upscale_from_128x64.cpp", "u8g_dev_uc1701_mini12864_HAL.cpp", "ultralcd_st7920_u8glib_rrd_AVR.cpp" ]:
            if (p / f).is_file():
                mixedin += [ f ]
        p = mpath / "src/feature/bedlevel/abl"
        for f in [ "abl.cpp", "abl.h" ]:
            if (p / f).is_file():
                mixedin += [ f ]
        p = mpath / "src/gcode/feature/pause"
        for f in [ "G60.cpp", "G61.cpp" ]:
            if (p / f).is_file():
                mixedin += [ f ]
        if mixedin:
            exit_with_error("Old files fell into your Marlin folder.", "Remove these files and try again:", [ "* " + f for f in mixedin ])

        #
        # Check FILAMENT_RUNOUT_SCRIPT has a %c parammeter when required
        #
        if 'FILAMENT_RUNOUT_SENSOR' in env['MARLIN_FEATURES'] and 'NUM_RUNOUT_SENSORS' in env['MARLIN_FEATURES']:
            if env['MARLIN_FEATURES']['NUM_RUNOUT_SENSORS'].isdigit() and int(env['MARLIN_FEATURES']['NUM_RUNOUT_SENSORS']) > 1:
                if 'FILAMENT_RUNOUT_SCRIPT' in env['MARLIN_FEATURES']:
                    frs = env['MARLIN_FEATURES']['FILAMENT_RUNOUT_SCRIPT']
                    if "M600" in frs and "%c" not in frs:
                        exit_with_error("FILAMENT_RUNOUT_SCRIPT needs a %c parameter (e.g., \"M600 T%c\") when NUM_RUNOUT_SENSORS is > 1.")


    sanity_check_target()
