#
# fastboot_upload.py
# Upload firmware to an ATmega2560 with the mega2560-fastboot bootloader
# (https://github.com/ellensp/mega2560-fastboot).
#
# The bootloader is switched to 1 Mbaud, pages are sent ahead of the replies, and
# the result is verified with a CRC-32 computed on the board. A 230 KB build
# uploads in about 10 seconds instead of about 60.
#
# A board with the stock stk500v2 bootloader gets a plain STK500v2 upload at
# 115200 with a full readback verify instead, as avrdude would do.
#
import time, zlib
from SCons.Script import DefaultEnvironment
env = DefaultEnvironment()

F_CPU     = 16000000
PAGE_SIZE = 256
APP_END   = 0x3E000     # Start of the 8K boot section
FAST_BAUD = 1000000     # Exact at 16 MHz (UBRR 1, double speed)
WINDOW    = 4           # Page writes in flight

CMD_SIGN_ON, CMD_LOAD_ADDRESS = 0x01, 0x06
CMD_ENTER_PROGMODE, CMD_LEAVE_PROGMODE = 0x10, 0x11
CMD_PROGRAM_FLASH, CMD_READ_FLASH = 0x13, 0x14
CMD_FB_INFO, CMD_FB_SET_BAUD, CMD_FB_CRC32 = 0x70, 0x71, 0x72
STATUS_CMD_OK = 0x00

def load_hex(path):
    '''Read an Intel HEX file into a flash image padded to whole pages.'''
    mem, base = {}, 0
    with open(path) as f:
        for line in f:
            line = line.strip()
            if not line.startswith(':'): continue
            b = bytes.fromhex(line[1:])
            addr, typ, data = (b[1] << 8) | b[2], b[3], b[4:4 + b[0]]
            if typ == 0:
                for i, v in enumerate(data): mem[base + addr + i] = v
            elif typ == 2: base = ((data[0] << 8) | data[1]) << 4
            elif typ == 4: base = ((data[0] << 8) | data[1]) << 16
            elif typ == 1: break
    if not mem or min(mem) != 0: raise Exception(f"'{path}' doesn't start at address 0")
    end = (max(mem) + PAGE_SIZE) // PAGE_SIZE * PAGE_SIZE
    if end > APP_END: raise Exception("Firmware overlaps the bootloader")
    img = bytearray(b'\xff' * end)
    for a, v in mem.items(): img[a] = v
    return bytes(img)

class STK500v2:
    def __init__(self, port):
        import serial
        self.s = serial.Serial(port, 115200, timeout=1)
        self.seq = 0

    def reset(self):
        # Pulse DTR to reset the board into the bootloader
        self.s.dtr = False; time.sleep(0.1); self.s.dtr = True; time.sleep(0.15)
        self.s.reset_input_buffer()

    def send(self, body):
        msg = bytes([0x1B, self.seq & 0xFF, len(body) >> 8, len(body) & 0xFF, 0x0E]) + bytes(body)
        cs = 0
        for b in msg: cs ^= b
        self.s.write(msg + bytes([cs]))
        self.seq += 1

    def recv(self, seq=None):
        '''
        Read the next reply. Bytes before a message start are skipped, as is a
        frame with a bad token or checksum, so stray firmware output can't
        misalign the replies. With seq, replies to other messages are skipped.
        '''
        end = time.time() + self.s.timeout
        while time.time() < end:
            if self.s.read(1) != b'\x1B': continue
            h = b'\x1B' + self.s.read(4)
            if len(h) < 5 or h[4] != 0x0E: continue
            n = (h[2] << 8) | h[3]
            if n > 2 * PAGE_SIZE: continue
            r = self.s.read(n + 1)
            if len(r) < n + 1: continue
            cs = 0
            for b in h + r: cs ^= b
            if cs or (seq is not None and h[1] != seq & 0xFF): continue
            return r[:n]
        raise TimeoutError("No reply from the bootloader")

    def cmd(self, body):
        seq = self.seq
        self.send(body)
        return self.recv(seq)

    def sign_on(self, tries=10):
        timeout, self.s.timeout = self.s.timeout, 0.25  # Retry soon. The bootloader answers in a few ms.
        try:
            for _ in range(tries):
                try:
                    if self.cmd([CMD_SIGN_ON])[1] == STATUS_CMD_OK:
                        time.sleep(0.05); self.s.reset_input_buffer()   # Drop late replies to earlier tries
                        return
                except TimeoutError:
                    pass
                time.sleep(0.05); self.s.reset_input_buffer()
        finally:
            self.s.timeout = timeout
        raise Exception("The bootloader isn't responding")

