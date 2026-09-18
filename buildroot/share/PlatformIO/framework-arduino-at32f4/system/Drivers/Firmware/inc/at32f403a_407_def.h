/**
  **************************************************************************
  * @file     at32f403a_407_def.h
  * @version  v2.0.9
  * @date     2022-04-25
  * @brief    at32f403a_407 macros header file
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to
  * download from Artery official website is the copyrighted work of Artery.
  * Artery authorizes customers to use, copy, and distribute the BSP
  * software and its related documentation for the purpose of design and
  * development in conjunction with Artery microcontrollers. Use of the
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AT32F403A_407_DEF_H
#define __AT32F403A_407_DEF_H

#ifdef __cplusplus
extern "C" {
#endif

/* gnu compiler */
#if defined (__GNUC__)
  #ifndef ALIGNED_HEAD
    #define ALIGNED_HEAD
  #endif
  #ifndef ALIGNED_TAIL
    #define ALIGNED_TAIL                 __attribute__ ((aligned (4)))
  #endif
#endif

/* arm compiler */
#if defined (__CC_ARM)
  #ifndef ALIGNED_HEAD
    #define ALIGNED_HEAD                 __align(4)
  #endif
  #ifndef ALIGNED_TAIL
    #define ALIGNED_TAIL
  #endif
#endif

/* iar compiler */
#if defined (__ICCARM__)
  #ifndef ALIGNED_HEAD
    #define ALIGNED_HEAD
  #endif
  #ifndef ALIGNED_TAIL
    #define ALIGNED_TAIL
  #endif
#endif

#ifdef __cplusplus
}
#endif

#endif
