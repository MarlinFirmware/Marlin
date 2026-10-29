# Print hosts in Docker

Compose files for running a print host against a Marlin printer or the Marlin simulator:

| File | Host | Web UI |
| --- | --- | --- |
| `mainsail.yml` | Mainsail + Moonraker + [serial2moon](https://github.com/MarlinFirmware/serial2moon) | http://localhost:8088 |
| `octoprint.yml` | [OctoPrint](https://octoprint.org) | http://localhost:5050 |

    docker compose -f buildroot/share/hosts/mainsail.yml up -d      # or octoprint.yml
    docker compose -f buildroot/share/hosts/mainsail.yml down       # stop (add -v to erase its data)

Both need Docker with Compose v2.24 or newer. The settings are described at the top of each file and go in a `.env` file next to it. To run them against the simulator, see the [marlin-hosts-in-docker](../../skills/marlin-hosts-in-docker/SKILL.md) skill. Only one host can use the simulator's G-code socket at a time.

For a real printer, a USB device can only be passed through to a container on a Linux host.
