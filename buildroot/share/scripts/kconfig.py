#!/usr/bin/env python3
#
# Marlin 3D Printer Firmware
# Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
#
# Based on Sprinter and grbl.
# Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
#

# Generate Marlin TFT Images from bitmaps/PNG/JPG

import os
import sys

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
