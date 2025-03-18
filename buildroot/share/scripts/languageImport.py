#!/usr/bin/env python3
"""
languageImport.py

Import LCD language strings from a CSV file or Google Sheets
and write Marlin LCD language files based on the data.

Use languageExport.py to export CSV from the language files.

Google Sheets Link:
https://docs.google.com/spreadsheets/d/12yiy-kS84ajKFm7oQIrC4CF8ZWeu9pAR4zrgxH4ruk4/edit#gid=84528699

TODO: Use the defines and comments above the namespace from existing language files.
      Get the 'constexpr uint8_t CHARSIZE' from existing language files.
      Get the correct 'using namespace' for languages that don't inherit from English.

"""

import sys, re, requests, csv, datetime
#from languageUtil import namebyid

LANGHOME = "Marlin/src/lcd/language"
OUTDIR = 'out-language'

# Get the file path from the command line
FILEPATH = sys.argv[1] if len(sys.argv) > 1 else None

download = FILEPATH == 'download'

if not FILEPATH or download:
    SHEETID = "12yiy-kS84ajKFm7oQIrC4CF8ZWeu9pAR4zrgxH4ruk4"
    FILEPATH = 'https://docs.google.com/spreadsheet/ccc?key=%s&output=csv' % SHEETID

if FILEPATH.startswith('http'):
    response = requests.get(FILEPATH)
    assert response.status_code == 200, 'GET failed for %s' % FILEPATH
    csvdata = response.content.decode('utf-8')
else:
    if not FILEPATH.endswith('.csv'): FILEPATH += '.csv'
    with open(FILEPATH, 'r', encoding='utf-8') as f: csvdata = f.read()

if not csvdata:
    print("Error: couldn't read CSV data from %s" % FILEPATH)
    exit(1)

if download:
    DLNAME = sys.argv[2] if len(sys.argv) > 2 else 'languages.csv'
    if not DLNAME.endswith('.csv'): DLNAME += '.csv'
    with open(DLNAME, 'w', encoding='utf-8') as f: f.write(csvdata)
    print("Downloaded %s from %s" % (DLNAME, FILEPATH))
    exit(0)

lines = csvdata.splitlines()
print(lines)
reader = csv.reader(lines, delimiter=',')
gothead = False
columns = ['']
numcols = 0
strings_per_lang = {}
for row in reader:
    if not gothead:
        gothead = True
        numcols = len(row)
        if row[0] != 'name':
            print('Error: first column should be "name"')
            exit(1)
        # The rest of the columns are language codes and names
        for i in range(1, numcols):
            elms = row[i].split(' ')
            lang = elms[0]
            style = ('Wide' if elms[-1] == '(wide)' else 'Tall' if elms[-1] == '(tall)' else 'Narrow')
            columns.append({ 'lang': lang, 'style': style })
            if not lang in strings_per_lang: strings_per_lang[lang] = {}
            if not style in strings_per_lang[lang]: strings_per_lang[lang][style] = {}
        continue
    # Add the named string for all the included languages
    name = row[0]
    for i in range(1, numcols):
        str_key = row[i]
        if str_key:
            col = columns[i]
            strings_per_lang[col['lang']][col['style']][name] = str_key

# Create a folder for the imported language outfiles
from pathlib import Path
Path.mkdir(Path(OUTDIR), exist_ok=True)

FILEHEADER = '''
/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * %s
 *
 * LCD Menu Messages
 * See also https://marlinfw.org/docs/development/lcd_language.html
 *
 * Substitutions are applied for the following characters when used in menu items titles:
 *
 *   $ displays an inserted string
 *   { displays  '0'....'10' for indexes 0 - 10
 *   ~ displays  '1'....'11' for indexes 0 - 10
 *   * displays 'E1'...'E11' for indexes 0 - 10 (By default. Uses LCD_FIRST_TOOL)
 *   @ displays an axis name such as XYZUVW, or E for an extruder
 */

'''

