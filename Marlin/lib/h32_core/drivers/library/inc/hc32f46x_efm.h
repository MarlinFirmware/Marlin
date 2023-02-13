/*****************************************************************************
 * Copyright (C) 2016, Huada Semiconductor Co.,Ltd All rights reserved.
 *
 * This software is owned and published by:
 * Huada Semiconductor Co.,Ltd ("HDSC").
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
/** \file hc32f46x_efm.h
 **
 ** A detailed description is available at
 ** @link EfmGroup EFM description @endlink
 **
 **   - 2018-10-29  1.0  Chengy First version for Device Driver Library of EFM.
 **
 ******************************************************************************/
#ifndef __HC32F46X_EFM_H__
#define __HC32F46X_EFM_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_common.h"
#include "ddl_config.h"

#if (DDL_EFM_ENABLE == DDL_ON)

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

    /**
     *******************************************************************************
     ** \defgroup EfmGroup Embedded Flash Management unit(EFM)
     **
     ******************************************************************************/
    //@{

    /*******************************************************************************
     * Global type definitions ('typedef')
     ******************************************************************************/
    /**
     *******************************************************************************
     ** \brief  The flash status.
     **
     ******************************************************************************/
    typedef enum en_efm_flash_status
    {
        FlashReady = 1u,       ///< The flash ready flag.
        FlashRWErr = 2u,       ///< The flash read/write error flag.
        FlashEOP = 3u,         ///< The flash end of operation flag.
        FlashPgMissMatch = 4u, ///< The flash program miss match flag.
        FlashPgSizeErr = 5u,   ///< The flash program size error flag.
        FlashPgareaPErr = 6u,  ///< The flash program protect area error flag.
        FlashWRPErr = 7u,      ///< The flash write protect error flag.
    } en_efm_flash_status_t;

    /**
     *******************************************************************************
     ** \brief  The flash read mode.
     **
     ******************************************************************************/
    typedef enum en_efm_read_md
    {
        NormalRead = 0u,     ///< The flash normal read mode.
        UltraPowerRead = 1u, ///< The flash ultra power read mode.
    } en_efm_read_md_t;

    /**
     *******************************************************************************
     ** \brief  The flash erase program mode.
     **
     ******************************************************************************/
    typedef enum en_efm_erase_pgm_md
    {
        ReadOnly1 = 0u,       ///< The flash read only.
        SingleProgram = 1u,   ///< The flash single program.
        SingleProgramRB = 2u, ///< The flash single program with read back.
        SequenceProgram = 3u, ///< The flash sequence program.
        SectorErase = 4u,     ///< The flash sector erase.
        MassErase = 5u,       ///< The flash mass erase.
        ReadOnly2 = 6u,       ///< The flash read only.
        ReadOnly3 = 7u,       ///< The flash read only.
    } en_efm_erase_pgm_md_t;

    /**
     *******************************************************************************
     ** \brief  The flash interrupt select.
     **
     ******************************************************************************/
    typedef enum en_efm_int_sel
    {
        PgmErsErrInt = 0u, ///< The flash program / erase error interrupt.
        EndPgmInt = 1u,    ///< The flash end of program interrupt.
        ReadErrInt = 2u,   ///< The flash read collided error interrupt.
    } en_efm_int_sel_t;

    /**
     *******************************************************************************
     ** \brief  The bus state while flash program & erase.
     **
     ******************************************************************************/
    typedef enum en_efm_bus_sta
    {
        BusBusy = 0u,    ///< The bus busy while flash program & erase.
        BusRelease = 1u, ///< The bus release while flash program & erase.
    } en_efm_bus_sta_t;

    /**
     *******************************************************************************
     ** \brief  Structure of windows protect address.
     **
     ** \note   None.
     **
     ******************************************************************************/
    typedef struct stc_efm_win_protect_addr
    {
        uint32_t StartAddr; ///< The protect start address.
        uint32_t EndAddr;   ///< The protect end address.
    } stc_efm_win_protect_addr_t;

    /**
     *******************************************************************************
     ** \brief  Structure of unique ID.
     **
     ** \note   None.
     **
     ******************************************************************************/
    typedef struct stc_efm_unique_id
    {
        uint32_t uniqueID1; ///< unique ID 1.
        uint32_t uniqueID2; ///< unique ID 2.
        uint32_t uniqueID3; ///< unique ID 3.
    } stc_efm_unique_id_t;
    /*******************************************************************************
     * Global pre-processor symbols/macros ('#define')
     ******************************************************************************/
    /* Flach latency cycle (0~15) */
