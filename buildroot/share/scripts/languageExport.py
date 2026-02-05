#!/usr/bin/env python3
'''
languageExport.py [--single] [--translate]

Export LCD language strings to CSV files for easier translation.
Use languageImport.py to import CSV into the language files.

Use --single to export all languages to a single CSV file.
'''

import re, argparse
from pathlib import Path
from sys import argv, exit
from languageUtil import *

LANGHOME = "Marlin/src/lcd/language"
OUTDIR = Path('out-csv')

def language_export(args={}):
    # A dictionary to contain strings for each language.
    # Init with 'en' so English will always be first.
    language_strings = { 'en': {} }

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
        if langcode != 'en' and args.languages and langcode not in args.languages: continue

        # Open the file
        f = open(langfile, 'r', encoding='utf-8')
        if not f: continue

        # Flags to indicate a wide or tall section
        wideflag, tallflag = False, False
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
                wideflag, tallflag = False, False
            elif re.match(r'#if.*WIDTH\s*>=?\s*2[01].*', line): wideflag = True
            elif re.match(r'#if.*LCD_HEIGHT\s*>=?\s*4.*', line): tallflag = True

            # For string-defining lines capture the string data
            match = re.match(r'LSTR\s+([A-Z0-9_]+)\s*=\s*(.+)\s*', line)
            if match:
                # Name and quote-sanitized value
                name, value = match.group(1), match.group(2).replace('\\"', '$$$')

                # Remove all _UxGT wrappers from the value in a non-greedy way
                value = re.sub(r'_UxGT\((".*?")\)', r'\1', value)

                # Multi-line strings get one or more bars | for identification
                multiline = 0
                multimatch = re.match(r'.*MSG_(\d)_LINE\s*\(\s*(.+?)\s*\).*', value)
                if multimatch:
                    multiline = int(multimatch.group(1))
                    value = '|' + re.sub(r'"\s*,\s*"', '|', multimatch.group(2))

                # Wrap inline defines in parentheses
                value = re.sub(r' *([A-Z0-9]+_[A-Z0-9_]+) *', r'(\1)', value)
                # Remove quotes around strings
                value = re.sub(r'"(.*?)"', r'\1', value).replace('$$$', '""')
                # Store all unique names as dictionary keys
                names[name] = 1
                # Store the string as narrow or wide
                strings['tall' if tallflag else 'wide' if wideflag else 'narrow'][name] = value

                # Increment the string counter
                stringcount += 1
                # Break for testing
                if args.limit and stringcount >= int(args.limit): break

        # Close the file
        f.close()
        # Store the array in the dict
        language_strings[langcode] = strings

    # Get the codes of all imported languages
    langcodes = list(language_strings.keys())

    print("Languages:", ' '.join(langcodes))

    # Print the array
    #print(language_strings)

    # Report the total number of unique strings
    print("Found %s distinct LCD strings." % len(names))

    #exit(0)

    # Add missing translations, if specified
    if args.translate:

        MIN_TRANSLATE_LEN = 2
        NEVER_TRANSLATE_LANGS = ( 'el_CY', 'fr_na' )
        NEVER_TRANSLATE_NAMES = (
          "MSG_MARLIN", "MSG_CUSTOM_MENU_MAIN_TITLE",
          "MSG_PID_P", "MSG_PID_P_E",
          "MSG_PID_I", "MSG_PID_I_E",
          "MSG_PID_D", "MSG_PID_D_E",
          "MSG_PID_C", "MSG_PID_C_E",
          "MSG_PID_F", "MSG_PID_F_E",
          "MSG_BACKLASH_N",
          "MSG_SHORT_DAY", "MSG_SHORT_HOUR", "MSG_SHORT_MINUTE",
          "MSG_FTM_ZV", "MSG_FTM_ZVD", "MSG_FTM_ZVDD", "MSG_FTM_ZVDDD",
          "MSG_FTM_EI", "MSG_FTM_2HEI", "MSG_FTM_3HEI", "MSG_FTM_MZV"
        )

        import ollama

        DEFAULT_MODEL = (
          "qwen3:32b",                 #  0  22 GB
          "gpt-oss:20b",               #  1  13 GB
          "llama3.3",                  #  2  45 GB
          "deepseek-r1:14b",           #  3   9 GB
          "deepseek-r1-qwen-14b",      #  4  15 GB
          "devstral:24b",              #  5  15 GB
          "qwen3-coder:30b",           #  6  18 GB
          "mistral-small-3.2",         #  7  14 GB
          "openthinker:32b",           #  8  19 GB
          "deepseek-v2",               #  9   9 GB
          "deepseek-coder-v2",         # 10   9 GB
          "llama3.2:3b-instruct-fp16"  # 11   6 GB
        )[0]

        llm_model = args.model if args.model else DEFAULT_MODEL

        def get_system_prompt(args, sect):
            if sect == 'narrow':
                length_limit = "no more than 18 characters long! Use common abbreviations whenever necessary"
            elif sect == 'tall':
                length_limit = "no more than 3 strings of 20 characters. Use common abbreviations if necessary"
            else: # wide
                length_limit = "around the same length as the given example(s)"

            no_thinking = "" if args.dothink else "Do not think! Just translate.\n"

            system_prompt_text = f"""You are an expert in language translation in the context of 3D printing.
You will be given a list of existing translations and will be asked to provide a new translation in the given language.
When provided, the English (en) translation should be considered the most authoritative source.
Named variable substitutions are written as UPPERCASE_WITH_UNDERSCORES. Never translate or modify these!
The symbols `@`, `~`, `*`, `{{`, and `$` are special characters used for substitution. Never translate or modify these!
Your translations must be {length_limit}.
Assume that variable substitutions such as (MACHINE_NAME) are short strings for the purpose of character counting.
{no_thinking}For each translation requested, respond only with the translated string, no introduction, explanation, or assessment.
This clean output will be perfect for our use case."""

            return [{ 'role': 'system', 'content': system_prompt_text }]

        # Send a prompt to Ollama and return the reply text
        def prompt_with_ollama(SYSTEM_PROMPT, prompt:str):
            msg = [{ 'role': 'user', 'content': prompt }]
            response = ollama.chat(model=llm_model, messages=SYSTEM_PROMPT + msg, stream=False)
            reply = response['message']['content'].strip('\n')
            reply = re.sub(r'<think>[\s\S]+</think>\n*', '', reply)
            reply = re.sub(r'(^"|"$)', '', reply)
            return reply

        # For each named string fill in any missing translations
        for sect in ('narrow','wide','tall'):
            system_prompt = get_system_prompt(args, sect)
            for name in names.keys():
                if name in NEVER_TRANSLATE_NAMES: continue
                en_string = language_strings['en'][sect][name] if name in language_strings['en'][sect] else ""
                glyphs = len(en_string)

                done = {} # All existing translations for the given name
                todo = [] # Missing translation keys to create
                for lang in langcodes:
                    strings = language_strings[lang]
                    if name in strings[sect]:
                        done[lang] = strings[sect][name]
                    elif glyphs >= MIN_TRANSLATE_LEN and lang not in NEVER_TRANSLATE_LANGS:
                        todo += [lang]

                # For each untranslated language, fill in a translation
                for lang in todo:
                    # Show existing translations to the LLM and ask for one more
                    prompt = [ f"Please translate the following string into {language_name(lang)} ({lang})." ]
                    if lang.endswith("_na"):
                        prompt += [ "(Substitute plain unaccented ASCII characters for accented characters in the output.)" ]
                    prompt += [ "Here are the existing translations:" ]
                    for dlang in done.keys():
                        prompt += [ f"- {dlang} {language_name(dlang)}: \"{done[dlang]}\"" ]
                    prompt = '\n'.join(prompt)
                    #print(f"Prompt: {prompt}")
                    reply = prompt_with_ollama(system_prompt, prompt)
                    newstring = reply.replace('–','-').replace('‑','-').replace('／','/').replace('’',"'").replace('…','...').replace('\u202F',' ').replace('\uFEFF', '').replace('！', '! ').replace('。', '. ').replace('ç','ç').replace('ş','ş').replace('６','6').replace('＠', '@').replace('～', '~')
                    newstring = re.sub(r'([!.]) $', '\1', newstring)
                    if newstring != en_string:
                        print(f"{name} ({lang}) = \"{newstring}\"")
                        done[lang] = newstring
                        if not sect in language_strings[lang]: language_strings[lang][sect] = {}
                        language_strings[lang][sect][name] = newstring
                    else:
                        print(f"{name} ({lang}) = (same as English)")

    # Write a single language entry to the CSV file with narrow, wide, and tall strings
    def write_csv_lang(f, strings, name):
        f.write(',')
        if name in strings['narrow']: f.write('"%s"' % strings['narrow'][name])
        f.write(',')
        if name in strings['wide']: f.write('"%s"' % strings['wide'][name])
        f.write(',')
        if name in strings['tall']: f.write('"%s"' % strings['tall'][name])

    if args.single:
        #
        # Export one large sheet containing all specified languages
        #
        with open("languages.csv", 'w', encoding='utf-8') as f:
            header = ['name']
            for lang in langcodes:
                lname = lang + ' ' + language_name(lang)
                header += [lname, lname + ' (wide)', lname + ' (tall)']
            f.write('"' + '","'.join(header) + '"\n')

            for name in names.keys():
                f.write('"' + name + '"')
                for lang in langcodes: write_csv_lang(f, language_strings[lang], name)
                f.write('\n')
    else:
        #
        # Export a separate sheet for each language
        #
        OUTDIR.mkdir(exist_ok=True)

        for lang in langcodes:
            with open(OUTDIR / f"language_{lang}.csv", 'w', encoding='utf-8') as f:
                lname = lang + ' ' + language_name(lang)
                header = ['name', lname, lname + ' (wide)', lname + ' (tall)']
                f.write('"' + '","'.join(header) + '"\n')

                for name in names.keys():
                    f.write('"' + name + '"')
                    write_csv_lang(f, language_strings[lang], name)
                    f.write('\n')

if __name__ == "__main__":
    # Check for the path to the language files
    if not Path(LANGHOME).is_dir():
        print(f"Error: Couldn't find the '{LANGHOME}' directory.")
        print("Edit LANGHOME or cd to the root of the repo before running.")
        exit(1)

    # Parse and validate all arguments
    parser = argparse.ArgumentParser(description="Export LCD language strings to CSV with optional translation")
    parser.add_argument('-l', '--languages', action="append", default=None, help="Languages to translate (otherwise all)")
    parser.add_argument('-s', '--single', action="store_true", help="Output a single spreadsheet (languages.csv)")
    parser.add_argument('-v', '--verbose', action="store_true", help="Extra output for debugging")
    parser.add_argument('-n', '--limit', default=0, help="Limit the number of exported items")
    parser.add_argument('-t', '--translate', action="store_true", help="Use an LLM to translate strings")
    parser.add_argument('-d', '--dothink', action="store_true", help="Use thinking if the model supports it")
    parser.add_argument('-m', '--model', default=None, help="Override the default LLM model for translation")
    args = parser.parse_args()

    if not args.translate:
        if args.model: print("--model ignored when not translating")
        if args.dothink: print("--dothink ignored when not translating")

    language_export(args)
