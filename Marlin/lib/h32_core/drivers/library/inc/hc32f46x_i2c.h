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
/** \file hc32f46x_i2c.h
 **
 ** A detailed description is available at
 ** @link I2cGroup Inter-Integrated Circuit(I2C) description @endlink
 **
 **   - 2018-10-16  1.0  Wangmin  First version for Device Driver Library of I2C.
 **
 ******************************************************************************/

#ifndef __HC32F46x_I2C_H__
#define __HC32F46x_I2C_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_common.h"
#include "ddl_config.h"

#if (DDL_I2C_ENABLE == DDL_ON)

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

    /**
     *******************************************************************************
     ** \defgroup I2cGroup Inter-Integrated Circuit (I2C)
     **
     ******************************************************************************/
    //@{

    /*******************************************************************************
     * Global type definitions ('typedef')
     ******************************************************************************/
    /**
     *******************************************************************************
     ** \brief I2c mode enumeration
     **
     ******************************************************************************/
    typedef enum en_i2c_mode
    {
        I2cMaster = 0u, ///< I2C master mode
        I2cSlave,       ///< I2C slave mode
    } en_i2c_mode_t;

    /**
     *******************************************************************************
     ** \brief I2c configuration structure
     **
     ******************************************************************************/
    typedef struct stc_i2c_init
    {
        en_i2c_mode_t enI2cMode; ///< I2C mode config, master or slave
        uint32_t u32Pclk3;       ///< Plck3 frequency
        uint32_t u32Baudrate;    ///< I2C baudrate config
        uint32_t u32SclTime;     ///< The SCL rising and falling time, count of T(pclk3)
    } stc_i2c_init_t;

    /**
     *******************************************************************************
     ** \brief I2c SMBUS configuration structure
     **
     ******************************************************************************/
    typedef struct stc_i2c_smbus_init
    {
        en_functional_state_t enHostAdrMatchFunc;    ///< SMBUS host address matching function
        en_functional_state_t enDefaultAdrMatchFunc; ///< SMBUS default address matching function
        en_functional_state_t enAlarmAdrMatchFunc;   ///< SMBUS Alarm address matching function
    } stc_i2c_smbus_init_t;

    /**
     *******************************************************************************
     ** \brief I2c digital filter mode enumeration
     **
     ******************************************************************************/
    typedef enum en_i2c_digital_filter_mode
    {
        Filter1BaseCycle = 0u, ///< I2C digital filter ability 1 base cycle
        Filter2BaseCycle = 1u, ///< I2C digital filter ability 2 base cycle
        Filter3BaseCycle = 2u, ///< I2C digital filter ability 3 base cycle
        Filter4BaseCycle = 3u, ///< I2C digital filter ability 4 base cycle
    } en_i2c_digital_filter_mode_t;

    /**
     *******************************************************************************
     ** \brief I2c address bit enumeration
     **
     ******************************************************************************/
    typedef enum en_address_bit
    {
        Adr7bit = 0u,  ///< I2C address length is 7 bits
        Adr10bit = 1u, ///< I2C address length is 10 bits
    } en_address_bit_t;

    /**
     *******************************************************************************
     ** \brief I2c clock timeout switch enumeration
     **
     ******************************************************************************/
    typedef enum en_clock_timeout_switch
    {
        TimeoutFunOff = 0u, ///< I2C SCL pin time out function off
        LowTimerOutOn = 3u, ///< I2C SCL pin high level time out function on
        HighTimeOutOn = 5u, ///< I2C SCL pin low level time out function on
        BothTimeOutOn = 7u, ///< I2C SCL pin both(low and high) level time out function on
    } en_clock_timeout_switch_t;

    /**
     *******************************************************************************
     ** \brief I2c clock timeout initialize structure
     **
     ******************************************************************************/
    typedef struct stc_clock_timeout_init
    {
        en_clock_timeout_switch_t enClkTimeOutSwitch; ///< I2C clock timeout function switch
        uint16_t u16TimeOutHigh;                      ///< I2C clock timeout period for High level
        uint16_t u16TimeOutLow;                       ///< I2C clock timeout period for Low level
    } stc_clock_timeout_init_t;

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* define interrupt enable bit for I2C_CR2 register */
#define I2C_CR2_STARTIE (0x00000001ul)
#define I2C_CR2_SLADDR0EN (0x00000002ul)
#define I2C_CR2_SLADDR1EN (0x00000004ul)
#define I2C_CR2_TENDIE (0x00000008ul)
#define I2C_CR2_STOPIE (0x00000010ul)
#define I2C_CR2_RFULLIE (0x00000040ul)
#define I2C_CR2_TEMPTYIE (0x00000080ul)
#define I2C_CR2_ARLOIE (0x00000200ul)
#define I2C_CR2_NACKIE (0x00001000ul)
#define I2C_CR2_TMOURIE (0x00004000ul)
#define I2C_CR2_GENCALLIE (0x00100000ul)
#define I2C_CR2_SMBDEFAULTIE (0x00200000ul)
#define I2C_CR2_SMBHOSTIE (0x00400000ul)
#define I2C_CR2_SMBALRTIE (0x00800000ul)

