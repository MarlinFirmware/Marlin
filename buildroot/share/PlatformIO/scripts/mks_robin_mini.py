#
# buildroot/share/PlatformIO/scripts/mks_robin_mini.py
#
import marlin
marlin.prepare_robin("0x08005000", "mks_robin_mini.ld", "Robin_mini.bin")
