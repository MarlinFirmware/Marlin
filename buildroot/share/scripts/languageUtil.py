#!/usr/bin/env python3
'''
languageUtil.py

Shared support for the Marlin LCD language tools:

  languageCheck.py    Report missing strings and lint existing translations
  languageExport.py   Export language strings to CSV / JSON for translators
  languageImport.py   Merge CSV / JSON translations into the language files

The language files in Marlin/src/lcd/language are parsed into a simple model
(LangFile) that can be written back out. Entries that aren't modified are
written back verbatim so a load / save round trip doesn't produce spurious
changes.

Each string is identified by section ('narrow', 'wide', 'tall') and name.
  - The "Narrow" namespace holds the base strings.
  - The "Wide" namespace overrides strings for displays wider than 20 columns.
  - The "Tall" namespace overrides strings for displays with 4 or more lines.

"Flat" interchange format used in CSV / JSON:
  - Macros are written in parentheses, e.g. "Preheat (PREHEAT_1_LABEL)"
  - Multi-line strings begin with a bar and separate lines with bars.
    e.g., "|Wait for|filament load" is MSG_2_LINE("Wait for", "filament load")
  - Marlin substitution characters are kept as-is: $ @ ~ * {
'''

import re
from pathlib import Path
from collections import OrderedDict

LANGHOME = Path("Marlin/src/lcd/language")
TFTFONTHOME = Path("Marlin/src/lcd/tft/fontdata/Unifont/20px")
SECTIONS = ('narrow', 'wide', 'tall')

# Suggested maximum display widths (in characters) for linting
NARROW_MAX, WIDE_MAX, LINE_MAX = 19, 30, 20

#
# Language metadata
#   size   : CHARSIZE for the language
#   iso    : DISPLAY_CHARSET_ISO10646_* suffix (if any)
#   noext  : Uses NOT_EXTENDED_ISO10646_1_5X7 (no extended glyphs in the 5x7 font)
#   tft    : TFT extra-glyph script category (see lcd/tft/fontdata/fontdata.h)
#   parent : Language inherited from, if not English
#   derived: Generated from another language file
#
LANGNAME = {
  'an':      { 'size':1, 'iso': "1",     'name':"Aragonese", 'noext':1 },
  'bg':      { 'size':2, 'iso': "5",     'name':"Bulgarian", 'tft':"Cyrillic" },
  'ca':      { 'size':2,                 'name':"Catalan" },
  'cz':      { 'size':2, 'iso': "CZ",    'name':"Czech", 'tft':"Latin_Extended_A" },
  'da':      { 'size':2, 'iso': "1",     'name':"Danish" },
  'de':      { 'size':2,                 'name':"German" },
  'el':      { 'size':2, 'iso': "GREEK", 'name':"Greek", 'tft':"Greek" },
  'el_CY':   { 'size':2,                 'name':"Greek (Cyprus)", 'tft':"Greek", 'parent':'el' },
  'el_gr':   { 'size':2, 'iso': "GREEK", 'name':"Greek (Greece)", 'tft':"Greek" },
  'en':      { 'size':2,                 'name':"English" },
  'es':      { 'size':2,                 'name':"Spanish" },
  'eu':      { 'size':1, 'iso': "1",     'name':"Basque-Euskera", 'noext':1 },
  'fi':      { 'size':2, 'iso': "1",     'name':"Finnish" },
  'fr':      { 'size':2, 'iso': "1",     'name':"French" },
  'fr_na':   { 'size':1, 'iso': "1",     'name':"French (no accent)", 'noext':1, 'derived':'fr' },
  'gl':      { 'size':1, 'iso': "1",     'name':"Galician" },
  'hg':      { 'size':2, 'iso': "1",     'name':"Hinglish (Hindi-Latin)", 'noext':1 },
  'hr':      { 'size':2, 'iso': "1",     'name':"Croatian (Hrvatski)", 'tft':"Latin_Extended_A" },
  'hu':      { 'size':2,                 'name':"Hungarian / Magyar", 'tft':"Latin_Extended_A" },
  'id':      { 'size':2, 'iso': "1",     'name':"Indonesian", 'noext':1 },
  'it':      { 'size':1, 'iso': "1",     'name':"Italian" },
  'jp_kana': { 'size':3, 'iso': "KANA",  'name':"Japanese (Kana)", 'tft':"Katakana" },
  'ko_KR':   { 'size':1,                 'name':"Korean", 'tft':"Korean" },
  'nl':      { 'size':1, 'iso': "1",     'name':"Dutch", 'noext':1 },
  'pl':      { 'size':2, 'iso': "PL",    'name':"Polish", 'tft':"Latin_Extended_A" },
  'pt':      { 'size':2, 'iso': "1",     'name':"Portuguese" },
  'pt_br':   { 'size':2,                 'name':"Portuguese (Brazil)" },
  'ro':      { 'size':2,                 'name':"Romanian" },
  'ru':      { 'size':2, 'iso': "5",     'name':"Russian", 'tft':"Cyrillic" },
  'sk':      { 'size':2, 'iso': "SK",    'name':"Slovak", 'tft':"Latin_Extended_A" },
  'sv':      { 'size':2, 'iso': "1",     'name':"Swedish" },
  'tr':      { 'size':2, 'iso': "TR",    'name':"Turkish", 'tft':"Latin_Extended_A" },
  'uk':      { 'size':2, 'iso': "5",     'name':"Ukrainian", 'tft':"Cyrillic" },
  'vi':      { 'size':2,                 'name':"Vietnamese", 'tft':"Vietnamese" },
  'zh_CN':   { 'size':3,                 'name':"Simplified Chinese", 'tft':"Simplified_Chinese" },
  'zh_TW':   { 'size':3,                 'name':"Traditional Chinese", 'tft':"Traditional_Chinese" }
}

