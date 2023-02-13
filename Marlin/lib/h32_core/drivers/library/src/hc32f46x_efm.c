/******************************************************************************
 * Copyright (C) 2016, Huada Semiconductor Co.,Ltd All rights reserved.
 *
 * This software is owned and published by:
 * Huada Semiconductor Co.,Ltd. ("HDSC").
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
/** \file hc32f46x_efm.c
 **
 ** A detailed description is available at
 ** @link EfmGroup EFM description @endlink
 **
 **   - 2018-10-29  1.0  Chengy First version for Device Driver Library of EFM.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32f46x_efm.h"
#include "hc32f46x_utility.h"

#if (DDL_EFM_ENABLE == DDL_ON)

/**
 *******************************************************************************
 ** \addtogroup EfmGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define EFM_LOCK (0x00000000u)
#define EFM_UNLOCK (0x00000001u)
#define EFM_KEY1 (0x0123ul)
#define EFM_KEY2 (0x3210ul)

#define EFM_PROTECT_ADDR_MSK (0x0007FFFFu)

/*  Parameter validity check for pointer. */
#define IS_VALID_POINTER(x) (NULL != (x))

/*  Parameter validity check for flash latency. */
#define IS_VALID_FLASH_LATENCY(x) \
    (((x) == EFM_LATENCY_0) ||    \
     ((x) == EFM_LATENCY_1) ||    \
     ((x) == EFM_LATENCY_2) ||    \
     ((x) == EFM_LATENCY_3) ||    \
     ((x) == EFM_LATENCY_4) ||    \
     ((x) == EFM_LATENCY_5) ||    \
     ((x) == EFM_LATENCY_6) ||    \
     ((x) == EFM_LATENCY_7) ||    \
     ((x) == EFM_LATENCY_8) ||    \
     ((x) == EFM_LATENCY_9) ||    \
     ((x) == EFM_LATENCY_10) ||   \
     ((x) == EFM_LATENCY_11) ||   \
     ((x) == EFM_LATENCY_12) ||   \
     ((x) == EFM_LATENCY_13) ||   \
     ((x) == EFM_LATENCY_14) ||   \
     ((x) == EFM_LATENCY_15))

/*  Parameter validity check for read mode. */
#define IS_VALID_READ_MD(MD) \
    (((MD) == NormalRead) || \
     ((MD) == UltraPowerRead))

/*  Parameter validity check for erase/program mode. */
#define IS_VALID_ERASE_PGM_MD(MD) \
    (((MD) == ReadOnly1) ||       \
     ((MD) == SingleProgram) ||   \
     ((MD) == SingleProgramRB) || \
     ((MD) == SequenceProgram) || \
     ((MD) == SectorErase) ||     \
     ((MD) == MassErase) ||       \
     ((MD) == ReadOnly2) ||       \
     ((MD) == ReadOnly3))

/*  Parameter validity check for flash flag. */
#define IS_VALID_FLASH_FLAG(flag)      \
    (((flag) == EFM_FLAG_WRPERR) ||    \
     ((flag) == EFM_FLAG_PEPRTERR) ||  \
     ((flag) == EFM_FLAG_PGSZERR) ||   \
     ((flag) == EFM_FLAG_PGMISMTCH) || \
     ((flag) == EFM_FLAG_EOP) ||       \
     ((flag) == EFM_FLAG_RWERR) ||     \
     ((flag) == EFM_FLAG_RDY))

/*  Parameter validity check for flash clear flag. */
#define IS_VALID_CLEAR_FLASH_FLAG(flag) \
    (((flag) == EFM_FLAG_WRPERR) ||     \
     ((flag) == EFM_FLAG_PEPRTERR) ||   \
     ((flag) == EFM_FLAG_PGSZERR) ||    \
     ((flag) == EFM_FLAG_PGMISMTCH) ||  \
     ((flag) == EFM_FLAG_EOP) ||        \
     ((flag) == EFM_FLAG_RWERR))

/*  Parameter validity check for flash interrupt. */
#define IS_VALID_EFM_INT_SEL(int) \
    (((int) == PgmErsErrInt) ||   \
     ((int) == EndPgmInt) ||      \
     ((int) == ReadErrInt))

