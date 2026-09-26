#!/usr/bin/env python3
'''
config-migrate.py - Upgrade or downgrade Configuration.h / Configuration_adv.h

Carry a customized configuration from one Marlin version to another by
replaying the user's changes onto the stock files of the target version.

  1. Find the stock base the user's files started from (the closest match
     among all commits and tags that touched the config files), or use --from.
  2. Diff base -> user to get the user's intent: options enabled/disabled,
     values changed, trailing comments edited, and lines added.
  3. Apply that intent to the stock files of the target (--to), following
     option renames found in Changes.h and in the git history between the two.

Rules for user changes:
  - Changed values always win. If upstream also changed the default, the
    conflict is listed in the report.
  - Options the target no longer has (and no known rename) are moved,
    commented out, into a block at the end of the file.
  - Unknown lines the user added are inserted after the same stock line they
    followed originally, or go to the end block if that line is gone.
  - Trailing comments on #define lines are kept if the user changed them.
    Other comment edits are dropped in favor of the target's comments.

Requires a full git clone of Marlin with tags (not a ZIP download):
  git clone https://github.com/MarlinFirmware/Marlin.git

Usage:
  config-migrate.py [CONFIG_DIR] --to REV [--from REV] [-o OUT_DIR | --in-place]

  CONFIG_DIR   Folder with the user's Configuration.h / Configuration_adv.h
               (default: Marlin)
  --to REV     Target version: tag, branch or commit (default: HEAD)
  --from REV   Stock version the user's files are based on (default: detect)
  -o OUT_DIR   Where to write the migrated files (default: CONFIG_DIR/migrated)
  --in-place   Overwrite the files in CONFIG_DIR (originals saved as .bak)
  --report F   Also write the report to file F
  --repo DIR   Marlin git repository (default: the one containing this script)
'''

import argparse, difflib, re, subprocess, sys
from pathlib import Path

FILES = ('Configuration.h', 'Configuration_adv.h')
GIT_DIR = 'Marlin/'
CHANGES = ('Marlin/src/inc/Changes.h', 'Marlin/src/inc/SanityCheck.h')
COPYRIGHT_RE = re.compile(r'(.*Copyright \(c\) )(\d{4}(?:\s*-\s*\d{4})?)(.*)')
IGNORE = {'CONFIGURATION_H_VERSION', 'CONFIGURATION_ADV_H_VERSION'}

DEFINE_RE = re.compile(r'^(\s*)(//\s*)?#define\s+([A-Za-z_]\w*)(\([^)]*\))?(.*)$', re.S)
COND_RE = re.compile(r'^\s*#\s*(if|ifdef|ifndef|elif|else|endif)\b(.*)')
QUICK_RE = re.compile(r'^[ \t]*(//)?[ \t]*#define[ \t]+(\w+)(\([^)]*\))?[ \t]*([^\n]*)$', re.M)
ERROR_RE = re.compile(r'#error\s+"([^"]+)"')

def squash(s): return ' '.join(s.split())

def short(s, n=50):
    s = squash(s.replace('\\\n', ' '))
    return s if len(s) <= n else s[:n - 3] + '...'

def split_comment(s):
    '''Split code from a trailing // or /* comment, ignoring quoted text.'''
    quote = None
    for i, c in enumerate(s):
        if quote:
            if c == '\\': continue
            if c == quote: quote = None
        elif c in '"\'':
            quote = c
        elif s.startswith('//', i) or s.startswith('/*', i):
            return s[:i], s[i:]
    return s, ''

class Define:
    def __init__(self, fname, idx, occ, ctx, m, line):
        self.file, self.idx, self.occ, self.ctx, self.raw = fname, idx, occ, ctx, line
        self.indent, self.enabled, self.name, self.params = m[1], not m[2], m[3], m[4] or ''
        rest = m[5]
        if '\n' in rest:            # Multi-line value, keep it whole
            code, self.comment = rest, ''
        else:
            code, self.comment = split_comment(rest)
            self.comment = self.comment.rstrip()
        self.value = code.strip()
        self.gap = code[:len(code) - len(code.lstrip())] if self.value else ' '
        self.ccol = len(line) - len(rest) + len(code) if self.comment else 0

    @property
    def key(self): return (self.name, self.occ)

    def render(self, enabled, value, comment, like=None):
        '''This define with new state, keeping its layout (or the layout of 'like').'''
        if '\n' in value or '\n' in self.value:
            if value == self.value:
                body = self.raw.lstrip()
                if body.startswith('//'): body = body[2:].lstrip()
                return self.indent + ('' if enabled else '//') + body
            value = value.strip()
        lay = like or self
        code = self.indent + ('' if enabled else '//') + '#define ' + self.name + self.params
        if value: code += (lay.gap or ' ') + value
        if comment:
            col = lay.ccol + len(self.name) - len(lay.name) + (enabled - lay.enabled) * -2 if like and like.ccol else self.ccol
            code += ' ' * max(1, col - len(code)) + comment
        return code

class ConfigFile:
    '''A config file as logical lines (continuations joined) with parsed defines.'''
    def __init__(self, fname, text):
        self.name = fname
        self.nl = '\r\n' if '\r\n' in text else '\n'
        raw = text.replace('\r\n', '\n').split('\n')
        self.final_nl = raw[-1] == ''
        if self.final_nl: raw.pop()
        self.lines, self.defs, self.at, self.names = [], [], {}, {}
        self.phys = []                  # logical line index -> 1-based physical line number
        ctx, block, i = [], False, 0
        while i < len(raw):
            line = raw[i]; i += 1
            if block:
                self.phys.append(i)
                self.lines.append(line)
                if '*/' in line: block = False
                continue
            while line.endswith('\\') and i < len(raw):
                line += '\n' + raw[i]; i += 1
            idx = len(self.lines)
            self.phys.append(i - line.count('\n'))
            self.lines.append(line)
            s = line.lstrip()
            if s.startswith('/*') and '*/' not in s[2:]:
                block = True
                continue
            m = COND_RE.match(line)
            if m:
                kw, cond = m[1], squash(split_comment(m[2])[0])
                if kw.startswith('if'): ctx.append(f'{kw} {cond}')
                elif kw == 'endif': ctx and ctx.pop()
                elif ctx: ctx[-1] = f'{kw} {cond}'
                continue
            m = DEFINE_RE.match(line)
            if m:
                occ = len(self.names.get(m[3], []))
                d = Define(fname, idx, occ, ' / '.join(ctx), m, line)
                self.defs.append(d)
                self.at[idx] = d
                self.names.setdefault(d.name, []).append(d)
        self.norms = [f'#define {self.at[i].name}#{self.at[i].occ}' if i in self.at else squash(l)
                      for i, l in enumerate(self.lines)]
        self.keys = { d.key: d for d in self.defs }

