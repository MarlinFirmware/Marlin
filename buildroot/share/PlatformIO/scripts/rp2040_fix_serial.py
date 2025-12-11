#!/usr/bin/env python
"""
rp2040_fix_serial.py

Resolves multiple definition linker error for _UART1_ symbol on RP2040 builds.

PROBLEM:
--------
Marlin's serial port numbering: -1=USB, 0=UART0, 1=UART1
Arduino's serial port numbering: Serial=USB, Serial1=UART0, Serial2=UART1

To remap these, Marlin creates a custom Serial0 object backed by UART0 with
configurable pins (SERIAL1_TX_PIN/SERIAL1_RX_PIN). However, the Arduino
framework also creates a Serial1 object backed by the same UART0 hardware,
resulting in duplicate _UART1_ symbols at link time.

SOLUTION:
---------
Apply -Wl,--allow-multiple-definition ONLY during the linking phase (not during
compilation). This is more targeted than applying it globally in build_flags.

The linker will use the FIRST definition it encounters (Marlin's Serial0),
effectively making the Arduino framework's Serial1 object unused but harmless.

WHY THIS IS SAFE:
-----------------
1. The conflict is limited to a single symbol (_UART1_) with identical underlying
   hardware access
2. Marlin never uses the Arduino framework's Serial1, only its own Serial0
3. The linker flag is applied ONLY at link time, not during compilation
4. The alternative (modifying the Arduino framework) is not maintainable

ALTERNATIVES CONSIDERED:
------------------------
- Using weak symbols: Requires modifying Arduino framework source
- Runtime pin reconfiguration: arduino::UART lacks setTX/setRX methods
- Not creating Serial0: Would require extensive Marlin serial code changes

WHY USE A SCRIPT VS BUILD_FLAGS:
---------------------------------
Functionally, there is NO DIFFERENCE between adding -Wl,--allow-multiple-definition
to build_flags in platformio.ini versus using this script. Both pass the same flag
to the linker and have the same global scope (all symbols during linking).

The GNU linker does NOT support applying --allow-multiple-definition to specific
symbols only - it's all-or-nothing for the entire link operation.

This script exists ONLY for documentation purposes:
1. The extensive comments here explain WHY this workaround is needed
2. It makes it clear this is an intentional workaround, not a mistake
3. Future maintainers can understand the specific issue being solved
"""
Import("env")

# Apply the linker flag globally during linking
# NOTE: This affects ALL symbols, not just _UART0_/_UART1_
# The GNU linker does not support selective symbol-level granularity
env.Append(LINKFLAGS=[
    "-Wl,--allow-multiple-definition",  # Required for Marlin Serial0/Serial1 vs Arduino Serial1/Serial2
])

