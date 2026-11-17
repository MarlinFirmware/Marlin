#!/usr/bin/env python3
#
# Regression test for buildroot/share/PlatformIO/scripts/schema.py
#
# Usage: python3 buildroot/tests/schema-test.py   (run from the repo root)
#
# A /* ... */ comment that opens and closes on one line (as in a multi-line #define
# joined by '\') must not start a block comment, or the following lines - including
# #endif - get swallowed and the enclosing #if leaks onto later options.
#
import os, sys, tempfile
from pathlib import Path

scripts = Path(__file__).resolve().parent.parent / 'share' / 'PlatformIO' / 'scripts'
sys.path.insert(0, str(scripts))
import schema

CONF = '#define SERIAL_PORT 0\n'
CONF_ADV = r'''
#if HAS_CHAMELEON
  #define CHAMELEON_SEQ \
    { 180,  +20, 1500 }, /* Cool to 180C */ \
    {   0,   -5,  500 }, \
    { 180,  -80,  500 }  /* Heat up again */
  //#define CHAMELEON_GCODE "M106 S255"
#endif // HAS_CHAMELEON

/**
 * @section stats
 */
#if ENABLED(PRINTCOUNTER)
  #define AFTER_COUNTER 1
#endif

#define AFTER_ALL 1 /* inline */
'''

def main():
    with tempfile.TemporaryDirectory() as td:
        os.chdir(td)
        Path('Marlin').mkdir()
        Path('Marlin/Configuration.h').write_text(CONF)
        Path('Marlin/Configuration_adv.h').write_text(CONF_ADV)
        sch = schema.extract()['advanced']
    opts = { n:i for sec in sch.values() for n,i in sec.items() }
    def req(n): return opts[n].get('requires') if isinstance(opts[n], dict) else opts[n][0].get('requires')
    assert req('CHAMELEON_SEQ') == '(HAS_CHAMELEON)', req('CHAMELEON_SEQ')
    assert req('CHAMELEON_GCODE') == '(HAS_CHAMELEON)', req('CHAMELEON_GCODE')
    assert req('AFTER_COUNTER') == '(ENABLED(PRINTCOUNTER))', req('AFTER_COUNTER')
    assert req('AFTER_ALL') is None, req('AFTER_ALL')
    print('schema-test: OK')

if __name__ == '__main__':
    main()
