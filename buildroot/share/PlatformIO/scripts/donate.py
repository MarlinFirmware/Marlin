#
# donate.py
# Print a formatted donate block at the end of a successful build.
# Links are read from .github/FUNDING.yml so they stay in sync with GitHub's Sponsor button.
#
import os
import pioutil
if not os.environ.get('MARLIN_LESS_NOISE') and pioutil.is_pio_build():

    import re, shutil
    from pathlib import Path
    env = pioutil.env

    FUNDING_YML = Path('.github', 'FUNDING.yml')
    DONATE_TEXT = "Marlin is free and open source, built by volunteers. If you find it useful, please consider supporting its development:"

    # FUNDING.yml platform keys and how to turn each entry into a URL
    PLATFORMS = {
        'github':  ("GitHub Sponsors", "https://github.com/sponsors/{}"),
        'patreon': ("Patreon",         "https://www.patreon.com/{}"),
        'ko_fi':   ("Ko-fi",           "https://ko-fi.com/{}"),
        'custom':  ("Donate",          "{}"),
    }

    # Colorized output, as in signature.py
    if os.environ.get('NO_COLOR'):
        title_c = rule_c = heart_c = link_c = off_c = ''
    else:
        title_c, rule_c, heart_c, link_c, off_c = (
            "\u001b[1;33m", "\u001b[33m", "\u001b[35m", "\u001b[36m", "\u001b[0m"
        )

    def get_links():
        '''Return [(label, url)] from FUNDING.yml. Each value is a name or a [list, of, names].'''
        links = []
        if not FUNDING_YML.is_file(): return links
        for line in FUNDING_YML.read_text(encoding='utf-8').splitlines():
            m = re.match(r'^(\w+):\s*(.+?)\s*$', line)
            if not m or m[1] not in PLATFORMS: continue
            label, url = PLATFORMS[m[1]]
            for name in m[2].strip('[]').split(','):
                name = name.strip().strip('\'"')
                if name: links.append((label, url.format(name)))
        return links

    def get_version():
        '''Return SHORT_BUILD_VERSION from the (cached) preprocessed configuration'''
        from preprocessor import run_preprocessor
        for line in run_preprocessor(env):
            m = re.match(r'^#define\s+SHORT_BUILD_VERSION\s+"(.*)"\s*$', line.decode('utf-8', 'replace').strip())
            if m: return m[1]
        return ''

    def show_donate(source, target, env):
        import textwrap

        links = get_links()
        if not links: return

        width = max(60, min(shutil.get_terminal_size(fallback=(80, 24)).columns - 1, 100))
        rule = rule_c + '─' * width + off_c
        label_w = max(len(label) for label, _ in links)

        print()
        print(rule)
        print(title_c + "  Marlin %s" % get_version() + off_c)
        print(rule)
        wrapped = textwrap.wrap(DONATE_TEXT, width=width - 5)
        print('  ' + heart_c + '♥' + off_c + '  ' + wrapped[0])
        for line in wrapped[1:]: print('     ' + line)
        print()
        for label, url in links:
            print('     ' + label.ljust(label_w) + '  ' + link_c + url + off_c)
        print(rule)
        print()

    # Pass cmdstr=None so SCons doesn't echo the action's Python signature
    env.AddPostAction("$PROGPATH", env.Action(show_donate, None))
