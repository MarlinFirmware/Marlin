#!/usr/bin/env python3
"""Generate and validate CONFIGURATION_VERSION_HASH.

Usage:
  python buildroot/share/PlatformIO/scripts/config_version_hash.py --update
  python buildroot/share/PlatformIO/scripts/config_version_hash.py --check

On Windows, the same command can be run with:
  py -3 buildroot/share/PlatformIO/scripts/config_version_hash.py --check
"""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[4]
CONFIG_H_PATH = REPO_ROOT / "Marlin" / "Configuration.h"
CONFIG_ADV_H_PATH = REPO_ROOT / "Marlin" / "Configuration_adv.h"

FNV_OFFSET_BASIS = 2166136261
FNV_PRIME = 16777619


def calc_hash(value: str) -> int:
    h = FNV_OFFSET_BASIS
    for byte in value.encode("utf-8"):
        h ^= byte
        h = (h * FNV_PRIME) & 0xFFFFFFFF
    return h


def read_version(path: str | Path, name: str) -> str:
    match = re.search(rf"^(#define\s+{name}\s+)([0-9A-Za-z_]+)(\s*)(?:\/\/.*)?$", Path(path).read_text(encoding="utf-8"), re.MULTILINE)
    if match is None:
        raise ValueError(f"Could not find {name} in {path}")
    return match.group(2)


def validate_configuration_versions(config_h: str | Path, config_adv_h: str | Path) -> tuple[str, str]:
    config_h_version = read_version(config_h, 'CONFIGURATION_H_VERSION')
    config_adv_h_version = read_version(config_adv_h, 'CONFIGURATION_ADV_H_VERSION')
    if config_h_version != config_adv_h_version:
        raise ValueError(
            f"CONFIGURATION_H_VERSION ({config_h_version}) and CONFIGURATION_ADV_H_VERSION ({config_adv_h_version}) do not match. "
            "These configuration files are version-specific. If you are migrating this config to another version, "
            "make them match first and then run: buildroot/share/PlatformIO/scripts/config_version_hash.py --update"
        )
    return config_h_version, config_adv_h_version


def combined_version_string(config_h: str | Path, config_adv_h: str | Path) -> str:
    config_h_version, config_adv_h_version = validate_configuration_versions(config_h, config_adv_h)
    return f"{config_h_version}{config_adv_h_version}"


def expected_hash(config_h: str | Path, config_adv_h: str | Path) -> int:
    return calc_hash(combined_version_string(config_h, config_adv_h))


def update_header(path: str | Path) -> int:
    config_h = CONFIG_H_PATH
    config_adv_h = CONFIG_ADV_H_PATH
    hash_value = expected_hash(config_h, config_adv_h)
    text = Path(path).read_text(encoding="utf-8")

    pattern = re.compile(
        r"^(?P<prefix>(?:\/\/\s*)?)#define\s+CONFIGURATION_VERSION_HASH\s+(?P<value>0x[0-9A-Fa-f]+|[0-9]+)(?:[uUlL]{1,3})?(?:\s*(?:\/\/.*)?)$",
        re.MULTILINE,
    )
    match = pattern.search(text)

    if match:
        prefix = match.group("prefix") or ""
        replacement = (
            f"{prefix}#define CONFIGURATION_VERSION_HASH 0x{hash_value:08X}u "
            "// Must match both CONFIGURATION_H_VERSION and CONFIGURATION_ADV_H_VERSION."
        )
        new_text = pattern.sub(replacement, text, count=1)
    else:
        block = (
            "\n\n// Hash of the paired configuration versions. Update with the script config_version_hash.py if the version changes.\n"
            f"#define CONFIGURATION_VERSION_HASH 0x{hash_value:08X}u // Must match both CONFIGURATION_H_VERSION and CONFIGURATION_ADV_H_VERSION.\n"
        )
        new_text = text.rstrip() + block

    Path(path).write_text(new_text, encoding="utf-8")
    return hash_value


def check_header(path: str | Path) -> tuple[bool, str, int]:
    config_h = CONFIG_H_PATH
    config_adv_h = CONFIG_ADV_H_PATH
    config_h_version, config_adv_h_version = validate_configuration_versions(config_h, config_adv_h)
    expected = expected_hash(config_h, config_adv_h)
    text = Path(path).read_text(encoding="utf-8")
    match = re.search(r"^#define\s+CONFIGURATION_VERSION_HASH\s+(0x[0-9A-Fa-f]+|[0-9]+)(?:[uUlL]{1,3})?\s*(?:\/\/.*)?$", text, re.MULTILINE)
    if match is None:
        return True, f"{config_h_version}{config_adv_h_version}: CONFIGURATION_VERSION_HASH is disabled.", expected
    actual = int(match.group(1), 0)
    ok = actual == expected
    status = (
        f"{config_h_version}{config_adv_h_version}: expected 0x{expected:08X}, got 0x{actual:08X}"
        if not ok else
        f"{config_h_version}{config_adv_h_version}: hash matches 0x{expected:08X}"
    )
    return ok, status, expected


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--file", type=Path, default=CONFIG_ADV_H_PATH, help="Configuration_adv.h path")
    parser.add_argument("--check", action="store_true", help="Only verify the combined hash")
    parser.add_argument("--update", action="store_true", help="Regenerate CONFIGURATION_VERSION_HASH")
    args = parser.parse_args()

    path = args.file
    if not path.exists():
        raise FileNotFoundError(f"Configuration file not found: {path}")

    try:
        if args.check:
            ok, status, _ = check_header(path)
            print(status)
            return 0 if ok else 1

        hash_value = update_header(path)
        print(f"Updated {path}: CONFIGURATION_VERSION_HASH = 0x{hash_value:08X}u")
        return 0
    except ValueError as exc:
        print(f"Error: {exc}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
