#!/usr/bin/env python
"""
Extract the builds used in Github CI, so that we can run them locally
"""
import yaml


with open('.github/workflows/test-builds.yml') as f:
	github_configuration = yaml.safe_load(f)
test_platforms = github_configuration\
	['jobs']['test_builds']['strategy']['matrix']['test-platform']
print(' '.join(test_platforms))
