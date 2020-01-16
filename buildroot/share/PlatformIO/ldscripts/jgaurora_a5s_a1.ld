MEMORY
{
  ram (rwx) : ORIGIN = 0x20000C00, LENGTH = 64K - 3K
  rom (rx)  : ORIGIN = 0x0800A000, LENGTH = 512K - 40K
}

/* Provide memory region aliases for common.inc */
REGION_ALIAS("REGION_TEXT", rom);
REGION_ALIAS("REGION_DATA", ram);
REGION_ALIAS("REGION_BSS", ram);
REGION_ALIAS("REGION_RODATA", rom);

/* Let common.inc handle the real work. */
INCLUDE common.inc
