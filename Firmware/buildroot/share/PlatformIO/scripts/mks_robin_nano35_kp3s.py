#
# buildroot/share/PlatformIO/scripts/mks_robin_nano35_kp3s.py
# KP3S board needs file name as Robin_nano.bin
#
import marlin
marlin.prepare_robin("0x08007000", "mks_robin_nano.ld", "Robin_nano.bin")