def infobyid(id, fld):
    if id in LANGNAME and fld in LANGNAME[id]:
        return LANGNAME[id][fld]
    return None

def language_name(id):       return infobyid(id, 'name') or '<unknown>'
def language_iso(id):        return infobyid(id, 'iso')
def language_charsize(id):   return infobyid(id, 'size')
def language_noext(id):      return infobyid(id, 'noext')
def language_tft(id):        return infobyid(id, 'tft')
def language_parent(id):     return infobyid(id, 'parent')
def language_derived(id):    return infobyid(id, 'derived')

# Strings that are never translated: symbols, technical abbreviations, macros
NEVER_TRANSLATE = (
  'LANGUAGE',
  'MSG_MARLIN', 'MSG_CUSTOM_MENU_MAIN_TITLE', 'MSG_TOOL_HEAD_TH',
  'MSG_PID_P', 'MSG_PID_P_E', 'MSG_PID_I', 'MSG_PID_I_E',
  'MSG_PID_D', 'MSG_PID_D_E', 'MSG_PID_C', 'MSG_PID_C_E',
  'MSG_PID_F', 'MSG_PID_F_E',
  'MSG_BACKLASH_N',
  'MSG_FTM_ZV', 'MSG_FTM_ZVD', 'MSG_FTM_ZVDD', 'MSG_FTM_ZVDDD',
  'MSG_FTM_EI', 'MSG_FTM_2HEI', 'MSG_FTM_3HEI', 'MSG_FTM_MZV'
)

#
# Tokenizer for the right-hand side of an LSTR definition
#   ('str', text) | ('id', name) | ('(',) | (')',) | (',',)
#
_TOKEN_RE = re.compile(r'"((?:[^"\\]|\\.)*)"|([A-Za-z_][A-Za-z0-9_]*)|([(),])|(\s+)')
_MACRO_RE = re.compile(r'\(([A-Z][A-Z0-9]*_[A-Z0-9_]+)\)')
_NLINE_RE = re.compile(r'MSG_\d_LINE$')

def tokenize(rhs:str):
    toks, pos = [], 0
    while pos < len(rhs):
        m = _TOKEN_RE.match(rhs, pos)
        if not m: raise ValueError(f"Can't parse: {rhs}")
        if m.group(1) is not None: toks.append(('str', m.group(1)))
        elif m.group(2): toks.append(('id', m.group(2)))
        elif m.group(3): toks.append((m.group(3),))
        pos = m.end()
    return toks

def unescape_c(s:str): return re.sub(r'\\(.)', r'\1', s)
def escape_c(s:str):   return s.replace('\\', '\\\\').replace('"', '\\"')

def rhs_to_flat(rhs:str):
    '''
    Convert the C expression of an LSTR into the flat interchange form.
    _UxGT() wrappers are dropped, macros become (MACRO),
    and MSG_n_LINE("a", "b") becomes |a|b
    '''
    out, lines = [], None
    for t in tokenize(rhs):
        if t[0] == 'id':
            name = t[1]
            if name == '_UxGT': pass
            elif _NLINE_RE.match(name): lines = []
            elif lines is not None: raise ValueError(f"Macro inside MSG_n_LINE: {rhs}")
            else: out.append(f'({name})')
        elif t[0] == 'str':
            (lines if lines is not None else out).append(unescape_c(t[1]))
    if lines is not None: return '|' + '|'.join(lines)
    return ''.join(out)

