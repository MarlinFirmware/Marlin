#
# stm32_ldscript_path.py
#
# STM32duino passes the variant linker script to '-Wl,--default-script' as a
# plain flag, so a project path with spaces or parentheses gets split by the
# shell. Pass it as a Literal so SCons quotes it like any other path.
#
import pioutil
if pioutil.is_pio_build():
    from SCons.Subst import Literal
    env = pioutil.env

    for i, flag in enumerate(env['LINKFLAGS']):
        if isinstance(flag, tuple) and flag[0] == '-Wl,--default-script':
            env['LINKFLAGS'][i] = (flag[0], Literal(env.subst(flag[1])))
