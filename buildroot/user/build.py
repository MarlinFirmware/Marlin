#
# shared build functions
#

import os
import subprocess

# user settings
environment = "<board-hardware>"
printer_name = "USER_<vendor-name>_<printer-name>"

# script location
this_dir = os.path.dirname(__file__)
base_dir = os.path.dirname(this_dir) 
root_dir = os.path.dirname(base_dir) 


def platformio_run():
    
    build_dir = os.path.join(root_dir, ".pioenvs", printer_name)
    build_flags = "-D" + printer_name
    
    print("environment=" + environment) 
    print("printer_name=" + printer_name) 
    print("build_dir=" + build_dir) 
    print("build_flags=" + build_flags) 
    
    # use per-printer build location
    os.environ['PLATFORMIO_BUILD_DIR'] = build_dir
    
    # activate configuration override
    os.environ['PLATFORMIO_BUILD_FLAGS'] = build_flags

    # work around #13801 - TODO
    os.environ['PLATFORMIO_EXTRA_SCRIPTS'] = ""
    
    command = ["platformio", "run", "--environment", environment, "--project-dir", root_dir]

    subprocess.run(command)
