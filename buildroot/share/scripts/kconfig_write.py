#!/usr/bin/env python3
"""
kconfig_write.py

Convert .config (generated from Kconfig) back into Marlin/Configuration.h and Marlin/Configuration_adv.h defines.
"""

CONFIG_MAP = {
    "Kconfig": "Configuration.h",
    "Kconfig_adv": "Configuration_adv.h"
}

def parse_config(config_file):
    defines = []
    with open(config_file, encoding="utf-8") as f:
        for line in f:
            line = line.strip()
            if line.startswith("CONFIG_"):
                key_val = line.split("=", 1)
                if len(key_val) != 2:
                    continue
                key, val = key_val
                macro = key[7:]  # Strip CONFIG_
                # Skip ENABLE macros
                if macro.endswith("_ENABLE"):
                    continue
                if val == "y":
                    defines.append(f"#define {macro}")
                elif val == "n":
                    defines.append(f"//#define {macro}")
                elif val.isdigit():
                    defines.append(f"#define {macro} {val}")
                elif val.startswith("\""):
                    defines.append(f"#define {macro} {val}")
                else:
                    defines.append(f"#define {macro} {val}")
    return defines

def write_header(defines, header_file):
    with open(header_file, "w", encoding="utf-8") as f:
        f.write("// Auto-generated from .config\n\n")
        for line in defines:
            f.write(line + "\n")
    print(f"✅ Wrote {header_file}")

def main():
    # Automatically process both .config and .config_adv files
    for config_file, header_file in [(".config", "Marlin/Configuration.h"), (".config_adv", "Marlin/Configuration_adv.h")]:
        try:
            defines = parse_config(config_file)

            # Read corresponding Kconfig file (based on your existing CONFIG_MAP)
            kconfig_file = "Kconfig" if header_file == "Marlin/Configuration.h" else "Kconfig_adv"
            with open(kconfig_file, encoding="utf-8") as f:
                relevant_keys = {line.strip().split()[1] for line in f if line.strip().startswith("config ")}

            # Filter defines by relevant keys
            header_defines = [d for d in defines if d.split()[1] in relevant_keys]
            write_header(header_defines, header_file)
        except FileNotFoundError:
            print(f"⚠️ File {config_file} or {kconfig_file} not found, skipping.")

if __name__ == "__main__":
    main()
