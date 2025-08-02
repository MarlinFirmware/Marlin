#!/bin/bash

# Get host user info
USERNAME=$(whoami)
USER_ID=$(id -u)
GROUP_ID=$(id -g)

# Build the container with correct args
docker build -t marlin-dev \
  --build-arg USERNAME="$USERNAME" \
  --build-arg USER_ID="$USER_ID" \
  --build-arg GROUP_ID="$GROUP_ID" \
  -f docker/Dockerfile .

echo "run all tests in the docker container:"
echo "make tests-all-local-docker"
echo "or a single test like:"
echo "make tests-single-local-docker TEST_TARGET=mega2560"

