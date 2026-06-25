#!/usr/bin/env python3
"""
fix_host_action_configs.py — Three-phase HOST action config fixer.

Usage:
    python3 fix_host_action_configs.py [--loop-limit=N] [--resume=DIR] [--limit=N]

Phases per config:
    start        -> build as-is; on failure disable HOST_PROMPT_SUPPORT, retry
    no_prompt    -> HOST_PROMPT_SUPPORT disabled; on failure disable HOST_ACTION_COMMANDS, skip
    skip_action  -> both HOST_* disabled; move to next config
"""

import subprocess, sys, os, re, argparse, signal

# Ensure we can import config from the sibling location
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
PLATFORMIO_SCRIPTS = os.path.join(SCRIPT_DIR, '..', 'share', 'PlatformIO', 'scripts')
sys.path.insert(0, PLATFORMIO_SCRIPTS)
import config  # type: ignore[import-untyped]

# ---------------------------------------------------------------------------
# Environment — derive all repo paths from CWD (not a hardcoded env var)
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
    # Strip ANSI escape codes so regex matching is simpler
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


def disable_config_option(config_dir, option):
    """
    Disable a config option in all existing config files under config_dir.
    Wraps config.enable(path, option, False).
    """
    files = config.resolve(config_dir)
    for fpath in files:
        if os.path.exists(fpath):
            config.enable(fpath, option, False)


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
        description='Fix HOST_* config options across all example configs.'
    )
    parser.add_argument(
        '--loop-limit', type=int, default=0,
        help='Stop after processing N configs (0 = unlimited, default)',
    )
    parser.add_argument(
        '--resume', type=str, default=None,
        help='Start at this config directory name',
    )
    # Passthrough args for build_all_examples
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
        next_phase = 'start'       # start | no_prompt | skip_action
        last_failing = ''

        build_args = []
        if args.resume:
            build_args.append(f'--resume={args.resume}')

        for iteration in range(1, 501):
            # Check loop limit
            if args.loop_limit > 0 and loop_count >= args.loop_limit:
                print(f"Loop limit ({args.loop_limit}) reached.")
                sys.exit(0)

            # --- Build ---------------------------------------------------
            # Resume only applies on the first iteration of each phase
            build_cmd_args = list(build_args) + build_opts  # BUG FIX: include build_opts

            if next_phase == 'start':
                pass  # first build — no special flags
            elif next_phase == 'no_prompt':
                # Remove --resume: --continue takes over from the stat file
                build_cmd_args = [a for a in build_cmd_args if not a.startswith('--resume=')]
                build_cmd_args.append('--continue')
            elif next_phase == 'skip_action':
                build_cmd_args = [a for a in build_cmd_args if not a.startswith('--resume=')]
                build_cmd_args.append('--skip')

            print(f"=== Iteration {iteration}: phase={next_phase} {' '.join(build_cmd_args)} ===")

            rc, output = run_build(build_cmd_args)
            print(output)

            # --- Success: build completed this config ---
            if rc == 0:
                if next_phase == 'start':
                    print("ALL DONE — built successfully")
                else:
                    print(f"ALL DONE — built after fixes (phase: {next_phase})")
                sys.exit(0)

            loop_count += 1

            # --- Analyse the failure ------------------------------------
            failing = get_failing_config(output)

            if not failing:
                print("ERROR: Could not determine failing config — stopping.")
                sys.exit(1)

            # If a different config is now failing (e.g. the retry of the previous
            # config succeeded but a later one failed), reset to the start phase.
            if last_failing and failing != last_failing:
                print(f"Failing config changed: {last_failing} -> {failing} — resetting.")
                next_phase = 'start'

            last_failing = failing
            config_dir = os.path.join(MARLIN_CONFIGS, 'config', 'examples', failing)

            if next_phase == 'start':
                print(f"Build failed: {failing}")
                print(f"Disabling HOST_PROMPT_SUPPORT in {failing} and retrying...")
                disable_config_option(config_dir, 'HOST_PROMPT_SUPPORT')
                next_phase = 'no_prompt'

            elif next_phase == 'no_prompt':
                print(f"Retry (no HOST_PROMPT_SUPPORT) failed: {failing}")
                print(f"Disabling HOST_ACTION_COMMANDS in {failing} and skipping...")
                disable_config_option(config_dir, 'HOST_ACTION_COMMANDS')
                next_phase = 'skip_action'

            elif next_phase == 'skip_action':
                print(f"Still failing after disabling both HOST_* options: {failing}")
                print("Skipping to next config.")
                next_phase = 'start'

            # After disable, clear stat file so next build starts fresh
            if os.path.exists(STAT_FILE):
                os.remove(STAT_FILE)

            print("")

        print("Hit max iterations (500)")
        sys.exit(1)

    finally:
        cleanup()


if __name__ == '__main__':
    main()
