#
# pioutil.py
#

# Make sure 'vscode init' is not the current command
def is_pio_build():
    from SCons.Script import DefaultEnvironment
    env = DefaultEnvironment()
    return not env.IsIntegrationDump() and not env.IsCleanTarget()

def get_pio_version():
    from platformio import util
    return util.pioversion_to_intstr()
