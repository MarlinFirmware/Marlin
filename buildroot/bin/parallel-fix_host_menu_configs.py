#!/usr/bin/env python3
"""
fix_host_menu_configs.py — Fix HOST_* menu-item config options.

Usage:
    python3 fix_host_menu_configs.py [--loop-limit=N] [--resume=DIR] [--limit=N]

Phases per config:
    start        -> build as-is; on failure enable HOST_* menu items, retry
    continue     -> HOST_* enabled; on failure check prog size or skip
    skip         -> skip to next config
"""

import subprocess, sys, os, re, argparse, signal

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
PLATFORMIO_SCRIPTS = os.path.join(SCRIPT_DIR, '..', 'share', 'PlatformIO', 'scripts')
sys.path.insert(0, PLATFORMIO_SCRIPTS)
import config  # type: ignore[import-untyped]

# ---------------------------------------------------------------------------
# Environment — derive repo path from CWD (not a hardcoded env var)
# This allows the parallel orchestrator to cd into a clone first.
# ---------------------------------------------------------------------------
MARLIN_REPO  = os.getcwd()
MARLIN_CONFIGS = os.environ.get('MARLIN_CONFIGS', '')
BUILDALL     = os.path.join(MARLIN_REPO, 'buildroot', 'bin', 'build_all_examples')
STAT_FILE    = os.path.join(MARLIN_REPO, '.pio', '.buildall')


def run_build(build_opts=None, resume=None):
    """Run build_all_examples. Returns (returncode, combined_output)."""
    cmd = [BUILDALL, '--base=' + MARLIN_CONFIGS]
    if resume:
        cmd.append('--resume=' + resume)
    if build_opts:
        cmd.extend(build_opts)

    proc = subprocess.run(
        cmd,
        capture_output=True,
        text=True,
        cwd=MARLIN_REPO,
    )
    output = proc.stdout + proc.stderr
    output_clean = re.sub(r'\x1b\[[0-9;]*m', '', output)
    return proc.returncode, output_clean


def get_failing_config(output):
    """Extract failing config directory from stat file or build output."""
    failing = ''
    if os.path.exists(STAT_FILE):
        try:
            with open(STAT_FILE) as f:
                line = f.read().strip()
            parts = line.split('*')
            if len(parts) >= 2:
                failing = parts[1]
        except (IOError, ValueError):
            pass

    if not failing:
        m = re.search(
            r'Set\s+HOST_START_MENU_ITEM\s+HOST_SHUTDOWN_MENU_ITEM\s+in:\s+(\S+)',
            output,
        )
        if m:
            failing = os.path.basename(m.group(1))

    if not failing:
        m = re.search(r'Failed to build\s+(\S+)', output)
        if m:
            failing = m.group(1)

    return failing


def enable_config_options(config_dir, *options):
    """Enable config options in all existing config files under config_dir."""
    files = config.resolve(config_dir)
    for fpath in files:
        if os.path.exists(fpath):
            for opt in options:
                config.enable(fpath, opt, True)


def disable_config_options(config_dir, *options):
    """Disable config options in all existing config files under config_dir."""
    files = config.resolve(config_dir)
    for fpath in files:
        if os.path.exists(fpath):
            for opt in options:
                config.enable(fpath, opt, False)


def stash_changes():
    """Stash any uncommitted changes. Returns True if we stashed."""
    result = subprocess.run(
        ['git', 'diff', '--quiet'],
        capture_output=True, cwd=MARLIN_REPO,
    )
    if result.returncode != 0:
        subprocess.run(['git', 'stash'], cwd=MARLIN_REPO, check=True)
        return True
    return False


def restore_stash(stashed):
    """Pop the stash if we created one."""
    if stashed:
        subprocess.run(['git', 'stash', 'pop'], cwd=MARLIN_REPO, check=True)


