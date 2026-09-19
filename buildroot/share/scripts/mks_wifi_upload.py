#!/usr/bin/env python3
"""
Send files to the SD card of a printer with an MKS WiFi module.

usage: mks_wifi_upload.py IP FILE [FILE ...]
       py mks_wifi_upload.py IP FILE [FILE ...]      (Windows)

'M587' reports the module's address.

The rate printed for each file covers the whole path, since the module only
answers once Marlin has written everything to the card. It is also given as the
baud rate a plain 8N1 serial link would need to carry it, ten bits per byte, to
compare against the two rates Marlin actually uses: it idles the port at 115200
(WIFI_BAUDRATE) and raises it to 1958400 (WIFI_UPLOAD_BAUDRATE) for a transfer.
Anything over 115200 equivalent means the high-speed path is working.

The equivalent stays well under 1958400 because the card write, not the serial
link, sets the pace. Small files say little - they are mostly fixed setup time.

Names are stored 8.3 on the card, so 'upload_short.gcode' arrives as
'UPLOA~1.GCO'.
"""

import sys, os, time, http.client, urllib.parse

SETTLE = 15     # the module refuses a new transfer until it has finished the last
TTY = sys.stdout.isatty()

def upload(host, path):
    name, size = os.path.basename(path), os.path.getsize(path)

    conn = http.client.HTTPConnection(host, timeout=300)
    conn.putrequest('POST', '/upload?X-Filename=' + urllib.parse.quote(name, safe=''))
    conn.putheader('Content-Type', 'application/octet-stream')
    conn.putheader('Content-Length', str(size))
    conn.endheaders()

    start, sent = time.monotonic(), 0
    with open(path, 'rb') as f:
        for chunk in iter(lambda: f.read(4096), b''):
            conn.sock.sendall(chunk)
            sent += len(chunk)
            if TTY: print(f"\r  {name[:40]} {100 * sent // size:3d}% ", end='', flush=True)

    resp = conn.getresponse()
    resp.read()
    elapsed = time.monotonic() - start
    conn.close()

    if TTY: print('\r' + ' ' * 50 + '\r', end='')
    if resp.status != 200:
        raise IOError(f"HTTP {resp.status} {resp.reason}")
    rate = size / elapsed
    print(f"{name}: {size} bytes in {elapsed:.1f}s = {rate / 1024:.1f} kB/s"
          f" = {round(rate * 10, -3):,.0f} baud equivalent")

def main(host, files):
    failed = 0
    for i, path in enumerate(files):
        if not os.path.isfile(path):
            print(f"{path}: not a file", file=sys.stderr)
            failed += 1
            continue
        for attempt in (1, 2):
            if i or attempt > 1: time.sleep(SETTLE)
            try:
                upload(host, path)
                break
            except (OSError, http.client.HTTPException) as e:
                print(f"{os.path.basename(path)}: failed, {e}", file=sys.stderr)
        else:
            failed += 1
    return 1 if failed else 0

if __name__ == '__main__':
    if len(sys.argv) < 3:
        sys.exit(__doc__.strip())
    sys.exit(main(sys.argv[1], sys.argv[2:]))
