/*
;*******************************************************************************
; Copyright (C) 2016, Huada Semiconductor Co.,Ltd All rights reserved.
;
; This software is owned and published by:
; Huada Semiconductor Co.,Ltd ("HDSC").
;
; BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND
; BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
;
; This software contains source code for use with HDSC
; components. This software is licensed by HDSC to be adapted only
; for use in systems utilizing HDSC components. HDSC shall not be
; responsible for misuse or illegal use of this software for devices not
; supported herein. HDSC is providing this software "AS IS" and will
; not be responsible for issues arising from incorrect user implementation
; of the software.
;
; Disclaimer:
; HDSC MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE,
; REGARDING THE SOFTWARE (INCLUDING ANY ACOOMPANYING WRITTEN MATERIALS),
; ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED USE, INCLUDING,
; WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, THE IMPLIED
; WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE OR USE, AND THE IMPLIED
; WARRANTY OF NONINFRINGEMENT.
; HDSC SHALL HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT,
; NEGLIGENCE OR OTHERWISE) FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT
; LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION,
; LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY LOSS) ARISING FROM USE OR
; INABILITY TO USE THE SOFTWARE, INCLUDING, WITHOUT LIMITATION, ANY DIRECT,
; INDIRECT, INCIDENTAL, SPECIAL OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA,
; SAVINGS OR PROFITS,
; EVEN IF Disclaimer HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
; YOU ASSUME ALL RESPONSIBILITIES FOR SELECTION OF THE SOFTWARE TO ACHIEVE YOUR
; INTENDED RESULTS, AND FOR THE INSTALLATION OF, USE OF, AND RESULTS OBTAINED
; FROM, THE SOFTWARE.
;
; This software may be replicated in part or whole for the licensed use,
; with the restriction that this Disclaimer and Copyright notice must be
; included with each copy of this software, whether used in part or whole,
; at all times.
;/
*/
/*****************************************************************************/
/*  File        hc32f46x_flash.ld                                            */
/*  Abstract    Linker script for HC32F46x Device with                       */
/*              512KByte FLASH, 192KByte RAM                                 */
/*  Version     V1.0                                                         */
/*  Date        2019-03-13                                                   */
/*****************************************************************************/

/* Use contiguous memory regions for simple. */
MEMORY
{
    FLASH_ISR   (rx): ORIGIN = 0x0000C000, LENGTH = 1024
    FLASH_ICG   (rx): ORIGIN = 0x0000C400, LENGTH = 32
    FLASH       (rx): ORIGIN = 0x0000C420, LENGTH = 208K
    RAM        (rwx): ORIGIN = 0x1FFF8000, LENGTH = 188K
    RAM_RET    (rwx): ORIGIN = 0x200F0000, LENGTH = 4K
}

ENTRY(Reset_Handler)

SECTIONS
{
    .vectors :
    {
        KEEP(*(.vectors))
    } >FLASH_ISR

    .icg_sec :
    {
        KEEP(*(.icg_sec))
    } >FLASH_ICG

    .text :
    {
        *(.text)
        *(.text*)
        *(.glue_7)
        *(.glue_7t)
        *(.eh_frame)

        KEEP(*(.init))
        KEEP(*(.fini))
    } >FLASH

    .rodata :
    {
        . = ALIGN(4);
        *(.rodata)
        *(.rodata*)
        . = ALIGN(4);
    } >FLASH

    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } >FLASH

    __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } >FLASH
    __exidx_end = .;

    .preinit_array :
    {
        /* preinit data */
		PROVIDE_HIDDEN (__preinit_array_start = .);
		KEEP(*(.preinit_array))
		PROVIDE_HIDDEN (__preinit_array_end = .);
    } >FLASH

    .init_array :
    {
        /* init data */
		PROVIDE_HIDDEN (__init_array_start = .);
		KEEP(*(SORT(.init_array.*)))
		KEEP(*(.init_array))
		PROVIDE_HIDDEN (__init_array_end = .);
    } >FLASH

    .fini_array :
    {
        /* finit data */
		PROVIDE_HIDDEN (__fini_array_start = .);
		KEEP(*(SORT(.fini_array.*)))
		KEEP(*(.fini_array))
		PROVIDE_HIDDEN (__fini_array_end = .);
    } >FLASH

    /*__etext = ALIGN(4);
    .data : AT (__etext)
    {
        . = ALIGN(4);
        __data_start__ = .;
        *(vtable)
        *(.data)
        *(.data*)
        . = ALIGN(4);
        __data_end__ = .;
    } >RAM*/

    __etext = LOADADDR(.data);
    .data :
    {
        . = ALIGN(4);
        __data_start__ = .;
        *(vtable)
        *(.data)
        *(.data*)
        . = ALIGN(4);
        __data_end__ = .;
    } >RAM AT> FLASH

    __ram_ret_data_start = LOADADDR(.ram_ret_data);
    .ram_ret_data :
    {
        . = ALIGN(4);
        __data_start_ram_ret__ = .;
        *(.ram_ret_data)
        *(.ram_ret_data*)
        . = ALIGN(4);
        __data_end_ram_ret__ = .;
    } >RAM_RET AT> FLASH

    .bss :
    {
        . = ALIGN(4);
        __bss_start__ = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
    } >RAM

    .ram_ret_bss :
    {
        . = ALIGN(4);
        __bss_start_ram_ret__ = .;
        *(.ram_ret_bss)
        *(.ram_ret_bss*)
        . = ALIGN(4);
        __bss_end_ram_ret__ = .;
    } >RAM_RET

    .heap (COPY) :
    {
        __end__ = .;
        PROVIDE(end = .);
        *(.heap*)
        __HeapLimit = .;
    } >RAM

    .stack_dummy (COPY) :
    {
        *(.stack*)
    } >RAM

    __StackTop = ORIGIN(RAM) + LENGTH(RAM);
    __StackLimit = __StackTop - SIZEOF(.stack_dummy);
    PROVIDE(_stack = __StackTop);

    ASSERT(__StackLimit >= __HeapLimit, "region RAM overflowed with stack")
}