/*  Parameter validity check for flash address. */
#define IS_VALID_FLASH_ADDR(addr) \
    (((addr) == 0x00000000u) ||   \
     (((addr) >= 0x00000001u) &&  \
      ((addr) <= 0x0007FFDFu)))

/*  Parameter validity check for flash address. */
#define IS_VALID_OTP_LOCK_ADDR(addr) \
    (((addr) >= 0x03000FC0u) ||      \
     ((addr) <= 0x03000FF8u))
/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief Unlock the flash.
 **
 ** \param None
 **
 ** \retval None
 **
 ** \note   None
 **
 ******************************************************************************/
void EFM_Unlock(void)
{
    M4_EFM->FAPRT = EFM_KEY1;
    M4_EFM->FAPRT = EFM_KEY2;
}

/**
 *******************************************************************************
 ** \brief Lock the flash.
 **
 ** \param None
 **
 ** \retval None
 **
 ** \note   None
 **
 ******************************************************************************/
void EFM_Lock(void)
{
    if (EFM_UNLOCK == M4_EFM->FAPRT)
    {
        M4_EFM->FAPRT = EFM_KEY2;
        M4_EFM->FAPRT = EFM_KEY2;
    }
}

/**
 *******************************************************************************
 ** \brief Enable or disable the flash.
 **
 ** \param  [in] enNewState             The new state of the flash.
 ** \arg    Enable                      Enable flash.
 ** \arg    Disable                     Stop flash.
 **
 ** \retval None
 **
 ** \note   None
 **
 ******************************************************************************/
void EFM_FlashCmd(en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    M4_EFM->FSTP_f.FSTP = ((Enable == enNewState) ? 0ul : 1ul);
}
/**
 *******************************************************************************
 ** \brief Sets the code latency value..
 **
 ** \param  [in] u32Latency             specifies the FLASH Latency value.
 ** \arg    EFM_LATENCY_0               FLASH 0 Latency cycle
 ** \arg    EFM_LATENCY_1               FLASH 1 Latency cycle
 ** \arg    EFM_LATENCY_2               FLASH 2 Latency cycles
 ** \arg    EFM_LATENCY_3               FLASH 3 Latency cycles
 ** \arg    EFM_LATENCY_4               FLASH 4 Latency cycles
 ** \arg    EFM_LATENCY_5               FLASH 5 Latency cycles
 ** \arg    EFM_LATENCY_6               FLASH 6 Latency cycles
 ** \arg    EFM_LATENCY_7               FLASH 7 Latency cycles
 ** \arg    EFM_LATENCY_8               FLASH 8 Latency cycles
 ** \arg    EFM_LATENCY_9               FLASH 9 Latency cycles
 ** \arg    EFM_LATENCY_10              FLASH 10 Latency cycles
 ** \arg    EFM_LATENCY_11              FLASH 11 Latency cycles
 ** \arg    EFM_LATENCY_12              FLASH 12 Latency cycles
 ** \arg    EFM_LATENCY_13              FLASH 13 Latency cycles
 ** \arg    EFM_LATENCY_14              FLASH 14 Latency cycles
 ** \arg    EFM_LATENCY_15              FLASH 15 Latency cycles
 **
 ** \retval None
 **
 ** \note   None
 **
 ******************************************************************************/
void EFM_SetLatency(uint32_t u32Latency)
{
    DDL_ASSERT(IS_VALID_FLASH_LATENCY(u32Latency));

    M4_EFM->FRMC_f.FLWT = u32Latency;
}

/**
 *******************************************************************************
 ** \brief Enable or disable the flash instruction cache.
 **
 ** \param  [in] enNewState             The new state of the flash instruction cache.
 ** \arg    Enable                      Enable flash instruction cache.
 ** \arg    Disable                     Disable flash instruction cache.
 **
 ** \retval None
 **
 ** \note   None
 **
 ******************************************************************************/
void EFM_InstructionCacheCmd(en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    M4_EFM->FRMC_f.CACHE = enNewState;
}

/**
 *******************************************************************************
 ** \brief Enable or disable the data cache reset.
 **
 ** \param  [in] enNewState             The new state of the data cache reset.
 ** \arg    Enable                      Enable data cache reset.
 ** \arg    Disable                     Disable data cache reset.
 **
 ** \retval None
 **
 ** \note   None
 **
 ******************************************************************************/
