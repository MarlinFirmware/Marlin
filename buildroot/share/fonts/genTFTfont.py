#!/usr/bin/env python3
'''
genTFTfont.py

Generate the "extra glyphs" fonts used by TFT Color UI for languages with
non-Latin scripts. The output is a C++ font file in the "hieroglyphs" format
(FONT_MARLIN_HIEROGLYPHS_1BPP / _2BPP) described in lcd/tft/tft_string.h:

  unifont_t {                   // 8 bytes, little endian
    uint8_t  format;            // 0xA1 = 1bpp, 0xA2 = 2bpp
    uint8_t  capitalAHeight;
    uint16_t fontStartEncoding; // First glyph unicode
    uint16_t fontEndEncoding;   // Last glyph unicode
    int8_t   fontAscent, fontDescent;
  }
  // Then for each glyph, sorted by unicode:
  uniglyph_t {
    uint16_t unicode;
    uint8_t  bbxWidth, bbxHeight, dataSize;
    int8_t   dWidth, bbxOffsetX, bbxOffsetY;
  }
  uint8_t data[dataSize];       // Rows padded to whole bytes, MSB = leftmost pixel

Glyphs are rendered with FreeType using the same parameters as the original
font set (as verified byte-for-byte against the existing font files):
  - Unifont:  unifont-15.0.01.otf at 16px, 1bpp mono, pixel-doubled/tripled
              for the 20px/30px sizes.
  - NotoSans: 2bpp antialiased (8-bit coverage >> 6), default hinting, at the
              FreeType pixel size given as "pt" in each file's header comment.
              Fallback chain: NotoSans-Medium, NotoSansJP, NotoSansKR,
              NotoSansSC, NotoSansTC (first font that has the glyph wins).
  - The FreeType bitmap is trimmed of blank columns on the left and right and
    of blank rows on the top. Blank rows at the bottom are kept.

By default the glyphs already present in an existing font file are kept
verbatim and only new characters are rendered, so existing glyphs never change.
Use --rerender to render every glyph from the source fonts (e.g. to verify the
renderer against the files in the repository).

The header (license, includes, #if guard), header comment style, ascent,
descent, and capital 'A' height are taken from the existing file, which must
exist (use --template to start a new file from another file).

Requires: freetype-py  (pip install freetype-py)

Usage (from the Marlin repository root):

  # Add every character used by the language(s) using each category
  buildroot/share/fonts/genTFTfont.py -c Korean
  buildroot/share/fonts/genTFTfont.py -c Korean Simplified_Chinese Traditional_Chinese Katakana

  # Only one family / size
  buildroot/share/fonts/genTFTfont.py -c Korean -f NotoSans -s 14 19

  # Add specific characters
  buildroot/share/fonts/genTFTfont.py -c Korean --chars '각간'

  # Verify the renderer: re-render all glyphs and compare to the existing files
  buildroot/share/fonts/genTFTfont.py -c Korean --rerender --check

Afterwards update EXTRA_GLYPHS for the category in lcd/tft/fontdata/fontdata.h
(the script prints the required value, or use --update-fontdata).
'''

import argparse, re, sys
from pathlib import Path

SCRIPTDIR = Path(__file__).resolve().parent
REPO      = SCRIPTDIR.parents[2]
FONTDATA  = REPO / 'Marlin/src/lcd/tft/fontdata'

sys.path.insert(0, str(REPO / 'buildroot/share/scripts'))

