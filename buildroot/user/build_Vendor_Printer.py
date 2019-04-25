#!/usr/bin/env python

#
# example printer definition
# for vendor named "Vendor" and printer named "Printer"
# script name convention: "build_<vendor-name>_<printer-name>.py"
#

# use build functions
import build

# select printer hardware environment
# see $root_dir/platformio.ini for available environments
build.environment = "LPC1768"

# activate custom printer hardware variation
# define name convention: "USER_<vendor-name>_<printer-name>"
# see $root_dir/Marlin/user for how this define is used
build.printer_name = "USER_Vendor_Printer"

# invoke build of custom configuration
build.platformio_run()
