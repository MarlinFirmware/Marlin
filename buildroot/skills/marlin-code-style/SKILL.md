---
name: marlin-code-style
description: Use before committing C/C++ changes to Marlin, to match project conventions and indentation in new code without reformatting existing code.
---

# Marlin Code Style

Full standards: https://marlinfw.org/docs/development/coding_standards.html

## Rules of thumb

- Match the file you are editing: indentation (2 spaces, no tabs), brace placement, spacing, naming.
- Code inside preprocessor blocks (`#if` / `#else` / `#endif`) is indented one level per nesting depth, the same as code inside braces. Copy how nearby code does it.
- Use `ENABLED(FEATURE)` / `DISABLED(FEATURE)` for options, and `#if HAS_...` for derived capabilities.
- Keep optional code behind its feature flag so it costs nothing when disabled.
- Code must build on 8-bit AVR as well as 32-bit targets. Avoid unnecessary `double`, `String`, and heap allocation.
- Comments should say why, concisely. End every file with a newline. No trailing whitespace.

## Don't reformat existing code

- Existing code may have ad hoc alignment and spacing (aligned columns, grouped one-liners). Leave it alone.
- Don't re-indent, re-wrap, or re-space lines you aren't otherwise changing. Keep diffs small and focused.
- Exception: It's good to keep related eol comments aligned.
- Review `git diff` before committing and revert any whitespace-only churn.

## Auto-formatters

Don't run the auto-formatter on existing files. `buildroot/bin/format_code` and `uncrust` (uncrustify) get close to the Marlin style but don't match it exactly, and will produce unwanted churn. If you use one, apply it only to a scratch copy to see suggestions, then hand-apply what is correct to your own new lines.
