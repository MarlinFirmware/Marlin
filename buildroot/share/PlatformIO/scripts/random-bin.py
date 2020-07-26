import os
Import("env")

from datetime import datetime
env['PROGNAME'] = 'firmware-' + datetime.now().strftime("%Y%m%d-%H%M%S")
