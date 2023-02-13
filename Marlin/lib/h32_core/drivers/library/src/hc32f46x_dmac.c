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
/** \file hc32f46x_dmac.c
 **
 ** A detailed description is available at
 ** @link DmacGroup DMAC description @endlink
 **
 **   - 2018-11-18  1.0  Chengy First version for Device Driver Library of DMAC.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32f46x_dmac.h"
#include "hc32f46x_utility.h"

#if (DDL_DMAC_ENABLE == DDL_ON)

/**
 *******************************************************************************
 ** \addtogroup DmacGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define DMA_CNT (10u)
#define DMA_IDLE (0u)
#define DMA_BUSY (1u)

#define DMACH0 (0x01u)
#define DMACH1 (0x02u)
#define DMACH2 (0x04u)
#define DMACH3 (0x08u)

#define DMATIMEOUT1 (0x5000u)
#define DMATIMEOUT2 (0x1000u)

#define DMA_CHCTL_DEFAULT (0x00001000ul)
#define DMA_DTCTL_DEFAULT (0x00000001ul)
#define DMA_DAR_DEFAULT (0x00000000ul)
#define DMA_SAR_DEFAULT (0x00000000ul)
#define DMA_RPT_DEFAULT (0x00000000ul)
#define DMA_LLP_DEFAULT (0x00000000ul)
#define DMA_SNSEQCTL_DEFAULT (0x00000000ul)
#define DMA_DNSEQCTL_DEFAULT (0x00000000ul)
#define DMA_RCFGCTL_DEFAULT (0x00000000ul)

/*****************  Bits definition for DMA_INTSTAT0 register  ****************/
#define DMA_INTSTAT0_TRNERR_Pos (0U)  /*!< DMA_INTSTAT0: TRNERR Position */
#define DMA_INTSTAT0_REQERR_Pos (16U) /*!< DMA_INTSTAT0: REQERR Position */

/*****************  Bits definition for DMA_INTSTAT1 register  ****************/
#define DMA_INTSTAT1_TC_Pos (0U)   /*!< DMA_INTSTAT1: TC Position */
#define DMA_INTSTAT1_BTC_Pos (16U) /*!< DMA_INTSTAT1: BTC Position */

/*****************  Bits definition for DMA_INTMASK0 register  ****************/
#define DMA_INTMASK0_MSKTRNERR_Pos (0U)  /*!< DMA_INTMASK0: MSKTRNERR Position */
#define DMA_INTMASK0_MSKREQERR_Pos (16U) /*!< DMA_INTMASK0: MSKREQERR Position */

/*****************  Bits definition for DMA_INTMASK1 register  ****************/
#define DMA_INTMASK1_MSKTC_Pos (0U)   /*!< DMA_INTMASK1: MSKTC Position */
#define DMA_INTMASK1_MSKBTC_Pos (16U) /*!< DMA_INTMASK1: MSKBTC Position */

/*****************  Bits definition for DMA_INTCLR0 register  *****************/
#define DMA_INTCLR0_CLRTRNERR_Pos (0U)  /*!< DMA_INTCLR0: CLRTRNERR Position */
#define DMA_INTCLR0_CLRREQERR_Pos (16U) /*!< DMA_INTCLR0: CLRREQERR Position */

/*****************  Bits definition for DMA_INTCLR1 register  *****************/
#define DMA_INTCLR1_CLRTC_Pos (0U)   /*!< DMA_INTCLR1: CLRTC Position */
#define DMA_INTCLR1_CLRBTC_Pos (16U) /*!< DMA_INTCLR1: CLRBTC Position */

/*******************  Bits definition for DMA_CHEN register  ******************/
#define DMA_CHEN_CHEN_Pos (0U) /*!< DMA_CHEN: CHEN Position */

/**************  Bits definition for DMA_DTCTLx(x=0~3) register  **************/
#define DMA_DTCTL_BLKSIZE_Pos (0ul)                              /*!< DMA_DTCTLx: BLKSIZE Position */
#define DMA_DTCTL_BLKSIZE_Msk (0x3FFul << DMA_DTCTL_BLKSIZE_Pos) /*!< DMA_DTCTLx: BLKSIZE Mask 0x000003FF */
#define DMA_DTCTL_BLKSIZE (DMA_DTCTL_BLKSIZE_Msk)

#define DMA_DTCTL_CNT_Pos (16ul)                          /*!< DMA_DTCTLx: CNT Position */
#define DMA_DTCTL_CNT_Msk (0xFFFFul << DMA_DTCTL_CNT_Pos) /*!< DMA_DTCTLx: CNT Mask 0xFFFF0000 */
#define DMA_DTCTL_CNT (DMA_DTCTL_CNT_Msk)

/***************  Bits definition for DMA_RPTx(x=0~3) register  ***************/
#define DMA_RPT_SRPT_Pos (0ul)                         /*!< DMA_RPTx: SRPT Position */
#define DMA_RPT_SRPT_Msk (0x3FFul << DMA_RPT_SRPT_Pos) /*!< DMA_RPTx: SRPT Mask 0x000003FF */
#define DMA_RPT_SRPT (DMA_RPT_SRPT_Msk)

#define DMA_RPT_DRPT_Pos (16ul)                        /*!< DMA_RPTx: DRPT Position */
#define DMA_RPT_DRPT_Msk (0x3FFul << DMA_RPT_DRPT_Pos) /*!< DMA_RPTx: DRPT Mask 0x03FF0000 */
#define DMA_RPT_DRPT (DMA_RPT_DRPT_Msk)

/***************  Bits definition for DMA_RPTBx(x=0~3) register  ***************/
#define DMA_RPTB_SRPTB_Pos (0ul)                           /*!< DMA_RPTBx: SRPTB Position */
#define DMA_RPTB_SRPTB_Msk (0x3FFul << DMA_RPTB_SRPTB_Pos) /*!< DMA_RPTBx: SRPTB Mask 0x000003FF */
#define DMA_RPTB_SRPTB (DMA_RPTB_SRPTB_Msk)

#define DMA_RPTB_DRPTB_Pos (16ul)                          /*!< DMA_RPTBx: DRPTB Position */
#define DMA_RPTB_DRPTB_Msk (0x3FFul << DMA_RPTB_DRPTB_Pos) /*!< DMA_RPTBx: DRPTB Mask 0x03FF0000 */
#define DMA_RPTB_DRPTB (DMA_RPTB_DRPTB_Msk)

/*************  Bits definition for DMA_SNSEQCTLx(x=0~3) register  ************/
#define DMA_SNSEQCTL_SOFFSET_Pos (0ul)                                   /*!< DMA_SNSEQCTLx: SOFFSET Position */
#define DMA_SNSEQCTL_SOFFSET_Msk (0xFFFFFul << DMA_SNSEQCTL_SOFFSET_Pos) /*!< DMA_SNSEQCTLx: SOFFSET Mask 0x000FFFFF */
#define DMA_SNSEQCTL_SOFFSET (DMA_SNSEQCTL_SOFFSET_Msk)

#define DMA_SNSEQCTL_SNSCNT_Pos (20ul)                               /*!< DMA_SNSEQCTLx: SNSCNT Position */
#define DMA_SNSEQCTL_SNSCNT_Msk (0xFFFul << DMA_SNSEQCTL_SNSCNT_Pos) /*!< DMA_SNSEQCTLx: SNSCNT Mask 0xFFF00000 */
#define DMA_SNSEQCTL_SNSCNT (DMA_SNSEQCTL_SNSCNT_Msk)

/*************  Bits definition for DMA_SNSEQCTLBx(x=0~3) register  ************/
#define DMA_SNSEQCTLB_SNSDIST_Pos (0ul)                                    /*!< DMA_SNSEQCTLBx: SNSDIST Position */
#define DMA_SNSEQCTLB_SNSDIST_Msk (0xFFFFFul << DMA_SNSEQCTLB_SNSDIST_Pos) /*!< DMA_SNSEQCTLBx: SNSDIST Mask 0x000FFFFF */
#define DMA_SNSEQCTLB_SNSDIST (DMA_SNSEQCTLB_SNSDIST_Msk)

#define DMA_SNSEQCTLB_SNSCNTB_Pos (20ul)                                 /*!< DMA_SNSEQCTLBx: SNSCNTB Position */
#define DMA_SNSEQCTLB_SNSCNTB_Msk (0xFFFul << DMA_SNSEQCTLB_SNSCNTB_Pos) /*!< DMA_SNSEQCTLBx: SNSCNTB Mask 0xFFF00000 */
#define DMA_SNSEQCTLB_SNSCNTB (DMA_SNSEQCTLB_SNSCNTB_Msk)

/*************  Bits definition for DMA_DNSEQCTLx(x=0~3) register  ************/
#define DMA_DNSEQCTL_DOFFSET_Pos (0ul)                                   /*!< DMA_DNSEQCTLx: DOFFSET Position */
#define DMA_DNSEQCTL_DOFFSET_Msk (0xFFFFFul << DMA_DNSEQCTL_DOFFSET_Pos) /*!< DMA_DNSEQCTLx: DOFFSET Mask 0x000FFFFF */
#define DMA_DNSEQCTL_DOFFSET (DMA_DNSEQCTL_DOFFSET_Msk)

#define DMA_DNSEQCTL_DNSCNT_Pos (20ul)                               /*!< DMA_DNSEQCTLx: DNSCNT Position */
#define DMA_DNSEQCTL_DNSCNT_Msk (0xFFFul << DMA_DNSEQCTL_DNSCNT_Pos) /*!< DMA_DNSEQCTLx: DNSCNT Mask 0xFFF00000 */
#define DMA_DNSEQCTL_DNSCNT (DMA_DNSEQCTL_DNSCNT_Msk)

/*************  Bits definition for DMA_DNSEQCTLx(x=0~3) register  ************/
#define DMA_DNSEQCTLB_DNSDIST_Pos (0ul)                                    /*!< DMA_DNSEQCTLBx: DNSDIST Position */
#define DMA_DNSEQCTLB_DNSDIST_Msk (0xFFFFFul << DMA_DNSEQCTLB_DNSDIST_Pos) /*!< DMA_DNSEQCTLBx: DNSDIST Mask 0x000FFFFF */
#define DMA_DNSEQCTLB_DNSDIST (DMA_DNSEQCTLB_DNSDIST_Msk)

