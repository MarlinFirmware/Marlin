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
/*  Startup for GCC                                                          */
/*  Version     V1.0                                                         */
/*  Date        2019-03-13                                                   */
/*  Target-mcu  HC32F46x                                                     */
/*****************************************************************************/

/*
;//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
*/

                .syntax     unified
                .arch       armv7e-m
                .cpu        cortex-m4
                .fpu        softvfp
                .thumb

/*
;<h> Stack Configuration
;  <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
;</h>
*/
                .equ        Stack_Size, 0x00000400

                .section    .stack
                .align      3
                .globl      __StackTop
                .globl      __StackLimit
__StackLimit:
                .space      Stack_Size
                .size       __StackLimit, . - __StackLimit
__StackTop:
                .size       __StackTop, . - __StackTop


/*
;<h> Heap Configuration
;  <o> Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
;</h>
*/
                .equ        Heap_Size, 0x00000C00

                .if         Heap_Size != 0                     /* Heap is provided */
                .section    .heap
                .align      3
                .globl      __HeapBase
                .globl      __HeapLimit
__HeapBase:
                .space      Heap_Size
                .size       __HeapBase, . - __HeapBase
__HeapLimit:
                .size       __HeapLimit, . - __HeapLimit
                .endif

/*
;<h> Interrupt vector table start.
*/
                .section    .vectors, "a", %progbits
                .align      2
                .type       __Vectors, %object
                .globl      __Vectors
                .globl      __Vectors_End
                .globl      __Vectors_Size
__Vectors:
                .long       __StackTop                         /*     Top of Stack */
                .long       Reset_Handler                      /*     Reset Handler */
                .long       NMI_Handler                        /* -14 NMI Handler */
                .long       HardFault_Handler                  /* -13 Hard Fault Handler */
                .long       MemManage_Handler                  /* -12 MPU Fault Handler */
                .long       BusFault_Handler                   /* -11 Bus Fault Handler */
                .long       UsageFault_Handler                 /* -10 Usage Fault Handler */
                .long       0                                  /*     Reserved */
                .long       0                                  /*     Reserved */
                .long       0                                  /*     Reserved */
                .long       0                                  /*     Reserved */
                .long       SVC_Handler                        /*  -5 SVCall Handler */
                .long       DebugMon_Handler                   /*  -4 Debug Monitor Handler */
                .long       0                                  /*     Reserved */
                .long       PendSV_Handler                     /*  -2 PendSV Handler */
                .long       SysTick_Handler                    /*  -1 SysTick Handler */

                /* Interrupts */
                .long       IRQ000_Handler
                .long       IRQ001_Handler
                .long       IRQ002_Handler
                .long       IRQ003_Handler
                .long       IRQ004_Handler
                .long       IRQ005_Handler
                .long       IRQ006_Handler
                .long       IRQ007_Handler
                .long       IRQ008_Handler
                .long       IRQ009_Handler
                .long       IRQ010_Handler
                .long       IRQ011_Handler
                .long       IRQ012_Handler
                .long       IRQ013_Handler
                .long       IRQ014_Handler
                .long       IRQ015_Handler
                .long       IRQ016_Handler
                .long       IRQ017_Handler
                .long       IRQ018_Handler
                .long       IRQ019_Handler
                .long       IRQ020_Handler
                .long       IRQ021_Handler
                .long       IRQ022_Handler
                .long       IRQ023_Handler
                .long       IRQ024_Handler
                .long       IRQ025_Handler
                .long       IRQ026_Handler
                .long       IRQ027_Handler
                .long       IRQ028_Handler
                .long       IRQ029_Handler
                .long       IRQ030_Handler
                .long       IRQ031_Handler
                .long       IRQ032_Handler
                .long       IRQ033_Handler
                .long       IRQ034_Handler
                .long       IRQ035_Handler
                .long       IRQ036_Handler
                .long       IRQ037_Handler
                .long       IRQ038_Handler
                .long       IRQ039_Handler
                .long       IRQ040_Handler
                .long       IRQ041_Handler
                .long       IRQ042_Handler
                .long       IRQ043_Handler
                .long       IRQ044_Handler
                .long       IRQ045_Handler
                .long       IRQ046_Handler
                .long       IRQ047_Handler
                .long       IRQ048_Handler
                .long       IRQ049_Handler
                .long       IRQ050_Handler
                .long       IRQ051_Handler
                .long       IRQ052_Handler
                .long       IRQ053_Handler
                .long       IRQ054_Handler
                .long       IRQ055_Handler
                .long       IRQ056_Handler
                .long       IRQ057_Handler
                .long       IRQ058_Handler
                .long       IRQ059_Handler
                .long       IRQ060_Handler
                .long       IRQ061_Handler
                .long       IRQ062_Handler
                .long       IRQ063_Handler
                .long       IRQ064_Handler
                .long       IRQ065_Handler
                .long       IRQ066_Handler
                .long       IRQ067_Handler
                .long       IRQ068_Handler
                .long       IRQ069_Handler
                .long       IRQ070_Handler
                .long       IRQ071_Handler
                .long       IRQ072_Handler
                .long       IRQ073_Handler
                .long       IRQ074_Handler
                .long       IRQ075_Handler
                .long       IRQ076_Handler
                .long       IRQ077_Handler
                .long       IRQ078_Handler
                .long       IRQ079_Handler
                .long       IRQ080_Handler
                .long       IRQ081_Handler
                .long       IRQ082_Handler
                .long       IRQ083_Handler
                .long       IRQ084_Handler
                .long       IRQ085_Handler
                .long       IRQ086_Handler
                .long       IRQ087_Handler
                .long       IRQ088_Handler
                .long       IRQ089_Handler
                .long       IRQ090_Handler
                .long       IRQ091_Handler
                .long       IRQ092_Handler
                .long       IRQ093_Handler
                .long       IRQ094_Handler
                .long       IRQ095_Handler
                .long       IRQ096_Handler
                .long       IRQ097_Handler
                .long       IRQ098_Handler
                .long       IRQ099_Handler
                .long       IRQ100_Handler
                .long       IRQ101_Handler
                .long       IRQ102_Handler
                .long       IRQ103_Handler
                .long       IRQ104_Handler
                .long       IRQ105_Handler
                .long       IRQ106_Handler
                .long       IRQ107_Handler
                .long       IRQ108_Handler
                .long       IRQ109_Handler
                .long       IRQ110_Handler
                .long       IRQ111_Handler
                .long       IRQ112_Handler
                .long       IRQ113_Handler
                .long       IRQ114_Handler
                .long       IRQ115_Handler
                .long       IRQ116_Handler
                .long       IRQ117_Handler
                .long       IRQ118_Handler
                .long       IRQ119_Handler
                .long       IRQ120_Handler
                .long       IRQ121_Handler
                .long       IRQ122_Handler
                .long       IRQ123_Handler
                .long       IRQ124_Handler
                .long       IRQ125_Handler
                .long       IRQ126_Handler
                .long       IRQ127_Handler
                .long       IRQ128_Handler
                .long       IRQ129_Handler
                .long       IRQ130_Handler
                .long       IRQ131_Handler
                .long       IRQ132_Handler
                .long       IRQ133_Handler
                .long       IRQ134_Handler
                .long       IRQ135_Handler
                .long       IRQ136_Handler
                .long       IRQ137_Handler
                .long       IRQ138_Handler
                .long       IRQ139_Handler
                .long       IRQ140_Handler
                .long       IRQ141_Handler
                .long       IRQ142_Handler
                .long       IRQ143_Handler

