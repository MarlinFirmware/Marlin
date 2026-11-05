---
name: marlin-codebase-navigation
description: Use when you need to find where something is defined, what uses it, or how a command or setting flows through Marlin's source. Uses only git, grep, and the repo layout, with optional tools as extras.
---

# Navigate the Marlin Source

Marlin is a large C++ tree where most code is compiled only under certain options. These steps work with nothing but `git` and a shell. Better tools are listed at the end as optional.

For the layout, singletons, and processing pipeline, read `AGENTS.md` (sections 2 to 4 and 10) first. This skill covers how to search, and where the common things live.

## Where things live

| Looking for | Look in |
| --- | --- |
| A G-code command `Mnnn` / `Gnnn` | `git grep -n "GcodeSuite::M913"`. Handlers are in `Marlin/src/gcode/<group>/`, and some files hold several (`M911-M914.cpp`). The dispatch `switch` is in `Marlin/src/gcode/gcode.cpp`, and declarations are in `gcode/gcode.h` |
| What runs each loop | `setup()` and `loop()` in `Marlin/src/MarlinCore.cpp` |
| Motion, planner, stepper ISR | `Marlin/src/module/` (`motion`, `planner`, `stepper`) |
| Temperature, PID, MPC | `Marlin/src/module/temperature.cpp` |
| Optional feature code | `Marlin/src/feature/` and `gcode/feature/` |
| An option's derived flags (`HAS_*`) | `Marlin/src/inc/Conditionals-*.h` |
| Why a config combination is rejected | `Marlin/src/inc/SanityCheck.h`; renamed options are in `Changes.h` |
| A board's pins | `Marlin/src/pins/<family>/pins_<BOARD>.h`; the `#elif MB(...)` chain is in `pins/pins.h`; the ID is in `core/boards.h` |
| Platform-specific code | `Marlin/src/HAL/<FAMILY>/`, and that folder's `AGENTS.md` |
| An LCD string | `Marlin/src/lcd/language/language_en.h` (`MSG_*`) |
| A menu screen | `Marlin/src/lcd/menu/menu_*.cpp` |
| A serial or touch display | `Marlin/src/lcd/extui/` |
| Which source folders a build includes | `ini/features.ini` and `ini/*.ini` |
| A CI test config | `buildroot/tests/<env>/config-NN.ini` |

## Search recipes

    git grep -n "OPTION_NAME" -- Marlin                  # every use, including config comments
    git grep -n "SOME_OPTION" -- 'Marlin/src/inc/*'      # where it is derived or validated
    git grep -nE "\bfunction_name\(" -- Marlin/src       # calls and definitions
    git grep -n "STR_SOMETHING\|MSG_SOMETHING" -- Marlin/src/lcd
    git log -S"suspicious_string" --oneline -- Marlin/src   # when it was introduced or removed
    git log --oneline -20 -- Marlin/src/module/motion.cpp   # recent changes to a file
    git blame -L 120,160 Marlin/src/module/motion.cpp       # who changed these lines, then read that PR

## Tracing a setting or command

1. Find the option in `Marlin/Configuration.h` or `Configuration_adv.h`.
2. `git grep` it under `Marlin/src/inc` to see what it enables and what it conflicts with.
3. `git grep` it under the rest of `Marlin/src` to find the code it gates.
4. For a G-code, go from the `case` in `gcode/gcode.cpp` to its handler, then follow the calls into `module/`.
5. If a symbol seems to have no users, check for macro use (`TERN`, `ENABLED`) and for different spellings under other options before deciding it is dead.

## Pitfalls

- **Most code is conditional.** A match inside `#if ENABLED(X)` is not compiled unless `X` is on. Read the enclosing `#if`s, and build with the option on and off.
- **Macros hide calls.** `SERIAL_ECHO*`, `TERN*`, `LOOP_*`, and `HAL_PATH` includes do not look like ordinary function calls, so search for the macro name too.
- **Old issues and forks use old paths.** `ultralcd.*` is now `marlinui.*`, and `Conditionals_post.h` was split into the numbered files. Search the current name before applying advice from an old thread.
- **Line numbers drift.** Quote a symbol and a file, not only a line number, in a PR or comment.
- **IDE diagnostics can mislead.** Marlin builds with GCC. A Clang-based language server often reports errors on headers that build fine. Trust the build (skill: marlin-build-test).

## Optional tools

None of these are required, and none replace reading the code.

- `rg` (ripgrep) is faster than `git grep` on the whole tree and handles the same patterns.
- Your AI tool's own search (for example Claude Code's file and content search, or a code-exploration sub-agent) works well for broad questions such as "where is homing decided". Check its answer with `git grep`.
- A symbol index such as `universal-ctags` helps with jump-to-definition in an editor. Because of the `#if` blocks it can show several definitions of one name.
- A code-knowledge-graph MCP server (for example codebase-memory-mcp) can locate definitions, line ranges, and function size from a natural-language query. In testing on Marlin, its call-graph results were incomplete: callers came back at file level and macro-dispatched code had no callees. Use it to locate code and `git grep` to find every use. It indexes a snapshot of the checked-out tree, so re-index after switching branches.
- If you try a language-server plugin, expect noise from the heavy preprocessor use and keep a real build as the final check.
