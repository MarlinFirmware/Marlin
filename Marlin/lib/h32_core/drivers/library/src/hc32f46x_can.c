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
/** \file hc32f46x_can.c
 **
 ** A detailed description is available at
 ** @link CanGroup CAN description @endlink
 **
 **   - 2018-12-13  1.0  Lux First version for Device Driver Library of CAN.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32f46x_can.h"
#include "hc32f46x_utility.h"

#if (DDL_CAN_ENABLE == DDL_ON)

/**
 *******************************************************************************
 ** \addtogroup CanGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define CAN_RESET_ENABLE() (M4_CAN->CFG_STAT_f.RESET = 1u)
#define CAN_RESET_DISABLE()                 \
    do                                      \
    {                                       \
        do                                  \
        {                                   \
            M4_CAN->CFG_STAT_f.RESET = 0u;  \
        } while (M4_CAN->CFG_STAT_f.RESET); \
    } while (0)

#define CAN_ACF_ID_REG_SEL ((uint8_t)0x00u)
#define CAN_ACF_MASK_REG_SEL ((uint8_t)0x01u)

/*! Parameter validity check for CAN Mode \a CanMode. */
#define IS_CAN_MODE_VALID(CanMode)              \
    ((CanExternalLoopBackMode == (CanMode)) ||  \
     (CanInternalLoopBackMode == (CanMode)) ||  \
     (CanTxSignalPrimaryMode == (CanMode)) ||   \
     (CanTxSignalSecondaryMode == (CanMode)) || \
     (CanListenOnlyMode == (CanMode)))

/*! Parameter validity check for CAN Tx Cmd \a TxCmd. */
#define IS_TX_CMD_VALID(TxCmd)        \
    ((CanPTBTxCmd == (TxCmd)) ||      \
     (CanPTBTxAbortCmd == (TxCmd)) || \
     (CanSTBTxOneCmd == (TxCmd)) ||   \
     (CanSTBTxAllCmd == (TxCmd)) ||   \
     (CanSTBTxAbortCmd == (TxCmd)))

/*! Parameter validity check for CAN status \a enCanStatus. */
#define IS_CAN_STATUS_VALID(enCanStatus) \
    ((CanRxActive == (enCanStatus)) ||   \
     (CanTxActive == (enCanStatus)) ||   \
     (CanBusoff == (enCanStatus)))

/*! Parameter validity check for CAN Irq type \a enCanIrqType. */
#define IS_CAN_IRQ_TYPE_VALID(enCanIrqType)         \
    ((CanRxIrqEn == (enCanIrqType)) ||              \
     (CanRxOverIrqEn == (enCanIrqType)) ||          \
     (CanRxBufFullIrqEn == (enCanIrqType)) ||       \
     (CanRxBufAlmostFullIrqEn == (enCanIrqType)) || \
     (CanTxPrimaryIrqEn == (enCanIrqType)) ||       \
     (CanTxSecondaryIrqEn == (enCanIrqType)) ||     \
     (CanErrorIrqEn == (enCanIrqType)) ||           \
     (CanErrorPassiveIrqEn == (enCanIrqType)) ||    \
     (CanArbiLostIrqEn == (enCanIrqType)) ||        \
     (CanBusErrorIrqEn == (enCanIrqType)))

/*! Parameter validity check for CAN Irq flag type \a enCanIrqFLg. */
#define IS_CAN_IRQ_FLAG_VALID(enCanIrqFLg)           \
    ((CanTxBufFullIrqFlg == (enCanIrqFLg)) ||        \
     (CanRxIrqFlg == (enCanIrqFLg)) ||               \
     (CanRxOverIrqFlg == (enCanIrqFLg)) ||           \
     (CanRxBufFullIrqFlg == (enCanIrqFLg)) ||        \
     (CanRxBufAlmostFullIrqFlg == (enCanIrqFLg)) ||  \
     (CanTxPrimaryIrqFlg == (enCanIrqFLg)) ||        \
     (CanTxSecondaryIrqFlg == (enCanIrqFLg)) ||      \
     (CanErrorIrqFlg == (enCanIrqFLg)) ||            \
     (CanAbortIrqFlg == (enCanIrqFLg)) ||            \
     (CanErrorWarningIrqFlg == (enCanIrqFLg)) ||     \
     (CanErrorPassivenodeIrqFlg == (enCanIrqFLg)) || \
     (CanErrorPassiveIrqFlg == (enCanIrqFLg)) ||     \
     (CanArbiLostIrqFlg == (enCanIrqFLg)) ||         \
     (CanBusErrorIrqFlg == (enCanIrqFLg)))

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
 ** \brief  Configures the can base functions
 **
 ** \param  [in] pstcCanInitCfg            The can initial config struct.
 **
 ** \retval None
 **
 ** \note   None
 **
 ******************************************************************************/
