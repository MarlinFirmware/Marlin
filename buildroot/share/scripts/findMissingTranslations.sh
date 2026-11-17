#!/usr/bin/env bash
#
# findMissingTranslations.sh
#
# Locate all language strings needing an update based on English
#
# Usage: findMissingTranslations.sh [language codes]
#
# If no language codes are specified then all languages will be checked.
# See languageCheck.py for more options (lint, JSON output, etc.).
#

HERE=$(cd "$(dirname "$0")" && pwd)
cd "$HERE/../../.." || exit 1
exec python3 "$HERE/languageCheck.py" --missing "$@"
