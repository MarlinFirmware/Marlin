#!/usr/bin/env python3
'''
languageCheck.py [options] [language ...]

Check Marlin LCD language files against English (language_en.h).

  - Missing strings, per section (Narrow, Wide, Tall)
  - Extra strings that English doesn't define (stale or misplaced)
  - Placeholder mismatches ($ @ ~ * { and printf %-specifiers)
  - Macro mismatches (e.g., PREHEAT_1_LABEL, MEDIA_TYPE_XX)
  - Strings that are probably too long for the display
  - Characters with no glyph in the TFT extra-font for the language (--glyphs)
    (DOGM font data can be regenerated with buildroot/share/fonts/genallfont.sh)
  - Reference comments that don't match the current English string
  - Entries out of English order

Examples:
  languageCheck.py                   # Summary for all languages
  languageCheck.py -m de fr          # List missing strings for de and fr
  languageCheck.py -l it             # Lint (no missing list) for it
  languageCheck.py --json -m vi      # Machine-readable report
  languageCheck.py --strict          # Exit 1 on lint errors (for CI)
'''

import sys, json, argparse
from languageUtil import *

def check_language(en, lf, parent=None, check_glyphs=False):
    rep = { 'code': lf.code, 'name': language_name(lf.code), 'missing': [], 'extra': [],
            'errors': [], 'warnings': [] }

    rep['missing'] = [ { 'section': s, 'name': n, 'en': en.resolve(s, n).flat }
                       for s, n in missing_strings(en, lf, parent) ]

    glyphs = tft_glyphs(lf.code) if check_glyphs else None
    en_names = en.all_names()

    for s in SECTIONS:
        idx = { n:i for i, n in enumerate(en.sections[s]) }
        last = -1
        for name, e in lf.sections[s].items():
            where = f'{s}:{name}'
            if name not in en_names:
                rep['extra'].append(where)
                continue
            try:
                flat = e.flat
            except ValueError as ex:
                rep['errors'].append(f'{where}: {ex}')
                continue

            # Order check (warn once per section)
            if name in idx:
                if idx[name] < last and last >= 0:
                    rep['warnings'].append(f'{where}: out of English order')
                    last = 1 << 30
                elif last != 1 << 30:
                    last = idx[name]

            ref = en.resolve(s, name)
            if ref is None or name == 'LANGUAGE': continue
            eflat = ref.flat

            # Substitution characters must match English.
            # A printf specifier may be omitted, but never added.
            (subs, pf), (esubs, epf) = flat_placeholders(flat), flat_placeholders(eflat)
            if subs != esubs or any(p not in epf for p in pf):
                rep['errors'].append(f'{where}: placeholders {subs!r} {pf} != English {esubs!r} {epf} :: "{flat}"')
            elif pf != epf:
                rep['warnings'].append(f'{where}: omits {epf} :: "{flat}"')

            # Macros should match, allowing a language-specific MEDIA_TYPE_XX
            # and harmless character / symbol macros.
            def norm(ms): return sorted('MEDIA_TYPE' if m.startswith('MEDIA_TYPE_') else m for m in ms
                                        if not m.startswith(('LCD_STR_', 'STR_', 'SUPERSCRIPT_')))
            if norm(flat_macros(flat)) != norm(flat_macros(eflat)):
                # Dropping the MEDIA_TYPE is fine, adding unknown ones is not
                extra = set(norm(flat_macros(flat))) - set(norm(flat_macros(eflat)))
                (rep['errors'] if extra else rep['warnings']).append(
                    f'{where}: macros {flat_macros(flat)} != English {flat_macros(eflat)}')

            # Multi-line shape
            el, ll = flat_lines(eflat), flat_lines(flat)
            if (el is None) != (ll is None):
                rep['errors'].append(f'{where}: multi-line mismatch with English :: "{flat}"')

            # Length, relative to English and the display
            w, ew = flat_width(flat), flat_width(eflat)
            limit = (LINE_MAX if ll else NARROW_MAX if s == 'narrow' else WIDE_MAX)
            if w > max(limit, ew + 2):
                rep['warnings'].append(f'{where}: long ({w} > {max(limit, ew)}) :: "{flat}"')

            # Glyphs missing from the TFT font
            if glyphs is not None:
                bad = sorted({ c for c in flat_text(flat) if ord(c) > 0xFF and c not in glyphs })
                if bad: rep['warnings'].append(f'{where}: no TFT glyph for {"".join(bad)}')

            # Stale reference comment
            if e.comment is not None and e.comment != eflat.strip() and e.raw is not None:
                rep['warnings'].append(f'{where}: comment "{e.comment}" != English "{eflat}"')

    return rep

def main():
    require_repo_root()
    parser = argparse.ArgumentParser(description="Check Marlin LCD language files against English")
    parser.add_argument('language', nargs='*', help="language codes to check (default: all)")
    parser.add_argument('-m', '--missing', action='store_true', help="list each missing string")
    parser.add_argument('-l', '--lint', action='store_true', help="list lint errors and warnings")
    parser.add_argument('-e', '--errors', action='store_true', help="list lint errors only")
    parser.add_argument('-j', '--json', action='store_true', help="output a JSON report")
    parser.add_argument('-g', '--glyphs', action='store_true', help="warn about characters missing from the TFT extra font")
    parser.add_argument('-s', '--strict', action='store_true', help="exit with an error if any lint errors are found")
    args = parser.parse_args()

    codes = parse_language_args(args.language) or language_codes()
    en, langs = load_languages(codes)

    reports = []
    for code, lf in langs.items():
        p = language_parent(code)
        parent = (langs.get(p) or LangFile.load(p)) if p else None
        reports.append(check_language(en, lf, parent, args.glyphs))

    total = sum(1 for s in SECTIONS for n in en.sections[s] if is_translatable(en, s, n))

    if args.json:
        print(json.dumps({ 'total': total, 'languages': reports }, ensure_ascii=False, indent=1))
    else:
        print(f"{total} translatable LCD strings in English\n")
        print(f"{'code':8} {'language':24} {'missing':>8} {'done':>6} {'extra':>6} {'errors':>7} {'warn':>6}")
        for r in reports:
            done = 100 * (total - len(r['missing'])) / total
            print(f"{r['code']:8} {r['name'][:24]:24} {len(r['missing']):8} {done:5.1f}% {len(r['extra']):6} {len(r['errors']):7} {len(r['warnings']):6}")
        for r in reports:
            show_lint = args.lint or args.errors
            if not ((args.missing and r['missing']) or (show_lint and (r['errors'] or r['warnings'] or r['extra']))): continue
            print(f"\n== language_{r['code']}.h ({r['name']})")
            if args.missing:
                for m in r['missing']: print(f"  missing {m['section']:6} {m['name']:36} \"{m['en']}\"")
            if show_lint:
                for x in r['extra']: print(f"  extra   {x}")
                for x in r['errors']: print(f"  ERROR   {x}")
                if args.lint:
                    for x in r['warnings']: print(f"  warn    {x}")

    if args.strict and any(r['errors'] for r in reports): exit(1)

if __name__ == '__main__':
    main()
