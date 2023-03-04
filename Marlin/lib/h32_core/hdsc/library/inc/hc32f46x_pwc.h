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
/** \file hc32f46x_pwc.h
 **
 ** A detailed description is available at
 ** @link PwcGroup PWC description @endlink
 **
 **   - 2018-10-28  1.0  Chengy First version for Device Driver Library of PWC.
 **
 ******************************************************************************/
#ifndef __HC32F46X_PWC_H__
#define __HC32F46X_PWC_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_common.h"
#include "ddl_config.h"

#if (DDL_PWC_ENABLE == DDL_ON)

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

    /**
     *******************************************************************************
     ** \defgroup PwcGroup Power Control(PWC)
     **
     ******************************************************************************/
    //@{

    /*******************************************************************************
     * Global type definitions ('typedef')
     ******************************************************************************/
    /**
     *******************************************************************************
     ** \brief  The power down mode.
     **
     ******************************************************************************/
    typedef enum en_pwc_powerdown_md
    {
        PowerDownMd1 = 0u, ///< Power down mode 1.
        PowerDownMd2 = 1u, ///< Power down mode 2.
        PowerDownMd3 = 2u, ///< Power down mode 3.
        PowerDownMd4 = 3u, ///< Power down mode 4.
    } en_pwc_powerdown_md_t;

    /**
     *******************************************************************************
     ** \brief  The IO retain status under power down mode.
     **
     ******************************************************************************/
    typedef enum en_pwc_iortn
    {
        IoPwrDownRetain = 0u, ///< Io keep under power down mode.
        IoPwrRstRetain = 1u,  ///< Io keep after power reset.
        IoHighImp = 2u,       ///< IO high impedance either power down or power reset.
    } en_pwc_iortn_t;

    /**
     *******************************************************************************
     ** \brief  The driver ability while different speed mode enter stop mode.
     **
     ******************************************************************************/
    typedef enum en_pwc_stopdas
    {
        StopHighspeed = 0u, ///< The driver ability while high speed mode enter stop mode.
        StopUlowspeed = 3u, ///< The driver ability while ultra_low speed mode enter stop mode.
    } en_pwc_stopdas_t;

    /**
     *******************************************************************************
     ** \brief  The dynamic voltage scaling.
     **
     ******************************************************************************/
    typedef enum en_pwc_dynvol_sca
    {
        Voltage09 = 2u, ///< The voltage 0.9V.
        Voltage11 = 3u, ///< The voltage 1.1V.
    } en_pwc_dvoltage_sca_t;

    /**
     *******************************************************************************
     ** \brief  The dynamic power driver ability scaling.
     **
     ******************************************************************************/
    typedef enum en_pwc_drvability_sca
    {
        Ulowspeed = 8u,  ///< The ultra_low speed.
        HighSpeed = 15u, ///< The high speed.
    } en_pwc_drvability_sca_t;

    /**
     *******************************************************************************
     ** \brief  The power down wake up time select.
     **
     ******************************************************************************/
    typedef enum en_pwc_waketime_sel
    {
        Vcap01 = 0u,   ///< Wake up while vcap capacitance 2*0.1uf.
        Vcap0047 = 1u, ///< Wake up while vcap capacitance 2*0.047uf.
    } en_pwc_waketime_sel_t;

    /**
     *******************************************************************************
     ** \brief  The wait or not wait flash stable while stop mode awake.
     **
     ******************************************************************************/
    typedef enum en_pwc_stop_flash_sel
    {
        FlashWait = 0u,    ///< wait flash stable.
        FlashNotWait = 1u, ///< Not Wait flash stable.
    } en_pwc_stop_flash_sel_t;

    /**
     *******************************************************************************
     ** \brief  The clk value while stop mode awake.
     **
     ******************************************************************************/
    typedef enum en_pwc_stop_clk_sel
    {
        ClkFix = 0u, ///< clock fix.
        ClkMrc = 1u, ///< clock source is MRC, only ram code.
    } en_pwc_stop_clk_sel_t;

    /**
     *******************************************************************************
     ** \brief  The power down wake up event edge select.
     **
     ******************************************************************************/
    typedef enum en_pwc_edge_sel
    {
        EdgeFalling = 0u, ///< Falling edge.
        EdgeRising = 1u,  ///< Rising edge.
    } en_pwc_edge_sel_t;

    /**
     *******************************************************************************
     ** \brief  The voltage detect edge select.
     **
     ******************************************************************************/
    typedef enum en_pwc_pvdedge_sel
    {
        OverVcc = 0u,  ///< PVD > VCC.
        BelowVcc = 1u, ///< PVD < VCC.
    } en_pwc_pvdedge_sel_t;

    /**
     *******************************************************************************
     ** \brief  The flag of wake_up timer compare result.
     **
     ******************************************************************************/
    typedef enum en_pwc_wkover_flag
    {
        UnEqual = 0u, ///< Timer value unequal with the wake_up compare value whitch set.
        Equal = 1u,   ///< Timer value equal with the wake_up compare value whitch set..
    } en_pwc_wkover_flag_t;

    /**
     *******************************************************************************
     ** \brief  The RAM power down control.
     **
     ******************************************************************************/
    typedef enum en_pwc_ram_pwr_ctl
    {
        DynamicCtl = 0u, ///< Ram dynamic.
        PowDownCtl = 1u, ///< Ram power down.
    } en_pwc_ram_pwr_ctl_t;

    /**
     *******************************************************************************
     ** \brief  The RAM operating mode.
     **
     ******************************************************************************/
    typedef enum en_pwc_ram_op_md
    {
        HighSpeedMd = 0x8043, ///< Work at high speed.
        UlowSpeedMd = 0x9062, ///< Work at ultra low speed.
    } en_pwc_ram_op_md_t;

    /**
     *******************************************************************************
     ** \brief  The wake up clock select.
     **
     ******************************************************************************/
    typedef enum en_pwc_wkclk_sel
    {
        Wk64hz = 0u,   ///< 64Hz.
        WkXtal32 = 1u, ///< Xtal32.
        WkLrc = 2u,    ///< Lrc.
    } en_pwc_wkclk_sel_t;

    /**
     *******************************************************************************
     ** \brief  The pvd digital filtering sampling clock select.
     **
     ******************************************************************************/
    typedef enum en_pwc_pvdfiltclk_sel
    {
        PvdLrc025 = 0u, ///< 0.25 LRC cycle.
        PvdLrc05 = 1u,  ///< 0.5 LRC cycle.
        PvdLrc1 = 2u,   ///< LRC 1 div.
        PvdLrc2 = 3u,   ///< LRC 2 div.
    } en_pwc_pvdfiltclk_sel_t;

    /**
     *******************************************************************************
     ** \brief  The pvd2 level select.
     **
     ******************************************************************************/
    typedef enum en_pwc_pvd2level_sel
    {
        Pvd2Level21 = 0u, ///< 2.1V.
        Pvd2Level23 = 1u, ///< 2.3V.
        Pvd2Level25 = 2u, ///< 2.5V.
        Pvd2Level26 = 3u, ///< 2.6V.
        Pvd2Level27 = 4u, ///< 2.7V.
        Pvd2Level28 = 5u, ///< 2.8V.
        Pvd2Level29 = 6u, ///< 2.9V.
        Pvd2Level11 = 7u, ///< 1.1V.
    } en_pwc_pvd2level_sel_t;

    /**
     *******************************************************************************
     ** \brief  The pvd1 level select.
     **
     ******************************************************************************/
    typedef enum en_pwc_pvd1level_sel
    {
        Pvd1Level20 = 0u, ///< 2.0V.
        Pvd1Level21 = 1u, ///< 2.1V.
        Pvd1Level23 = 2u, ///< 2.3V.
        Pvd1Level25 = 3u, ///< 2.5V.
        Pvd1Level26 = 4u, ///< 2.6V.
        Pvd1Level27 = 5u, ///< 2.7V.
        Pvd1Level28 = 6u, ///< 2.8V.
        Pvd1Level29 = 7u, ///< 2.9V.
    } en_pwc_pvd1level_sel_t;

    /**
     *******************************************************************************
     ** \brief  The pvd interrupt select.
     **
     ******************************************************************************/
    typedef enum en_pwc_pvd_int_sel
    {
        NonMskInt = 0u, ///< Non-maskable Interrupt.
        MskInt = 1u,    ///< Maskable Interrupt.
    } en_pwc_pvd_int_sel_t;

    /**
     *******************************************************************************
     ** \brief  The handle of pvd mode.
     **
     ******************************************************************************/
    typedef enum en_pwc_pvd_md
    {
        PvdInt = 0u,   ///< The handle of pvd is interrupt.
        PvdReset = 1u, ///< The handle of pvd is reset.
    } en_pwc_pvd_md_t;

    /**
     *******************************************************************************
     ** \brief  The flag of pvd detect.
     **
     ******************************************************************************/
    typedef enum en_pwc_pvd_flag
    {
        Pvd1Flag = 0u, ///< The flag of pvd1 detect.
        Pvd2Flag = 1u, ///< The flag of pvd2 detect.
    } en_pwc_pvd_flag_t;

    /**
     *******************************************************************************
     ** \brief  The power mode  configuration.
     **
     ******************************************************************************/
    typedef struct stc_pwc_pwr_mode_cfg
    {
        en_pwc_powerdown_md_t enPwrDownMd;    ///< Power down mode.
        en_functional_state_t enRLdo;         ///< Enable or disable RLDO.
        en_functional_state_t enRetSram;      ///< Enable or disable Ret_Sram.
        en_pwc_iortn_t enIoRetain;            ///< IO retain.
        en_functional_state_t enVPll;         ///< Enable or disable PLL vcc.
        en_functional_state_t enVHrc;         ///< Enable or disable HRC vcc.
        en_pwc_drvability_sca_t enDrvAbility; ///< Driver ability scale.
        en_pwc_dvoltage_sca_t enDynVol;       ///< Dynamic voltage.
        en_pwc_waketime_sel_t enPwrDWkupTm;   ///< The power down wake up time select.
    } stc_pwc_pwr_mode_cfg_t;

    /**
     *******************************************************************************
     ** \brief  The stop mode configuration.
     **
     ******************************************************************************/
    typedef struct stc_pwc_stop_mode_cfg
    {
        en_pwc_stopdas_t enStpDrvAbi;        ///< Driver ability while enter stop mode.
        en_pwc_stop_flash_sel_t enStopFlash; ///< Flash mode while stop mode awake.
        en_pwc_stop_clk_sel_t enStopClk;     ///< Clock value while stop mode awake.
        en_functional_state_t enPll;         ///< Whether the PLL enable or disable while enter stop mode.
    } stc_pwc_stop_mode_cfg_t;

    /**
     *******************************************************************************
     ** \brief  The power down wake_up event configuration.
     **
     ******************************************************************************/
    typedef struct stc_pwc_wkup_edge_cfg
    {
        en_pwc_edge_sel_t enPtwk0Edge;   ///< Ptwk0 edge cfg (rising or falling).
        en_pwc_edge_sel_t enPtwk1Edge;   ///< Ptwk1 edge cfg (rising or falling).
        en_pwc_edge_sel_t enPtwk2Edge;   ///< Ptwk2 edge cfg (rising or falling).
        en_pwc_edge_sel_t enPtwk3Edge;   ///< Ptwk2 edge cfg (rising or falling).
        en_pwc_pvdedge_sel_t enPvd1Edge; ///< Pvd1 edge cfg.
        en_pwc_pvdedge_sel_t enPvd2Edge; ///< Pvd1 edge cfg.
        en_pwc_edge_sel_t enNmiEdge;     ///< Ptwk2 edge cfg (rising or falling).
    } stc_pwc_wkup_edge_cfg_t;

    /**
     *******************************************************************************
     ** \brief  The Ram config(include operating mode & power down control).
     **
     ******************************************************************************/
    typedef struct stc_pwc_ram_cfg
    {
        en_pwc_ram_pwr_ctl_t enRam0;   ///< Ram0(0x20000000-0x2000FFFF) power down control.
        en_pwc_ram_pwr_ctl_t enRam1;   ///< Ram1(0x20010000-0x2001FFFF).
        en_pwc_ram_pwr_ctl_t enRam2;   ///< Ram2(0x20020000-0x20026FFF).
        en_pwc_ram_pwr_ctl_t enRam3;   ///< Rom3(0x1FFF8000-0x1FFFFFFF).
        en_pwc_ram_pwr_ctl_t enUsbfs;  ///< Usbfs.
        en_pwc_ram_pwr_ctl_t enSdioc0; ///< Sdioc0.
        en_pwc_ram_pwr_ctl_t enSdioc1; ///< Sdioc1.
        en_pwc_ram_pwr_ctl_t enCan;    ///< Can.
        en_pwc_ram_pwr_ctl_t enCache;  ///< Cache.
        en_pwc_ram_op_md_t enRamOpMd;  ///< Ram operate mode(high or ultra low speed).
    } stc_pwc_ram_cfg_t;

    /**
     *******************************************************************************
     ** \brief  The power down wake_up timer control.
     **
     ******************************************************************************/
    typedef struct stc_pwc_wktm_ctl
    {
        uint16_t u16WktmCmp;               ///< The wake_up timer compare value.
        en_pwc_wkover_flag_t enWkOverFlag; ///< The flag of compare result.
        en_pwc_wkclk_sel_t enWkclk;        ///< The clock of wake_up timer.
        en_functional_state_t enWktmEn;    ///< Enable or disable wake_up timer.
    } stc_pwc_wktm_ctl_t;

    /**
     *******************************************************************************
     ** \brief  The pvd control.
     **
     ******************************************************************************/
    typedef struct stc_pwc_pvd_ctl
    {
        en_functional_state_t enPvdIREn;     ///< Enable or disable pvd interrupt(reset).
        en_pwc_pvd_md_t enPvdMode;           ///< The handle of pvd is interrupt or reset.
        en_functional_state_t enPvdCmpOutEn; ///< Enable or disable pvd output compare result .
    } stc_pwc_pvd_ctl_t;

    /**
     *******************************************************************************
     ** \brief  The power down wake_up event configuration.
     **
     ******************************************************************************/
    typedef struct stc_pwc_pvd_cfg
    {
        stc_pwc_pvd_ctl_t stcPvd1Ctl;          ///< Pvd1 control configuration.
        stc_pwc_pvd_ctl_t stcPvd2Ctl;          ///< Pvd2 control configuration.
        en_functional_state_t enPvd1FilterEn;  ///< Pvd1 filtering enable or disable.
        en_functional_state_t enPvd2FilterEn;  ///< Pvd2 filtering enable or disable.
        en_pwc_pvdfiltclk_sel_t enPvd1Filtclk; ///< Pvd1 filtering sampling clock.
        en_pwc_pvdfiltclk_sel_t enPvd2Filtclk; ///< Pvd2 filtering sampling clock.
        en_pwc_pvd1level_sel_t enPvd1Level;    ///< Pvd1 voltage.
        en_pwc_pvd2level_sel_t enPvd2Level;    ///< Pvd2 voltage.
        en_pwc_pvd_int_sel_t enPvd1Int;        ///< Pvd1 interrupt.
        en_pwc_pvd_int_sel_t enPvd2Int;        ///< Pvd2 interrupt.
    } stc_pwc_pvd_cfg_t;

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define PWC_PDWKEN0_WKUP00 ((uint8_t)0x01)
#define PWC_PDWKEN0_WKUP01 ((uint8_t)0x02)
#define PWC_PDWKEN0_WKUP02 ((uint8_t)0x04)
#define PWC_PDWKEN0_WKUP03 ((uint8_t)0x08)
#define PWC_PDWKEN0_WKUP10 ((uint8_t)0x10)
#define PWC_PDWKEN0_WKUP11 ((uint8_t)0x20)
#define PWC_PDWKEN0_WKUP12 ((uint8_t)0x40)
#define PWC_PDWKEN0_WKUP13 ((uint8_t)0x80)

