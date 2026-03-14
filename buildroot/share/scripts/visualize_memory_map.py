#!/usr/bin/env python3
"""
Visualize memory map

Memory Map Visualizer for Marlin Firmware (AVR and ARM/STM32)
Generates an interactive HTML page showing memory layout with color coding
Uses nm to extract symbols from ELF file

Author: Dust
Additional contributor: Thomas Toka (Windows support)
Implementation assistance: Claude Sonnet 4.5
"""

import re
import sys
import subprocess
import argparse
import struct
import os
import shutil
from pathlib import Path
from collections import defaultdict

def parse_size(size_str):
    """Parse size string with optional KB/MB suffix to bytes.

    Examples:
        '512KB' -> 524288
        '20KB' -> 20480
        '256' -> 256
        '1MB' -> 1048576
    """
    size_str = str(size_str).strip().upper()

    # Match number with optional unit
    match = re.match(r'^([0-9.]+)\s*(B|KB|K|MB|M)?$', size_str)
    if not match:
        raise argparse.ArgumentTypeError(f"Invalid size format: {size_str}. Use formats like '512KB', '20KB', or '262144'")

    value = float(match.group(1))
    unit = match.group(2) or 'B'

    # Convert to bytes
    if unit in ('B', ''):
        return int(value)
    elif unit in ('KB', 'K'):
        return int(value * 1024)
    elif unit in ('MB', 'M'):
        return int(value * 1024 * 1024)
    else:
        raise argparse.ArgumentTypeError(f"Unknown unit: {unit}")

def detect_architecture(elf_path):
    """Detect if ELF is AVR or ARM by reading ELF header directly (cross-platform)"""
    try:
        with open(elf_path, 'rb') as f:
            # Read ELF header
            elf_header = f.read(20)  # First 20 bytes contain what we need

            # Check ELF magic number
            if elf_header[:4] != b'\x7fELF':
                print(f"Warning: Not a valid ELF file: {elf_path}", file=sys.stderr)
                return 'avr'  # Default to AVR

            # ELF class (32-bit or 64-bit) is at byte 4
            # Machine type is at bytes 18-19 (little endian)
            e_machine = struct.unpack('<H', elf_header[18:20])[0]

            # ELF machine types:
            # 0x53 (83) = AVR
            # 0x28 (40) = ARM
            if e_machine == 0x53:
                return 'avr'
            elif e_machine == 0x28:
                return 'arm'
            else:
                print(f"Warning: Unknown machine type 0x{e_machine:02x}, defaulting to AVR", file=sys.stderr)
                return 'avr'
    except Exception as e:
        print(f"Warning: Could not read ELF header from {elf_path}: {e}", file=sys.stderr)
        return 'avr'  # Default to AVR if reading fails

def find_nm_tool(arch):
    """
    Find the appropriate nm tool for the architecture (Windows/Linux/macOS).

    Search order:
      1) PATH (shutil.which)
      2) Project-local PlatformIO: <project>/.pio/packages/...
      3) User PlatformIO: ~.platformio/packages/...
      4) Fallback to bare tool name (keeps legacy behavior)
    """
    exe = '.exe' if os.name == 'nt' else ''
    tool = f'avr-nm{exe}' if arch == 'avr' else f'arm-none-eabi-nm{exe}'

    # 1) PATH
    found = shutil.which(tool)
    if found:
        return found

    # Script path: <project>/buildroot/share/scripts/visualize_memory_map.py
    # Project root is 3 parents up from scripts/
    script_dir = Path(__file__).resolve().parent
    try:
        project_root = script_dir.parents[3]
    except IndexError:
        project_root = Path.cwd()

    # 2) Project-local PlatformIO
    pio_packages = project_root / '.pio' / 'packages'

    # 3) User PlatformIO
    user_packages = Path.home() / '.platformio' / 'packages'

    if arch == 'avr':
        candidates = [
            pio_packages / 'toolchain-atmelavr' / 'bin' / tool,
            user_packages / 'toolchain-atmelavr' / 'bin' / tool,
        ]
    else:
        candidates = [
            pio_packages / 'toolchain-gccarmnoneeabi' / 'bin' / tool,
            user_packages / 'toolchain-gccarmnoneeabi' / 'bin' / tool,
        ]

    for c in candidates:
        if c.exists():
            return str(c)

    # 4) Fallback
    return tool

def normalize_address(addr, arch):
    """Normalize addresses for different architectures
    STM32: Flash at 0x08000000, RAM at 0x20000000
    AVR: Flash at 0x00000000, RAM at 0x00800000+ (varies by chip)
    """
    if arch == 'arm':
        # STM32 Flash starts at 0x08000000, RAM at 0x20000000
        if addr >= 0x20000000:  # RAM
            return addr - 0x20000000
        elif addr >= 0x08000000:  # Flash
            return addr - 0x08000000
    elif arch == 'avr':
        # AVR RAM typically starts at 0x00800000 or higher
        # Flash starts at 0x00000000
        if addr >= 0x00800000:  # RAM region
            return addr - 0x00800000
    return addr

def get_memory_type_arm(addr):
    """Determine if ARM address is Flash or RAM"""
    if addr >= 0x20000000:
        return 'ram'
    else:  # 0x08000000 range
        return 'flash'

