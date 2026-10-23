#!/usr/bin/env python3
#
# buildroot/share/make/validate_conditionals.py
# Assert that preprocessor conditionals are well formed
#
# Catches mistakes the compiler cannot report, or reports only as a
# confusing cascade far from the cause:
#
#   - #if / #ifdef / #ifndef left unterminated at end of file
#   - #endif with no matching #if
#   - #else / #elif outside any conditional
#   - #elif after #else, or a second #else in the same block
#   - stray tokens after #endif / #else (invalid, silently tolerated by GCC)
#   - #ifdef / #ifndef given more than one macro name
#   - object-like #define whose body is juxtaposed literals, e.g.
#     "#define X_STEP_PIN P1 02" (missing underscore) or a duplicated value
#
# Usage: validate_conditionals.py [-v] [--selftest] [path ...]
#        Paths default to 'Marlin'. Exits 1 if any error is found.
#

import sys, os, re

do_log = False
def logmsg(msg, line):
    if do_log: print(msg, line)

def err(where, msg):
    print(f"[ERROR] {where:52} {msg}")

def warn(where, msg):
    print(f"[WARNING] {where:50} {msg}")

#
# Blank out comments and string / character literals, preserving line numbering
# so reported line numbers still match the file. Handles the two constructs
# that trip up naive scanners of this codebase:
#   - '/*' inside a string, e.g. 'variants/*/variant.h' in an #error message
#   - C++ digit separators, e.g. 250'000, which are not character literals
#
def strip_code(s):
    out = []; i = 0; n = len(s); state = 'code'
    while i < n:
        c, c2 = s[i], s[i:i+2]
        if state == 'code':
            if c2 == '/*': state = 'block'; i += 2; continue
            if c2 == '//': state = 'line'; i += 2; continue
            if c == '"': state = 'str'; out.append(' '); i += 1; continue
            if c == "'":
                prev = out[-1] if out else ''
                if prev.isalnum() or prev == '.':    # digit separator, not a literal
                    i += 1; continue
                state = 'chr'; out.append(' '); i += 1; continue
            out.append(c); i += 1; continue
        if state == 'block':
            if c2 == '*/': state = 'code'; i += 2; continue
            out.append('\n' if c == '\n' else ''); i += 1; continue
        if state == 'line':
            if c == '\n': state = 'code'; out.append('\n')
            i += 1; continue
        if c == '\\': out.append(' '); i += 2; continue
        if (state == 'str' and c == '"') or (state == 'chr' and c == "'"):
            state = 'code'; out.append(' '); i += 1; continue
        out.append('\n' if c == '\n' else ' '); i += 1; continue
    return ''.join(out)

OPEN   = re.compile(r'^\s*#\s*(if|ifdef|ifndef)\b(.*)$')
ELIF   = re.compile(r'^\s*#\s*(elif|elifdef|elifndef)\b')
ELSE   = re.compile(r'^\s*#\s*else\b(.*)$')
ENDIF  = re.compile(r'^\s*#\s*endif\b(.*)$')
IFDEFN = re.compile(r'^\s*#\s*(ifdef|ifndef|elifdef|elifndef)\s+(.*)$')
DEFINE = re.compile(r'^\s*#\s*define\s+([A-Za-z_]\w*)(\(|\s+)(.*)$')
NUMBER = re.compile(r'^[+-]?(0[xX][0-9a-fA-F]+|\d+\.?\d*)[uUlLfF]*$')
IDENT  = re.compile(r'^[A-Za-z_]\w*$')
TRAIL  = re.compile(r'^(//.*|/\*.*)?$')