#define PWC_PDWKEN1_WKUP20 ((uint8_t)0x01)
#define PWC_PDWKEN1_WKUP21 ((uint8_t)0x02)
#define PWC_PDWKEN1_WKUP22 ((uint8_t)0x04)
#define PWC_PDWKEN1_WKUP23 ((uint8_t)0x08)
#define PWC_PDWKEN1_WKUP30 ((uint8_t)0x10)
#define PWC_PDWKEN1_WKUP31 ((uint8_t)0x20)
#define PWC_PDWKEN1_WKUP32 ((uint8_t)0x40)
#define PWC_PDWKEN1_WKUP33 ((uint8_t)0x80)

#define PWC_PDWKEN2_PVD1 ((uint8_t)0x01)
#define PWC_PDWKEN2_PVD2 ((uint8_t)0x02)
#define PWC_PDWKEN2_NMI ((uint8_t)0x04)
#define PWC_PDWKEN2_RTCPRD ((uint8_t)0x10)
#define PWC_PDWKEN2_RTCAL ((uint8_t)0x20)
#define PWC_PDWKEN2_XTAL32 ((uint8_t)0x40)
#define PWC_PDWKEN2_WKTM ((uint8_t)0x80)

#define PWC_STOPWKUPEN_EIRQ0 ((uint32_t)0x00000001)
#define PWC_STOPWKUPEN_EIRQ1 ((uint32_t)0x00000002)
#define PWC_STOPWKUPEN_EIRQ2 ((uint32_t)0x00000004)
#define PWC_STOPWKUPEN_EIRQ3 ((uint32_t)0x00000008)
#define PWC_STOPWKUPEN_EIRQ4 ((uint32_t)0x00000010)
#define PWC_STOPWKUPEN_EIRQ5 ((uint32_t)0x00000020)
#define PWC_STOPWKUPEN_EIRQ6 ((uint32_t)0x00000040)
#define PWC_STOPWKUPEN_EIRQ7 ((uint32_t)0x00000080)
#define PWC_STOPWKUPEN_EIRQ8 ((uint32_t)0x00000100)
#define PWC_STOPWKUPEN_EIRQ9 ((uint32_t)0x00000200)
#define PWC_STOPWKUPEN_EIRQ10 ((uint32_t)0x00000400)
#define PWC_STOPWKUPEN_EIRQ11 ((uint32_t)0x00000800)
#define PWC_STOPWKUPEN_EIRQ12 ((uint32_t)0x00001000)
#define PWC_STOPWKUPEN_EIRQ13 ((uint32_t)0x00002000)
#define PWC_STOPWKUPEN_EIRQ14 ((uint32_t)0x00004000)
#define PWC_STOPWKUPEN_EIRQ15 ((uint32_t)0x00008000)
#define PWC_STOPWKUPEN_SWDT ((uint32_t)0x00010000)
#define PWC_STOPWKUPEN_VDU1 ((uint32_t)0x00020000)
#define PWC_STOPWKUPEN_VDU2 ((uint32_t)0x00040000)
#define PWC_STOPWKUPEN_CMPI0 ((uint32_t)0x00080000)
#define PWC_STOPWKUPEN_WKTM ((uint32_t)0x00100000)
#define PWC_STOPWKUPEN_RTCAL ((uint32_t)0x00200000)
#define PWC_STOPWKUPEN_RTCPRD ((uint32_t)0x00400000)
#define PWC_STOPWKUPEN_TMR0 ((uint32_t)0x00800000)
#define PWC_STOPWKUPEN_USARTRXD ((uint32_t)0x02000000)

