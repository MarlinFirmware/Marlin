#!/usr/bin/env python3
"""
kconfig_gen.py

Convert Marlin's Configuration.h/adv.h defines into Kconfig entries.

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

DEFAULT_INPUTS = [
    ("Marlin/Configuration.h", "Kconfig", False),
    ("Marlin/Configuration_adv.h", "Kconfig_adv", False)
]

define_re = re.compile(r'^(?P<comment>\s*\/\/+)?\s*#define\s+(?P<key>[A-Za-z0-9_]+)(?:\s+(?P<val>[^/]*?))?\s*(?://\s*(?P<help>.*))?$')
undef_re = re.compile(r'^\s*#undef\s+([A-Za-z0-9_]+)')
ifdef_re = re.compile(r'^\s*#\s*(ifdef|ifndef|elif|else|endif)\s*(\w+)?')
if_re = re.compile(r'^\s*#if\s+(.+)')
enabled_if_re = re.compile(r'^\s*#if\s+ENABLED\((\w+)\)')
disabled_if_re = re.compile(r'^\s*#if\s+DISABLED\((\w+)\)')
int_re = re.compile(r'^-?\d+$')

CONDITIONAL_FILES = [
    "Marlin/src/inc/Conditionals-1-axes.h",
    "Marlin/src/inc/Conditionals-2-LCD.h",
    "Marlin/src/inc/Conditionals-3-etc.h",
    "Marlin/src/inc/Conditionals-6-type.h",
    "Marlin/src/inc/MarlinConfig.h",
    "Marlin/src/inc/MarlinConfigPre.h"
]

def is_complex_string(val):
    if val.startswith('"') and val.endswith('"'):
        return '"' in val[1:-1] or '\\n' in val
    return any(tok in val for tok in [" ", "(", ")", "+", "STRINGIFY", "PREHEAT", "\\n"])

class MacroState:
    def __init__(self):
        self.macros = {}
        self.undefs = set()

    def define(self, name, value=None, enabled=True):
        self.macros[name] = {"enabled": enabled, "value": value}

    def undef(self, name):
        self.undefs.add(name)
        if name in self.macros:
            self.macros[name]["enabled"] = False

    def is_defined(self, name):
        return name in self.macros and self.macros[name]["enabled"] and name not in self.undefs

    def get_value(self, name):
        if self.is_defined(name):
            return self.macros[name].get("value")
        return None

    def get_enabled(self, name):
        return self.is_defined(name)

    def update_from(self, other):
        for k, v in other.macros.items():
            self.macros[k] = v.copy()
        self.undefs.update(other.undefs)

    def copy(self):
        new = MacroState()
        new.macros = {k: v.copy() for k, v in self.macros.items()}
        new.undefs = set(self.undefs)
        return new

def eval_expr(expr, macros):
    expr = expr.strip()
    # Handle defined(X)
    if expr.startswith("defined("):
        macro = expr[8:-1]
        return macros.is_defined(macro)
    # Handle ENABLED(X)
    elif expr.startswith("ENABLED("):
        macro = expr[8:-1]
        return macros.is_defined(macro)
    # Handle DISABLED(X)
    elif expr.startswith("DISABLED("):
        macro = expr[9:-1]
        return not macros.is_defined(macro)
    # Handle macro == value
    elif "==" in expr:
        parts = expr.split("==", 1)
        macro = parts[0].strip()
        val = parts[1].strip()
        macro_val = macros.get_value(macro)
        return str(macro_val) == val
    # Handle macro != value
    elif "!=" in expr:
        parts = expr.split("!=", 1)
        macro = parts[0].strip()
        val = parts[1].strip()
        macro_val = macros.get_value(macro)
        return str(macro_val) != val
    # Handle numeric literals
    elif expr.isdigit() or (expr.startswith('-') and expr[1:].isdigit()):
        return int(expr) != 0
    # Handle macro as boolean
    elif expr in macros.macros:
        return macros.is_defined(expr)
    # Handle simple logical expressions (limited)
    elif "&&" in expr or "||" in expr:
        expr = expr.replace("&&", " and ").replace("||", " or ")
        try:
            tokens = re.split(r'(\s+and\s+|\s+or\s+)', expr)
            eval_tokens = []
            for tok in tokens:
                tok = tok.strip()
                if tok in ("and", "or", ""):
                    eval_tokens.append(tok)
                else:
                    if tok in macros.macros:
                        eval_tokens.append(str(macros.is_defined(tok)))
                    else:
                        eval_tokens.append(tok)
            return eval(" ".join(eval_tokens))
        except Exception:
            return False
    return False

def build_global_macro_state():
    macro_state = MacroState()
    macro_state = parse_macros_from_file("Marlin/Configuration.h", macro_state)
    for cond_file in CONDITIONAL_FILES:
        macro_state = parse_macros_from_file(cond_file, macro_state)
    return macro_state

def parse_macros_from_file(filename, macro_state=None):
    if macro_state is None:
        macro_state = MacroState()
    if not os.path.exists(filename):
        return macro_state

    with open(filename, "r", encoding="utf-8") as infile:
        condition_stack = []
        for line in infile:
            line = line.strip()
            # Handle #undef
            m_undef = undef_re.match(line)
            if m_undef:
                macro_state.undef(m_undef.group(1))
                continue

            # Handle #define
            m_define = define_re.match(line)
            if m_define:
                key = m_define.group("key")
                val = (m_define.group("val") or "").strip()
                is_enabled = m_define.group("comment") is None
                macro_state.define(key, val, is_enabled)
                continue

            # Handle #if, #ifdef, #ifndef, #elif, #else, #endif
            m_ifdef = ifdef_re.match(line)
            m_if = if_re.match(line)
            if m_ifdef:
                directive, macro = m_ifdef.group(1), m_ifdef.group(2)
                if directive == "ifdef" and macro:
                    condition_stack.append(macro_state.is_defined(macro))
                elif directive == "ifndef" and macro:
                    condition_stack.append(not macro_state.is_defined(macro))
                elif directive == "elif" and macro:
                    if condition_stack:
                        condition_stack.pop()
                    condition_stack.append(macro_state.is_defined(macro))
                elif directive == "else":
                    if condition_stack:
                        last = condition_stack.pop()
                        condition_stack.append(not last)
                elif directive == "endif":
                    if condition_stack:
                        condition_stack.pop()
                continue
            elif m_if:
                expr = m_if.group(1)
                condition_stack.append(eval_expr(expr, macro_state))
                continue

            m_enabled = enabled_if_re.match(line)
            m_disabled = disabled_if_re.match(line)
            if m_enabled:
                macro = m_enabled.group(1)
                condition_stack.append(macro_state.is_defined(macro))
                continue
            elif m_disabled:
                macro = m_disabled.group(1)
                condition_stack.append(not macro_state.is_defined(macro))
                continue

            if False in condition_stack:
                continue

    return macro_state

def parse_defines_with_conditionals(input_file, output_file, macro_state, disable_all=False):
    with open(input_file, "r", encoding="utf-8") as infile, \
         open(output_file, "w", encoding="utf-8", newline="\n") as outfile:

        outfile.write(f"# Auto-generated {os.path.basename(output_file)} file\n\n")
        condition_stack = []
        seen_keys = set()
        local_macros = macro_state.copy()

        for line in infile:
            line = line.strip()
            # Handle #undef
            m_undef = undef_re.match(line)
            if m_undef:
                local_macros.undef(m_undef.group(1))
                continue

            # Handle #define
            m_define = define_re.match(line)
            if m_define:
                key = m_define.group("key")
                val = (m_define.group("val") or "").strip()
                is_enabled = m_define.group("comment") is None
                # Force-disable all defines if requested
                if disable_all:
                    is_enabled = False
                local_macros.define(key, val, is_enabled)
                # Don't continue here; we want to process the config output below

            # Handle #if, #ifdef, #ifndef, #elif, #else, #endif
            m_ifdef = ifdef_re.match(line)
            m_if = if_re.match(line)
            if m_ifdef:
                directive, macro = m_ifdef.group(1), m_ifdef.group(2)
                if directive == "ifdef" and macro:
                    condition_stack.append(local_macros.is_defined(macro))
                elif directive == "ifndef" and macro:
                    condition_stack.append(not local_macros.is_defined(macro))
                elif directive == "elif" and macro:
                    if condition_stack:
                        condition_stack.pop()
                    condition_stack.append(local_macros.is_defined(macro))
                elif directive == "else":
                    if condition_stack:
                        last = condition_stack.pop()
                        condition_stack.append(not last)
                elif directive == "endif":
                    if condition_stack:
                        condition_stack.pop()
                continue
            elif m_if:
                expr = m_if.group(1)
                condition_stack.append(eval_expr(expr, local_macros))
                continue

            m_enabled = enabled_if_re.match(line)
            m_disabled = disabled_if_re.match(line)
            if m_enabled:
                macro = m_enabled.group(1)
                condition_stack.append(local_macros.is_defined(macro))
                continue
            elif m_disabled:
                macro = m_disabled.group(1)
                condition_stack.append(not local_macros.is_defined(macro))
                continue

            # If inside a false conditional block, skip #define/#undef
            if False in condition_stack:
                continue

            # Only process #define lines for config output
            if not m_define:
                continue

            key = m_define.group("key")
            if key in seen_keys:
                continue
            seen_keys.add(key)

            val = (m_define.group("val") or "").strip()
            help_comment = m_define.group("help")
            is_enabled = local_macros.get_enabled(key)
            macro_val = local_macros.get_value(key)
            if macro_val is not None:
                val = macro_val

            enable_key = f"{key}_ENABLE"
            enable_prompt = f"Enable {key}"
            default_enable = "y" if is_enabled else "n"
            depends_on = None  # Could be enhanced to track condition_stack

            # Write enable config
            outfile.write(f"config {enable_key}\n")
            outfile.write(f'    bool "{enable_prompt}"\n')
            outfile.write(f"    default {default_enable}\n")
            if depends_on:
                outfile.write(f"    depends on {depends_on}\n")
            outfile.write("\n")

            val_lower = str(val).lower()
            if val_lower in {"true", "false"}:
                default_opt = "true" if val_lower == "true" else "false"
                outfile.write("choice\n")
                outfile.write(f'    prompt "Select {key} (true/false)"\n')
                outfile.write(f"    depends on {enable_key}\n")
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
                outfile.write(f"    depends on {enable_key}\n")
                outfile.write(f"    default {key}_{default_opt}\n\n")
                for opt in choices:
                    outfile.write(f"config {key}_{opt}\n")
                    outfile.write(f'    bool "{opt}"\n\n')
                outfile.write("endchoice\n\n")
                continue

            if int_re.match(str(val)):
                num = int(val)
                outfile.write(f"config {key}\n")
                outfile.write(f'    int "{key}"\n')
                outfile.write(f"    depends on {enable_key}\n")
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
            outfile.write(f"    depends on {enable_key}\n")
            if not is_complex_string(str(val)):
                val_str = str(val).strip('"')
                outfile.write(f'    default "{val_str}"\n')
            else:
                outfile.write(f"    # default string skipped due to complexity: {val}\n")
            if help_comment:
                outfile.write("    help\n      " + help_comment.strip() + "\n")
            outfile.write("\n")

    print(f"\u2705 Wrote {output_file} from {input_file}")

def main():
    macro_state = build_global_macro_state()
    for input_file, output_file, disable_all in DEFAULT_INPUTS:
        if not os.path.exists(input_file):
            print(f"\u26a0\ufe0f  Skipping missing file: {input_file}")
            continue
        parse_defines_with_conditionals(input_file, output_file, macro_state, disable_all=disable_all)

if __name__ == "__main__":
    main()
