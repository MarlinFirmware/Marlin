/*******************************************************************************
 * @file    tiny_sscanf.h
 * @author  MCD Application Team
 * @version V1.1.4
 * @date    08-January-2018
 * @brief   Header for driver tiny_sscanf.c module
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics International N.V.
 * All rights reserved.</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted, provided that the following conditions are met:
 *
 * 1. Redistribution of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of other
 *    contributors to this software may be used to endorse or promote products
 *    derived from this software without specific written permission.
 * 4. This software, including modifications and/or derivative works of this
 *    software, must execute solely and exclusively on microcontroller or
 *    microprocessor devices manufactured by or for STMicroelectronics.
 * 5. Redistribution and use of this software other than as permitted under
 *    this license is void and will automatically terminate your rights under
 *    this license.
 *
 * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
 * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
 * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

#ifndef __TINY_SSCANF_H__
#define __TINY_SSCANF_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/**
 * @brief  Read formatted data from string
 *
 * Reads data from s and stores them according to parameter format into the
 * locations given by the additional arguments, as if scanf was used, but
 * reading from s instead of the standard input (stdin).
 *
 * The additional arguments should point to already allocated objects of the
 * type specified by their corresponding format specifier within the format string.
 *
 * @param  C string that the function processes as its source to retrieve the data.
 * @param  C string that contains a format string that follows the same specifications
 *         as format in scanf (see scanf for details).
 * @param  Depending on the format string, the function may expect a sequence of
 *         additional arguments, each containing a pointer to allocated storage
 *         where the interpretation of the extracted characters is stored with
 *         the appropriate type.
 *         There should be at least as many of these arguments as the number of
 *         values stored by the format specifiers. Additional arguments are
 *         ignored by the function.
 * @retval The number of items in the argument list successfully filled. This
 *         count can match the expected number of items or be less (even zero)
 *         in the case of a matching failure
 * @note   Current supported formats are %hx, %hhx, %ul, %d,...
 */
int tiny_sscanf(const char *str, const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* __TINY_SSCANF_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
