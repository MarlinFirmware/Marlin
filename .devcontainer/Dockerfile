# See here for image contents: https://github.com/microsoft/vscode-dev-containers/tree/v0.194.0/containers/ubuntu/.devcontainer/base.Dockerfile

# [Choice] Ubuntu version: bionic, focal
ARG VARIANT="focal"
FROM mcr.microsoft.com/vscode/devcontainers/base:0-${VARIANT}


RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
     && apt-get -y install --no-install-recommends \
     python3 \
     python3-pip \
     build-essential

RUN ln -s /usr/bin/python3 /usr/bin/python

RUN pip install -U https://github.com/platformio/platformio-core/archive/develop.zip
RUN platformio update
# To get the test platforms
RUN pip install PyYaml