__Vectors_End:
                .equ        __Vectors_Size, __Vectors_End - __Vectors
                .size       __Vectors, . - __Vectors
/*
;<h> Interrupt vector table end.
*/

/*
;<h> Reset handler start.
*/
                .section    .text.Reset_Handler
                .align      2
                .weak       Reset_Handler
                .type       Reset_Handler, %function
                .globl      Reset_Handler
Reset_Handler:
                /* Set stack top pointer. */
                ldr         sp, =__StackTop
/* Single section scheme.
 *
 * The ranges of copy from/to are specified by following symbols
 *   __etext: LMA of start of the section to copy from. Usually end of text
 *   __data_start__: VMA of start of the section to copy to
 *   __data_end__: VMA of end of the section to copy to
 *
 * All addresses must be aligned to 4 bytes boundary.
 */
                /* Copy data from read only memory to RAM. */
CopyData:
                ldr         r1, =__etext
                ldr         r2, =__data_start__
                ldr         r3, =__data_end__
CopyLoop:
                cmp         r2, r3
                ittt        lt
                ldrlt       r0, [r1], #4
                strlt       r0, [r2], #4
                blt         CopyLoop

CopyData1:
                ldr         r1, =__ram_ret_data_start
                ldr         r2, =__data_start_ram_ret__
                ldr         r3, =__data_end_ram_ret__
CopyLoop1:
                cmp         r2, r3
                ittt        lt
                ldrlt       r0, [r1], #4
                strlt       r0, [r2], #4
                blt         CopyLoop1

/* This part of work usually is done in C library startup code.
 * Otherwise, define this macro to enable it in this startup.
 *
 * There are two schemes too.
 * One can clear multiple BSS sections. Another can only clear one section.
 * The former is more size expensive than the latter.
 *
 * Define macro __STARTUP_CLEAR_BSS_MULTIPLE to choose the former.
 * Otherwise define macro __STARTUP_CLEAR_BSS to choose the later.
 */