def parse_nm_output(elf_path, arch):
    """Parse nm output to extract all symbols with sizes"""
    symbols = []
    special_symbols = {}  # For heap, stack, bss markers
    zero_size_important = []  # For important symbols that nm doesn't report size for

    nm_tool = find_nm_tool(arch)

    # Early, explicit failure with a helpful hint (prevents WinError 2 confusion)
    nm_tool_path = Path(nm_tool)
    if not nm_tool_path.exists() and shutil.which(nm_tool) is None:
        print(f"Error: nm tool not found: {nm_tool}", file=sys.stderr)
        print("Hint: Build once with PlatformIO so .pio/packages is populated, or add the toolchain bin directory to PATH.", file=sys.stderr)
        print("On Windows for ARM builds, expected path looks like: .pio\\packages\\toolchain-gccarmnoneeabi\\bin\\arm-none-eabi-nm.exe", file=sys.stderr)
        sys.exit(1)

    # First, get special symbols and important zero-size markers (without --print-size)
    try:
        result = subprocess.run(
            [nm_tool, '--demangle', str(elf_path)],
            capture_output=True,
            text=True,
            check=True
        )

        for line in result.stdout.strip().split('\n'):
            parts = line.split(None, 2)
            if len(parts) >= 3:
                addr_str, sym_type, name = parts
                addr_raw = int(addr_str, 16)

                # Capture special symbols for heap/stack display
                if name in ['__heap_start', '__heap_end', '__bss_end', '_end', '__stack', '__data_end', '__bss_start', '_estack', '_sstack']:
                    # Normalize address for architecture
                    addr = normalize_address(addr_raw, arch)
                    special_symbols[name] = addr

                # Capture important zero-size symbols that define memory regions
                elif name in ['g_pfnVectors', '__vectors', '__trampolines_start', '__trampolines_end',
                              '__ctors_start', '__ctors_end', '__dtors_start', '__dtors_end',
                              '__init', '__fini', '_sidata', '_sdata', '_edata', '_sbss', '_ebss']:
                    zero_size_important.append({
                        'name': name,
                        'addr_raw': addr_raw,
                        'addr': normalize_address(addr_raw, arch),
                        'sym_type': sym_type,
                        'size': 0
                    })
    except subprocess.CalledProcessError as e:
        print(f"Error running {nm_tool} for special symbols: {e}", file=sys.stderr)

    if special_symbols:
        print(f"  Special symbols found: {list(special_symbols.keys())}")
        for name, addr in special_symbols.items():
            print(f"    {name}: 0x{addr:08x}")

    # Now get regular symbols with sizes (using size-sort for better ordering)
    try:
        result = subprocess.run(
            [nm_tool, '--size-sort', '--print-size', '--demangle', str(elf_path)],
            capture_output=True,
            text=True,
            check=True
        )

        for line in result.stdout.strip().split('\n'):
            # Format: address size type name
            # Example: 00001234 00000100 T _ZN10GcodeSuite3G28Ev
            parts = line.split(None, 3)
            if len(parts) >= 4:
                addr_str, size_str, sym_type, name = parts
                addr_raw = int(addr_str, 16)
                size = int(size_str, 16)

                # Skip zero-size symbols EXCEPT for important startup symbols
                # These mark important regions that nm doesn't report sizes for
                if size == 0 and name not in ['__vectors', '__trampolines_start', '__trampolines_end',
                                                '__ctors_start', '__ctors_end', '__dtors_start', '__dtors_end',
                                                '__init', '__fini', 'g_pfnVectors']:
                    continue

                # Normalize address for architecture
                addr = normalize_address(addr_raw, arch)

                # Categorize by symbol type
                # t/T = text (code), d/D = initialized data, b/B = uninitialized data (BSS)
                # r/R = read-only data, V = weak object
                section = ''
                if sym_type in 'tTrR':
                    # For ARM, check actual address to determine section
                    if arch == 'arm':
                        mem_type = get_memory_type_arm(addr_raw)
                        if mem_type == 'flash':
                            section = '.text' if sym_type in 'tT' else '.rodata'
                        else:
                            section = '.data'  # Shouldn't happen for t/T/r/R but handle it
                    else:
                        section = '.text' if sym_type in 'tT' else '.rodata'
                elif sym_type in 'dD':
                    section = '.data'
                elif sym_type in 'bB':
                    section = '.bss'
                elif sym_type in 'vV':  # Weak objects
                    # For ARM, check actual address
                    if arch == 'arm':
                        mem_type = get_memory_type_arm(addr_raw)
                        section = '.data' if mem_type == 'ram' else '.rodata'
                    else:
                        section = '.data'  # Usually in RAM for weak objects
                else:
                    # For ARM, check actual address for unknown types
                    if arch == 'arm':
                        mem_type = get_memory_type_arm(addr_raw)
                        section = '.data' if mem_type == 'ram' else '.text'
                    else:
                        section = '.other'

                # Extract module name from demangled name
                module = categorize_symbol(name)

                symbols.append({
                    'name': name,
                    'addr': addr,
                    'addr_raw': addr_raw,  # Store original address for display
                    'size': size,
                    'section': section,
                    'module': module,
                    'type': 'symbol',
                    'sym_type': sym_type
                })
    except subprocess.CalledProcessError as e:
        print(f"Error running nm: {e}", file=sys.stderr)
        sys.exit(1)

    # Add important zero-size symbols that were captured earlier
    for zero_sym in zero_size_important:
        # Determine section based on symbol type and address
        sym_type = zero_sym['sym_type']
        addr_raw = zero_sym['addr_raw']

        if sym_type in 'tTrR':
            if arch == 'arm':
                mem_type = get_memory_type_arm(addr_raw)
                section = '.text' if sym_type in 'tT' else '.rodata' if mem_type == 'flash' else '.data'
            else:
                section = '.text' if sym_type in 'tT' else '.rodata'
        elif sym_type in 'dD':
            section = '.data'
        elif sym_type in 'bB':
            section = '.bss'
        else:
            if arch == 'arm':
                mem_type = get_memory_type_arm(addr_raw)
                section = '.data' if mem_type == 'ram' else '.text'
            else:
                section = '.other'

        module = categorize_symbol(zero_sym['name'])

        symbols.append({
            'name': zero_sym['name'],
            'addr': zero_sym['addr'],
            'addr_raw': zero_sym['addr_raw'],
            'size': 0,
            'section': section,
            'module': module,
            'type': 'symbol',
            'sym_type': sym_type
        })

    # Post-process: Calculate sizes for zero-size symbols based on next symbol
    # Sort symbols by address
    symbols.sort(key=lambda x: x['addr'])

    for i, sym in enumerate(symbols):
        if sym['size'] == 0 and i + 1 < len(symbols):
            # Find next symbol with non-zero address in same memory space
            _, _, mem_type = categorize_section(sym['section'])
            for next_sym in symbols[i + 1:]:
                _, _, next_mem_type = categorize_section(next_sym['section'])
                # Look for next symbol in same address space
                if next_mem_type == mem_type and next_sym['addr'] > sym['addr']:
                    # Calculate size as gap to next symbol
                    calculated_size = next_sym['addr'] - sym['addr']
                    if calculated_size > 0 and calculated_size < 100000:  # Sanity check
                        sym['size'] = calculated_size
                        # Update name to show it's important
                        if sym['name'] == '__vectors':
                            sym['name'] = 'Interrupt Vectors (__vectors)'
                            sym['module'] = 'Core'
                        elif '__trampolines' in sym['name']:
                            sym['module'] = 'Core'
                    break

    return symbols, special_symbols

def categorize_symbol(name, warn_conflicts=False):
    """Categorize symbol by module based on name patterns"""
    name_lower = name.lower()

    # Pattern definitions with their categories
    # NOTE: Order matters! More specific patterns should come before general ones
    patterns = [
        (['ubl', 'unified_bed_leveling'], 'UBL'),
        (['tmcstepper', 'tmc2130', 'tmc2208', 'tmc2209', 'tmc2660', 'tmc5160', 'tmcmarlin'], 'TMCLibrary'),
        (['planner'], 'Planner'),
        (['stepper'], 'Stepper'),
        (['temperature'], 'Temperature'),
        (['ftmotion', 'ft_motion'], 'FT_Motion'),
        (['motion', 'homing'], 'Motion'),
        (['gcode', 'gcodesuite'], 'GCode'),
        (['lcd', 'ui', 'marlinui'], 'LCD/UI'),
        (['probe'], 'Probe'),
        (['endstop'], 'Endstops'),
        (['serial'], 'Serial'),
        (['settings', 'eeprom'], 'Settings'),
        (['marlincore'], 'Core'),  # Note: 'marlin::' checked separately
    ]

    matches = []

    # Check all patterns
    for pattern_list, category in patterns:
        for pattern in pattern_list:
            if pattern in name_lower:
                matches.append((category, pattern))
                break  # Only need one match per category

    # Special case for marlin:: prefix (case-sensitive)
    if 'marlin::' in name:
        matches.append(('Core', 'marlin::'))

    # Warn if multiple categories match
    if warn_conflicts and len(matches) > 1:
        categories = [m[0] for m in matches]
        print(f"  ⚠ Ambiguous: '{name}' matches {len(matches)} categories: {', '.join([f'{c} ({p})' for c, p in matches])} → using {categories[0]}")

    # Return first match or 'Other'
    if matches:
        return matches[0][0]
    else:
        return 'Other'