def main():
    parser = argparse.ArgumentParser(
        description='Fix HOST_* menu-item options across all example configs.'
    )
    parser.add_argument(
        '--loop-limit', type=int, default=0,
        help='Stop after processing N configs (0 = unlimited, default)',
    )
    parser.add_argument(
        '--resume', type=str, default=None,
        help='Start at this config directory name',
    )
    parser.add_argument('--limit', type=int, default=None)
    parser.add_argument('--nofail', action='store_true')
    parser.add_argument('--many', action='store_true')
    parser.add_argument('--archive', action='store_true')
    args, unknown = parser.parse_known_args()

    build_opts = []
    if args.limit:
        build_opts.append(f'--limit={args.limit}')
    if args.nofail:
        build_opts.append('--nofail')
    if args.many:
        build_opts.append('--many')
    if args.archive:
        build_opts.append('-a')
    build_opts.extend(unknown)

    # Clear stale stat file
    if os.path.exists(STAT_FILE):
        os.remove(STAT_FILE)

    subprocess.run(
        [os.path.join(MARLIN_REPO, 'buildroot', 'bin', 'restore_configs')],
        cwd=MARLIN_REPO, check=True, capture_output=True,
    )

    stashed = stash_changes()

    def cleanup(signum=None, frame=None):
        restore_stash(stashed)
        if signum is not None:
            sys.exit(128 + signum)

    signal.signal(signal.SIGINT, cleanup)
    signal.signal(signal.SIGTERM, cleanup)

    try:
        loop_count = 0
        next_phase = 'start'        # start | continue | skip
        last_failing = ''
        patch_count = 0
        has_prog_size = False

        build_args = []
        if args.resume:
            build_args.append(f'--resume={args.resume}')

        for iteration in range(1, 501):
            if args.loop_limit > 0 and loop_count >= args.loop_limit:
                print(f"Loop limit ({args.loop_limit}) reached.")
                sys.exit(0)

            # Include build_opts in every invocation
            build_cmd_args = list(build_args) + build_opts

            if next_phase == 'continue':
                # Remove --resume: --continue takes over from the stat file
                build_cmd_args = [a for a in build_cmd_args if not a.startswith('--resume=')]
                build_cmd_args.append('--continue')
            elif next_phase == 'skip':
                build_cmd_args = [a for a in build_cmd_args if not a.startswith('--resume=')]
                build_cmd_args.append('--skip')

            print(f"=== Iteration {iteration}: phase={next_phase} {' '.join(build_cmd_args)} ===")

            rc, output = run_build(build_cmd_args)
            print(output)

            if rc == 0:
                print(f"ALL DONE — built successfully after {patch_count} patch(es)")
                sys.exit(0)

            loop_count += 1
            failing = get_failing_config(output)
            has_prog_size = "Error: The program size" in output

            if failing:
                print(f"HOST_* menu-item error in: {failing}")
                config_dir = os.path.join(MARLIN_CONFIGS, 'config', 'examples', failing)
                print(f"Patching {config_dir} ...")
                enable_config_options(config_dir, 'HOST_START_MENU_ITEM', 'HOST_SHUTDOWN_MENU_ITEM')
                last_failing = failing
                patch_count += 1
                next_phase = 'continue'

            elif has_prog_size:
                print(f"Program size overflow — reverting HOST_* patches and skipping {last_failing}.")
                config_dir = os.path.join(MARLIN_CONFIGS, 'config', 'examples', last_failing)
                disable_config_options(config_dir, 'HOST_START_MENU_ITEM', 'HOST_SHUTDOWN_MENU_ITEM')
                next_phase = 'skip'

            else:
                print("Non-HOST_* error detected — skipping this config.")
                print("=== Last 30 lines of output ===")
                for line in output.splitlines()[-30:]:
                    print(line)
                next_phase = 'skip'

            if os.path.exists(STAT_FILE):
                os.remove(STAT_FILE)
            print("")

        print("Hit max iterations (500)")
        sys.exit(1)

    finally:
        cleanup()


if __name__ == '__main__':
    main()