#define DMA_DNSEQCTLB_DNSCNTB_Pos (20ul)                                 /*!< DMA_DNSEQCTLBx: DNSCNTB Position */
#define DMA_DNSEQCTLB_DNSCNTB_Msk (0xFFFul << DMA_DNSEQCTLB_DNSCNTB_Pos) /*!< DMA_DNSEQCTLBx: DNSCNTB Mask 0xFFF00000 */
#define DMA_DNSEQCTLB_DNSCNTB (DMA_DNSEQCTLB_DNSCNTB_Msk)

/****************  Bits definition for DMA_LLPx(x=0~7) register  **************/
#define DMA_LLP_LLP_Pos (2ul)                             /*!< DMA_LLPx: LLP Position */
#define DMA_LLP_LLP_Msk (0x3FFFFFFFul << DMA_LLP_LLP_Pos) /*!< DMA_LLPx: LLP Mask 0xFFFFFFC */
#define DMA_LLP_LLP (DMA_LLP_LLP_Msk)

/***************  Bits definition for DMA_CHxCTL(x=0~3) register  *************/
#define DMA_CHCTL_SINC_Pos (0ul)                         /*!< DMA_CHxCTL: SINC Position */
#define DMA_CHCTL_SINC_Msk (0x3ul << DMA_CHCTL_SINC_Pos) /*!< DMA_CHxCTL: SINC Mask 0x00000003 */
#define DMA_CHCTL_SINC (DMA_CHCTL_SINC_Msk)

#define DMA_CHCTL_DINC_Pos (2ul)                         /*!< DMA_CHxCTL: DINC Position */
#define DMA_CHCTL_DINC_Msk (0x3ul << DMA_CHCTL_DINC_Pos) /*!< DMA_CHxCTL: DINC Mask 0x0000000C */
#define DMA_CHCTL_DINC (DMA_CHCTL_DINC_Msk)

#define DMA_CHCTL_SRPTEN_Pos (4ul)                           /*!< DMA_CHxCTL: SRPTEN Position */
#define DMA_CHCTL_SRPTEN_Msk (0x1ul << DMA_CHCTL_SRPTEN_Pos) /*!< DMA_CHxCTL: SRPTEN Mask 0x00000010 */
#define DMA_CHCTL_SRPTEN (DMA_CHCTL_SRPTEN_Msk)

#define DMA_CHCTL_DRPTEN_Pos (5ul)                           /*!< DMA_CHxCTL: DRPTEN Position */
#define DMA_CHCTL_DRPTEN_Msk (0x1ul << DMA_CHCTL_DRPTEN_Pos) /*!< DMA_CHxCTL: DRPTEN Mask 0x00000020 */
#define DMA_CHCTL_DRPTEN (DMA_CHCTL_DRPTEN_Msk)

#define DMA_CHCTL_SNSEQEN_Pos (6ul)                            /*!< DMA_CHxCTL: SNSEQEN Position */
#define DMA_CHCTL_SNSEQEN_Msk (0x1ul << DMA_CHCTL_SNSEQEN_Pos) /*!< DMA_CHxCTL: SNSEQEN Mask 0x00000040 */
#define DMA_CHCTL_SNSEQEN (DMA_CHCTL_SNSEQEN_Msk)

#define DMA_CHCTL_DNSEQEN_Pos (7ul)                            /*!< DMA_CHxCTL: DNSEQEN Position */
#define DMA_CHCTL_DNSEQEN_Msk (0x1ul << DMA_CHCTL_DNSEQEN_Pos) /*!< DMA_CHxCTL: DNSEQEN Mask 0x00000080 */
#define DMA_CHCTL_DNSEQEN (DMA_CHCTL_DNSEQEN_Msk)

#define DMA_CHCTL_HSIZE_Pos (8ul)                          /*!< DMA_CHxCTL: HSIZE Position */
#define DMA_CHCTL_HSIZE_Msk (0x3ul << DMA_CHCTL_HSIZE_Pos) /*!< DMA_CHxCTL: HSIZE Mask 0x00000300 */
#define DMA_CHCTL_HSIZE (DMA_CHCTL_HSIZE_Msk)

#define DMA_CHCTL_LLPEN_Pos (10ul)                         /*!< DMA_CHxCTL: LLPEN Position */
#define DMA_CHCTL_LLPEN_Msk (0x1ul << DMA_CHCTL_LLPEN_Pos) /*!< DMA_CHxCTL: LLPEN Mask 0x00000400 */
#define DMA_CHCTL_LLPEN (DMA_CHCTL_LLPEN_Msk)

#define DMA_CHCTL_LLPRUN_Pos (11ul)                          /*!< DMA_CHxCTL: LLPRUN Position */
#define DMA_CHCTL_LLPRUN_Msk (0x1ul << DMA_CHCTL_LLPRUN_Pos) /*!< DMA_CHxCTL: LLPRUN Mask 0x00000800 */
#define DMA_CHCTL_LLPRUN (DMA_CHCTL_LLPRUN_Msk)

#define DMA_CHCTL_IE_Pos (12ul)                      /*!< DMA_CHxCTL: IE Position */
#define DMA_CHCTL_IE_Msk (0x1ul << DMA_CHCTL_IE_Pos) /*!< DMA_CHxCTL: IE Mask 0x00001000 */
#define DMA_CHCTL_IE (DMA_CHCTL_IE_Msk)

/***********************  DMA REGISTERx(x=0~3) register  **********************/
#define _DMA_CH_REG_OFFSET(ch) ((ch)*0x40ul)
#define _DMA_CH_REG(reg_base, ch) (*(volatile uint32_t *)((uint32_t)(reg_base) + _DMA_CH_REG_OFFSET(ch)))

#define WRITE_DMA_CH_REG(reg_base, ch, val) (_DMA_CH_REG((reg_base), (ch)) = (val))
#define READ_DMA_CH_REG(reg_base, ch) (_DMA_CH_REG((reg_base), (ch)))

#define SET_DMA_CH_REG_BIT(reg_base, ch, pos) (_DMA_CH_REG((reg_base), (ch)) |= (1ul << (pos)))
#define CLR_DMA_CH_REG_BIT(reg_base, ch, pos) (_DMA_CH_REG((reg_base), (ch)) &= (~(1ul << (pos))))

#define WRITE_DMA_CH_TRGSEL(reg_base, ch, val) ((*(volatile uint32_t *)((uint32_t)(reg_base) + (ch)*4ul)) = (val))