def categorize_section(section_name):
    """Categorize section for color coding"""
    if section_name.startswith('.text'):
        return 'code', '#4CAF50', 'flash'       # Green code
    elif section_name.startswith('.data'):
        return 'data', '#2196F3', 'ram'         # Blue initialized data
    elif section_name.startswith('.bss'):
        return 'bss', '#FF9800', 'ram'          # Orange uninitialized data
    elif section_name.startswith('.rodata'):
        return 'rodata', '#9C27B0', 'flash'     # Purple read-only data
    else:
        return 'other', '#757575', 'flash'      # Gray other

def generate_memory_blocks_html(items, module_colors, zoom_level=1, total_memory_size=None, special_symbols=None, arch='avr'):
    """Generate HTML for memory blocks as a byte-by-byte map
    zoom_level: multiplier (1x = 2 pixels per byte + 1px delimiter, 2x = 4 pixels + 2px delimiter, etc.)
    total_memory_size: total available memory (to show unused space at end)
    special_symbols: dict of special symbols like __heap_start, __stack for RAM visualization
    arch: architecture ('avr' or 'arm') to determine address display format
    """
    if not items:
        return '<div style="color: #858585;">No data</div>'

    # Calculate actual pixels per byte and delimiter size
    pixels_per_byte = zoom_level * 2

    # Find memory range
    min_addr = min(item['addr'] for item in items)
    max_addr = max(item['addr'] + item['size'] for item in items)

    # Determine address base for ARM architecture
    addr_base = 0
    if arch == 'arm' and items:
        # Check if this is RAM or Flash by looking at items' original addresses
        # Find first item that has addr_raw set
        for item in items:
            if 'addr_raw' in item:
                if item['addr_raw'] >= 0x20000000:
                    addr_base = 0x20000000  # RAM
                else:
                    addr_base = 0x08000000  # Flash
                break

    # If total memory size specified, extend to show full memory
    if total_memory_size:
        total_size = total_memory_size
    else:
        total_size = max_addr - min_addr

    # Create byte array for memory map
    memory_map = [None] * total_size

    # Fill in the memory map with symbol info
    for item in items:
        start = item['addr'] - min_addr
        end = start + item['size']
        _, default_color, _ = categorize_section(item['section'])
        color = module_colors.get(item['module'], default_color)

        for i in range(start, min(end, total_size)):
            memory_map[i] = {
                'color': color,
                'name': item['name'],
                'addr': item['addr'],
                'addr_raw': item.get('addr_raw', item['addr']),  # Store original address for ARM display
                'size': item['size'],
                'section': item['section'],
                'module': item['module']
            }

    # Add heap/stack regions if this is RAM and we have special symbols
    if special_symbols:
        heap_start = special_symbols.get('__heap_start', special_symbols.get('_end', special_symbols.get('__bss_end')))

        # Stack grows downward from top of RAM on AVR
        # Reserve top portion of RAM for stack (typically starts 256-512 bytes from top)
        if heap_start:
            # Check if heap_start is within our RAM range
            heap_start_idx = heap_start - min_addr

            if 0 <= heap_start_idx < total_size:
                # Stack reserve: last 256 bytes of RAM
                stack_reserve_bytes = 256
                stack_start_idx = total_size - stack_reserve_bytes

                # Heap region: from end of BSS to start of stack reserve
                heap_end_idx = stack_start_idx

                # Mark heap region
                for i in range(heap_start_idx, heap_end_idx):
                    if 0 <= i < total_size and memory_map[i] is None:
                        addr = min_addr + i
                        memory_map[i] = {
                            'color': '#404040',
                            'name': 'Heap (available)',
                            'addr': addr,
                            'addr_raw': addr + addr_base if arch == 'arm' else addr,
                            'size': heap_end_idx - heap_start_idx,
                            'section': '.heap',
                            'module': 'Heap'
                        }

                # Mark stack reserve region (top 256 bytes)
                for i in range(stack_start_idx, total_size):
                    if 0 <= i < total_size and memory_map[i] is None:
                        addr = min_addr + i
                        memory_map[i] = {
                            'color': '#505050',
                            'name': 'Stack (reserve)',
                            'addr': addr,
                            'addr_raw': addr + addr_base if arch == 'arm' else addr,
                            'size': stack_reserve_bytes,
                            'section': '.stack',
                            'module': 'Stack'
                        }

    # Generate HTML as rows of pixels
    html = ''
    # Calculate bytes per row to keep display width constant at ~1000 pixels
    bytes_per_row = 1000 // pixels_per_byte

    # Iterate through memory in rows
    for row_start in range(0, total_size, bytes_per_row):
        row_end = min(row_start + bytes_per_row, total_size)
        html += '        <div class="memory-row">\n'

        # Iterate through each byte in this row
        i = row_start
        while i < row_end:
            byte_info = memory_map[i]

            # Find consecutive bytes with same data
            count = 1
            while (i + count < row_end and
                   ((byte_info is None and memory_map[i + count] is None) or
                    (byte_info is not None and memory_map[i + count] is not None and
                     memory_map[i + count]['name'] == byte_info['name']))):
                count += 1

            if byte_info is None:
                # Unallocated or unused space
                addr = i + min_addr
                # For ARM, add back the base address for display
                display_addr = addr + addr_base if arch == 'arm' else addr
                # Check if next byte is different (need delimiter)
                next_i = i + count
                needs_delimiter = False
                if next_i < row_end:
                    next_info = memory_map[next_i]
                    if next_info is not None:
                        needs_delimiter = True

                if addr >= max_addr:
                    # Unused space at end
                    if needs_delimiter:
                        html += f'          <div class="memory-pixel" style="flex-grow: {count}; background: #1a1a1a;" data-unused="true" data-addr="0x{display_addr:08x}" data-size="{count}" title="Unused: {count} bytes"></div>\n'
                        html +=  '          <div class="memory-delimiter"></div>\n'
                    else:
                        html += f'          <div class="memory-pixel" style="flex-grow: {count}; background: #1a1a1a;" data-unused="true" data-addr="0x{display_addr:08x}" data-size="{count}" title="Unused: {count} bytes"></div>\n'
                else:
                    # Gap between symbols - likely linker-inserted code, padding, or alignment
                    if needs_delimiter:
                        html += f'          <div class="memory-pixel" style="flex-grow: {count}; background: #3a3a3a;" data-unallocated="true" data-addr="0x{display_addr:08x}" data-size="{count}" title="Linker code/padding: {count} bytes"></div>\n'
                        html +=  '          <div class="memory-delimiter"></div>\n'
                    else:
                        html += f'          <div class="memory-pixel" style="flex-grow: {count}; background: #3a3a3a;" data-unallocated="true" data-addr="0x{display_addr:08x}" data-size="{count}" title="Linker code/padding: {count} bytes"></div>\n'
            else:
                # Symbol data
                display_name = byte_info['name'].replace('&', '&amp;').replace('<', '&lt;').replace('>', '&gt;').replace('"', '&quot;')

                # Check if next byte is different (need delimiter)
                next_i = i + count
                needs_delimiter = False
                if next_i < row_end:
                    next_info = memory_map[next_i]
                    if next_info is None or next_info['name'] != byte_info['name']:
                        needs_delimiter = True

                # For display: ARM uses raw addresses (0x08000000/0x20000000 ranges), AVR uses normalized
                display_addr = byte_info['addr_raw'] if arch == 'arm' else byte_info['addr']

                if needs_delimiter:
                    html += f'''          <div class="memory-pixel"
                   style="flex-grow: {count}; background: {byte_info['color']};"
                   data-name="{display_name}"
                   data-addr="0x{display_addr:08x}"
                   data-size="{byte_info['size']}"
                   data-section="{byte_info['section']}"
                   data-module="{byte_info['module']}"></div>
'''
                    html += f'          <div class="memory-delimiter"></div>\n'
                else:
                    html += f'''          <div class="memory-pixel"
                   style="flex-grow: {count}; background: {byte_info['color']};"
                   data-name="{display_name}"
                   data-addr="0x{display_addr:08x}"
                   data-size="{byte_info['size']}"
                   data-section="{byte_info['section']}"
                   data-module="{byte_info['module']}"></div>
'''

            i += count

        html += '        </div>\n'

    return html

