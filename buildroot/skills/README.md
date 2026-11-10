# Marlin Contributor Skills

Step-by-step procedures for working on Marlin, written for AI coding agents and equally useful as human cheat sheets. Each skill is a folder containing a `SKILL.md` with YAML front matter (`name`, `description`) followed by instructions. They refer to the scripts in `buildroot/bin` and the `Makefile` so the procedures stay accurate as the scripts evolve.

All commands assume the current directory is the repository root.

| Skill | Purpose |
| --- | --- |
| [marlin-build-test](marlin-build-test/SKILL.md) | Build a board environment, apply test configs, run CI-style tests |
| [marlin-config-options](marlin-config-options/SKILL.md) | Find, enable, disable, and set configuration options from the command line |
| [marlin-code-style](marlin-code-style/SKILL.md) | Follow the Marlin coding standards and indentation; leave existing code unformatted |
| [marlin-pr-workflow](marlin-pr-workflow/SKILL.md) | Prepare a focused, reviewable Pull Request |
| [marlin-solving-bugs](marlin-solving-bugs/SKILL.md) | Investigate, isolate, fix, and verify a bug report |
| [marlin-extui-backends](marlin-extui-backends/SKILL.md) | Add or change ExtUI display backends and the ExtUI API |
| [marlin-unit-tests](marlin-unit-tests/SKILL.md) | Write and run native unit tests; macOS GCC setup |
| [marlin-codebase-navigation](marlin-codebase-navigation/SKILL.md) | Find definitions, uses, and flows in the source with git and grep |
| [marlin-hal-work](marlin-hal-work/SKILL.md) | Add a board, extend a HAL, or create a new HAL |
| [marlin-fork-harvesting](marlin-fork-harvesting/SKILL.md) | Evaluate a Marlin fork and port its features cleanly |
| [marlin-mainsail-connector](marlin-mainsail-connector/SKILL.md) | Run Mainsail/Fluidd with a Marlin printer or the simulator through serial2moon |
| [marlin-hosts-in-docker](marlin-hosts-in-docker/SKILL.md) | Run OctoPrint or Mainsail in Docker against the simulator or a printer to test host-facing behavior |

## Adding a skill

- Make a folder `buildroot/skills/<skill-name>/` with a `SKILL.md`.
- Start `description` with when to use the skill. Keep the skill short and procedural.
- Refer to scripts in `buildroot/bin` instead of duplicating their logic, and verify every command you document.
- Skills are for all contributors. Don't include personal paths or maintainer-only procedures.
- Add the skill to the table above.