# Family configuration
#   sizes: Marlin font size -> (FreeType pixel size, integer scale factor)
FAMILIES = {
  'Unifont': {
    'bpp': 1,
    'fonts': ('Unifont/unifont-15.0.01.otf', 'Unifont/unifont_upper-15.0.01.otf'),
    'sizes': { 10:(16, 1), 20:(16, 2), 30:(16, 3) },
    'path': lambda cat, sz: FONTDATA / f'Unifont/{sz}px/Unifont_{cat}_{sz}.cpp',
  },
  'NotoSans': {
    'bpp': 2,
    'fonts': ('NotoSans/NotoSans-Medium.ttf', 'NotoSans/NotoSansJP-Medium.otf', 'NotoSans/NotoSansKR-Medium.otf',
              'NotoSans/NotoSansSC-Medium.otf', 'NotoSans/NotoSansTC-Medium.otf'),
    'sizes': { 14:(19, 1), 16:(22, 1), 19:(26, 1), 26:(36, 1), 27:(37, 1), 28:(38, 1), 29:(40, 1) },
    'path': lambda cat, sz: FONTDATA / f'NotoSans/Medium_{sz}px/NotoSans_Medium_{cat}_{sz}.cpp',
  },
}

# Categories in the "hieroglyphs" format (glyphs carry their own unicode)
CATEGORIES = ('Katakana', 'Korean', 'Simplified_Chinese', 'Traditional_Chinese')

def s8(v): return v - 256 if v > 127 else v
def u8(v): return v & 0xFF

#
# Parsing of existing font files
#
_ARRAY_RE = re.compile(r'^(?P<pre>.*?)^(?P<hcom>//[^\n]*)\n(?P<ext>extern const uint8_t (?P<name>\w+)\[)(?P<size>\d+)(?P<ext2>\] = \{\n)(?P<body>.*?\n)\};\n(?P<post>.*)$', re.S | re.M)

