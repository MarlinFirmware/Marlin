#
# pioutil.py
#

from SCons.Script import DefaultEnvironment
env = DefaultEnvironment()

def is_pio_build():
    """Make sure 'vscode init' is not the current command."""
    if "IsCleanTarget" in dir(env) and env.IsCleanTarget(): return False
    return not env.IsIntegrationDump()

def get_pio_version():
    from platformio import util
    return util.pioversion_to_intstr()
