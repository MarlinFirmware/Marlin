#
# random-bin.py
# Set a unique firmware name based on current date and time
#
Import("env")

from datetime import datetime
env_name = str(env["PIOENV"])
env['PROGNAME'] = "firmware_%s_DW7.4.2" % (env_name)
#env['PROGNAME'] .= datetime.now().strftime("-%Y%m%d-%H%M%S")
