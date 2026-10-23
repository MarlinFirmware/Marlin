#!/usr/bin/env python3
"""
parallel_build.py — Run config-example loop tests in parallel across Marlin clones.

Replaces parallel_build_test.sh. Uses argparse and subprocess directly instead
of shell argument parsing and eval.

Usage:
    python3 buildroot/bin/parallel_build.py [OPTIONS]

Options:
    --script=SCRIPT          Loop script to run (default: fix_host_action_configs.py)
    --clones=N               Number of parallel clones (default: 4)
    --tmux                   Use tmux to observe all builds (default: yes)
    --no-tmux                Run clones in background without tmux
    --extra-args="ARGS"      Extra args to pass to each loop script instance
    --base=PATH              Config repo base path (default: $MARLIN_CONFIGS)
    --marlin-src=PATH        Marlin source repo path (default: $MARLIN_REPO)
    --clone-dir=DIR          Base directory for clones (default: parent of MARLIN_REPO)
    --clone-prefix=NAME      Clone directory name prefix (default: marlin-clone)
    --dry-run                Show what would be done without doing it

The loop script must support:
    --resume=<path>   Start from a specific config sub-path
    --loop-limit=N    Stop after processing N configs

Each build takes ~19s. In sequence, 386 builds take ~120 min.
With 16 parallel clones, it takes ~8 min.
"""

import argparse
import glob
import os
import shlex
import subprocess
import sys

# ---------------------------------------------------------------------------
# Defaults
# ---------------------------------------------------------------------------
DEFAULT_SCRIPT = "buildroot/bin/parallel-build_and_log.py"
DEFAULT_CLONES = 4
DEFAULT_PREFIX = "marlin-clone"


def shlex_join(parts):
    """Join command parts into a single shell-escaped string."""
    try:
        import shlex
        return " ".join(shlex.quote(p) for p in parts)
    except Exception:
        return " ".join(parts)


def discover_configs(base):
    """Return sorted list of config directory names under base/config/examples/.

    Looks for Configuration.h or Configuration_adv.h files and returns their
    parent directory names relative to base/config/examples/.
    """
    examples = os.path.join(base, "config", "examples")
    if not os.path.isdir(examples):
        return None, f"Config examples directory not found: {examples}"

    dirs = set()
    for pattern in ("Configuration.h", "Configuration_adv.h"):
        for path in glob.glob(os.path.join(examples, "**", pattern), recursive=True):
            dirs.add(os.path.dirname(path))

    result = sorted(os.path.relpath(d, examples) for d in dirs)
    return result, None


def compute_batches(config_dirs, n_clones):
    """Divide sorted config directories into n_clones contiguous batches.

    Returns a list of (start_dir, count) tuples. When there are more clones
    than configs, extras get empty batches.
    """
    total = len(config_dirs)
    batches = []

    if n_clones >= total:
        for i in range(n_clones):
            if i < total:
                batches.append((config_dirs[i], 1))
            else:
                batches.append(("", 0))
    else:
        base_size = total // n_clones
        remainder = total % n_clones
        idx = 0
        for c in range(n_clones):
            sz = base_size + (1 if c < remainder else 0)
            batches.append((config_dirs[idx], sz))
            idx += sz

    return batches


def prepare_clone(clone_num, clone_dir_base, clone_prefix, marlin_src, dry_run):
    """Create or reset a single clone directory. Returns the clone path."""
    clone_dir = os.path.join(clone_dir_base, f"{clone_prefix}-{clone_num}")

    if os.path.isdir(clone_dir):
        print(f"  Clone {clone_num}: already exists at {clone_dir} (reusing)")
    else:
        print(f"  Clone {clone_num}: creating at {clone_dir} ...")
        if not dry_run:
            subprocess.run(["git", "clone", marlin_src, clone_dir], check=True)

    if not dry_run:
        # Ensure clean state on the expected branch
        subprocess.run(
            ["git", "fetch", "origin"],
            cwd=clone_dir, capture_output=True,
        )
        head_branch = subprocess.run(
            ["git", "rev-parse", "--abbrev-ref", "HEAD"],
            capture_output=True, text=True, cwd=clone_dir,
        ).stdout.strip()
        subprocess.run(
            ["git", "checkout", "-f", head_branch],
            cwd=clone_dir, capture_output=True,
        )
        subprocess.run(
            ["git", "reset", "--hard", "HEAD"],
            cwd=clone_dir, capture_output=True,
        )
        subprocess.run(
            ["git", "clean", "-ffd"],
            cwd=clone_dir, capture_output=True,
        )

    return clone_dir


