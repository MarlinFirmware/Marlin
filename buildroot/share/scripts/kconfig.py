#!/usr/bin/env python3
"""
extract_defines.py

Convert Marlin's Configuration.h/adv.h defines (even if disabled or conditional)
into Kconfig entries with ENABLE toggles and conditional `depends on` logic.

Supports:
- #define and //#define
- #ifdef / #ifndef / #elif / #else / #endif
- #if ENABLED(MACRO) / DISABLED(MACRO)
"""

import sys
import re
import os

CHOICE_MAP = {
    "BAUDRATE": ["9600", "57600", "115200", "230400", "250000", "576000"],
}

RANGE_MAP = {
    "XYZ_STEPS_PER_UNIT": (1, 500),
}

define_re = re.compile(r'^(?P<comment>\s*\/\/+)?\s*#define\s+(?P<key>[A-Za-z0-9_]+)(?:\s+(?P<val>[^/]*?))?\s*(?://\s*(?P<help>.*))?$')
ifdef_re = re.compile(r'^\s*#\s*(ifdef|ifndef|elif|else|endif)\s*(\w+)?')
enabled_if_re = re.compile(r'^\s*#if\s+ENABLED\((\w+)\)')
disabled_if_re = re.compile(r'^\s*#if\s+DISABLED\((\w+)\)')
int_re = re.compile(r'^-?\d+$')

DEFAULT_INPUTS = [
    ("Marlin/Configuration.h", "Kconfig"),
    ("Marlin/Configuration_adv.h", "Kconfig_adv")
]

def kconfig_dep_from_macro(macro, is_not=False):
    enable_macro = f"{macro}_ENABLE"
    return f"!{enable_macro}" if is_not else enable_macro

def is_complex_string(val):
    return any(tok in val for tok in ['"', ' ', '(', ')', '+', 'STRINGIFY', 'PREHEAT', '\\n'])

def parse_defines(input_file, output_file):
    with open(input_file, 'r', encoding='utf-8') as infile, \
         open(output_file, 'w', encoding='utf-8', newline='\n') as outfile:

        title = os.path.basename(output_file)
        outfile.write(f"# Auto-generated {title} file\n\n")

        condition_stack = []
        seen_keys = set()

        for line in infile:
            m_if = ifdef_re.match(line)
            if m_if:
                directive, macro = m_if.group(1), m_if.group(2)
                if directive == "ifdef" and macro:
                    condition_stack.append(kconfig_dep_from_macro(macro))
                elif directive == "ifndef" and macro:
                    condition_stack.append(kconfig_dep_from_macro(macro, is_not=True))
                elif directive == "elif" and macro:
                    if condition_stack:
                        condition_stack.pop()
                    condition_stack.append(kconfig_dep_from_macro(macro))
                elif directive == "else":
                    if condition_stack:
                        cond = condition_stack.pop()
                        condition_stack.append(f"!({cond})" if not cond.startswith("!") else cond[2:-1])
                elif directive == "endif":
                    if condition_stack:
                        condition_stack.pop()
                continue

            # Handle ENABLED() and DISABLED()
            m_enabled = enabled_if_re.match(line)
            m_disabled = disabled_if_re.match(line)
            if m_enabled:
                macro = m_enabled.group(1)
                condition_stack.append(kconfig_dep_from_macro(macro))
                continue
            elif m_disabled:
                macro = m_disabled.group(1)
                condition_stack.append(kconfig_dep_from_macro(macro, is_not=True))
                continue

            m = define_re.match(line)
            if not m:
                continue

            key = m.group("key")
            if key in seen_keys:
                continue
            seen_keys.add(key)

            val = (m.group("val") or "").strip()
            help_comment = m.group("help")
            is_enabled = m.group("comment") is None

            enable_key = f"{key}_ENABLE"
            enable_prompt = f"Enable {key}"
            default_enable = "y" if is_enabled else "n"
            depends_on = " && ".join(condition_stack) if condition_stack else None

            if not (depends_on and (enable_key in depends_on or f"!{enable_key}" in depends_on)):
                outfile.write(f"config {enable_key}\n")
                outfile.write(f'    bool "{enable_prompt}"\n')
                outfile.write(f"    default {default_enable}\n")
                if depends_on:
                    outfile.write(f"    depends on {depends_on}\n")
                outfile.write("\n")

            val_lower = val.lower()
            if val_lower in {"true", "false"}:
                default_opt = "true" if val_lower == "true" else "false"
                outfile.write("choice\n")
                outfile.write(f'    prompt "Select {key} (true/false)"\n')
                outfile.write(f"    depends on {enable_key}")
                if depends_on and enable_key not in depends_on:
                    outfile.write(f" && {depends_on}")
                outfile.write("\n")
                outfile.write(f"    default {key}_{default_opt}\n\n")
                for opt in ["true", "false"]:
                    outfile.write(f"config {key}_{opt}\n")
                    outfile.write(f'    bool "{opt}"\n\n')
                outfile.write("endchoice\n\n")
                continue

            if key in CHOICE_MAP:
                choices = CHOICE_MAP[key]
                default_opt = val if val in choices else choices[0]

                outfile.write("choice\n")
                outfile.write(f'    prompt "Select {key}"\n')
                outfile.write(f"    depends on {enable_key}")
                if depends_on and enable_key not in depends_on:
                    outfile.write(f" && {depends_on}")
                outfile.write("\n")
                outfile.write(f"    default {key}_{default_opt}\n\n")
                for opt in choices:
                    outfile.write(f"config {key}_{opt}\n")
                    outfile.write(f'    bool "{opt}"\n\n')
                outfile.write("endchoice\n\n")
                continue

            if int_re.match(val):
                num = int(val)
                outfile.write(f"config {key}\n")
                outfile.write(f'    int "{key}"\n')
                outfile.write(f"    depends on {enable_key}")
                if depends_on and enable_key not in depends_on:
                    outfile.write(f" && {depends_on}")
                outfile.write("\n")
                if key in RANGE_MAP:
                    lo, hi = RANGE_MAP[key]
                    outfile.write(f"    range {lo} {hi}\n")
                outfile.write(f"    default {num}\n")
                if help_comment:
                    outfile.write("    help\n      " + help_comment.strip() + "\n")
                outfile.write("\n")
                continue

            if not val:
                continue

            outfile.write(f"config {key}\n")
            outfile.write(f'    string "{key}"\n')
            outfile.write(f"    depends on {enable_key}")
            if depends_on and enable_key not in depends_on:
                outfile.write(f" && {depends_on}")
            outfile.write("\n")

            if not is_complex_string(val):
                val_str = val.strip('"')
                outfile.write(f'    default "{val_str}"\n')
            else:
                outfile.write(f'    # default string skipped due to complexity: {val}\n')

            if help_comment:
                outfile.write("    help\n      " + help_comment.strip() + "\n")
            outfile.write("\n")

    print(f"\u2705 Wrote {output_file} from {input_file}")

def main():
    if len(sys.argv) > 1:
        input_files = [(f, f"Kconfig_{os.path.splitext(os.path.basename(f))[0].lower()}") for f in sys.argv[1:]]
    else:
        input_files = DEFAULT_INPUTS

    for input_file, output_file in input_files:
        if not os.path.exists(input_file):
            print(f"\u26a0\ufe0f  Skipping missing file: {input_file}")
            continue
        parse_defines(input_file, output_file)

if __name__ == "__main__":
    main()
