/*
 * Linker script for Generic STM32F103RE boards, using the generic bootloader (which takes the lower 8k of memory)
 */
MEMORY
{
	ram (rwx) : ORIGIN = 0x20000000, LENGTH = 64K
	rom (rx) : ORIGIN = 0x08002000, LENGTH = 504K
}


/* Provide memory region aliases for common.inc */
REGION_ALIAS("REGION_TEXT", rom);
REGION_ALIAS("REGION_DATA", ram);
REGION_ALIAS("REGION_BSS", ram);
REGION_ALIAS("REGION_RODATA", rom);

/* Let common.inc handle the real work. */
INCLUDE common.inc
