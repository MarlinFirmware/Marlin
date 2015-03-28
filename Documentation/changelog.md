### Version 1.0.3
* Reduced code size, maybe a lot depending on your configuration.
* Improved support for Delta, SCARA, and COREXY kinematics.
* Move parts of Configuration files to `Conditionals.h` and `SanityCheck.h`.
* Clean up of temperature code.
* Enhanced `G29` with improved grid bed leveling based on Roxy code. See documentation.
* Various bugs fixed from 1.0.2.
* EEPROM layout updated to `V17`.
* Added `M204` travel acceleration options.
* `M204` "`P`" parameter replaces "`S`." "`S`" retained for backward compatibility.
* Support for more RAMPS-based boards.
* Configurator utility under development.
* `M404` "`N`" parameter replaced with "`W`." ("`N`" is for line numbers only).
* Much cleanup of the code.
* Improved support for Cyrillic and accented languages.
* LCD controller knob acceleration.
* Improved compatibility with various sensors, MAX6675 thermocouple.
* Filament runout sensor support.
* Filament width measurement support.
* Support for TMC and L6470 stepper drivers.
* Better support of G-Code `;` comments, `\`, `N` line numbers, and `*` checksums.
* Moved GCode handling code into individual functions per-code.

### Version 1.0.2
* Progress bar for character-based LCD displays.

### Version 1.0.1

### Version 1.0.0
* Initial release
