---
name: marlin-fork-harvesting
description: Use when evaluating a GitHub fork of Marlin (a vendor's or a community member's) and porting a feature, board, or display driver into mainline as a clean, reviewable PR.
---

# Port a Feature from a Marlin Fork

Forks often hold support for hardware that mainline lacks. Their code can't be merged as-is: it is based on an older Marlin, may restructure the HALs, and may not follow the project style. Port the part that belongs in mainline, and port it cleanly.

Check the fork's license and keep attribution. Say in the PR where the code came from and credit the author.

## 1. Decide whether it is worth porting

| Question | How to check |
| --- | --- |
| Is it maintained? | Last push date |
| Is it really Marlin? | It has `Marlin/src/` |
| Is it already in mainline? | Search the Marlin PRs and `git log` for the board or feature |
| Does it add hardware or features mainline lacks? | See step 3 |
| Is the quality reasonable? | Read two or three key files |

The vendor's own repo is a secondary source. It is often a frozen snapshot from the product launch, based on an old Marlin, with stale board IDs and an outdated layout. Use it only to cross-check pin assignments and names, not as a port base.

## 2. Add the fork as a remote

    git remote add <author-slug> https://github.com/<Author>/Marlin
    git fetch <author-slug>
    git branch -r | grep <author-slug>

## 3. Find only the relevant changes

A fork that diverged long ago shows thousands of changed files, nearly all of it mainline drift. Filter to what matters:

    git log --oneline upstream/bugfix-2.1.x..<slug>/<branch> | wc -l
    git diff --name-only upstream/bugfix-2.1.x <slug>/<branch> | grep -iE '<machine>|src/pins/|src/HAL/|src/lcd/'
    git show <slug>/<branch>:Marlin/Configuration.h | grep -E 'MOTHERBOARD|MACHINE_NAME'

Check each of these:

- **New HAL?** If the board needs an MCU family that mainline doesn't support, decide with the maintainers whether the HAL is part of this PR or a separate one before you write code.
- **Board entry?** Check `Marlin/src/core/boards.h`. A new board needs a `boards.h` entry, a `pins.h` entry, and a pins file.
- **UI code?** Large display-driver or UI changes should go in a separate PR from the core feature.

## 4. Port, don't cherry-pick

Make a clean-room port in most cases. Read the fork's code, then write it in mainline's style on a fresh branch:

    git checkout -b <topic> upstream/bugfix-2.1.x

Cherry-pick only when the commits are linear, clean, and conflict-free.

Never port a fork's HAL restructuring (deleted platforms, renamed folders, moved files). Take the target board's pins file and the genuinely new driver or feature.

### Rules

1. **Use mainline's names and layout.** A fork's folder names may differ (for example `hc32f46x/` vs mainline `hc32f4/`). Check `ls Marlin/src/pins/` and follow it.
2. **Board IDs must be verified against `boards.h`.** A fork's ID is often already taken in mainline. Use the next free ID in the right architecture range:

       grep '#define BOARD_' Marlin/src/core/boards.h | awk '{print $3}' | sort -n

   Keep the `boards.h` order the same as the `#elif MB()` order in `pins.h`.

3. **Config defaults** go in the `Conditionals-*.h` layer. `SanityCheck.h` only validates currently-valid combinations.
4. **Renamed or removed options** get an `#error` in `Marlin/src/inc/Changes.h` that points to the replacement.
5. **Old G-codes and M-codes** that the port replaces should keep working as shims where reasonable.
6. **EEPROM data** follows the struct pattern in `settings.cpp`, with a version bump.
7. **Don't copy old APIs.** Fork code often uses names removed from mainline. Fix each compile error against current headers rather than adding compatibility macros.

### New ExtUI display drivers

See skill marlin-extui-backends. In particular:

- Add the driver folder to `ini/features.ini`. If it is missing, the whole driver is silently excluded and every ExtUI callback is "undefined".
- Add the new LCD define to the lists in `Conditionals-2-LCD.h`, and add its `SanityCheck.h` entries.
- Compare the fork's handlers with the current `ui_api.h` (renamed `onMediaMounted`, handlers with new argument types, newer handlers such as `onSurviveInKilled`).

### Per-HAL notes

Read `Marlin/src/HAL/<FAMILY>/AGENTS.md`. For example, a HAL may lack an EEPROM backend the fork relied on (`PersistentStore::*` undefined at link), or need a board-defined crystal frequency.

## 5. Build and test

    buildroot/bin/opt_set MOTHERBOARD BOARD_<NAME>
    buildroot/bin/mftest -a -n1 -y
    buildroot/bin/restore_configs

- Build with the feature disabled and enabled.
- Add a test config for new options (skill: marlin-build-test). A test config must satisfy the sanity checks, for example `NOZZLE_AS_PROBE` needs a zero probe offset, and bed leveling with a physical probe needs `Z_SAFE_HOMING`.
- Say honestly in the PR what you built, and that no one has tested on hardware if that is so.

## 6. Commit and PR

Split it: core pins and board first, display driver second, extra features after that (skill: marlin-pr-workflow). Credit the fork's author in the PR description.
