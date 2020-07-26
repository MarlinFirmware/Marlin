Import("env")

from datetime import datetime

env['PROGNAME'] = datetime.now().strftime("firmware-%Y%m%d-%H%M%S")
