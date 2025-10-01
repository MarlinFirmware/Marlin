#!/usr/bin/env python
"""
Extract the builds used in Github CI, so that we can run them locally
"""
import yaml

# Set the yaml file to parse
yaml_file = '.github/workflows/ci-build-tests.yml'

# Parse the yaml file, and load it into a dictionary (github_configuration)
with open(yaml_file) as f:
    github_configuration = yaml.safe_load(f)

# Print out the test platforms
print(' '.join(github_configuration['jobs']['test_builds']['strategy']['matrix']['test-platform']))