void CAN_Init(stc_can_init_config_t *pstcCanInitCfg)
{
    if (NULL != pstcCanInitCfg)
    {
        M4_CAN->RCTRL_f.SACK = pstcCanInitCfg->enCanSAck;
        M4_CAN->TCTRL_f.TSMODE = pstcCanInitCfg->enCanSTBMode;
        M4_CAN->RCTRL_f.RBALL = pstcCanInitCfg->enCanRxBufAll;
        M4_CAN->RCTRL_f.ROM = pstcCanInitCfg->enCanRxBufMode;

        M4_CAN->RTIE = 0x00u;

        CAN_RESET_ENABLE();

        M4_CAN->BT_f.PRESC = pstcCanInitCfg->stcCanBt.PRESC;
        M4_CAN->BT_f.SEG_1 = pstcCanInitCfg->stcCanBt.SEG_1;
        M4_CAN->BT_f.SEG_2 = pstcCanInitCfg->stcCanBt.SEG_2;
        M4_CAN->BT_f.SJW = pstcCanInitCfg->stcCanBt.SJW;

        M4_CAN->LIMIT_f.AFWL = pstcCanInitCfg->stcWarningLimit.CanWarningLimitVal;
        M4_CAN->LIMIT_f.EWL = pstcCanInitCfg->stcWarningLimit.CanErrorWarningLimitVal;

        CAN_RESET_DISABLE();
    }
}

/**
 *******************************************************************************
 ** \brief  De-Init (RESET CAN register)
 **
 ** \param  None
 **
 ** \retval None
 **
 ** \note   None
 **
 ******************************************************************************/
void CAN_DeInit(void)
{
    CAN_RESET_ENABLE();
}

/**
 *******************************************************************************
 ** \brief  Configures the can Mode
 **
 ** \param  [in] enMode                 The can mode enum. @ref en_can_mode_t
 ** \param  [in] enNewState             The new state of the can filter chanel.
 ** \arg    Enable                      Enable filter.
 ** \arg    Disable                     Disable filter.
 ** \retval None
 **
 ** \note   None
 **
 ******************************************************************************/
void CAN_ModeConfig(en_can_mode_t enMode, en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_CAN_MODE_VALID(enMode));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    if (CanListenOnlyMode == enMode)
    {
        M4_CAN->TCMD_f.LOM = enNewState;
    }
    else
    {
        if (Enable == enNewState)
        {
            M4_CAN->CFG_STAT |= enMode;
        }
        else
        {
            M4_CAN->CFG_STAT &= ~enMode;
        }
    }
}

/**
 *******************************************************************************
 ** \brief  Configures the can acceptance filter
 **
 ** \param  [in] pstcFilter             The can filter config struct.
 **                                     @ref stc_can_filter_t
 ** \param  [in] enNewState             The new state of the can filter chanel.
 ** \arg    Enable                      Enable filter.
 ** \arg    Disable                     Disable filter.
 **
 ** \retval None
 **
 ** \note   None
 **
 ******************************************************************************/
