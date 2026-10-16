# Novin3dp Cosmic Cyan theme for Marlin TFT Color UI

This first implementation adds a low-overhead visual theme for the existing Marlin TFT Color UI. It does not replace Marlin menu/action logic or change motion, thermal, probing, SD-card, or print-control behavior.

## Hardware target

- MKS Robin Nano V3.x family (STM32F407)
- MKS TS35 when connected as a native Marlin TFT Color UI display
- Deep navy background, cyan/ice-blue accents, restrained amber/red for heat and warning states

## Enable the theme

In the printer's `Marlin/Configuration.h`, in the TFT display configuration section, set:

```cpp
#define TFT_THEME NOVIN3DP
```

The display driver and UI must already be configured for the actual board/display. Do not enable a different display transport just to select this theme. If the configuration does not use `TFT_COLOR_UI`, this theme file will not affect the active interface.

## Memory approach

The theme uses RGB565 color constants and existing Marlin drawing primitives. It adds no full-screen background image, animation, framebuffer, or extra font, so the theme itself adds effectively no runtime image-buffer overhead.

Marlin's current TFT Color UI uses existing image assets, including 4-bpp image data. This commit does **not** claim that all icons have become 1-bpp. A separate image-format change must add a 1-bpp-capable renderer and compatible asset pipeline; simply replacing image arrays would not work with the current renderer.

Raw monochrome bitmap storage, excluding alignment/metadata:

- 24x24: 72 bytes
- 32x32: 128 bytes
- 48x48: 288 bytes
- 64x64: 512 bytes

## Design intent

- Keep temperature, bed, fan, XYZ coordinates and print status primary on the status page.
- Make Print/Files, Prepare/Move, Temperature, Leveling and Settings the main actions.
- Keep Pause/Resume and Stop explicit during printing.
- Use no full-screen decorative bitmap for the cosmic background.
- Use red for hotend/stop, amber for bed heat, cyan for normal controls and mint for positive/confirmed states.

## Validation status

This is a theme-only, non-default addition. It has not been compiled against a specific machine configuration or tested on physical TS35 hardware. Before enabling it on a production printer, build with the exact configuration and verify touch calibration, display orientation, temperatures and print-control actions.