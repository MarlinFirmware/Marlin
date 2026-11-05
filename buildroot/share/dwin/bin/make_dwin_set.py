#!/usr/bin/env python3
#
# make_dwin_set.py
#
# Build Marlin's unified DWIN_SET for Ender-3 V2 family DWIN displays
# from the sources in buildroot/share/dwin, as described in dwin_set.toml.
#
#  Marlin 3D Printer Firmware
#  Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <https://www.gnu.org/licenses/>.
#
# Usage:
#   make_dwin_set.py [--render] [--out DIR] [--compare OLD.tar.gz]
#
#   --render    Re-render the icons listed in [render] from icons-svg/
#               (needs rsvg-convert) before building.
#   --out DIR   Output folder (default: buildroot/share/dwin/build).
#   --compare   Report differences against an existing DWIN_SET tarball.
#
# Output: DIR/DWIN_SET/ (the folder to copy to the display's SD card)
#         DIR/DWIN_SET.tar.gz (reproducible: same sources give the same bytes)
#
# Requires Python 3.11+ and Pillow.
#

import argparse, gzip, io, os, re, shutil, struct, subprocess, sys, tarfile, tempfile
import xml.etree.ElementTree as ET
from typing import NoReturn

try:
    import tomllib
except ImportError:
    sys.exit("Error: Python 3.11 or newer is required (tomllib)")

try:
    from PIL import Image
except ImportError:
    sys.exit("Error: Pillow is required (python3 -m pip install pillow)")

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import DWIN_ICO

ROOT = os.path.dirname(HERE)    # buildroot/share/dwin
BANK = 32 * 1024                # Each numbered file starts at a 32KB bank

def fail(msg) -> NoReturn:
    print("Error:", msg)
    sys.exit(1)

def render_icons(cfg):
    """Render SVG icons to baseline JPEGs at exact pixel sizes."""
    rsvg = shutil.which('rsvg-convert')
    if not rsvg: fail("--render needs rsvg-convert (librsvg)")
    ET.register_namespace('', 'http://www.w3.org/2000/svg')
    for item in cfg.get('render', {}).get('icons', []):
        src = os.path.join(ROOT, 'icons-svg', item['svg'])
        out = os.path.join(ROOT, item['out'])
        w, h = item['size']
        bg = item.get('bg', '#000000')
        with tempfile.TemporaryDirectory() as tmp:
            svgfile = src
            angle = item.get('rotate', 0)
            if angle:
                # Rotate the drawing about the center of the viewBox
                tree = ET.parse(src)
                svg = tree.getroot()
                vb = [float(v) for v in re.split(r'[ ,]+', svg.get('viewBox', f'0 0 {w} {h}').strip())]
                cx, cy = vb[0] + vb[2] / 2, vb[1] + vb[3] / 2
                g = ET.Element('{http://www.w3.org/2000/svg}g', {'transform': f'rotate({angle} {cx} {cy})'})
                for child in list(svg):
                    if child.tag.endswith('defs') or child.tag.endswith('metadata'): continue
                    svg.remove(child); g.append(child)
                svg.append(g)
                svgfile = os.path.join(tmp, 'rot.svg')
                tree.write(svgfile)
            png = os.path.join(tmp, 'out.png')
            subprocess.run([rsvg, '-w', str(w), '-h', str(h), '-b', bg, svgfile, '-o', png], check=True)
            img = Image.open(png).convert('RGB')
            if img.size != (w, h): fail(f"{item['svg']} rendered at {img.size}, expected {(w, h)}")
            # Baseline JPEG, no subsampling, no metadata
            img.save(out, 'JPEG', quality=100, subsampling=0, optimize=True)
        print(f"  rendered {item['out']}")

def check_banks(files, nbanks):
    """Check that numbered files fit in flash and don't overlap.
    files: list of (name, size). Fonts (.HZK) and config files aren't in the image flash.
    """
    spans = []
    for name, size in files:
        m = re.match(r'^(\d+)', name)
        if not m or not re.search(r'\.(jpe?g|ico)$', name, re.I): continue
        first = int(m.group(1))
        last = first + (size - 1) // BANK
        spans.append((first, last, name, size))
        if last >= nbanks: fail(f"{name} ({size} bytes) runs past bank {nbanks - 1}")
    spans.sort()
    ok = True
    for i, (f1, l1, n1, s1) in enumerate(spans):
        for f2, l2, n2, s2 in spans[i+1:]:
            if f2 > l1: break
            kind1, kind2 = n1.lower().endswith('.ico'), n2.lower().endswith('.ico')
            if kind1 == kind2:
                print(f"  OVERLAP: {n1} (banks {f1}-{l1}) and {n2} (banks {f2}-{l2})")
                ok = False
            else:
                # The shipped set has 3_bg.jpg with 3.ICO. Report it, but don't fail.
                print(f"  note: {n1} and {n2} share bank {f2} (image vs icon file)")
    for f, l, n, s in spans:
        print(f"  banks {f:2d}-{l:2d}  {n:28s} {s:7d} bytes")
    if not ok: fail("bank overlap")

