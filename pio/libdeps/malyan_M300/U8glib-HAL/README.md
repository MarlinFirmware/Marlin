# U8glib-HAL

u8g by Oliver Kraus with extensions for ARM and the Marlin HAL.

The extensions allow access to the u8g library by "non native" CPUs/systems.

AVR, Arduino, and Due access to the library is not changed.

Enabling the extensions by defining the flag **`U8G_HAL_LINKS`**. This can be done in `platformio.ini` by adding **`-DU8G_HAL_LINKS`** to any environment's build flags.

### The following files provide the links between the u8g library and the "non native" CPUs/systems.

#### `#include <LCD_defines.h>`
The `LCD_defines.h` header brings in the top level link file which:
- Defines link files for SPI routines, delay routines, I2C routines and class definitions.
- Connects u8g com generic com defines to the desired HAL com routine.

#### `#include <LCD_delay.h>`
Contains code or declarations for the u8g routines:
- `void U8g_delay(int msec)`
- `void u8g_MicroDelay(void)`
- `void u8g_10MicroDelay(void)`

#### `#include <LCD_pin_routines.h>`
Contains code or declarations for the u8g routines:
- `void u8g_SetPinOutput(uint8_t internal_pin_number)`
- `void u8g_SetPinInput(uint8_t internal_pin_number)`
- `void u8g_SetPinLevel(uint8_t  pin, uint8_t  pin_status)`
- `uint8_t u8g_GetPinLevel(uint8_t pin)`

#### `#include <LCD_I2C_routines.h>`
Contains code or declarations for the u8g routines:
- `void u8g_i2c_init(uint8_t options)`
- `uint8_t u8g_i2c_wait(uint8_t mask, uint8_t pos)`
- `uint8_t u8g_i2c_start(uint8_t sla)`
- `uint8_t u8g_i2c_send_byte(uint8_t data)`
- `void u8g_i2c_stop(void)`
