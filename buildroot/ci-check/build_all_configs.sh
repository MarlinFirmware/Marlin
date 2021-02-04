#!/bin/bash
echo "This script will attempt to build Marlin for all known configurations script"
echo "In case of failure, the current configuration remains on your repository"
echo "To revert to your current version, run 'git checkout -f' "
self=`basename "$0"`


# Check dependencies
which curl 1>/dev/null 2>&1 || { echo "curl not found, please install it"; exit 1; }
which git 1>/dev/null 2>&1 || { echo "git not found, please install it"; exit 2; }
if [ -z "$1" ]; then
  echo "Expected calling parameter: $self base_branch"
  exit 3
fi

# Check if called in the right folder
if [ ! -e "Marlin/src" ]; then
  echo "This script must be called from the root folder of a Marlin repository, please navigate to this folder and call:"
  echo "buildroot/ci-check/$self $1"
  exit 4
fi

# Check if the current repository has unmerged changes
git diff --quiet || { echo "Your current repository is not clean. Either commit your change or stash them, and re-run this script"; exit 5; }

# Ok, let's do our stuff now
# First extract the current temporary folder
echo "Fetching configuration repository"
if [ ! -e "./tmp/README.md" ]; then
  git clone --single-branch --branch "$1" https://github.com/MarlinFirmware/Configurations.git ./tmp || { echo "Failed to clone the configuration repository"; exit 6; }
fi

echo 
echo "Start building now..."
echo "====================="
shopt -s nullglob
for config in tmp/config/examples/*/; do
  [ -d "${config}" ] || continue
  base=`basename "$config"`
  echo "Testing $base: "
    ./buildroot/ci-check/build_config.sh "internal" "$base" "$config" || { echo "Failed to build $base"; exit 7; }
  echo "Passed"
done