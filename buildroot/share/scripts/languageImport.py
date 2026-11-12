#!/usr/bin/env python3
'''
languageImport.py [options] SOURCE [SOURCE ...]

Merge translated LCD strings into the Marlin language files.

Unlike a full regeneration, merging only touches the strings provided.
Everything else in each language file (header, defines, untouched lines)
is preserved, so the resulting diff shows only real changes.

Sources:
  out-todo/todo_xx.json     Work-pack from 'languageExport.py --todo'
  out-json/language_xx.json Section tables from 'languageExport.py --json'
  languages.csv             CSV from 'languageExport.py' (one or many languages)
  download [NAME.csv]       Download the shared Google Sheet as CSV

Each imported string is validated against English before it's written:
placeholders ($ @ ~ * { and printf specifiers), macros, and multi-line shape
must match. Invalid strings are reported and skipped. Strings identical to the
English text are kept, marking them as reviewed (e.g., "BLTouch", "Jerk").

Options:
  -n, --dry-run   Report what would change without writing
  -r, --replace   Replace existing translations (default: only add missing)
  -l LANG         Only import the given language(s)
  --no-sort       Don't sort entries into English order (sorting is the default)

Derived languages (e.g., fr_na from fr) are regenerated automatically
whenever their source language is updated.

Google Sheets:
https://docs.google.com/spreadsheets/d/12yiy-kS84ajKFm7oQIrC4CF8ZWeu9pAR4zrgxH4ruk4/edit#gid=84528699
'''

import sys, csv, json, argparse
from pathlib import Path
from languageUtil import *

SHEETID = "12yiy-kS84ajKFm7oQIrC4CF8ZWeu9pAR4zrgxH4ruk4"

def read_source(path):
    '''
    Read a source file and return { code: [ (section, name, flat), ... ] }
    '''
    path = Path(path)
    data = {}
    if path.suffix == '.json':
        j = json.loads(path.read_text(encoding='utf-8'))
        if 'strings' in j:      # Work-pack
            code = j['language']
            data[code] = [ (it['section'], it['name'], it.get('text', '')) for it in j['strings'] ]
        else:                   # Section tables
            code = path.stem.replace('language_', '')
            data[code] = [ (s, n, v) for s in SECTIONS for n, v in j.get(s, {}).items() ]
    else:
        rows = list(csv.reader(path.read_text(encoding='utf-8').splitlines()))
        head = rows[0]
        if head[0] != 'name': raise ValueError(f"{path}: first column should be 'name'")
        cols = []
        for h in head[1:]:
            elms = h.split(' ')
            sect = 'wide' if elms[-1] == '(wide)' else 'tall' if elms[-1] == '(tall)' else 'narrow'
            cols.append((elms[0], sect))
        for row in rows[1:]:
            for (code, sect), val in zip(cols, row[1:]):
                data.setdefault(code, []).append((sect, row[0], val))
    return data

def validate(en, section, name, flat):
    'Return an error string if the flat string is unsuitable, else None'
    ref = en.resolve(section, name)
    if ref is None: return "not defined in English"
    eflat = ref.flat
    (subs, pf), (esubs, epf) = flat_placeholders(flat), flat_placeholders(eflat)
    if subs != esubs: return f"substitutions '{subs}' != English '{esubs}'"
    if any(p not in epf for p in pf): return f"printf {pf} not in English {epf}"
    el, ll = flat_lines(eflat), flat_lines(flat)
    if (el is None) != (ll is None): return "multi-line shape differs from English"
    if ll is not None and not 1 <= len(ll) <= 3: return "multi-line strings need 1-3 lines"
    def norm(ms): return { 'MEDIA_TYPE' if m.startswith('MEDIA_TYPE_') else m for m in ms }
    extra = norm(flat_macros(flat)) - norm(flat_macros(eflat)) - { m for m in flat_macros(flat) if m.startswith(('LCD_STR_', 'STR_', 'SUPERSCRIPT_')) }
    if extra: return f"unknown macros {sorted(extra)}"
    if '"' in flat_text(flat) or '\\' in flat_text(flat): return "contains a quote or backslash"
    return None

def main():
    require_repo_root()
    parser = argparse.ArgumentParser(description="Merge translated LCD strings into Marlin language files")
    parser.add_argument('source', nargs='+', help="work-pack JSON, language JSON, CSV, or 'download'")
    parser.add_argument('-n', '--dry-run', action='store_true', help="report changes without writing files")
    parser.add_argument('-r', '--replace', action='store_true', help="replace existing translations")
    parser.add_argument('-l', '--language', action='append', help="only import the given language(s)")
    parser.add_argument('--no-sort', dest='sort', action='store_false', help="don't sort entries into English order")
    args = parser.parse_args()

    if args.source[0] == 'download':
        import requests
        url = f'https://docs.google.com/spreadsheet/ccc?key={SHEETID}&output=csv'
        response = requests.get(url)
        assert response.status_code == 200, f'GET failed for {url}'
        name = args.source[1] if len(args.source) > 1 else 'languages.csv'
        if not name.endswith('.csv'): name += '.csv'
        Path(name).write_text(response.content.decode('utf-8'), encoding='utf-8')
        print(f"Downloaded {name} from {url}")
        return

    only = parse_language_args(args.language)
    incoming = {}
    for src in args.source:
        for code, items in read_source(src).items():
            incoming.setdefault(code, []).extend(items)

    en = LangFile.load('en')
    status, written = 0, set()
    for code, items in incoming.items():
        if code == 'en' or (only and code not in only): continue
        if not (LANGHOME / f'language_{code}.h').exists():
            print(f"{code}: no language_{code}.h (create it first)")
            status = 1
            continue
        if language_derived(code):
            print(f"{code}: generated from {language_derived(code)}, skipping")
            continue
        lf = LangFile.load(code)
        added = replaced = skipped = 0
        for section, name, flat in items:
            flat = flat or ''
            if not flat.strip(): continue
            if name in NEVER_TRANSLATE: continue
            err = validate(en, section, name, flat)
            if err:
                print(f"  {code} {section}:{name}: {err} :: \"{flat}\"")
                skipped += 1
                status = 1
                continue
            # Identical to English is allowed; it records that the string was reviewed
            # and is intentionally the same (e.g., technical terms like "BLTouch").
            cur = lf.sections[section].get(name)
            if cur:
                if cur.flat == flat or not args.replace: continue
                replaced += 1
            else:
                added += 1
            lf.set(section, name, flat)
        if args.sort and (added or replaced): lf.sort_like(en)
        print(f"{code}: {added} added, {replaced} replaced, {skipped} skipped")
        if not args.dry_run and (added or replaced):
            lf.save(en)
            written.add(code)

    # Regenerate derived languages (e.g., fr_na) from their updated sources
    if not args.dry_run:
        for code in LANGNAME:
            src = language_derived(code)
            if src and src in written:
                derive_language(en, code)
                print(f"{code}: regenerated from {src}")

    exit(status)

if __name__ == '__main__':
    main()
