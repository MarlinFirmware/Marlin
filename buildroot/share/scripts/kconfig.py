#!/usr/bin/env python3
"""
kconfig.py [--clean] .config Marlin

Generate configuration files using Kconfig through terminal UI
"""
import os, sys

# Map config keys to their target header
CONFIG_H_KEYS = {
    "BAUDRATE": "int",
    "MOTHERBOARD": "string",
}
ADV_H_KEYS = {
    "ADVANCED_PAUSE_FEATURE": "bool",
    "ARC_SUPPORT": "bool",
}

def parse_config(config_path):
    options = {}
    with open(config_path) as f:
        for line in f:
            line = line.strip()
            if line.startswith("CONFIG_"):
                key, val = line.split("=", 1)
                options[key[7:]] = val
    return options

def write_header(options, keys, out_path, header_name):
    with open(out_path, "w") as f:
        f.write(f"// Auto-generated {header_name} by gen_headers.py\n")
        for key, typ in keys.items():
            if key in options:
                val = options[key]
                if typ == "bool":
                    if val == "y":
                        f.write(f"#define {key}\n")
                    # If 'n', do not define
                elif typ == "string":
                    # Remove any wrapping quotes, then add them back
                    v = val.strip()
                    if v.startswith('"') and v.endswith('"'):
                        v = v[1:-1]
                    f.write(f'#define {key} "{v}"\n')
                # ...existing code...
                else:
                    f.write(f"#define {key} {val}\n")

if __name__ == "__main__":
    config_path = ".config"
    if not os.path.exists(config_path):
        print("No .config file found. Run menuconfig first.")
        sys.exit(1)
    options = parse_config(config_path)
    write_header(options, CONFIG_H_KEYS, "Configuration.h", "Configuration.h")
    write_header(options, ADV_H_KEYS, "Configuration_adv.h", "Configuration_adv.h")
    print("Generated Configuration.h and Configuration_adv.h")
