#
# STM32F1_build_flags.py
# Add build_flags for the base STM32F1_maple environment (stm32f1-maple.ini)
#
import pioutil
if pioutil.is_pio_build():

    # Dynamic build flags for generic compile options
    pioutil.env.Prepend(BUILD_FLAGS=[
        "-std=gnu++14",
        "-Os",
        "-mcpu=cortex-m3",
        "-mthumb",

        "-fsigned-char",
        "-fno-move-loop-invariants",
        "-fno-strict-aliasing",

        "--specs=nano.specs",
        "--specs=nosys.specs",

        "-MMD", "-MP",

        "-IMarlin/src/HAL/STM32F1",

        "-DTARGET_STM32F1",
        "-DARDUINO_ARCH_STM32",
        "-DPLATFORM_M997_SUPPORT"
    ])
