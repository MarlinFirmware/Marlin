# Creality Ender Support

This branch is a reverse-engineered version based on the unpublished firmware from Creality. It is **not** the authoritative source, but has been carefully re-built by looking at their firmware and inferring the base version and configuration they used. The basis is the firmware version from "Jul 31 2017 10:16:30". It is based on Marlin 1.0.1, because

* 1.0.0 had very different serial output in `setup()` and overall code structure.
* 1.0.2 changed the `VERSION_STRING` to include a leading space, and `lcd_init` uses `SET_INPUT` instead of `pinMode`.

Configurations were found by seeing what code was compiled into the firmware, and constants used there.

For U8Glib, at least version 1.14 and at most 1.17 is used, because

* 1.12 didn't have the extra speed argument to u8g_InitCom.
* 1.13 didn't have the soft reset instruction for UC1701 initialization.
* 1.18 has a new directory structure.

## Bitmaps

The bootscreen and custom status screens come from Creality's firmware.