def flat_lines(flat:str):
    'The lines of a multi-line flat string, or None'
    return flat[1:].split('|') if flat.startswith('|') else None

def flat_to_rhs(flat:str):
    'Convert a flat interchange string into a C expression'
    lines = flat_lines(flat)
    if lines is not None:
        if not 1 <= len(lines) <= 3: raise ValueError(f"Multi-line strings need 1-3 lines: {flat}")
        inner = ', '.join(f'"{escape_c(s)}"' for s in lines)
        return f'_UxGT(MSG_{len(lines)}_LINE({inner}))'

    parts, pos = [], 0
    for m in _MACRO_RE.finditer(flat):
        if m.start() > pos: parts.append(('str', flat[pos:m.start()]))
        parts.append(('id', m.group(1)))
        pos = m.end()
    if pos < len(flat): parts.append(('str', flat[pos:]))
    if not parts: return '_UxGT("")'
    return ' '.join(f'_UxGT("{escape_c(v)}")' if k == 'str' else v for k, v in parts)

#
# Estimated display width of a flat string (longest line), with typical
# widths for substitutions and macros.
#
_SUBST_WIDTH = { '$':4, '@':1, '~':1, '*':2, '{':1 }
_MACRO_WIDTH = { 'MACHINE_NAME_SUBST':8, 'CUSTOM_MENU_MAIN_TITLE':8 }

def _macro_width(name):
    if name.startswith('PREHEAT_'): return 3
    if name.startswith('MEDIA_TYPE_'): return 5
    if name.startswith('LCD_STR_'): return 1
    return _MACRO_WIDTH.get(name, 4)

def flat_width(flat:str):
    lines = flat_lines(flat) or [ flat ]
    def w(s):
        s = _MACRO_RE.sub(lambda m: '\x00' * _macro_width(m.group(1)), s)
        return sum(_SUBST_WIDTH.get(c, 1) for c in s)
    return max(w(s) for s in lines)

def flat_placeholders(flat:str):
    'Marlin substitution characters and printf specifiers, for comparison'
    s = _MACRO_RE.sub('', flat)
    subs = ''.join(sorted(c for c in s if c in '$@~*{'))
    pfmt = sorted(re.findall(r'%[-+0#]*\d*(?:\.\d+)?[a-zA-Z]', s))
    return subs, pfmt

def flat_macros(flat:str):
    return sorted(_MACRO_RE.findall(flat))

def flat_text(flat:str):
    'The literal text of a flat string, without macros or line bars'
    return _MACRO_RE.sub('', flat).replace('|', '')

class LangEntry:
    'A single LSTR definition'
    def __init__(self, name, rhs, comment=None, raw=None):
        self.name = name
        self.rhs = rhs          # C expression, without the trailing ';'
        self.comment = comment  # Trailing comment text, without '//'
        self.raw = raw          # Original source line, if unmodified

    @property
    def flat(self): return rhs_to_flat(self.rhs)

    def set_flat(self, flat):
        if flat != self.flat:
            self.rhs, self.raw = flat_to_rhs(flat), None

    def format(self, section, en_flat=None):
        'Format as a line of code. Unmodified lines are kept verbatim if the comment is current.'
        if en_flat is not None: en_flat = en_flat.strip()
        if self.raw is not None and (en_flat is None or self.comment == en_flat):
            return self.raw
        indent, width = ('  ', 34) if section == 'narrow' else ('    ', 32)
        line = f'{indent}LSTR {self.name:<{width}} = {self.rhs};'
        comm = en_flat if en_flat is not None else self.comment
        if comm:
            # Align comments the same as languageImport always has
            line += ' ' * (95 - len(line) if len(line) < 95 else 1) + '// ' + comm
        return line