#define PWC_PTWK0_WKUPFLAG ((uint8_t)0x01)
#define PWC_PTWK1_WKUPFLAG ((uint8_t)0x02)
#define PWC_PTWK2_WKUPFLAG ((uint8_t)0x04)
#define PWC_PTWK3_WKUPFLAG ((uint8_t)0x08)
#define PWC_PVD1_WKUPFLAG ((uint8_t)0x10)
#define PWC_PVD2_WKUPFLAG ((uint8_t)0x20)
#define PWC_NMI_WKUPFLAG ((uint8_t)0x40)

#define PWC_RXD0_WKUPFLAG ((uint8_t)0x04)
#define PWC_RTCPRD_WKUPFALG ((uint8_t)0x10)
#define PWC_RTCAL_WKUPFLAG ((uint8_t)0x20)
#define PWC_XTAL32ER_WKUPFALG ((uint8_t)0x30)
#define PWC_WKTM_WKUPFLAG ((uint8_t)0x40)

#define PWC_WKTMCMP_MSK ((uint16_t)0x0FFF)

#define PWC_FCG0_PERIPH_RAMHS ((uint32_t)0x00000001)
#define PWC_FCG0_PERIPH_RAM0 ((uint32_t)0x00000010)
#define PWC_FCG0_PERIPH_ECCRAM ((uint32_t)0x00000100)
#define PWC_FCG0_PERIPH_RetRAM ((uint32_t)0x00000400)
#define PWC_FCG0_PERIPH_DMA1 ((uint32_t)0x00004000)
#define PWC_FCG0_PERIPH_DMA2 ((uint32_t)0x00008000)
#define PWC_FCG0_PERIPH_FCM ((uint32_t)0x00010000)
#define PWC_FCG0_PERIPH_PTDIS ((uint32_t)0x00020000)
#define PWC_FCG0_PERIPH_AES ((uint32_t)0x00100000)
#define PWC_FCG0_PERIPH_HASH ((uint32_t)0x00200000)
#define PWC_FCG0_PERIPH_TRNG ((uint32_t)0x00400000)
#define PWC_FCG0_PERIPH_CRC ((uint32_t)0x00800000)
#define PWC_FCG0_PERIPH_DCU1 ((uint32_t)0x01000000)
#define PWC_FCG0_PERIPH_DCU2 ((uint32_t)0x02000000)
#define PWC_FCG0_PERIPH_DCU3 ((uint32_t)0x04000000)
#define PWC_FCG0_PERIPH_DCU4 ((uint32_t)0x08000000)
#define PWC_FCG0_PERIPH_KEY ((uint32_t)0x80000000)

