# Creality Ender-3

The configuration is based on Creality3D's Github but with some improvements. All Features of the original Firmware are included.

For U8Glib, at least version 1.14 and at most 1.17 is used, because

* 1.12 didn't have the extra speed argument to u8g_InitCom.
* 1.13 didn't have the soft reset instruction for UC1701 initialization.
* 1.18 has a new directory structure.

## Bitmaps

The bootscreen and custom status screens come from Creality's firmware.
