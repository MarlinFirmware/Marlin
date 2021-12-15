## CR-10 Mini with BLTouch

Configuration for the BigTreeTech SKR E3 Mini V2 in a Creality CR-10 Mini using the stock CR-10 display, and a BL Touch v3.1 ABL probe mounted on a Bullseye fan duct. Additional features are enabled to take advantage of the capabilities of the BTT SKR E3 Mini V2 board, detailed below.

Compile this in the `STM32F103RC_btt_512K` environment. The test board had 512K of Flash and this configuration uses a little over half of that. If your board has only 256K, you will have to disable some features to make the firmware fit.

### Configuration.h notes:

```cpp
#define USE_PROBE_FOR_Z_HOMING
```
This configuration uses only the probe for Z homing. The Z-stop switch is NOT enabled and can be disconnected.

```cpp
#define Z_MIN_PROBE_PIN PC14
```
Plug the BLTouch Black/White connector into the top two pins of the 5-pin Z-PROBE port with the white whire "up". Do not plug the connector in to the Z-MIN port where the limit switch was plugged in.

```cpp
#define NOZZLE_TO_PROBE_OFFSET { -38, -8, 0 }
```
These are the offsets for a left-side mounted BL Touch on a Bullseye fan duct base. Change them as required for your particular BL Touch mount.

```cpp
#define MULTIPLE_PROBING 2
```
Bed probing will test each point twice. 1st probe will be "fast" Z, 2nd will use the slower Z rate.

```cpp
#define Z_MIN_PROBE_REPEATABILITY_TEST
```
M48 Enabled to establish probe deviation value.

```cpp
#define AUTO_BED_LEVELING_BILINEAR
```
Change this as desired. BILINEAR will work for most printers.

```cpp
//#define RESTORE_LEVELING_AFTER_G28
```
This is disabled to work around an issue where —even with an `M420` command in start G-Code— ABL would toggle to the opposite of whatever the ABL state was (Enabled/Disabled) at the time a print job started.

```cpp
#define GRID_MAX_POINTS_X 5
```
This configuration is set to use a 5x5 (25 point) probing grid. Change as desired.

```cpp
#define EXTRAPOLATE_BEYOND_GRID
```
By default, this is disabled. Enabling this seemed to provide better mesh data.

```cpp
#define LCD_BED_LEVELING
```
Provides control panel probe controls.

```cpp
#define LEVEL_BED_CORNERS
```
Provides control panel bed tramming controls.

```cpp
#define Z_SAFE_HOMING
```
Ensures the BL Touch probe is not hanging off the edge of the bed when Z homing.

```cpp
#define CR10_STOCKDISPLAY
```
If you are using the stock display on your CR-10 Mini, this **MUST** be enabled.

### Configuration_adv.h notes:

```cpp
#define LIN_ADVANCE
```
This is enabled, but the K value is set to 0 which effectively disables LIN_ADVANCE. Calibrate Linear Advance and set your own K value and recompile.

```cpp
#define ARC_SUPPORT
```
Enables G2/G3 moves to smooth curves in your prints. Required for the Arc Welder plugin for OctoPrint etc.

```cpp
#define ARC_P_CIRCLES
```
Normally disabled by default.
