#!/usr/bin/env python3
#
# mc-apply.py
#
#  Apply firmware configuration from a JSON file (marlin_config.json).
#
#  usage: mc-apply.py [-h] [--opt] [--verbose] [config_file]
#
#  Process Marlin firmware configuration.
#
#  positional arguments:
#    config_file  Path to the configuration file.
#
#  optional arguments:
#    -h, --help   show this help message and exit
#    --opt        Output as an option setting script.
#    --verbose    Enable verbose logging (0-2)
#
import json, sys, os, re, shutil, datetime
import config
import argparse
from pathlib import Path

verbose = 0
def blab(str, level=1):
    if verbose >= level: print(f"[mc-apply] {str}")

def config_path(cpath):
    return Path("Marlin", cpath)

def normalize_value(v):
    """
    Normalize configuration values to consistent format.
    Returns tuple: (action, value) where action is 'enable', 'disable', or 'set'

    - "on", "true", True, "" -> ('enable', '')  - Enable without value
    - "off", "false", False -> ('disable', '')  - Disable/comment out
    - Any other value -> ('set', value)         - Enable with value
    """
    # Convert to string for comparison, handle JSON booleans
    if isinstance(v, bool):
        v_str = 'true' if v else 'false'
    else:
        v_str = str(v).strip().lower()

    # Check for enable values
    if v_str in ('on', 'true', ''):
        return ('enable', '')

    # Check for disable values
    elif v_str in ('off', 'false'):
        return ('disable', '')

    # Everything else is a value to set
    else:
        return ('set', v if not isinstance(v, bool) else v_str)

# Disable all (most) defined options in the configuration files.
def disable_all_options():
    blab("Disabling all configuration options...")
    # Create a regex to match the option and capture parts of the line
    regex = re.compile(r'^(\s*)(#define\s+)([A-Z0-9_]+\b)(\s?)(\s*)(.*?)(\s*)(//.*)?$', re.IGNORECASE)

    # Disable all enabled options in both Config files
    for file in ("Configuration.h", "Configuration_adv.h"):
        fullpath = config_path(file)
        if not fullpath.exists():
            blab(f"File not found: {fullpath}", 0)
            continue

        lines = fullpath.read_text(encoding='utf-8').split('\n')
        found = False
        for i in range(len(lines)):
            line = lines[i]
            match = regex.match(line)
            if match:
                name = match[3].upper()
                if name in ('CONFIGURATION_H_VERSION', 'CONFIGURATION_ADV_H_VERSION', 'CONFIG_EXAMPLES_DIR'): continue
                if name.startswith('_'): continue
                found = True
                # Comment out the define
                lines[i] = re.sub(r'^(\s*)(#define)(\s{1,3})?(\s*)', r'\1//\2 \4', line)
                blab(f"Disable {name}", 2)

        # If the option was found, write the modified lines
        if found:
            fullpath.write_text('\n'.join(lines), encoding='utf-8')
            blab(f"Updated {file}")

# Fetch configuration files from GitHub given the path.
# Return True if any files were fetched.
def fetch_example(url):
    blab(f"Fetching example configuration from: {url}")
    if url.endswith("/"): url = url[:-1]
    if not url.startswith('http'):
        brch = "bugfix-2.1.x"
        if '@' in url: url, brch = map(str.strip, url.split('@'))
        if url == 'examples/default': url = 'default'
        url = f"https://raw.githubusercontent.com/MarlinFirmware/Configurations/{brch}/config/{url}"
    url = url.replace("%", "%25").replace(" ", "%20")

    # Find a suitable fetch command
    if shutil.which("curl") is not None:
        fetch = "curl -L -s -S -f -o"
    elif shutil.which("wget") is not None:
        fetch = "wget -q -O"
    else:
        blab("Couldn't find curl or wget", 0)
        return False

    # Reset configurations to default
    blab("Resetting configurations to default...")
    os.system("git checkout HEAD Marlin/*.h")

    # Try to fetch the remote files
    gotfile = False
    for fn in ("Configuration.h", "Configuration_adv.h", "_Bootscreen.h", "_Statusscreen.h"):
        if os.system(f"{fetch} wgot {url}/{fn} >/dev/null 2>&1") == 0:
            shutil.move('wgot', config_path(fn))
            gotfile = True
            blab(f"Fetched {fn}", 2)

    if Path('wgot').exists(): shutil.rmtree('wgot')

    if gotfile:
        blab("Example configuration fetched successfully")
    else:
        blab("Failed to fetch example configuration", 0)

    return gotfile