def check_pattern_conflicts():
    """Check for pattern conflicts and warn about potential issues"""
    patterns = [
        (['ubl', 'unified_bed_leveling'], 'UBL'),
        (['planner'], 'Planner'),
        (['stepper'], 'Stepper'),
        (['temperature'], 'Temperature'),
        (['ftmotion', 'ft_motion'], 'FT_Motion'),
        (['motion', 'homing'], 'Motion'),
        (['gcode', 'gcodesuite'], 'GCode'),
        (['lcd', 'ui', 'marlinui'], 'LCD/UI'),
        (['probe'], 'Probe'),
        (['endstop'], 'Endstops'),
        (['serial'], 'Serial'),
        (['settings', 'eeprom'], 'Settings'),
        (['marlincore'], 'Core'),
    ]

    conflicts = []

    # Check for substring overlaps
    for i, (patterns1, cat1) in enumerate(patterns):
        for j, (patterns2, cat2) in enumerate(patterns):
            if i >= j:  # Only check each pair once
                continue
            for p1 in patterns1:
                for p2 in patterns2:
                    # Check if one is substring of the other
                    if p1 in p2 or p2 in p1:
                        # Determine which comes first in the list (has priority)
                        if i < j:
                            conflicts.append((p1, cat1, p2, cat2, 'priority'))
                        else:
                            conflicts.append((p2, cat2, p1, cat1, 'priority'))

    if conflicts:
        print("Pattern conflict analysis:")
        for p1, c1, p2, c2 in conflicts:
            print(f"  ℹ '{p1}' ({c1}) vs '{p2}' ({c2}) - '{p1}' has priority")

    return len(conflicts)

def generate_csv(symbols, flash_size, ram_size, output_dir='.', arch='avr'):
    """Generate CSV files for RAM and Flash memory usage"""
    import csv

    flash_items = []
    ram_items = []

    # Separate symbols by memory type
    for item in symbols:
        _, _, mem_type = categorize_section(item['section'])
        if mem_type == 'flash':
            flash_items.append(item)
        else:
            ram_items.append(item)

    # Sort by address (lowest first)
    flash_items.sort(key=lambda x: x['addr'])
    ram_items.sort(key=lambda x: x['addr'])

    # Calculate totals for percentages
    total_flash = sum(item['size'] for item in flash_items)
    total_ram = sum(item['size'] for item in ram_items)

    # Generate Flash CSV
    flash_csv_path = Path(output_dir) / 'flash.csv'
    with open(flash_csv_path, 'w', newline='', encoding='utf-8') as f:
        writer = csv.writer(f)
        writer.writerow(['Address', 'Size', 'Percentage', 'Name'])

        for item in flash_items:
            # For AVR use normalized address, for ARM use raw address
            addr = item['addr_raw'] if arch == 'arm' else item['addr']
            size = item['size']
            percentage = (size / total_flash * 100) if total_flash > 0 else 0
            name = item['name']
            writer.writerow([f'0x{addr:08x}', size, f'{percentage:.3f}', name])

    print(f"✓ Generated Flash CSV: {flash_csv_path}")
    print(f"  Entries: {len(flash_items):,}")

    # Generate RAM CSV
    ram_csv_path = Path(output_dir) / 'ram.csv'
    with open(ram_csv_path, 'w', newline='', encoding='utf-8') as f:
        writer = csv.writer(f)
        writer.writerow(['Address', 'Size', 'Percentage', 'Name'])

        for item in ram_items:
            # For AVR use normalized address, for ARM use raw address
            addr = item['addr_raw'] if arch == 'arm' else item['addr']
            size = item['size']
            percentage = (size / total_ram * 100) if total_ram > 0 else 0
            name = item['name']
            writer.writerow([f'0x{addr:08x}', size, f'{percentage:.3f}', name])

    print(f"✓ Generated RAM CSV: {ram_csv_path}")
    print(f"  Entries: {len(ram_items):,}")