#define EFM_LATENCY_0 (0ul)
#define EFM_LATENCY_1 (1ul)
#define EFM_LATENCY_2 (2ul)
#define EFM_LATENCY_3 (3ul)
#define EFM_LATENCY_4 (4ul)
#define EFM_LATENCY_5 (5ul)
#define EFM_LATENCY_6 (6ul)
#define EFM_LATENCY_7 (7ul)
#define EFM_LATENCY_8 (8ul)
#define EFM_LATENCY_9 (9ul)
#define EFM_LATENCY_10 (10ul)
#define EFM_LATENCY_11 (11ul)
#define EFM_LATENCY_12 (12ul)
#define EFM_LATENCY_13 (13ul)
#define EFM_LATENCY_14 (14ul)
#define EFM_LATENCY_15 (15ul)

/* Flash flag */
#define EFM_FLAG_WRPERR (0x00000001ul)
#define EFM_FLAG_PEPRTERR (0x00000002ul)
#define EFM_FLAG_PGSZERR (0x00000004ul)
#define EFM_FLAG_PGMISMTCH (0x00000008ul)
#define EFM_FLAG_EOP (0x00000010ul)
#define EFM_FLAG_RWERR (0x00000020ul)
#define EFM_FLAG_RDY (0x00000100ul)

    /*******************************************************************************
     * Global variable definitions ('extern')
     ******************************************************************************/

    /*******************************************************************************
     * Global function prototypes (definition in C source)
     ******************************************************************************/
    void EFM_Unlock(void);
    void EFM_Lock(void);
    void EFM_FlashCmd(en_functional_state_t enNewState);
    void EFM_SetLatency(uint32_t u32Latency);
    void EFM_InstructionCacheCmd(en_functional_state_t enNewState);
    void EFM_DataCacheRstCmd(en_functional_state_t enNewState);
    void EFM_SetReadMode(en_efm_read_md_t enReadMD);
    void EFM_ErasePgmCmd(en_functional_state_t enNewState);
    void EFM_SetErasePgmMode(en_efm_erase_pgm_md_t enReadMD);
    void EFM_InterruptCmd(en_efm_int_sel_t enInt, en_functional_state_t enNewState);
    en_flag_status_t EFM_GetFlagStatus(uint32_t u32flag);
    void EFM_ClearFlag(uint32_t u32flag);
    en_efm_flash_status_t EFM_GetStatus(void);
    void EFM_SetWinProtectAddr(stc_efm_win_protect_addr_t stcAddr);
    void EFM_SetBusState(en_efm_bus_sta_t enState);
    en_result_t EFM_SingleProgram(uint32_t u32Addr, uint32_t u32Data);
    en_result_t EFM_SingleProgramRB(uint32_t u32Addr, uint32_t u32Data);
    en_result_t EFM_SequenceProgram(uint32_t u32Addr, uint32_t u32Len, void *pBuf);
    en_result_t EFM_SectorErase(uint32_t u32Addr);
    en_result_t EFM_MassErase(uint32_t u32Addr);
    en_result_t EFM_OtpLock(uint32_t u32Addr);
    stc_efm_unique_id_t EFM_ReadUID(void);

    //@} // EfmGroup

#ifdef __cplusplus
}
#endif

#endif /* DDL_EFM_ENABLE */

#endif /* __HC32F46X_EFM_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
