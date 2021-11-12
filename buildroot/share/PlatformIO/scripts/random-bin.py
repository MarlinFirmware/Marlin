#
# random-bin.py
# Set a unique firmware name based on current date and time
#
import pioutil
if pioutil.is_pio_build():
	from datetime import datetime
	Import("env")
  env_name = str(env["PIOENV"])
  env['PROGNAME'] = "firmware_%s_DW7.4.3" % (env_name)
	#env['PROGNAME'] = datetime.now().strftime("firmware-%Y%m%d-%H%M%S")