#
# Check one file. Returns (errors, warnings).
#
def check_file(path):
    errs = warns = 0
    # 'utf-8-sig' drops a UTF-8 BOM, which Windows editors may add. A BOM is not
    # matched by \s, so leaving it in place would hide a directive on line 1.
    # Text mode maps CRLF and CR to \n, so DOS and old-Mac endings are fine.
    raw = open(path, encoding='utf-8-sig', errors='replace').read().split('\n')
    code = strip_code('\n'.join(raw)).split('\n')
    path = path.replace(os.sep, '/')             # report POSIX-style paths on Windows too

    stack = []                                      # [line, text, seen_else]
    for num, line in enumerate(code, 1):
        where = f'{path}:{num}'

        m = OPEN.match(line)
        if m:
            stack.append([num, line.strip(), False])
            continue

        if ELIF.match(line):
            if not stack:
                err(where, 'Stray #elif outside any conditional'); errs += 1
            elif stack[-1][2]:
                err(where, f'#elif after #else (block opened on line {stack[-1][0]})'); errs += 1
            continue

        if ELSE.match(line):
            if not stack:
                err(where, 'Stray #else outside any conditional'); errs += 1
            elif stack[-1][2]:
                err(where, f'Second #else in one block (opened on line {stack[-1][0]})'); errs += 1
            else:
                stack[-1][2] = True
            continue

        if ENDIF.match(line):
            if stack:
                stack.pop()
            else:
                err(where, 'Extra #endif with no matching #if'); errs += 1
            continue

    for num, text, _ in stack:
        err(f'{path}:{num}', f'Unterminated conditional: {text[:56]}')
        errs += 1

    # Checks that need the original text, not the stripped copy
    for num, line in enumerate(raw, 1):
        where = f'{path}:{num}'

        for pat, name in ((ENDIF, '#endif'), (ELSE, '#else')):
            m = pat.match(line)
            if m and not TRAIL.match(m.group(m.lastindex).strip()):
                warn(where, f'Tokens after {name}: {m.group(m.lastindex).strip()[:40]}')
                warns += 1

        m = IFDEFN.match(code[num-1])
        if m and len(m.group(2).split()) > 1:
            warn(where, f'#{m.group(1)} takes one macro name: {m.group(2).strip()[:40]}')
            warns += 1

    # Object-like #define with a body of juxtaposed literals. Kept deliberately
    # tight: bare numbers, or one identifier followed by bare numbers. Anything
    # with an operator, bracket, comma or string is a normal macro body.
    for num, line in enumerate(code, 1):
        m = DEFINE.match(line)
        if not m or m.group(2) == '(': continue
        toks = m.group(3).strip().split()
        if len(toks) < 2: continue
        if all(NUMBER.match(t) for t in toks) \
        or (IDENT.match(toks[0]) and all(NUMBER.match(t) for t in toks[1:])):
            err(f'{path}:{num}', f'Juxtaposed values in #define {m.group(1)}: {" ".join(toks)[:40]}')
            errs += 1

    return errs, warns

SOURCE_EXT = ('.h', '.hpp', '.c', '.cpp', '.ino')

def gather(paths):
    found = []
    for p in paths:
        if os.path.isfile(p):
            found.append(p)
            continue
        for root, dirs, files in os.walk(p):
            dirs[:] = [d for d in dirs if d not in ('.git', '.pio')]
            found += [os.path.join(root, f) for f in sorted(files) if f.endswith(SOURCE_EXT)]
    return sorted(found)

#
# Verify the checker itself against known-bad and known-good input, so a
# lexer regression can't quietly turn into a clean report.
#
def selftest():
    import tempfile
    bad = '''#if ENABLED(A)
  #define X 1
#if ENABLED(B)
  #define X 2
#endif
'''
    worse = '''#if A
#else
#elif B
#endif
#endif Z
#ifdef A B
#define P1_02_PIN P1 02
'''
    # Every construct here has tripped up a naive checker on this codebase
    good = '''/* #if in a block comment */
// #if in a line comment
#error "path/*/glob.h is not a comment"
static int n = 250'000;
static float f = 0.000'750f;
#define LIST { 1, 2, 3 }
#define EXPR (A + B)
#define CALL _EPIN(I_E_INDEX, STEP)
#define TWOSTR "a" "b"
#if REAL
#else
#endif // REAL
'''
    # Windows-flavoured input: CRLF endings, and a UTF-8 BOM ahead of line 1
    crlf = bad.replace('\n', '\r\n')
    bom = '\ufeff' + bad
    bom_crlf = '\ufeff' + crlf

    import io, contextlib
    cases = (('bad.h', bad, 1, 0), ('worse.h', worse, 4, 2), ('good.h', good, 0, 0),
             ('crlf.h', crlf, 1, 0), ('bom.h', bom, 1, 0), ('bom_crlf.h', bom_crlf, 1, 0))
    fails = 0
    with tempfile.TemporaryDirectory() as td:
        for name, text, want_e, want_w in cases:
            f = os.path.join(td, name)
            with open(f, 'wb') as fh: fh.write(text.encode('utf-8'))
            with contextlib.redirect_stdout(io.StringIO()):     # findings are expected here
                e, w = check_file(f)
            ok = (e, w) == (want_e, want_w)
            if not ok: fails += 1
            print(f'  {name:8} errors={e} warnings={w}  expected {want_e}/{want_w}  {"OK" if ok else "FAIL"}')
    print('Self-test ' + ('failed' if fails else 'passed'))
    return fails

def main(argv):
    global do_log
    paths = []
    for arg in argv:
        if arg == '-v': do_log = True
        elif arg == '--selftest':
            sys.exit(1 if selftest() else 0)
        else: paths.append(arg)

    files = gather(paths or ['Marlin'])
    logmsg('Files to check:', len(files))

    errs = warns = 0
    for f in files:
        e, w = check_file(f)
        errs += e; warns += w

    print(f'Checked {len(files)} files, {errs} errors, {warns} warnings')
    return errs

if __name__ == '__main__':
    if main(sys.argv[1:]):
        sys.exit(1)