/* Single BSS section scheme.
 *
 * The BSS section is specified by following symbols
 *   __bss_start__: start of the BSS section.
 *   __bss_end__: end of the BSS section.
 *
 * Both addresses must be aligned to 4 bytes boundary.
 */
                /* Clear BSS section. */
ClearBss:
                ldr         r1, =__bss_start__
                ldr         r2, =__bss_end__

                movs        r0, 0
ClearLoop:
                cmp         r1, r2
                itt         lt
                strlt       r0, [r1], #4
                blt         ClearLoop

ClearBss1:
                ldr         r1, =__bss_start_ram_ret__
                ldr         r2, =__bss_end_ram_ret__

                movs        r0, 0
ClearLoop1:
                cmp         r1, r2
                itt         lt
                strlt       r0, [r1], #4
                blt         ClearLoop1

                /* Call the clock system initialization function. */
                bl          SystemInit
                bl          __libc_init_array
                /* Call the application's entry point. */
                bl          main
                bx          lr
                .size       Reset_Handler, . - Reset_Handler
/*
;<h> Reset handler end.
*/

/*
;<h> Default handler start.
*/
                .section    .text.Default_Handler, "ax", %progbits
                .align      2
Default_Handler:
                b           .
                .size       Default_Handler, . - Default_Handler
/*
;<h> Default handler end.
*/

/* Macro to define default exception/interrupt handlers.
 * Default handler are weak symbols with an endless loop.
 * They can be overwritten by real handlers.
 */
                .macro      Set_Default_Handler  Handler_Name
                .weak       \Handler_Name
                .set        \Handler_Name, Default_Handler
                .endm

