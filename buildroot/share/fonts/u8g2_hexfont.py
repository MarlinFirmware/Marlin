#!/usr/bin/env python3
#
# u8g2_hexfont.py
#
# Convert the C string literal font data written by bdfconv into a
# hex byte array, 16 bytes per line. Other lines pass through.
#
# Usage: u8g2_hexfont.py font.h > font_hex.h
#
# License: BSD
#
import re, sys

ESCAPES = { 'a': 7, 'b': 8, 't': 9, 'n': 10, 'v': 11, 'f': 12, 'r': 13, '\\': 92, '"': 34, "'": 39, '?': 63 }

def decode(s):
    """Decode the body of one C string literal (without the quotes) into bytes."""
    out = bytearray()
    i = 0
    while i < len(s):
        c = s[i]
        if c != '\\':
            out.append(ord(c))
            i += 1
            continue
        i += 1
        m = re.match(r'[0-7]{1,3}', s[i:])
        if m:
            out.append(int(m.group(0), 8))
            i += len(m.group(0))
        elif s[i] in ESCAPES:
            out.append(ESCAPES[s[i]])
            i += 1
        else:
            sys.exit(f"Error: unsupported escape \\{s[i]}")
    return out

def convert(lines):
    out = []
    decl = None
    for line in lines:
        if decl is None:
            # Array declaration from bdfconv: const uint8_t name[N] U8G2_FONT_SECTION("name") =
            if re.match(r'^const uint8_t \w+\[\d+\].*=\s*$', line):
                decl, size, data = line.rstrip(), int(re.search(r'\[(\d+)\]', line).group(1)), bytearray()
            else:
                out.append(line)
            continue
        # One literal per line; the last line ends with ";"
        m = re.match(r'^\s*"(.*)"\s*(;?)\s*$', line)
        if not m: sys.exit(f"Error: unexpected line in font data: {line!r}")
        data += decode(m.group(1))
        if m.group(2):
            # The array size includes the string terminator
            if len(data) + 1 != size: sys.exit(f"Error: decoded {len(data)} bytes for [{size}]")
            out.append(decl + ' {\n')
            rows = [ ','.join(f'0x{b:02X}' for b in data[i:i+16]) for i in range(0, len(data), 16) ]
            out.append(',\n'.join('  ' + r for r in rows) + '\n')
            out.append('};\n')
            decl = None
    if decl is not None: sys.exit("Error: font data ended without ';'")
    return out

if __name__ == '__main__':
    if len(sys.argv) != 2: sys.exit("Usage: u8g2_hexfont.py font.h > font_hex.h")
    # Font files are plain ASCII except for the copyright comment, so pass bytes through as latin-1
    with open(sys.argv[1], encoding='latin-1', newline='') as f:
        sys.stdout.reconfigure(encoding='latin-1', newline='')
        sys.stdout.writelines(convert(f.readlines()))
