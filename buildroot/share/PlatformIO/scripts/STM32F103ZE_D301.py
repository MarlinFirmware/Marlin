
import pioutil
if pioutil.is_pio_build():
    import marlin

    STM32_FLASH_SIZE = 512

    # Relocate firmware from 0x08000000 to 0x08007000
    marlin.relocate_firmware("0x08007000")
    