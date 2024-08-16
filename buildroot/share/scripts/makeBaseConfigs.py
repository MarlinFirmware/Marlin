#!/usr/bin/env python3
#
# makeBaseConfigs.py
#
# Create "base" config files that can be overridden by a minimal config file.
# The intention is to include base config files as part of the Marlin package
# so users can use a minimal configuration with the familiar header format.
# This provides an Arduino-compatible way to apply configurations without the
# complications that come from a plain config.ini solution.
#
import re
from pathlib import Path

def make_base_configs():
    no_disabled = True

    # Create a regex to match options and capture line parts
    define_patt = re.compile(r'^(\s*)((//\s*)?#define\s+)([A-Z0-9_]+\b)(\s*)(.*?)(\s*)(//.*)?$', re.IGNORECASE)
    ifndef_patt = re.compile(r'^(\s*#ifndef\s*.*?)(\s*//.*)?$', re.IGNORECASE)
    ifstat_patt = re.compile(r'^(\s*#(((if|ifn?def|elif)\s*.*?)|else|endif))(\s*//.*)?$', re.IGNORECASE)
    coment_patt = re.compile(r'/\*.*?\*/', re.DOTALL)
    contin_patt = re.compile(r'\\\n\s*')

    # Wrap all config options in #ifndef
    for file in ('Configuration.h', 'Configuration_adv.h'):
        fullpath = Path('Marlin', file)
        fulltext = fullpath.read_text(encoding='utf-8')
        fulltext = coment_patt.sub('', fulltext)
        fulltext = contin_patt.sub(' ', fulltext)

        is_ifndef = False

        lines_out = []
        for line in fulltext.split('\n'):

            was_ifndef = is_ifndef              # Previous line was #ifndef?
            is_ifndef = ifndef_patt.match(line) # This line is #ifndef?

            m = ifstat_patt.match(line)
            if m: lines_out += [m[1]] ; continue

            m = define_patt.match(line)
            if m:
                if no_disabled and line.strip().startswith('//'): continue

                name = m[4]
                if name in ('CONFIG_EXAMPLES_DIR', 'CONFIG_EXPORT'): continue
                if name.startswith('_'): continue

                if not was_ifndef: lines_out += [f'{m[1]}#ifndef {name}']

                entab = '' if was_ifndef else '  '
                indented = f'{entab}{m[1]}{m[2]}{m[4]} {m[6]}'.rstrip()
                lines_out += [indented]

                if not was_ifndef: lines_out += [f'{m[1]}#endif']

        # Final text needs some additional cleanup to remove empty blocks
        empty_patt = re.compile(r'(\s*#if.+)(\n\s*#el.+)*(\n\s*#endif.*)')
        ifelse_patt = re.compile(r'(\s*#(el)?if\s+)(.+)\n\s*#else')
        ifelif_patt = re.compile(r'(\s*#if\s+)(.+)\n\s*#elif\s*(.+)')
        out_text = '\n'.join(lines_out)
        while True:
            old_text = out_text
            out_text = ifelse_patt.sub(r'\1!(\3)', out_text)
            out_text = ifelif_patt.sub(r'\1!(\2) && (\3)\n', out_text)
            out_text = empty_patt.sub('', out_text)
            if out_text == old_text: break

        # Store the final result to Marlin/src/inc/BaseConfiguration.h BaseConfiguration_adv.h
        outname = f'Base{file}'
        outpath = Path('Marlin', 'src', 'inc', outname)

        with outpath.open('w') as outfile:

            from datetime import datetime
            header_tpl = Path('buildroot', 'share', 'extras', 'file_header.h').read_text(encoding='utf-8')
            header_tpl = re.sub(r'(\(c\))\s*\d+\s*(MarlinFirmware)', rf'\1 {datetime.now().year} \2', header_tpl)
            header_tpl = re.sub(r'\*/\n+/\*\*', '*/\n#pragma once\n\n/**', header_tpl)
            header_tpl = header_tpl.replace(r'$(filename)', f"{outname} - Generated using makeBaseConfigs.py")

            outfile.write(header_tpl + out_text + '\n')

# Always run from the command line.
# Later we can add a "minimal" flag to config to do this automatically.
if __name__ == "__main__":
    import sys
    args = sys.argv[1:]
    if len(args) > 0:
        print(f"Usage: {sys.argv[0]}")
    elif not Path('Marlin', 'src').is_dir():
        print(f"Please run {sys.argv[0]} from the root of the Marlin workspace.")
    else:
        make_base_configs()