def signature(text):
    '''Cheap fingerprint of a config file for base detection.'''
    sig, occ = set(), {}
    for m in QUICK_RE.finditer(text):
        name = m[2]
        n = occ.get(name, 0); occ[name] = n + 1
        sig.add((name, n, not m[1], squash(split_comment(m[4])[0])))
    return sig, set(squash(l) for l in text.splitlines() if l.strip() and '#define' not in l)

def distance(a, b):
    return len(a[0] ^ b[0]) + len(a[1] ^ b[1]) / 100

#
# Git access
#
class Git:
    def __init__(self, repo):
        self.repo = repo
        self.batch = None

    def check(self):
        '''Exit with advice if the repo lacks the git history this tool needs.'''
        clone = ('This tool reads the history of the Marlin configuration files from git,\n'
                 'so it needs a git clone of Marlin, not a ZIP download:\n\n'
                 '  git clone https://github.com/MarlinFirmware/Marlin.git\n\n'
                 'Then run this script from the clone, or point to it with --repo DIR.')
        try:
            r = subprocess.run(['git', '-C', str(self.repo), 'rev-parse', '--is-shallow-repository'],
                               capture_output=True, text=True, encoding='utf-8', errors='replace')
        except FileNotFoundError:
            sys.exit('git is not installed or not on the PATH.\n\n' + clone)
        if r.returncode:
            sys.exit(f'{self.repo} is not a git repository (was Marlin downloaded as a ZIP?).\n\n' + clone)
        if r.stdout.strip() == 'true':
            sys.exit(f'{self.repo} is a shallow clone without the full history. Fetch it with:\n\n'
                     f'  git -C {self.repo} fetch --unshallow --tags')
        if not self.run('tag', '--list').stdout.strip():
            print(f'Warning: {self.repo} has no tags, so versions like 2.1.2.5 cannot be named.\n'
                  f'Fetch them with:  git -C {self.repo} fetch --tags', file=sys.stderr)

    def run(self, *args, check=True):
        r = subprocess.run(['git', '-C', str(self.repo), *args], capture_output=True,
                           text=True, encoding='utf-8', errors='replace')
        if check and r.returncode:
            sys.exit(f'git {" ".join(args)}: {r.stderr.strip()}')
        return r

    def show(self, rev, path):
        r = self.run('show', f'{rev}:{path}', check=False)
        return None if r.returncode else r.stdout

    def blob(self, sha):
        '''Read a blob through a persistent cat-file process.'''
        if not self.batch:
            self.batch = subprocess.Popen(['git', '-C', str(self.repo), 'cat-file', '--batch'],
                                          stdin=subprocess.PIPE, stdout=subprocess.PIPE)
        self.batch.stdin.write(sha.encode() + b'\n'); self.batch.stdin.flush()
        size = int(self.batch.stdout.readline().split()[2])
        data = self.batch.stdout.read(size + 1)[:-1]
        return data.decode('utf-8', 'replace')

    def rev(self, rev): return self.run('rev-parse', '--verify', f'{rev}^{{commit}}').stdout.strip()

    def same_tags(self, sha, paths):
        '''Tags whose files at paths are identical to those at sha.'''
        tags = self.run('tag').stdout.split()
        q = [f'{r}:{p}' for r in [sha] + tags for p in paths]
        out = subprocess.run(['git', '-C', str(self.repo), 'cat-file', '--batch-check=%(objectname)'],
                             input='\n'.join(q) + '\n', capture_output=True,
                             text=True, encoding='utf-8', errors='replace').stdout.split('\n')
        n = len(paths)
        want = out[:n]
        return [t for i, t in enumerate(tags) if out[(i + 1) * n:(i + 2) * n] == want]

    def describe(self, sha):
        r = self.run('describe', '--tags', '--exact-match', sha, check=False)
        return r.stdout.strip() if r.returncode == 0 else sha[:10]

    def is_ancestor(self, a, b): return self.run('merge-base', '--is-ancestor', a, b, check=False).returncode == 0

    def date(self, sha): return int(self.run('show', '-s', '--format=%ct', sha).stdout)

#
# Base detection
#
def detect_base(git, user, target):
    '''Find the commit whose stock config files are closest to the user's.'''
    paths = [GIT_DIR + f for f in user]
    log = git.run('log', '--format=C %H', '--raw', '--no-abbrev', 'HEAD', '--tags', '--', *paths).stdout
    blobs = { f: {} for f in user }     # file -> blob -> [commits introducing it]
    commit = None
    for line in log.splitlines():
        if line.startswith('C '):
            commit = line[2:]
        elif line.startswith(':'):
            meta, path = line.split('\t')[:2]
            sha = meta.split()[3]
            f = path[len(GIT_DIR):]
            if f in blobs and set(sha) != {'0'}:
                blobs[f].setdefault(sha, []).append(commit)

    usig = { f: signature(t) for f, t in user.items() }
    scores = {}                         # blob -> distance
    stock_lines = {}                    # file -> every line seen in candidate stock versions
    for f, bl in blobs.items():
        uver = re.search(r'#define\s+CONFIGURATION_(ADV_)?H_VERSION\s+(\w+)', user[f])
        texts = { sha: git.blob(sha) for sha in bl }
        lines = stock_lines[f] = set()
        for t in texts.values(): lines.update(squash(l) for l in t.splitlines())
        if uver and any(uver[0] in t for t in texts.values()):
            texts = { sha: t for sha, t in texts.items() if uver[0] in t }
        for sha, t in texts.items():
            scores[sha] = distance(usig[f], signature(t))

    # Score whole commits built from the best blobs of each file
    cands = set()
    for f, bl in blobs.items():
        best = sorted((s for s in bl if s in scores), key=scores.get)[:8]
        for s in best: cands.update(bl[s])
    ranked = []
    for c in cands:
        total = 0
        for f in user:
            r = git.run('rev-parse', f'{c}:{GIT_DIR}{f}', check=False)
            sha = r.stdout.strip()
            if r.returncode or sha not in scores:
                if sha and not r.returncode:
                    scores[sha] = distance(usig[f], signature(git.blob(sha)))
                else:
                    total = None; break
            total += scores[sha]
        if total is not None:
            ranked.append((total, c))
    if not ranked: sys.exit('Could not find a stock base for these files. Use --from.')
    top = min(t for t, _ in ranked)
    ties = [c for t, c in ranked if t == top]
    order = lambda c: (git.is_ancestor(c, target), git.describe(c) != c[:10], git.date(c))
    pick = sorted(ties, key=order)[-1]

    # A file that fits another version better (e.g. an old Configuration.h
    # beside a current Configuration_adv.h) gets its own base
    bases = {}
    for f, bl in blobs.items():
        own = git.run('rev-parse', f'{pick}:{GIT_DIR}{f}', check=False).stdout.strip()
        best = min((s for s in bl if s in scores), key=scores.get, default=None)
        if best and scores[best] < scores.get(own, 1e9):
            bases[f] = sorted(bl[best], key=order)[-1]
        else:
            bases[f] = pick
    return pick, top, bases, stock_lines

