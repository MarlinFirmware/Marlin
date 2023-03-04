/*
 * HC32F460 SOC with 256kb flash (HC32F460xCxx) with bootloader for sd firmware flashing.
 * the flash contains a bootloader from 0x0000 to 0xC000
 */

FLASH_START = 0x0000C000;
FLASH_SIZE = 256K;

/*
 * all HC32F46x SOCs have 192kb of RAM, of which 4kb are retained in power-down mode
 */
RAM_START = 0x1FFF8000;
RAM_SIZE = 188K;
RAM_RET_START = 0x200F0000;
RAM_RET_SIZE = 4K;

INCLUDE common.ld