void CAN_FilterConfig(const stc_can_filter_t *pstcFilter, en_functional_state_t enNewState)
{
    if (NULL != pstcFilter)
    {
        DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

        CAN_RESET_ENABLE();

        //<<Acceptance filter address
        M4_CAN->ACFCTRL_f.ACFADR = pstcFilter->enFilterSel;

        //<<ID config
        M4_CAN->ACFCTRL_f.SELMASK = CAN_ACF_ID_REG_SEL;
        M4_CAN->ACF = pstcFilter->u32CODE;

        //<<MASK config
        M4_CAN->ACFCTRL_f.SELMASK = CAN_ACF_MASK_REG_SEL;
        M4_CAN->ACF = pstcFilter->u32MASK;

        //<<Frame format config
        M4_CAN->ACF_f.AIDEE = ((pstcFilter->enAcfFormat >> 1ul) & 0x01u);
        M4_CAN->ACF_f.AIDE = (pstcFilter->enAcfFormat & 0x01ul);

        if (Enable == enNewState)
        {
            M4_CAN->ACFEN |= (uint8_t)(1ul << pstcFilter->enFilterSel);
        }
        else
        {
            M4_CAN->ACFEN &= (uint8_t)(~(1ul << (pstcFilter->enFilterSel)));
        }

        CAN_RESET_DISABLE();
    }
}

/**
 *******************************************************************************
 ** \brief  Configures the can Tx frame set
 **
 ** \param  [in] pstcTxFrame            The can Tx frame struct.
 **                                     @ref stc_can_txframe_t
 ** \retval None
 **
 ** \note   None
 **
 ******************************************************************************/
void CAN_SetFrame(stc_can_txframe_t *pstcTxFrame)
{
    if (NULL != pstcTxFrame)
    {
        M4_CAN->TCMD_f.TBSEL = pstcTxFrame->enBufferSel;
        M4_CAN->TBUF0 = pstcTxFrame->TBUF32_0;
        M4_CAN->TBUF1 = pstcTxFrame->TBUF32_1;
        M4_CAN->TBUF2 = pstcTxFrame->TBUF32_2[0];
        M4_CAN->TBUF3 = pstcTxFrame->TBUF32_2[1];

        if (CanSTBSel == pstcTxFrame->enBufferSel)
        {
            M4_CAN->TCTRL_f.TSNEXT = Enable;
        }
    }
}

/**
 *******************************************************************************
 ** \brief  Configures the can Tx Command
 **
 ** \param  [in] enTxCmd            The can Tx Command.
 **
 ** \retval Can Tx buffer status    @ref en_can_tx_buf_status_t
 **
 ** \note   None
 **
 ******************************************************************************/
en_can_tx_buf_status_t CAN_TransmitCmd(en_can_tx_cmd_t enTxCmd)
{
    DDL_ASSERT(IS_TX_CMD_VALID(enTxCmd));

    M4_CAN->TCMD |= enTxCmd;

    return (en_can_tx_buf_status_t)M4_CAN->TCTRL_f.TSSTAT;
}

/**
 *******************************************************************************
 ** \brief  Configures the can Rx frame
 **
 ** \param  [in] pstcRxFrame        The can Rx frame.
 **                                 @ref stc_can_rxframe_t
 ** \retval Can rx buffer status    @ref en_can_rx_buf_status_t
 **
 ** \note   None
 **
 ******************************************************************************/
en_can_rx_buf_status_t CAN_Receive(stc_can_rxframe_t *pstcRxFrame)
{
    if (NULL != pstcRxFrame)
    {
        pstcRxFrame->RBUF32_0 = M4_CAN->RBUF0;
        pstcRxFrame->RBUF32_1 = M4_CAN->RBUF1;
        pstcRxFrame->RBUF32_2[0] = M4_CAN->RBUF2;
        pstcRxFrame->RBUF32_2[1] = M4_CAN->RBUF3;

        M4_CAN->RCTRL_f.RREL = 1u;
    }
    return (en_can_rx_buf_status_t)M4_CAN->RCTRL_f.RSSTAT;
}

/**
 *******************************************************************************
 ** \brief  Get the can Error Status
 **
 ** \param  None
 **
 ** \retval en_can_error_t          The can error status
 **
 ** \note   None
 **
 ******************************************************************************/
en_can_error_t CAN_ErrorStatusGet(void)
{
    en_can_error_t enRet = UNKOWN_ERROR;

    if (6u > M4_CAN->EALCAP_f.KOER)
    {
        enRet = (en_can_error_t)M4_CAN->EALCAP_f.KOER;
    }
    return enRet;
}