# Iterate over the languages which correspond to the columns
# The columns are assumed to be grouped by language in the order Narrow, Wide, Tall
# TODO: Go through lang only, then impose the order Narrow, Wide, Tall.
#       So if something is missing or out of order everything still gets built correctly.

f = None
gotlang = {}
for i in range(1, numcols):
    #if i > 6: break # Testing
    col = columns[i]
    lang, style = col['lang'], col['style']

    # If we haven't already opened a file for this language, do so now
    if not lang in gotlang:
        gotlang[lang] = {}
        if f: f.close()
        fn = "%s/language_%s.h" % (OUTDIR, lang)
        f = open(fn, 'w', encoding='utf-8')
        if not f:
            print("Failed to open %s." % fn)
            exit(1)

        # Write the opening header for the new language file
        #f.write(FILEHEADER % namebyid(lang))
        f.write('/**\n * Imported from %s on %s at %s\n */\n' % (FILEPATH, datetime.date.today(), datetime.datetime.now().strftime("%H:%M:%S")))

    # Start a namespace for the language and style
    f.write('\nnamespace Language%s_%s {\n' % (style, lang))

    # Wide and tall namespaces inherit from the others
    if style == 'Wide':
        f.write('  using namespace LanguageNarrow_%s;\n' % lang)
        f.write('  #if LCD_WIDTH >= 20 || HAS_DWIN_E3V2\n')
    elif style == 'Tall':
        f.write('  using namespace LanguageWide_%s;\n' % lang)
        f.write('  #if LCD_HEIGHT >= 4\n')
    elif lang != 'en':
        f.write('  using namespace Language_en; // Inherit undefined strings from English\n')

    # Formatting for the lines
    indent = '  ' if style == 'Narrow' else '    '
    width = 34 if style == 'Narrow' else 32
    lstr_fmt = '%sLSTR %%-%ds = %%s;%%s\n' % (indent, width)

    # Emit all the strings for this language and style
    for name in strings_per_lang[lang][style].keys():
        # Get the raw string value
        val = strings_per_lang[lang][style][name]
        # Count the number of bars
        if val.startswith('|'):
            bars = val.count('|')
            val = val[1:]
        else:
            bars = 0
        # Escape backslashes, substitute quotes, and wrap in _UxGT("...")
        val = '_UxGT("%s")' % val.replace('\\', '\\\\').replace('"', '$$$')
        # Move named references outside of the macro
        val = re.sub(r'\(([A-Z0-9]+_[A-Z0-9_]+)\)', r'") \1 _UxGT("', val)
        # Remove all empty _UxGT("") that result from the above
        val = re.sub(r'\s*_UxGT\(""\)\s*', '', val)
        # No wrapper needed for just spaces
        val = re.sub(r'_UxGT\((" +")\)', r'\1', val)
        # Multi-line strings start with a bar...
        if bars:
            # Wrap the string in MSG_#_LINE(...) and split on bars
            val = re.sub(r'^_UxGT\((.+)\)', r'_UxGT(MSG_%s_LINE(\1))' % bars, val)
            val = val.replace('|', '", "')
        # Restore quotes inside the string
        val = val.replace('$$$', '\\"')
        # Add a comment with the English string for reference
        comm = ''
        if lang != 'en' and 'en' in strings_per_lang:
            en = strings_per_lang['en']
            if name in en[style]: str_key = en[style][name]
            elif name in en['Narrow']: str_key = en['Narrow'][name]
            if str_key:
                cfmt = '%%%ss// %%s' % (50 - len(val) if len(val) < 50 else 1)
                comm = cfmt % (' ', str_key)

        # Write out the string definition
        f.write(lstr_fmt % (name, val, comm))

    if style == 'Wide' or style == 'Tall': f.write('  #endif\n')

    f.write('}\n') # End namespace

    # Assume the 'Tall' namespace comes last
    if style == 'Tall': f.write('\nnamespace Language_%s {\n  using namespace LanguageTall_%s;\n}\n' % (lang, lang))

# Close the last-opened output file
if f: f.close()
