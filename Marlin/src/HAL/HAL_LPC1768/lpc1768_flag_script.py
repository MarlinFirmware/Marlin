#dynaomic build flags for generic compile options
if __name__ == "__main__":
  print " ".join([  "-std=gnu11",
                    "-std=gnu++11",
                    "-Os",
                    "-mcpu=cortex-m3",
                    "-mthumb",

                    "-ffreestanding",
                    "-fsigned-char",
                    "-fno-move-loop-invariants",

                    "-Wall",
                    "--specs=nano.specs",
                    "--specs=nosys.specs",

                    "-IMarlin/frameworks/CMSIS/LPC1768/include",
                    "-IMarlin/frameworks/CMSIS/LPC1768/lib",
                    "-IMarlin/src/HAL",

                    "-MMD",
                    "-MP",
                    "-DTARGET_LPC1768",
                    "-DIS_REARM"
                  ])

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
          "-Os",
          "-mcpu=cortex-m3",
          "-ffreestanding",
          "-mthumb",
          "--specs=nano.specs",
          "--specs=nosys.specs",
          "-u_printf_float",
          "-Wl,-TMarlin/frameworks/CMSIS/LPC1768/Re-ARM/LPC1768.ld,--gc-sections"
      ],
  )