/**
 *******************************************************************************
 ** \brief  Get the can Status
 **
 ** \param  enCanStatus             The can status
 ** \arg    true
 ** \arg    false
 ** \retval bool
 **
 ** \note   None
 **
 ******************************************************************************/
bool CAN_StatusGet(en_can_status_t enCanStatus)
{
    bool bRet = false;
    DDL_ASSERT(IS_CAN_STATUS_VALID(enCanStatus));

    if (M4_CAN->CFG_STAT & enCanStatus)
    {
        bRet = true;
    }
    return bRet;
}

/**
 *******************************************************************************
 ** \brief  Configures the can Interrupt enable
 **
 ** \param  [in] enCanIrqType            The can interrupt type.
 ** \param  [in] enNewState             The new state of the can interrupt.
 ** \arg    Enable                      Enable.
 ** \arg    Disable                     Disable.
 **
 ** \retval None
 **
 ** \note   None
 **
 ******************************************************************************/
void CAN_IrqCmd(en_can_irq_type_t enCanIrqType, en_functional_state_t enNewState)
{
    volatile uint32_t *u32pIE;

    DDL_ASSERT(IS_CAN_IRQ_TYPE_VALID(enCanIrqType));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    u32pIE = (volatile uint32_t *)(&M4_CAN->RTIE);

    if (Enable == enNewState)
    {
        *u32pIE |= enCanIrqType;
    }
    else
    {
        *u32pIE &= ~((uint32_t)enCanIrqType);
    }
}

/**
 *******************************************************************************
 ** \brief  Get the can Interrupt Flag
 **
 ** \param  [in] enCanIrqFlgType            The can interrupt Flag.
 **
 ** \retval bool
 **
 ** \note   None
 **
 ******************************************************************************/
bool CAN_IrqFlgGet(en_can_irq_flag_type_t enCanIrqFlgType)
{
    volatile uint32_t *u32pIE = NULL;
    bool bRet = false;

    DDL_ASSERT(IS_CAN_IRQ_FLAG_VALID(enCanIrqFlgType));

    u32pIE = (volatile uint32_t *)(&M4_CAN->RTIE);

    if (*u32pIE & enCanIrqFlgType)
    {
        bRet = true;
    }
    return bRet;
}

/**
 *******************************************************************************
 ** \brief  Clear the can Interrupt Flag
 **
 ** \param  [in] enCanIrqFlgType           The can interrupt type.
 **
 ** \retval None
 **
 ** \note   None
 **
 ******************************************************************************/
void CAN_IrqFlgClr(en_can_irq_flag_type_t enCanIrqFlgType)
{
    volatile uint32_t *u32pIE = NULL;

    DDL_ASSERT(IS_CAN_IRQ_FLAG_VALID(enCanIrqFlgType));

    u32pIE = (volatile uint32_t *)(&M4_CAN->RTIE);

    *u32pIE |= enCanIrqFlgType;
}

/**
 *******************************************************************************
 ** \brief  Get the can Rx Error Counter
 **
 ** \param  None
 **
 ** \retval Error Counter(0~255)
 **
 ** \note   None
 **
 ******************************************************************************/
uint8_t CAN_RxErrorCntGet(void)
{
    return M4_CAN->RECNT;
}

/**
 *******************************************************************************
 ** \brief  Get the can Tx Error Counter
 **
 ** \param  None
 **
 ** \retval Error Counter(0~255)
 **
 ** \note   None
 **
 ******************************************************************************/
uint8_t CAN_TxErrorCntGet(void)
{
    return M4_CAN->TECNT;
}

/**
 *******************************************************************************
 ** \brief  Get the can Arbitration lost captrue
 **
 ** \param  None
 **
 ** \retval address(0~31)
 **
 ** \note   None
 **
 ******************************************************************************/
uint8_t CAN_ArbitrationLostCap(void)
{
    return M4_CAN->EALCAP_f.ALC;
}

//@} // CanGroup

#endif /* DDL_CAN_ENABLE */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
