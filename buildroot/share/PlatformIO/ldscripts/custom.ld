/**
 * custom.ld - Linker script using custom environment defines
 *
 * LD_MAX_SIZE seems to be defined by the base platform build.
 *
 * Provided by offset_and_rename.py :
 *   LD_MAX_DATA_SIZE <= board_upload.maximum_ram_size
 *   LD_FLASH_OFFSET  <= board_build.offset -- also used for marlin.relocate_vtab()
 */

MEMORY
{
  ram (rwx) : ORIGIN = 0x20000000, LENGTH = LD_MAX_DATA_SIZE
  rom (rx)  : ORIGIN = 0x08000000 + LD_FLASH_OFFSET, LENGTH = LD_MAX_SIZE - LD_FLASH_OFFSET
}

/* Provide memory region aliases for common.inc */
REGION_ALIAS("REGION_TEXT", rom);
REGION_ALIAS("REGION_DATA", ram);
REGION_ALIAS("REGION_BSS", ram);
REGION_ALIAS("REGION_RODATA", rom);

/* Let common.inc handle the real work. */
INCLUDE common.inc