class FontFile:
  def __init__(self, path):
    self.path = Path(path)
    text = self.path.read_text(encoding='utf-8')
    m = _ARRAY_RE.match(text)
    if not m: raise ValueError(f'{path}: unrecognized font file layout')
    self.m = m
    nums = [int(x) for x in re.findall(r'-?\d+', re.sub(r'//[^\n]*', '', m['body']))]
    self.header = nums[:8]
    self.format = nums[0]
    if self.format & 0xF0 != 0xA0: raise ValueError(f'{path}: not a hieroglyphs-format font (0x{self.format:02X})')
    self.bpp = self.format & 0x0F
    self.glyphs = {}
    data, i = nums[8:], 0
    while i < len(data):
      u = data[i] | (data[i+1] << 8)
      w, h = data[i+2], data[i+3]
      n = ((w * self.bpp + 7) // 8) * h       # Real data size (the dataSize byte may have wrapped)
      self.glyphs[u] = data[i+2:i+8+n]        # w,h,ds,dw,ox,oy + bitmap (raw bytes)
      i += 8 + n
    if len(self.glyphs) and max(self.glyphs) != (self.header[4] | self.header[5] << 8):
      print(f'warning: {path}: fontEndEncoding does not match the last glyph', file=sys.stderr)

#
# Rendering
#
class Renderer:
  def __init__(self, family, size):
    import freetype
    self.ft = freetype
    cfg = FAMILIES[family]
    self.bpp = cfg['bpp']
    self.px, self.scale = cfg['sizes'][size]
    self.faces = []
    for f in cfg['fonts']:
      p = FONTDATA / f
      if p.exists():
        face = freetype.Face(str(p))
        face.set_pixel_sizes(0, self.px)
        self.faces.append((p.name, face))

  def face_for(self, ch):
    for name, face in self.faces:
      if face.get_char_index(ord(ch)): return name, face
    return None, None

  def render(self, ch):
    'Return the glyph as a list of bytes [w,h,ds,dw,ox,oy,data...] or None'
    ft = self.ft
    name, face = self.face_for(ch)
    if not face: return None
    flags = ft.FT_LOAD_RENDER | (ft.FT_LOAD_TARGET_MONO if self.bpp == 1 else 0)
    face.load_char(ch, flags)
    g = face.glyph; b = g.bitmap
    W, H = b.width, b.rows
    buf, pitch = b.buffer, b.pitch
    rows = []
    for y in range(H):
      if self.bpp == 1:
        rows.append([ (buf[y * pitch + x // 8] >> (7 - x % 8)) & 1 for x in range(W) ])
      else:
        rows.append([ buf[y * pitch + x] >> (8 - self.bpp) for x in range(W) ])

    dw = int(g.advance.x / 64)
    if W == 0 or H == 0 or not any(any(r) for r in rows):
      x0 = x1 = y0 = 0; y1 = -1; rows = []
      left = top = 0
    else:
      left, top = g.bitmap_left, g.bitmap_top
      cols = [ x for x in range(W) if any(r[x] for r in rows) ]
      x0, x1 = cols[0], cols[-1]
      y0 = next(y for y in range(H) if any(rows[y]))      # Trim top only
      y1 = H - 1
    px = [ r[x0:x1+1] for r in rows[y0:y1+1] ]
    w, h = (x1 - x0 + 1 if px else 0), len(px)
    ox, oy = left + x0, top - y0 - h

    sc = self.scale
    if sc > 1:
      px = [ [ v for v in r for _ in range(sc) ] for r in px for _ in range(sc) ]
      w, h, ox, oy, dw = w * sc, h * sc, ox * sc, oy * sc, dw * sc

    ppb = 8 // self.bpp
    data = []
    for r in px:
      for i in range(0, w, ppb):
        v = 0
        for k in range(ppb):
          if i + k < w: v |= r[i + k] << (8 - self.bpp - k * self.bpp)
        data.append(v)
    return [w, h, u8(len(data)), u8(dw), u8(ox), u8(oy)] + data

#
# Output
#
def emit(ff:FontFile, glyphs:dict, legacy_size=False):
  m = ff.m
  codes = sorted(glyphs)
  start, end = codes[0], codes[-1]
  hdr = list(ff.header)
  hdr[2:6] = [start & 0xFF, start >> 8, end & 0xFF, end >> 8]
  # Ascent and descent must cover every glyph. TFT_String uses the extents of all loaded fonts.
  tops = [ s8(glyphs[u][5]) + glyphs[u][1] for u in codes if glyphs[u][1] ]
  bots = [ s8(glyphs[u][5]) for u in codes if glyphs[u][1] ]
  if tops: hdr[6] = u8(max(s8(hdr[6]), max(tops)))
  if bots: hdr[7] = u8(min(s8(hdr[7]), min(bots)))
  size = 8 + sum(2 + len(glyphs[u]) for u in codes)   # The real array length
  # The original tool computed the size from the (wrapped) uint8 dataSize field
  if legacy_size: size = 8 + sum(8 + glyphs[u][2] for u in codes)

  hcom = m['hcom']
  hcom = re.sub(r'range: 0x[0-9a-fA-F]+-0x[0-9a-fA-F]+', f'range: 0x{start:04x}-0x{end:04x}', hcom)
  hcom = re.sub(r'glyphs: \d+', f'glyphs: {len(codes)}', hcom)

  out = [ m['pre'], hcom, '\n', m['ext'], str(size), m['ext2'] ]
  out.append('  ' + ','.join(str(v) for v in hdr) + ', // unifont_t\n')
  for u in codes:
    out.append(f'  // 0x{u:04x}  {chr(u)}\n')
    out.append('  ' + ','.join(str(v) for v in [u & 0xFF, u >> 8] + glyphs[u]) + ',\n')
  out += [ '};\n', m['post'] ]
  return ''.join(out), len(codes)

#
# Character sets
#
def language_chars(cat):
  'All characters > 0xFF used by the languages assigned to a TFT category'
  from languageUtil import LANGNAME, LangFile, SECTIONS, flat_text
  chars = set()
  for code, info in LANGNAME.items():
    if info.get('tft') != cat: continue
    lf = LangFile.load(code)
    chars |= { c for s in SECTIONS for e in lf.sections[s].values() for c in flat_text(e.flat) if ord(c) > 0xFF }
  # U+2026 '…' is remapped to the Symbols font by TFT_String::glyph()
  chars.discard('\u2026')
  return chars

def update_fontdata(cat, count):
  p = FONTDATA / 'fontdata.h'
  t = p.read_text(encoding='utf-8')
  t2, n = re.subn(rf'(#define FONT_EXTRA\s+{cat}\n\s*#define EXTRA_GLYPHS\s+)\d+', rf'\g<1>{count}', t)
  if not n: print(f'warning: EXTRA_GLYPHS for {cat} not found in fontdata.h', file=sys.stderr)
  elif t2 != t: p.write_text(t2, encoding='utf-8')

def main():
  ap = argparse.ArgumentParser(description='Generate TFT Color UI extra-glyph fonts', formatter_class=argparse.RawDescriptionHelpFormatter, epilog=__doc__.split('Usage', 1)[-1])
  ap.add_argument('-c', '--category', nargs='+', required=True, choices=CATEGORIES, help='script category (FONT_EXTRA)')
  ap.add_argument('-f', '--family', nargs='+', choices=FAMILIES.keys(), default=list(FAMILIES.keys()))
  ap.add_argument('-s', '--size', nargs='+', type=int, help='font size(s) (default: all sizes for the family)')
  ap.add_argument('--chars', default='', help='extra characters to include')
  ap.add_argument('--no-lang', action='store_true', help="don't add the characters used in the language files")
  ap.add_argument('--rerender', action='store_true', help='re-render all glyphs instead of keeping the existing glyph data')
  ap.add_argument('--check', action='store_true', help='compare with the existing file instead of writing it')
  ap.add_argument('--template', help='existing font file to use as a template if the target file does not exist')
  ap.add_argument('--legacy-size', action='store_true', help='compute the array size from the uint8 dataSize like the original tool (for --check)')
  ap.add_argument('--update-fontdata', action='store_true', help='update EXTRA_GLYPHS in fontdata.h')
  args = ap.parse_args()

  status = 0
  for cat in args.category:
    want = set(args.chars)
    if not args.no_lang: want |= language_chars(cat)
    counts = set()
    for fam in args.family:
      cfg = FAMILIES[fam]
      for sz in (args.size or cfg['sizes'].keys()):
        if sz not in cfg['sizes']: continue
        path = cfg['path'](cat, sz)
        ff = FontFile(path if path.exists() else args.template)
        if not path.exists(): ff.glyphs = {}
        rend = Renderer(fam, sz)

        codes = set(ff.glyphs) | { ord(c) for c in want }
        glyphs, added, missing = {}, [], []
        for u in sorted(codes):
          if u in ff.glyphs and not args.rerender:
            glyphs[u] = ff.glyphs[u]
          else:
            g = rend.render(chr(u))
            if g is None:
              missing.append(chr(u))
              if u in ff.glyphs: glyphs[u] = ff.glyphs[u]
              continue
            glyphs[u] = g
            if u not in ff.glyphs: added.append(chr(u))

        text, count = emit(ff, glyphs, args.legacy_size)
        counts.add(count)
        rel = path.relative_to(REPO)
        if missing: print(f'{rel}: no glyph in source fonts for: {"".join(missing)}', file=sys.stderr)
        if args.check:
          old = path.read_text(encoding='utf-8')
          if text == old: print(f'{rel}: identical ({count} glyphs)')
          else:
            status = 1
            diff = [ chr(u) for u in glyphs if u in ff.glyphs and glyphs[u] != ff.glyphs[u] ]
            print(f'{rel}: DIFFERS ({count} glyphs, {len(added)} new, {len(diff)} changed: {"".join(diff)})')
        else:
          path.write_text(text, encoding='utf-8')
          print(f'{rel}: {count} glyphs (+{len(added)}) {"".join(added)}')

    if len(counts) > 1: print(f'warning: {cat} glyph counts differ between files: {sorted(counts)}', file=sys.stderr)
    if counts:
      n = max(counts)
      print(f'{cat}: EXTRA_GLYPHS {n}')
      if args.update_fontdata and not args.check: update_fontdata(cat, n)
  return status

if __name__ == '__main__':
  sys.exit(main())
