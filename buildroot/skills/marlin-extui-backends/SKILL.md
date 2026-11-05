---
name: marlin-extui-backends
description: Use when adding or changing an ExtUI display backend (DGUS, Anycubic, Nextion, Malyan, FTDI EVE, Neptune 3, IA Creality, serial panels) or the ExtUI API in ui_api.h / ui_api.cpp.
---

# Marlin ExtUI Backends

ExtUI (`Marlin/src/lcd/extui/ui_api.{h,cpp}`) is the API used by serial and touch displays. Backends live in subfolders of `Marlin/src/lcd/extui/`. `example/example.cpp` is the template for a new one.

## Rules

1. **Event handlers (`ExtUI::on*()`) have weak, empty defaults in `ui_api.cpp`.** A backend defines only the handlers it uses. Don't add empty `{}` stubs to a backend, and delete them if you see them. A new handler declared in `ui_api.h` gets exactly one weak empty default in the `WEAK_EXTUI` block in `ui_api.cpp`, inside the same `#if` guard as its declaration. No backend needs to change.
2. **API functions (`get*` / `set*`) are not event handlers.** They get one real implementation in `ui_api.cpp`, next to the matching getter, and are never weak or inside a backend. A setter moved into the backends leaves empty stubs that silently do nothing.
3. **Keep the `ExtUI` namespace flat.** Don't use the `using namespace Base` override pattern from `language_*.h`: `ui_api.h` already declares every handler in `ExtUI`, and name hiding breaks overloads such as `onUserConfirmRequired` and `onMeshUpdate`.
4. **Backends keep to their own folder.** Add the folder to `ini/features.ini` (`build_src_filter=+<src/lcd/extui/<folder>>`), or it will be silently excluded and the link fails with every callback undefined.
5. **Wire the new LCD define in `Conditionals-2-LCD.h`** (the `IS_EXTUI` list, the `LCD_IS_SERIAL_HOST` list if it is a serial display, and the "only one LCD" count), and add its checks to `SanityCheck.h`. Copy a similar backend's entries.
6. **Handlers must match the current `ui_api.h` signatures.** Backends ported from forks often use renamed or removed handlers. Diff against the full callback list in `ui_api.h` before building.
7. **After a bulk deletion, tidy up.** Remove runs of blank lines, a blank line directly after `#if` or before `#endif`, orphaned comments, and emptied `#if` blocks. Diff against the base and check that the only `+` lines are the ones you intended.

## A panel that speaks G-code (PanelDue and similar)

Treat it as an ExtUI backend that owns `LCD_SERIAL`, with the whole protocol inside the backend. Add it to `LCD_IS_SERIAL_HOST` (which auto-assigns `LCD_SERIAL_PORT`) rather than routing it through `SERIAL_PORT_2`.

- `LCD_SERIAL` is never a G-code source. The queue reads only `NUM_SERIAL` ports, and `SERIAL_ECHO*` never reaches `LCD_SERIAL`. The backend must call `LCD_SERIAL.begin(LCD_BAUDRATE)` in `onStartup()`, read lines in `onIdle()`, and write replies to `LCD_SERIAL` itself, as Malyan does.
- Answer status queries directly from `onIdle()`, never through the queue. `idle()` keeps running during `G28`, `M109`, and SD prints, so the panel stays connected while Marlin is busy.
- Run other commands by injection. `queue.inject()` overwrites the single `injected_commands` buffer (64 bytes), so keep your own FIFO. Hand over whole lines only when `queue.injected_commands[0] == 0`. Send a line too long for the buffer with `queue.enqueue_one()` and retry if that fails.
- Apply temperature, fan, speed, and flow changes with the ExtUI setters, so they take effect even during a blocking `M109`.
- Buffer replies and drain them with `LCD_SERIAL_TX_BUFFER_FREE()` (`SERIAL_GET_TX_BUFFER_FREE()` on ESP32 and RP2040). A 1 KB reply takes about 180 ms at 57600 baud, which is too long to block `idle()` during a print.
- Keep each JSON or status reply on one line. Clients such as PanelDue discard a partial object when they see a newline.
- If the panel has its own buzzer, add it to the `USE_MARLINUI_BUZZER` list in `Conditionals-5-post.h` and define `MarlinUI::buzz()` in the backend. Without a `BEEPER_PIN`, `HAS_SOUND` is off and `M300` isn't compiled.

## Verify

1. Build every backend the change touches. Find the tests that cover them:

       grep -rliE "elegoo_neptune_3_tft|dwin_lcd_proui|touch_ui_ftdi_eve|extui_example|dgus_lcd_ui" buildroot/tests

   Build one with `buildroot/bin/mftest -t <env> -n <N> -y`.

2. Backends with no CI test need a manual build. Use an STM32 board, not AVR RAMPS, because serial LCDs on RAMPS fail on a pin conflict with Serial Port 3:

       buildroot/bin/restore_configs
       buildroot/bin/opt_set MOTHERBOARD BOARD_BTT_SKR_PRO_V1_1 SERIAL_PORT -1 LCD_SERIAL_PORT 1
       buildroot/bin/opt_enable EEPROM_SETTINGS SDSUPPORT AUTO_BED_LEVELING_BILINEAR FIX_MOUNTED_PROBE Z_SAFE_HOMING PIDTEMPBED
       buildroot/bin/opt_enable MALYAN_LCD            # or the backend's own option(s)
       buildroot/bin/mftest -a -n1 -y

   Some backends need extra options (for example `LCD_BED_TRAMMING` and `BABYSTEPPING` for Anycubic, `CLASSIC_JERK` for DGUS E3S1PRO). Read the first error.

3. Judge each build by its log: count `[SUCCESS]` and `error:`. Don't pass `-s` to `mftest`, which hides the log. Run builds one at a time in a single clone, since parallel builds collide on the `.pio` state.
4. Confirm an override really happened. With the ARM toolchain's `nm`:

       arm-none-eabi-nm -C .pio/build/<env>/firmware.elf | grep -E ' ExtUI::on[A-Za-z]+\('

   `T` is the backend's own definition, and `W` is the weak default. A handler you defined that still shows `W` wasn't compiled, so check the guards around it.

5. Run `buildroot/bin/restore_configs` between builds and at the end. `git status --short` should then show only your source edits.
6. For a serial-panel backend, also run it end to end in the simulator with a scripted panel emulator. Check that replies are valid single lines, that the panel is still answered while Marlin is busy, and that each translated command takes effect.
