# This HAL is for STM32F103 boards used with libmaple/stm32duino Arduino core.

# This HAL is in development. Currently has been tested in Malyan M200 (103CBT6), Chitu 3d (103ZET6), and custom boards(103VET6).

### The stm32 core needs a modification in the file util.h to avoid conflict with Marlin macros for Debug.
Since only 1 file needs change in the stm32duino core, it's preferable over making changes to Marlin.


After these lines:
<>
#else
#define ASSERT_FAULT(exp) (void)((0))
#endif
<>

### Main developers:
Victorpv
xC000005


### Most up to date repository for this HAL:
https://github.com/victorpv/Marlin/tree/bugfix-2.0.x

PRs should only be sent to Marlin bugfix-2.0.x branch once tested in printing so not to introduce new bugs.
For testing/dev, you can submit to the above branch
