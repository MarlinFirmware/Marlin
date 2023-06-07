#!/usr/bin/env python3
#
# exportTranslations.py
#
# Export LCD language strings to CSV files for easier translation.
# Use importTranslations.py to import CSV into the language files.
#

import re
from pathlib import Path

# Write multiple sheets if true, otherwise write one giant sheet
MULTISHEET = True

# Where to look for the language files
LANGHOME = "Marlin/src/lcd/language"

# Check for the path to the language files
if not Path(LANGHOME).is_dir():
    print("Error: Couldn't find the '%s' directory." % LANGHOME)
    print("Edit LANGHOME or cd to the root of the repo before running.")
    exit(1)

# A dictionary to contain language names
LANGNAME = {
    'an': "Aragonese",
    'bg': "Bulgarian",
    'ca': "Catalan",
    'cz': "Czech",
    'da': "Danish",
    'de': "German",
    'el': "Greek", 'el_CY': "Greek (Cyprus)", 'el_gr': "Greek (Greece)",
    'en': "English",
    'es': "Spanish",
    'eu': "Basque-Euskera",
    'fi': "Finnish",
    'fr': "French", 'fr_na': "French (no accent)",
    'gl': "Galician",
    'hr': "Croatian (Hrvatski)",
    'hu': "Hungarian / Magyar",
    'it': "Italian",
    'jp_kana': "Japanese (Kana)",
    'ko_KR': "Korean",
    'nl': "Dutch",
    'pl': "Polish",
    'pt': "Portuguese", 'pt_br': "Portuguese (Brazil)",
    'ro': "Romanian",
    'ru': "Russian",
    'sk': "Slovak",
    'sv': "Swedish",
    'tr': "Turkish",
    'uk': "Ukrainian",
    'vi': "Vietnamese",
    'zh_CN': "Simplified Chinese", 'zh_TW': "Traditional Chinese"
}

# A limit just for testing
LIMIT = 0

# A dictionary to contain strings for each language.
# Init with 'en' so English will always be first.
language_strings = { 'en': 0 }

# A dictionary to contain all distinct LCD string names
names = {}

# Get all "language_*.h" files
langfiles = sorted(list(Path(LANGHOME).glob('language_*.h')))

# Read each language file
for langfile in langfiles:
    # Get the language code from the filename
    langcode = langfile.name.replace('language_', '').replace('.h', '')

    # Skip 'test' and any others that we don't want
    if langcode in ['test']: continue

    # Open the file
    f = open(langfile, 'r', encoding='utf-8')
    if not f: continue

    # Flags to indicate a wide or tall section
    wideflag = False
    tallflag = False
    # A counter for the number of strings in the file
    stringcount = 0
    # A dictionary to hold all the strings
    strings = { 'narrow': {}, 'wide': {}, 'tall': {} }
    # Read each line in the file
    for line in f:
        # Clean up the line for easier parsing
        line = line.split("//")[0].strip()
        if line.endswith(';'): line = line[:-1].strip()

        # Check for wide or tall sections, assume no complicated nesting
        if line.startswith("#endif") or line.startswith("#else"):
            wideflag = False
            tallflag = False
        elif re.match(r'#if.*WIDTH\s*>=?\s*2[01].*', line): wideflag = True
        elif re.match(r'#if.*LCD_HEIGHT\s*>=?\s*4.*', line): tallflag = True

        # For string-defining lines capture the string data
        match = re.match(r'LSTR\s+([A-Z0-9_]+)\s*=\s*(.+)\s*', line)
        if match:
            # The name is the first captured group
            name = match.group(1)
            # The value is the second captured group
            value = match.group(2)
            # Replace escaped quotes temporarily
            value = value.replace('\\"', '__Q__')

            # Remove all _UxGT wrappers from the value in a non-greedy way
            value = re.sub(r'_UxGT\((".*?")\)', r'\1', value)

            # Multi-line strings will get one or more bars | for identification
            multiline = 0
            multimatch = re.match(r'.*MSG_(\d)_LINE\s*\(\s*(.+?)\s*\).*', value)
            if multimatch:
                multiline = int(multimatch.group(1))
                value = '|' + re.sub(r'"\s*,\s*"', '|', multimatch.group(2))

            # Wrap inline defines in parentheses
            value = re.sub(r' *([A-Z0-9]+_[A-Z0-9_]+) *', r'(\1)', value)
            # Remove quotes around strings
            value = re.sub(r'"(.*?)"', r'\1', value).replace('__Q__', '"')
            # Store all unique names as dictionary keys
            names[name] = 1
            # Store the string as narrow or wide
            strings['tall' if tallflag else 'wide' if wideflag else 'narrow'][name] = value

            # Increment the string counter
            stringcount += 1
            # Break for testing
            if LIMIT and stringcount >= LIMIT: break

    # Close the file
    f.close()
    # Store the array in the dict
    language_strings[langcode] = strings

# Get the language codes from the dictionary
langcodes = list(language_strings.keys())

# Print the array
#print(language_strings)

# Write a single language entry to the CSV file with narrow, wide, and tall strings
def write_csv_lang(f, strings, name):
    f.write(',')
    if name in strings['narrow']: f.write('"%s"' % strings['narrow'][name])
    f.write(',')
    if name in strings['wide']: f.write('"%s"' % strings['wide'][name])
    f.write(',')
    if name in strings['tall']: f.write('"%s"' % strings['tall'][name])

if MULTISHEET:
    #
    # Export a separate sheet for each language
    #
    OUTDIR = 'csv-out'
    Path.mkdir(Path(OUTDIR), exist_ok=True)

    for lang in langcodes:
        f = open("%s/language_%s.csv" % (OUTDIR, lang), 'w', encoding='utf-8')
        if not f: continue

        lname = lang + ' ' + LANGNAME[lang]
        header = ['name', lname, lname + ' (wide)', lname + ' (tall)']
        f.write('"' + '","'.join(header) + '"\n')

        for name in names.keys():
            f.write('"' + name + '"')
            write_csv_lang(f, language_strings[lang], name)
            f.write('\n')
        f.close()

else:
    #
    # Export one large sheet containing all languages
    #
    f = open("languages.csv", 'w', encoding='utf-8')
    if f:
        header = ['name']
        for lang in langcodes:
            lname = lang + ' ' + LANGNAME[lang]
            header += [lname, lname + ' (wide)', lname + ' (tall)']
        f.write('"' + '","'.join(header) + '"\n')

        for name in names.keys():
            f.write('"' + name + '"')
            for lang in langcodes: write_csv_lang(f, language_strings[lang], name)
            f.write('\n')
        f.close()
