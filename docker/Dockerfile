FROM python:3.11-bookworm

# Install build dependencies for simulator UI
RUN apt-get update && apt-get install -y \
    libsdl2-dev \
    libsdl2-net-dev \
    libgl1-mesa-dev \
    libegl1-mesa-dev \
    libglu1-mesa-dev \
    libx11-dev \
    libxext-dev \
    libxrandr-dev \
    libxinerama-dev \
    libxcursor-dev \
    libxi-dev \
    libglm-dev \
    && rm -rf /var/lib/apt/lists/*

# Disable warnings about not having a TTY
ARG DEBIAN_FRONTEND=noninteractive

# Disable debconf warnings
ARG DEBCONF_NOWARNINGS="yes"

# Upgrade pip
RUN pip install --upgrade pip

# Install platformio toolchain / framework and pyyaml
RUN pip install -U platformio PyYaml

# Upgrade platformio using development version / branch
RUN pio upgrade --dev

# Set working directory
WORKDIR /code

# Set volumes / mount points that we are using
VOLUME /code /root/.platformio

#ENV PATH /code/buildroot/bin/:/code/buildroot/tests/:${PATH}