def tmux_available():
    """Check if tmux is installed."""
    import shutil
    return shutil.which("tmux") is not None


def launch_tmux(session_name, batches, clone_dir_base, clone_prefix, script_path, extra_args):
    """Launch all clones in a shared tmux session."""
    subprocess.run(["tmux", "new-session", "-d", "-s", session_name], check=True)
    clone_count = len(batches)

    prefix = "python3 " if script_path.endswith(".py") else ""

    for c in range(clone_count):
        start_dir, count = batches[c]
        if count == 0:
            continue

        clone_dir = os.path.join(clone_dir_base, f"{clone_prefix}-{c}")
        window_name = f"clone-{c}"

        # Build the command to run in the pane
        run_cmd = "cd {} && {} --resume={} --loop-limit={}".format(
            shlex.quote(clone_dir),
            f"{prefix}{script_path}",
            shlex.quote(start_dir),
            count,
        )
        if extra_args:
            run_cmd += f" {extra_args}"
        run_cmd += f"; echo 'Clone {c} finished. Press Enter to close.'; read"

        if c == 0:
            # Rename the initial window and send the command
            subprocess.run(
                ["tmux", "rename-window", "-t", f"{session_name}:0", window_name],
                check=False,
            )
            subprocess.run(
                ["tmux", "send-keys", "-t", f"{session_name}:0", run_cmd, "Enter"],
                check=False,
            )
        else:
            # Create a new window and send the command
            subprocess.run(
                ["tmux", "new-window", "-t", session_name, "-n", window_name],
                check=False,
            )
            subprocess.run(
                ["tmux", "send-keys", "-t", f"{session_name}:{c}", run_cmd, "Enter"],
                check=False,
            )
        print(f"  Started {window_name}: --resume={start_dir} --loop-limit={count}")

    print()
    print(f"All clones running. Attach with:")
    print(f"  tmux attach -t {session_name}")
    print()
    print("When all windows finish, kill the session with:")
    print(f"  tmux kill-session -t {session_name}")


def launch_background(batches, clone_dir_base, clone_prefix, script_path, extra_args):
    """Launch all clones as detached background processes."""
    print("=== Launching clones in background ===")
    clone_count = len(batches)

    for c in range(clone_count):
        start_dir, count = batches[c]
        if count == 0:
            continue

        clone_dir = os.path.join(clone_dir_base, f"{clone_prefix}-{c}")
        log_file = os.path.join(clone_dir, "build-test.log")

        cmd_parts = [
            f"cd {shlex_join([clone_dir])} &&",
            script_path,
            f"--resume={shlex.quote(start_dir)}",
            f"--loop-limit={count}",
        ]
        if extra_args:
            cmd_parts.append(extra_args)
        cmd_parts += [">", log_file, "2>&1"]

        run_cmd = " ".join(cmd_parts)
        subprocess.Popen(run_cmd, shell=True, executable="/bin/bash")
        print(f"  Started clone-{c}: --resume={start_dir} --loop-limit={count}  (log: {log_file})")

    print()
    print("Clones running in background. Tail logs to monitor progress.")


def dry_run_report(batches, clone_dir_base, clone_prefix, script_path, extra_args):
    """Print what would be done without actually doing it."""
    print("[dry-run] Would launch the following commands:")
    for c, (start_dir, count) in enumerate(batches):
        if count == 0:
            continue
        clone_dir = os.path.join(clone_dir_base, f"{clone_prefix}-{c}")
        cmd = f"cd {shlex.quote(clone_dir)} && {script_path} --resume={shlex.quote(start_dir)} --loop-limit={count}"
        if extra_args:
            cmd += f" {extra_args}"
        print(f"  Clone {c}: {cmd}")