def generate_html(symbols, special_symbols, output_path, flash_size, ram_size, arch='avr', show_list=False):
    """Generate interactive HTML visualization"""

    # Calculate total memory and gather stats
    total_flash = 0
    total_ram = 0
    module_stats = defaultdict(lambda: {'flash': 0, 'ram': 0})

    flash_items = []
    ram_items = []

    for item in symbols:
        color, mem_type = categorize_section(item['section'])
        if mem_type == 'flash':
            total_flash += item['size']
            module_stats[item['module']]['flash'] += item['size']
            flash_items.append(item)
        else:  # RAM
            total_ram += item['size']
            module_stats[item['module']]['ram'] += item['size']
            ram_items.append(item)

    # Sort by address for linear display
    flash_items.sort(key=lambda x: x['addr'])
    ram_items.sort(key=lambda x: x['addr'])

    # Memory sizes from command line arguments
    FLASH_SIZE = flash_size
    RAM_SIZE = ram_size

    print(f"  Special symbols found: {list(special_symbols.keys())}")
    if special_symbols:
        for sym, addr in special_symbols.items():
            print(f"    {sym}: 0x{addr:08x}")

    # Calculate section ranges
    section_ranges = {}
    for item in symbols:
        section = item['section']
        addr = item['addr']
        end_addr = addr + item['size']

        if section not in section_ranges:
            section_ranges[section] = {'start': addr, 'end': end_addr, 'type': categorize_section(section)[2]}
        else:
            section_ranges[section]['start'] = min(section_ranges[section]['start'], addr)
            section_ranges[section]['end'] = max(section_ranges[section]['end'], end_addr)

    # Add heap and stack to section ranges if available
    if special_symbols:
        heap_start = special_symbols.get('__heap_start', special_symbols.get('_end', special_symbols.get('__bss_end')))
        if heap_start and ram_items:
            # Find RAM base address
            ram_base = min(item['addr'] for item in ram_items)

            # For ARM, check if we have _estack which marks the end of RAM/top of stack
            if '_estack' in special_symbols:
                # _estack marks the top of the stack area (end of RAM)
                stack_end = special_symbols['_estack']
                # Reserve some stack space (ARM typically has larger stacks)
                stack_reserve_bytes = min(2048, (RAM_SIZE - (heap_start - ram_base)) // 2)
                stack_start = stack_end - stack_reserve_bytes
                heap_end = stack_start
            else:
                # AVR-style: reserve fixed stack space at end of RAM
                stack_reserve_bytes = 256
                heap_end = ram_base + RAM_SIZE - stack_reserve_bytes
                stack_start = heap_end
                stack_end = ram_base + RAM_SIZE

            section_ranges['.heap'] = {'start': heap_start, 'end': heap_end, 'type': 'ram'}
            section_ranges['.stack'] = {'start': stack_start, 'end': stack_end, 'type': 'ram'}

            print(f"  Heap: 0x{heap_start:08x} - 0x{heap_end:08x} ({heap_end - heap_start} bytes)")
            print(f"  Stack: 0x{stack_start:08x} - 0x{stack_end:08x} ({stack_end - stack_start} bytes)")

    # Module colors
    module_colors = {
        'UBL': '#4CAF50',
        'Planner': '#2196F3',
        'Stepper': '#FF5722',
        'Temperature': '#FF9800',
        'FT_Motion': '#E91E63',
        'Motion': '#9C27B0',
        'GCode': '#00BCD4',
        'LCD/UI': '#FFEB3B',
        'Probe': '#8BC34A',
        'Endstops': '#FF6F00',
        'Serial': '#3F51B5',
        'Settings': '#009688',
        'TMCLibrary': "#C10091",
        'Core': '#795548',
        'Other': '#757575'
    }

    # Generate HTML
    html = f"""<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>Marlin Memory Map Visualization</title>
    <style>
        * {{ margin: 0; padding: 0; box-sizing: border-box; }}
        body {{
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background: #1e1e1e;
            color: #d4d4d4;
            padding: 20px;
        }}
        .header {{
            text-align: center;
            margin-bottom: 30px;
            padding: 20px;
            background: #252526;
            border-radius: 8px;
        }}
        .header h1 {{ color: #61dafb; margin-bottom: 10px; }}
        .stats {{
            display: flex;
            justify-content: center;
            gap: 40px;
            margin-top: 20px;
            flex-wrap: wrap;
        }}
        .stat {{ text-align: center; }}
        .stat-value {{ font-size: 32px; font-weight: bold; color: #61dafb; }}
        .stat-label {{ font-size: 14px; color: #858585; margin-top: 5px; }}

        .memory-tabs {{
            display: flex;
            justify-content: flex-start;
            gap: 10px;
            margin-bottom: 20px;
        }}

        .zoom-controls {{
            display: flex;
            justify-content: center;
            align-items: center;
            gap: 10px;
            margin-bottom: 20px;
            padding: 15px;
            background: #252526;
            border-radius: 6px;
        }}

        .zoom-label {{
            color: #858585;
            font-size: 14px;
        }}

        .zoom-button {{
            padding: 8px 16px;
            background: #2d2d30;
            border: 2px solid #3c3c3c;
            color: #d4d4d4;
            cursor: pointer;
            border-radius: 4px;
            font-size: 14px;
            font-weight: bold;
            transition: all 0.3s;
        }}

        .zoom-button:hover {{
            background: #3c3c3c;
            border-color: #61dafb;
        }}

        .zoom-button.active {{
            background: #61dafb;
            color: #1e1e1e;
            border-color: #61dafb;
        }}
        .tab-button {{
            padding: 12px 30px;
            background: #2d2d30;
            border: 2px solid #3c3c3c;
            color: #d4d4d4;
            cursor: pointer;
            border-radius: 6px;
            font-size: 16px;
            font-weight: bold;
            transition: all 0.3s;
        }}

        .tab-button:hover {{
            background: #3c3c3c;
            border-color: #61dafb;
        }}

        .tab-button.active {{
            background: #61dafb;
            color: #1e1e1e;
            border-color: #61dafb;
        }}

        .container {{
            display: flex;
            gap: 20px;
        }}

        .memory-map {{
            flex: 1;
            background: #252526;
            border-radius: 8px;
            padding: 20px;
        }}

        .memory-view {{
            display: none;
        }}

        .memory-view.active {{
            display: block;
        }}

        .legend {{
            width: 300px;
            min-width: 300px;
            background: #252526;
            border-radius: 8px;
            padding: 20px;
            height: fit-content;
            position: sticky;
            top: 20px;
        }}

        .legend h3 {{ margin-bottom: 15px; color: #61dafb; }}
        .legend-item {{
            display: flex;
            align-items: center;
            margin-bottom: 10px;
            padding: 8px;
            border-radius: 4px;
            cursor: pointer;
        }}
        .legend-item:hover {{ background: #2d2d30; }}
        .legend-color {{
            width: 20px;
            height: 20px;
            margin-right: 10px;
            border-radius: 3px;
        }}

        .module-stats {{
            margin-top: 20px;
            max-height: 400px;
            overflow-y: auto;
        }}

        .module-item {{
            padding: 8px;
            margin-bottom: 8px;
            background: #2d2d30;
            border-radius: 4px;
            border-left: 3px solid #61dafb;
        }}

        .module-name {{ font-weight: bold; margin-bottom: 5px; }}
        .module-size {{ font-size: 12px; color: #858585; }}

        .memory-blocks {{
            margin-top: 20px;
            min-height: 200px;
            border: 1px solid #3c3c3c;
            padding: 0;
        }}

        .memory-row {{
            display: flex;
            width: 100%;
            height: 20px;
            flex-wrap: nowrap;
        }}

        .memory-pixel {{
            height: 20px;
            cursor: pointer;
            transition: all 0.2s;
            position: relative;
            flex-grow: 1;
            flex-shrink: 0;
            min-width: 1px;
        }}

        .memory-delimiter {{
            height: 20px;
            flex-grow: 0;
            flex-shrink: 0;
            width: 1px;
            background: #000000;
        }}

        .memory-pixel:hover {{
            transform: scaleY(1.2);
            z-index: 100;
            box-shadow: 0 0 10px rgba(97, 218, 251, 0.5);
        }}

        .tooltip {{
            position: fixed;
            background: #3c3c3c;
            border: 1px solid #61dafb;
            padding: 10px;
            border-radius: 4px;
            pointer-events: none;
            z-index: 1000;
            max-width: 400px;
            display: none;
            box-shadow: 0 4px 6px rgba(0,0,0,0.3);
        }}

        .tooltip-title {{ font-weight: bold; margin-bottom: 5px; color: #61dafb; word-break: break-all; }}
        .tooltip-info {{ font-size: 12px; line-height: 1.4; }}

        .details-panel {{
            position: fixed;
            right: -500px;
            top: 0;
            width: 500px;
            height: 100%;
            background: #252526;
            box-shadow: -2px 0 10px rgba(0,0,0,0.5);
            transition: right 0.3s;
            padding: 20px;
            overflow-y: auto;
            z-index: 2000;
        }}

        .details-panel.open {{ right: 0; }}

        .close-btn {{
            float: right;
            background: #d73a49;
            color: white;
            border: none;
            padding: 8px 16px;
            border-radius: 4px;
            cursor: pointer;
        }}

        .close-btn:hover {{ background: #cb2431; }}

        ::-webkit-scrollbar {{ width: 8px; }}
        ::-webkit-scrollbar-track {{ background: #1e1e1e; }}
        ::-webkit-scrollbar-thumb {{ background: #424242; border-radius: 4px; }}
        ::-webkit-scrollbar-thumb:hover {{ background: #4e4e4e; }}
    </style>
</head>
<body>
    <div class="header">
        <h1>Marlin Firmware Memory Map</h1>
        <div class="stats">
            <div class="stat">
                <div class="stat-value">{total_ram:,}</div>
                <div class="stat-label">RAM Used ({100.0 * total_ram / RAM_SIZE:.1f}% of {RAM_SIZE:,})</div>
            </div>
            <div class="stat">
                <div class="stat-value">{total_flash:,}</div>
                <div class="stat-label">Flash Used ({100.0 * total_flash / FLASH_SIZE:.1f}% of {FLASH_SIZE:,})</div>
            </div>
            <div class="stat">
                <div class="stat-value">{len(symbols):,}</div>
                <div class="stat-label">Symbols</div>
            </div>
        </div>
    </div>

    <div class="memory-tabs">
        <button class="tab-button active" onclick="showMemoryType('ram')">RAM ({total_ram:,} / {RAM_SIZE:,} bytes)</button>
        <button class="tab-button" onclick="showMemoryType('flash')">Flash Memory ({total_flash:,} / {FLASH_SIZE:,} bytes)</button>
        <span class="zoom-label">Zoom:</span>
        <button class="zoom-button active" onclick="setZoom(1)">1x</button>
        <button class="zoom-button" onclick="setZoom(2)">2x</button>
        <button class="zoom-button" onclick="setZoom(4)">4x</button>
        <button class="zoom-button" onclick="setZoom(8)">8x</button>
    </div>

    <div class="container">
        <div class="memory-map">
            <div id="flash-view" class="memory-view">
                <h3>Flash Memory Layout (hover for details)</h3>
                <div class="memory-blocks zoom-1" id="flashBlocks-1" style="display: block;">
{generate_memory_blocks_html(flash_items, module_colors, 1, FLASH_SIZE, None, arch)}                </div>
                <div class="memory-blocks zoom-2" id="flashBlocks-2" style="display: none;">
{generate_memory_blocks_html(flash_items, module_colors, 2, FLASH_SIZE, None, arch)}                </div>
                <div class="memory-blocks zoom-4" id="flashBlocks-4" style="display: none;">
{generate_memory_blocks_html(flash_items, module_colors, 4, FLASH_SIZE, None, arch)}                </div>
                <div class="memory-blocks zoom-8" id="flashBlocks-8" style="display: none;">
{generate_memory_blocks_html(flash_items, module_colors, 8, FLASH_SIZE, None, arch)}                </div>
            </div>

            <div id="ram-view" class="memory-view active">
                <h3>RAM Layout (hover for details)</h3>
                <div class="memory-blocks zoom-1" id="ramBlocks-1" style="display: block;">
{generate_memory_blocks_html(ram_items, module_colors, 1, RAM_SIZE, special_symbols, arch)}                </div>
                <div class="memory-blocks zoom-2" id="ramBlocks-2" style="display: none;">
{generate_memory_blocks_html(ram_items, module_colors, 2, RAM_SIZE, special_symbols, arch)}                </div>
                <div class="memory-blocks zoom-4" id="ramBlocks-4" style="display: none;">
{generate_memory_blocks_html(ram_items, module_colors, 4, RAM_SIZE, special_symbols, arch)}                </div>
                <div class="memory-blocks zoom-8" id="ramBlocks-8" style="display: none;">
{generate_memory_blocks_html(ram_items, module_colors, 8, RAM_SIZE, special_symbols, arch)}                </div>
            </div>
        </div>

        <div class="legend">
            <h3>Modules</h3>
"""

    # Add module legend
    for module, color in sorted(module_colors.items()):
        flash_size = module_stats[module]['flash']
        ram_size = module_stats[module]['ram']
        if flash_size > 0 or ram_size > 0:
            html += f'''            <div class="legend-item">
                <div class="legend-color" style="background: {color};"></div>
                <span>{module}</span>
            </div>
'''

    html += """
            <div class="module-stats">
                <h3 style="margin-top: 20px; margin-bottom: 10px;">Module Statistics</h3>
"""

    # Add module statistics sorted by total size
    for module, stats in sorted(module_stats.items(), key=lambda x: x[1]['flash'] + x[1]['ram'], reverse=True):
        if stats['flash'] > 0 or stats['ram'] > 0:
            html += f'''                <div class="module-item">
                    <div class="module-name">{module}</div>
                    <div class="module-size">Flash: {stats['flash']:,} bytes | RAM: {stats['ram']:,} bytes</div>
                </div>
'''

    html += """            </div>
            <div class="section-table-container" style="margin-top: 20px;">
                <h3 style="margin-bottom: 10px;">Section Map</h3>
                <table class="section-table" style="width: 100%; font-size: 12px; border-collapse: collapse;">
                    <thead>
                        <tr style="background: #2d2d30; border-bottom: 2px solid #61dafb;">
                            <th style="padding: 8px; text-align: left;">Section</th>
                            <th style="padding: 8px; text-align: right;">Start</th>
                            <th style="padding: 8px; text-align: right;">End</th>
                            <th style="padding: 8px; text-align: right;">Size</th>
                        </tr>
                    </thead>
                    <tbody>
"""

    # Add section rows sorted by start address
    for section, info in sorted(section_ranges.items(), key=lambda x: x[1]['start']):
        size = info['end'] - info['start']
        mem_type = info['type'].upper()
        html += f'''                        <tr style="border-bottom: 1px solid #3c3c3c;">
                            <td style="padding: 6px; font-weight: bold;">{section}</td>
                            <td style="padding: 6px; text-align: right; font-family: monospace;">0x{info['start']:08x}</td>
                            <td style="padding: 6px; text-align: right; font-family: monospace;">0x{info['end']:08x}</td>
                            <td style="padding: 6px; text-align: right;">{size:,}</td>
                        </tr>
'''

    html += """                    </tbody>
                </table>
            </div>        </div>
    </div>
"""

    # Add memory list tables if requested
    if show_list:
        # Sort items by address
        flash_items_sorted = sorted(flash_items, key=lambda x: x['addr'])
        ram_items_sorted = sorted(ram_items, key=lambda x: x['addr'])

        # Calculate totals for percentages
        total_flash_used = sum(item['size'] for item in flash_items_sorted)
        total_ram_used = sum(item['size'] for item in ram_items_sorted)

        html += """
    <div class="memory-list-container" style="margin-top: 40px; padding: 20px; background: #252526; border-radius: 8px;">
        <h2 style="color: #61dafb; margin-bottom: 20px;">Memory Usage Details</h2>

        <div class="list-tabs" style="display: flex; gap: 10px; margin-bottom: 20px;">
            <button class="list-tab-button active" onclick="showListTab('ram-list')" style="padding: 10px 20px; background: #61dafb; color: #1e1e1e; border: none; border-radius: 4px; cursor: pointer; font-weight: bold;">RAM</button>
            <button class="list-tab-button" onclick="showListTab('flash-list')" style="padding: 10px 20px; background: #2d2d30; color: #d4d4d4; border: 2px solid #3c3c3c; border-radius: 4px; cursor: pointer; font-weight: bold;">Flash Memory</button>
        </div>

        <div id="ram-list" class="list-view" style="display: block;">
            <h3 style="color: #61dafb; margin-bottom: 15px;">RAM Symbols (""" + f"{len(ram_items_sorted):,}" + """ entries)</h3>
            <div style="max-height: 600px; overflow-y: auto;">
                <table id="ram-table" style="width: 100%; border-collapse: collapse; font-size: 12px;">
                    <thead style="position: sticky; top: 0; background: #2d2d30; z-index: 10;">
                        <tr style="border-bottom: 2px solid #61dafb;">
                            <th style="padding: 10px; text-align: left; color: #61dafb; cursor: pointer; user-select: none; width: 120px;" onclick="sortTable('ram-table', 0)">
                                Address <span style="font-size: 10px;">▲▼</span>
                            </th>
                            <th style="padding: 10px; text-align: right; color: #61dafb; cursor: pointer; user-select: none; width: 100px; white-space: nowrap;" onclick="sortTable('ram-table', 1)">
                                Size (bytes) <span style="font-size: 10px;">▲▼</span>
                            </th>
                            <th style="padding: 10px; text-align: right; color: #61dafb; cursor: pointer; user-select: none; width: 100px; white-space: nowrap;" onclick="sortTable('ram-table', 2)">
                                Percentage <span style="font-size: 10px;">▲▼</span>
                            </th>
                            <th style="padding: 10px; text-align: left; color: #61dafb; cursor: pointer; user-select: none;" onclick="sortTable('ram-table', 3)">
                                Name <span style="font-size: 10px;">▲▼</span>
                            </th>
                        </tr>
                    </thead>
                    <tbody>
"""

        for item in ram_items_sorted:
            # For AVR use normalized address, for ARM use raw address
            addr = item['addr_raw'] if arch == 'arm' else item['addr']
            size = item['size']
            percentage = (size / total_ram_used * 100) if total_ram_used > 0 else 0
            name = item['name'].replace('&', '&amp;').replace('<', '&lt;').replace('>', '&gt;').replace('"', '&quot;')
            html += f'''                        <tr style="border-bottom: 1px solid #3c3c3c;">
                            <td style="padding: 8px; font-family: monospace;">0x{addr:08x}</td>
                            <td style="padding: 8px; text-align: right;">{size:,}</td>
                            <td style="padding: 8px; text-align: right;">{percentage:.3f}%</td>
                            <td style="padding: 8px; word-break: break-all;">{name}</td>
                        </tr>
'''

        html += """                    </tbody>
                </table>
            </div>
        </div>

        <div id="flash-list" class="list-view" style="display: none;">
            <h3 style="color: #61dafb; margin-bottom: 15px;">Flash Memory Symbols (""" + f"{len(flash_items_sorted):,}" + """ entries)</h3>
            <div style="max-height: 600px; overflow-y: auto;">
                <table id="flash-table" style="width: 100%; border-collapse: collapse; font-size: 12px;">
                    <thead style="position: sticky; top: 0; background: #2d2d30; z-index: 10;">
                        <tr style="border-bottom: 2px solid #61dafb;">
                            <th style="padding: 10px; text-align: left; color: #61dafb; cursor: pointer; user-select: none; width: 120px;" onclick="sortTable('flash-table', 0)">
                                Address <span style="font-size: 10px;">▲▼</span>
                            </th>
                            <th style="padding: 10px; text-align: right; color: #61dafb; cursor: pointer; user-select: none; width: 100px; white-space: nowrap;" onclick="sortTable('flash-table', 1)">
                                Size (bytes) <span style="font-size: 10px;">▲▼</span>
                            </th>
                            <th style="padding: 10px; text-align: right; color: #61dafb; cursor: pointer; user-select: none; width: 100px; white-space: nowrap;" onclick="sortTable('flash-table', 2)">
                                Percentage <span style="font-size: 10px;">▲▼</span>
                            </th>
                            <th style="padding: 10px; text-align: left; color: #61dafb; cursor: pointer; user-select: none;" onclick="sortTable('flash-table', 3)">
                                Name <span style="font-size: 10px;">▲▼</span>
                            </th>
                        </tr>
                    </thead>
                    <tbody>
"""

        for item in flash_items_sorted:
            # For AVR use normalized address, for ARM use raw address
            addr = item['addr_raw'] if arch == 'arm' else item['addr']
            size = item['size']
            percentage = (size / total_flash_used * 100) if total_flash_used > 0 else 0
            name = item['name'].replace('&', '&amp;').replace('<', '&lt;').replace('>', '&gt;').replace('"', '&quot;')
            html += f'''                        <tr style="border-bottom: 1px solid #3c3c3c;">
                            <td style="padding: 8px; font-family: monospace;">0x{addr:08x}</td>
                            <td style="padding: 8px; text-align: right;">{size:,}</td>
                            <td style="padding: 8px; text-align: right;">{percentage:.3f}%</td>
                            <td style="padding: 8px; word-break: break-all;">{name}</td>
                        </tr>
'''

        html += """                    </tbody>
                </table>
            </div>
        </div>
    </div>
"""

    html += """
    <div class="tooltip" id="tooltip">
        <div class="tooltip-title" id="tooltipTitle"></div>
        <div class="tooltip-info" id="tooltipInfo"></div>
    </div>

    <script>
        const tooltip = document.getElementById('tooltip');
        const totalFlash = """ + str(total_flash) + """;
        const totalRam = """ + str(total_ram) + """;

        function setupBlockHandlers() {
            document.querySelectorAll('.memory-pixel').forEach(block => {
                block.addEventListener('mouseenter', (e) => {
                    if (block.dataset.unused === 'true') {
                        const addr = block.dataset.addr;
                        const size = block.dataset.size;

                        document.getElementById('tooltipTitle').textContent = 'Unused Memory';
                        document.getElementById('tooltipInfo').innerHTML = `
                            Address: ${addr}<br>
                            Size: ${parseInt(size).toLocaleString()} bytes
                        `;

                        tooltip.style.display = 'block';
                        return;
                    }

                    if (block.dataset.unallocated === 'true') {
                        const addr = block.dataset.addr;
                        const size = block.dataset.size;

                        document.getElementById('tooltipTitle').textContent = 'Linker Code/Padding';
                        document.getElementById('tooltipInfo').innerHTML = `
                            Address: ${addr}<br>
                            Size: ${parseInt(size).toLocaleString()} bytes
                        `;

                        tooltip.style.display = 'block';
                        return;
                    }

                    const name = block.dataset.name;
                    if (!name) return;

                    const addr = block.dataset.addr;
                    const size = block.dataset.size;
                    const section = block.dataset.section;
                    const module = block.dataset.module;

                    const isFlash = section === '.text' || section === '.rodata';
                    const percentage = ((parseInt(size) / (isFlash ? totalFlash : totalRam)) * 100).toFixed(3);

                    document.getElementById('tooltipTitle').textContent = name;
                    document.getElementById('tooltipInfo').innerHTML = `
                        Address: ${addr}<br>
                        Size: ${parseInt(size).toLocaleString()} bytes<br>
                        Section: ${section}<br>
                        Module: ${module}<br>
                        Percentage: ${percentage}% of ${isFlash ? 'Flash' : 'RAM'}
                    `;

                    tooltip.style.display = 'block';
                });

                block.addEventListener('mousemove', (e) => {
                    tooltip.style.left = (e.clientX + 15) + 'px';
                    tooltip.style.top = (e.clientY + 15) + 'px';
                });

                block.addEventListener('mouseleave', () => {
                    tooltip.style.display = 'none';
                });
            });
        }

        setupBlockHandlers();

        function showMemoryType(type) {
            // Update tab buttons
            document.querySelectorAll('.tab-button').forEach(btn => {
                btn.classList.remove('active');
            });
            event.target.classList.add('active');

            // Update views
            document.querySelectorAll('.memory-view').forEach(view => {
                view.classList.remove('active');
            });
            document.getElementById(type + '-view').classList.add('active');
        }

        function setZoom(level) {
            // Update zoom buttons
            document.querySelectorAll('.zoom-button').forEach(btn => {
                btn.classList.remove('active');
            });
            event.target.classList.add('active');

            // Hide all zoom levels
            document.querySelectorAll('.memory-blocks').forEach(blocks => {
                blocks.style.display = 'none';
            });

            // Show selected zoom level
            document.querySelectorAll('.zoom-' + level).forEach(blocks => {
                blocks.style.display = 'block';
            });

            // Re-setup event handlers for new blocks
            setupBlockHandlers();
        }

        function showListTab(tabId) {
            // Update tab buttons
            document.querySelectorAll('.list-tab-button').forEach(btn => {
                btn.classList.remove('active');
                btn.style.background = '#2d2d30';
                btn.style.color = '#d4d4d4';
                btn.style.border = '2px solid #3c3c3c';
            });
            event.target.classList.add('active');
            event.target.style.background = '#61dafb';
            event.target.style.color = '#1e1e1e';
            event.target.style.border = 'none';

            // Update views
            document.querySelectorAll('.list-view').forEach(view => {
                view.style.display = 'none';
            });
            document.getElementById(tabId).style.display = 'block';
        }

        // Table sorting functionality
        const sortState = {}; // Track sort direction for each table and column

        function sortTable(tableId, columnIndex) {
            const table = document.getElementById(tableId);
            const tbody = table.getElementsByTagName('tbody')[0];
            const rows = Array.from(tbody.getElementsByTagName('tr'));

            // Initialize sort state for this table/column if not exists
            const key = tableId + '-' + columnIndex;
            if (!sortState[key]) {
                sortState[key] = 'asc';
            }

            // Toggle sort direction
            const isAscending = sortState[key] === 'asc';
            sortState[key] = isAscending ? 'desc' : 'asc';

            // Sort rows
            rows.sort((a, b) => {
                const cellA = a.getElementsByTagName('td')[columnIndex];
                const cellB = b.getElementsByTagName('td')[columnIndex];
                let valA = cellA.textContent.trim();
                let valB = cellB.textContent.trim();

                // Handle different data types
                if (columnIndex === 0) {
                    // Address - parse hex
                    valA = parseInt(valA, 16);
                    valB = parseInt(valB, 16);
                } else if (columnIndex === 1) {
                    // Size - remove commas and parse as number
                    valA = parseInt(valA.replace(/,/g, ''));
                    valB = parseInt(valB.replace(/,/g, ''));
                } else if (columnIndex === 2) {
                    // Percentage - remove % and parse as float
                    valA = parseFloat(valA.replace('%', ''));
                    valB = parseFloat(valB.replace('%', ''));
                } else {
                    // Name - string comparison (case insensitive)
                    valA = valA.toLowerCase();
                    valB = valB.toLowerCase();
                }

                if (valA < valB) return isAscending ? -1 : 1;
                if (valA > valB) return isAscending ? 1 : -1;
                return 0;
            });

            // Rebuild tbody with sorted rows
            rows.forEach(row => tbody.appendChild(row));
        }
    </script>
</body>
</html>
"""

    with open(output_path, 'w') as f:
        f.write(html)

    print(f"✓ Generated memory visualization: {output_path}")
    print(f"  Total Flash: {total_flash:,} bytes")
    print(f"  Total RAM: {total_ram:,} bytes")
    print(f"  Symbols: {len(symbols):,}")
    print(f"  Flash symbols: {len(flash_items):,}")
    print(f"  RAM symbols: {len(ram_items):,}")

if __name__ == '__main__':
    parser = argparse.ArgumentParser(
        description='Generate interactive memory map visualization from AVR/ARM ELF file',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog='''Examples:
  # Use defaults (256KB Flash/8KB RAM for AVR, 512KB Flash/20KB RAM for ARM)
  %(prog)s firmware.elf output.html

  # ATmega1284 with 128KB Flash, 16KB RAM
  %(prog)s firmware.elf output.html --flash 128KB --ram 16KB

  # STM32F407 with 1MB Flash, 128KB RAM
  %(prog)s firmware.elf output.html --flash 1MB --ram 128KB

  # Check for pattern conflicts and ambiguous symbol matches
  %(prog)s firmware.elf output.html --flash 512KB --ram 20KB --warn-conflicts
'''
    )

    parser.add_argument('elf_file', type=Path,
                        help='Path to the ELF file to analyze')
    parser.add_argument('output_file', type=Path, nargs='?', default=Path('memory_map.html'),
                        help='Output HTML file path (default: memory_map.html)')
    parser.add_argument('--flash', type=parse_size, default=None,
                        help='Total Flash memory size (e.g., 256KB, 512KB, 262144 bytes) (default: 256KB for AVR, 512KB for ARM)')
    parser.add_argument('--ram', type=parse_size, default=None,
                        help='Total RAM size (e.g., 8KB, 20KB, 8192 bytes) (default: 8KB for AVR, 20KB for ARM)')
    parser.add_argument('--arch', choices=['avr', 'arm', 'auto'], default='auto',
                        help='Architecture (default: auto-detect)')
    parser.add_argument('--warn-conflicts', action='store_true',
                        help='Warn about symbols that match multiple category patterns')
    parser.add_argument('--csv', action='store_true',
                        help='Generate CSV files (flash.csv and ram.csv) with memory usage data')
    parser.add_argument('--list', action='store_true',
                        help='Display memory usage data in HTML tables below the visualization')

    args = parser.parse_args()

    if not args.elf_file.exists():
        print(f"Error: ELF file not found: {args.elf_file}")
        sys.exit(1)

    # Detect architecture
    if args.arch == 'auto':
        arch = detect_architecture(args.elf_file)
        print(f"Detected architecture: {arch.upper()}")
    else:
        arch = args.arch
        print(f"Using specified architecture: {arch.upper()}")

    # Set default sizes based on architecture if not specified
    if args.flash is None:
        args.flash = 256*1024 if arch == 'avr' else 512*1024  # ATmega2560: 256KB, STM32F103RE: 512KB
    if args.ram is None:
        args.ram = 8*1024 if arch == 'avr' else 20*1024  # ATmega2560: 8KB, STM32F103RE: 20KB

    print(f"Memory sizes: Flash={args.flash:,} bytes, RAM={args.ram:,} bytes")

    # Check for pattern conflicts if requested
    if args.warn_conflicts:
        num_conflicts = check_pattern_conflicts()
        if num_conflicts == 0:
            print("✓ No pattern conflicts detected")
        print()

    symbols, special_symbols = parse_nm_output(args.elf_file, arch)

    # Categorize with conflict warnings if requested
    if args.warn_conflicts:
        print("Checking for ambiguous symbol matches...")
        for symbol in symbols:
            symbol['module'] = categorize_symbol(symbol['name'], warn_conflicts=True)
        print()

    # Generate CSV files if requested
    if args.csv:
        output_dir = args.output_file.parent if args.output_file.parent != Path('.') else Path('.')
        generate_csv(symbols, args.flash, args.ram, output_dir, arch)
        print()

    generate_html(symbols, special_symbols, args.output_file, args.flash, args.ram, arch, args.list)
