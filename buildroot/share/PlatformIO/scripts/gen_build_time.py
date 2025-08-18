#
# gen_build_time.py
#

Import("env")
from datetime import datetime
from pathlib import Path

# Get the current build time formatted
build_time = datetime.now().strftime("%Y-%m-%d %H:%M:%S")

# Define the target path to the source file
out_src = Path("Marlin/src/gcode/host/M115.cpp")

# The new line to define FW_BUILD_TIME
new_define_line = f'#define FW_BUILD_TIME "{build_time}"'

updated_lines = []

# Flag to check if FW_BUILD_TIME has been inserted
inserted_build_time = False

# Ensure the parent directory exists
out_src.parent.mkdir(parents=True, exist_ok=True)

# Read the existing content of the file if it exists
if out_src.exists():
    with open(out_src, 'r', encoding='utf-8') as f:
        for line in f:
            if line.strip().startswith('#define FW_BUILD_TIME'):
                # If an existing FW_BUILD_TIME define is found
                if not inserted_build_time:
                    updated_lines.append(new_define_line + '\n')
                    inserted_build_time = True
            elif '*/' in line and not inserted_build_time:
                # Insert FW_BUILD_TIME after end of comment block
                updated_lines.append(line)
                updated_lines.append(new_define_line + '\n')
                inserted_build_time = True
                print(f"Inserted FW_BUILD_TIME after '*/' in {out_src}")
            else:
                # Append all other lines as they are
                updated_lines.append(line)

# Write the updated content back to the file
out_src.write_text("".join(updated_lines), encoding='utf-8')
print(f"Updated {out_src} with build time: {build_time}")
