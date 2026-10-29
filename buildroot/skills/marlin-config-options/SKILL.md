---
name: marlin-config-options
description: Use when you need to find, enable, disable, or set Marlin configuration options (Configuration.h / Configuration_adv.h) from the command line. Wraps buildroot/bin/opt_find, opt_enable, opt_disable, opt_set, and opt_add.
---

# Marlin Configuration Options from the Command Line

These scripts edit `Marlin/Configuration.h` and `Marlin/Configuration_adv.h` in place. Run them from the repo root. Commit or stash your own edits first so the changes are easy to revert, or run `buildroot/bin/restore_configs` afterward (which does a `git checkout` of the config files, discarding uncommitted edits to them).

## Find

    buildroot/bin/opt_find LIN_ADV         # enabled or disabled options matching the name
    buildroot/bin/opt_find -e BABYSTEP     # only enabled
    buildroot/bin/opt_find -d BABYSTEP     # only disabled

## Enable / disable

    buildroot/bin/opt_enable LIN_ADVANCE ADAPTIVE_STEP_SMOOTHING
    buildroot/bin/opt_disable ADAPTIVE_STEP_SMOOTHING

`opt_enable` exits with an error if an option can't be found.

## Set a value

    buildroot/bin/opt_set MOTHERBOARD BOARD_RAMPS_14_EFB EXTRUDERS 2

Takes `<name> <value>` pairs. An option that does not exist is added to `Configuration.h`.

## Use example configurations

    buildroot/bin/use_example_configs Creality/CR-10/CrealityV1

Fetches the named example from the Configurations repo (branch matched to your working branch, otherwise `bugfix-2.1.x`). Use `release-2.0.9.4:Creality/CR-10/CrealityV1` to pick a branch or tag.

## Tips

- Option names are matched exactly by enable/disable/set. Use `opt_find` first.
- Some options need others (see `Marlin/src/inc/SanityCheck.h`). A build error from a sanity check tells you what is missing.
- The CI test files in `buildroot/tests/` use these same operations. Read a few as examples.
