#
# buildroot/share/PlatformIO/scripts/mks_robin_lite.py
#
import marlin
marlin.prepare_robin("0x08005000", "mks_robin_lite.ld", "mksLite.bin")
