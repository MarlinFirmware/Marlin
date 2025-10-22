# copy_map.py
from shutil import copyfile
Import("env")

map_path = f".pio/build/{env['PIOENV']}/firmware.map"
copyfile(map_path, "firmware.map")