#define PWC_FCG1_PERIPH_CAN ((uint32_t)0x00000001)
#define PWC_FCG1_PERIPH_QSPI ((uint32_t)0x00000008)
#define PWC_FCG1_PERIPH_I2C1 ((uint32_t)0x00000010)
#define PWC_FCG1_PERIPH_I2C2 ((uint32_t)0x00000020)
#define PWC_FCG1_PERIPH_I2C3 ((uint32_t)0x00000040)
#define PWC_FCG1_PERIPH_USBFS ((uint32_t)0x00000100)
#define PWC_FCG1_PERIPH_SDIOC1 ((uint32_t)0x00000400)
#define PWC_FCG1_PERIPH_SDIOC2 ((uint32_t)0x00000800)
#define PWC_FCG1_PERIPH_I2S1 ((uint32_t)0x00001000)
#define PWC_FCG1_PERIPH_I2S2 ((uint32_t)0x00002000)
#define PWC_FCG1_PERIPH_I2S3 ((uint32_t)0x00004000)
#define PWC_FCG1_PERIPH_I2S4 ((uint32_t)0x00008000)
#define PWC_FCG1_PERIPH_SPI1 ((uint32_t)0x00010000)
#define PWC_FCG1_PERIPH_SPI2 ((uint32_t)0x00020000)
#define PWC_FCG1_PERIPH_SPI3 ((uint32_t)0x00040000)
#define PWC_FCG1_PERIPH_SPI4 ((uint32_t)0x00080000)
#define PWC_FCG1_PERIPH_USART1 ((uint32_t)0x01000000)
#define PWC_FCG1_PERIPH_USART2 ((uint32_t)0x02000000)
#define PWC_FCG1_PERIPH_USART3 ((uint32_t)0x04000000)
#define PWC_FCG1_PERIPH_USART4 ((uint32_t)0x08000000)

