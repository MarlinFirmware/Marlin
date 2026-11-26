---
name: marlin-solving-bugs
description: Use when investigating and fixing a Marlin bug report, from reproducing it to a verified patch. Covers triage, finding the code path, isolating the cause, a minimal root-cause fix, and verification.
---

# Fix a Marlin Bug

Find the root cause before you write any fix. Marlin has many HALs and a huge number of configuration combinations, and there is no hardware-in-the-loop CI, so the evidence you gather matters more than the size of the patch.

## 1. Triage the report

- Get the exact Marlin version or commit, the board, and `Configuration.h` / `Configuration_adv.h` (a `marlin_config.json` or a Configurations ZIP is best).
- Find the first version that failed, and the last that worked. If the reporter can test, ask for a bisect (`git bisect`) between those points.
- Check whether `bugfix-2.1.x` still has the bug. Fixes land there first, and old reports are often already fixed.
- Files move between releases. Search for the current location of any path in an old report: `ultralcd.*` became `marlinui.*`, and `Conditionals_post.h` became `Conditionals-5-post.h`.

## 2. Find the code path

    git grep -n "OPTION_NAME" -- Marlin/src            # who uses an option
    git log --oneline -20 -- Marlin/src/module/motion.cpp
    git log -S"suspicious_string" --oneline -- Marlin/src
    git log --oneline 2.1.2..2.1.3 -- Marlin/src/module/probe.cpp

- G-code handlers are in `Marlin/src/gcode/`, with one file per command or small group.
- Trace which `#if` paths the reporter's config enables. A bug often appears only for one combination of options.
- A regression usually has a specific commit. Read its diff and its PR discussion to learn what it was meant to do.

## 3. Isolate the cause

- Reproduce on the smallest config you can, using `buildroot/bin/opt_*` to set only the needed options (skill: marlin-config-options).
- Test as much as possible in the native simulator ([MarlinSimUI](https://github.com/MarlinFirmware/MarlinSimUI)) or a unit test, so you can run and re-run the scenario.
- Change one thing at a time and keep notes. A temporary `SERIAL_ECHOLNPGM` can help, but remove every debug statement before committing.
- State your hypothesis and the evidence before coding the fix. If you can, show that reverting the suspect change removes the symptom.

## 4. Fix the root cause

- Fix the wrong logic itself. Don't add workarounds elsewhere that hide the symptom.
- Make the smallest diff that does it. No reformatting, no unrelated cleanup (skill: marlin-code-style).
- Think about every configuration the changed code is compiled in. Build with the relevant options both on and off.

Common causes:

| Symptom | Look for |
| --- | --- |
| Wrong distance or speed | mm vs steps vs mm/s units; missing conversion |
| Works with one option, breaks with another | A missing `ENABLED()` guard, or the wrong layer in `Conditionals-*.h` |
| Wrong axis | `X_AXIS` used where `Z_AXIS` was meant; macro for the wrong axis |
| Fails only on AVR | Integer overflow (use `uint32_t`), `double`, RAM and flash limits |
| Fails only on one board | HAL differences: timers, serial buffering, pin multiplexing |
| Position drift after an event | A reset of `current_position` or the planner position without `sync_plan_position()` |
| Lost settings after upgrade | A `settings.cpp` layout change without a version bump |

## 5. Verify

- Build with `mftest` (skill: marlin-build-test). At least one AVR (tight flash) and one 32-bit environment for common code.
- Run the reporter's scenario again, not just a build. A passing build is not proof the bug is fixed.
- If the logic is testable without hardware, add a unit test that fails before your fix and passes after.
- If you add a config interaction, add a check to `Marlin/src/inc/SanityCheck.h`.

## 6. Submit

Follow skill marlin-pr-workflow. In the PR description, say what the root cause was, how you reproduced it, and what you tested (including on what hardware, if any). Reference the issue in the description, not the title. Don't claim the issue is closed until the reporter or another tester confirms it on hardware.

## Pitfalls

- **Debug code left in.** `#define DEBUG_OUT 1`, `DEBUG_ECHOLNPGM`, and temporary prints must not be in the final diff. Check with `git diff | grep -n DEBUG_`.
- **Stale `case` labels.** After an enum value is removed, a `case REMOVED_VALUE:` can survive a rebase. Grep for removed names when a PR touches enums.
- **Shared string buffers.** The `numtostr` family (`ftostr*`, `i16tostr*`) writes into one shared buffer. Never pass one function's result into another, and don't hold a pointer across calls.
- **Variables declared outside the guard of their only uses** cause `-Wunused-variable`, and CI treats warnings as errors. Put the declaration inside the same guard.
- **A function whose signature uses a type from a guarded header** needs its definition guarded the same way as its declaration.
- **Verify a premise before relying on it.** For example, `IS_SCARA` also covers TPARA, so grep the macro definition before saying a path doesn't apply to a variant.

## Reading a failed CI run

    gh run view <run_id> --repo MarlinFirmware/Marlin --json jobs -q '.jobs[] | select(.conclusion=="failure") | [.databaseId, .name] | @tsv'
    gh run view <run_id> --job <job_id> --log | grep -E "error:|FAILED|undefined" | head -30

A job that failed in under 30 seconds is nearly always a compile or sanity-check error. Longer ones are often timeouts.
