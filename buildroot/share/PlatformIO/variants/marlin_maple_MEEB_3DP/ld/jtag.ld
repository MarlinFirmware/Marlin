/*
 * libmaple linker script for "JTAG" builds.
 *
 * A "JTAG" build puts .text (and .rodata) in Flash, and
 * .data/.bss/heap (of course) in SRAM, but links starting at the
 * Flash and SRAM starting addresses (0x08000000 and 0x20000000
 * respectively). This will wipe out a Maple bootloader if there's one
 * on the board, so only use this if you know what you're doing.
 *
 * Of course, a "JTAG" build is perfectly usable for upload over SWD,
 * the system memory bootloader, etc. The name is just a historical
 * artifact.
 */

/*
 * This pulls in the appropriate MEMORY declaration from the right
 * subdirectory of stm32/mem/ (the environment must call ld with the
 * right include directory flags to make this happen). Boards can also
 * use this file to use any of libmaple's memory-related hooks (like
 * where the heap should live).
 */
INCLUDE mem-jtag.inc

/* Provide memory region aliases for common.inc */
REGION_ALIAS("REGION_TEXT", rom);
REGION_ALIAS("REGION_DATA", ram);
REGION_ALIAS("REGION_BSS", ram);
REGION_ALIAS("REGION_RODATA", rom);

/* Let common.inc handle the real work. */
INCLUDE common.inc
