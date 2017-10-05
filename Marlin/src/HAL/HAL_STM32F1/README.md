# This HAL is for STM32F103 boards used with libmaple/stm32duino Arduino core.

# This HAL is in development and has not been tested with an actual printer.

### Working functionality
+USB serial communication
+Steppers (over 60Khz max with single stepping)
+ADC (Using DMA for low cpu usage and latency) 
+I2C Eeprom
+Eeprom emulation in flash (without wear levelling, so should not enable print counters)

### Not working or not tested
+LCDs (possible issue with drivers written for AVR)
+SPI Flash (not tested, should work out of the box)
+Buttons (not tested, should work out of the box)
+Endstops (not tested, should work out of the box)
+PWM output for FETs (not tested, should work out of the box)
+USART ports (not tested, should work out of the box)
+SDCard (not working at first test, but should work with small changes, perhaps clock)


### The stm32 core needs a modification in the file util.h to avoid conflict with Marlin macros for Debug.
Since only 1 file needs change in the stm32duino core, it's preferable over making changes to Marlin.
This does not affect stm32duino/libmaple functionality in any way, since those #defines are only using within this one file, and we undefine them after they are used


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

### Main developers for this HAL:
Victorpv
xC0000005


### Most up to date repository for this HAL:
https://github.com/victorpv/Marlin/tree/bugfix-2.0.x

PRs should be first sent to that fork, and once tested merged to Marlin bugfix-2.0.x branch.


