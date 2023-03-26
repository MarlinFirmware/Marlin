#!/usr/bin/env python
"""
Extract the builds used in Github CI, so that we can run them locally
"""
import sys, yaml

# Handle the case where the user doesn't provide a yaml file
if len(sys.argv) != 2:
    print('Usage: get_test_targets.py <path to yaml file>')
    sys.exit(1)

# Get the yaml file from the command line
yaml_file = sys.argv[1]

# Parse the yaml file, and load it into a dictionary (github_configuration)
with open(yaml_file) as f:
    github_configuration = yaml.safe_load(f)

# Print out the test platforms
print(' '.join(github_configuration['jobs']['test_builds']['strategy']['matrix']['test-platform']))

# Exit with success
sys.exit(0)