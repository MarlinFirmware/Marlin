# AGENTS.md — HAL/SAM3U (Atmel AT91SAM3U4E)

Working notes for `Marlin/src/HAL/SAM3U/`, the HAL for the **4pi** controller
(`BOARD_4PI`, 3200). See `Marlin/src/HAL/AGENTS.md` for how the HAL folder as a
whole plugs into the `inc/` cascade and the `ini` build-time source selection.

## What makes this HAL different

It is the only Marlin HAL that builds **bare-metal on CMSIS with no Arduino
core**. Everything an Arduino core would normally provide is in this folder.

The nearest relative is `HAL/DUE` (SAM3X8E) — same vendor, same PIO/TC/UART
peripherals — and much here is recognisably descended from it. But the parts
are not interchangeable. The differences that actually shape the code:

| | SAM3X8E (Due) | SAM3U4E (4pi) |
|---|---|---|
| Arduino core | yes | **none** |
| Flash / SRAM | 512KB / 96KB | **256KB / 48KB usable** |
| Timer/Counter | 3 blocks × 3 ch, **32-bit** | 1 block × 3 ch, **16-bit** |
| PWM channels | 8 | 4 (none on 4pi loads) |
| PIO pulls | up and down | **pull-up only** |
| ADC | one 12-bit, 16 ch | 12-bit ADC12B (8 ch) + separate 10-bit ADC |
| USB | UOTGHS | UDPHS |
| Peripheral mux | A/B/C/D | **A/B only** (`PIO_ABSR`) |

### 16-bit timers are the big one

`hal_timer_t` is `uint16_t` and `HAL_TIMER_TYPE_MAX` is `0xFFFF`, as on AVR —
not `uint32_t` as on the Due. Marlin's stepper ISR already copes: it clamps the
interval to `HAL_TIMER_TYPE_MAX` and re-enters. Do not "fix" this by widening
the type.

The stepper clock is `TIMER_CLOCK3` (MCK/32 = 3MHz), chosen to mirror AVR's
proven 2MHz rather than for maximum resolution. That puts the slowest
representable step rate at `3000000 / 65535` ≈ 46 steps/s, far below any usable
feedrate. `TIMER_CLOCK2` (12MHz) would give finer resolution but raise the
floor to ~183 steps/s, which slow Z and extruder moves genuinely reach.

All three channels are spoken for — stepper (TC0 ch0), temperature (ch1), tone
(ch2). There is no fourth, which is why `inc/SanityCheck.h` rejects enabling
servos and sound together.

## Layout

```
include/          The Arduino-core stand-in. On the compiler's -I path.
  Arduino.h         The slice of the Arduino API Marlin's shared layer calls
  pinmapping.h      Pin numbering: (port * 32 + bit) -> PA0..PC31
  SPI.h             Arduino SPIClass + the global `SPI` object core code uses
  instance/         Generated CMSIS instance headers (see below)
startup.c         Vector table + Reset_Handler (replaces the package's)
main.cpp          Entry point: clock, SysTick, then setup()/loop()
clock.{h,cpp}     PLLA to 96MHz, flash wait states, PMC helpers
arduino.cpp       pinMode/digitalWrite/millis/micros/attachInterrupt/...
adc.{h,cpp}       ADC12B driver
timers.{h,cpp}    TC0's three channels
fastio.h          Direct PIO access; constant-folds for constant pins
MarlinSerial.*    The one hardware UART
MarlinSerialUSB.* Marlin serial over native USB
usb/               CDC-ACM device on UDPHS
HAL_SPI.cpp       Bit-banged SPI master
eeprom/           Flash-backed PersistentStore
inc/              Family Conditionals + SanityCheck
```

## Things that will bite you

**The CMSIS package is incomplete and partly broken.**
`framework-cmsis-atmel` ships the SAM3U `component/` and `pio/` headers but
*not* `instance/`, which `sam3u4e.h` includes unconditionally. `include/instance/`
holds generated replacements — regenerate with
`buildroot/share/scripts/gen_sam3u_instance_headers.py` if the package changes.
Its `startup_sam3u.c` also does not compile against its own `sam3u4e.h` (it
names reserved vector slots that struct does not have), which is why
`startup.c` here replaces it.

**`SPI` is a macro.** `sam3u4e.h` defines `SPI` as the peripheral base pointer,
which collides with the global `SPI` object Marlin core code expects.
`include/SPI.h` pulls the device header in first, saves the base as
`SAM3U_SPI_REGS`, then `#undef`s the macro. Keep that ordering.

**`SystemInit()` is never called.** The CMSIS `Reset_Handler` for this part does
not call it, so the clock is brought up explicitly at the top of `main()`.
Static constructors therefore run on the 4MHz internal RC — harmless, just slow.

**`WDT_MR` is write-once after reset.** `clock.cpp` disables the watchdog only
when `USE_WATCHDOG` is off; otherwise it leaves the 16s default running so
`MarlinHAL::watchdog_init()` gets the one write that counts.

**Flash wait states.** 96MHz needs `FWS = 3` (4 read cycles) per datasheet Table
42-55. The original 4pi/Sprinter firmware set 2 while running at 96MHz, which is
out of spec — do not copy that.

**Writing flash requires running from RAM.** `eeprom/eeprom_flash.cpp` puts its
programming routine in `.ramfunc` and masks interrupts around it, because the
controller cannot serve instruction fetches while programming. Verify after any
change that `flash_write_page` still links at a `0x2000xxxx` address.