#define PWC_FCG2_PERIPH_TIM01 ((uint32_t)0x00000001)
#define PWC_FCG2_PERIPH_TIM02 ((uint32_t)0x00000002)
#define PWC_FCG2_PERIPH_TIMA1 ((uint32_t)0x00000004)
#define PWC_FCG2_PERIPH_TIMA2 ((uint32_t)0x00000008)
#define PWC_FCG2_PERIPH_TIMA3 ((uint32_t)0x00000010)
#define PWC_FCG2_PERIPH_TIMA4 ((uint32_t)0x00000020)
#define PWC_FCG2_PERIPH_TIMA5 ((uint32_t)0x00000040)
#define PWC_FCG2_PERIPH_TIMA6 ((uint32_t)0x00000080)
#define PWC_FCG2_PERIPH_TIM41 ((uint32_t)0x00000100)
#define PWC_FCG2_PERIPH_TIM42 ((uint32_t)0x00000200)
#define PWC_FCG2_PERIPH_TIM43 ((uint32_t)0x00000400)
#define PWC_FCG2_PERIPH_EMB ((uint32_t)0x00008000)
#define PWC_FCG2_PERIPH_TIM61 ((uint32_t)0x00010000)
#define PWC_FCG2_PERIPH_TIM62 ((uint32_t)0x00020000)
#define PWC_FCG2_PERIPH_TIM63 ((uint32_t)0x00040000)