void EFM_DataCacheRstCmd(en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    M4_EFM->FRMC_f.CRST = enNewState;
}

/**
 *******************************************************************************
 ** \brief Set the flash read mode.
 **
 ** \param  [in] enReadMD               The flash read mode.
 ** \arg    NormalRead                  Normal read mode.
 ** \arg    UltraPowerRead              Ultra_Low power read mode.
 **
 ** \retval None.
 **
 ** \note   None
 **
 ******************************************************************************/
void EFM_SetReadMode(en_efm_read_md_t enReadMD)
{
    DDL_ASSERT(IS_VALID_READ_MD(enReadMD));
    M4_EFM->FRMC_f.SLPMD = enReadMD;
}

/**
 *******************************************************************************
 ** \brief Enable or disable erase / program.
 **
 ** \param  [in] enNewState             The new state of the erase / program.
 ** \arg    Enable                      Enable erase / program.
 ** \arg    Disable                     Disable erase / program.
 **
 ** \retval None
 **
 ** \note   None
 **
 ******************************************************************************/
void EFM_ErasePgmCmd(en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    M4_EFM->FWMC_f.PEMODE = enNewState;
}

/**
 *******************************************************************************
 ** \brief Set the flash erase program mode.
 **
 ** \param  [in] enReadMD               The flash erase program mode.
 ** \arg    ReadOnly1                   The flash read only 1.
 ** \arg    SingleProgram               The flash single program.
 ** \arg    SingleProgramRB             The flash single program with read back.
 ** \arg    SequenceProgram             The flash sequence program.
 ** \arg    SectorErase                 The flash sector erase.
 ** \arg    MassErase                   The flash mass erase.
 ** \arg    ReadOnly2                   The flash read only 2.
 ** \arg    ReadOnly3                   The flash read only 3.
 **
 ** \retval None.
 **
 ** \note   None
 **
 ******************************************************************************/
void EFM_SetErasePgmMode(en_efm_erase_pgm_md_t enReadMD)
{
    DDL_ASSERT(IS_VALID_ERASE_PGM_MD(enReadMD));
    M4_EFM->FWMC_f.PEMOD = enReadMD;
}
/**
 *******************************************************************************
 ** \brief Enable or disable the specified interrupt.
 **
 ** \param  [in] enInt                  The specified interrupt.
 ** \arg    PgmErsErrInt                Program erase error interrupt.
 ** \arg    EndPgmInt                   End of Program interrupt.
 ** \arg    ReadErrInt                  Read collided error flag.
 **
 ** \param  [in] enNewState             The new state of the specified interrupt.
 ** \arg    Enable                      Enable the specified interrupt.
 ** \arg    Disable                     Disable the specified interrupt.
 **
 ** \retval None
 **
 ** \note   None
 **
 ******************************************************************************/
void EFM_InterruptCmd(en_efm_int_sel_t enInt, en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));
    DDL_ASSERT(IS_VALID_EFM_INT_SEL(enInt));

    switch (enInt)
    {
    case PgmErsErrInt:
        M4_EFM->FITE_f.PEERRITE = enNewState;
        break;
    case EndPgmInt:
        M4_EFM->FITE_f.OPTENDITE = enNewState;
        break;
    case ReadErrInt:
        M4_EFM->FITE_f.RDCOLERRITE = enNewState;
        break;
    default:
        break;
    }
}

/**
 *******************************************************************************
 ** \brief Checks whether the specified FLASH flag is set or not..
 **
 ** \param [in] u32flag                 Specifies the FLASH flag to check.
 ** \arg   EFM_FLAG_WRPERR              Flash write protect error flag.
 ** \arg   EFM_FLAG_PEPRTERR            Flash program protect area error flag.
 ** \arg   EFM_FLAG_PGSZERR             Flash program size error flag.
 ** \arg   EFM_FLAG_PGMISMTCH           Flash program miss match flag.
 ** \arg   EFM_FLAG_EOP                 Flash end of program flag.
 ** \arg   EFM_FLAG_RWERR               Flash read write error flag.
 ** \arg   EFM_FLAG_RDY                 Flash ready flag.
 **
 ** \retval The flash status.
 **
 ** \note   None
 **
 ******************************************************************************/
