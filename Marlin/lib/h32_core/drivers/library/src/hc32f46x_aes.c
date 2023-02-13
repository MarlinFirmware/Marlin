/*******************************************************************************
 * Copyright (C) 2016, Huada Semiconductor Co., Ltd. All rights reserved.
 *
 * This software is owned and published by:
 * Huada Semiconductor Co., Ltd. ("HDSC").
 *
 * BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND
 * BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
 *
 * This software contains source code for use with HDSC
 * components. This software is licensed by HDSC to be adapted only
 * for use in systems utilizing HDSC components. HDSC shall not be
 * responsible for misuse or illegal use of this software for devices not
 * supported herein. HDSC is providing this software "AS IS" and will
 * not be responsible for issues arising from incorrect user implementation
 * of the software.
 *
 * Disclaimer:
 * HDSC MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE,
 * REGARDING THE SOFTWARE (INCLUDING ANY ACCOMPANYING WRITTEN MATERIALS),
 * ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED USE, INCLUDING,
 * WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, THE IMPLIED
 * WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE OR USE, AND THE IMPLIED
 * WARRANTY OF NONINFRINGEMENT.
 * HDSC SHALL HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT,
 * NEGLIGENCE OR OTHERWISE) FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT
 * LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION,
 * LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY LOSS) ARISING FROM USE OR
 * INABILITY TO USE THE SOFTWARE, INCLUDING, WITHOUT LIMITATION, ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA,
 * SAVINGS OR PROFITS,
 * EVEN IF Disclaimer HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * YOU ASSUME ALL RESPONSIBILITIES FOR SELECTION OF THE SOFTWARE TO ACHIEVE YOUR
 * INTENDED RESULTS, AND FOR THE INSTALLATION OF, USE OF, AND RESULTS OBTAINED
 * FROM, THE SOFTWARE.
 *
 * This software may be replicated in part or whole for the licensed use,
 * with the restriction that this Disclaimer and Copyright notice must be
 * included with each copy of this software, whether used in part or whole,
 * at all times.
 */
/******************************************************************************/
/** \file hc32f46x_aes.c
 **
 ** A detailed description is available at
 ** @link AesGroup Aes description @endlink
 **
 **   - 2018-10-20  1.0 Wuze First version for Device Driver Library of Aes.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32f46x_aes.h"
#include "hc32f46x_utility.h"

#if (DDL_AES_ENABLE == DDL_ON)

/**
 *******************************************************************************
 ** \addtogroup AesGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* AES block length in bytes is 16. */
#define AES_BLOCK_LEN ((uint8_t)16)

/* Each encryption operation takes 440 system clock cycles. */
#define AES_ENCRYPT_TIMEOUT (440u)

/* Each decryption operation takes 580 system clock cycles. */
#define AES_DECRYPT_TIMEOUT (580u)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void AES_WriteData(const uint8_t *pu8SrcData);
static void AES_ReadData(uint8_t *pu8Dest);
static void AES_WriteKey(const uint8_t *pu8Key);

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief  AES128 encryption(ECB mode).
 **
 ** \param  [in] pu8Plaintext           Pointer to plaintext(the source data which will be encrypted)
 **
 ** \param  [in] u32PlaintextSize       Length of plaintext in bytes.
 **
 ** \param  [in] pu8Key                 Pointer to the AES key.
 **
 ** \param  [out] pu8Ciphertext         The destination address to store the result of the encryption.
 **
 ** \retval Ok                          No error occurred.
 ** \retval ErrorTimeout                AES works timeout.
 ** \retval ErrorInvalidParameter       Parameter error.
 **
 ******************************************************************************/
