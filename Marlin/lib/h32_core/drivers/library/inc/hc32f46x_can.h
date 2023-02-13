/*****************************************************************************
 * Copyright (C) 2016, Huada Semiconductor Co., Ltd All rights reserved.
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
/** \file hc32f46x_can.h
 **
 ** A detailed description is available at
 ** @link CanGroup CAN description @endlink
 **
 **   - 2018-11-27  1.0  Lux First version for Device Driver Library of CAN
 **
 ******************************************************************************/
#ifndef __HC32F46X_CAN_H__
#define __HC32F46X_CAN_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_common.h"
#include "ddl_config.h"

#if (DDL_CAN_ENABLE == DDL_ON)

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

    /**
     *******************************************************************************
     ** \defgroup CanGroup Controller Area Network(CAN)
     **
     ******************************************************************************/
    //@{

    /*******************************************************************************
     * Global type definitions ('typedef')
     ******************************************************************************/

    /**
     *******************************************************************************
     ** \brief  The Can error types.
     ******************************************************************************/
    typedef enum
    {
        NO_ERROR = 0U,
        BIT_ERROR = 1U,
        FORM_ERROR = 2U,
        STUFF_ERROR = 3U,
        ACK_ERROR = 4U,
        CRC_ERROR = 5U,
        UNKOWN_ERROR = 6U,
    } en_can_error_t;

    /**
     *******************************************************************************
     ** \brief  The Can transmit buffer select.(TCMD)
     ******************************************************************************/
    typedef enum
    {
        CanPTBSel = 0U, ///< high-priority buffer
        CanSTBSel = 1U, ///< secondary buffer
    } en_can_buffer_sel_t;

    /**
     *******************************************************************************
     ** \brief  The Can warning limits.(AFWL)
     ******************************************************************************/
    typedef struct stc_can_warning_limit
    {
        uint8_t CanWarningLimitVal;      ///< Receive buffer almost full warning limit
        uint8_t CanErrorWarningLimitVal; ///< Programmable error warning limit
    } stc_can_warning_limit_t;

    /**
     *******************************************************************************
     ** \brief  The Acceptance Filters Frame Format Check.(ACF)
     ******************************************************************************/
    typedef enum en_can_acf_format_en
    {
        CanStdFrames = 0x02u, ///< Accepts only Standard frames
        CanExtFrames = 0x03u, ///< Accepts only Extended frames
        CanAllFrames = 0x00u, ///< Accepts both standard or extended frames
    } en_can_acf_format_en_t;

    /**
     *******************************************************************************
     ** \brief  The Acceptance Filters Enable.(ACFEN)
     ******************************************************************************/
    typedef enum en_can_filter_sel
    {
        CanFilterSel1 = 0u, ///< The Acceptance Filter 1 Enable
        CanFilterSel2 = 1u, ///< The Acceptance Filter 2 Enable
        CanFilterSel3 = 2u, ///< The Acceptance Filter 3 Enable
        CanFilterSel4 = 3u, ///< The Acceptance Filter 4 Enable
        CanFilterSel5 = 4u, ///< The Acceptance Filter 5 Enable
        CanFilterSel6 = 5u, ///< The Acceptance Filter 6 Enable
        CanFilterSel7 = 6u, ///< The Acceptance Filter 7 Enable
        CanFilterSel8 = 7u, ///< The Acceptance Filter 8 Enable
    } en_can_filter_sel_t;

    /**
     *******************************************************************************
     ** \brief  The can interrupt enable.(IE)
     ******************************************************************************/
    typedef enum
    {
        //<<Can Rx or Tx Irq En
        CanRxIrqEn = 0x00000080,              ///< Receive interrupt enable
        CanRxOverIrqEn = 0x00000040,          ///< RB overrun interrupt enable
        CanRxBufFullIrqEn = 0x00000020,       ///< RB full interrupt enable
        CanRxBufAlmostFullIrqEn = 0x00000010, ///< RB almost full interrupt enable
        CanTxPrimaryIrqEn = 0x00000008,       ///< Transmission primary interrupt enable
        CanTxSecondaryIrqEn = 0x00000004,     ///< Transmission secondary enable
        CanErrorIrqEn = 0x00000002,           ///< Error interrupt enable

        //<<Can Error Irq En
        CanErrorPassiveIrqEn = 0x00200000, ///< Error passive mode active enable
        CanArbiLostIrqEn = 0x00080000,     ///< Arbitration lost interrupt enable
        CanBusErrorIrqEn = 0x00020000,     ///< Bus error interrupt enable

    } en_can_irq_type_t;

    /**
     *******************************************************************************
     ** \brief  The can interrupt flag.(IF)
     ******************************************************************************/
    typedef enum
    {
        //<<Can Tx or Tx Irq Flg
        CanTxBufFullIrqFlg = 0x00000001,       ///<
        CanRxIrqFlg = 0x00008000,              ///< Receive interrupt flag
        CanRxOverIrqFlg = 0x00004000,          ///< RB overrun interrupt flag
        CanRxBufFullIrqFlg = 0x00002000,       ///< RB full interrupt flag
        CanRxBufAlmostFullIrqFlg = 0x00001000, ///< RB almost full interrupt flag
        CanTxPrimaryIrqFlg = 0x00000800,       ///< Transmission primary interrupt flag
        CanTxSecondaryIrqFlg = 0x00000400,     ///< Transmission secondary interrupt flag
        CanErrorIrqFlg = 0x00000200,           ///< Error interrupt flag
        CanAbortIrqFlg = 0x00000100,           ///< Abort interrupt flag

        //<< Can Error Irq Flg
        CanErrorWarningIrqFlg = 0x00800000,     ///< Error warning limit reached flag
        CanErrorPassivenodeIrqFlg = 0x00400000, ///< Error passive mode active flag
        CanErrorPassiveIrqFlg = 0x00100000,     ///< Error passive interrupt flag
        CanArbiLostIrqFlg = 0x00040000,         ///< Arbitration lost interrupt flag
        CanBusErrorIrqFlg = 0x00010000,         ///< Bus error interrupt flag
    } en_can_irq_flag_type_t;

    /**
     *******************************************************************************
     ** \brief  The can mode.(TCMD)
     ******************************************************************************/
    typedef enum
    {
        CanExternalLoopBackMode = 0x40,  ///< Loop back mode, external
        CanInternalLoopBackMode = 0x20,  ///< Loop back mode, internal
        CanTxSignalPrimaryMode = 0x10,   ///< Transmission primary single shot mode for PTB
        CanTxSignalSecondaryMode = 0x08, ///< Transmission secondary single shot mode for STB
        CanListenOnlyMode = 0xFF,        ///< Listen only mode
    } en_can_mode_t;

    /**
     *******************************************************************************
     ** \brief  The can status.(STAT)
     ******************************************************************************/
    typedef enum
    {
        CanRxActive = 0x04, ///< Reception active
        CanTxActive = 0x02, ///< Transmission active
        CanBusoff = 0x01,   ///< Bus off
    } en_can_status_t;

    /**
     *******************************************************************************
     ** \brief  The Can Tx Command.(TCMD)
     ******************************************************************************/
    typedef enum
    {
        CanPTBTxCmd = 0x10,      ///< Transmit primary for PTB
        CanPTBTxAbortCmd = 0x08, ///< Transmit primary abort for PTB
        CanSTBTxOneCmd = 0x04,   ///< Transmit secondary one frame for STB
        CanSTBTxAllCmd = 0x02,   ///< Transmit secondary all frames for STB
        CanSTBTxAbortCmd = 0x01, ///< Transmit secondary abort for STB
    } en_can_tx_cmd_t;

    /**
     *******************************************************************************
     ** \brief  The Can Transmit buffer secondary operation mode.(TCTRL)
     ******************************************************************************/
    typedef enum
    {
        CanSTBFifoMode = 0,    ///< FIFO mode
        CanSTBPrimaryMode = 1, ///< Priority decision mode
    } en_can_stb_mode_t;

    /**
     *******************************************************************************
     ** \brief  The Can Self-ACKnowledge.(RCTRL)
     ******************************************************************************/
    typedef enum
    {
        CanSelfAckEnable = 1,  ///< Self-ACK when LBME=1
        CanSelfAckDisable = 0, ///< no self-ACK
    } en_can_self_ack_en_t;

    /**
     *******************************************************************************
     ** \brief  The Can Receive Buffer Overflow Mode.(RCTRL)
     ******************************************************************************/
    typedef enum
    {
        CanRxBufOverwritten = 0, ///< The oldest message will be overwritten
        CanRxBufNotStored = 1,   ///< The new message will not be stored
    } en_can_rx_buf_mode_en_t;

    /**
     *******************************************************************************
     ** \brief  The Can Receive Buffer Stores All data frames.(RCTRL)
     ******************************************************************************/
    typedef enum
    {
        CanRxNormal = 0, ///< Normal operation
        CanRxAll = 1,    ///< RB stores correct data frames as well as data frames with error
    } en_can_rx_buf_all_t;

    /**
     *******************************************************************************
     ** \brief  The Can Receive Buffer Status.(RSTAT)
     ******************************************************************************/
    typedef enum
    {
        CanRxBufEmpty = 0,         ///< Empty
        CanRxBufnotAlmostFull = 1, ///< >empty and <almost full
        CanRxBufAlmostFull = 2,    ///< >=almost full, but not full and no overflow
        CanRxBufFull = 3,          ///< full
    } en_can_rx_buf_status_t;

    /**
     *******************************************************************************
     ** \brief  The Can Transmission secondary Status.(TSSTAT)
     ******************************************************************************/
    typedef enum
    {
        CanTxBufEmpty = 0,       ///< TTEN=0 or  TTTBM=0: STB is empty
                                 ///< TTEN=1 and TTTBM=1: PTB and STB are empty
        CanTxBufnotHalfFull = 1, ///< TTEN=0 or  TTTBM=0: STB is less than or equal to half full
                                 ///< TTEN=1 and TTTBM=1: PTB and STB are not empty and not full
        CanTxBufHalfFull = 2,    ///< TTEN=0 or  TTTBM=0: STB is more than half full
                                 ///< TTEN=1 and TTTBM=1: None
        CanTxBufFull = 3,        ///< TTEN=0 or  TTTBM=0: STB is full
                                 ///< TTEN=1 and TTTBM=1: PTB and STB are full
    } en_can_tx_buf_status_t;

    /**
     *******************************************************************************
     ** \brief  Configuration structure of CAN Acceptance Filter Code and Mask.
     ******************************************************************************/
    typedef struct stc_can_filter
    {
        uint32_t u32CODE;                   ///< Acceptance CODE
        uint32_t u32MASK;                   ///< Acceptance MASK
        en_can_filter_sel_t enFilterSel;    ///< The Acceptance Filters Enable
        en_can_acf_format_en_t enAcfFormat; ///< The Acceptance Filters Frame Format Check.
    } stc_can_filter_t;

    /**
     *******************************************************************************
     ** \brief  Configuration structure of CAN Bit Timing.
     ******************************************************************************/
    typedef struct stc_can_bt
    {
        uint8_t SEG_1; ///< Bit timing segment 1(Tseg_1 = (SEG_1 + 2)*TQ)
        uint8_t SEG_2; ///< Bit timing segment 2(Tseg_2 = (SEG_2 + 1)*TQ)
        uint8_t SJW;   ///< Synchronization jump width(Tsjw = (SJW + 1)*TQ)
        uint8_t PRESC; ///< The Prescaler divides the system clock to get the time quanta clock tq_clk(TQ)
    } stc_can_bt_t;

    /**
     *******************************************************************************
     ** \brief  Configuration structure of CAN Control Frame.
     ******************************************************************************/
    typedef struct
    {
        uint32_t DLC : 4;        ///< Data length code
        uint32_t RESERVED0 : 2;  ///< Ignore
        uint32_t RTR : 1;        ///< Remote transmission request
        uint32_t IDE : 1;        ///< IDentifier extension
        uint32_t RESERVED1 : 24; ///< Ignore
    } stc_can_txcontrol_t;

    /**
     *******************************************************************************
     ** \brief  Configuration structure of CAN Tx Frame.
     ******************************************************************************/
    typedef struct stc_can_txframe
    {
        union
        {
            uint32_t TBUF32_0; ///< Ignore
            uint32_t StdID;    ///< Standard ID
            uint32_t ExtID;    ///< Extended ID
        };
        union
        {
            uint32_t TBUF32_1;             ///< Ignore
            stc_can_txcontrol_t Control_f; ///< CAN Tx Control
        };
        union
        {
            uint32_t TBUF32_2[2]; ///< Ignore
            uint8_t Data[8];      ///< CAN data
        };
        en_can_buffer_sel_t enBufferSel; ///< CAN Tx buffer select

    } stc_can_txframe_t;

    /**
     *******************************************************************************
     ** \brief  Configuration structure of CAN Rx Ctrl.
     ******************************************************************************/
    typedef struct
    {
        uint8_t DLC : 4;       ///< Data length code
        uint8_t RESERVED0 : 2; ///< Ignore
        uint8_t RTR : 1;       ///< Remote transmission request
        uint8_t IDE : 1;       ///< IDentifier extension
    } stc_can_rxcontrol_t;

    /**
     *******************************************************************************
     ** \brief  Configuration structure of CAN status.
     ******************************************************************************/
    typedef struct
    {
        uint8_t RESERVED0 : 4; ///< Ignore
        uint8_t TX : 1;        ///< TX is set to 1 if the loop back mode is activated
        uint8_t KOER : 3;      ///< Kind of error
    } stc_can_status_t;

    /**
     *******************************************************************************
     ** \brief  Configuration structure of CAN control, status and cycletime.
     ******************************************************************************/
    typedef struct
    {
        stc_can_rxcontrol_t Control_f; ///< @ref stc_can_rxcontrol_t
        stc_can_status_t Status_f;     ///< @ref stc_can_status_t
        uint16_t CycleTime;            ///< TTCAN cycletime
    } stc_can_cst_t;

    /**
     *******************************************************************************
     ** \brief  Configuration structure of CAN Rx frame.
     ******************************************************************************/
    typedef struct stc_can_rxframe
    {
        union
        {
            uint32_t RBUF32_0; ///< Ignore
            uint32_t StdID;    ///< Standard ID
            uint32_t ExtID;    ///< Extended ID
        };
        union
        {
            uint32_t RBUF32_1; ///< Ignore
            stc_can_cst_t Cst; ///< @ref stc_can_cst_t
        };
        union
        {
            uint32_t RBUF32_2[2]; ///< Ignore
            uint8_t Data[8];      ///< CAN data
        };

    } stc_can_rxframe_t;

    /**
     *******************************************************************************
     ** \brief  Configuration structure of CAN Rx frame.
     ******************************************************************************/
    typedef struct stc_can_init_config
    {
        en_can_rx_buf_all_t enCanRxBufAll;       ///< @ref en_can_rx_buf_all_t
        en_can_rx_buf_mode_en_t enCanRxBufMode;  ///< @ref en_can_rx_buf_mode_en_t
        en_can_self_ack_en_t enCanSAck;          ///< @ref en_can_self_ack_en_t
        en_can_stb_mode_t enCanSTBMode;          ///< @ref en_can_stb_mode_t
        stc_can_bt_t stcCanBt;                   ///< @ref stc_can_bt_t
        stc_can_warning_limit_t stcWarningLimit; ///< @ref stc_can_warning_limit_t
    } stc_can_init_config_t;

    /**
     *******************************************************************************
     ** \brief                         CAN TTCAN
     ******************************************************************************/
    /**
     *******************************************************************************
     ** \brief  Configuration structure of CAN TTCAN pointer to a TB message slot
     ******************************************************************************/
    typedef enum
    {
        CanTTcanPTBSel = 0x00u,  ///< PTB
        CanTTcanSTB1Sel = 0x01u, ///< STB1
        CanTTcanSTB2Sel = 0x02u, ///< STB2
        CanTTcanSTB3Sel = 0x03u, ///< STB3
        CanTTcanSTB4Sel = 0x04u, ///< STB4
    } en_can_ttcan_tbslot_t;

    /**
     *******************************************************************************
     ** \brief  Configuration structure of CAN TTCAN Timer prescaler
     ******************************************************************************/
    typedef enum
    {
        CanTTcanTprescDiv1 = 0x00u, ///< Div1
        CanTTcanTprescDiv2 = 0x01u, ///< Div2
        CanTTcanTprescDiv3 = 0x02u, ///< Div3
        CanTTcanTprescDiv4 = 0x03u, ///< Div4
    } en_can_ttcan_Tpresc_t;

    /**
     *******************************************************************************
     ** \brief  Configuration structure of CAN TTCAN Trigger type
     ******************************************************************************/
    typedef enum
    {
        CanTTcanImmediate = 0x00,  ///< Immediate trigger for immediate transmission
        CanTTcanTime = 0x01,       ///< Time trigger for receive trigger
        CanTTcanSingle = 0x02,     ///< Single shot transmit trigger for exclusive time windows
        CanTTcanTransStart = 0x03, ///< Transmit start trigger for merged arbitrating time windows
        CanTTcanTransStop = 0x04,  ///< Transmit stop trigger for merged arbitrating time windows
    } en_can_ttcan_trigger_type_t;

    typedef enum
    {
        CanTTcanWdtTriggerIrq = 0x80, ///< Watch trigger interrupt
        CanTTcanTimTriggerIrq = 0x10, ///< Time trigger interrupt
    } en_can_ttcan_irq_type_t;

    typedef struct stc_can_ttcan_ref_msg
    {
        uint8_t u8IDE; ///< Reference message IDE:1-Extended; 0-Standard;
        union          ///< Reference message ID
        {
            uint32_t RefStdID; ///< Reference standard ID
            uint32_t RefExtID; ///< Reference Extended ID
        };
    } stc_can_ttcan_ref_msg_t;

    typedef struct stc_can_ttcan_trigger_config
    {
        en_can_ttcan_tbslot_t enTbSlot;         ///< Transmit trigger TB slot pointer
        en_can_ttcan_trigger_type_t enTrigType; ///< Trigger type
        en_can_ttcan_Tpresc_t enTpresc;         ///< Timer prescaler
        uint8_t u8Tew;                          ///< Transmit enable window
        uint16_t u16TrigTime;                   ///< TTCAN trigger time
        uint16_t u16WatchTrigTime;              ///< TTCAN watch trigger time register
    } stc_can_ttcan_trigger_config_t;

    /*******************************************************************************
     * Global pre-processor symbols/macros ('#define')
     ******************************************************************************/

    /*******************************************************************************
     * Global variable definitions ('extern')
     ******************************************************************************/

    /*******************************************************************************
     * Global function prototypes (definition in C source)
     ******************************************************************************/
    void CAN_Init(stc_can_init_config_t *pstcCanInitCfg);
    void CAN_DeInit(void);
    void CAN_IrqCmd(en_can_irq_type_t enCanIrqType, en_functional_state_t enNewState);
    bool CAN_IrqFlgGet(en_can_irq_flag_type_t enCanIrqFlgType);
    void CAN_IrqFlgClr(en_can_irq_flag_type_t enCanIrqFlgType);
    void CAN_ModeConfig(en_can_mode_t enMode, en_functional_state_t enNewState);
    en_can_error_t CAN_ErrorStatusGet(void);
    bool CAN_StatusGet(en_can_status_t enCanStatus);

    void CAN_FilterConfig(const stc_can_filter_t *pstcFilter, en_functional_state_t enNewState);
    void CAN_SetFrame(stc_can_txframe_t *pstcTxFrame);
    en_can_tx_buf_status_t CAN_TransmitCmd(en_can_tx_cmd_t enTxCmd);
    en_can_rx_buf_status_t CAN_Receive(stc_can_rxframe_t *pstcRxFrame);

    uint8_t CAN_ArbitrationLostCap(void);
    uint8_t CAN_RxErrorCntGet(void);
    uint8_t CAN_TxErrorCntGet(void);

    //<< void CAN_TTCAN_Enable(void);
    //<< void CAN_TTCAN_Disable(void);
    //<< void CAN_TTCAN_IrqCmd(void);
    //<< void CAN_TTCAN_ReferenceMsgSet(stc_can_ttcan_ref_msg_t *pstcRefMsg);
    //<< void CAN_TTCAN_TriggerConfig(stc_can_ttcan_trigger_config_t *pstcTriggerCfg);

    //@} // CanGroup

#ifdef __cplusplus
}
#endif

#endif /* DDL_CAN_ENABLE */

#endif /* __HC32F46X_CAN_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
