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
/** \file hc32f46x_mpu.h
 **
 ** A detailed description is available at
 ** @link MpuGroup MPU description @endlink
 **
 **   - 2018-10-20  1.0 Hongjh First version for Device Driver Library of MPU.
 **
 ******************************************************************************/
#ifndef __HC32F46x_MPU_H__
#define __HC32F46x_MPU_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_common.h"
#include "ddl_config.h"

#if (DDL_MPU_ENABLE == DDL_ON)

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

    /**
     *******************************************************************************
     ** \defgroup MpuGroup Memory Protection Unit(MPU)
     **
     ******************************************************************************/
    //@{

    /*******************************************************************************
     * Global type definitions ('typedef')
     ******************************************************************************/
    /**
     *******************************************************************************
     ** \brief MPU region number enumeration
     **
     ******************************************************************************/
    typedef enum en_mpu_region_num
    {
        MpuRegionNum0 = 0u,   ///< MPU region number 0
        MpuRegionNum1 = 1u,   ///< MPU region number 1
        MpuRegionNum2 = 2u,   ///< MPU region number 2
        MpuRegionNum3 = 3u,   ///< MPU region number 3
        MpuRegionNum4 = 4u,   ///< MPU region number 4
        MpuRegionNum5 = 5u,   ///< MPU region number 5
        MpuRegionNum6 = 6u,   ///< MPU region number 6
        MpuRegionNum7 = 7u,   ///< MPU region number 7
        MpuRegionNum8 = 8u,   ///< MPU region number 8
        MpuRegionNum9 = 9u,   ///< MPU region number 9
        MpuRegionNum10 = 10u, ///< MPU region number 10
        MpuRegionNum11 = 11u, ///< MPU region number 11
        MpuRegionNum12 = 12u, ///< MPU region number 12
        MpuRegionNum13 = 13u, ///< MPU region number 13
        MpuRegionNum14 = 14u, ///< MPU region number 14
        MpuRegionNum15 = 15u, ///< MPU region number 15
    } en_mpu_region_num_t;

    /**
     *******************************************************************************
     ** \brief MPU region size enumeration
     **
     ******************************************************************************/
    typedef enum en_mpu_region_size
    {
        MpuRegionSize32Byte = 4u,    ///< 32 Byte
        MpuRegionSize64Byte = 5u,    ///< 64 Byte
        MpuRegionSize128Byte = 6u,   ///< 126 Byte
        MpuRegionSize256Byte = 7u,   ///< 256 Byte
        MpuRegionSize512Byte = 8u,   ///< 512 Byte
        MpuRegionSize1KByte = 9u,    ///< 1K Byte
        MpuRegionSize2KByte = 10u,   ///< 2K Byte
        MpuRegionSize4KByte = 11u,   ///< 4K Byte
        MpuRegionSize8KByte = 12u,   ///< 8K Byte
        MpuRegionSize16KByte = 13u,  ///< 16K Byte
        MpuRegionSize32KByte = 14u,  ///< 32K Byte
        MpuRegionSize64KByte = 15u,  ///< 64K Byte
        MpuRegionSize128KByte = 16u, ///< 128K Byte
        MpuRegionSize256KByte = 17u, ///< 256K Byte
        MpuRegionSize512KByte = 18u, ///< 512K Byte
        MpuRegionSize1MByte = 19u,   ///< 1M Byte
        MpuRegionSize2MByte = 20u,   ///< 2M Byte
        MpuRegionSize4MByte = 21u,   ///< 4M Byte
        MpuRegionSize8MByte = 22u,   ///< 8M Byte
        MpuRegionSize16MByte = 23u,  ///< 16M Byte
        MpuRegionSize32MByte = 24u,  ///< 32M Byte
        MpuRegionSize64MByte = 25u,  ///< 64M Byte
        MpuRegionSize128MByte = 26u, ///< 128M Byte
        MpuRegionSize256MByte = 27u, ///< 256M Byte
        MpuRegionSize512MByte = 28u, ///< 512M Byte
        MpuRegionSize1GByte = 29u,   ///< 1G Byte
        MpuRegionSize2GByte = 30u,   ///< 2G Byte
        MpuRegionSize4GByte = 31u,   ///< 4G Byte
    } en_mpu_region_size_t;

    /**
     *******************************************************************************
     ** \brief MPU region enumeration
     **
     ******************************************************************************/
    typedef enum en_mpu_region_type
    {
        SMPU1Region = 0u, ///< System DMA_1 MPU
        SMPU2Region = 1u, ///< System DMA_2 MPU
        FMPURegion = 2u,  ///< System USBFS_DMA MPU
    } en_mpu_region_type_t;

    /**
     *******************************************************************************
     ** \brief MPU action selection enumeration
     **
     ******************************************************************************/
    typedef enum en_mpu_action_sel
    {
        MpuNoneAction = 0u,   ///< MPU don't action.
        MpuTrigBusError = 1u, ///< MPU trigger bus error
        MpuTrigNmi = 2u,      ///< MPU trigger bus NMI interrupt
        MpuTrigReset = 3u,    ///< MPU trigger reset
    } en_mpu_action_sel_t;

    /**
     *******************************************************************************
     ** \brief MPU IP protection mode enumeration
     **
     ******************************************************************************/
    typedef enum en_mpu_ip_prot_mode
    {
        AesReadProt = (1ul << 0),      ///< AES read protection
        AesWriteProt = (1ul << 1),     ///< AES write protection
        HashReadProt = (1ul << 2),     ///< HASH read protection
        HashWriteProt = (1ul << 3),    ///< HASH write protection
        TrngReadProt = (1ul << 4),     ///< TRNG read protection
        TrngWriteProt = (1ul << 5),    ///< TRNG write protection
        CrcReadProt = (1ul << 6),      ///< CRC read protection
        CrcWriteProt = (1ul << 7),     ///< CRC write protection
        FmcReadProt = (1ul << 8),      ///< FMC read protection
        FmcWriteProt = (1ul << 9),     ///< FMC write protection
        WdtReadProt = (1ul << 12),     ///< WDT read protection
        WdtWriteProt = (1ul << 13),    ///< WDT write protection
        SwdtReadProt = (1ul << 14),    ///< WDT read protection
        SwdtWriteProt = (1ul << 15),   ///< WDT write protection
        BksramReadProt = (1ul << 16),  ///< BKSRAM read protection
        BksramWriteProt = (1ul << 17), ///< BKSRAM write protection
        RtcReadProt = (1ul << 18),     ///< RTC read protection
        RtcWriteProt = (1ul << 19),    ///< RTC write protection
        DmpuReadProt = (1ul << 20),    ///< DMPU read protection
        DmpuWriteProt = (1ul << 21),   ///< DMPU write protection
        SramcReadProt = (1ul << 22),   ///< SRAMC read protection
        SramcWriteProt = (1ul << 23),  ///< SRAMC write protection
        IntcReadProt = (1ul << 24),    ///< INTC read protection
        IntcWriteProt = (1ul << 25),   ///< INTC write protection
        SyscReadProt = (1ul << 26),    ///< SYSC read protection
        SyscWriteProt = (1ul << 27),   ///< SYSC write protection
        MstpReadProt = (1ul << 28),    ///< MSTP read protection
        MstpWriteProt = (1ul << 29),   ///< MSTP write protection
        BusErrProt = (1ul << 31),      ///< BUSERR write protection
    } en_mpu_ip_prot_mode_t;

    /**
     *******************************************************************************
     ** \brief MPU protection region permission
     **
     ******************************************************************************/
    typedef struct stc_mpu_prot_region_permission
    {
        en_mpu_action_sel_t enAction; ///< Specifies MPU action

        en_functional_state_t enRegionEnable; ///< Disable: Disable region protection; Enable:Enable region protection

        en_functional_state_t enWriteEnable; ///< Disable: Prohibited to write; Enable:permitted to write

        en_functional_state_t enReadEnable; ///< Disable: Prohibited to read; Enable:permitted to read

    } stc_mpu_prot_region_permission_t;

    /**
     *******************************************************************************
     ** \brief MPU background region permission
     **
     ******************************************************************************/
    typedef struct stc_mpu_bkgd_region_permission
    {
        en_functional_state_t enWriteEnable; ///< Disable: Prohibited to write; Enable:permitted to write

        en_functional_state_t enReadEnable; ///< Disable: Prohibited to read; Enable:permitted to read
    } stc_mpu_bkgd_region_permission_t_t;

    /**
     *******************************************************************************
     ** \brief MPU background region initialization configuration
     **
     ******************************************************************************/
    typedef struct stc_mpu_bkgd_region_init
    {
        stc_mpu_bkgd_region_permission_t_t stcSMPU1BkgdPermission; ///< Specifies SMPU1 background permission and this stuctrue detail refer of @ref stc_mpu_bkgd_region_permission_t_t

        stc_mpu_bkgd_region_permission_t_t stcSMPU2BkgdPermission; ///< Specifies SMPU2 background permission and this stuctrue detail refer @ref stc_mpu_bkgd_region_permission_t_t

        stc_mpu_bkgd_region_permission_t_t stcFMPUBkgdPermission; ///< Specifies FMPU background permission and this stuctrue detail refer @ref stc_mpu_bkgd_region_permission_t_t
    } stc_mpu_bkgd_region_init_t;

    /**
     *******************************************************************************
     ** \brief MPU protect region initialization configuration
     **
     ******************************************************************************/
    typedef struct stc_mpu_prot_region_init
    {
        uint32_t u32RegionBaseAddress; ///< Specifies region base address

        en_mpu_region_size_t enRegionSize; ///< Specifies region size and This parameter can be a value of @ref en_mpu_region_size_t

        stc_mpu_prot_region_permission_t stcSMPU1Permission; ///< Specifies DMA1 MPU region permission and this structure detail refer @ref stc_mpu_prot_region_permission_t

        stc_mpu_prot_region_permission_t stcSMPU2Permission; ///< Specifies DMA2 MPU region permission and this structure detail refer @ref stc_mpu_prot_region_permission_t

        stc_mpu_prot_region_permission_t stcFMPUPermission; ///< Specifies USBFS-DMA MPU region permission and this structure detail refer @ref stc_mpu_prot_region_permission_t
    } stc_mpu_prot_region_init_t;

    /*******************************************************************************
     * Global pre-processor symbols/macros ('#define')
     ******************************************************************************/

    /*******************************************************************************
     * Global variable definitions ('extern')
     ******************************************************************************/

    /*******************************************************************************
     * Global function prototypes (definition in C source)
     ******************************************************************************/
    en_result_t MPU_ProtRegionInit(en_mpu_region_num_t enRegionNum,
                                   const stc_mpu_prot_region_init_t *pstcInitCfg);
    en_result_t MPU_BkgdRegionInit(const stc_mpu_bkgd_region_init_t *pstcInitCfg);
    en_result_t MPU_SetRegionSize(en_mpu_region_num_t enRegionNum,
                                  en_mpu_region_size_t enRegionSize);
    en_mpu_region_size_t MPU_GetRegionSize(en_mpu_region_num_t enRegionNum);
    en_result_t MPU_SetRegionBaseAddress(en_mpu_region_num_t enRegionNum,
                                         uint32_t u32RegionBaseAddr);
    uint32_t MPU_GetRegionBaseAddress(en_mpu_region_num_t enRegionNum);
    en_result_t MPU_SetNoPermissionAcessAction(en_mpu_region_type_t enMpuRegionType,
                                               en_mpu_action_sel_t enActionSel);
    en_mpu_action_sel_t MPU_GetNoPermissionAcessAction(en_mpu_region_type_t enMpuRegionType);
    en_result_t MPU_ProtRegionCmd(en_mpu_region_num_t enRegionNum,
                                  en_mpu_region_type_t enMpuRegionType,
                                  en_functional_state_t enState);
    en_result_t MPU_RegionTypeCmd(en_mpu_region_type_t enMpuRegionType,
                                  en_functional_state_t enState);
    en_flag_status_t MPU_GetStatus(en_mpu_region_type_t enMpuRegionType);
    en_result_t MPU_ClearStatus(en_mpu_region_type_t enMpuRegionType);
    en_result_t MPU_SetProtRegionReadPermission(en_mpu_region_num_t enRegionNum,
                                                en_mpu_region_type_t enMpuRegionType,
                                                en_functional_state_t enState);
    en_functional_state_t MPU_GetProtRegionReadPermission(en_mpu_region_num_t enRegionNum,
                                                          en_mpu_region_type_t enMpuRegionType);
    en_result_t MPU_SetProtRegionWritePermission(en_mpu_region_num_t enRegionNum,
                                                 en_mpu_region_type_t enMpuRegionType,
                                                 en_functional_state_t enState);
    en_functional_state_t MPU_GetProtRegionWritePermission(en_mpu_region_num_t enRegionNum,
                                                           en_mpu_region_type_t enMpuRegionType);
    en_result_t MPU_SetBkgdRegionReadPermission(en_mpu_region_type_t enMpuRegionType,
                                                en_functional_state_t enState);
    en_functional_state_t MPU_GetBkgdRegionReadPermission(en_mpu_region_type_t enMpuRegionType);
    en_result_t MPU_SetBkgdRegionWritePermission(en_mpu_region_type_t enMpuRegionType,
                                                 en_functional_state_t enState);
    en_functional_state_t MPU_GetBkgdRegionWritePermission(en_mpu_region_type_t enMpuRegionType);
    en_result_t MPU_WriteProtCmd(en_functional_state_t enState);
    en_result_t MPU_IpProtCmd(uint32_t u32ProtMode,
                              en_functional_state_t enState);

    //@} // MpuGroup

#ifdef __cplusplus
}
#endif

#endif /* DDL_MPU_ENABLE */

#endif /* __HC32F46x_MPU_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