en_result_t AES_Encrypt(const uint8_t *pu8Plaintext,
                        uint32_t u32PlaintextSize,
                        const uint8_t *pu8Key,
                        uint8_t *pu8Ciphertext)
{
    en_result_t enRet = ErrorInvalidParameter;
    uint32_t u32BlockOffset;
    uint32_t u32Index;
    __IO uint32_t u32TimeCount;

    if ((NULL != pu8Plaintext) &&
        (0u != u32PlaintextSize) &&
        (NULL != pu8Key) &&
        (NULL != pu8Ciphertext) &&
        (0u == (u32PlaintextSize & 0xFu)) &&       /* u32PlaintextSize % AES_BLOCK_LEN */
        (0u == ((uint32_t)pu8Plaintext & 0x3u)) && /* (uint32_t)pu8Ciphertext % 4u */
        (0u == ((uint32_t)pu8Key & 0x3u)) &&       /* (uint32_t)pu8Key % 4u */
        (0u == ((uint32_t)pu8Ciphertext & 0x3u)))  /* (uint32_t)pu8Plaintext % 4u */
    {
        /* Write the key to the register. */
        AES_WriteKey(pu8Key);
        u32BlockOffset = 0u;
        while (0u != u32PlaintextSize)
        {
            /* Stop AES calculating. */
            bM4_AES_CR_START = 0u;

            /* Write data. */
            u32Index = u32BlockOffset * AES_BLOCK_LEN;
            AES_WriteData(&pu8Plaintext[u32Index]);

            /* Set AES encrypt. */
            bM4_AES_CR_MODE = 0u;

            /* Start AES calculating. */
            bM4_AES_CR_START = 1u;

            enRet = ErrorTimeout;
            u32TimeCount = 0u;
            while (u32TimeCount < AES_ENCRYPT_TIMEOUT)
            {
                if (bM4_AES_CR_START == 0u)
                {
                    enRet = Ok;
                    break;
                }
                u32TimeCount++;
            }

            if (enRet == ErrorTimeout)
            {
                break;
            }

            AES_ReadData(&pu8Ciphertext[u32Index]);
            u32PlaintextSize -= AES_BLOCK_LEN;
            u32BlockOffset++;
        }

        /* Stop AES calculating. */
        bM4_AES_CR_START = 0u;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief  AES128 decryption(ECB mode).
 **
 ** \param  [in] pu8Ciphertext          Pointer to ciphertext(the source data which will be decrypted)
 **
 ** \param  [in] u32CiphertextSize      Length of ciphertext in bytes.
 **
 ** \param  [in] pu8Key                 Pointer to the AES key.
 **
 ** \param  [out] pu8Plaintext          The destination address to store the result of the decryption.
 **
 ** \retval Ok                          No error occurred.
 ** \retval ErrorTimeout                AES works timeout.
 ** \retval ErrorInvalidParameter       Parameter error.
 **
 ******************************************************************************/
en_result_t AES_Decrypt(const uint8_t *pu8Ciphertext,
                        uint32_t u32CiphertextSize,
                        const uint8_t *pu8Key,
                        uint8_t *pu8Plaintext)
{
    en_result_t enRet = ErrorInvalidParameter;
    uint32_t u32BlockOffset;
    uint32_t u32Index;
    __IO uint32_t u32TimeCount;

    if ((NULL != pu8Ciphertext) &&
        (0u != u32CiphertextSize) &&
        (NULL != pu8Key) &&
        (NULL != pu8Plaintext) &&
        (0u == (u32CiphertextSize & 0xFu)) &&       /* u32CiphertextSize % AES_BLOCK_LEN */
        (0u == ((uint32_t)pu8Ciphertext & 0x3u)) && /* (uint32_t)pu8Ciphertext % 4u */
        (0u == ((uint32_t)pu8Key & 0x3u)) &&        /* (uint32_t)pu8Key % 4u */
        (0u == ((uint32_t)pu8Plaintext & 0x3u)))    /* (uint32_t)pu8Plaintext % 4u */
    {
        /* Write the key to the register. */
        AES_WriteKey(pu8Key);
        u32BlockOffset = 0u;
        while (0u != u32CiphertextSize)
        {
            /* Stop AES calculating. */
            bM4_AES_CR_START = 0u;

            /* Write data. */
            u32Index = u32BlockOffset * AES_BLOCK_LEN;
            AES_WriteData(&pu8Ciphertext[u32Index]);

            /* Set AES decrypt. */
            bM4_AES_CR_MODE = 1u;

            /* Start AES calculating. */
            bM4_AES_CR_START = 1u;

            enRet = ErrorTimeout;
            u32TimeCount = 0u;
            while (u32TimeCount < AES_DECRYPT_TIMEOUT)
            {
                if (bM4_AES_CR_START == 0u)
                {
                    enRet = Ok;
                    break;
                }
                u32TimeCount++;
            }

            if (enRet == ErrorTimeout)
            {
                break;
            }

            AES_ReadData(&pu8Plaintext[u32Index]);
            u32CiphertextSize -= AES_BLOCK_LEN;
            u32BlockOffset++;
        }

        /* Stop AES calculating. */
        bM4_AES_CR_START = 0u;
    }

    return enRet;
}

/*******************************************************************************
 * Function implementation - local ('static')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief  Writes the input buffer in data register.
 **
 ** \param  [in] pu8SrcData             Pointer to source data buffer.
 **
 ** \retval None.
 **
 ******************************************************************************/
static void AES_WriteData(const uint8_t *pu8SrcData)
{
    uint8_t i;
    uint32_t u32SrcAddr = (uint32_t)pu8SrcData;
    uint32_t u32DrAddr = (uint32_t) & (M4_AES->DR0);

    for (i = 0u; i < 4u; i++)
    {
        *(__IO uint32_t *)u32DrAddr = *(uint32_t *)u32SrcAddr;
        u32SrcAddr += 4u;
        u32DrAddr += 4u;
    }
}

/**
 *******************************************************************************
 ** \brief  Reads the from data register.
 **
 ** \param  [out] pu8Dest               Pointer to the destination buffer.
 **
 ** \retval None.
 **
 ******************************************************************************/
static void AES_ReadData(uint8_t *pu8Dest)
{
    uint8_t i;
    uint32_t u32DestAddr = (uint32_t)pu8Dest;
    uint32_t u32DrAddr = (uint32_t) & (M4_AES->DR0);

    for (i = 0u; i < 4u; i++)
    {
        *(uint32_t *)u32DestAddr = *(__IO uint32_t *)u32DrAddr;
        u32DestAddr += 4u;
        u32DrAddr += 4u;
    }
}

/**
 *******************************************************************************
 ** \brief  Writes the input buffer in key register.
 **
 ** \param  [in] pu8Key             Pointer to AES key.
 **
 ** \retval None.
 **
 ******************************************************************************/
static void AES_WriteKey(const uint8_t *pu8Key)
{
    uint8_t i;
    uint32_t u32SrcKeyAddr = (uint32_t)pu8Key;
    uint32_t u32KeyAddr = (uint32_t) & (M4_AES->KR0);

    for (i = 0u; i < 4u; i++)
    {
        *(__IO uint32_t *)u32KeyAddr = *(uint32_t *)u32SrcKeyAddr;
        u32SrcKeyAddr += 4u;
        u32KeyAddr += 4u;
    }
}

//@} // AesGroup

#endif /* DDL_AES_ENABLE */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
