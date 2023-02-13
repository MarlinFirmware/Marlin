/*****************************************************************************
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
/** \file hc32f46x_dmac.h
 **
 ** A detailed description is available at
 ** @link DmacGroup DMAC description @endlink
 **
 **   - 2018-11-18  1.0  Chengy First version for Device Driver Library of DMAC.
 **
 ******************************************************************************/
#ifndef __HC32F46X_DMAC_H__
#define __HC32F46X_DMAC_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_common.h"
#include "ddl_config.h"

#if (DDL_DMAC_ENABLE == DDL_ON)

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

    /**
     *******************************************************************************
     ** \defgroup DmacGroup Direct Memory Access Control(DMAC)
     **
     ******************************************************************************/
    //@{

    /*******************************************************************************
     * Global type definitions ('typedef')
     ******************************************************************************/

    /**
     *******************************************************************************
     ** \brief DMA Channel
     **
     ******************************************************************************/
    typedef enum en_dma_channel
    {
        DmaCh0 = 0u,  ///< DMA channel 0
        DmaCh1 = 1u,  ///< DMA channel 1
        DmaCh2 = 2u,  ///< DMA channel 2
        DmaCh3 = 3u,  ///< DMA channel 3
        DmaChMax = 4u ///< DMA channel max
    } en_dma_channel_t;

    /**
     *******************************************************************************
     ** \brief DMA transfer data width
     **
     ******************************************************************************/
    typedef enum en_dma_transfer_width
    {
        Dma8Bit = 0u,  ///< 8 bit transfer via DMA
        Dma16Bit = 1u, ///< 16 bit transfer via DMA
        Dma32Bit = 2u  ///< 32 bit transfer via DMA
    } en_dma_transfer_width_t;

    /**
     *******************************************************************************
     ** \brief DMA flag
     **
     ******************************************************************************/
    typedef enum en_dma_flag
    {
        DmaTransferComplete = 0u, ///< DMA transfer complete
        DmaBlockComplete = 1u,    ///< DMA block transfer complete
        DmaTransferErr = 2u,      ///< DMA transfer error
        DmaReqErr = 3u,           ///< DMA transfer request error
        DmaFlagMax = 4u
    } en_dma_flag_t;

    /**
     *******************************************************************************
     ** \brief DMA address mode
     **
     ******************************************************************************/
    typedef enum en_dma_address_mode
    {
        AddressFix = 0u,      ///< Address fixed
        AddressIncrease = 1u, ///< Address increased
        AddressDecrease = 2u, ///< Address decreased
    } en_dma_address_mode_t;

    /**
     *******************************************************************************
     ** \brief DMA link list pointer mode
     **
     ******************************************************************************/
    typedef enum en_dma_llp_mode
    {
        LlpWaitNextReq = 0u, ///< DMA trigger transfer after wait next request
        LlpRunNow = 1u,      ///< DMA trigger transfer now
    } en_dma_llp_mode_t;

    /**
     *******************************************************************************
     ** \brief DMA interrupt selection
     **
     ******************************************************************************/
    typedef enum en_dma_irq_sel
    {
        TrnErrIrq = 0u,     ///< Select DMA transfer error interrupt
        TrnReqErrIrq = 1u,  ///< Select DMA transfer req over error interrupt
        TrnCpltIrq = 2u,    ///< Select DMA transfer completion interrupt
        BlkTrnCpltIrq = 3u, ///< Select DMA block completion interrupt
        DmaIrqSelMax = 4u
    } en_dma_irq_sel_t;

    /**
     *******************************************************************************
     ** \brief DMA re_config count mode
     **
     ******************************************************************************/
    typedef enum en_dma_recfg_cnt_mode
    {
        CntFix = 0u,     ///< Fix
        CntSrcAddr = 1u, ///< Source address mode
        CntDesAddr = 2u, ///< Destination address mode
    } en_dma_recfg_cnt_mode_t;

    /**
     *******************************************************************************
     ** \brief DMA re_config destination address mode
     **
     ******************************************************************************/
    typedef enum en_dma_recfg_daddr_mode
    {
        DaddrFix = 0u,  ///< Fix
        DaddrNseq = 1u, ///< No_sequence address
        DaddrRep = 2u,  ///< Repeat address
    } en_dma_recfg_daddr_mode_t;

    /**
     *******************************************************************************
     ** \brief DMA re_config source address mode
     **
     ******************************************************************************/
    typedef enum en_dma_recfg_saddr_mode
    {
        SaddrFix = 0u,  ///< Fix
        SaddrNseq = 1u, ///< No_sequence address
        SaddrRep = 2u,  ///< Repeat address
    } en_dma_recfg_saddr_mode_t;

    /**
     *******************************************************************************
     ** \brief DMA channel status
     **
     ******************************************************************************/
    typedef enum en_dma_ch_flag
    {
        DmaSta = 0u,    ///< DMA status.
        ReCfgSta = 1u,  ///< DMA re_configuration status.
        DmaCh0Sta = 2u, ///< DMA channel 0 status.
        DmaCh1Sta = 3u, ///< DMA channel 1 status.
        DmaCh2Sta = 4u, ///< DMA channel 2 status.
        DmaCh3Sta = 5u, ///< DMA channel 3 status.
    } en_dma_ch_flag_t;

    /**
     *******************************************************************************
     ** \brief DMA llp descriptor
     **
     ******************************************************************************/
    typedef struct stc_dma_llp_descriptor
    {
        uint32_t SARx; ///< DMA source address register
        uint32_t DARx; ///< DMA destination address register
        union
        {
            uint32_t DTCTLx;
            stc_dma_dtctl_field_t DTCTLx_f; ///< DMA data control register
        };
        union
        {
            uint32_t RPTx;
            stc_dma_rpt_field_t RPTx_f; ///< DMA repeat control register
        };
        union
        {
            uint32_t SNSEQCTLx;
            stc_dma_snseqctl_field_t SNSEQCTLx_f; ///< DMA source no-sequence control register
        };
        union
        {
            __IO uint32_t DNSEQCTLx;
            stc_dma_dnseqctl_field_t DNSEQCTLx_f; ///< DMA destination no-sequence control register
        };
        union
        {
            uint32_t LLPx;
            stc_dma_llp_field_t LLPx_f; ///< DMA link-list-pointer register
        };
        union
        {
            uint32_t CHxCTL;
            stc_dma_ch0ctl_field_t CHxCTL_f; ///< DMA channel control register
        };
    } stc_dma_llp_descriptor_t;

    /**
     *******************************************************************************
     ** \brief DMA no-sequence function configuration
     **
     ******************************************************************************/
    typedef struct stc_dma_nseq_cfg
    {
        uint32_t u32Offset; ///< DMA no-sequence offset.
        uint16_t u16Cnt;    ///< DMA no-sequence count.
    } stc_dma_nseq_cfg_t;

    /**
     *******************************************************************************
     ** \brief DMA no-sequence function configuration
     **
     ******************************************************************************/
    typedef struct stc_dma_nseqb_cfg
    {
        uint32_t u32NseqDist; ///< DMA no-sequence district interval.
        uint16_t u16CntB;     ///< DMA no-sequence count.
    } stc_dma_nseqb_cfg_t;

    /**
     *******************************************************************************
     ** \brief DMA re_config configuration
     **
     ******************************************************************************/
    typedef struct stc_dma_recfg_ctl
    {
        uint16_t u16SrcRptBSize;             ///< The source repeat size.
        uint16_t u16DesRptBSize;             ///< The destination repeat size.
        en_functional_state_t enReCfgLlp;    ///< Enable llp re_config function or not.
        en_dma_channel_t enReCfgCh;          ///< DMA re_config channel.
        en_dma_recfg_saddr_mode_t enSaddrMd; ///< DMA re_config source address mode.
        en_dma_recfg_daddr_mode_t enDaddrMd; ///< DMA re_config destination address mode.
        en_dma_recfg_cnt_mode_t enCntMd;     ///< DMA re_config count mode.
        stc_dma_nseq_cfg_t stcSrcNseqBCfg;   ///< The source no_sequence re_config.
        stc_dma_nseq_cfg_t stcDesNseqBCfg;   ///< The destination no_sequence re_config.
    } stc_dma_recfg_ctl_t;

    /**
     *******************************************************************************
     ** \brief DMA channel configuration
     **
     ******************************************************************************/
    typedef struct stc_dma_ch_cfg
    {
        en_dma_address_mode_t enSrcInc;     ///< DMA source address update mode.
        en_dma_address_mode_t enDesInc;     ///< DMA destination address update mode.
        en_functional_state_t enSrcRptEn;   ///< Enable source repeat function or not.
        en_functional_state_t enDesRptEn;   ///< Enable destination repeat function or not.
        en_functional_state_t enSrcNseqEn;  ///< Enable source no_sequence function or not.
        en_functional_state_t enDesNseqEn;  ///< Enable destination no_sequence function or not.
        en_dma_transfer_width_t enTrnWidth; ///< DMA transfer data width.
        en_functional_state_t enLlpEn;      ///< Enable linked list pointer function or not.
        en_dma_llp_mode_t enLlpMd;          ///< Dma linked list pointer mode.
        en_functional_state_t enIntEn;      ///< Enable interrupt function or not.
    } stc_dma_ch_cfg_t;

    /**
     *******************************************************************************
     ** \brief DMA configuration
     **
     ******************************************************************************/
    typedef struct stc_dma_config
    {
        uint16_t u16BlockSize;            ///< Transfer block size = 1024, when 0 is set.
        uint16_t u16TransferCnt;          ///< Transfer counter.
        uint32_t u32SrcAddr;              ///< The source address.
        uint32_t u32DesAddr;              ///< The destination address.
        uint16_t u16SrcRptSize;           ///< The source repeat size.
        uint16_t u16DesRptSize;           ///< The destination repeat size.
        uint32_t u32DmaLlp;               ///< The Dma linked list pointer address
        stc_dma_nseq_cfg_t stcSrcNseqCfg; ///< The source no_sequence configuration.
        stc_dma_nseq_cfg_t stcDesNseqCfg; ///< The destination no_sequence configuration.
        stc_dma_ch_cfg_t stcDmaChCfg;     ///< The Dma channel configuration.
    } stc_dma_config_t;

    /*******************************************************************************
     * Global pre-processor symbols/macros ('#define')
     ******************************************************************************/

    /*******************************************************************************
     * Global variable definitions ('extern')
     ******************************************************************************/

    /*******************************************************************************
     * Global function prototypes (definition in C source)
     ******************************************************************************/
    void DMA_Cmd(M4_DMA_TypeDef *pstcDmaReg, en_functional_state_t enNewState);
    en_result_t DMA_EnableIrq(M4_DMA_TypeDef *pstcDmaReg, uint8_t u8Ch, en_dma_irq_sel_t enIrqSel);
    en_result_t DMA_DisableIrq(M4_DMA_TypeDef *pstcDmaReg, uint8_t u8Ch, en_dma_irq_sel_t enIrqSel);
    en_flag_status_t DMA_GetIrqFlag(M4_DMA_TypeDef *pstcDmaReg, uint8_t u8Ch, en_dma_irq_sel_t enIrqSel);
    en_result_t DMA_ClearIrqFlag(M4_DMA_TypeDef *pstcDmaReg, uint8_t u8Ch, en_dma_irq_sel_t enIrqSel);
    en_result_t DMA_ChannelCmd(M4_DMA_TypeDef *pstcDmaReg, uint8_t u8Ch, en_functional_state_t enNewState);
    void DMA_InitReConfig(M4_DMA_TypeDef *pstcDmaReg, uint8_t u8Ch, const stc_dma_recfg_ctl_t *pstcDmaReCfg);
    void DMA_ReCfgCmd(M4_DMA_TypeDef *pstcDmaReg, en_functional_state_t enNewState);
    en_flag_status_t DMA_GetChFlag(M4_DMA_TypeDef *pstcDmaReg, en_dma_ch_flag_t enDmaChFlag);

    en_result_t DMA_SetSrcAddress(M4_DMA_TypeDef *pstcDmaReg, uint8_t u8Ch, uint32_t u32Address);
    en_result_t DMA_SetDesAddress(M4_DMA_TypeDef *pstcDmaReg, uint8_t u8Ch, uint32_t u32Address);
    en_result_t DMA_SetBlockSize(M4_DMA_TypeDef *pstcDmaReg, uint8_t u8Ch, uint16_t u16BlkSize);
    en_result_t DMA_SetTransferCnt(M4_DMA_TypeDef *pstcDmaReg, uint8_t u8Ch, uint16_t u16TrnCnt);
    en_result_t DMA_SetSrcRptSize(M4_DMA_TypeDef *pstcDmaReg, uint8_t u8Ch, uint16_t u16Size);
    en_result_t DMA_SetDesRptSize(M4_DMA_TypeDef *pstcDmaReg, uint8_t u8Ch, uint16_t u16Size);
    en_result_t DMA_SetSrcRptbSize(M4_DMA_TypeDef *pstcDmaReg, uint8_t u8Ch, uint16_t u16Size);
    en_result_t DMA_SetDesRptbSize(M4_DMA_TypeDef *pstcDmaReg, uint8_t u8Ch, uint16_t u16Size);
    en_result_t DMA_SetSrcNseqCfg(M4_DMA_TypeDef *pstcDmaReg, uint8_t u8Ch, const stc_dma_nseq_cfg_t *pstcSrcNseqCfg);
    en_result_t DMA_SetSrcNseqBCfg(M4_DMA_TypeDef *pstcDmaReg, uint8_t u8Ch, const stc_dma_nseqb_cfg_t *pstcSrcNseqBCfg);
    en_result_t DMA_SetDesNseqCfg(M4_DMA_TypeDef *pstcDmaReg, uint8_t u8Ch, const stc_dma_nseq_cfg_t *pstDesNseqCfg);
    en_result_t DMA_SetDesNseqBCfg(M4_DMA_TypeDef *pstcDmaReg, uint8_t u8Ch, const stc_dma_nseqb_cfg_t *pstDesNseqBCfg);
    en_result_t DMA_SetLLP(M4_DMA_TypeDef *pstcDmaReg, uint8_t u8Ch, uint32_t u32Pointer);

    void DMA_SetTriggerSrc(const M4_DMA_TypeDef *pstcDmaReg, uint8_t u8Ch, en_event_src_t enSrc);
    void DMA_SetReConfigTriggerSrc(en_event_src_t enSrc);
    void DMA_ChannelCfg(M4_DMA_TypeDef *pstcDmaReg, uint8_t u8Ch, const stc_dma_ch_cfg_t *pstcChCfg);
    void DMA_InitChannel(M4_DMA_TypeDef *pstcDmaReg, uint8_t u8Ch, const stc_dma_config_t *pstcDmaCfg);
    void DMA_DeInit(M4_DMA_TypeDef *pstcDmaReg, uint8_t u8Ch);

    //@} // DmacGroup

#ifdef __cplusplus
}
#endif

#endif /* DDL_DMAC_ENABLE */

#endif /* __HC32F46X_DMAC_H__*/

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