def upload_firmware(port, path):
    img = load_hex(path)
    pages = len(img) // PAGE_SIZE
    t0 = time.time()

    stk = STK500v2(port)
    stk.reset()
    stk.sign_on()

    # The stock bootloader answers unknown commands with STATUS_CMD_FAILED
    info = stk.cmd([CMD_FB_INFO])
    fast = info[1] == STATUS_CMD_OK
    window = 1
    if fast:
        ring = (info[3] << 8) | info[4]
        window = max(1, min(WINDOW, ring // (PAGE_SIZE + 16) - 1))
        try:
            stk.cmd([CMD_FB_SET_BAUD, F_CPU // 8 // FAST_BAUD])    # UBRR + 1
            stk.s.baudrate = FAST_BAUD
            stk.sign_on(3)
        except Exception as e:
            # e.g., a USB serial driver that can't do 1 Mbaud. A reset restarts the bootloader at 115200.
            print(f"No connection at {FAST_BAUD} baud ({e}), continuing at 115200")
            stk.s.baudrate = 115200
            stk.reset()
            stk.sign_on()
        print(f"fastboot v{info[2]} at {stk.s.baudrate} baud, {pages} pages")
    else:
        print(f"Stock bootloader at 115200 baud, {pages} pages")
        print("Tip: the mega2560-fastboot bootloader uploads about 6x faster. See https://github.com/ellensp/mega2560-fastboot")

    stk.cmd([CMD_ENTER_PROGMODE, 200, 100, 25, 32, 0, 0x53, 3, 0xAC, 0x53, 0, 0])
    stk.cmd([CMD_LOAD_ADDRESS, 0x80, 0, 0, 0])
    sent = done = 0
    while done < pages:
        while sent < pages and sent - done < window:
            stk.send([CMD_PROGRAM_FLASH, PAGE_SIZE >> 8, PAGE_SIZE & 0xFF, 0xC1, 10, 0x40, 0x4C, 0x20, 0, 0]
                     + list(img[sent * PAGE_SIZE:(sent + 1) * PAGE_SIZE]))
            sent += 1
        r = stk.recv()
        if r[0] != CMD_PROGRAM_FLASH or r[1] != STATUS_CMD_OK: raise Exception(f"Write failed at page {done}")
        done += 1

    if fast:
        n = len(img)
        stk.s.timeout = 1 + n / 50000   # The board sums over 200 KB/s
        r = stk.cmd([CMD_FB_CRC32, 0, 0, 0, 0, n >> 24, (n >> 16) & 0xFF, (n >> 8) & 0xFF, n & 0xFF])
        stk.s.timeout = 1
        if int.from_bytes(r[2:6], 'big') != zlib.crc32(img): raise Exception("Verify failed (CRC-32 mismatch)")
    else:
        stk.cmd([CMD_LOAD_ADDRESS, 0x80, 0, 0, 0])
        for i in range(pages):
            r = stk.cmd([CMD_READ_FLASH, PAGE_SIZE >> 8, PAGE_SIZE & 0xFF, 0x20])
            if r[2:2 + PAGE_SIZE] != img[i * PAGE_SIZE:(i + 1) * PAGE_SIZE]: raise Exception(f"Verify failed at page {i}")

    stk.cmd([CMD_LEAVE_PROGMODE, 1, 1])     # Start the firmware
    stk.s.close()
    print(f"{len(img)} bytes written and verified in {time.time() - t0:.1f} s")

#-----------------#
# Upload Callback #
#-----------------#
def Upload(source, target, env):
    env.AutodetectUploadPort()
    port = env.subst('$UPLOAD_PORT')
    print(f"Uploading '{source[0]}' on '{port}'")
    upload_firmware(port, str(source[0]))
    return 0

# Attach custom upload callback
env.Replace(UPLOADCMD=Upload)
