---
name: marlin-pr-workflow
description: Use when preparing, splitting, or submitting a Marlin Pull Request. Covers PR size, branch, commit messages, rebase, and the PR template.
---

# Marlin Pull Request Workflow

Policy background: `.github/contributing.md` (AI-Assisted Contributions) and `AGENTS.md` section 11.

## 1. Keep it small

A PR does one thing and builds on its own. If your working tree holds more than one logical change, or a reviewer would struggle to follow it, split it into two or more PRs:

- refactor first, behavior change second
- core logic first, LCD/UI support second
- one HAL, board family, or pin file group per PR
- unrelated fixes in their own PRs

To split: create a new branch from `upstream/bugfix-2.1.x` for each piece and bring over only its changes (`git checkout <work-branch> -- <paths>` or `git cherry-pick -n`, then commit selectively with `git add -p`). Build each branch separately. State any dependency between the PRs in their descriptions.

## 2. Branch

    git fetch upstream
    git checkout -b my_change upstream/bugfix-2.1.x

PRs target `bugfix-2.1.x`, never a release branch.

## 3. Before committing

- Format only your changed files (skill: marlin-code-style).
- Build the affected environments (skill: marlin-build-test).
- Restore the config files after testing (`buildroot/bin/restore_configs` discards uncommitted edits to them, so commit any real config changes first). Don't commit test-only config edits.
- Add tests for any new feature or option: a `buildroot/tests/<env>/config-NN.ini` build test, and a unit test under `Marlin/tests` when the logic can run without hardware (skill: marlin-build-test).
- Review `git diff` for unrelated changes.

## 4. Commit

- Imperative mood, 72 characters or fewer on the first line, gitmoji-style emoji prefix like recent `git log` entries (🐛 fix, ✨ feature, ⚡️ performance, 🔧 config, 🩹 minor fix, 🧑‍💻 developer, 🚸 UX).
- One logical change per commit. Squash fixups.

## 5. Keep current

Rebase, don't merge:

    git fetch upstream
    git rebase upstream/bugfix-2.1.x
    git push -f

## 6. Open the PR

Fill in every section of `.github/pull_request_template.md`. No issue numbers in the title. List what you built and tested, and on what hardware if any. Attach a Configurations ZIP if reviewers need one to test.

The `mf*` helper scripts (`mfnew`, `mfrb`, `mfpr`, ...) described at https://marlinfw.org/docs/development/git_scripts.html are optional conveniences for these steps.