def ico_index(data):
    """Return {index: (w, h, jpeg bytes)} for an ICO file's contents."""
    out = {}
    for i in range(256):
        w, h, off, l3, l21 = struct.unpack('>HHLBH', data[i*16:i*16+11])
        n = l3 * 65536 + l21
        if n: out[i] = (w, h, data[off:off+n])
    return out

def compare(old_tgz, new_dir):
    print(f"\nComparing with {old_tgz}:")
    old = {}
    with tarfile.open(old_tgz) as tf:
        for m in tf.getmembers():
            f = tf.extractfile(m) if m.isfile() else None
            if f: old[os.path.basename(m.name)] = f.read()
    new = { n: open(os.path.join(new_dir, n), 'rb').read() for n in os.listdir(new_dir) }
    for n in sorted(set(old) | set(new)):
        if n not in new: print(f"  removed  {n}")
        elif n not in old: print(f"  added    {n}")
        elif old[n] == new[n]: print(f"  same     {n}")
        elif n.upper().endswith('.ICO'):
            a, b = ico_index(old[n]), ico_index(new[n])
            added = sorted(set(b) - set(a)); removed = sorted(set(a) - set(b))
            changed = sorted(i for i in set(a) & set(b) if a[i] != b[i])
            print(f"  changed  {n}: icons added {added or '-'}, removed {removed or '-'}, changed {changed or '-'}")
        else: print(f"  changed  {n}")

def make_tarball(src_dir, setname, tgz):
    """Write a reproducible tar.gz (sorted names, fixed times and owners)."""
    buf = io.BytesIO()
    with tarfile.open(fileobj=buf, mode='w', format=tarfile.PAX_FORMAT) as tf:
        def add(path, arcname, isdir):
            ti = tarfile.TarInfo(arcname)
            ti.mtime = 0; ti.uid = ti.gid = 0; ti.uname = ti.gname = ''
            if isdir:
                ti.type = tarfile.DIRTYPE; ti.mode = 0o755
                tf.addfile(ti)
            else:
                ti.mode = 0o644; ti.size = os.path.getsize(path)
                with open(path, 'rb') as f: tf.addfile(ti, f)
        add(src_dir, setname, True)
        for n in sorted(os.listdir(src_dir)):
            add(os.path.join(src_dir, n), f'{setname}/{n}', False)
    with open(tgz, 'wb') as f:
        with gzip.GzipFile(filename='', mode='wb', fileobj=f, mtime=0, compresslevel=9) as gz:
            gz.write(buf.getvalue())

def main():
    ap = argparse.ArgumentParser(description="Build Marlin's unified DWIN_SET")
    ap.add_argument('--render', action='store_true', help='re-render SVG icons first (needs rsvg-convert)')
    ap.add_argument('--out', default=os.path.join(ROOT, 'build'), help='output folder')
    ap.add_argument('--compare', metavar='TGZ', help='compare with an existing DWIN_SET tarball')
    args = ap.parse_args()

    with open(os.path.join(ROOT, 'dwin_set.toml'), 'rb') as f:
        cfg = tomllib.load(f)
    setname = cfg['set']['name']

    if args.render:
        print("Rendering icons from SVG...")
        render_icons(cfg)

    outset = os.path.join(args.out, setname)
    if os.path.exists(outset): shutil.rmtree(outset)
    os.makedirs(outset)

    print("Copying static files...")
    static = os.path.join(ROOT, cfg['set']['static'])
    for n in sorted(os.listdir(static)):
        if n.startswith('.'): continue
        shutil.copyfile(os.path.join(static, n), os.path.join(outset, n))

    print("Building icon files...")
    for ico in cfg['ico']:
        dest = os.path.join(outset, ico['file'])
        if os.path.exists(dest): fail(f"{ico['file']} is also in {cfg['set']['static']}/")
        try:
            DWIN_ICO.DWIN_ICO_File().createFile(os.path.join(ROOT, ico['icons']), dest)
        except Exception as e:
            fail(f"{ico['file']}: {e}")

    print("Checking flash banks...")
    files = [ (n, os.path.getsize(os.path.join(outset, n))) for n in sorted(os.listdir(outset)) ]
    check_banks(files, cfg['set'].get('banks', 16))

    tgz = os.path.join(args.out, setname + '.tar.gz')
    make_tarball(outset, setname, tgz)
    print(f"\nWrote {outset}/ and {tgz}")

    if args.compare: compare(args.compare, outset)

if __name__ == '__main__':
    main()
