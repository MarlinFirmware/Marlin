/*
 * libmaple linker script for RAM builds.
 *
 * A Flash build puts .text, .rodata, and .data/.bss/heap (of course)
 * in SRAM, but offsets the sections by enough space to store the
 * Maple bootloader, which uses low memory.
 */

/*
 * This pulls in the appropriate MEMORY declaration from the right
 * subdirectory of stm32/mem/ (the environment must call ld with the
 * right include directory flags to make this happen). Boards can also
 * use this file to use any of libmaple's memory-related hooks (like
 * where the heap should live).
 */
INCLUDE mem-ram.inc

/* Provide memory region aliases for common.inc */
REGION_ALIAS("REGION_TEXT", ram);
REGION_ALIAS("REGION_DATA", ram);
REGION_ALIAS("REGION_BSS", ram);
REGION_ALIAS("REGION_RODATA", ram);

/* Let common.inc handle the real work. */
INCLUDE common.inc
