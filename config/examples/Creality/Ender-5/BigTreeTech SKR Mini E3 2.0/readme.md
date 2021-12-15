# Ender-5 with BigTreeTech SKR Mini E3 v2.0

## Build with "Auto Build Marlin"

For the best experience building with PlatformIO in VSCode, open up the "Extensions" panel and install the "Auto Build Marlin" extension. After installation, open up the Auto Build Marlin view and click the "Build" button for the "512K" environment.

## Build with PlatformIO

For direct PlatformIO build, locate the `env:STM32F103RC_btt_512K` group the PlatformIO panel and click the "Build" item in that group to start the build.

To make the main "Build" button apply to this board's environment, edit the `platformio.ini` section `[platformio]` and change `default_envs` to

```ini
default_envs = STM32F103RC_btt_512K
```

## Older Leadscrew

**Note:** This Ender-5 `Configuration.h` applies to the newer leadscrew with 800 steps/mm. If your printer has the older leadscrew, change the Z component of `DEFAULT_AXIS_STEP_PER_UNIT` to 400 as in the example below:

```cpp
#define DEFAULT_AXIS_STEPS_PER_UNIT { 80, 80, 400, 93 }
```