en_flag_status_t EFM_GetFlagStatus(uint32_t u32flag)
{
    DDL_ASSERT(IS_VALID_FLASH_FLAG(u32flag));

    return ((0ul == (M4_EFM->FSR & u32flag)) ? Reset : Set);
}

/**
 *******************************************************************************
 ** \brief Checks whether the specified FLASH flag is set or not..
 **
 ** \param [in] u32flag                 Specifies the FLASH flag to clear.
 ** \arg   EFM_FLAG_WRPERR              Flash write protect error flag.
 ** \arg   EFM_FLAG_PEPRTERR            Flash program protect area error flag.
 ** \arg   EFM_FLAG_PGSZERR             Flash program size error flag.
 ** \arg   EFM_FLAG_PGMISMTCH           Flash program miss match flag.
 ** \arg   EFM_FLAG_EOP                 Flash end of program flag.
 ** \arg   EFM_FLAG_RWERR               Flash read write error flag.
 **
 ** \retval The flash status.
 **
 ** \note   None
 **
 ******************************************************************************/
void EFM_ClearFlag(uint32_t u32flag)
{
    // DDL_ASSERT(IS_VALID_CLEAR_FLASH_FLAG(u32flag));

    M4_EFM->FSCLR = u32flag;
}
/**
 *******************************************************************************
 ** \brief Get the flash status.
 **
 ** \param None
 **
 ** \retval The flash status.
 **
 ** \note   None
 **
 ******************************************************************************/
en_efm_flash_status_t EFM_GetStatus(void)
{
    en_efm_flash_status_t enFlashStatus = FlashEOP;

    if (1ul == M4_EFM->FSR_f.RDY)
    {
        enFlashStatus = FlashReady;
    }
    else if (1ul == M4_EFM->FSR_f.RDCOLERR)
    {
        enFlashStatus = FlashRWErr;
    }
    else if (1ul == M4_EFM->FSR_f.OPTEND)
    {
        enFlashStatus = FlashEOP;
    }
    else if (1ul == M4_EFM->FSR_f.PGMISMTCH)
    {
        enFlashStatus = FlashPgMissMatch;
    }
    else if (1ul == M4_EFM->FSR_f.PGSZERR)
    {
        enFlashStatus = FlashPgSizeErr;
    }
    else if (1ul == M4_EFM->FSR_f.PEPRTERR)
    {
        enFlashStatus = FlashPgareaPErr;
    }
    else if (1ul == M4_EFM->FSR_f.PEWERR)
    {
        enFlashStatus = FlashWRPErr;
    }
    else
    {
        // else
    }

    return enFlashStatus;
}

/**
 *******************************************************************************
 ** \brief Set flash the windows protect address.
 **
 ** \param  [in] stcAddr                The specified windows protect address.
 ** \arg    StartAddr                   The start of windows protect address.
 ** \arg    EndAddr                     The end of windows protect address.
 **
 ** \retval None
 **
 ** \note   None
 **
 ******************************************************************************/
void EFM_SetWinProtectAddr(stc_efm_win_protect_addr_t stcAddr)
{
    M4_EFM->FPMTSW_f.FPMTSW = (stcAddr.StartAddr & EFM_PROTECT_ADDR_MSK);
    M4_EFM->FPMTEW_f.FPMTEW = (stcAddr.EndAddr & EFM_PROTECT_ADDR_MSK);
}

/**
 *******************************************************************************
 ** \brief Set bus state while flash program & erase.
 **
 ** \param  [in] enState                The specified bus state while flash program & erase.
 ** \arg    BusBusy                     The bus busy.
 ** \arg    BusRelease                  The bus release.
 **
 ** \retval None
 **
 ** \note   None
 **
 ******************************************************************************/
void EFM_SetBusState(en_efm_bus_sta_t enState)
{
    M4_EFM->FWMC_f.BUSHLDCTL = enState;
}

/**
 *******************************************************************************
 ** \brief Flash single program without read back.
 **
 ** \param  [in] u32Addr                The specified program address.
 ** \param  [in] u32Data                The specified program data.
 **
 ** \retval en_result_t
 **
 ** \note   None
 **
 ******************************************************************************/