#define MODIFY_DMA_CH_REG(reg_base, ch, msk, val)                                                                          \
    {                                                                                                                      \
        do                                                                                                                 \
        {                                                                                                                  \
            WRITE_DMA_CH_REG((reg_base), (ch), ((READ_DMA_CH_REG((reg_base), (ch)) & (~(msk))) | ((val) << (msk##_Pos)))); \
        } while (0);                                                                                                       \
    }

/*! Parameter valid check for Dmac register pointer. */
#define IS_VALID_DMA_REG(x) \
    ((M4_DMA1 == (x)) ||    \
     (M4_DMA2 == (x)))

/*! Parameter valid check for Dmac Channel. */
#define IS_VALID_CH(x)  \
    ((DmaCh0 == (x)) || \
     (DmaCh1 == (x)) || \
     (DmaCh2 == (x)) || \
     (DmaCh3 == (x)))

/*! Parameter valid check for Dmac irq selection. */
#define IS_VALID_IRQ_SEL(x)   \
    ((TrnErrIrq == (x)) ||    \
     (TrnReqErrIrq == (x)) || \
     (TrnCpltIrq == (x)) ||   \
     (BlkTrnCpltIrq == (x)))

/*! Parameter valid check for Dmac re_config count mode. */
#define IS_VALID_CNT_MODE(x) \
    ((CntFix == (x)) ||      \
     (CntSrcAddr == (x)) ||  \
     (CntDesAddr == (x)))

/*! Parameter valid check for Dmac re_config source address mode. */
#define IS_VALID_SADDR_MODE(x) \
    ((SaddrFix == (x)) ||      \
     (SaddrNseq == (x)) ||     \
     (SaddrRep == (x)))

/*! Parameter valid check for Dmac re_config destination address mode. */
#define IS_VALID_DADDR_MODE(x) \
    ((DaddrFix == (x)) ||      \
     (DaddrNseq == (x)) ||     \
     (DaddrRep == (x)))

/*! Parameter valid check for Dmac status. */
#define IS_VALID_DMA_STA(x) \
    ((DmaSta == (x)) ||     \
     (ReCfgSta == (x)) ||   \
     (DmaCh0Sta == (x)) ||  \
     (DmaCh1Sta == (x)) ||  \
     (DmaCh2Sta == (x)) ||  \
     (DmaCh3Sta == (x)))

/*! Parameter valid check for Dmac transfer data width. */
#define IS_VALID_TRN_WIDTH(x) \
    ((Dma8Bit == (x)) ||      \
     (Dma16Bit == (x)) ||     \
     (Dma32Bit == (x)))

/*! Parameter valid check for Dmac address mode. */
#define IS_VALID_ADDR_MODE(x)    \
    ((AddressFix == (x)) ||      \
     (AddressIncrease == (x)) || \
     (AddressDecrease == (x)))

/*! Parameter valid check for Dmac link-list-pointer mode. */
#define IS_VALID_LLP_MODE(x)    \
    ((LlpWaitNextReq == (x)) || \
     (LlpRunNow == (x)))

/*! Parameter valid check for Dmac transfer block size. */
#define IS_VALID_BLKSIZE(x) \
    (!((x) & (uint16_t)(~(DMA_DTCTL_BLKSIZE_Msk >> DMA_DTCTL_BLKSIZE_Pos))))

/*! Parameter valid check for Dmac transfer count. */
#define IS_VALID_TRNCNT(x) \
    (!((x) & ~(DMA_DTCTL_CNT_Msk >> DMA_DTCTL_CNT_Pos)))

/*! Parameter valid check for Dmac source repeat size. */
#define IS_VALID_SRPT_SIZE(x) \
    (!((x) & ~(DMA_RPT_SRPT_Msk >> DMA_RPT_SRPT_Pos)))

/*! Parameter valid check for Dmac destination repeat size. */
#define IS_VALID_DRPT_SIZE(x) \
    (!((x) & ~(DMA_RPT_DRPT_Msk >> DMA_RPT_DRPT_Pos)))

/*! Parameter valid check for Dmac source repeatB size. */
#define IS_VALID_SRPTB_SIZE(x) \
    (!((x) & ~(DMA_RPTB_SRPTB_Msk >> DMA_RPTB_SRPTB_Pos)))

/*! Parameter valid check for Dmac destinationB repeat size. */
#define IS_VALID_DRPTB_SIZE(x) \
    (!((x) & ~(DMA_RPTB_DRPTB_Msk >> DMA_RPTB_DRPTB_Pos)))

/*! Parameter valid check for Dmac source no-sequence count. */
#define IS_VALID_SNSCNT(x) \
    (!((x) & ~(DMA_SNSEQCTL_SNSCNT_Msk >> DMA_SNSEQCTL_SNSCNT_Pos)))

/*! Parameter valid check for Dmac source no-sequence offset. */
#define IS_VALID_SNSOFFSET(x) \
    (!((x) & ~(DMA_SNSEQCTL_SOFFSET_Msk >> DMA_SNSEQCTL_SOFFSET_Pos)))

/*! Parameter valid check for Dmac source no-sequence countB. */
#define IS_VALID_SNSCNTB(x) \
    (!((x) & ~(DMA_SNSEQCTLB_SNSCNTB_Msk >> DMA_SNSEQCTLB_SNSCNTB_Pos)))

/*! Parameter valid check for Dmac source no-sequence district interval. */
#define IS_VALID_SNSDIST(x) \
    (!((x) & ~(DMA_SNSEQCTLB_SNSDIST_Msk >> DMA_SNSEQCTLB_SNSDIST_Pos)))

/*! Parameter valid check for Dmac destination no-sequence count. */
#define IS_VALID_DNSCNT(x) \
    (!((x) & ~(DMA_DNSEQCTL_DNSCNT_Msk >> DMA_DNSEQCTL_DNSCNT_Pos)))

/*! Parameter valid check for Dmac destination no-sequence offset. */
#define IS_VALID_DNSOFFSET(x) \
    (!((x) & ~(DMA_DNSEQCTL_DOFFSET_Msk >> DMA_DNSEQCTL_DOFFSET_Pos)))

/*! Parameter valid check for Dmac destination no-sequence countB. */
#define IS_VALID_DNSCNTB(x) \
    (!((x) & ~(DMA_DNSEQCTLB_DNSCNTB_Msk >> DMA_DNSEQCTLB_DNSCNTB_Pos)))

/*! Parameter valid check for Dmac destination no-sequence district interval. */
#define IS_VALID_DNSDIST(x) \
    (!((x) & ~(DMA_DNSEQCTLB_DNSDIST_Msk >> DMA_DNSEQCTLB_DNSDIST_Pos)))

/*! Parameter valid check for Dmac link-list-pointer. */
#define IS_VALID_LLP(x) (!((x) & ~DMA_LLP_LLP_Msk))

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static volatile uint8_t DmaChEnState = DMA_IDLE;

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 *******************************************************************************
 ** \brief  Enable or disable the dma.
 **
 ** \param  [in] pstcDmaReg             The pointer to dma register
 ** \arg    M4_DMA1                     DMAC unit 1 register
 ** \arg    M4_DMA2                     DMAC unit 2 register
 **
 ** \param  [in] enNewState             The new state of dma
 ** \arg    Enable                      Enable dma.
 ** \arg    Disable                     Disable dma.
 **
 ** \retval None
 **
 ** \note   None
 **
 ******************************************************************************/
void DMA_Cmd(M4_DMA_TypeDef *pstcDmaReg, en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_VALID_DMA_REG(pstcDmaReg));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    pstcDmaReg->EN_f.EN = enNewState;
}

/**
 *******************************************************************************
 ** \brief  Enable the specified dma interrupt.
 **
 ** \param  [in] pstcDmaReg             The pointer to dma register
 ** \arg    M4_DMA1                     DMAC unit 1 register
 ** \arg    M4_DMA2                     DMAC unit 2 register
 **
 ** \param  [in] u8Ch                   The specified dma channel.
 ** \param  [in] enIrqSel               The specified dma flag.
 ** \arg    TrnErrIrq                   The DMA transfer error interrupt.
 ** \arg    TrnReqErrIrq                DMA transfer req over error interrupt.
 ** \arg    TrnCpltIrq                  DMA transfer completion interrupt.
 ** \arg    BlkTrnCpltIrq               DMA block completion interrupt.
 **
 ** \retval Ok                          Interrupt enabled normally.
 ** \retval ErrorInvalidParameter       u8Ch or enIrqSel is invalid.
 **
 ** \note   None
 **
 ******************************************************************************/
en_result_t DMA_EnableIrq(M4_DMA_TypeDef *pstcDmaReg,
                          uint8_t u8Ch,
                          en_dma_irq_sel_t enIrqSel)
{
    en_result_t enRet = Ok;

    DDL_ASSERT(IS_VALID_DMA_REG(pstcDmaReg));
    DDL_ASSERT(IS_VALID_CH(u8Ch));
    DDL_ASSERT(IS_VALID_IRQ_SEL(enIrqSel));

    if (!IS_VALID_CH(u8Ch))
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        switch (enIrqSel)
        {
        case TrnErrIrq:
            pstcDmaReg->INTMASK0 &= ~(1ul << (u8Ch + DMA_INTMASK0_MSKTRNERR_Pos));
            break;
        case TrnReqErrIrq:
            pstcDmaReg->INTMASK0 &= ~(1ul << (u8Ch + DMA_INTMASK0_MSKREQERR_Pos));
            break;
        case TrnCpltIrq:
            pstcDmaReg->INTMASK1 &= ~(1ul << (u8Ch + DMA_INTMASK1_MSKTC_Pos));
            break;
        case BlkTrnCpltIrq:
            pstcDmaReg->INTMASK1 &= ~(1ul << (u8Ch + DMA_INTMASK1_MSKBTC_Pos));
            break;
        default:
            enRet = ErrorInvalidParameter;
            break;
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief  Enable the specified dma interrupt.
 **
 ** \param  [in] pstcDmaReg             The pointer to dma register
 ** \arg    M4_DMA1                     DMAC unit 1 register
 ** \arg    M4_DMA2                     DMAC unit 2 register
 **
 ** \param  [in] u8Ch                   The specified dma channel.
 ** \param  [in] enIrqSel               The specified dma flag.
 ** \arg    TrnErrIrq                   The DMA transfer error interrupt.
 ** \arg    TrnReqErrIrq                DMA transfer req over error interrupt.
 ** \arg    TrnCpltIrq                  DMA transfer completion interrupt.
 ** \arg    BlkTrnCpltIrq               DMA block completion interrupt.
 **
 ** \retval Ok                          Interrupt disabled normally.
 ** \retval ErrorInvalidParameter       u8Ch or enIrqSel is invalid.
 **
 ** \note   None
 **
 ******************************************************************************/
en_result_t DMA_DisableIrq(M4_DMA_TypeDef *pstcDmaReg,
                           uint8_t u8Ch,
                           en_dma_irq_sel_t enIrqSel)
{
    en_result_t enRet = Ok;

    DDL_ASSERT(IS_VALID_DMA_REG(pstcDmaReg));
    DDL_ASSERT(IS_VALID_CH(u8Ch));
    DDL_ASSERT(IS_VALID_IRQ_SEL(enIrqSel));

    if (!IS_VALID_CH(u8Ch))
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        switch (enIrqSel)
        {
        case TrnErrIrq:
            pstcDmaReg->INTMASK0 |= (1ul << (u8Ch + DMA_INTMASK0_MSKTRNERR_Pos));
            break;
        case TrnReqErrIrq:
            pstcDmaReg->INTMASK0 |= (1ul << (u8Ch + DMA_INTMASK0_MSKREQERR_Pos));
            break;
        case TrnCpltIrq:
            pstcDmaReg->INTMASK1 |= (1ul << (u8Ch + DMA_INTMASK1_MSKTC_Pos));
            break;
        case BlkTrnCpltIrq:
            pstcDmaReg->INTMASK1 |= (1ul << (u8Ch + DMA_INTMASK1_MSKBTC_Pos));
            break;
        default:
            enRet = ErrorInvalidParameter;
            break;
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief  Get the specified dma interrupt flag status.
 **
 ** \param  [in] pstcDmaReg             The pointer to dma register
 ** \arg    M4_DMA1                     DMAC unit 1 register
 ** \arg    M4_DMA2                     DMAC unit 2 register
 **
 ** \param  [in] u8Ch                   The specified dma channel.
 ** \param  [in] enIrqSel               The specified dma flag.
 ** \arg    TrnErrIrq                   The DMA transfer error interrupt.
 ** \arg    TrnReqErrIrq                DMA transfer req over error interrupt.
 ** \arg    TrnCpltIrq                  DMA transfer completion interrupt.
 ** \arg    BlkTrnCpltIrq               DMA block completion interrupt.
 **
 ** \retval the specified dma flag status
 **
 ** \note   None
 **
 ******************************************************************************/
en_flag_status_t DMA_GetIrqFlag(M4_DMA_TypeDef *pstcDmaReg, uint8_t u8Ch, en_dma_irq_sel_t enIrqSel)
{
    uint32_t u32IntStat = 0ul;

    DDL_ASSERT(IS_VALID_DMA_REG(pstcDmaReg));
    DDL_ASSERT(IS_VALID_CH(u8Ch));
    DDL_ASSERT(IS_VALID_IRQ_SEL(enIrqSel));

    switch (enIrqSel)
    {
    case TrnErrIrq:
        u32IntStat = (pstcDmaReg->INTSTAT0 & (1ul << (u8Ch + DMA_INTSTAT0_TRNERR_Pos)));
        break;
    case TrnReqErrIrq:
        u32IntStat = (pstcDmaReg->INTSTAT0 & (1ul << (u8Ch + DMA_INTSTAT0_REQERR_Pos)));
        break;
    case TrnCpltIrq:
        u32IntStat = (pstcDmaReg->INTSTAT1 & (1ul << (u8Ch + DMA_INTSTAT1_TC_Pos)));
        break;
    case BlkTrnCpltIrq:
        u32IntStat = (pstcDmaReg->INTSTAT1 & (1ul << (u8Ch + DMA_INTSTAT1_BTC_Pos)));
        break;
    default:
        break;
    }

    return (u32IntStat ? Set : Reset);
}

/**
 *******************************************************************************
 ** \brief  Clear the specified dma interrupt.
 **
 ** \param  [in] pstcDmaReg             The pointer to dma register
 ** \arg    M4_DMA1                     DMAC unit 1 register
 ** \arg    M4_DMA2                     DMAC unit 2 register
 **
 ** \param  [in] u8Ch                   The specified dma channel.
 ** \param  [in] enIrqSel               The specified dma flag.
 ** \arg    TrnErrIrq                   The DMA transfer error interrupt.
 ** \arg    TrnReqErrIrq                DMA transfer req over error interrupt.
 ** \arg    TrnCpltIrq                  DMA transfer completion interrupt.
 ** \arg    BlkTrnCpltIrq               DMA block completion interrupt.
 **
 ** \retval Ok                          Clear flag successfully.
 ** \retval ErrorInvalidParameter       u8Ch or enIrqSel is invalid.
 **
 ** \note   None
 **
 ******************************************************************************/
en_result_t DMA_ClearIrqFlag(M4_DMA_TypeDef *pstcDmaReg, uint8_t u8Ch, en_dma_irq_sel_t enIrqSel)
{
    en_result_t enRet = Ok;

    DDL_ASSERT(IS_VALID_DMA_REG(pstcDmaReg));
    DDL_ASSERT(IS_VALID_CH(u8Ch));
    DDL_ASSERT(IS_VALID_IRQ_SEL(enIrqSel));

    if (!IS_VALID_CH(u8Ch))
    {
        enRet = ErrorInvalidParameter;
    }
    else
    {
        switch (enIrqSel)
        {
        case TrnErrIrq:
            pstcDmaReg->INTCLR0 |= (1ul << (u8Ch + DMA_INTCLR0_CLRTRNERR_Pos));
            break;
        case TrnReqErrIrq:
            pstcDmaReg->INTCLR0 |= (1ul << (u8Ch + DMA_INTCLR0_CLRREQERR_Pos));
            break;
        case TrnCpltIrq:
            pstcDmaReg->INTCLR1 |= (1ul << (u8Ch + DMA_INTCLR1_CLRTC_Pos));
            break;
        case BlkTrnCpltIrq:
            pstcDmaReg->INTCLR1 |= (1ul << (u8Ch + DMA_INTCLR1_CLRBTC_Pos));
            break;
        default:
            enRet = ErrorInvalidParameter;
            break;
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief  Enable or disable the specified dma channel.
 **
 ** \param  [in] pstcDmaReg             The pointer to dma register
 ** \arg    M4_DMA1                     DMAC unit 1 register
 ** \arg    M4_DMA2                     DMAC unit 2 register
 **
 ** \param  [in] u8Ch                   The specified dma channel.
 ** \param  [in] enNewState             The new state of dma
 ** \arg    Enable                      Enable dma.
 ** \arg    Disable                     Disable dma.
 **
 ** \retval None
 **
 ** \note   None
 **
 ******************************************************************************/
en_result_t DMA_ChannelCmd(M4_DMA_TypeDef *pstcDmaReg, uint8_t u8Ch, en_functional_state_t enNewState)
{
    uint16_t u16Timeout = 0u;
    uint32_t u32Temp = 0u;
    uint32_t u32Cnt;

    DDL_ASSERT(IS_VALID_DMA_REG(pstcDmaReg));
    DDL_ASSERT(IS_VALID_CH(u8Ch));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    if (DMA_IDLE == DmaChEnState)
    {
        DmaChEnState = DMA_BUSY;

        /* Read back channel enable register except current channel */
        u32Temp = (pstcDmaReg->CHEN & (~(1ul << u8Ch)));
        if (0ul != u32Temp)
        {
            if (((pstcDmaReg->CHEN & 0x01ul) == 0x01ul) && (u8Ch != DmaCh0))
            {
                u32Cnt = pstcDmaReg->DTCTL0_f.CNT;
                if (pstcDmaReg->MONDTCTL0_f.CNT > DMA_CNT)
                {
                    /* not wait. */
                }
                else if (pstcDmaReg->MONDTCTL0_f.CNT < u32Cnt)
                {
                    while (Reset != (pstcDmaReg->CHEN & 0x01ul))
                    {
                        u16Timeout++;
                        if (u16Timeout > DMATIMEOUT1)
                        {
                            DmaChEnState = DMA_IDLE;
                            return ErrorTimeout;
                        }
                    }
                }
            }
            if (((pstcDmaReg->CHEN & 0x02ul) == 0x02ul) && (u8Ch != DmaCh1))
            {
                u32Cnt = pstcDmaReg->DTCTL1_f.CNT;
                if (pstcDmaReg->MONDTCTL1_f.CNT > DMA_CNT)
                {
                    /* not wait. */
                }
                else if (pstcDmaReg->MONDTCTL1_f.CNT < u32Cnt)
                {
                    u16Timeout = 0u;
                    while (Reset != (pstcDmaReg->CHEN & 0x02ul))
                    {
                        u16Timeout++;
                        if (u16Timeout > DMATIMEOUT1)
                        {
                            DmaChEnState = DMA_IDLE;
                            return ErrorTimeout;
                        }
                    }
                }
            }
            if (((pstcDmaReg->CHEN & 0x04ul) == 0x04ul) && (u8Ch != DmaCh2))
            {
                u16Timeout = 0u;
                u32Cnt = pstcDmaReg->DTCTL2_f.CNT;
                if (pstcDmaReg->MONDTCTL2_f.CNT > DMA_CNT)
                {
                    /* not wait. */
                }
                else if (pstcDmaReg->MONDTCTL2_f.CNT < u32Cnt)
                {
                    while (Reset != (pstcDmaReg->CHEN & 0x04ul))
                    {
                        u16Timeout++;
                        if (u16Timeout > DMATIMEOUT1)
                        {
                            DmaChEnState = DMA_IDLE;
                            return ErrorTimeout;
                        }
                    }
                }
            }
            if (((pstcDmaReg->CHEN & 0x08ul) == 0x08ul) && (u8Ch != DmaCh3))
            {
                u16Timeout = 0u;
                u32Cnt = pstcDmaReg->DTCTL3_f.CNT;
                if (pstcDmaReg->MONDTCTL3_f.CNT > DMA_CNT)
                {
                    /* not wait. */
                }
                else if (pstcDmaReg->MONDTCTL3_f.CNT < u32Cnt)
                {
                    while (Reset != (pstcDmaReg->CHEN & 0x08ul))
                    {
                        u16Timeout++;
                        if (u16Timeout > DMATIMEOUT1)
                        {
                            DmaChEnState = DMA_IDLE;
                            return ErrorTimeout;
                        }
                    }
                }
            }
        }

        switch (enNewState)
        {
        case Enable:
            pstcDmaReg->CHEN |= (1ul << (u8Ch + DMA_CHEN_CHEN_Pos)) & 0x0fu;
            break;
        case Disable:
            pstcDmaReg->CHEN &= (~(1ul << (u8Ch + DMA_CHEN_CHEN_Pos))) & 0x0fu;
            break;
        }

        DmaChEnState = DMA_IDLE;
        return Ok;
    }

    return Error;
}

/**
 *******************************************************************************
 ** \brief  DMA Re_Config control configuration.
 **
 ** \param  [in] pstcDmaReg             The pointer to dma register
 ** \arg    M4_DMA1                     DMAC unit 1 register
 ** \arg    M4_DMA2                     DMAC unit 2 register
 **
 ** \param  [in] u8Ch                   The specified dma channel.
 ** \param  [in] pstcDmaReCfg           The configuration struct of DMA.
 ** \arg    u16SrcRptBSize              The source repeat size.
 ** \arg    u16SrcRptBSize;             The source repeat size.
 ** \arg    u16DesRptBSize;             The destination repeat size.
 ** \arg    enReCfgLlp;                 Enable llp re_config function or not.
 ** \arg    enReCfgCh;                  DMA re_config channel.
 ** \arg    enSaddrMd;                  DMA re_config source address mode.
 ** \arg    enDaddrMd;                  DMA re_config destination address mode.
 ** \arg    enCntMd;                    DMA re_config count mode.
 ** \arg    stcSrcNseqBCfg;             The source no_sequence re_config.
 ** \arg    stcDesNseqBCfg;             The destination no_sequence re_config.
 **
 ** \retval None
 **
 ** \note   This function should be used while DMA disable.
 **
 ******************************************************************************/
void DMA_InitReConfig(M4_DMA_TypeDef *pstcDmaReg, uint8_t u8Ch,
                      const stc_dma_recfg_ctl_t *pstcDmaReCfg)
{
    DDL_ASSERT(IS_VALID_DMA_REG(pstcDmaReg));
    DDL_ASSERT(IS_VALID_CH(u8Ch));
    DDL_ASSERT(IS_VALID_CNT_MODE(pstcDmaReCfg->enCntMd));
    DDL_ASSERT(IS_VALID_DADDR_MODE(pstcDmaReCfg->enDaddrMd));
    DDL_ASSERT(IS_VALID_SADDR_MODE(pstcDmaReCfg->enSaddrMd));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcDmaReCfg->enReCfgLlp));

    pstcDmaReg->RCFGCTL_f.RCFGLLP = pstcDmaReCfg->enReCfgLlp;
    pstcDmaReg->RCFGCTL_f.SARMD = pstcDmaReCfg->enSaddrMd;
    pstcDmaReg->RCFGCTL_f.DARMD = pstcDmaReCfg->enDaddrMd;
    pstcDmaReg->RCFGCTL_f.CNTMD = pstcDmaReCfg->enCntMd;

    /* Set DMA source repeat size B. */
    MODIFY_DMA_CH_REG(&pstcDmaReg->RPTB0, u8Ch, DMA_RPT_SRPT, (uint32_t)pstcDmaReCfg->u16SrcRptBSize);
    /* Set DMA destination repeat size B. */
    MODIFY_DMA_CH_REG(&pstcDmaReg->RPTB0, u8Ch, DMA_RPT_DRPT, (uint32_t)pstcDmaReCfg->u16DesRptBSize);
    /* Set DMA source no_sequence B. */
    MODIFY_DMA_CH_REG(&pstcDmaReg->SNSEQCTLB0, u8Ch, DMA_SNSEQCTL_SOFFSET, pstcDmaReCfg->stcSrcNseqBCfg.u32Offset);
    MODIFY_DMA_CH_REG(&pstcDmaReg->SNSEQCTLB0, u8Ch, DMA_SNSEQCTL_SNSCNT, (uint32_t)pstcDmaReCfg->stcSrcNseqBCfg.u16Cnt);
    /* Set DMA destination no_sequence B. */
    MODIFY_DMA_CH_REG(&pstcDmaReg->DNSEQCTLB0, u8Ch, DMA_DNSEQCTL_DOFFSET, pstcDmaReCfg->stcDesNseqBCfg.u32Offset);
    MODIFY_DMA_CH_REG(&pstcDmaReg->DNSEQCTLB0, u8Ch, DMA_DNSEQCTL_DNSCNT, (uint32_t)pstcDmaReCfg->stcDesNseqBCfg.u16Cnt);
}

/**
 *******************************************************************************
 ** \brief  Disable or enable DMA Re_Config.
 **
 ** \param  [in] pstcDmaReg             The pointer to dma register
 ** \arg    M4_DMA1                     DMAC unit 1 register
 ** \arg    M4_DMA2                     DMAC unit 2 register
 **
 ** \param  [in] enNewState             The new state of dma
 ** \arg    Enable                      Enable dma.
 ** \arg    Disable                     Disable dma.
 **
 ** \retval None
 **
 ** \note   None
 **
 ******************************************************************************/
void DMA_ReCfgCmd(M4_DMA_TypeDef *pstcDmaReg, en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_VALID_DMA_REG(pstcDmaReg));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    pstcDmaReg->RCFGCTL_f.RCFGEN = enNewState;
}

/**
 *******************************************************************************
 ** \brief  Get the specified dma flag status.
 **
 ** \param  [in] pstcDmaReg             The pointer to dma register
 ** \arg    M4_DMA1                     DMAC unit 1 register
 ** \arg    M4_DMA2                     DMAC unit 2 register
 **
 ** \param  [in] enDmaChFlag            The specified dma flag.
 ** \arg    DmaSta                      The DMA status.
 ** \arg    ReCfgSta                    The DMA re_config stauts.
 ** \arg    DmaCh0Sta                   The DMA channel 0 status.
 ** \arg    DmaCh1Sta                   The DMA channel 1 status.
 ** \arg    DmaCh2Sta                   The DMA channel 2 status.
 ** \arg    DmaCh3Sta                   The DMA channel 3 status.
 **
 ** \retval the specified dma flag status
 **
 ** \note   None
 **
 ******************************************************************************/
en_flag_status_t DMA_GetChFlag(M4_DMA_TypeDef *pstcDmaReg, en_dma_ch_flag_t enDmaChFlag)
{
    uint32_t u32IntStat = 0ul;

    DDL_ASSERT(IS_VALID_DMA_REG(pstcDmaReg));
    DDL_ASSERT(IS_VALID_DMA_STA(enDmaChFlag));

    switch (enDmaChFlag)
    {
    case DmaSta:
        u32IntStat = pstcDmaReg->CHSTAT_f.DMAACT;
        break;
    case ReCfgSta:
        u32IntStat = pstcDmaReg->CHSTAT_f.RCFGACT;
        break;
    case DmaCh0Sta:
        u32IntStat = (pstcDmaReg->CHSTAT_f.CHACT & DMACH0);
        break;
    case DmaCh1Sta:
        u32IntStat = (pstcDmaReg->CHSTAT_f.CHACT & DMACH1);
        break;
    case DmaCh2Sta:
        u32IntStat = (pstcDmaReg->CHSTAT_f.CHACT & DMACH2);
        break;
    case DmaCh3Sta:
        u32IntStat = (pstcDmaReg->CHSTAT_f.CHACT & DMACH3);
        break;
    default:
        break;
    }
    return (u32IntStat ? Set : Reset);
}

/**
 *******************************************************************************
 ** \brief  Set the source address of the specified dma channel.
 **
 ** \param  [in] pstcDmaReg             The pointer to dma register
 ** \arg    M4_DMA1                     DMAC unit 1 registers
 ** \arg    M4_DMA2                     DMAC unit 2 registers
 **
 ** \param  [in] u8Ch                   The specified dma channel.
 ** \param  [in] u32Address             The source address.
 **
 ** \retval None
 **
 ** \note   None
 **
 ******************************************************************************/
en_result_t DMA_SetSrcAddress(M4_DMA_TypeDef *pstcDmaReg, uint8_t u8Ch, uint32_t u32Address)
{
    uint16_t u16Timeout = 0u;
    en_result_t enRet = Ok;

    DDL_ASSERT(IS_VALID_DMA_REG(pstcDmaReg));
    DDL_ASSERT(IS_VALID_CH(u8Ch));

    WRITE_DMA_CH_REG(&pstcDmaReg->SAR0, u8Ch, u32Address);

    /* Ensure the address has been writed */
    while (u32Address != READ_DMA_CH_REG(&pstcDmaReg->MONSAR0, u8Ch))
    {
        u16Timeout++;
        if (u16Timeout > DMATIMEOUT2)
        {
            enRet = ErrorTimeout;
        }
        else
        {
            WRITE_DMA_CH_REG(&pstcDmaReg->SAR0, u8Ch, u32Address);
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief  Set the destination address of the specified dma channel.
 **
 ** \param  [in] pstcDmaReg             The pointer to dma register
 ** \arg    M4_DMA1                     DMAC unit 1 registers
 ** \arg    M4_DMA2                     DMAC unit 2 registers
 **
 ** \param  [in] u8Ch                   The specified dma channel.
 ** \param  [in] u32Address             The destination address.
 **
 ** \retval None
 **
 ** \note   None
 **
 ******************************************************************************/
en_result_t DMA_SetDesAddress(M4_DMA_TypeDef *pstcDmaReg, uint8_t u8Ch, uint32_t u32Address)
{
    uint16_t u16Timeout = 0u;
    en_result_t enRet = Ok;

    DDL_ASSERT(IS_VALID_DMA_REG(pstcDmaReg));
    DDL_ASSERT(IS_VALID_CH(u8Ch));

    WRITE_DMA_CH_REG(&pstcDmaReg->DAR0, u8Ch, u32Address);

    /* Ensure the address has been writed */
    while (u32Address != READ_DMA_CH_REG(&pstcDmaReg->MONDAR0, u8Ch))
    {
        u16Timeout++;
        if (u16Timeout > DMATIMEOUT2)
        {
            enRet = ErrorTimeout;
        }
        else
        {
            WRITE_DMA_CH_REG(&pstcDmaReg->DAR0, u8Ch, u32Address);
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief  Set the block size of the specified dma channel.
 **
 ** \param  [in] pstcDmaReg             The pointer to dma register
 ** \arg    M4_DMA1                     DMAC unit 1 registers
 ** \arg    M4_DMA2
 **
 ** \param  [in] u8Ch                   The specified dma channel.
 ** \param  [in] u16BlkSize             The block size.
 **
 ** \retval None.
 **
 ** \note   None
 **
 ******************************************************************************/
en_result_t DMA_SetBlockSize(M4_DMA_TypeDef *pstcDmaReg, uint8_t u8Ch, uint16_t u16BlkSize)
{
    uint16_t u16Timeout = 0u;
    en_result_t enRet = Ok;

    DDL_ASSERT(IS_VALID_DMA_REG(pstcDmaReg));
    DDL_ASSERT(IS_VALID_CH(u8Ch));
    DDL_ASSERT(IS_VALID_BLKSIZE(u16BlkSize));

    MODIFY_DMA_CH_REG(&pstcDmaReg->DTCTL0, u8Ch, DMA_DTCTL_BLKSIZE, (uint32_t)u16BlkSize);

    /* Ensure the block size has been writed */
    while (u16BlkSize != (uint16_t)(READ_DMA_CH_REG(&pstcDmaReg->MONDTCTL0, u8Ch) & DMA_DTCTL_BLKSIZE))
    {
        u16Timeout++;
        if (u16Timeout > DMATIMEOUT2)
        {
            enRet = ErrorTimeout;
        }
        else
        {
            MODIFY_DMA_CH_REG(&pstcDmaReg->DTCTL0, u8Ch, DMA_DTCTL_BLKSIZE, (uint32_t)u16BlkSize);
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief  Set the transfer count of the specified dma channel.
 **
 ** \param  [in] pstcDmaReg             The pointer to dma register
 ** \arg    M4_DMA1                     DMAC unit 1 registers
 ** \arg    M4_DMA2
 **
 ** \param  [in] u8Ch                   The specified dma channel.
 ** \param  [in] u16TrnCnt              The transfer count.
 **
 ** \retval None
 **
 ** \note   None
 **
 ******************************************************************************/
en_result_t DMA_SetTransferCnt(M4_DMA_TypeDef *pstcDmaReg, uint8_t u8Ch, uint16_t u16TrnCnt)
{
    uint16_t u16Timeout = 0u;
    en_result_t enRet = Ok;

    DDL_ASSERT(IS_VALID_DMA_REG(pstcDmaReg));
    DDL_ASSERT(IS_VALID_CH(u8Ch));
    DDL_ASSERT(IS_VALID_TRNCNT(u16TrnCnt));

    MODIFY_DMA_CH_REG(&pstcDmaReg->DTCTL0, u8Ch, DMA_DTCTL_CNT, (uint32_t)u16TrnCnt);

    /* Ensure the transfer count has been writed */
    while (u16TrnCnt != ((READ_DMA_CH_REG(&pstcDmaReg->MONDTCTL0, u8Ch) & DMA_DTCTL_CNT) >> DMA_DTCTL_CNT_Pos))
    {
        u16Timeout++;
        if (u16Timeout > DMATIMEOUT2)
        {
            enRet = ErrorTimeout;
        }
        else
        {
            MODIFY_DMA_CH_REG(&pstcDmaReg->DTCTL0, u8Ch, DMA_DTCTL_CNT, (uint32_t)u16TrnCnt);
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief  Set the source repeat size of the specified dma channel.
 **
 ** \param  [in] pstcDmaReg             The pointer to dma register
 ** \arg    M4_DMA1                     DMAC unit 1 registers
 ** \arg    M4_DMA2
 **
 ** \param  [in] u8Ch                   The specified dma channel.
 ** \param  [in] u16Size                The source repeat size.
 **
 ** \retval None
 **
 ** \note   None
 **
 ******************************************************************************/
en_result_t DMA_SetSrcRptSize(M4_DMA_TypeDef *pstcDmaReg, uint8_t u8Ch, uint16_t u16Size)
{
    uint16_t u16Timeout = 0u;
    en_result_t enRet = Ok;

    DDL_ASSERT(IS_VALID_DMA_REG(pstcDmaReg));
    DDL_ASSERT(IS_VALID_CH(u8Ch));
    DDL_ASSERT(IS_VALID_SRPT_SIZE(u16Size));

    MODIFY_DMA_CH_REG(&pstcDmaReg->RPT0, u8Ch, DMA_RPT_SRPT, (uint32_t)u16Size);

    /* Ensure the source repeat size has been writed */
    while (u16Size != (uint16_t)(READ_DMA_CH_REG(&pstcDmaReg->MONRPT0, u8Ch) & DMA_RPT_SRPT))
    {
        u16Timeout++;
        if (u16Timeout > DMATIMEOUT2)
        {
            enRet = ErrorTimeout;
        }
        else
        {
            MODIFY_DMA_CH_REG(&pstcDmaReg->RPT0, u8Ch, DMA_RPT_SRPT, (uint32_t)u16Size);
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief  Set the destination repeat size of the specified dma channel.
 **
 ** \param  [in] pstcDmaReg             The pointer to dma register
 ** \arg    M4_DMA1                     DMAC unit 1 registers
 ** \arg    M4_DMA2
 **
 ** \param  [in] u8Ch                   The specified dma channel.
 ** \param  [in] u16Size                The destination repeat size.
 **
 ** \retval None
 **
 ** \note   None
 **
 ******************************************************************************/
en_result_t DMA_SetDesRptSize(M4_DMA_TypeDef *pstcDmaReg, uint8_t u8Ch, uint16_t u16Size)
{
    uint16_t u16Timeout = 0u;
    en_result_t enRet = Ok;

    DDL_ASSERT(IS_VALID_DMA_REG(pstcDmaReg));
    DDL_ASSERT(IS_VALID_CH(u8Ch));
    DDL_ASSERT(IS_VALID_DRPT_SIZE(u16Size));

    MODIFY_DMA_CH_REG(&pstcDmaReg->RPT0, u8Ch, DMA_RPT_DRPT, (uint32_t)u16Size);

    /* Ensure the destination repeat size has been writed */
    while (u16Size != ((READ_DMA_CH_REG(&pstcDmaReg->MONRPT0, u8Ch) & DMA_RPT_DRPT) >> DMA_RPT_DRPT_Pos))
    {
        u16Timeout++;
        if (u16Timeout > DMATIMEOUT2)
        {
            enRet = ErrorTimeout;
        }
        else
        {
            MODIFY_DMA_CH_REG(&pstcDmaReg->RPT0, u8Ch, DMA_RPT_DRPT, (uint32_t)u16Size);
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief  Set the source repeat size of the specified dma channel.
 **
 ** \param  [in] pstcDmaReg             The pointer to dma register
 ** \arg    M4_DMA1                     DMAC unit 1 registers
 ** \arg    M4_DMA2
 **
 ** \param  [in] u8Ch                   The specified dma channel.
 ** \param  [in] u16Size                The source repeat size.
 **
 ** \retval None
 **
 ** \note   None
 **
 ******************************************************************************/
en_result_t DMA_SetSrcRptbSize(M4_DMA_TypeDef *pstcDmaReg, uint8_t u8Ch, uint16_t u16Size)
{
    uint16_t u16Timeout = 0u;
    en_result_t enRet = Ok;

    DDL_ASSERT(IS_VALID_DMA_REG(pstcDmaReg));
    DDL_ASSERT(IS_VALID_CH(u8Ch));
    DDL_ASSERT(IS_VALID_SRPTB_SIZE(u16Size));

    MODIFY_DMA_CH_REG(&pstcDmaReg->RPTB0, u8Ch, DMA_RPTB_SRPTB, (uint32_t)u16Size);

    /* Ensure the source repeat size has been writed */
    while (u16Size != (uint16_t)(READ_DMA_CH_REG(&pstcDmaReg->RPTB0, u8Ch) & DMA_RPTB_SRPTB))
    {
        u16Timeout++;
        if (u16Timeout > DMATIMEOUT2)
        {
            enRet = ErrorTimeout;
        }
        else
        {
            MODIFY_DMA_CH_REG(&pstcDmaReg->RPTB0, u8Ch, DMA_RPTB_SRPTB, (uint32_t)u16Size);
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief  Set the destination repeat size of the specified dma channel.
 **
 ** \param  [in] pstcDmaReg             The pointer to dma register
 ** \arg    M4_DMA1                     DMAC unit 1 registers
 ** \arg    M4_DMA2
 **
 ** \param  [in] u8Ch                   The specified dma channel.
 ** \param  [in] u16Size                The destination repeat size.
 **
 ** \retval None
 **
 ** \note   None
 **
 ******************************************************************************/
en_result_t DMA_SetDesRptBSize(M4_DMA_TypeDef *pstcDmaReg, uint8_t u8Ch, uint16_t u16Size)
{
    uint16_t u16Timeout = 0u;
    en_result_t enRet = Ok;

    DDL_ASSERT(IS_VALID_DMA_REG(pstcDmaReg));
    DDL_ASSERT(IS_VALID_CH(u8Ch));
    DDL_ASSERT(IS_VALID_DRPTB_SIZE(u16Size));

    MODIFY_DMA_CH_REG(&pstcDmaReg->RPTB0, u8Ch, DMA_RPTB_DRPTB, (uint32_t)u16Size);

    /* Ensure the destination repeat size has been writed */
    while (u16Size != ((READ_DMA_CH_REG(&pstcDmaReg->RPTB0, u8Ch) & DMA_RPTB_DRPTB) >> DMA_RPTB_DRPTB_Pos))
    {
        u16Timeout++;
        if (u16Timeout > DMATIMEOUT2)
        {
            enRet = ErrorTimeout;
        }
        else
        {
            MODIFY_DMA_CH_REG(&pstcDmaReg->RPTB0, u8Ch, DMA_RPTB_DRPTB, (uint32_t)u16Size);
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief  Set the source no-sequence offset & count of the specified dma channel.
 **
 ** \param  [in] pstcDmaReg             The pointer to dma register
 ** \arg    M4_DMA1                     DMAC unit 1 registers
 ** \arg    M4_DMA2
 **
 ** \param  [in] u8Ch                   The specified dma channel.
 ** \param  [in] pstcSrcNseqCfg
 ** \arg    u32offset                   The source no-sequence offset.
 ** \arg    u16cnt                      The source no-sequence count.
 **
 ** \retval None
 **
 ** \note   None
 **
 ******************************************************************************/
en_result_t DMA_SetSrcNseqCfg(M4_DMA_TypeDef *pstcDmaReg, uint8_t u8Ch,
                              const stc_dma_nseq_cfg_t *pstcSrcNseqCfg)
{
    uint16_t u16Timeout = 0u;
    en_result_t enRet = Ok;

    DDL_ASSERT(IS_VALID_DMA_REG(pstcDmaReg));
    DDL_ASSERT(IS_VALID_CH(u8Ch));
    DDL_ASSERT(IS_VALID_SNSOFFSET(pstcSrcNseqCfg->u32Offset));
    DDL_ASSERT(IS_VALID_SNSCNT(pstcSrcNseqCfg->u16Cnt));

    MODIFY_DMA_CH_REG(&pstcDmaReg->SNSEQCTL0, u8Ch,
                      DMA_SNSEQCTL_SOFFSET, pstcSrcNseqCfg->u32Offset);
    MODIFY_DMA_CH_REG(&pstcDmaReg->SNSEQCTL0, u8Ch,
                      DMA_SNSEQCTL_SNSCNT, (uint32_t)pstcSrcNseqCfg->u16Cnt);

    /* Ensure the no-sequence offset & count has been writed */
    while ((pstcSrcNseqCfg->u32Offset | ((uint32_t)pstcSrcNseqCfg->u16Cnt << DMA_SNSEQCTL_SNSCNT_Pos)) != READ_DMA_CH_REG(&pstcDmaReg->MONSNSEQCTL0, u8Ch))
    {
        u16Timeout++;
        if (u16Timeout > DMATIMEOUT2)
        {
            enRet = ErrorTimeout;
        }
        else
        {
            MODIFY_DMA_CH_REG(&pstcDmaReg->SNSEQCTL0, u8Ch,
                              DMA_SNSEQCTL_SOFFSET, pstcSrcNseqCfg->u32Offset);
            MODIFY_DMA_CH_REG(&pstcDmaReg->SNSEQCTL0, u8Ch,
                              DMA_SNSEQCTL_SNSCNT, (uint32_t)pstcSrcNseqCfg->u16Cnt);
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief  Set the source no-sequence offset & count of the specified dma channel.
 **
 ** \param  [in] pstcDmaReg             The pointer to dma register
 ** \arg    M4_DMA1                     DMAC unit 1 registers
 ** \arg    M4_DMA2
 **
 ** \param  [in] u8Ch                   The specified dma channel.
 ** \param  [in] pstcSrcNseqBCfg
 ** \arg    u32NseqDist                 The source no-sequence district interval.
 ** \arg    u16cntB                     The source no-sequence count.
 **
 ** \retval None
 **
 ** \note   None
 **
 ******************************************************************************/
en_result_t DMA_SetSrcNseqBCfg(M4_DMA_TypeDef *pstcDmaReg, uint8_t u8Ch,
                               const stc_dma_nseqb_cfg_t *pstcSrcNseqBCfg)
{
    uint16_t u16Timeout = 0u;
    en_result_t enRet = Ok;

    DDL_ASSERT(IS_VALID_DMA_REG(pstcDmaReg));
    DDL_ASSERT(IS_VALID_CH(u8Ch));
    DDL_ASSERT(IS_VALID_SNSDIST(pstcSrcNseqBCfg->u32NseqDist));
    DDL_ASSERT(IS_VALID_SNSCNTB(pstcSrcNseqBCfg->u16CntB));

    MODIFY_DMA_CH_REG(&pstcDmaReg->SNSEQCTLB0, u8Ch, DMA_SNSEQCTLB_SNSDIST, pstcSrcNseqBCfg->u32NseqDist);
    MODIFY_DMA_CH_REG(&pstcDmaReg->SNSEQCTLB0, u8Ch, DMA_SNSEQCTLB_SNSCNTB, (uint32_t)pstcSrcNseqBCfg->u16CntB);

    /* Ensure the no-sequence offset & count has been writed */
    while ((pstcSrcNseqBCfg->u32NseqDist | ((uint32_t)pstcSrcNseqBCfg->u16CntB << DMA_SNSEQCTLB_SNSCNTB_Pos)) != READ_DMA_CH_REG(&pstcDmaReg->SNSEQCTLB0, u8Ch))
    {
        u16Timeout++;
        if (u16Timeout > DMATIMEOUT2)
        {
            enRet = ErrorTimeout;
        }
        else
        {
            MODIFY_DMA_CH_REG(&pstcDmaReg->SNSEQCTLB0, u8Ch,
                              DMA_SNSEQCTLB_SNSDIST, pstcSrcNseqBCfg->u32NseqDist);
            MODIFY_DMA_CH_REG(&pstcDmaReg->SNSEQCTLB0, u8Ch,
                              DMA_SNSEQCTLB_SNSCNTB, (uint32_t)pstcSrcNseqBCfg->u16CntB);
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief  Set the destination no-sequence offset & count of the specified dma channel.
 **
 ** \param  [in] pstcDmaReg             The pointer to dma register
 ** \arg    M4_DMA1                     DMAC unit 1 registers
 ** \arg    M4_DMA2
 **
 ** \param  [in] u8Ch                   The specified dma channel.
 ** \param  [in] pstcDesNseqCfg
 ** \arg    u32offset                   The destination no-sequence offset.
 ** \arg    u16cnt                      The destination no-sequence count.
 **
 ** \retval None
 **
 ** \note   None
 **
 ******************************************************************************/
en_result_t DMA_SetDesNseqCfg(M4_DMA_TypeDef *pstcDmaReg, uint8_t u8Ch,
                              const stc_dma_nseq_cfg_t *pstcDesNseqCfg)
{
    uint16_t u16Timeout = 0u;
    en_result_t enRet = Ok;

    DDL_ASSERT(IS_VALID_DMA_REG(pstcDmaReg));
    DDL_ASSERT(IS_VALID_CH(u8Ch));
    DDL_ASSERT(IS_VALID_DNSOFFSET(pstcDesNseqCfg->u32Offset));
    DDL_ASSERT(IS_VALID_DNSCNT(pstcDesNseqCfg->u16Cnt));

    MODIFY_DMA_CH_REG(&pstcDmaReg->DNSEQCTL0, u8Ch, DMA_DNSEQCTL_DOFFSET, pstcDesNseqCfg->u32Offset);
    MODIFY_DMA_CH_REG(&pstcDmaReg->DNSEQCTL0, u8Ch, DMA_DNSEQCTL_DNSCNT, (uint32_t)pstcDesNseqCfg->u16Cnt);

    /* Ensure the no-sequence offset & count has been writed */
    while ((pstcDesNseqCfg->u32Offset | ((uint32_t)pstcDesNseqCfg->u16Cnt << DMA_DNSEQCTL_DNSCNT_Pos)) != READ_DMA_CH_REG(&pstcDmaReg->MONDNSEQCTL0, u8Ch))
    {
        u16Timeout++;
        if (u16Timeout > DMATIMEOUT2)
        {
            enRet = ErrorTimeout;
        }
        else
        {
            MODIFY_DMA_CH_REG(&pstcDmaReg->DNSEQCTL0, u8Ch,
                              DMA_DNSEQCTL_DOFFSET, pstcDesNseqCfg->u32Offset);
            MODIFY_DMA_CH_REG(&pstcDmaReg->DNSEQCTL0, u8Ch,
                              DMA_DNSEQCTL_DNSCNT, (uint32_t)pstcDesNseqCfg->u16Cnt);
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief  Set the destination no-sequence offset & count of the specified dma channel.
 **
 ** \param  [in] pstcDmaReg             The pointer to dma register
 ** \arg    M4_DMA1                     DMAC unit 1 registers
 ** \arg    M4_DMA2
 **
 ** \param  [in] u8Ch                   The specified dma channel.
 ** \param  [in] pstcDesNseqBCfg
 ** \arg    u32offset                   The destination no-sequence offset.
 ** \arg    u16cnt                      The destination no-sequence count.
 **
 ** \retval None
 **
 ** \note   None
 **
 ******************************************************************************/
en_result_t DMA_SetDesNseqBCfg(M4_DMA_TypeDef *pstcDmaReg, uint8_t u8Ch,
                               const stc_dma_nseqb_cfg_t *pstcDesNseqBCfg)
{
    uint16_t u16Timeout = 0u;
    en_result_t enRet = Ok;

    DDL_ASSERT(IS_VALID_DMA_REG(pstcDmaReg));
    DDL_ASSERT(IS_VALID_CH(u8Ch));
    DDL_ASSERT(IS_VALID_DNSDIST(pstcDesNseqBCfg->u32NseqDist));
    DDL_ASSERT(IS_VALID_DNSCNTB(pstcDesNseqBCfg->u16CntB));

    MODIFY_DMA_CH_REG(&pstcDmaReg->DNSEQCTLB0, u8Ch, DMA_DNSEQCTLB_DNSDIST, pstcDesNseqBCfg->u32NseqDist);
    MODIFY_DMA_CH_REG(&pstcDmaReg->DNSEQCTLB0, u8Ch, DMA_DNSEQCTLB_DNSCNTB, (uint32_t)pstcDesNseqBCfg->u16CntB);

    /* Ensure the no-sequence offset & count has been writed */
    while ((pstcDesNseqBCfg->u32NseqDist | ((uint32_t)pstcDesNseqBCfg->u16CntB << DMA_DNSEQCTLB_DNSCNTB_Pos)) != READ_DMA_CH_REG(&pstcDmaReg->DNSEQCTLB0, u8Ch))
    {
        u16Timeout++;
        if (u16Timeout > DMATIMEOUT2)
        {
            enRet = ErrorTimeout;
        }
        else
        {
            MODIFY_DMA_CH_REG(&pstcDmaReg->DNSEQCTLB0, u8Ch,
                              DMA_DNSEQCTLB_DNSDIST, pstcDesNseqBCfg->u32NseqDist);
            MODIFY_DMA_CH_REG(&pstcDmaReg->DNSEQCTLB0, u8Ch,
                              DMA_DNSEQCTLB_DNSCNTB, (uint32_t)pstcDesNseqBCfg->u16CntB);
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief  Set linked list pointer of the specified dma channel.
 **
 ** \param  [in] pstcDmaReg             The pointer to dma register
 ** \arg    M4_DMA1                     DMAC unit 1 registers
 ** \arg    M4_DMA2
 **
 ** \param  [in] u8Ch                   The specified dma channel.
 ** \param  [in] u32Pointer             The decriptor pointer.
 **
 ** \retval None.
 **
 ** \note   None
 **
 ******************************************************************************/
en_result_t DMA_SetLLP(M4_DMA_TypeDef *pstcDmaReg, uint8_t u8Ch, uint32_t u32Pointer)
{
    uint16_t u16Timeout = 0u;
    en_result_t enRet = Ok;

    DDL_ASSERT(IS_VALID_DMA_REG(pstcDmaReg));
    DDL_ASSERT(IS_VALID_CH(u8Ch));
    DDL_ASSERT(IS_VALID_LLP(u32Pointer));

    WRITE_DMA_CH_REG(&pstcDmaReg->LLP0, u8Ch, u32Pointer);

    /* Ensure the destination repeat size has been writed */
    while (u32Pointer != ((READ_DMA_CH_REG(&pstcDmaReg->LLP0, u8Ch) & DMA_LLP_LLP) >> DMA_LLP_LLP_Pos))
    {
        u16Timeout++;
        if (u16Timeout > DMATIMEOUT2)
        {
            enRet = ErrorTimeout;
        }
        else
        {
            WRITE_DMA_CH_REG(&pstcDmaReg->LLP0, u8Ch, u32Pointer);
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief  Set The DMA trigger source.
 **
 ** \param  [in] pstcDmaReg             The pointer to dma register
 ** \arg    M4_DMA1                     DMAC unit 1 registers
 ** \arg    M4_DMA2
 **
 ** \param  [in] u8Ch                   The specified dma channel.
 ** \param  [in] enSrc                  The DMA trigger source.
 **
 ** \retval None.
 **
 ** \note   Before call this function, shoud ensure enable AOS.
 **
 ******************************************************************************/
void DMA_SetTriggerSrc(const M4_DMA_TypeDef *pstcDmaReg, uint8_t u8Ch, en_event_src_t enSrc)
{
    DDL_ASSERT(IS_VALID_DMA_REG(pstcDmaReg));

    if (M4_DMA1 == pstcDmaReg)
    {
        WRITE_DMA_CH_TRGSEL(&M4_AOS->DMA1_TRGSEL0, u8Ch, enSrc);
    }
    else if (M4_DMA2 == pstcDmaReg)
    {
        WRITE_DMA_CH_TRGSEL(&M4_AOS->DMA2_TRGSEL0, u8Ch, enSrc);
    }
    else
    {
        // else
    }
}

/**
 *******************************************************************************
 ** \brief  Set linked list pointer of the specified dma channel.
 **
 ** \param  [in] enSrc                  The DMA trigger source.
 **
 ** \retval None.
 **
 ** \note   Before call this function, should ensure enable AOS.
 **
 ******************************************************************************/
void DMA_SetReConfigTriggerSrc(en_event_src_t enSrc)
{

    M4_AOS->DMATRGSELRC_f.TRGSEL = enSrc;
}
/**
 *******************************************************************************
 ** \brief  The configuration of the specified dma channel.
 **
 ** \param  [in] pstcDmaReg             The pointer to dma register
 ** \arg    M4_DMA1                     DMAC unit 1 registers
 ** \arg    M4_DMA2
 **
 ** \param  [in] u8Ch                   The specified dma channel.
 ** \param  [in] pstcChCfg              The configuration pointer.
 ** \arg    enSrcInc                    The source address mode.
 ** \arg    enDesInc                    The destination address mode.
 ** \arg    enSrcRptEn                  The source repeat function(enable or disable).
 ** \arg    enDesRptEn                  The destination repeat function(enable or disable).
 ** \arg    enSrcNseqEn                 The source no_sequence function(enable or disable).
 ** \arg    enDesNseqEn                 The destination no_sequence function(enable or disable).
 ** \arg    enTrnWidth                  The transfer data width.
 ** \arg    enLlpEn                     The linked list pointer function(enable or disable).
 ** \arg    enLlpMd                     The linked list pointer mode.
 ** \arg    enIntEn                     The interrupt function(enable or disable).
 **
 ** \retval None.
 **
 ** \note   None
 **
 ******************************************************************************/
void DMA_ChannelCfg(M4_DMA_TypeDef *pstcDmaReg, uint8_t u8Ch,
                    const stc_dma_ch_cfg_t *pstcChCfg)
{
    DDL_ASSERT(IS_VALID_DMA_REG(pstcDmaReg));
    DDL_ASSERT(IS_VALID_CH(u8Ch));
    DDL_ASSERT(IS_VALID_ADDR_MODE(pstcChCfg->enSrcInc));
    DDL_ASSERT(IS_VALID_ADDR_MODE(pstcChCfg->enDesInc));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcChCfg->enSrcRptEn));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcChCfg->enDesRptEn));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcChCfg->enSrcNseqEn));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcChCfg->enDesNseqEn));
    DDL_ASSERT(IS_VALID_TRN_WIDTH(pstcChCfg->enTrnWidth));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcChCfg->enLlpEn));
    DDL_ASSERT(IS_VALID_LLP_MODE(pstcChCfg->enLlpMd));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcChCfg->enIntEn));

    /* Set the source address mode. */
    MODIFY_DMA_CH_REG(&pstcDmaReg->CH0CTL, u8Ch, DMA_CHCTL_SINC, pstcChCfg->enSrcInc);
    /* Set the destination address mode. */
    MODIFY_DMA_CH_REG(&pstcDmaReg->CH0CTL, u8Ch, DMA_CHCTL_DINC, pstcChCfg->enDesInc);
    /* Enable or disable source repeat function. */
    MODIFY_DMA_CH_REG(&pstcDmaReg->CH0CTL, u8Ch, DMA_CHCTL_SRPTEN, pstcChCfg->enSrcRptEn);
    /* Enable or disable destination repeat function. */
    MODIFY_DMA_CH_REG(&pstcDmaReg->CH0CTL, u8Ch, DMA_CHCTL_DRPTEN, pstcChCfg->enDesRptEn);
    /* Enable or disable source no_sequence function. */
    MODIFY_DMA_CH_REG(&pstcDmaReg->CH0CTL, u8Ch, DMA_CHCTL_SNSEQEN, pstcChCfg->enSrcNseqEn);
    /* Enable or disable destination no_sequence function. */
    MODIFY_DMA_CH_REG(&pstcDmaReg->CH0CTL, u8Ch, DMA_CHCTL_DNSEQEN, pstcChCfg->enDesNseqEn);
    /* Set the transfer data width. */
    MODIFY_DMA_CH_REG(&pstcDmaReg->CH0CTL, u8Ch, DMA_CHCTL_HSIZE, pstcChCfg->enTrnWidth);
    /* Enable or disable linked list pointer no_sequence function. */
    MODIFY_DMA_CH_REG(&pstcDmaReg->CH0CTL, u8Ch, DMA_CHCTL_LLPEN, pstcChCfg->enLlpEn);
    /* Set the linked list pointer mode. */
    MODIFY_DMA_CH_REG(&pstcDmaReg->CH0CTL, u8Ch, DMA_CHCTL_LLPRUN, pstcChCfg->enLlpMd);
    /* Enable or disable channel interrupt function. */
    MODIFY_DMA_CH_REG(&pstcDmaReg->CH0CTL, u8Ch, DMA_CHCTL_IE, pstcChCfg->enIntEn);
}

/**
 *******************************************************************************
 ** \brief  The configuration of the specified dma channel.
 **
 ** \param  [in] pstcDmaReg             The pointer to dma register
 ** \arg    M4_DMA1                     DMAC unit 1 registers
 ** \arg    M4_DMA2
 **
 ** \param  [in] u8Ch                   The specified dma channel.
 ** \param  [in] pstcDmaCfg             The configuration pointer.
 ** \arg    enSrcInc                    The source address mode.
 ** \arg    enDesInc                    The destination address mode.
 ** \arg    enSrcRptEn                  The source repeat function(enable or disable).
 ** \arg    enDesRptEn                  The destination repeat function(enable or disable).
 ** \arg    enSrcNseqEn                 The source no_sequence function(enable or disable).
 ** \arg    enDesNseqEn                 The destination no_sequence function(enable or disable).
 ** \arg    enTrnWidth                  The transfer data width.
 ** \arg    enLlpEn                     The linked list pointer function(enable or disable).
 ** \arg    enLlpMd                     The linked list pointer mode.
 ** \arg    enIntEn                     The interrupt function(enable or disable).
 **
 ** \retval None.
 **
 ** \note   This function should be used after enable DMAx clk(PWC_Fcg0PeriphClockCmd)
 **         and before channel enable.
 **
 ******************************************************************************/
void DMA_InitChannel(M4_DMA_TypeDef *pstcDmaReg, uint8_t u8Ch,
                     const stc_dma_config_t *pstcDmaCfg)
{
    DDL_ASSERT(IS_VALID_DMA_REG(pstcDmaReg));
    DDL_ASSERT(IS_VALID_CH(u8Ch));
    DDL_ASSERT(IS_VALID_BLKSIZE(pstcDmaCfg->u16BlockSize));
    DDL_ASSERT(IS_VALID_TRNCNT(pstcDmaCfg->u16TransferCnt));
    DDL_ASSERT(IS_VALID_SRPT_SIZE(pstcDmaCfg->u16SrcRptSize));
    DDL_ASSERT(IS_VALID_DRPT_SIZE(pstcDmaCfg->u16DesRptSize));

    /* Enable DMA. */
    DMA_Cmd(pstcDmaReg, Enable);
    /* Disable DMA interrupt */
    CLR_DMA_CH_REG_BIT(&pstcDmaReg->CH0CTL, u8Ch, DMA_CHCTL_IE_Pos);
    /* Set DMA source address. */
    WRITE_DMA_CH_REG(&pstcDmaReg->SAR0, u8Ch, pstcDmaCfg->u32SrcAddr);
    /* Set DMA destination address. */
    WRITE_DMA_CH_REG(&pstcDmaReg->DAR0, u8Ch, pstcDmaCfg->u32DesAddr);
    /* Set DMA transfer block size. */
    MODIFY_DMA_CH_REG(&pstcDmaReg->DTCTL0, u8Ch, DMA_DTCTL_BLKSIZE, (uint32_t)pstcDmaCfg->u16BlockSize);
    /* Set DMA transfer count. */
    MODIFY_DMA_CH_REG(&pstcDmaReg->DTCTL0, u8Ch, DMA_DTCTL_CNT, (uint32_t)pstcDmaCfg->u16TransferCnt);
    /* Set DMA source repeat size. */
    MODIFY_DMA_CH_REG(&pstcDmaReg->RPT0, u8Ch, DMA_RPT_SRPT, (uint32_t)pstcDmaCfg->u16SrcRptSize);
    /* Set DMA destination repeat size. */
    MODIFY_DMA_CH_REG(&pstcDmaReg->RPT0, u8Ch, DMA_RPT_DRPT, (uint32_t)pstcDmaCfg->u16DesRptSize);
    /* Set DMA source no_sequence. */
    MODIFY_DMA_CH_REG(&pstcDmaReg->SNSEQCTL0, u8Ch, DMA_SNSEQCTL_SOFFSET, pstcDmaCfg->stcSrcNseqCfg.u32Offset);
    MODIFY_DMA_CH_REG(&pstcDmaReg->SNSEQCTL0, u8Ch, DMA_SNSEQCTL_SNSCNT, (uint32_t)pstcDmaCfg->stcSrcNseqCfg.u16Cnt);
    /* Set DMA destination no_sequence. */
    MODIFY_DMA_CH_REG(&pstcDmaReg->DNSEQCTL0, u8Ch, DMA_DNSEQCTL_DOFFSET, pstcDmaCfg->stcDesNseqCfg.u32Offset);
    MODIFY_DMA_CH_REG(&pstcDmaReg->DNSEQCTL0, u8Ch, DMA_DNSEQCTL_DNSCNT, (uint32_t)pstcDmaCfg->stcDesNseqCfg.u16Cnt);
    /* Set DMA linked list pointer. */
    WRITE_DMA_CH_REG(&pstcDmaReg->LLP0, u8Ch, pstcDmaCfg->u32DmaLlp);
    /* Set DMA channel parameter. */
    DMA_ChannelCfg(pstcDmaReg, u8Ch, &pstcDmaCfg->stcDmaChCfg);
}

void DMA_DeInit(M4_DMA_TypeDef *pstcDmaReg, uint8_t u8Ch)
{
    DDL_ASSERT(IS_VALID_DMA_REG(pstcDmaReg));
    DDL_ASSERT(IS_VALID_CH(u8Ch));

    /* reset dma channel */
    WRITE_DMA_CH_REG(&pstcDmaReg->CH0CTL, u8Ch, DMA_CHCTL_DEFAULT);
    WRITE_DMA_CH_REG(&pstcDmaReg->DTCTL0, u8Ch, DMA_DTCTL_DEFAULT);
    WRITE_DMA_CH_REG(&pstcDmaReg->DAR0, u8Ch, DMA_DAR_DEFAULT);
    WRITE_DMA_CH_REG(&pstcDmaReg->SAR0, u8Ch, DMA_SAR_DEFAULT);
    WRITE_DMA_CH_REG(&pstcDmaReg->SNSEQCTL0, u8Ch, DMA_SNSEQCTL_DEFAULT);
    WRITE_DMA_CH_REG(&pstcDmaReg->DNSEQCTL0, u8Ch, DMA_DNSEQCTL_DEFAULT);
    WRITE_DMA_CH_REG(&pstcDmaReg->RPT0, u8Ch, DMA_RPT_DEFAULT);
    WRITE_DMA_CH_REG(&pstcDmaReg->LLP0, u8Ch, DMA_LLP_DEFAULT);
    WRITE_DMA_CH_REG(&pstcDmaReg->RCFGCTL, u8Ch, DMA_RCFGCTL_DEFAULT);

    /* Set trigger source event max */
    DMA_SetTriggerSrc(pstcDmaReg, u8Ch, EVT_MAX);
    /* disable channel */
    DMA_ChannelCmd(pstcDmaReg, u8Ch, Disable);
}

#endif /* DDL_DMAC_ENABLE */

//@} // DmacGroup

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
