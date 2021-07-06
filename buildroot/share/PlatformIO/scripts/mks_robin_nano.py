#
# buildroot/share/PlatformIO/scripts/mks_robin_nano.py
#
import marlin
marlin.prepare_robin("0x08007000", "mks_robin_nano.ld", "Robin_nano.bin")
