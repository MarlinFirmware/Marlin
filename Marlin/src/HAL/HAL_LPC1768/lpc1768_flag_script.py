from __future__ import print_function
import sys

#dynamic build flags for generic compile options
if __name__ == "__main__":
  args = " ".join([ "-std=gnu11",
                    "-std=gnu++11",
                    "-Os",
                    "-mcpu=cortex-m3",
                    "-mthumb",

                    "-fsigned-char",
                    "-fno-move-loop-invariants",
                    "-fno-strict-aliasing",
                    "-fsingle-precision-constant",

                    "--specs=nano.specs",
                    "--specs=nosys.specs",

                    # For external libraries
                    "-IMarlin/src/HAL/HAL_LPC1768/include",

                    # For MarlinFirmware/U8glib-HAL
                    "-IMarlin/src/HAL/HAL_LPC1768/u8g",
                    "-DU8G_HAL_LINKS",

                    "-MMD",
                    "-MP",
                    "-DTARGET_LPC1768"
                  ])

  for i in range(1, len(sys.argv)):
    args += " " + sys.argv[i]

  print(args)

# extra script for linker options
else:
  from SCons.Script import DefaultEnvironment
  env = DefaultEnvironment()
  env.Append(
      ARFLAGS=["rcs"],

      ASFLAGS=["-x", "assembler-with-cpp"],

      CXXFLAGS=[
          "-fabi-version=0",
          "-fno-use-cxa-atexit",
          "-fno-threadsafe-statics"
      ],
      LINKFLAGS=[
          "-Wl,-Tframeworks/CMSIS/LPC1768/system/LPC1768.ld,--gc-sections",
          "-Os",
          "-mcpu=cortex-m3",
          "-mthumb",
          "--specs=nano.specs",
          "--specs=nosys.specs",
          "-u_printf_float"
      ],
  )
