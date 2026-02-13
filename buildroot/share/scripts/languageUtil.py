#!/usr/bin/env python3
#
# languageUtil.py
#

# A dictionary to contain language names
LANGNAME = {
  'an':      { 'size':1, 'iso': "1",     'name':"Aragonese", 'noext':1 },
  'bg':      { 'size':2, 'iso': "5",     'name':"Bulgarian" },
  'ca':      { 'size':2,                 'name':"Catalan" },
  'cz':      { 'size':2, 'iso': "CZ",    'name':"Czech" },
  'da':      { 'size':2, 'iso': "1",     'name':"Danish" },
  'de':      { 'size':2,                 'name':"German" },
  'el':      { 'size':2, 'iso': "GREEK", 'name':"Greek" },
  'el_CY':   { 'size':2,                 'name':"Greek (Cyprus)" },
  'el_gr':   { 'size':2, 'iso': "GREEK", 'name':"Greek (Greece)" },
  'en':      { 'size':2,                 'name':"English" },
  'es':      { 'size':2,                 'name':"Spanish" },
  'eu':      { 'size':1, 'iso': "1",     'name':"Basque-Euskera", 'noext':1 },
  'fi':      { 'size':2, 'iso': "1",     'name':"Finnish" },
  'fr':      { 'size':2, 'iso': "1",     'name':"French" },
  'fr_na':   { 'size':1, 'iso': "1",     'name':"French (no accent)", 'noext':1 },
  'gl':      { 'size':1, 'iso': "1",     'name':"Galician" },
  'hr':      { 'size':2, 'iso': "1",     'name':"Croatian (Hrvatski)" },
  'hu':      { 'size':2,                 'name':"Hungarian / Magyar" },
  'it':      { 'size':1, 'iso': "1",     'name':"Italian" },
  'jp_kana': { 'size':3, 'iso': "KANA",  'name':"Japanese (Kana)" },
  'ko_KR':   { 'size':1,                 'name':"Korean" },
  'nl':      { 'size':1, 'iso': "1",     'name':"Dutch", 'noext':1 },
  'pl':      { 'size':2, 'iso': "PL",    'name':"Polish" },
  'pt':      { 'size':2, 'iso': "1",     'name':"Portuguese" },
  'pt_br':   { 'size':2,                 'name':"Portuguese (Brazil)" },
  'ro':      { 'size':2,                 'name':"Romanian" },
  'ru':      { 'size':2, 'iso': "5",     'name':"Russian" },
  'sk':      { 'size':2, 'iso': "SK",    'name':"Slovak" },
  'sv':      { 'size':2, 'iso': "1",     'name':"Swedish" },
  'tr':      { 'size':2, 'iso': "TR",    'name':"Turkish" },
  'uk':      { 'size':2, 'iso': "5",     'name':"Ukrainian" },
  'vi':      { 'size':2,                 'name':"Vietnamese" },
  'zh_CN':   { 'size':3,                 'name':"Simplified Chinese" },
  'zh_TW':   { 'size':3,                 'name':"Traditional Chinese" }
}

def infobyid(id, fld):
    if id in LANGNAME and fld in LANGNAME[id]:
        return LANGNAME[id][fld]
    return None

def language_name(id):       return infobyid(id, 'name') or '<unknown>'
def language_iso(id):        return infobyid(id, 'iso')
def language_charsize(id):   return infobyid(id, 'size')
def language_noext(id):      return infobyid(id, 'noext')