en_result_t EFM_SingleProgram(uint32_t u32Addr, uint32_t u32Data)
{
    en_result_t enRet = Ok;
    uint8_t u8tmp;
    uint16_t u16Timeout = 0u;

    DDL_ASSERT(IS_VALID_FLASH_ADDR(u32Addr));

    /* CLear the error flag. */
    EFM_ClearFlag(EFM_FLAG_WRPERR | EFM_FLAG_PEPRTERR | EFM_FLAG_PGSZERR |
                  EFM_FLAG_PGMISMTCH | EFM_FLAG_EOP | EFM_FLAG_RWERR);

    /* read back CACHE */
    u8tmp = (uint8_t)M4_EFM->FRMC_f.CACHE;

    M4_EFM->FRMC_f.CACHE = Disable;

    /* Enable program. */
    EFM_ErasePgmCmd(Enable);
    /* Set single program mode. */
    EFM_SetErasePgmMode(SingleProgram);
    /* program data. */
    *(uint32_t *)u32Addr = u32Data;

    while (1ul != M4_EFM->FSR_f.RDY)
    {
        u16Timeout++;
        if (u16Timeout > 0x1000u)
        {
            enRet = ErrorTimeout;
        }
    }

    if (u32Data != *(uint32_t *)u32Addr)
    {
        enRet = Error;
    }

    EFM_ClearFlag(EFM_FLAG_EOP);
    /* Set read only mode. */
    EFM_SetErasePgmMode(ReadOnly1);
    EFM_ErasePgmCmd(Disable);

    /* recover CACHE */
    M4_EFM->FRMC_f.CACHE = u8tmp;

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Flash single program with read back.
 **
 ** \param  [in] u32Addr                The specified program address.
 ** \param  [in] u32Data                The specified program data.
 **
 ** \retval en_result_t
 **
 ** \note   None
 **
 ******************************************************************************/
en_result_t EFM_SingleProgramRB(uint32_t u32Addr, uint32_t u32Data)
{
    en_result_t enRet = Ok;
    uint8_t u8tmp = 0u;
    uint16_t u16Timeout = 0u;

    DDL_ASSERT(IS_VALID_FLASH_ADDR(u32Addr));

    /* CLear the error flag. */
    EFM_ClearFlag(EFM_FLAG_WRPERR | EFM_FLAG_PEPRTERR | EFM_FLAG_PGSZERR |
                  EFM_FLAG_PGMISMTCH | EFM_FLAG_EOP | EFM_FLAG_RWERR);

    /* read back CACHE */
    u8tmp = (uint8_t)M4_EFM->FRMC_f.CACHE;

    M4_EFM->FRMC_f.CACHE = Disable;

    /* Enable program. */
    EFM_ErasePgmCmd(Enable);
    /* Set single program with read back mode. */
    EFM_SetErasePgmMode(SingleProgramRB);
    /* program data. */
    *(uint32_t *)u32Addr = u32Data;

    while (1ul != M4_EFM->FSR_f.RDY)
    {
        u16Timeout++;
        if (u16Timeout > 0x1000u)
        {
            enRet = ErrorTimeout;
        }
    }

    if (1ul == M4_EFM->FSR_f.PGMISMTCH)
    {
        enRet = Error;
    }

    EFM_ClearFlag(EFM_FLAG_EOP);
    /* Set read only mode. */
    EFM_SetErasePgmMode(ReadOnly1);
    EFM_ErasePgmCmd(Disable);

    /* recover CACHE */
    M4_EFM->FRMC_f.CACHE = u8tmp;

    return enRet;
}

static void *EFM_Memcpy(void *pvDst, void *pvSrc, uint32_t u32Count)
{
    uint8_t *u8TmpDst = (uint8_t *)pvDst;
    uint8_t *u8TmpSrc = (uint8_t *)pvSrc;

    DDL_ASSERT(IS_VALID_POINTER(pvDst));
    DDL_ASSERT(IS_VALID_POINTER(pvSrc));

    while (u32Count--)
    {
        *u8TmpDst++ = *u8TmpSrc++;
    }

    return pvDst;
}
/**
 *******************************************************************************
 ** \brief Flash sequence program.
 **
 ** \param  [in] u32Addr                The specified program address.
 ** \param  [in] u32Len                 The len of specified program data.
 ** \param  [in] *pBuf                  The pointer of specified program data.
 **
 ** \retval en_result_t
 **
 ** \note   None
 **
 ******************************************************************************/
en_result_t EFM_SequenceProgram(uint32_t u32Addr, uint32_t u32Len, void *pBuf)
{
    en_result_t enRet = Ok;
    uint8_t u8tmp;
    uint32_t i;
    uint16_t u16Timeout = 0u;
    uint32_t u32Tmp = 0xFFFFFFFFul;
    // uint32_t *u32pSrc = pBuf;
    uint32_t *u32pSrc = (uint32_t *)pBuf;
    uint32_t *u32pDest = (uint32_t *)u32Addr;
    uint32_t u32LoopWords = u32Len >> 2ul;
    uint32_t u32RemainBytes = u32Len % 4ul;

    DDL_ASSERT(IS_VALID_FLASH_ADDR(u32Addr));
    DDL_ASSERT(IS_VALID_POINTER(pBuf));

    /* CLear the error flag. */
    EFM_ClearFlag(EFM_FLAG_WRPERR | EFM_FLAG_PEPRTERR | EFM_FLAG_PGSZERR |
                  EFM_FLAG_PGMISMTCH | EFM_FLAG_EOP | EFM_FLAG_RWERR);

    /* read back CACHE */
    u8tmp = (uint8_t)M4_EFM->FRMC_f.CACHE;

    M4_EFM->FRMC_f.CACHE = Disable;

    /* Enable program. */
    EFM_ErasePgmCmd(Enable);
    /* Set sequence program mode. */
    EFM_SetErasePgmMode(SequenceProgram);
    /* clear read collided error flag.*/
    EFM_ClearFlag(EFM_FLAG_RWERR);
    EFM_ClearFlag(EFM_FLAG_WRPERR);

    /* program data. */
    for (i = 0ul; i < u32LoopWords; i++)
    {
        *u32pDest++ = *u32pSrc++;
        /* wait operate end. */
        while (1ul != M4_EFM->FSR_f.OPTEND)
        {
            u16Timeout++;
            if (u16Timeout > 0x1000u)
            {
                enRet = ErrorTimeout;
            }
        }
        /* clear end flag. */
        EFM_ClearFlag(EFM_FLAG_EOP);
    }
    if (u32RemainBytes)
    {
        EFM_Memcpy(&u32Tmp, u32pSrc, u32RemainBytes);
        *u32pDest++ = u32Tmp;
    }

    /* Set read only mode. */
    EFM_SetErasePgmMode(ReadOnly1);

    u16Timeout = 0u;
    while (1ul != M4_EFM->FSR_f.RDY)
    {
        u16Timeout++;
        if (u16Timeout > 0x1000u)
        {
            enRet = ErrorTimeout;
        }
    }

    EFM_ClearFlag(EFM_FLAG_EOP);
    EFM_ErasePgmCmd(Disable);

    /* recover CACHE */
    M4_EFM->FRMC_f.CACHE = u8tmp;

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Flash sector erase.
 **
 ** \param  [in] u32Addr                The uncertain(random) address in the specified sector.
 **
 ** \retval en_result_t
 **
 ** \note   The address should be word align.
 **
 ******************************************************************************/
en_result_t EFM_SectorErase(uint32_t u32Addr)
{
    uint8_t u8tmp;
    uint16_t u16Timeout = 0u;
    en_result_t enRet = Ok;

    DDL_ASSERT(IS_VALID_FLASH_ADDR(u32Addr));

    /* CLear the error flag. */
    EFM_ClearFlag(EFM_FLAG_WRPERR | EFM_FLAG_PEPRTERR | EFM_FLAG_PGSZERR |
                  EFM_FLAG_PGMISMTCH | EFM_FLAG_EOP | EFM_FLAG_RWERR);

    /* read back CACHE */
    u8tmp = (uint8_t)M4_EFM->FRMC_f.CACHE;

    M4_EFM->FRMC_f.CACHE = Disable;

    /* Enable erase. */
    EFM_ErasePgmCmd(Enable);
    /* Set sector erase mode. */
    EFM_SetErasePgmMode(SectorErase);

    *(uint32_t *)u32Addr = 0x12345678u;

    while (1ul != M4_EFM->FSR_f.RDY)
    {
        u16Timeout++;
        if (u16Timeout > 0x1000u)
        {
            enRet = ErrorTimeout;
        }
    }

    EFM_ClearFlag(EFM_FLAG_EOP);
    /* Set read only mode. */
    EFM_SetErasePgmMode(ReadOnly1);
    EFM_ErasePgmCmd(Disable);

    /* recover CACHE */
    M4_EFM->FRMC_f.CACHE = u8tmp;

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Flash mass erase.
 **
 ** \param  [in] u32Addr                The uncertain(random) address in the flash.
 **
 ** \retval en_result_t
 **
 ** \note   The address should be word align.
 **
 ******************************************************************************/
en_result_t EFM_MassErase(uint32_t u32Addr)
{
    uint8_t u8tmp;
    uint16_t u16Timeout = 0u;
    en_result_t enRet = Ok;

    DDL_ASSERT(IS_VALID_FLASH_ADDR(u32Addr));

    /* CLear the error flag. */
    EFM_ClearFlag(EFM_FLAG_WRPERR | EFM_FLAG_PEPRTERR | EFM_FLAG_PGSZERR |
                  EFM_FLAG_PGMISMTCH | EFM_FLAG_EOP | EFM_FLAG_RWERR);

    /* read back CACHE */
    u8tmp = (uint8_t)M4_EFM->FRMC_f.CACHE;

    M4_EFM->FRMC_f.CACHE = Disable;

    /* Enable erase. */
    EFM_ErasePgmCmd(Enable);
    /* Set sector erase mode. */
    EFM_SetErasePgmMode(MassErase);

    *(uint32_t *)u32Addr = 0x12345678u;

    while (1ul != M4_EFM->FSR_f.RDY)
    {
        u16Timeout++;
        if (u16Timeout > 0x1000u)
        {
            enRet = ErrorTimeout;
        }
    }

    EFM_ClearFlag(EFM_FLAG_EOP);
    /* Set read only mode. */
    EFM_SetErasePgmMode(ReadOnly1);
    EFM_ErasePgmCmd(Disable);

    /* recover CACHE */
    M4_EFM->FRMC_f.CACHE = u8tmp;

    return enRet;
}

/**
 *******************************************************************************
 ** \brief  Lock OTP data block.
 **
 ** \param  u32Addr                     The addr to lock.
 **
 ** \retval en_result_t
 **
 ** \note   None
 **
 ******************************************************************************/
en_result_t EFM_OtpLock(uint32_t u32Addr)
{
    DDL_ASSERT(IS_VALID_OTP_LOCK_ADDR(u32Addr));
    uint16_t u16Timeout = 0u;
    en_result_t enRet = Ok;

    /* Enable program. */
    EFM_ErasePgmCmd(Enable);
    /* Set single program mode. */
    EFM_SetErasePgmMode(SingleProgram);

    /* Lock the otp block. */
    *(uint32_t *)u32Addr = 0ul;

    while (1ul != M4_EFM->FSR_f.RDY)
    {
        u16Timeout++;
        if (u16Timeout > 0x1000u)
        {
            enRet = ErrorTimeout;
        }
    }

    EFM_ClearFlag(EFM_FLAG_EOP);
    /* Set read only mode. */
    EFM_SetErasePgmMode(ReadOnly1);
    EFM_ErasePgmCmd(Disable);

    return enRet;
}
/**
 *******************************************************************************
 ** \brief read unique ID.
 **
 ** \param None
 **
 ** \retval uint32_t
 **
 ** \note   None
 **
 ******************************************************************************/
stc_efm_unique_id_t EFM_ReadUID(void)
{
    stc_efm_unique_id_t stcUID;

    stcUID.uniqueID1 = M4_EFM->UQID1;
    stcUID.uniqueID2 = M4_EFM->UQID2;
    stcUID.uniqueID3 = M4_EFM->UQID3;

    return stcUID;
}

//@} // EfmGroup

#endif /* DDL_EFM_ENABLE */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