def report_version(conf):
    if 'VERSION' in conf:
        blab("Configuration version information:")
        for k, v in sorted(conf['VERSION'].items()):
            print(k + ': ' + v)

def write_opt_file(conf, outpath='Marlin/apply_config.sh'):
    blab(f"Writing configuration script to {outpath}")
    with open(outpath, 'w', encoding='utf-8') as outfile:
        for key, val in conf.items():
            if key in ('__INITIAL_HASH', '__directives__', 'VERSION'): continue

            # Other keys are assumed to be configs
            if not type(val) is dict:
                continue

            # Write config commands to the script file
            lines = []
            for k, v in sorted(val.items()):
                action, norm_val = normalize_value(v)

                if action == 'enable':
                    lines += [f'opt_enable {k}']
                    blab(f"  opt_enable {k}", 2)
                elif action == 'disable':
                    lines += [f'opt_disable {k}']
                    blab(f"  opt_disable {k}", 2)
                else:  # action == 'set'
                    norm_val = str(norm_val).replace('"', '\\"').replace("'", "\\'").replace(' ', '\\ ')
                    lines += [f'opt_set {k} {norm_val}']
                    blab(f"  opt_set {k} {norm_val}", 2)

            outfile.write('\n'.join(lines))

        print('Config script written to: ' + outpath)

def back_up_config(name):
    # Back up the existing file before modifying it
    conf_path = 'Marlin/' + name
    if not os.path.exists(conf_path):
        blab(f"Config file not found: {conf_path}", 0)
        return

    with open(conf_path, 'r', encoding='utf-8') as f:
        # Write a filename.bak#.ext retaining the original extension
        parts = conf_path.split('.')
        nr = ''
        while True:
            bak_path = '.'.join(parts[:-1]) + f'.bak{nr}.' + parts[-1]
            if os.path.exists(bak_path):
                nr = 1 if nr == '' else nr + 1
                continue

            with open(bak_path, 'w', encoding='utf-8', newline='') as b:
                b.writelines(f.readlines())
                blab(f"Backed up {conf_path} to {bak_path}", 2)
                break

def process_directives(directives):
    """Process special directives before applying config options"""
    if not isinstance(directives, list):
        directives = [directives]

    for directive in directives:
        directive = directive.strip()
        blab(f"Processing directive: {directive}")

        # Handle [disable] directive
        if directive == "[disable]":
            disable_all_options()

        # Handle example fetching (examples/path or example/path)
        elif directive.startswith('examples/') or directive.startswith('example/'):
            if directive.startswith('example/'):
                directive = 'examples' + directive[7:]
            fetch_example(directive)

        # Handle direct URLs
        elif directive.startswith('http://') or directive.startswith('https://'):
            fetch_example(directive)

        else:
            blab(f"Unknown directive: {directive}", 0)

def apply_config(conf):
    # Process directives first if they exist
    if '__directives__' in conf:
        blab("=" * 20 + " Processing directives...")
        process_directives(conf['__directives__'])

    # Apply configuration options
    blab("=" * 20 + " Applying configuration options...")
    for key in conf:
        if key in ('__INITIAL_HASH', '__directives__', 'VERSION'): continue

        # Skip non-dict values
        if not isinstance(conf[key], dict):
            continue

        back_up_config(key)

        for k, v in conf[key].items():
            action, norm_val = normalize_value(v)
            conf_file = 'Marlin/' + key

            if action == 'enable':
                blab(f"Enabling {k}", 2)
                config.enable(conf_file, k, True)
            elif action == 'disable':
                blab(f"Disabling {k}", 2)
                config.enable(conf_file, k, False)
            else:  # action == 'set'
                blab(f"Setting {k} = {norm_val}", 2)
                config.set(conf_file, k, norm_val)

def main():
    global verbose

    parser = argparse.ArgumentParser(description='Process Marlin firmware configuration.')
    parser.add_argument('--opt', action='store_true', help='Output as an option setting script.')
    parser.add_argument('--verbose', '-v', type=int, default=0, help='Verbose logging level (0-2, default: 0)')
    parser.add_argument('config_file', nargs='?', default='marlin_config.json', help='Path to the configuration file.')

    args = parser.parse_args()

    # Set verbose level
    verbose = args.verbose

    try:
        infile = open(args.config_file, 'r', encoding='utf-8')
    except:
        print(f'No {args.config_file} found.')
        sys.exit(1)

    conf = json.load(infile)
    report_version(conf)

    if args.opt:
        write_opt_file(conf)
    else:
        apply_config(conf)

if __name__ == '__main__':
    main()
