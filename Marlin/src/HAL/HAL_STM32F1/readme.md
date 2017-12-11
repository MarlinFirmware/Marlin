# This HAL is for STM32F103 boards used with libmaple/stm32duino Arduino core.

# This HAL is in development and has not been tested with an actual printer.

### The stm32 core needs a modification in the file util.h to avoid conflict with Marlin macros for Debug.
Since only 1 file needs change in the stm32duino core, it's preferable over making changes to Marlin.


After these lines:
<>
#else
#define ASSERT_FAULT(exp) (void)((0))
#endif
<>

Add the following 3 lines:
<>
#undef DEBUG_NONE
#undef DEBUG_FAULT
#undef DEBUG_ALL
<>

### Main developers:
Victorpv


### Most up to date repository for this HAL:
https://github.com/victorpv/Marlin/tree/bugfix-2.0.x

PRs should be first sent to that fork, and once tested merged to Marlin bugfix-2.0.x branch.