def main():
    parser = argparse.ArgumentParser(
        description="Run config-example loop tests in parallel across Marlin clones.",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog=__doc__,
    )
    parser.add_argument(
        "--script", type=str, default=DEFAULT_SCRIPT,
        help="Loop script to run (default: %(default)s)",
    )
    parser.add_argument(
        "--clones", type=int, default=DEFAULT_CLONES,
        help="Number of parallel clones (default: %(default)s)",
    )
    tmux_group = parser.add_mutually_exclusive_group()
    tmux_group.add_argument(
        "--tmux", dest="use_tmux", action="store_true", default=True,
        help="Use tmux to observe all builds (default)",
    )
    tmux_group.add_argument(
        "--no-tmux", dest="use_tmux", action="store_false",
        help="Run clones in background without tmux",
    )
    parser.add_argument(
        "--extra-args", type=str, default="",
        help="Extra args to pass to each loop script instance",
    )
    parser.add_argument(
        "--base", type=str, default=None,
        help="Config repo base path (default: $MARLIN_CONFIGS)",
    )
    parser.add_argument(
        "--marlin-src", type=str, default=None,
        help="Marlin source repo path (default: $MARLIN_REPO)",
    )
    parser.add_argument(
        "--clone-dir", type=str, default=None,
        help="Base directory for clones (default: parent of MARLIN_REPO)",
    )
    parser.add_argument(
        "--clone-prefix", type=str, default=DEFAULT_PREFIX,
        help="Clone directory name prefix (default: %(default)s)",
    )
    parser.add_argument(
        "--dry-run", action="store_true",
        help="Show what would be done without doing it",
    )

    args = parser.parse_args()

    # Resolve paths from environment
    marlin_src = args.marlin_src or os.environ.get("MARLIN_REPO", "")
    base = args.base or os.environ.get("MARLIN_CONFIGS", "")

    if not marlin_src:
        print("ERROR: MARLIN_REPO not set and --marlin-src not provided.", file=sys.stderr)
        sys.exit(1)
    if not base:
        print("ERROR: MARLIN_CONFIGS not set and --base not provided.", file=sys.stderr)
        sys.exit(1)

    clone_dir_base = args.clone_dir or os.path.dirname(marlin_src.rstrip("/"))
    script_path = os.path.join(marlin_src, args.script)

    # Validate inputs
    if not os.path.isdir(os.path.join(base, "config", "examples")):
        print(f"ERROR: Config examples directory not found: {base}/config/examples", file=sys.stderr)
        sys.exit(1)
    if not os.path.isfile(script_path):
        print(f"ERROR: Loop script not found: {script_path}", file=sys.stderr)
        sys.exit(1)

    # Discover configs
    config_dirs, err = discover_configs(base)
    if err or not config_dirs:
        print(f"ERROR: {err or 'No config directories found.'}", file=sys.stderr)
        sys.exit(1)

    total = len(config_dirs)
    print(f"Found {total} config directories")
    print(f"Using {args.clones} parallel clones")
    print(f"Loop script: {script_path}")
    print()

    # Compute batch assignments
    batches = compute_batches(config_dirs, args.clones)

    print("=== Batch assignments ===")
    for c in range(args.clones):
        start_dir, count = batches[c]
        if count > 0:
            print(f"  Clone {c}: start={start_dir}  configs={count}")
        else:
            print(f"  Clone {c}: (idle — no configs assigned)")
    print()

    # Prepare clones
    print("=== Preparing clones ===")
    for c in range(args.clones):
        if batches[c][1] > 0:
            prepare_clone(c, clone_dir_base, args.clone_prefix, marlin_src, args.dry_run)
    print()

    # Dry-run: just report what we'd do
    if args.dry_run:
        dry_run_report(batches, clone_dir_base, args.clone_prefix, script_path, args.extra_args)
        sys.exit(0)

    # Launch builds
    if args.use_tmux:
        if not tmux_available():
            print("tmux not found — falling back to background processes")
            launch_background(batches, clone_dir_base, args.clone_prefix, script_path, args.extra_args)
        else:
            session_name = f"marlin-build-{int(subprocess.check_output(['date', '+%s']).decode().strip())}"
            print(f"=== Launching in tmux session: {session_name} ===")
            launch_tmux(session_name, batches, clone_dir_base, args.clone_prefix, script_path, args.extra_args)
    else:
        launch_background(batches, clone_dir_base, args.clone_prefix, script_path, args.extra_args)

    print()
    print("Done launching.")


if __name__ == "__main__":
    main()
