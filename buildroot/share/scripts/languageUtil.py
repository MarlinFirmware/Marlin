#!/usr/bin/env python3
#
# languageUtil.py
#

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

def namebyid(id):
    if id in LANGNAME: return LANGNAME[id]
    return '<unknown>'
