---
name: marlin-hosts-in-docker
description: Use when testing Marlin's host-facing behavior (serial output, host actions, prompts, file transfer) with OctoPrint or Mainsail running in Docker against the Marlin simulator or a real printer.
---

# Print hosts in Docker against the Marlin simulator

Hosts such as OctoPrint and Mainsail are the main consumers of Marlin's serial output. Running them in Docker against the simulator lets you check `ok` handling, `M155`/`M154` auto-reports, host action commands (`//action:`), prompts, pause/resume/cancel, and G-code streaming without hardware.

The compose files are in `buildroot/share/hosts/`:

- `mainsail.yml`: Mainsail + Moonraker + serial2moon (Mainsail expects Klipper; serial2moon stands in for it). See also skill marlin-mainsail-connector.
- `octoprint.yml`: OctoPrint, with a one-file plugin that lets it open the simulator's TCP socket.

Requires Docker with Compose v2.24 or newer. Run all commands from the repository root.

## 1. Build and run the simulator with a TCP serial socket

    buildroot/bin/use_example_configs Simulator
    buildroot/bin/opt_enable SERIAL_PORT_2                  # serial port 3 is the TCP socket
    # for Mainsail, the recommended host options (see skill marlin-mainsail-connector)
    pio run -e simulator_linux_debug                        # or simulator_windows, simulator_macos_debug (see skill marlin-unit-tests for GCC on macOS)
    .pio/build/simulator_linux_debug/MarlinSimulator --serial-port 8099

Without `SERIAL_PORT_2` the socket accepts connections but Marlin never reads them. The socket serves **one client at a time**: stop one host before starting the other, and close any other client first (a stale connection makes the next one hang silently).

Restore the example config files afterward with `buildroot/bin/restore_configs`.

## 2. Start a host

From the host's point of view the simulator is on the computer running Docker, reachable as `host.docker.internal:8099` (built into Docker Desktop; the compose files add it for Linux Docker Engine).

### OctoPrint

    docker compose -f buildroot/share/hosts/octoprint.yml up -d

Open http://localhost:5050 (not 5000, which macOS uses for AirPlay), finish the setup wizard, then in the Connection panel choose port `socket://host.docker.internal:8099` and baud 115200 (the simulator ignores the baud). `SIM_ADDR` in `.env` changes the address. OctoPrint's data (settings, uploads, logs) lives in the `octoprint` volume; `docker compose ... down -v` erases it and brings the setup wizard back.

OctoPrint can't open a TCP socket as a printer port by itself: its `additionalPorts` setting is a list of file globs, so a `socket://` entry never appears. `octoprint.yml` therefore mounts a small plugin (`marlin_sim_socket.py`) that adds the port through the `octoprint.comm.transport.serial.additional_port_names` and `...serial.factory` hooks and opens it with `serial.serial_for_url`.

To drive it from a script: the API key is in `/octoprint/octoprint/config.yaml` (`api: key:`) inside the container, and the user-level API needs the wizard (or `octoprint user add`) done first.

    docker compose -f buildroot/share/hosts/octoprint.yml exec octoprint \
      awk '/^api/{getline;print $2}' /octoprint/octoprint/config.yaml
    curl -H "X-Api-Key: $KEY" -H 'Content-Type: application/json' \
      -d '{"command":"connect","port":"socket://host.docker.internal:8099","baudrate":115200}' \
      localhost:5050/api/connection
    curl -H "X-Api-Key: $KEY" -H 'Content-Type: application/json' \
      -d '{"commands":["M105","G28"]}' localhost:5050/api/printer/command

The terminal tab in the web UI shows every line sent and received.

### Mainsail

Put these in `buildroot/share/hosts/.env` (or export them):

    S2M_TRANSPORT=tcp

then:

    docker compose -f buildroot/share/hosts/mainsail.yml up -d

Open http://localhost:8088. Check what the bridge reports without a browser:

    curl -s localhost:7125/server/info                                       # klippy_state "ready"
    curl -s -X POST 'localhost:7125/printer/gcode/script?script=G28'
    docker compose -f buildroot/share/hosts/mainsail.yml logs serial2moon   # tx/rx lines

`mainsail.yml` uses serial2moon's `marlin-connector` image from the MarlinFirmware fork, which has the TCP transport and Marlin support that are still going into the upstream project. To test a local serial2moon build, `docker build -t s2m-local .` in its repository and set `S2M_IMAGE=s2m-local` in `.env`.

## Real printers

On a Linux host the USB device can be passed to the container (`devices:` in `octoprint.yml`; `mainsail.yml` does this already and autodetects the port). Docker Desktop on macOS and Windows can't pass USB through: run the host natively there, or use the simulator.

## Pitfalls

- Port conflicts: 5000 is taken by macOS AirPlay, 8099 by whichever host connected first.
- A host that connects first leaves the socket busy: `lsof -iTCP:8099` shows who holds it.
- Both hosts keep their state in Docker volumes; `down -v` is the reset.
- The first OctoPrint start takes about 30 seconds before the page responds.
