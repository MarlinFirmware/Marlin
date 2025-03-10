/*!
    \file    usb_conf.h
    \brief   general low level driver configuration

    \version 2020-09-04, V3.0.0, demo for GD32F4xx
*/

/*
    Copyright (c) 2020, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#ifndef __USB_CONF_H
#define __USB_CONF_H

#include "gd32_usb_helper.h"
// #include "gd32f4xx.h"
// #include "gd32f407v_start.h"

#define USE_USB_FS
#define USE_USB_HS

/****************** USB OTG FS PHY CONFIGURATION *******************************
 *  The USB OTG FS Core supports one on-chip Full Speed PHY.
 *  The USE_EMBEDDED_PHY symbol is defined in the project compiler preprocessor 
 *  when FS core is used.
*******************************************************************************/

#if 1//def USE_USB_FS
    #define USB_FS_CORE
#endif

#if 1 //def USE_USB_HS
    #define USB_HS_CORE
#endif

/*******************************************************************************
 *                     FIFO Size Configuration in Host mode
 *
 *  (i) Receive data FIFO size = (Largest Packet Size / 4) + 1 or 
 *                                2x (Largest Packet Size / 4) + 1, If a 
 *                                high-bandwidth channel or multiple isochronous 
 *                                channels are enabled
 *
 *  (ii) For the host nonperiodic Transmit FIFO is the largest maximum packet size 
 *       for all supported nonperiodic OUT channels. Typically, a space 
 *       corresponding to two Largest Packet Size is recommended.
 *
 *  (iii) The minimum amount of RAM required for Host periodic Transmit FIFO is 
 *        the largest maximum packet size for all supported periodic OUT channels.
 *        If there is at least one High Bandwidth Isochronous OUT endpoint, 
 *        then the space must be at least two times the maximum packet size for 
 *        that channel.
*******************************************************************************/

/****************** USB OTG FS CONFIGURATION **********************************/
#ifdef USB_FS_CORE

    #define RX_FIFO_FS_SIZE                         128
    #define TX0_FIFO_FS_SIZE                        64
    #define TX1_FIFO_FS_SIZE                        64
    #define TX2_FIFO_FS_SIZE                        64
    #define TX3_FIFO_FS_SIZE                        0
    
    #define USB_RX_FIFO_FS_SIZE                            128
    #define USB_HTX_NPFIFO_FS_SIZE                         96
    #define USB_HTX_PFIFO_FS_SIZE                          96
    
    
#endif

#ifdef USB_HS_CORE
    #define RX_FIFO_HS_SIZE                          512
    #define TX0_FIFO_HS_SIZE                         128
    #define TX1_FIFO_HS_SIZE                         128
    #define TX2_FIFO_HS_SIZE                         128
    #define TX3_FIFO_HS_SIZE                         0
    #define TX4_FIFO_HS_SIZE                         0
    #define TX5_FIFO_HS_SIZE                         0
    
    #define USB_RX_FIFO_HS_SIZE                            512
    #define USB_HTX_NPFIFO_HS_SIZE                         256
    #define USB_HTX_PFIFO_HS_SIZE                          256


    #ifdef USE_ULPI_PHY
        #define USB_ULPI_PHY_ENABLED
    #endif

    #ifdef USE_EMBEDDED_PHY
        #define USB_EMBEDDED_PHY_ENABLED
    #endif

//    #define USB_HS_INTERNAL_DMA_ENABLED
#endif

#define USB_SOF_OUTPUT                                     0
#define USB_LOW_POWER                                      0

//#define USB_LOW_PWR_ENABLE

/****************** USB OTG MODE CONFIGURATION ********************************/
#define USE_HOST_MODE
#define USE_DEVICE_MODE
//#define USE_OTG_MODE

#ifndef USB_FS_CORE
    #ifndef USB_HS_CORE
        #error "USB_HS_CORE or USB_FS_CORE should be defined"
    #endif
#endif

#ifndef USE_DEVICE_MODE
    #ifndef USE_HOST_MODE
        #error "USE_DEVICE_MODE or USE_HOST_MODE should be defined"
    #endif
#endif

// #ifndef USE_USB_HS
//     #ifndef USE_USB_FS
//         #error "USE_USB_HS or USE_USB_FS should be defined"
//     #endif
// #endif

/****************** C Compilers dependent keywords ****************************/
/* In HS mode and when the DMA is used, all variables and data structures dealing
   with the DMA during the transaction process should be 4-bytes aligned */
#ifdef USB_HS_INTERNAL_DMA_ENABLED
    #if defined   (__GNUC__)            /* GNU Compiler */
        #define __ALIGN_END __attribute__ ((aligned(4)))
        #define __ALIGN_BEGIN
    #else
        #define __ALIGN_END
        #if defined   (__CC_ARM)        /* ARM Compiler */
            #define __ALIGN_BEGIN __align(4)
        #elif defined (__ICCARM__)      /* IAR Compiler */
            #define __ALIGN_BEGIN
        #elif defined  (__TASKING__)    /* TASKING Compiler */
            #define __ALIGN_BEGIN __align(4)
        #endif                          /* __CC_ARM */
    #endif                              /* __GNUC__ */
#else
    #define __ALIGN_BEGIN
    #define __ALIGN_END   
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */

/* __packed keyword used to decrease the data type alignment to 1-byte */
#if defined (__CC_ARM)         /* ARM Compiler */
    #define __packed    __packed
#elif defined (__ICCARM__)     /* IAR Compiler */
    #define __packed    __packed
#elif defined   ( __GNUC__ )   /* GNU Compiler */
    #ifndef __packed
        #define __packed    __attribute__ ((__packed__))
    #endif
#elif defined   (__TASKING__)  /* TASKING Compiler */
    #define __packed    __unaligned
#endif /* __CC_ARM */

#endif /* __USB_CONF_H */
