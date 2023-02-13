# GNU ARM Embedded Toolchain Installation

1. download the [ARM GNU Toolchain for bare-metal target (arm-none-eabi) Version 12.2.rel1](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads) and place it in this folder
    - the file should be called `arm-gnu-toolchain-12.2.rel1-x86_64-arm-none-eabi.tar.xz` (for x86_64 host)
2. if the version or architecture you downloaded does not match `12.2.rel1` and `x86_64`, adjust the `VERSION` and `HOST_ARCH` variables in `install-toolchain.sh` and `uninstall-toolchain.sh`
3. run `install-toolchain.sh` to install the toolchain
4. if you want to uninstall the toolchain, run `uninstall-toolchain.sh`