#
# Renames
#
def expand(pattern):
    '''Expand [XYZ], [0-7] and (A|B) groups into (choices, name) pairs.'''
    m = re.search(r'\[([A-Z0-9](?:-[A-Z0-9])?[A-Z0-9]*)\]|\(([A-Z0-9_|]+)\)', pattern)
    if not m: return [((), pattern)]
    if m[1]:
        alts = [chr(c) for c in range(ord(m[1][0]), ord(m[1][2]) + 1)] if '-' in m[1] else list(m[1])
    else:
        alts = m[2].split('|')
    out = []
    for a in alts:
        for ch, name in expand(pattern[:m.start()] + a + pattern[m.end():]):
            out.append(((a,) + ch, name))
    return out

NAME = r'[A-Z][A-Za-z0-9_\[\]\(\)|-]*[A-Za-z0-9_\]\)]'
RENAME_RES = [
    re.compile(rf'^({NAME})(?: settings| \([^)]*\))? (?:is now(?: just| called)?|has been renamed(?: to)?|have been renamed(?: to)?|was renamed(?: to)?|has been replaced by|is replaced with|replaced with)(?: \([^)]*\))? ({NAME})(.*)$'),
    re.compile(rf'^({NAME}) is deprecated\. Use ({NAME}) instead\.?$'),
    re.compile(rf'^({NAME}(?:(?:, |,? and |/){NAME})+) are now(?: \([^)]*\))? ({NAME}(?:(?:, |,? and |/){NAME})+)\.?$'),
]
LIST_SEP = re.compile(r', and |, | and |/')

def is_option(name): return re.fullmatch(r'[A-Za-z_]\w*', name) and ('_' in name or name.isupper())

def changes_renames(text):
    '''Parse "X is now Y" style messages. Returns {old: (new, note)}, all messages, and the plain renames.'''
    renames, pure, msgs = {}, set(), [' '.join(m.split()) for m in ERROR_RE.findall(text or '')]
    for msg in msgs:
        for n, rx in enumerate(RENAME_RES):
            m = rx.match(msg)
            if not m: continue
            if n < 2:
                pairs, note = [(m[1], m[2])], (m[3].strip() if n == 0 else '')
                if note == '.': note = ''
                if note or re.match(rf'^{re.escape(m[1])} \(', msg): note = msg
            else:
                olds, news = LIST_SEP.split(m[1]), LIST_SEP.split(m[2])
                if len(olds) != len(news): break
                pairs, note = list(zip(olds, news)), ''
            for old, new in pairs:
                olds, news = expand(old), expand(new)
                if len(news) == 1:
                    mapping = [(o, news[0][1]) for _, o in olds]
                else:
                    nd = dict(news)
                    mapping = [(o, nd[ch]) for ch, o in olds if ch in nd]
                for o, nn in mapping:
                    if is_option(o) and is_option(nn) and o != nn:
                        renames[o] = (nn, note or (msg if len(olds) > 1 and len(news) == 1 else ''))
                        if not renames[o][1]: pure.add(msg)
            break
    return renames, msgs, pure

def msg_names(msg):
    '''Regexes for the option names a Changes.h message talks about.'''
    out = []
    for tok in re.findall(r'[A-Z0-9_*\[\]()|-]+', msg):
        tok = tok.strip('().-')
        if '_' not in tok and '*' not in tok: continue
        rx = re.escape(tok).replace(r'\*', r'\w*').replace(r'\[AXIS\]', r'\w+').replace(r'\[TYPE\]', r'\w+')
        rx = re.sub(r'\\\[([A-Z0-9\\-]+)\\\]', lambda m: '[' + m[1].replace('\\', '') + ']', rx)
        rx = re.sub(r'\\\(([A-Z0-9_\\|]+)\\\)', lambda m: '(?:' + m[1].replace('\\', '') + ')', rx)
        try: out.append(re.compile(rx + '$'))
        except re.error: pass
    return out

def history_renames(git, older, newer):
    '''Guess renames from commits that remove one option and add another in the same hunk.'''
    if git.is_ancestor(older, newer):
        rng = [f'{older}..{newer}']
    else:
        rng = [f'--since={git.date(older)}', newer]
    log = git.run('log', '-p', '--reverse', '--format=@@@COMMIT %h %s', *rng, '--', *(GIT_DIR + f for f in FILES)).stdout
    found = []
    def flush(commit, hunks):
        if not commit: return
        allrem = set().union(*(h[0] for h in hunks)) if hunks else set()
        alladd = set().union(*(h[1] for h in hunks)) if hunks else set()
        for rem, add, remv, addv in hunks:
            rs = [n for n in rem if n not in alladd]
            ns = [n for n in add if n not in allrem]
            pairs = sorted(((difflib.SequenceMatcher(None, o, n).ratio(), o, n) for o in rs for n in ns), reverse=True)
            used = set()
            for ratio, o, n in pairs:
                if o in used or n in used or ratio < 0.5: continue
                used.update((o, n))
                same = o.upper().replace('_', '') == n.upper().replace('_', '')
                if remv[o] or addv[n]: strong = remv[o] == addv[n]
                else: strong = o.split('_')[-1].upper() == n.split('_')[-1].upper()
                found.append((o, n, commit, same or strong))
    commit, hunks = None, []
    for line in log.splitlines():
        if line.startswith('@@@COMMIT '):
            flush(commit, hunks)
            commit, hunks = line[10:], []
        elif line.startswith('@@'):
            hunks.append(([], [], {}, {}))
        elif hunks and line[:1] in '-+' and not line.startswith(('---', '+++')):
            m = DEFINE_RE.match(line[1:])
            if m and m[3] not in IGNORE:
                h = hunks[-1]
                lst, vals = (h[0], h[2]) if line[0] == '-' else (h[1], h[3])
                if m[3] not in lst: lst.append(m[3])
                vals[m[3]] = squash(split_comment(m[5])[0])
    flush(commit, hunks)
    return found