class LangFile:
    '''
    A parsed language_xx.h file
      preamble : Text before the first namespace (license, header comment, defines)
      charsize : The CHARSIZE value (or None)
      sections : { 'narrow': OrderedDict(name: LangEntry), 'wide': ..., 'tall': ... }
      flat_ns  : True for files with a single 'Language_xx' namespace (e.g., el_CY)
    '''
    def __init__(self, code, path=None):
        self.code = code
        self.path = Path(path) if path else LANGHOME / f'language_{code}.h'
        self.preamble = ''
        self.charsize = None
        self.sections = { s: OrderedDict() for s in SECTIONS }
        self.flat_ns = False
        self.inherit = 'Language_en'
        self.inherit_comment = ' // Inherit undefined strings from English'

    @classmethod
    def load(cls, code, path=None):
        lf = cls(code, path)
        lf.parse(lf.path.read_text(encoding='utf-8'))
        return lf

    def parse(self, text):
        lines = text.split('\n')
        first = next(i for i, l in enumerate(lines) if l.startswith('namespace '))
        self.preamble = '\n'.join(lines[:first])
        section = None
        for line in lines[first:]:
            m = re.match(r'namespace Language(Narrow|Wide|Tall)?_(\w+?) \{', line)
            if m:
                if m.group(1):
                    section = m.group(1).lower()
                elif m.group(2) == self.code and section is None:
                    self.flat_ns, section = True, 'narrow'
                else:
                    section = None
                continue
            m = re.match(r'\s*using namespace (\w+);(.*)$', line)
            if m:
                if section == 'narrow': self.inherit, self.inherit_comment = m.group(1), m.group(2)
                continue
            m = re.match(r'\s*constexpr uint8_t CHARSIZE\s*=\s*(\d+);', line)
            if m:
                self.charsize = int(m.group(1))
                continue
            m = re.match(r'\s*LSTR\s+(\w+)\s*=\s*(.*)$', line)
            if m:
                if section is None: raise ValueError(f"{self.path}: LSTR outside of a namespace: {line}")
                rhs, comment = split_statement(m.group(2))
                if m.group(1) in self.sections[section]:
                    raise ValueError(f"{self.path}: Duplicate {m.group(1)} in {section} section")
                self.sections[section][m.group(1)] = LangEntry(m.group(1), rhs, comment, line)

    def all_names(self):
        return { n for s in SECTIONS for n in self.sections[s] }

    def has(self, section, name):
        'Does this file override the given string for the given section?'
        if section == 'tall': return name in self.sections['tall'] or name in self.sections['wide']
        return name in self.sections[section]

    def resolve(self, section, name):
        'The effective entry for a section (Tall > Wide > Narrow) or None'
        order = { 'narrow':('narrow',), 'wide':('wide','narrow'), 'tall':('tall','wide','narrow') }[section]
        for s in order:
            if name in self.sections[s]: return self.sections[s][name]
        return None

    def set(self, section, name, flat):
        sect = self.sections[section]
        if name in sect: sect[name].set_flat(flat)
        else: sect[name] = LangEntry(name, flat_to_rhs(flat))

    def sort_like(self, en:'LangFile'):
        'Sort entries in each section to match the English order'
        nidx = { n:i for i, n in enumerate(en.sections['narrow']) }
        for s in SECTIONS:
            idx = { n:i for i, n in enumerate(en.sections[s]) }
            def key(kv):
                n = kv[0]
                if n == 'LANGUAGE': return (-1, 0)
                if n in idx: return (0, idx[n])
                if n in nidx: return (1, nidx[n])
                return (2, 0)
            self.sections[s] = OrderedDict(sorted(self.sections[s].items(), key=key))

    def render(self, en:'LangFile|None'=None):
        'Render the file. Given English, refresh the reference comments.'
        out = [ self.preamble ]
        def en_flat(section, name):
            if en is None or self.code == 'en' or name == 'LANGUAGE': return None
            e = en.resolve(section, name)
            return e.flat if e else None

        def emit(section):
            for name, e in self.sections[section].items():
                out.append(e.format(section, en_flat(section, name)))
                if name == 'LANGUAGE': out.append('')

        def head(ns):
            out.append(f'namespace {ns} {{')
            if self.code != 'en' or self.flat_ns:
                out.append(f'  using namespace {self.inherit};{self.inherit_comment}')
            out.append('')
            if self.charsize: out.append(f'  constexpr uint8_t CHARSIZE              = {self.charsize};')

        if self.flat_ns:
            head(f'Language_{self.code}')
            emit('narrow')
            out += [ '}', '' ]
            return '\n'.join(out)

        head(f'LanguageNarrow_{self.code}')
        emit('narrow')
        out += [ '}', '',
                 f'namespace LanguageWide_{self.code} {{',
                 f'  using namespace LanguageNarrow_{self.code};',
                  '  #if LCD_WIDTH > 20 || HAS_DWIN_E3V2' ]
        emit('wide')
        out += [ '  #endif', '}', '',
                 f'namespace LanguageTall_{self.code} {{',
                 f'  using namespace LanguageWide_{self.code};',
                  '  #if LCD_HEIGHT >= 4',
                  '    // Filament Change screens show up to 3 lines on a 4-line display' ]
        emit('tall')
        out += [ '  #endif', '}', '',
                 f'namespace Language_{self.code} {{',
                 f'  using namespace LanguageTall_{self.code};',
                  '}', '' ]
        return '\n'.join(out)

    def save(self, en:'LangFile|None'=None, path=None):
        Path(path or self.path).write_text(self.render(en), encoding='utf-8', newline='')

