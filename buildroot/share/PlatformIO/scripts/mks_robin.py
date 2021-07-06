#
# buildroot/share/PlatformIO/scripts/mks_robin.py
#
import marlin
marlin.prepare_robin("0x08007000", "mks_robin.ld", "Robin.bin")
