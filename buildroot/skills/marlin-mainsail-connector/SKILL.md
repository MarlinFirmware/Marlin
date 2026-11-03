---
name: marlin-mainsail-connector
description: Use when running Mainsail or Fluidd with a Marlin printer or the Marlin simulator through serial2moon, or when testing Marlin's host-facing serial output against them.
---

# Mainsail with Marlin (serial2moon)

Mainsail and Fluidd only talk to Moonraker, and Moonraker expects Klipper behind it. [serial2moon](https://github.com/Lukas-Heiligenbrunner/serial2moon) stands in for Klipper: it speaks Marlin G-code over serial (or TCP) and gives the real Moonraker the Klipper API, so the whole Mainsail/Moonraker stack works with Marlin and no firmware change. It runs on a host computer (a Raspberry Pi, or any machine with Docker), not on the printer.

    Mainsail --> Moonraker --> serial2moon --> USB serial or TCP --> Marlin

## Recommended firmware options

All optional; each one makes the web UI more accurate. From the repository root:

    buildroot/bin/opt_enable EMERGENCY_PARSER ADVANCED_OK AUTO_REPORT_POSITION AUTO_REPORT_REAL_POSITION \
      REPORT_FAN_CHANGE M114_REALTIME M115_GEOMETRY_REPORT CANCEL_OBJECTS SET_PROGRESS_MANUALLY
    buildroot/bin/opt_set BUFSIZE 8

- `EMERGENCY_PARSER` makes `M112` (the web UI's emergency stop) work while a print streams.
- `M115_GEOMETRY_REPORT` lets the bridge read the machine's travel limits at connect. It also reads max feedrate and acceleration from `M503`.
- `ADVANCED_OK` and a larger `BUFSIZE` let the host keep more lines in flight.
- Leave `AUTO_REPORT_FANS` off: it reports fan tachometer RPM, not fan speed, and needs a tachometer pin.
- `HOST_ACTION_COMMANDS` and `HOST_PROMPT_SUPPORT` (on by default) let pause, resume, cancel and filament prompts from the printer's LCD reach the web UI.

## Install for a real printer

- **Raspberry Pi:** flash the image from the serial2moon Releases page, plug in the printer, open `http://<pi-hostname>/`. Updates run from the `HOST_UPDATE_CHECK` / `HOST_UPDATE` macros.
- **Linux host with Docker:** `docker compose -f buildroot/share/hosts/mainsail.yml up -d` (or download that file into an empty folder). Mainsail is on port 8088. Settings (serial port, baud, ports) go in a `.env` file; the comments at the top of the file list them. Docker Desktop on macOS/Windows can't pass USB devices through.

The printer is autodetected over USB at the usual bauds.

## Use it with the Marlin simulator

The simulator exposes a raw G-code socket on TCP port 8099, so the whole stack runs on one computer without hardware.

1. Build the simulator from the Simulator example (see skill marlin-build-test; on macOS install GCC first, see skill marlin-unit-tests):

       buildroot/bin/use_example_configs Simulator
       buildroot/bin/opt_enable SERIAL_PORT_2     # serial port 3 = the TCP socket
       # ...plus the recommended options above
       pio run -e simulator_linux_debug           # or simulator_windows, simulator_macos_debug

   Without `SERIAL_PORT_2` the socket accepts connections but Marlin never reads them.

2. Run it: `.pio/build/<env>/MarlinSimulator --serial-port 8099`. The socket takes one client at a time, so close other connections to port 8099 first.
3. Start the stack against it (see skill marlin-hosts-in-docker for details):

       echo S2M_TRANSPORT=tcp > buildroot/share/hosts/.env
       docker compose -f buildroot/share/hosts/mainsail.yml up -d

4. Open `http://localhost:8088`. The console, temperatures, moves, uploads and prints all run against the simulator.

The TCP transport needs a serial2moon newer than v0.1.0; `mainsail.yml` uses the fork image that has it.

## Check it from the command line

Moonraker's API shows what the bridge reports, without a browser:

    curl -s localhost:7125/server/info                    # klippy_state "ready", warnings
    curl -s -X POST 'localhost:7125/printer/gcode/script?script=G28'
    curl -s 'localhost:7125/printer/objects/query?toolhead&configfile'

The serial traffic is in the bridge's log: `docker compose logs serial2moon`, which shows each line sent (`serial2moon::tx`) and received (`serial2moon::rx`).

## Pitfalls

- PlatformIO caches the MarlinSimUI `cutting-edge.zip` download and keeps using it, so a new build can get an old simulator. To get the current one, delete `.pio/libdeps/<env>` and the cached zip in `~/.platformio/.cache/downloads/` (its name is the SHA-1 of the URL: `printf %s <url> | shasum`), then rebuild. Don't use `pio pkg update` for this: it reports "up-to-date" and rewrites `platformio.ini`.

- Mainsail labels the backend "Klipper" in its connection screens. That text is Mainsail's; serial2moon is what's behind it.
- Klipper-only features (input shaper calibration, `BED_MESH_CALIBRATE`, macros) are accepted but don't map to Marlin. Use the Marlin G-codes in the console.
- `homed_axes` in the web UI comes from the `G28` commands the bridge sent. Marlin doesn't report its homed state yet.
