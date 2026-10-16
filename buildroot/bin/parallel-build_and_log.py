#!/usr/bin/env python3
"""
build_and_log.py — Simple parallel build runner with shared error logging.

This is the baseline script for a simple parallel build: it builds each
config example and logs any failures to a shared log file so problems can
be triaged after all parallel builds have completed.

Usage:
    python3 build_and_log.py [--loop-limit=N] [--resume=DIR] [--limit=N]
                              [--nofail] [--many] [--archive] [-a]

The log file is written to .pio/build_errors.log inside MARLIN_REPO,
so all parallel clones write to the same shared location when their
MARLIN_REPO env var points at the same place (or can be merged later).
"""

import subprocess
import sys
import os
import re
import argparse
import signal
import datetime

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
PLATFORMIO_SCRIPTS = os.path.join(SCRIPT_DIR, '..', 'share', 'PlatformIO', 'scripts')
sys.path.insert(0, PLATFORMIO_SCRIPTS)
import config  # type: ignore[import-untyped]

# ---------------------------------------------------------------------------
# Environment — derive repo path from CWD (not a hardcoded env var)
# ---------------------------------------------------------------------------
MARLIN_REPO  = os.getcwd()
MARLIN_CONFIGS = os.environ.get('MARLIN_CONFIGS', '')
BUILDALL     = os.path.join(MARLIN_REPO, 'buildroot', 'bin', 'build_all_examples')
STAT_FILE    = os.path.join(MARLIN_REPO, '.pio', '.buildall')
LOG_FILE     = os.path.join(MARLIN_REPO, '.pio', 'build_errors.log')

# How many lines to capture from the tail of failed build output
TAIL_LINES = 40


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
    # Strip ANSI escape codes so log file is clean
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
        m = re.search(r'Failed to build\s+(\S+)', output)
        if m:
            failing = m.group(1)

    return failing


def ensure_log_dir():
    """Make sure the .pio directory and log file parent exist."""
    os.makedirs(os.path.dirname(LOG_FILE), exist_ok=True)


def log_error(failing, output_tail, extra=""):
    """Append failure info to the shared error log file."""
    ensure_log_dir()
    timestamp = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    with open(LOG_FILE, 'a', encoding='utf-8') as f:
        f.write(f"\n{'='*72}\n")
        f.write(f"[{timestamp}] FAILURE: {failing}\n")
        if extra:
            f.write(f"  Note: {extra}\n")
        f.write(f"{'='*72}\n")
        f.write(output_tail)
        f.write("\n")
    print(f"  -> Logged to {LOG_FILE}")


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
        description='Build all config examples and log failures to a shared log file.'
    )
    parser.add_argument(
        '--loop-limit', type=int, default=0,
        help='Stop after processing N configs (0 = unlimited, default)',
    )
    parser.add_argument(
        '--resume', type=str, default=None,
        help='Start at this config directory name',
    )
    parser.add_argument(
        '--limit', type=int, default=None,
        help='Pass --limit to build_all_examples',
    )
    parser.add_argument(
        '--nofail', action='store_true',
        help='Pass --nofail to build_all_examples (continue on fail)',
    )
    parser.add_argument(
        '--many', action='store_true',
        help='Pass --many to build_all_examples (all envs per config)',
    )
    parser.add_argument(
        '--archive', action='store_true',
        help='Pass -a to build_all_examples',
    )
    parser.add_argument(
        '--tail', type=int, default=TAIL_LINES,
        help=f'Lines of output to log on failure (default: {TAIL_LINES})',
    )
    args, unknown = parser.parse_known_args()

    # Collect passthrough flags
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

    # Ensure fresh log — back up previous if it exists
    if os.path.exists(LOG_FILE):
        backup = LOG_FILE + '.prev'
        if os.path.exists(backup):
            os.remove(backup)
        os.rename(LOG_FILE, backup)

    ensure_log_dir()
    with open(LOG_FILE, 'w', encoding='utf-8') as f:
        f.write(f"Build error log started at {datetime.datetime.now().isoformat()}\n")

    # Restore stock configs
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
        skip_next = False
        build_args = []
        if args.resume:
            build_args.append(f'--resume={args.resume}')

        for iteration in range(1, 501):
            # Check loop limit
            if args.loop_limit > 0 and loop_count >= args.loop_limit:
                print(f"Loop limit ({args.loop_limit}) reached.")
                sys.exit(0)

            # --- Build ---------------------------------------------------
            build_cmd_args = list(build_args) + build_opts
            # After a logged failure, --skip tells build_all_examples to skip
            # the config recorded in the stat file and continue to the next.
            # Guard: if the stat file has vanished (e.g. external deletion),
            # --skip would cause build_all_examples to exit 0 with "Nothing to
            # skip" — a false success.  Fall back to a fresh run instead.
            if skip_next:
                skip_next = False
                if os.path.exists(STAT_FILE):
                    build_cmd_args = [a for a in build_cmd_args if not a.startswith('--resume=')]
                    build_cmd_args.append('--skip')
                else:
                    print("WARNING: stat file missing before --skip — starting fresh run.")

            print(f"=== Iteration {iteration} {' '.join(build_cmd_args)} ===")

            rc, output = run_build(build_cmd_args)
            print(output)

            # --- Success: all remaining configs built fine ---
            if rc == 0:
                print("Build succeeded for all configs.")
                sys.exit(0)

            loop_count += 1

            # --- Failure: log and skip over the bad config ---
            failing = get_failing_config(output)
            if not failing:
                failing = f"unknown-config-{iteration}"

            output_tail = "\n".join(output.splitlines()[-args.tail:])
            log_error(failing, output_tail)

            # Leave stat file intact so --skip can target this config next call.
            skip_next = True

            print(f"Logged error for {failing}. Moving to next config.")
            print("")

        print("Hit max iterations (500)")
        sys.exit(1)

    finally:
        cleanup()


if __name__ == '__main__':
    main()