/* define status bit for I2C_SR register */
#define I2C_SR_STARTF (0x00000001ul)
#define I2C_SR_SLADDR0F (0x00000002ul)
#define I2C_SR_SLADDR1F (0x00000004ul)
#define I2C_SR_TENDF (0x00000008ul)
#define I2C_SR_STOPF (0x00000010ul)
#define I2C_SR_RFULLF (0x00000040ul)
#define I2C_SR_TEMPTYF (0x00000080ul)
#define I2C_SR_ARLOF (0x00000200ul)
#define I2C_SR_NACKDETECTF (0x00000400ul)
#define I2C_SR_NACKSENDF (0x00001000ul)
#define I2C_SR_TMOUTF (0x00004000ul)
#define I2C_SR_MSL (0x00010000ul)
#define I2C_SR_BUSY (0x00020000ul)
#define I2C_SR_TRA (0x00040000ul)
#define I2C_SR_GENCALLF (0x00100000ul)
#define I2C_SR_SMBDEFAULTF (0x00200000ul)
#define I2C_SR_SMBHOSTF (0x00400000ul)
#define I2C_SR_SMBALRTF (0x00800000ul)

/* define status clear bit for I2C_CLR register*/
#define I2C_CLR_STARTFCLR (0x00000001ul)
#define I2C_CLR_SLADDR0FCLR (0x00000002ul)
#define I2C_CLR_SLADDR1FCLR (0x00000004ul)
#define I2C_CLR_TENDFCLR (0x00000008ul)
#define I2C_CLR_STOPFCLR (0x00000010ul)
#define I2C_CLR_RFULLFCLR (0x00000040ul)
#define I2C_CLR_TEMPTYFCLR (0x00000080ul)
#define I2C_CLR_ARLOFCLR (0x00000200ul)
#define I2C_CLR_NACKFCLR (0x00001000ul)
#define I2C_CLR_TMOUTFCLR (0x00004000ul)
#define I2C_CLR_GENCALLFCLR (0x00100000ul)
#define I2C_CLR_SMBDEFAULTFCLR (0x00200000ul)
#define I2C_CLR_SMBHOSTFCLR (0x00400000ul)
#define I2C_CLR_SMBALRTFCLR (0x00800000ul)
#define I2C_CLR_MASK (0x00F056DFul)
    /*******************************************************************************
     * Global variable definitions ('extern')
     ******************************************************************************/

    /*******************************************************************************
      Global function prototypes (definition in C source)
     ******************************************************************************/
    void I2C_BaudrateConfig(M4_I2C_TypeDef *pstcI2Cx, uint32_t u32Baudrate, uint32_t u32SclTime, uint32_t u32Pclk3);
    en_result_t I2C_DeInit(M4_I2C_TypeDef *pstcI2Cx);
    en_result_t I2C_Init(M4_I2C_TypeDef *pstcI2Cx, const stc_i2c_init_t *pstcI2C_InitStruct);
    void I2C_Cmd(M4_I2C_TypeDef *pstcI2Cx, en_functional_state_t enNewState);
    en_result_t I2C_SmbusConfig(M4_I2C_TypeDef *pstcI2Cx, const stc_i2c_smbus_init_t *pstcI2C_SmbusInitStruct);
    void I2C_SmBusCmd(M4_I2C_TypeDef *pstcI2Cx, en_functional_state_t enNewState);
    void I2C_SoftwareResetCmd(M4_I2C_TypeDef *pstcI2Cx, en_functional_state_t enNewState);

    ////////////////////////////////////////////////////////////////////////////////////////
    void I2C_DigitalFilterConfig(M4_I2C_TypeDef *pstcI2Cx, en_i2c_digital_filter_mode_t enDigiFilterMode);
    void I2C_DigitalFilterCmd(M4_I2C_TypeDef *pstcI2Cx, en_functional_state_t enNewState);
    void I2C_AnalogFilterCmd(M4_I2C_TypeDef *pstcI2Cx, en_functional_state_t enNewState);
    void I2C_GeneralCallCmd(M4_I2C_TypeDef *pstcI2Cx, en_functional_state_t enNewState);
    void I2C_SlaveAdr0Config(M4_I2C_TypeDef *pstcI2Cx, en_functional_state_t enNewState, en_address_bit_t enAdrMode, uint8_t u8Adr);
    void I2C_SlaveAdr1Config(M4_I2C_TypeDef *pstcI2Cx, en_functional_state_t enNewState, en_address_bit_t enAdrMode, uint8_t u8Adr);
    en_result_t I2C_ClkTimeOutConfig(M4_I2C_TypeDef *pstcI2Cx, const stc_clock_timeout_init_t *pstcTimoutInit);
    void I2C_IntCmd(M4_I2C_TypeDef *pstcI2Cx, uint32_t u32IntEn, en_functional_state_t enNewState);
    void I2C_FastAckConfig(M4_I2C_TypeDef *pstcI2Cx, en_functional_state_t enNewState);

    ///////////////////////////////////////////////////////////////////////////////////////
    void I2C_GenerateStart(M4_I2C_TypeDef *pstcI2Cx, en_functional_state_t enNewState);
    void I2C_GenerateReStart(M4_I2C_TypeDef *pstcI2Cx, en_functional_state_t enNewState);
    void I2C_GenerateStop(M4_I2C_TypeDef *pstcI2Cx, en_functional_state_t enNewState);
    void I2C_SendData(M4_I2C_TypeDef *pstcI2Cx, uint8_t u8Data);
    uint8_t I2C_ReadData(M4_I2C_TypeDef *pstcI2Cx);
    void I2C_NackConfig(M4_I2C_TypeDef *pstcI2Cx, en_functional_state_t enNewState);
    en_flag_status_t I2C_GetStatus(M4_I2C_TypeDef *pstcI2Cx, uint32_t u32StatusBit);
    void I2C_WriteStatus(M4_I2C_TypeDef *pstcI2Cx, uint32_t u32StatusBit, en_flag_status_t enStatus);
    void I2C_ClearStatus(M4_I2C_TypeDef *pstcI2Cx, uint32_t u32StatusBit);

    //@} // I2cGroup

#ifdef __cplusplus
}
#endif

#endif /* DDL_I2C_ENABLE */

#endif /* __HC32F46x_I2C_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