/* Default exception/interrupt handler */

                Set_Default_Handler    NMI_Handler
                Set_Default_Handler    HardFault_Handler
                Set_Default_Handler    MemManage_Handler
                Set_Default_Handler    BusFault_Handler
                Set_Default_Handler    UsageFault_Handler
                Set_Default_Handler    SVC_Handler
                Set_Default_Handler    DebugMon_Handler
                Set_Default_Handler    PendSV_Handler
                Set_Default_Handler    SysTick_Handler

                Set_Default_Handler    IRQ000_Handler
                Set_Default_Handler    IRQ001_Handler
                Set_Default_Handler    IRQ002_Handler
                Set_Default_Handler    IRQ003_Handler
                Set_Default_Handler    IRQ004_Handler
                Set_Default_Handler    IRQ005_Handler
                Set_Default_Handler    IRQ006_Handler
                Set_Default_Handler    IRQ007_Handler
                Set_Default_Handler    IRQ008_Handler
                Set_Default_Handler    IRQ009_Handler
                Set_Default_Handler    IRQ010_Handler
                Set_Default_Handler    IRQ011_Handler
                Set_Default_Handler    IRQ012_Handler
                Set_Default_Handler    IRQ013_Handler
                Set_Default_Handler    IRQ014_Handler
                Set_Default_Handler    IRQ015_Handler
                Set_Default_Handler    IRQ016_Handler
                Set_Default_Handler    IRQ017_Handler
                Set_Default_Handler    IRQ018_Handler
                Set_Default_Handler    IRQ019_Handler
                Set_Default_Handler    IRQ020_Handler
                Set_Default_Handler    IRQ021_Handler
                Set_Default_Handler    IRQ022_Handler
                Set_Default_Handler    IRQ023_Handler
                Set_Default_Handler    IRQ024_Handler
                Set_Default_Handler    IRQ025_Handler
                Set_Default_Handler    IRQ026_Handler
                Set_Default_Handler    IRQ027_Handler
                Set_Default_Handler    IRQ028_Handler
                Set_Default_Handler    IRQ029_Handler
                Set_Default_Handler    IRQ030_Handler
                Set_Default_Handler    IRQ031_Handler
                Set_Default_Handler    IRQ032_Handler
                Set_Default_Handler    IRQ033_Handler
                Set_Default_Handler    IRQ034_Handler
                Set_Default_Handler    IRQ035_Handler
                Set_Default_Handler    IRQ036_Handler
                Set_Default_Handler    IRQ037_Handler
                Set_Default_Handler    IRQ038_Handler
                Set_Default_Handler    IRQ039_Handler
                Set_Default_Handler    IRQ040_Handler
                Set_Default_Handler    IRQ041_Handler
                Set_Default_Handler    IRQ042_Handler
                Set_Default_Handler    IRQ043_Handler
                Set_Default_Handler    IRQ044_Handler
                Set_Default_Handler    IRQ045_Handler
                Set_Default_Handler    IRQ046_Handler
                Set_Default_Handler    IRQ047_Handler
                Set_Default_Handler    IRQ048_Handler
                Set_Default_Handler    IRQ049_Handler
                Set_Default_Handler    IRQ050_Handler
                Set_Default_Handler    IRQ051_Handler
                Set_Default_Handler    IRQ052_Handler
                Set_Default_Handler    IRQ053_Handler
                Set_Default_Handler    IRQ054_Handler
                Set_Default_Handler    IRQ055_Handler
                Set_Default_Handler    IRQ056_Handler
                Set_Default_Handler    IRQ057_Handler
                Set_Default_Handler    IRQ058_Handler
                Set_Default_Handler    IRQ059_Handler
                Set_Default_Handler    IRQ060_Handler
                Set_Default_Handler    IRQ061_Handler
                Set_Default_Handler    IRQ062_Handler
                Set_Default_Handler    IRQ063_Handler
                Set_Default_Handler    IRQ064_Handler
                Set_Default_Handler    IRQ065_Handler
                Set_Default_Handler    IRQ066_Handler
                Set_Default_Handler    IRQ067_Handler
                Set_Default_Handler    IRQ068_Handler
                Set_Default_Handler    IRQ069_Handler
                Set_Default_Handler    IRQ070_Handler
                Set_Default_Handler    IRQ071_Handler
                Set_Default_Handler    IRQ072_Handler
                Set_Default_Handler    IRQ073_Handler
                Set_Default_Handler    IRQ074_Handler
                Set_Default_Handler    IRQ075_Handler
                Set_Default_Handler    IRQ076_Handler
                Set_Default_Handler    IRQ077_Handler
                Set_Default_Handler    IRQ078_Handler
                Set_Default_Handler    IRQ079_Handler
                Set_Default_Handler    IRQ080_Handler
                Set_Default_Handler    IRQ081_Handler
                Set_Default_Handler    IRQ082_Handler
                Set_Default_Handler    IRQ083_Handler
                Set_Default_Handler    IRQ084_Handler
                Set_Default_Handler    IRQ085_Handler
                Set_Default_Handler    IRQ086_Handler
                Set_Default_Handler    IRQ087_Handler
                Set_Default_Handler    IRQ088_Handler
                Set_Default_Handler    IRQ089_Handler
                Set_Default_Handler    IRQ090_Handler
                Set_Default_Handler    IRQ091_Handler
                Set_Default_Handler    IRQ092_Handler
                Set_Default_Handler    IRQ093_Handler
                Set_Default_Handler    IRQ094_Handler
                Set_Default_Handler    IRQ095_Handler
                Set_Default_Handler    IRQ096_Handler
                Set_Default_Handler    IRQ097_Handler
                Set_Default_Handler    IRQ098_Handler
                Set_Default_Handler    IRQ099_Handler
                Set_Default_Handler    IRQ100_Handler
                Set_Default_Handler    IRQ101_Handler
                Set_Default_Handler    IRQ102_Handler
                Set_Default_Handler    IRQ103_Handler
                Set_Default_Handler    IRQ104_Handler
                Set_Default_Handler    IRQ105_Handler
                Set_Default_Handler    IRQ106_Handler
                Set_Default_Handler    IRQ107_Handler
                Set_Default_Handler    IRQ108_Handler
                Set_Default_Handler    IRQ109_Handler
                Set_Default_Handler    IRQ110_Handler
                Set_Default_Handler    IRQ111_Handler
                Set_Default_Handler    IRQ112_Handler
                Set_Default_Handler    IRQ113_Handler
                Set_Default_Handler    IRQ114_Handler
                Set_Default_Handler    IRQ115_Handler
                Set_Default_Handler    IRQ116_Handler
                Set_Default_Handler    IRQ117_Handler
                Set_Default_Handler    IRQ118_Handler
                Set_Default_Handler    IRQ119_Handler
                Set_Default_Handler    IRQ120_Handler
                Set_Default_Handler    IRQ121_Handler
                Set_Default_Handler    IRQ122_Handler
                Set_Default_Handler    IRQ123_Handler
                Set_Default_Handler    IRQ124_Handler
                Set_Default_Handler    IRQ125_Handler
                Set_Default_Handler    IRQ126_Handler
                Set_Default_Handler    IRQ127_Handler
                Set_Default_Handler    IRQ128_Handler
                Set_Default_Handler    IRQ129_Handler
                Set_Default_Handler    IRQ130_Handler
                Set_Default_Handler    IRQ131_Handler
                Set_Default_Handler    IRQ132_Handler
                Set_Default_Handler    IRQ133_Handler
                Set_Default_Handler    IRQ134_Handler
                Set_Default_Handler    IRQ135_Handler
                Set_Default_Handler    IRQ136_Handler
                Set_Default_Handler    IRQ137_Handler
                Set_Default_Handler    IRQ138_Handler
                Set_Default_Handler    IRQ139_Handler
                Set_Default_Handler    IRQ140_Handler
                Set_Default_Handler    IRQ141_Handler
                Set_Default_Handler    IRQ142_Handler
                Set_Default_Handler    IRQ143_Handler

                .end