**The fastio macros do not range-check.** `_WRITE`/`_READ` and friends are
deliberately unguarded so they collapse to a single store. A pin of `-1` does
not fail — it resolves to `PIOA` bit 31, which on the 4pi is `Y_DIR`. Any
optional pin reaching fastio must be wrapped in `PIN_EXISTS(...)` first.
`digitalWrite()`/`pinMode()` *are* guarded; the macros are not.

**`spi_pins.h` runs after `pins.h`.** In the config cascade
(`MarlinConfigPre-5-post.h`) `pins.h` and its `pins_postprocess.h` come first,
and postprocess has already forced `SD_SS_PIN` to `-1` if nothing supplied one.
So `spi_pins.h` can only *react* to a chip select, never default one — chip
selects belong in the board pins file or the user's config. Only the bus pins
(SCK/MISO/MOSI), which are fixed in silicon, are set there.

**No hardware PWM on any load.** Heaters and fans use Marlin's software PWM, as
the original 4pi firmware did. `inc/Conditionals_adv.h` forces `FAN_SOFT_PWM`
and `set_pwm_duty()` degrades to an on/off threshold.

## Build

`ini/sam3u.ini` defines `env:4pi`. `framework` is deliberately empty;
`buildroot/share/PlatformIO/scripts/sam3u_build.py` adds the CMSIS include
paths and restores libc/libm/libgcc (the bare builder links `-nostdlib`).
The linker script is Marlin's own —
`buildroot/share/PlatformIO/variants/SAM3U/sam3u4e_marlin.ld` — because the
stock one neither reserves flash for EEPROM nor lays SRAM out the way
`freeMemory()` expects.

```
pio run -e 4pi
```

Upload is SAM-BA. To reach the bootloader, short the RESET pads on top of the
board and power-cycle.

## USB

`usb/usb_cdc.cpp` is a self-contained CDC-ACM device on the SAM3U's UDPHS
controller — this is `SERIAL_PORT -1`, and how the 4pi is normally used. It was
written against the datasheet using the 4pi reference firmware's `at91lib` UDPHS
stack for the hardware sequences, but it is not a port of that framework.

Two decisions to be aware of before changing it:

- **It forces full speed.** UDPHS is high-speed capable, but 12Mbit/s already
  dwarfs what Marlin needs, and full speed means one descriptor set and 64-byte
  banks instead of two sets and 512-byte banks. To go high speed: drop the
  `UDPHS_TST` write in `usb_cdc_init()`, set `EP_BULK_SIZE` to 512, and add
  device_qualifier and other_speed_configuration descriptors.
- **It uses the FIFO, not DMA.** At 64 bytes a packet the copy is trivial.

`bulk_in_start()` masks the UDPHS interrupt because the transmit path is
reachable from both the ISR and the idle task; the ISR calls
`bulk_in_start_locked()` directly. Don't collapse the two.

## Serial ports

Five hardware ports plus USB, usable in any of Marlin's three slots
(`SERIAL_PORT`, `SERIAL_PORT_2`, `SERIAL_PORT_3`) in any combination:

| N | Peripheral | RX | TX | PIO function |
|---|---|---|---|---|
| -1 | USB CDC | — | — | — |
| 0 | UART | PA11 | PA12 | A |
| 1 | USART0 | PA19 | PA18 | A |
| 2 | USART1 | PA21 | PA20 | A |
| 3 | USART2 | PA23 | PA22 | A |
| 4 | USART3 | PC13 | PC12 | B |

All five are driven through a single `Uart*` view: the UART and the USARTs
share register offsets and bit positions for everything the driver touches
(CR, IER/IDR/IMR, SR/CSR, RHR, THR, BRGR). The one real difference is the mode
register — a USART needs word length, parity and stop bits programmed in
`US_MR`'s own field layout, where the UART's are fixed in hardware. `begin()`
branches on `DESC.is_usart` for exactly that.

**Do not test `USING_HW_SERIALn` in HAL headers.** Marlin sets those in
`Conditionals-5-post.h`, which runs *after* `HAL.h` enters the cascade, so they
read as undefined here and every port silently compiles out. `MarlinSerial.h`
defines `SAM3U_SERIAL_IN_USE(N)` from the port assignments directly for this
reason; use that.

Which ports are *usable* is a board question, not a HAL one. On the 4pi only
port 0 and USB are wired to anything free — every USART shares pins with a
board function, two of them with heater outputs — so `inc/SanityCheck.h`
rejects ports 2–4 and warns on port 1. Other SAM3U boards would relax that.

The Due's `HAL/DUE/usb/` is **not** reusable here — that is UOTGHS, a different
peripheral.

## Not implemented yet
- **Onboard SD.** The socket is on HSMCI (PA3..PA8, 4-bit), not SPI, so Marlin's
  stock `Sd2Card` cannot drive it. Needs an HSMCI driver.
- **Hardware SPI.** `HAL_SPI.cpp` bit-bangs; the only device on the bus is the
  AD5206 digipot, where it costs nothing.

## Related

- Board pin map: `Marlin/src/pins/sam3u/pins_4PI.h`
- Reference material: `referance material/` — the SAM3U datasheet, the original
  4pi/Sprinter firmware, and the board's schematic and pinmap.