#define PWC_FCG3_PERIPH_ADC1 ((uint32_t)0x00000001)
#define PWC_FCG3_PERIPH_ADC2 ((uint32_t)0x00000002)
#define PWC_FCG3_PERIPH_DAC ((uint32_t)0x00000010)
#define PWC_FCG3_PERIPH_CMP ((uint32_t)0x00000100)
#define PWC_FCG3_PERIPH_OTS ((uint32_t)0x00001000)

    /*******************************************************************************
     * Global variable definitions ('extern')
     ******************************************************************************/

    /*******************************************************************************
     * Global function prototypes (definition in C source)
     ******************************************************************************/
    void PWC_PowerModeCfg(const stc_pwc_pwr_mode_cfg_t *pstcPwrMdCfg);
    void PWC_EnterPowerDownMd(void);

    void PWC_PdWakeup0Cmd(uint32_t u32Wkup0Event, en_functional_state_t enNewState);
    void PWC_PdWakeup1Cmd(uint32_t u32Wkup1Event, en_functional_state_t enNewState);
    void PWC_PdWakeup2Cmd(uint32_t u32Wkup2Event, en_functional_state_t enNewState);
    void PWC_PdWkupEdgeCfg(const stc_pwc_wkup_edge_cfg_t *pstcWkupEdgeCfg);

    en_flag_status_t PWC_GetWakeup0Flag(uint8_t u8WkupFlag);
    en_flag_status_t PWC_GetWakeup1Flag(uint8_t u8WkupFlag);
    void PWC_ClearWakeup0Flag(uint8_t u8WkupFlag);
    void PWC_ClearWakeup1Flag(uint8_t u8WkupFlag);
    void PWC_PwrMonitorCmd(en_functional_state_t enNewState);

    void PWC_Fcg0PeriphClockCmd(uint32_t u32Fcg0Periph, en_functional_state_t enNewState);
    void PWC_Fcg1PeriphClockCmd(uint32_t u32Fcg1Periph, en_functional_state_t enNewState);
    void PWC_Fcg2PeriphClockCmd(uint32_t u32Fcg2Periph, en_functional_state_t enNewState);
    void PWC_Fcg3PeriphClockCmd(uint32_t u32Fcg3Periph, en_functional_state_t enNewState);

    en_result_t PWC_StopModeCfg(const stc_pwc_stop_mode_cfg_t *pstcStpMdCfg);
    void PWC_StopWkupCmd(uint32_t u32Wkup0Event, en_functional_state_t enNewState);

    void PWC_EnterStopMd(void);
    void PWC_EnterSleepMd(void);

    void PWC_RamCfg(const stc_pwc_ram_cfg_t *pstcRamCfg);
    void PWC_Xtal32CsCmd(en_functional_state_t enNewState);
    void PWC_WktmControl(const stc_pwc_wktm_ctl_t *pstcWktmCtl);

    void PWC_PvdCfg(const stc_pwc_pvd_cfg_t *pstcPvdCfg);
    void PWC_Pvd1Cmd(en_functional_state_t enNewState);
    void PWC_Pvd2Cmd(en_functional_state_t enNewState);
    void PWC_ExVccCmd(en_functional_state_t enNewState);
    en_flag_status_t PWC_GetPvdFlag(en_pwc_pvd_flag_t enPvdFlag);

    void PWC_HrcPwrCmd(en_functional_state_t enNewState);
    void PWC_PllPwrCmd(en_functional_state_t enNewState);

    void PWC_IrqClkBackup(void);
    void PWC_IrqClkRecover(void);
    void set_vcap(void);

    //@} // PwcGroup

#ifdef __cplusplus
}
#endif

#endif /* DDL_PWC_ENABLE */

#endif /* __HC32F46X_PWC_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