def build_renames(git, base, target, b_names):
    '''Rename map from names in the base to names in the target.'''
    upgrade = git.date(base) <= git.date(target)
    older, newer = (base, target) if upgrade else (target, base)
    renames = {}
    for path in CHANGES:
        text = git.show(newer, path)
        if text and ERROR_RE.search(text):
            ren, msgs, pure = changes_renames(text)
            old_msgs = set()
            for p in CHANGES: old_msgs.update(ERROR_RE.findall(git.show(older, p) or ''))
            for o, (n, note) in ren.items():
                src, dst = (o, n) if upgrade else (n, o)
                renames.setdefault(src, []).append((dst, 'Changes.h', note))
            old_msgs = set(' '.join(m.split()) for m in old_msgs)
            new_msgs = [(m, msg_names(m)) for m in msgs if m not in old_msgs and m not in pure]
            break
    else:
        new_msgs = []

    hints = {}
    def explain(name):
        '''Changes.h messages in this range that mention the option, or a rename hint.'''
        msgs = [m for m, rxs in new_msgs if any(rx.match(name) for rx in rxs)]
        return msgs or ([hints[name]] if name in hints else [])

    for o, n, commit, strong in history_renames(git, older, newer):
        src, dst = (o, n) if upgrade else (n, o)
        if src in renames: continue
        if strong and not any(not re.search(rf'\b{dst}\b', msg) for msg in explain(src)):
            renames[src] = [(dst, f'guess from {commit}', '')]
        else:
            hints[src] = f'possibly now {dst} ({commit})'

    def resolve(name):
        '''Shortest rename chain from name to a name the target has.'''
        if name in b_names: return None
        todo, seen = [(name, [])], {name}
        while todo:
            cur, path = todo.pop(0)
            for nxt, src, note in renames.get(cur, []):
                if nxt in seen: continue
                seen.add(nxt)
                step = path + [(nxt, src, note)]
                if nxt in b_names: return step
                todo.append((nxt, step))
        return None

    return resolve, explain, upgrade

#
# Renames that also change the meaning of the value
#
# Converters take (value, enabled) and return (value, enabled), or None if they can't.
def _state(v, on):
    m = re.fullmatch(r'(ENABLED|DISABLED)\((\w+)\)', v)
    if m: v = f'TERN({m[2]}, LOW, HIGH)' if m[1] == 'ENABLED' else f'TERN({m[2]}, HIGH, LOW)'
    else: v = {'true': 'LOW', 'false': 'HIGH'}.get(v)
    return (v, on) if v else None

def _invert(v, on):
    m = re.fullmatch(r'TERN\((\w+), *(LOW|HIGH), *(LOW|HIGH)\)', v)
    if m and m[2] != m[3]: v = f'ENABLED({m[1]})' if m[2] == 'LOW' else f'DISABLED({m[1]})'
    else: v = {'LOW': 'true', 'HIGH': 'false'}.get(v)
    return (v, on) if v else None