def split_statement(rest:str):
    '''
    Split the text after 'LSTR NAME =' into (rhs, comment),
    allowing for ';' and '//' inside string literals.
    '''
    i, instr = 0, False
    while i < len(rest):
        c = rest[i]
        if instr:
            if c == '\\': i += 1
            elif c == '"': instr = False
        elif c == '"': instr = True
        elif c == ';':
            tail = rest[i+1:].strip()
            return rest[:i].strip(), (tail[2:].strip() if tail.startswith('//') else None)
        i += 1
    raise ValueError(f"Unterminated LSTR: {rest}")

def language_codes(include_en=False, include_test=False):
    'All language codes that have a language_xx.h file'
    codes = []
    for f in sorted(LANGHOME.glob('language_*.h')):
        code = f.stem[len('language_'):]
        if (code == 'test' and not include_test) or (code == 'en' and not include_en): continue
        codes.append(code)
    return codes

def parse_language_args(args_list):
    'Allow a space or comma delimited list, or repeated arguments'
    codes = []
    for a in (args_list or []): codes += re.split(r'[\s,]+', a.strip())
    return [ c for c in codes if c ]

def load_languages(codes=None):
    'Load English plus the given (or all) languages. Returns (en, { code: LangFile })'
    en = LangFile.load('en')
    langs = {}
    for code in (codes or language_codes()):
        if code != 'en': langs[code] = LangFile.load(code)
    return en, langs

def is_translatable(en:LangFile, section, name):
    if name in NEVER_TRANSLATE: return False
    e = en.resolve(section, name)
    return e is not None and re.search(r'[A-Za-z]{2}', flat_text(e.flat)) is not None

def missing_strings(en:LangFile, lf:LangFile, parent:'LangFile|None'=None):
    '''
    List (section, name) for each English string the language doesn't define.
    For Wide and Tall the language must provide its own override wherever
    English has one, otherwise the shorter (or English) string is shown.
    Strings defined by a parent language (e.g., el for el_CY) count.
    '''
    missing = []
    for s in SECTIONS:
        for name in en.sections[s]:
            if not is_translatable(en, s, name): continue
            if s == 'narrow':
                ok = name in lf.all_names() or (parent and name in parent.all_names())
            else:
                ok = lf.has(s, name) or (parent and parent.has(s, name))
            if not ok: missing.append((s, name))
    return missing

def tft_glyphs(code):
    'The set of extra TFT glyphs available for a language, or None if not limited'
    cat = language_tft(code)
    if not cat: return None
    if cat == 'Latin_Extended_A': return set(chr(c) for c in range(0x100, 0x180))
    f = TFTFONTHOME / f'Unifont_{cat}_20.cpp'
    if not f.exists(): return None
    return set(chr(int(h, 16)) for h in re.findall(r'//\s*0x([0-9a-fA-F]{4,5})\s', f.read_text(encoding='utf-8')))

def deaccent(s:str):
    'Replace accented characters with plain ASCII equivalents'
    import unicodedata
    for a, b in (('ß','ss'), ('œ','oe'), ('Œ','OE'), ('æ','ae'), ('Æ','AE'), ('«','"'), ('»','"'), ('°','')):
        s = s.replace(a, b)
    return ''.join(c for c in unicodedata.normalize('NFD', s) if unicodedata.category(c) != 'Mn')

def derive_language(en:LangFile, code:str):
    '''
    Regenerate a derived language (e.g., fr_na) from its source language.
    Strings are copied from the source with accents removed. The derived
    file's own preamble and CHARSIZE are kept.
    '''
    src = LangFile.load(language_derived(code))
    lf = LangFile.load(code)
    for s in SECTIONS:
        old = lf.sections[s]
        lf.sections[s] = OrderedDict()
        for name, e in src.sections[s].items():
            if name == 'LANGUAGE' and name in old:
                lf.sections[s][name] = old[name]
                continue
            rhs = deaccent(e.rhs)   # Macros and structure are preserved
            if name in old and old[name].rhs == rhs:
                lf.sections[s][name] = old[name]
            else:
                lf.sections[s][name] = LangEntry(name, rhs)
    lf.save(en)
    return lf

def require_repo_root():
    if not LANGHOME.is_dir():
        print(f"Error: Couldn't find '{LANGHOME}'. Run from the root of the Marlin repository.")
        exit(1)
