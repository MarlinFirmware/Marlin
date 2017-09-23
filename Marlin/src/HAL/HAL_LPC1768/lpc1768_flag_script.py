import sys

#dynamic build flags for generic compile options
if __name__ == "__main__":
  args = " ".join([ "-std=gnu11",
                    "-std=gnu++11",
                    "-Os",
                    "-mcpu=cortex-m3",
                    "-mthumb",

                    "-ffreestanding",
                    "-fsigned-char",
                    "-fno-move-loop-invariants",

                    "--specs=nano.specs",
                    "--specs=nosys.specs",

                    "-IMarlin/src/HAL",
                    "-IMarlin/src/HAL/HAL_LPC1768/include",

                    "-MMD",
                    "-MP",
                    "-DTARGET_LPC1768",
                    "-DIS_REARM"
                  ])

  for i in range(1, len(sys.argv)):
    args += " " + sys.argv[i]

  print args

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
      ],
  )
