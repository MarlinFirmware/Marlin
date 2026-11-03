#!/usr/bin/env python3
'''
languageExport.py [options]

Export Marlin LCD language strings for translation.

Formats:
  CSV  (default) One sheet per language in out-csv/language_xx.csv,
                 or a single languages.csv with --single.
                 Columns: name, xx, xx (wide), xx (tall)
  JSON (--json)  One file per language in out-json/language_xx.json:
                 { "narrow": { NAME: "string", ... }, "wide": {...}, "tall": {...} }

Work-packs (--todo):
  Export only the strings each language is missing, with everything a human
  or AI translator needs to do a good job:
    - The English string and its section (narrow / wide / tall)
    - Suggested maximum display width
    - Placeholders and macros that must be preserved
    - The language's existing translations, for consistent terminology
  Fill in the "text" fields and merge the result with languageImport.py.

Strings use the "flat" format described in languageUtil.py:
  (MACRO) for macros, a leading | with | separators for multi-line strings.

Examples:
  languageExport.py                      # CSV for all languages
  languageExport.py -s                   # A single languages.csv
  languageExport.py -l "de fr" --json    # JSON for German and French
  languageExport.py --todo -l vi         # Work-pack of missing Vietnamese strings
'''

import json, argparse, csv
from pathlib import Path
from languageUtil import *

def string_table(lf:LangFile):
    'All flat strings for a language as { section: { name: flat } }'
    return { s: OrderedDict((n, e.flat) for n, e in lf.sections[s].items()) for s in SECTIONS }

def names_in_order(en, langs):
    'All distinct string names, in English order, then any extras'
    names = OrderedDict()
    for s in SECTIONS:
        for n in en.sections[s]: names[n] = 1
    for lf in langs.values():
        for n in lf.all_names(): names[n] = 1
    return list(names)

def export_csv(en, langs, single, outdir):
    names = names_in_order(en, langs)
    tables = { 'en': string_table(en) } | { c: string_table(lf) for c, lf in langs.items() }

    def header(code):
        lname = f'{code} {language_name(code)}'
        return [ lname, lname + ' (wide)', lname + ' (tall)' ]

    def row(code, name):
        return [ tables[code][s].get(name, '') for s in SECTIONS ]

    def write(path, codes):
        with open(path, 'w', encoding='utf-8', newline='') as f:
            w = csv.writer(f, quoting=csv.QUOTE_ALL, lineterminator='\n')
            w.writerow(['name'] + sum((header(c) for c in codes), []))
            for name in names:
                w.writerow([name] + sum((row(c, name) for c in codes), []))
        print(f"Wrote {path}")

    if single:
        write(Path('languages.csv'), ['en'] + list(langs))
    else:
        outdir.mkdir(exist_ok=True)
        for code in ['en'] + list(langs):
            write(outdir / f'language_{code}.csv', [code])

def export_json(en, langs, outdir):
    outdir.mkdir(exist_ok=True)
    for code, lf in [('en', en)] + list(langs.items()):
        path = outdir / f'language_{code}.json'
        path.write_text(json.dumps(string_table(lf), ensure_ascii=False, indent=1) + '\n', encoding='utf-8')
        print(f"Wrote {path}")

def export_todo(en, langs, outdir, glossary_max):
    outdir.mkdir(exist_ok=True)
    for code, lf in langs.items():
        if language_derived(code):
            print(f"Skipping {code} (generated from {language_derived(code)})")
            continue
        p = language_parent(code)
        parent = (langs.get(p) or LangFile.load(p)) if p else None
        todo = []
        for s, name in missing_strings(en, lf, parent):
            eflat = en.resolve(s, name).flat
            subs, pfmt = flat_placeholders(eflat)
            lines = flat_lines(eflat)
            item = { 'section': s, 'name': name, 'en': eflat }
            # The existing shorter translation is a useful reference for wide / tall
            if s != 'narrow':
                have = lf.resolve('narrow', name)
                if have: item['narrow'] = have.flat
            item['max'] = LINE_MAX if lines else (NARROW_MAX if s == 'narrow' else WIDE_MAX)
            if lines: item['lines'] = len(lines)
            if subs: item['keep'] = subs
            if pfmt: item['printf'] = pfmt
            macros = flat_macros(eflat)
            if macros: item['macros'] = macros
            item['text'] = ''
            todo.append(item)

        if not todo:
            print(f"{code}: nothing to do")
            continue

        # Existing translations as a terminology reference
        glossary = OrderedDict()
        for n, e in lf.sections['narrow'].items():
            if n == 'LANGUAGE' or n not in en.sections['narrow']: continue
            glossary[en.sections['narrow'][n].flat] = e.flat
            if len(glossary) >= glossary_max: break

        pack = OrderedDict([
          ('language', code),
          ('name', language_name(code)),
          ('ascii_only', bool(language_noext(code))),
          ('instructions', [
            "Translate each 'en' string into the target language and put the result in 'text'.",
            "Keep each translation within 'max' display characters, abbreviating as a native speaker would on a small LCD menu.",
            "Preserve the substitution characters listed in 'keep' ($ @ ~ * {) exactly, placed where they make sense.",
            "Preserve printf specifiers listed in 'printf' (e.g., %i, %d, %s) exactly.",
            "Preserve macros like (PREHEAT_1_LABEL) exactly, including the parentheses.",
            "Multi-line strings start with '|' and separate lines with '|'. Keep the same number of lines.",
            "Use the 'glossary' of existing translations for consistent terminology.",
            "If 'ascii_only' is true, use only plain ASCII characters.",
            "Leave 'text' empty to skip a string. Untranslated strings fall back to English."
          ]),
          ('glossary', glossary),
          ('strings', todo)
        ])
        path = outdir / f'todo_{code}.json'
        path.write_text(json.dumps(pack, ensure_ascii=False, indent=1) + '\n', encoding='utf-8')
        print(f"Wrote {path} ({len(todo)} strings)")

def main():
    require_repo_root()
    parser = argparse.ArgumentParser(description="Export Marlin LCD language strings for translation")
    parser.add_argument('-l', '--language', action='append', help="language code(s) to export (default: all)")
    parser.add_argument('-s', '--single', action='store_true', help="export a single CSV (languages.csv)")
    parser.add_argument('-j', '--json', action='store_true', help="export JSON instead of CSV")
    parser.add_argument('-t', '--todo', action='store_true', help="export work-packs of missing strings (JSON)")
    parser.add_argument('-o', '--outdir', default=None, help="output folder")
    parser.add_argument('-g', '--glossary', type=int, default=400, help="max glossary entries in work-packs")
    args = parser.parse_args()

    codes = parse_language_args(args.language) or language_codes()
    en, langs = load_languages(codes)

    if args.todo:
        export_todo(en, langs, Path(args.outdir or 'out-todo'), args.glossary)
    elif args.json:
        export_json(en, langs, Path(args.outdir or 'out-json'))
    else:
        export_csv(en, langs, args.single, Path(args.outdir or 'out-csv'))

if __name__ == '__main__':
    main()
