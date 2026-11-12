#
# katapult_upload.py
# Upload firmware over USB serial via the Katapult bootloader (https://github.com/Arksine/katapult)
# without pressing any BOOT / RESET buttons.
#
# If the board is running Marlin with BOOTLOADER_KATAPULT, 'M997' is sent to reboot it
# into Katapult. If the board is already in Katapult mode (e.g., a first install or a
# previous incomplete flash) that step is skipped. Katapult's own 'flashtool.py' then
# uploads and SHA-verifies the firmware and jumps to the application.
#
# flashtool.py is fetched once from the Katapult repository (pinned commit, checksum
# verified) and cached in the PlatformIO build folder. To use a local copy instead
# (e.g., from an existing Katapult checkout) set the KATAPULT_FLASHTOOL environment
# variable to its path.
#
# Note: flashtool.py requires a POSIX system (Linux / macOS / WSL).
#
import os, time, hashlib, subprocess
from SCons.Script import DefaultEnvironment
env = DefaultEnvironment()

# Katapult USB CDC identity (OpenMoko VID)
KATAPULT_USB_VID = 0x1D50
KATAPULT_USB_PID = 0x6177

# STMicroelectronics VID, as used by the STM32 core's USB CDC
STM32_USB_CDC_VID = 0x0483

# Pinned flashtool.py from the Katapult repository (GPLv3, same license as Marlin)
FLASHTOOL_COMMIT = '509acf4b90260851e0199f1ac7ca7b6ff5dc188b'
FLASHTOOL_URL    = f'https://raw.githubusercontent.com/Arksine/katapult/{FLASHTOOL_COMMIT}/scripts/flashtool.py'
FLASHTOOL_SHA256 = '918de11f9310d434e32aeb0020a767f7900e9ba703a7f26f44874d8de1d0c126'

#-----------------#
# Upload Callback #
#-----------------#
def Upload(source, target, env):
    import serial
    from serial.tools import list_ports

    #----------------#
    # Port functions #
    #----------------#
    def _KatapultPorts():
        return [ p.device for p in list_ports.comports() if p.vid == KATAPULT_USB_VID and p.pid == KATAPULT_USB_PID ]

    def _GetUploadPort(env):
        # An explicitly configured 'upload_port' always wins
        portName = env.subst('$UPLOAD_PORT')
        if not portName:
            # Autodetection can land on an unrelated port (e.g., a Bluetooth device), so
            # look for a USB CDC device first and only fall back to PlatformIO's guess
            usb_ports = [ p for p in list_ports.comports() if p.vid is not None ]
            stm_ports = [ p.device for p in usb_ports if p.vid == STM32_USB_CDC_VID ]
            if len(stm_ports) == 1:
                portName = stm_ports[0]
            elif len(usb_ports) == 1:
                portName = usb_ports[0].device
            else:
                env.AutodetectUploadPort(env)
                portName = env.subst('$UPLOAD_PORT')
                if portName and portName not in [ p.device for p in usb_ports ]:
                    raise Exception(
                        f"Autodetected '{portName}', which is not a USB serial device.\n"
                        "Set 'upload_port' to the board's port and try again."
                    )
        if not portName:
            raise Exception('Error detecting the upload port.')
        return portName

    def _RequestBootloader(portName):
        # Send M997 so Marlin (with BOOTLOADER_KATAPULT) reboots into Katapult
        print(f"Sending M997 to '{portName}'...")
        upload_speed = env['UPLOAD_SPEED'] if 'UPLOAD_SPEED' in env else 250000
        port = serial.Serial(portName, baudrate=upload_speed, write_timeout=2, timeout=0.1)
        try:
            port.reset_input_buffer()
            port.write(b'\nM997\n')
            port.flush()
        finally:
            try:
                port.close()
            except serial.SerialException:
                pass  # The board may reset before the port is released

    def _WaitForKatapult(timeout=30):
        print('Waiting for Katapult to enumerate...')
        deadline = time.time() + timeout
        while time.time() < deadline:
            ports = _KatapultPorts()
            if ports: return ports[0]
            time.sleep(0.5)
        raise Exception(
            'Timed out waiting for the Katapult bootloader device (USB ID '
            f'{KATAPULT_USB_VID:04x}:{KATAPULT_USB_PID:04x}).\n'
            'Make sure Katapult is installed on the board and the running Marlin '
            'was built with BOOTLOADER_KATAPULT.'
        )

    #---------------------#
    # flashtool functions #
    #---------------------#
    def _GetFlashtool():
        # A user-provided flashtool (e.g., from a local Katapult checkout)
        localtool = os.environ.get('KATAPULT_FLASHTOOL')
        if localtool:
            if not os.path.isfile(localtool):
                raise Exception(f"KATAPULT_FLASHTOOL is set but '{localtool}' was not found")
            return localtool

        # Otherwise download once and cache in the build folder
        cached = os.path.join(env.subst('$PROJECT_BUILD_DIR'), 'katapult', f'flashtool-{FLASHTOOL_COMMIT[:8]}.py')
        if not os.path.isfile(cached):
            print(f'Downloading Katapult flashtool ({FLASHTOOL_URL})...')
            import requests
            response = requests.get(FLASHTOOL_URL, timeout=30)
            response.raise_for_status()
            data = response.content
            checksum = hashlib.sha256(data).hexdigest()
            if checksum != FLASHTOOL_SHA256:
                raise Exception(f'flashtool.py checksum mismatch (expected {FLASHTOOL_SHA256}, got {checksum})')
            os.makedirs(os.path.dirname(cached), exist_ok=True)
            with open(cached, 'wb') as f: f.write(data)
        return cached

    #---------------------#
    # Callback Entrypoint #
    #---------------------#
    if os.name == 'nt':
        raise Exception("Katapult's flashtool.py requires a POSIX system. On Windows, upload from WSL or use another upload method.")

    upload_firmware_source_path = os.path.join(env['PROJECT_BUILD_DIR'], env['PIOENV'], f"{env['PROGNAME']}.bin") if 'PROGNAME' in env else str(source[0])
    if not os.path.isfile(upload_firmware_source_path):
        raise Exception(f"Firmware file '{upload_firmware_source_path}' not found")

    flashtool = _GetFlashtool()

    # Skip the M997 handshake if the board is already sitting in Katapult
    katapult_ports = _KatapultPorts()
    if katapult_ports:
        katapult_port = katapult_ports[0]
        print(f"Katapult bootloader already active on '{katapult_port}'")
    else:
        _RequestBootloader(_GetUploadPort(env))
        katapult_port = _WaitForKatapult()

    print(f"Uploading '{os.path.basename(upload_firmware_source_path)}' via Katapult on '{katapult_port}'")
    result = subprocess.call([ env.subst('$PYTHONEXE'), flashtool, '-d', katapult_port, '-f', upload_firmware_source_path ])
    if result != 0:
        raise Exception('Katapult flash failed. The board may still be in Katapult mode; fix the issue and upload again.')

    print('Firmware update completed')
    return 0

# Attach custom upload callback
env.Replace(UPLOADCMD=Upload)
