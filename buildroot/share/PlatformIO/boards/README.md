# Marlin Custom Boards

Boards are specified in the INI files with the `board = board_name` field. If the board doesn't exist in the platform folder downloaded by PlatformIO then it should be defined in this folder.

The board definition JSON files in this folder provide build and upload information about boards that Marlin supports but which are not defined by any platform included with PlatformIO.

## Custom Variants

See the `buildroot/PlatformIO/variants` folder for custom variants referred to by `build.variant` in the JSON.
