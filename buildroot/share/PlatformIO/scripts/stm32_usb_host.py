#
# stm32_usb_host.py
#
# USB host (flash drive) support for STM32duino cores that ship ST's USB Host Library
# without building it. With USBHOST defined, build the library's Core and MSC class and use
# Marlin's low-level driver and configuration in Marlin/src/HAL/STM32/usb_host.
#
# Frameworks that provide their own host layer (the stm-flash-drive fork) are left alone.
#
import pioutil
if pioutil.is_pio_build():
    from pathlib import Path
    env = pioutil.env

    flags = ' '.join(env.get('BUILD_FLAGS', [])).split()
    if any(f == '-DUSBHOST' or f.startswith('-DUSBHOST=') for f in flags):
        framework = Path(env.PioPlatform().get_package_dir('framework-arduinoststm32'))
        if not (framework / 'cores' / 'arduino' / 'stm32' / 'usb_host').is_dir():
            lib = framework / 'system' / 'Middlewares' / 'ST' / 'STM32_USB_Host_Library'
            assert lib.is_dir(), f"USB Host Library not found in {framework}"
            env.Append(
                CPPDEFINES=['MARLIN_USBH_CONF'],
                CPPPATH=[
                    str(Path.cwd() / 'Marlin' / 'src' / 'HAL' / 'STM32' / 'usb_host'),
                    str(lib / 'Core' / 'Inc'),
                    str(lib / 'Class' / 'MSC' / 'Inc')
                ]
            )
            env.BuildSources('$BUILD_DIR/USBHostLibrary/Core', str(lib / 'Core' / 'Src'), '+<*> -<usbh_conf_template.c>')
            env.BuildSources('$BUILD_DIR/USBHostLibrary/MSC', str(lib / 'Class' / 'MSC' / 'Src'))