def _scale(k, ceil=False):
    def conv(v, on):
        try: x = float(v) * k
        except ValueError: return None
        if ceil: x = -(-x // 1)
        return (str(int(x)) if x == int(x) else f'{x:g}'), on
    return conv

def _to_flag(v, on): return ('', on and v == 'true') if v in ('true', 'false') else None
def _from_flag(v, on): return ('true' if on else 'false'), True

def _to_pin(v, on):
    m = re.fullmatch(r'_([A-Z]\d?)(MIN|MAX)_', v)
    return (f'{m[1]}_{m[2]}_PIN', on) if m else None
def _from_pin(v, on):
    m = re.fullmatch(r'([A-Z]\d?)_(MIN|MAX)_PIN', v)
    return (f'_{m[1]}{m[2]}_', on) if m else None

# (old name, new name, old->new value, new->old value)
VALUE_RULES = [
    (r'(\w+)_ENDSTOP_INVERTING', r'\1_ENDSTOP_HIT_STATE', _state, _invert),
    (r'INVERT_(\w+)_STEP_PIN',   r'STEP_STATE_\1',        _state, _invert),
    (r'DISABLE_INACTIVE_(\w+)',  r'DISABLE_IDLE_\1',      _to_flag, _from_flag),
    (r'(\w+)_USE_ENDSTOP',       r'\1_STOP_PIN',          _to_pin, _from_pin),
    (r'MINIMUM_STEPPER_PULSE',   r'MINIMUM_STEPPER_PULSE_NS', _scale(1000), _scale(0.001)),
    (r'TOUCH_IDLE_SLEEP',        r'DISPLAY_SLEEP_MINUTES',  _scale(1 / 60, True), _scale(60)),
]

def value_rule(name, upgrade):
    """(new name, value converter) if a known rule covers this name."""
    for old, new, fwd, back in VALUE_RULES:
        if upgrade:
            m = re.fullmatch(old, name)
            if m: return m.expand(new), fwd
        else:
            rx = re.sub(r'\\1', r'(\\w+)', new)
            m = re.fullmatch(rx, name)
            if m: return (old.replace(r'(\w+)', m[1]) if m.groups() else old), back
    return None

# Same name, new value format: (name, pattern of the new format, old->new, new->old)
def _thread_new(v):
    m = re.fullmatch(r'([345])([01])', v)
    return m and f'M{m[1]}_' + ('CW' if m[2] == '0' else 'CCW')
def _thread_old(v):
    m = re.fullmatch(r'M([345])_(CW|CCW)', v)
    return m and m[1] + ('0' if m[2] == 'CW' else '1')

FORMAT_RULES = [
    ('TRAMMING_SCREW_THREAD', r'M[345]_CC?W', _thread_new, _thread_old),
]

def same_value(a, b):
    """True if two values only differ in how they're written: 0.023/.023, (100)/100, 0/LOW."""
    def norm(v):
        v = squash(v)
        while v.startswith('(') and v.endswith(')') and v.count('(') == 1: v = v[1:-1].strip()
        v = { 'LOW': '0', 'HIGH': '1', 'false': '0', 'true': '1' }.get(v, v)
        try: return float(v)
        except ValueError: return v
    return norm(a) == norm(b)

def as_flag(value, enabled, target_value):
    """A true/false value for an option that is now a plain on/off flag."""
    if not squash(target_value) and squash(value) in ('true', 'false'):
        return '', enabled and squash(value) == 'true'
    return value, enabled

def format_value(name, value, target_value):
    """Convert value to the format target_value uses, if a rule knows how."""
    for rname, rx, fwd, back in FORMAT_RULES:
        if name != rname: continue
        v, tv = squash(value), squash(target_value)
        if re.fullmatch(rx, tv) and not re.fullmatch(rx, v): return fwd(v) or value
        if not re.fullmatch(rx, tv) and re.fullmatch(rx, v): return back(v) or value
    return value

# Separate options that became one array: (array option, old option for each element)
MERGE_RULES = [(f'PROBE_PT_{n}', [f'PROBE_PT_{n}_X', f'PROBE_PT_{n}_Y']) for n in (1, 2, 3)] + [
    ('HOMING_FEEDRATE_MM_M', ['HOMING_FEEDRATE_XY', 'HOMING_FEEDRATE_XY', 'HOMING_FEEDRATE_Z']),
]

def merge_rule(name):
    """(array option, old option per element) if name is one part of an option that became an array."""
    return next(((new, olds) for new, olds in MERGE_RULES if name in olds), None)

def split_rule(name):
    """Old option per element if name is an array option that used to be separate options."""
    return next((olds for new, olds in MERGE_RULES if name == new), None)

def pair_defs(a, u):
    '''Match each stock define to the user's copy of it. When the user has extra
    copies (e.g. commented out the stock line and added their own), prefer the
    active one in the same #if context. Returns {stock key: user define}.'''
    pair = {}
    for name, alist in a.names.items():
        ulist = u.names.get(name, [])
        if len(ulist) == len(alist):
            pair.update((ad.key, ud) for ad, ud in zip(alist, ulist))
            continue
        free = list(ulist)
        for ad in alist:
            if not free: break
            same = [d for d in free if d.ctx == ad.ctx] or free
            on = [d for d in same if d.enabled]
            ud = on[-1] if on else same[0]
            pair[ad.key] = ud
            free.remove(ud)
    return pair

#
# Migration
#
class Report:
    def __init__(self): self.sections = {}
    def add(self, section, text): self.sections.setdefault(section, []).append(text)
    def text(self, header):
        out = header[:]
        for sect in ('Applied', 'Renamed', 'Conflicts (your value kept)', 'Warnings', 'Defaults changed',
                     'Added lines placed', 'Moved to end block', 'Dropped'):
            if sect in self.sections:
                out += ['', f'{sect} ({len(self.sections[sect])}):'] + [f'  {s}' for s in self.sections[sect]]
        return '\n'.join(out) + '\n'

def is_directive(line): return line.lstrip().startswith('#')

def migrate(A, U, B, resolve, explain, report, orig_label, target_label, upgrade=True, stock_lines=None):
    '''A, U, B: {file: ConfigFile}. Returns {file: text}.'''
    b_names = {}
    for f in FILES:
        if f in B:
            for d in B[f].defs: b_names.setdefault(d.name, []).append(d)
    a_names = set(n for f in A for n in A[f].names)

    sets = {}           # (file, idx) -> (text, why)
    merges = {}         # (file, new name) -> (part names, {part name: user define})
    inserts = {}        # (file, idx) -> [lines]   insert after line idx (-1 = top)
    orphans = { f: [] for f in B }

    def find(name, fname, occ, ctx, count, value=None):
        cands = b_names.get(name)
        if not cands: return None
        same = [d for d in cands if d.file == fname] or cands
        if len(same) == count: return same[min(occ, len(same) - 1)]
        for d in same:
            if d.ctx == ctx: return d
        if value is not None:
            same = [d for d in same if ('{' in d.value) == ('{' in value)] or same
        return max(same, key=lambda d: (difflib.SequenceMatcher(None, d.ctx, ctx).ratio(), -abs(d.occ - occ)))

    def target_of(name, fname, occ, ctx, count, value=None):
        '''(target define, rename path, value converter)'''
        bd = find(name, fname, occ, ctx, count, value)
        if bd: return bd, None, None
        rule = value_rule(name, upgrade)
        if rule and rule[0] in b_names:
            return find(rule[0], fname, occ, ctx, count, value), [(rule[0], 'value converted', '')], rule[1]
        path = resolve(name)
        if path:
            return find(path[-1][0], fname, occ, ctx, count, value), path, None
        return None, None, None

    refs = {}
    def rename_refs(text, quiet=False):
        '''Update references to renamed options inside a value or added line.'''
        def sub(m):
            tok = m[0]
            if tok not in refs:
                path = None if tok in b_names or not is_option(tok) else resolve(tok)
                refs[tok] = path[-1][0] if path else tok
            return refs[tok]
        new = re.sub(r'\b[A-Za-z_]\w*\b', sub, text)
        if new != text and not quiet: report.add('Renamed', f'reference: {short(text, 40)} -> {short(new, 40)}')
        return new

    def orphan(fname, lines, why):
        orphans.setdefault(fname, []).append((lines, why))

    def apply(bd, enabled, value, comment, name, why, like=None):
        k = (bd.file, bd.idx)
        if k in sets and sets[k][1] != why:
            report.add('Warnings', f'{bd.name}: set by both {sets[k][1]} and {why}; kept the latter')
        text = bd.render(enabled, value, comment, like)
        if text != bd.raw: sets[k] = (text, why)

    for f in FILES:
        if f not in U: continue
        a, u = A[f], U[f]
        if f not in B:
            report.add('Warnings', f'{f} does not exist in {target_label}; your changes to it are not carried over')
            continue

        pair = pair_defs(a, u)
        paired = { ud.idx: ad for ad in a.defs if (ud := pair.get(ad.key)) }

        # Changes to stock options
        for ad in a.defs:
            if ad.name in IGNORE: continue
            ud = pair.get(ad.key)
            if ud and ud.enabled == ad.enabled and squash(ud.value) == squash(ad.value) \
                  and squash(ud.comment) == squash(ad.comment):
                # Left at the stock default: the target's default applies. List it if that default
                # differs (skipping guessed renames and values naming things the target doesn't have)
                bd, path, conv = target_of(ad.name, f, ad.occ, ad.ctx, len(a.names[ad.name]), ad.value)
                if not bd or path and any(src.startswith('guess') for _, src, _ in path): continue
                enabled, value = ad.enabled, rename_refs(ad.value, quiet=True)
                if conv:
                    r = conv(squash(value), enabled)
                    if r is None: continue
                    value, enabled = r
                else:
                    value = format_value(bd.name, value, bd.value)
                    value, enabled = as_flag(value, enabled, bd.value)
                known = b_names.keys() | {'LOW', 'HIGH', 'true', 'false'}
                if any(tok not in known for tok in re.findall(r'\b[A-Za-z_]\w*\b', re.sub(r'"[^"]*"|\'[^\']*\'', '', value))):
                    continue
                if (enabled or bd.enabled) and (enabled != bd.enabled or not same_value(value, bd.value)):
                    def state(on, v): return (short(v, 40) or 'on') if on else 'off' + (f' ({short(v, 30)})' if squash(v) else '')
                    report.add('Defaults changed', f'{bd.name}: was {state(enabled, value)}, now {state(bd.enabled, bd.value)}')
                continue
            if not ud and not ad.enabled: continue
            bd, path, conv = target_of(ad.name, f, ad.occ, ad.ctx, len(a.names[ad.name]), (ud or ad).value)
            if bd and conv and ud and conv(squash(ud.value), ud.enabled) is None:
                bd = None
                report.add('Warnings', f'{ad.name}: could not convert {short(ud.value)!r} for {path[0][0]}')
            if not bd and ud and (mr := merge_rule(ad.name)) and mr[0] in b_names:
                merges.setdefault((f, mr[0]), (mr[1], {}))[1][ad.name] = ud
                continue
            if not bd and ud and (olds := split_rule(ad.name)) and all(o in b_names for o in olds):
                vals = [v.strip() for v in ud.value.strip().strip('{}').split(',')]
                if len(vals) >= len(olds):
                    for o in dict.fromkeys(olds):
                        v = { vals[i] for i, n in enumerate(olds) if n == o }
                        if len(v) > 1: report.add('Warnings', f'{o}: {ad.name} has different values {sorted(v)}; used the first')
                        od = b_names[o][0]
                        apply(od, ud.enabled, vals[olds.index(o)], od.comment, o, f'{f}:{ad.name}')
                    report.add('Renamed', f'{ad.name} -> ' + ', '.join(dict.fromkeys(olds)) + ' (split)')
                    continue
            if not bd:
                if ud:
                    msgs = explain(ad.name)
                    orphan(f, [ud.raw], f'{ad.name}: ' + (' '.join(msgs) if msgs else f'not in {target_label}'))
                    for msg in msgs:
                        report.add('Warnings', f'{ad.name}: {msg}' if msg.startswith('possibly') else f'{ad.name}: Changes.h says "{msg}"')
                continue
            label = ad.name + (f'#{ad.occ + 1}' if len(a.names[ad.name]) > 1 else '')
            if path:
                report.add('Renamed', f'{label} -> ' + ' -> '.join(f'{n} ({src})' for n, src, _ in path))
                for n, src, note in path:
                    if note: report.add('Warnings', f'{label} -> {n}: check the value ({note})')
            enabled = ud.enabled if ud else False
            if conv and ud:
                enabled = conv(squash(ud.value), ud.enabled)[1]
            if ud and squash(ud.value) != squash(ad.value):
                value, was = rename_refs(ud.value), ad.value
                if conv:
                    value = conv(squash(value), ud.enabled)[0]
                    was = (conv(squash(was), ad.enabled) or (was,))[0]
                else:
                    value = format_value(bd.name, value, bd.value)
                    was = format_value(bd.name, was, bd.value)
                    value, enabled = as_flag(value, enabled, bd.value)
                if squash(bd.value) != squash(was) and squash(bd.value) != squash(value):
                    report.add('Conflicts (your value kept)',
                               f'{bd.name}: yours {short(value)!r}, stock was {short(was)!r}, now {short(bd.value)!r}')
            else:
                value = bd.value
            comment = ud.comment if ud and squash(ud.comment) != squash(ad.comment) else bd.comment
            if not ud:
                report.add('Applied', f'{label}: removed in your file, disabled')
            else:
                what = []
                if ud.enabled != ad.enabled: what.append('enabled' if ud.enabled else 'disabled')
                if squash(ud.value) != squash(ad.value): what.append(f'= {short(value)}')
                if comment is ud.comment and squash(ud.comment) != squash(ad.comment): what.append('comment')
                report.add('Applied', f'{label}: ' + ', '.join(what))
            like = ud if ud and squash(ud.value) != squash(ad.value) else None
            apply(bd, enabled, value, comment, ad.name, f'{f}:{ad.name}', like)
            # The target may split one option into #if/#else alternatives: set them all
            if value is not bd.value and len(a.names[ad.name]) == 1:
                for alt in b_names[bd.name]:
                    if alt is bd or alt.file != bd.file or alt.ctx.rsplit(' / ', 1)[0] != bd.ctx.rsplit(' / ', 1)[0]: continue
                    v = value
                    if alt.value.startswith('{') and alt.value.count(',') == 0 and not squash(v).startswith('{'): v = '{ ' + squash(v) + ' }'
                    elif not alt.value.startswith('{') and squash(v).startswith('{') and v.count(',') == 0: v = squash(v).strip('{} ')
                    apply(alt, enabled, v, alt.comment, ad.name, f'{f}:{ad.name}')

        # Separate options that are now one array
        for (mf, name), (names, parts) in merges.items():
            if mf != f: continue
            vals = []
            for n in names:
                d = parts.get(n) or pair.get((n, 0)) or (a.names[n][0] if n in a.names else None)
                vals.append(squash(d.value) if d else '0')
            bd = b_names[name][0]
            apply(bd, any(d.enabled for d in parts.values()), '{ ' + ', '.join(vals) + ' }', bd.comment, name, f'{f}:{name}')
            report.add('Renamed', ' + '.join(dict.fromkeys(names)) + f' -> {name} (merged)')

        # Lines the user added
        amap = {}
        for blk in difflib.SequenceMatcher(None, a.norms, B[f].norms, autojunk=False).get_matching_blocks():
            for k in range(blk.size): amap[blk.a + k] = blk.b + k

        def place(anchor, lines):
            for k in range(anchor, max(anchor - 30, -2), -1):
                if k == -1: return inserts.setdefault((f, -1), []).extend(lines) or 'top of file'
                if k in amap: return inserts.setdefault((f, amap[k]), []).extend(lines) or f'after line {amap[k] + 1}'
            for k in range(anchor + 1, min(anchor + 31, len(a.lines))):
                if k in amap: return inserts.setdefault((f, amap[k] - 1), []).extend(lines) or f'before line {amap[k] + 1}'
            return None

        def show(lines, n=4):
            lines = [squash(l) for l in lines if l.strip()]
            more = f'  (+{len(lines) - n} more lines)' if len(lines) > n else ''
            return '\n'.join(f'        {l[:110]}' for l in lines[:n]) + more

        stale_ifs = 0                   # stock #if lines dropped without their #endif
        unorms = [a.norms[paired[j].idx] if j in paired else ('+' + n if j in u.at else n) for j, n in enumerate(u.norms)]
        opcodes = difflib.SequenceMatcher(None, a.norms, unorms, autojunk=False).get_opcodes()
        for tag, i1, i2, j1, j2 in opcodes:
            if tag in ('equal', 'delete'): continue
            runs, run, edited = [], [], []
            stock = [a.lines[i] for i in range(i1, i2) if i not in a.at and a.lines[i].strip() and not is_directive(a.lines[i])]
            for j in range(j1, j2):
                line, ud = u.lines[j], u.at.get(j)
                if ud and j in paired or ud and ud.name in IGNORE:
                    keep = False
                elif ud and ud.name in a_names:
                    # Extra copy of a stock option
                    keep = False
                    if ud.name in b_names:
                        keep = True
                    else:
                        bd, path, conv = target_of(ud.name, f, 0, ud.ctx, 1)
                        if bd and not conv:
                            line = re.sub(rf'\b{ud.name}\b', bd.name, line, count=1)
                            keep = True
                        elif ud.enabled:
                            orphan(f, [line], f'{ud.name}: not in {target_label}')
                        else:
                            report.add('Dropped', f'{f} line {u.phys[j]}: commented-out copy of an option {target_label} no longer has\n' + show([line]))
                elif ud:
                    bd, path, conv = target_of(ud.name, f, ud.occ, ud.ctx, len(u.names[ud.name]))
                    if bd and not conv:
                        apply(bd, ud.enabled, rename_refs(ud.value), ud.comment or bd.comment, ud.name, f'{f}:{ud.name} (added)', ud)
                        report.add('Applied', f'{ud.name}: not in base, set in {target_label}')
                        keep = False
                    else:
                        keep = True
                elif is_directive(line) or not line.strip() or not stock:
                    keep = True
                else:
                    keep = False
                    edited.append(j)
                if keep:
                    if not run: run_start = j
                    run.append(rename_refs(line) if line.lstrip().startswith('#') else line)
                elif run:
                    runs.append((run_start, run)); run = []
            if run: runs.append((run_start, run))
            # A changed copyright year is kept on the target's matching copyright line
            for j in list(edited):
                m = COPYRIGHT_RE.match(u.lines[j])
                if not m: continue
                for i, bl in enumerate(B[f].lines):
                    bm = COPYRIGHT_RE.match(bl)
                    if bm and squash(bm[3]) == squash(m[3]):
                        if bm[2] != m[2]:
                            sets[(f, i)] = (bm[1] + m[2] + bm[3], f'{f}:copyright')
                            report.add('Applied', f'{f} line {i + 1}: copyright year {m[2]} kept')
                        edited.remove(j)
                        break
            if edited:
                report.add('Dropped', f'{f} line {u.phys[edited[0]]}: edited comment, replaced by the {target_label} text\n'
                           f'      yours:\n{show([u.lines[j] for j in edited])}' + (f'\n      stock:\n{show(stock)}' if stock else ''))
            for start, run in runs:
                if not any(l.strip() for l in run): continue
                body = [l for l in run if l.strip()]
                opens = sum(bool(re.match(r'\s*#\s*if', l)) for l in body)
                closes = sum(bool(re.match(r'\s*#\s*endif', l)) for l in body)
                old = stock_lines and all(squash(l) in stock_lines.get(f, ()) for l in body) \
                      and not any(re.match(r'\s*#\s*define\b', l) for l in body)   # an active #define is a setting
                if old and all(is_directive(l) for l in body) and not opens:
                    old = closes and stale_ifs >= closes    # only #endifs closing dropped stock #ifs
                if old:
                    stale_ifs += opens - closes
                    report.add('Dropped', f'{f} line {u.phys[start]}: text from another stock version, not your addition\n' + show(run, 2))
                    continue
                if tag != 'insert':
                    while not run[0].strip(): run.pop(0)
                    while not run[-1].strip(): run.pop()
                where = place(i1 - 1, run)
                first = squash(next(l for l in run if l.strip()))[:60]
                if where:
                    report.add('Added lines placed', f'{f} {where}: {first}' + (f' (+{len(run) - 1} lines)' if len(run) > 1 else ''))
                else:
                    orphan(f, run, f'added lines: no matching place in {target_label}')

    # Assemble output
    out = {}
    for f, b in B.items():
        if f not in U:
            out[f] = '\n'.join(b.lines).replace('\n', b.nl) + (b.nl if b.final_nl else '')
            report.add('Warnings', f'{f}: not in your folder; wrote the stock {target_label} file')
            continue
        lines = []
        for l in inserts.get((f, -1), []): lines.append(l)
        for i, l in enumerate(b.lines):
            lines.append(sets[(f, i)][0] if (f, i) in sets else l)
            lines.extend(inserts.get((f, i), []))
        if orphans.get(f):
            lines += ['', '//' + '=' * 75,
                      f'// Migrated from {orig_label}: not recognized by {target_label}.',
                      '// Review these, then delete this block.',
                      '//' + '=' * 75]
            for chunk, why in orphans[f]:
                lines.append(f'// {why}')
                for l in chunk:
                    lines += ['//' + p.lstrip() if p.strip() and not p.lstrip().startswith('//') else p for p in l.split('\n')]
                report.add('Moved to end block', f'{f}: {why}')
        text = U[f].nl.join('\n'.join(lines).split('\n'))
        out[f] = text + (b.nl if b.final_nl else '')
    return out

def migrate_texts(git, user, base, target, report, blabel, tlabel, stock_lines=None):
    '''Migrate {file: text} from stock commit base ({file: commit} or one commit) to target.'''
    bases = base if isinstance(base, dict) else { f: base for f in user }
    A, B, U = {}, {}, {}
    for f, t in user.items():
        U[f] = ConfigFile(f, t)
        at = git.show(bases[f], GIT_DIR + f)
        A[f] = ConfigFile(f, at if at is not None else '')
    for f in FILES:
        bt = git.show(target, GIT_DIR + f)
        if bt is not None: B[f] = ConfigFile(f, bt)

    b_names = set(n for c in B.values() for n in c.names)
    oldest = min(set(bases.values()), key=git.date)
    resolve, explain, upgrade = build_renames(git, oldest, target, b_names)
    return migrate(A, U, B, resolve, explain, report, blabel, tlabel, upgrade, stock_lines)

def write(path, text, bom=False):
    """Write text exactly as given (no newline translation), optionally with a UTF-8 BOM."""
    with open(path, 'w', encoding='utf-8-sig' if bom else 'utf-8', newline='') as fh: fh.write(text)

def main():
    # Never crash printing the report on a console that can't show every character
    for stream in (sys.stdout, sys.stderr):
        try: stream.reconfigure(errors='replace')
        except AttributeError: pass

    ap = argparse.ArgumentParser(
        formatter_class=argparse.RawDescriptionHelpFormatter,
        description='Upgrade or downgrade Marlin Configuration.h and Configuration_adv.h\n'
                    'to another Marlin version, keeping your changes.',
        epilog='''how it works:
  Finds the stock files your configuration started from, works out what you
  changed, and applies those changes to the stock files of the target version.
  Renamed options are followed (Changes.h and git history), and some values are
  converted, e.g. *_ENDSTOP_INVERTING true -> *_ENDSTOP_HIT_STATE LOW.

  - Your values and enabled/disabled options are kept. If upstream also
    changed the default, it is listed under "Conflicts".
  - Options you left at the default take the target's default. Where that
    differs from before, the option is listed under "Defaults changed".
  - Options the target no longer has go, commented out, to a block at the end
    of the file for you to review.
  - Lines you added are placed after the same stock line as before.
  - Edited block comments are replaced by the target's text ("Dropped" lists
    them); edited trailing comments on #define lines are kept.
  Review the report printed at the end before building.

examples:
  %(prog)s path/to/my-config --to 2.1.2.5
  %(prog)s path/to/my-config --to bugfix-2.1.x -o path/to/new-config
  %(prog)s path/to/my-config --to HEAD --from 2.0.9.3 --report report.txt

requires a full git clone of Marlin with tags (not a ZIP download):
  git clone https://github.com/MarlinFirmware/Marlin.git
''')
    ap.add_argument('config_dir', nargs='?', default='Marlin', metavar='CONFIG_DIR',
                    help='folder with your Configuration.h / Configuration_adv.h (default: Marlin)')
    ap.add_argument('--to', default='HEAD', metavar='REV',
                    help='target version: tag, branch or commit (default: HEAD)')
    ap.add_argument('--from', dest='base', metavar='REV',
                    help='stock version your files started from (default: detected)')
    ap.add_argument('-o', '--out', metavar='DIR',
                    help='where to write the migrated files (default: CONFIG_DIR/migrated)')
    ap.add_argument('--in-place', action='store_true',
                    help='overwrite the files in CONFIG_DIR, saving the originals as .bak')
    ap.add_argument('--report', metavar='FILE', help='also write the report to FILE')
    ap.add_argument('--repo', default=str(Path(__file__).resolve().parents[3]), metavar='DIR',
                    help='Marlin git clone to read history from (default: the one containing this script)')
    args = ap.parse_args()

    git = Git(args.repo)
    git.check()
    cdir = Path(args.config_dir)
    user, bom = {}, set()
    for f in FILES:
        p = cdir / f
        if p.exists():
            with open(p, encoding='utf-8', errors='replace', newline='') as fh: text = fh.read()
            if text.startswith('\ufeff'):
                text = text[1:]; bom.add(f)
            if text.strip(): user[f] = text
    if not user: sys.exit(f'No {" or ".join(FILES)} in {cdir}')

    target = git.rev(args.to)
    if args.base:
        base, score, stock_lines = git.rev(args.base), None, None
        bases = { f: base for f in user }
    else:
        print('Finding the stock version these files are based on...', file=sys.stderr)
        base, score, bases, stock_lines = detect_base(git, user, target)

    blabel, tlabel = git.describe(base), git.describe(target)
    report = Report()
    vers = { f: m[1] for f, t in user.items() if (m := re.search(r'#define\s+CONFIGURATION_(?:ADV_)?H_VERSION\s+(\w+)', t)) }
    if len(set(vers.values())) > 1:
        report.add('Warnings', 'Your files are from different versions: ' + ', '.join(f'{f} {v}' for f, v in vers.items()))
    for f, b in bases.items():
        if b != base:
            report.add('Warnings', f'{f} matches {git.describe(b)} better than {blabel}; used that as its base')
    out = migrate_texts(git, user, bases, target, report, blabel, tlabel, stock_lines)

    odir = cdir if args.in_place else Path(args.out) if args.out else cdir / 'migrated'
    odir.mkdir(parents=True, exist_ok=True)
    for f, text in out.items():
        p = odir / f
        if args.in_place: write(cdir / (f + '.bak'), user[f], f in bom)
        write(p, text, f in bom)

    def full(label, sha): return label if label == sha[:10] else f'{label} ({sha[:10]})'
    same = [x for x in git.same_tags(base, [GIT_DIR + f for f in user]) if x != blabel]
    if len(set(bases.values())) > 1:
        head = 'Base:   ' + ', '.join(f'{f} {full(git.describe(b), b)}' for f, b in bases.items())
    else:
        head = f'Base:   {full(blabel, base)}' + (f', same files as {", ".join(same)}' if same else '') \
             + (f', {score:g} differences from your files' if score is not None else '')
    header = [head,
              f'Target: {full(tlabel, target)}',
              f'Output: {odir}']
    rtext = report.text(header)
    print(rtext, end='')
    if args.report: write(args.report, rtext)

if __name__ == '__main__':
    main()
