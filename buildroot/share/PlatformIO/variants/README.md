# Marlin Custom Variants

This `buildroot/share/PlatformIO/variants` folder contains Marlin custom variants for both generic and custom boards.

Marlin specifies board variants in PlatformIO INI files in one of two ways:
- The `board_build.variant = VARIANT_NAME` field specifies the variant subfolder name directly.
- The `board = board_name` field names a custom board JSON file that contains a `build.variant` field.

## Variant File Naming

With the latest STM32 platform (17.x) the `variant.h` and `variant.cpp` files are required to have more unique names. If the variant is based on a generic board definition the name `variant_generic.h`/`.cpp` should be used. Otherwise, the capitalized name of the `board` should be used.

### Examples

| board | board file | variant | Variant Files |
|-------|------------|---------|---------------|
|`marlin_STM32F407VGT6_CCM`|`marlin_STM32F407VGT6_CCM.json`|`MARLIN_BTT_E3_RRF`|`variants/MARLIN_BTT_E3_RRF/variant.*`|
|`genericSTM32F103VE`|n/a|`MARLIN_F103Vx`|`variants/MARLIN_F103Vx/variant_generic.*`|
|`marlin_STM32F407ZE`|`marlin_STM32F407ZE.json`|`MARLIN_F407ZE`|`variants/MARLIN_F407ZE/variant_MARLIN_F407ZE.*`|

# Marlin Custom Boards

The `buildroot/share/PlatformIO/boards` folder contains Marlin's custom board definition JSON files. These files provide hardware IDs, board statistics, additional build flags, custom variant name, linker definition filename, remote debug options, upload devices, etc.
