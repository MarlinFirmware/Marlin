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
/** \file hc32f46x_mpu.c
 **
 ** A detailed description is available at
 ** @link MpuGroup MPU description @endlink
 **
 **   - 2018-10-20  1.0 Hongjh First version for Device Driver Library of MPU.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32f46x_mpu.h"
#include "hc32f46x_utility.h"

#if (DDL_MPU_ENABLE == DDL_ON)

/**
 *******************************************************************************
 ** \addtogroup MpuGroup
 ******************************************************************************/

//@{

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*!< Parameter valid check for MPU region number. */
#define IS_VALID_MPU_REGION_NUM(x) \
    ((MpuRegionNum0 == (x)) ||     \
     (MpuRegionNum1 == (x)) ||     \
     (MpuRegionNum2 == (x)) ||     \
     (MpuRegionNum3 == (x)) ||     \
     (MpuRegionNum4 == (x)) ||     \
     (MpuRegionNum5 == (x)) ||     \
     (MpuRegionNum6 == (x)) ||     \
     (MpuRegionNum7 == (x)) ||     \
     (MpuRegionNum8 == (x)) ||     \
     (MpuRegionNum9 == (x)) ||     \
     (MpuRegionNum10 == (x)) ||    \
     (MpuRegionNum11 == (x)) ||    \
     (MpuRegionNum12 == (x)) ||    \
     (MpuRegionNum13 == (x)) ||    \
     (MpuRegionNum14 == (x)) ||    \
     (MpuRegionNum15 == (x)))

/*!< Parameter valid check for MPU region size. */
#define IS_VALID_MPU_REGION_SIZE(x)    \
    ((MpuRegionSize32Byte == (x)) ||   \
     (MpuRegionSize64Byte == (x)) ||   \
     (MpuRegionSize128Byte == (x)) ||  \
     (MpuRegionSize256Byte == (x)) ||  \
     (MpuRegionSize512Byte == (x)) ||  \
     (MpuRegionSize1KByte == (x)) ||   \
     (MpuRegionSize2KByte == (x)) ||   \
     (MpuRegionSize4KByte == (x)) ||   \
     (MpuRegionSize8KByte == (x)) ||   \
     (MpuRegionSize16KByte == (x)) ||  \
     (MpuRegionSize32KByte == (x)) ||  \
     (MpuRegionSize64KByte == (x)) ||  \
     (MpuRegionSize128KByte == (x)) || \
     (MpuRegionSize256KByte == (x)) || \
     (MpuRegionSize512KByte == (x)) || \
     (MpuRegionSize1MByte == (x)) ||   \
     (MpuRegionSize2MByte == (x)) ||   \
     (MpuRegionSize4MByte == (x)) ||   \
     (MpuRegionSize8MByte == (x)) ||   \
     (MpuRegionSize16MByte == (x)) ||  \
     (MpuRegionSize32MByte == (x)) ||  \
     (MpuRegionSize64MByte == (x)) ||  \
     (MpuRegionSize128MByte == (x)) || \
     (MpuRegionSize256MByte == (x)) || \
     (MpuRegionSize512MByte == (x)) || \
     (MpuRegionSize1GByte == (x)) ||   \
     (MpuRegionSize2GByte == (x)) ||   \
     (MpuRegionSize4GByte == (x)))

/*!< Parameter valid check for MPU region type. */
#define IS_VALID_MPU_REGION_TYPE(x) \
    ((SMPU1Region == (x)) ||        \
     (SMPU2Region == (x)) ||        \
     (FMPURegion == (x)))

/*!< Parameter valid check for MPU action. */
#define IS_VALID_MPU_ACTION(x) \
    ((MpuTrigNmi == (x)) ||    \
     (MpuTrigReset == (x)) ||  \
     (MpuNoneAction == (x)) || \
     (MpuTrigBusError == (x)))

/******************************************************************************/
/*                                     MPU                                    */
/******************************************************************************/
/*!< Get the RGD register address of the specified MPU region */
#define MPU_RGDx(__REGION_NUM__) ((uint32_t)(&M4_MPU->RGD0) + ((uint32_t)(__REGION_NUM__)) * 4u)

/*!< Get the RGCR register address of the specified MPU region */
#define MPU_RGCRx(__REGION_NUM__) ((uint32_t)(&M4_MPU->RGCR0) + ((uint32_t)(__REGION_NUM__)) * 4u)

/*!< MPU RGD register: RGADDR position */
#define MPU_RGD_RGADDR_Pos (5u) /*!< MPU_RGD: RGADDR Position */

/*!< MPU write protection key */
#define MPU_WRITE_PROT_KEY (0x96A4ul)

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
 ** \brief Configure MPU protect region.
 **
 ** \param [in] enRegionNum             MPU region number
 ** \arg This parameter can be a value of @ref en_mpu_region_num_t
 ** \param [in] pstcInitCfg             Pointer to MPU protection region configuration structure
 ** \arg the structure detail refer @ref stc_mpu_prot_region_init_t
 **
 ** \retval Ok                          Set successfully
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - pstcInitCfg == NULL
 **                                     - pstcInitCfg->u32RegionBaseAddress is invalid
 **                                     - Other invalid configuration
 **
 ******************************************************************************/
en_result_t MPU_ProtRegionInit(en_mpu_region_num_t enRegionNum,
                               const stc_mpu_prot_region_init_t *pstcInitCfg)
{
    en_result_t enRet = ErrorInvalidParameter;
    uint32_t u32WriteProt = M4_MPU->WP;
    stc_mpu_rgd0_field_t *RGD_f = NULL;
    stc_mpu_rgcr0_field_t *RGCR_f = NULL;

    /* Check pointer parameters */
    if (NULL != pstcInitCfg)
    {
        DDL_ASSERT(IS_VALID_MPU_REGION_NUM(enRegionNum));
        DDL_ASSERT(IS_VALID_MPU_REGION_SIZE(pstcInitCfg->enRegionSize));
        DDL_ASSERT(IS_VALID_MPU_ACTION(pstcInitCfg->stcSMPU1Permission.enAction));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInitCfg->stcSMPU1Permission.enRegionEnable));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInitCfg->stcSMPU1Permission.enWriteEnable));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInitCfg->stcSMPU1Permission.enReadEnable));
        DDL_ASSERT(IS_VALID_MPU_ACTION(pstcInitCfg->stcSMPU2Permission.enAction));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInitCfg->stcSMPU2Permission.enRegionEnable));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInitCfg->stcSMPU2Permission.enWriteEnable));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInitCfg->stcSMPU2Permission.enReadEnable));
        DDL_ASSERT(IS_VALID_MPU_ACTION(pstcInitCfg->stcFMPUPermission.enAction));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInitCfg->stcFMPUPermission.enRegionEnable));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInitCfg->stcFMPUPermission.enWriteEnable));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInitCfg->stcFMPUPermission.enReadEnable));

        /* Check base address and region size */
        if (!(pstcInitCfg->u32RegionBaseAddress & (~(0xFFFFFFFFUL << ((uint32_t)pstcInitCfg->enRegionSize + 1UL)))))
        {
            /* Disable write protection of MPU register */
            M4_MPU->WP = (MPU_WRITE_PROT_KEY | 1ul);

            /*  Get RGD && RGCR register address */
            RGD_f = (stc_mpu_rgd0_field_t *)MPU_RGDx(enRegionNum);
            RGCR_f = (stc_mpu_rgcr0_field_t *)MPU_RGCRx(enRegionNum);

            /* Disable region protection function */
            RGCR_f->FRG0E = (uint32_t)0ul;
            RGCR_f->S1RG0E = (uint32_t)0ul;
            RGCR_f->S2RG0E = (uint32_t)0ul;

            /* Set region size */
            RGD_f->MPURG0SIZE = (uint32_t)(pstcInitCfg->enRegionSize);

            /* Set region base address */
            RGD_f->MPURG0ADDR = (pstcInitCfg->u32RegionBaseAddress >> MPU_RGD_RGADDR_Pos);

            /* Set region FMPU */
            RGCR_f->FRG0RP = (pstcInitCfg->stcFMPUPermission.enReadEnable) ? 0ul : 1ul;
            RGCR_f->FRG0WP = (pstcInitCfg->stcFMPUPermission.enWriteEnable) ? 0ul : 1ul;
            RGCR_f->FRG0E = (uint32_t)(pstcInitCfg->stcFMPUPermission.enRegionEnable);
            M4_MPU->CR_f.FMPUACT = (uint32_t)(pstcInitCfg->stcFMPUPermission.enAction);

            /* Set region SMPU1 */
            RGCR_f->S1RG0RP = (pstcInitCfg->stcSMPU1Permission.enReadEnable) ? 0ul : 1ul;
            RGCR_f->S1RG0WP = (pstcInitCfg->stcSMPU1Permission.enWriteEnable) ? 0ul : 1ul;
            RGCR_f->S1RG0E = (uint32_t)(pstcInitCfg->stcSMPU1Permission.enRegionEnable);
            M4_MPU->CR_f.SMPU1ACT = (uint32_t)(pstcInitCfg->stcSMPU1Permission.enAction);

            /* Set region SMPU2 */
            RGCR_f->S2RG0RP = (pstcInitCfg->stcSMPU2Permission.enReadEnable) ? 0ul : 1ul;
            RGCR_f->S2RG0WP = (pstcInitCfg->stcSMPU2Permission.enWriteEnable) ? 0ul : 1ul;
            RGCR_f->S2RG0E = (uint32_t)(pstcInitCfg->stcSMPU2Permission.enRegionEnable);
            M4_MPU->CR_f.SMPU2ACT = (uint32_t)(pstcInitCfg->stcSMPU2Permission.enAction);

            /* Recover write protection of MPU register */
            M4_MPU->WP = (MPU_WRITE_PROT_KEY | u32WriteProt);
            enRet = Ok;
        }
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Configure MPU background region.
 **
 ** \param [in] pstcInitCfg             Pointer to MPU background region configuration structure
 ** \arg the structure detail refer @ref stc_mpu_bkgd_region_init_t
 **
 ** \retval Ok                          Set successfully
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - pstcInitCfg == NULL
 **                                     - pstcInitCfg->u32RegionBaseAddress is invalid
 **                                     - Other invalid configuration
 **
 ******************************************************************************/
en_result_t MPU_BkgdRegionInit(const stc_mpu_bkgd_region_init_t *pstcInitCfg)
{
    uint32_t u32WriteProt = M4_MPU->WP;
    en_result_t enRet = ErrorInvalidParameter;

    /* Check pointer parameters */
    if (NULL != pstcInitCfg)
    {
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInitCfg->stcSMPU1BkgdPermission.enWriteEnable));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInitCfg->stcSMPU1BkgdPermission.enReadEnable));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInitCfg->stcSMPU2BkgdPermission.enWriteEnable));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInitCfg->stcSMPU2BkgdPermission.enReadEnable));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInitCfg->stcFMPUBkgdPermission.enWriteEnable));
        DDL_ASSERT(IS_FUNCTIONAL_STATE(pstcInitCfg->stcFMPUBkgdPermission.enReadEnable));

        /* Disable write protection of MPU register */
        M4_MPU->WP = (MPU_WRITE_PROT_KEY | 1ul);

        /* Set SMPU1 */
        M4_MPU->CR_f.SMPU1BWP = (pstcInitCfg->stcSMPU1BkgdPermission.enWriteEnable) ? 0ul : 1ul;
        M4_MPU->CR_f.SMPU1BRP = (pstcInitCfg->stcSMPU1BkgdPermission.enReadEnable) ? 0ul : 1ul;

        /* Set SMPU2 */
        M4_MPU->CR_f.SMPU2BWP = (pstcInitCfg->stcSMPU2BkgdPermission.enWriteEnable) ? 0ul : 1ul;
        M4_MPU->CR_f.SMPU2BRP = (pstcInitCfg->stcSMPU2BkgdPermission.enReadEnable) ? 0ul : 1ul;

        /* Set FMPU */
        M4_MPU->CR_f.FMPUBWP = (pstcInitCfg->stcFMPUBkgdPermission.enWriteEnable) ? 0ul : 1ul;
        M4_MPU->CR_f.FMPUBRP = (pstcInitCfg->stcFMPUBkgdPermission.enReadEnable) ? 0ul : 1ul;

        /* Recover write protection of MPU register */
        M4_MPU->WP = (MPU_WRITE_PROT_KEY | u32WriteProt);
        enRet = Ok;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Set MPU size of the specified region.
 **
 ** \param [in] enRegionNum             MPU region number
 ** \arg This parameter can be a value of @ref en_mpu_region_num_t
 ** \param [in] enRegionSize            MPU region size
 ** \arg This parameter can be a value of @ref en_mpu_region_size_t
 **
 ** \retval Ok                          Set successfully.
 **
 ******************************************************************************/
en_result_t MPU_SetRegionSize(en_mpu_region_num_t enRegionNum,
                              en_mpu_region_size_t enRegionSize)
{
    stc_mpu_rgd0_field_t *RGD_f = NULL;

    DDL_ASSERT(IS_VALID_MPU_REGION_NUM(enRegionNum));
    DDL_ASSERT(IS_VALID_MPU_REGION_SIZE(enRegionSize));

    RGD_f = (stc_mpu_rgd0_field_t *)MPU_RGDx(enRegionNum);
    RGD_f->MPURG0SIZE = (uint32_t)enRegionSize;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief Get MPU size of the specified region.
 **
 ** \param [in] enRegionNum             MPU region number
 ** \arg This parameter can be a value of @ref en_mpu_region_num_t
 **
 ** \retval MPU size of the specified region.
 **
 ******************************************************************************/
en_mpu_region_size_t MPU_GetRegionSize(en_mpu_region_num_t enRegionNum)
{
    stc_mpu_rgd0_field_t *RGD_f = NULL;

    DDL_ASSERT(IS_VALID_MPU_REGION_NUM(enRegionNum));

    RGD_f = (stc_mpu_rgd0_field_t *)MPU_RGDx(enRegionNum);

    return (en_mpu_region_size_t)(RGD_f->MPURG0SIZE);
}

/**
 *******************************************************************************
 ** \brief Set MPU base address of the specified region.
 **
 ** \param [in] enRegionNum             MPU region number
 ** \arg This parameter can be a value of @ref en_mpu_region_num_t
 ** \param [in] u32RegionBaseAddr       the specified base address
 **
 ** \retval Ok                          Set successfully.
 **
 ******************************************************************************/
en_result_t MPU_SetRegionBaseAddress(en_mpu_region_num_t enRegionNum,
                                     uint32_t u32RegionBaseAddr)
{
    stc_mpu_rgd0_field_t *RGD_f = NULL;

    DDL_ASSERT(IS_VALID_MPU_REGION_NUM(enRegionNum));

    RGD_f = (stc_mpu_rgd0_field_t *)MPU_RGDx(enRegionNum);
    RGD_f->MPURG0ADDR = (u32RegionBaseAddr >> MPU_RGD_RGADDR_Pos);

    return Ok;
}

/**
 *******************************************************************************
 ** \brief Get MPU base address of the specified region.
 **
 ** \param [in] enRegionNum             MPU region number
 ** \arg This parameter can be a value of @ref en_mpu_region_num_t
s **
 ** \retval MPU base address of the specified region.
 **
 ******************************************************************************/
uint32_t MPU_GetRegionBaseAddress(en_mpu_region_num_t enRegionNum)
{
    stc_mpu_rgd0_field_t *RGD_f = NULL;

    DDL_ASSERT(IS_VALID_MPU_REGION_NUM(enRegionNum));

    RGD_f = (stc_mpu_rgd0_field_t *)MPU_RGDx(enRegionNum);

    return (RGD_f->MPURG0ADDR << MPU_RGD_RGADDR_Pos);
}

/**
 *******************************************************************************
 ** \brief Set the action of the specified MPU region type.
 **
 ** \param [in] enMpuRegionType         the specified region type
 ** \arg SMPU1Region                    System DMA_1 MPU
 ** \arg SMPU2Region                    System DMA_2 MPU
 ** \arg FMPURegion                     System USBFS_DMA MPU
 ** \param [in] enActionSel             MPU action
 ** \arg MpuNoneAction                  MPU don't action.
 ** \arg MpuTrigBusError                MPU trigger bus error
 ** \arg MpuTrigNmi                     MPU trigger bus NMI interrupt
 ** \arg MpuTrigReset                   MPU trigger reset
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - enActionSel is invalid
 **
 ******************************************************************************/
en_result_t MPU_SetNoPermissionAcessAction(en_mpu_region_type_t enMpuRegionType,
                                           en_mpu_action_sel_t enActionSel)
{
    en_result_t enRet = Ok;

    DDL_ASSERT(IS_VALID_MPU_ACTION(enActionSel));
    DDL_ASSERT(IS_VALID_MPU_REGION_TYPE(enMpuRegionType));

    switch (enMpuRegionType)
    {
    case SMPU1Region:
        M4_MPU->CR_f.SMPU1ACT = (uint32_t)enActionSel;
        break;
    case SMPU2Region:
        M4_MPU->CR_f.SMPU2ACT = (uint32_t)enActionSel;
        break;
    case FMPURegion:
        M4_MPU->CR_f.FMPUACT = (uint32_t)enActionSel;
        break;
    default:
        enRet = ErrorInvalidParameter;
        break;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get the action of the specified MPU region type.
 **
 ** \param [in] enMpuRegionType         the specified region type
 ** \arg SMPU1Region                    System DMA_1 MPU
 ** \arg SMPU2Region                    System DMA_2 MPU
 ** \arg FMPURegion                     System USBFS_DMA MPU
 **
 ** \retval MpuNoneAction               MPU don't action.
 ** \retval MpuTrigBusError             MPU trigger bus error
 ** \retval MpuTrigNmi                  MPU trigger bus NMI interrupt
 ** \retval MpuTrigReset                MPU trigger reset
 **
 ******************************************************************************/
en_mpu_action_sel_t MPU_GetNoPermissionAcessAction(en_mpu_region_type_t enMpuRegionType)
{
    uint32_t u32ActionSel = 0u;

    DDL_ASSERT(IS_VALID_MPU_REGION_TYPE(enMpuRegionType));

    switch (enMpuRegionType)
    {
    case SMPU1Region:
        u32ActionSel = M4_MPU->CR_f.SMPU1ACT;
        break;
    case SMPU2Region:
        u32ActionSel = M4_MPU->CR_f.SMPU2ACT;
        break;
    case FMPURegion:
        u32ActionSel = M4_MPU->CR_f.FMPUACT;
        break;
    default:
        break;
    }

    return (en_mpu_action_sel_t)(u32ActionSel);
}

/**
 *******************************************************************************
 ** \brief Set MPU function of the specified region and type.
 **
 ** \param [in] enRegionNum             MPU region number
 ** \arg This parameter can be a value of @ref en_mpu_region_num_t
 ** \param [in] enMpuRegionType         the specified region type
 ** \arg SMPU1Region                    System DMA_1 MPU
 ** \arg SMPU2Region                    System DMA_2 MPU
 ** \arg FMPURegion                     System USBFS_DMA MPU
 ** \param [in] enState                 MPU region state
 ** \arg Enable                         Enable the specified MPU region function
 ** \arg Disable                        Disable the specified MPU region function
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - enMpuRegionType is invalid
 **
 ******************************************************************************/
en_result_t MPU_ProtRegionCmd(en_mpu_region_num_t enRegionNum,
                              en_mpu_region_type_t enMpuRegionType,
                              en_functional_state_t enState)
{
    en_result_t enRet = Ok;
    stc_mpu_rgcr0_field_t *RGCR_f = NULL;

    DDL_ASSERT(IS_FUNCTIONAL_STATE(enState));
    DDL_ASSERT(IS_VALID_MPU_REGION_NUM(enRegionNum));
    DDL_ASSERT(IS_VALID_MPU_REGION_TYPE(enMpuRegionType));

    RGCR_f = (stc_mpu_rgcr0_field_t *)MPU_RGCRx(enRegionNum);

    switch (enMpuRegionType)
    {
    case SMPU1Region:
        RGCR_f->S1RG0E = (uint32_t)enState;
        break;
    case SMPU2Region:
        RGCR_f->S2RG0E = (uint32_t)enState;
        break;
    case FMPURegion:
        RGCR_f->FRG0E = (uint32_t)enState;
        break;
    default:
        enRet = ErrorInvalidParameter;
        break;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Set MPU function of the specified region type.
 **
 ** \param [in] enMpuRegionType         the specified region type
 ** \arg SMPU1Region                    System DMA_1 MPU
 ** \arg SMPU2Region                    System DMA_2 MPU
 ** \arg FMPURegion                     System USBFS_DMA MPU
 ** \param [in] enState                 MPU region state
 ** \arg Enable                         Enable the specified type region function of MPU
 ** \arg Disable                        Disable the specified type region function of MPU
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       If one of following cases matches:
 **                                     - enMpuRegionType is invalid
 **
 ******************************************************************************/
en_result_t MPU_RegionTypeCmd(en_mpu_region_type_t enMpuRegionType,
                              en_functional_state_t enState)
{
    en_result_t enRet = Ok;

    DDL_ASSERT(IS_FUNCTIONAL_STATE(enState));
    DDL_ASSERT(IS_VALID_MPU_REGION_TYPE(enMpuRegionType));

    switch (enMpuRegionType)
    {
    case SMPU1Region:
        M4_MPU->CR_f.SMPU1E = (uint32_t)enState;
        break;
    case SMPU2Region:
        M4_MPU->CR_f.SMPU2E = (uint32_t)enState;
        break;
    case FMPURegion:
        M4_MPU->CR_f.FMPUE = (uint32_t)enState;
        break;
    default:
        enRet = ErrorInvalidParameter;
        break;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get MPU status
 **
 ** \param [in] enMpuRegionType         the specified region type
 ** \arg SMPU1Region                    System DMA_1 MPU
 ** \arg SMPU2Region                    System DMA_2 MPU
 ** \arg FMPURegion                     System USBFS_DMA MPU
 **
 ** \retval Set                         Flag is set.
 ** \retval Reset                       Flag is reset or enMpuRegionType is invalid.
 **
 ******************************************************************************/
en_flag_status_t MPU_GetStatus(en_mpu_region_type_t enMpuRegionType)
{
    uint32_t u32Flag = 0ul;

    DDL_ASSERT(IS_VALID_MPU_REGION_TYPE(enMpuRegionType));

    switch (enMpuRegionType)
    {
    case SMPU1Region:
        u32Flag = M4_MPU->SR_f.SMPU1EAF;
        break;
    case SMPU2Region:
        u32Flag = M4_MPU->SR_f.SMPU2EAF;
        break;
    case FMPURegion:
        u32Flag = M4_MPU->SR_f.FMPUEAF;
        break;
    default:
        break;
    }

    return (en_flag_status_t)(u32Flag);
}

/**
 *******************************************************************************
 ** \brief Clear MPU status.
 **
 ** \param [in] enMpuRegionType         the specified region type
 ** \arg SMPU1Region                    System DMA_1 MPU
 ** \arg SMPU2Region                    System DMA_2 MPU
 ** \arg FMPURegion                     System USBFS_DMA MPU
 **
 ** \retval Ok                          Clear flag successfully.
 ** \retval ErrorInvalidParameter       enMpuRegionType is invalid
 **
 ******************************************************************************/
en_result_t MPU_ClearStatus(en_mpu_region_type_t enMpuRegionType)
{
    en_result_t enRet = Ok;

    DDL_ASSERT(IS_VALID_MPU_REGION_TYPE(enMpuRegionType));

    switch (enMpuRegionType)
    {
    case SMPU1Region:
        M4_MPU->ECLR_f.SMPU1ECLR = 1u;
        break;
    case SMPU2Region:
        M4_MPU->ECLR_f.SMPU2ECLR = 1u;
        break;
    case FMPURegion:
        M4_MPU->ECLR_f.FMPUECLR = 1u;
        break;
    default:
        enRet = ErrorInvalidParameter;
        break;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Set MPU read permission of the specified protection region and enMpuRegionType.
 **
 ** \param [in] enRegionNum             MPU region number
 ** \arg This parameter can be a value of @ref en_mpu_region_num_t
 ** \param [in] enMpuRegionType         MPU region type
 ** \arg SMPU1Region                    System DMA_1 MPU
 ** \arg SMPU2Region                    System DMA_2 MPU
 ** \arg FMPURegion                     System USBFS_DMA MPU
 ** \param [in] enState                 MPU region state
 ** \arg Enable                         Enable the specified MPU region read permission
 ** \arg Disable                        Disable the specified MPU region read permission
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       enMpuRegionType is invalid
 **
 ******************************************************************************/
en_result_t MPU_SetProtRegionReadPermission(en_mpu_region_num_t enRegionNum,
                                            en_mpu_region_type_t enMpuRegionType,
                                            en_functional_state_t enState)
{
    en_result_t enRet = Ok;
    stc_mpu_rgcr0_field_t *RGCR_f = NULL;

    DDL_ASSERT(IS_FUNCTIONAL_STATE(enState));
    DDL_ASSERT(IS_VALID_MPU_REGION_NUM(enRegionNum));
    DDL_ASSERT(IS_VALID_MPU_REGION_TYPE(enMpuRegionType));

    RGCR_f = (stc_mpu_rgcr0_field_t *)MPU_RGCRx(enRegionNum);

    switch (enMpuRegionType)
    {
    case SMPU1Region:
        RGCR_f->S1RG0RP = (Enable == enState) ? 0ul : 1ul;
        break;
    case SMPU2Region:
        RGCR_f->S2RG0RP = (Enable == enState) ? 0ul : 1ul;
        break;
    case FMPURegion:
        RGCR_f->FRG0RP = (Enable == enState) ? 0ul : 1ul;
        break;
    default:
        enRet = ErrorInvalidParameter;
        break;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get MPU read permission of the specified protection region and enMpuRegionType.
 **
 ** \param [in] enRegionNum             MPU region number
 ** \arg This parameter can be a value of @ref en_mpu_region_num_t
 ** \param [in] enMpuRegionType         MPU region type
 ** \arg SMPU1Region                    System DMA_1 MPU
 ** \arg SMPU2Region                    System DMA_2 MPU
 ** \arg FMPURegion                     System USBFS_DMA MPU
 **
 ** \retval Enable                      Enable the specified MPU region read permission
 ** \retval Disable                     Disable the specified MPU region read permission
 **
 ******************************************************************************/
en_functional_state_t MPU_GetProtRegionReadPermission(en_mpu_region_num_t enRegionNum,
                                                      en_mpu_region_type_t enMpuRegionType)
{
    uint32_t u32State = 0u;
    stc_mpu_rgcr0_field_t *RGCR_f = NULL;

    DDL_ASSERT(IS_VALID_MPU_REGION_NUM(enRegionNum));
    DDL_ASSERT(IS_VALID_MPU_REGION_TYPE(enMpuRegionType));

    RGCR_f = (stc_mpu_rgcr0_field_t *)MPU_RGCRx(enRegionNum);

    switch (enMpuRegionType)
    {
    case SMPU1Region:
        u32State = RGCR_f->S1RG0RP;
        break;
    case SMPU2Region:
        u32State = RGCR_f->S2RG0RP;
        break;
    case FMPURegion:
        u32State = RGCR_f->FRG0RP;
        break;
    default:
        break;
    }

    return (u32State ? Disable : Enable);
}

/**
 *******************************************************************************
 ** \brief Set MPU write permission of the specified protection region and enMpuRegionType.
 **
 ** \param [in] enRegionNum             MPU region number
 ** \arg This parameter can be a value of @ref en_mpu_region_num_t
 ** \param [in] enMpuRegionType         MPU region type
 ** \arg SMPU1Region                    System DMA_1 MPU
 ** \arg SMPU2Region                    System DMA_2 MPU
 ** \arg FMPURegion                     System USBFS_DMA MPU
 ** \param [in] enState                 MPU region state
 ** \arg Enable                         Enable the specified MPU region write permission
 ** \arg Disable                        Disable the specified MPU region write permission
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       enMpuRegionType is invalid
 **
 ******************************************************************************/
en_result_t MPU_SetProtRegionWritePermission(en_mpu_region_num_t enRegionNum,
                                             en_mpu_region_type_t enMpuRegionType,
                                             en_functional_state_t enState)
{
    en_result_t enRet = Ok;
    stc_mpu_rgcr0_field_t *RGCR_f = NULL;

    DDL_ASSERT(IS_FUNCTIONAL_STATE(enState));
    DDL_ASSERT(IS_VALID_MPU_REGION_NUM(enRegionNum));
    DDL_ASSERT(IS_VALID_MPU_REGION_TYPE(enMpuRegionType));

    RGCR_f = (stc_mpu_rgcr0_field_t *)MPU_RGCRx(enRegionNum);

    switch (enMpuRegionType)
    {
    case SMPU1Region:
        RGCR_f->S1RG0WP = ((Enable == enState) ? 0ul : 1ul);
        break;
    case SMPU2Region:
        RGCR_f->S2RG0WP = ((Enable == enState) ? 0ul : 1ul);
        break;
    case FMPURegion:
        RGCR_f->FRG0WP = ((Enable == enState) ? 0ul : 1ul);
        break;
    default:
        enRet = ErrorInvalidParameter;
        break;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get MPU write permission of the specified protection region and enMpuRegionType.
 **
 ** \param [in] enRegionNum             MPU region number
 ** \arg This parameter can be a value of @ref en_mpu_region_num_t
 ** \param [in] enMpuRegionType         MPU region type
 ** \arg SMPU1Region                    System DMA_1 MPU
 ** \arg SMPU2Region                    System DMA_2 MPU
 ** \arg FMPURegion                     System USBFS_DMA MPU
 **
 ** \retval Enable                      Enable the specified MPU region read permission
 ** \retval Disable                     Disable the specified MPU region read permission
 **
 ******************************************************************************/
en_functional_state_t MPU_GetProtRegionWritePermission(en_mpu_region_num_t enRegionNum,
                                                       en_mpu_region_type_t enMpuRegionType)
{
    uint32_t u32State = 0u;
    stc_mpu_rgcr0_field_t *RGCR_f = NULL;

    DDL_ASSERT(IS_VALID_MPU_REGION_NUM(enRegionNum));
    DDL_ASSERT(IS_VALID_MPU_REGION_TYPE(enMpuRegionType));

    RGCR_f = (stc_mpu_rgcr0_field_t *)MPU_RGCRx(enRegionNum);

    switch (enMpuRegionType)
    {
    case SMPU1Region:
        u32State = RGCR_f->S1RG0WP;
        break;
    case SMPU2Region:
        u32State = RGCR_f->S2RG0WP;
        break;
    case FMPURegion:
        u32State = RGCR_f->FRG0WP;
        break;
    default:
        break;
    }

    return (u32State ? Disable : Enable);
}

/**
 *******************************************************************************
 ** \brief Set MPU read permission of the specified background region and enMpuRegionType.
 **
 ** \param [in] enMpuRegionType         MPU region type
 ** \arg SMPU1Region                    System DMA_1 MPU
 ** \arg SMPU2Region                    System DMA_2 MPU
 ** \arg FMPURegion                     System USBFS_DMA MPU
 ** \param [in] enState                 MPU region state
 ** \arg Enable                         Enable the specified MPU region read permission
 ** \arg Disable                        Disable the specified MPU region read permission
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       enMpuRegionType is invalid
 **
 ******************************************************************************/
en_result_t MPU_SetBkgdRegionReadPermission(en_mpu_region_type_t enMpuRegionType,
                                            en_functional_state_t enState)
{
    en_result_t enRet = Ok;

    DDL_ASSERT(IS_FUNCTIONAL_STATE(enState));
    DDL_ASSERT(IS_VALID_MPU_REGION_TYPE(enMpuRegionType));

    switch (enMpuRegionType)
    {
    case SMPU1Region:
        M4_MPU->CR_f.SMPU1BRP = ((Enable == enState) ? 0ul : 1ul);
        break;
    case SMPU2Region:
        M4_MPU->CR_f.SMPU2BRP = ((Enable == enState) ? 0ul : 1ul);
        break;
    case FMPURegion:
        M4_MPU->CR_f.FMPUBRP = ((Enable == enState) ? 0ul : 1ul);
        break;
    default:
        enRet = ErrorInvalidParameter;
        break;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get MPU read permission of the specified background region and enMpuRegionType.
 **
 ** \param [in] enMpuRegionType         MPU region type
 ** \arg SMPU1Region                    System DMA_1 MPU
 ** \arg SMPU2Region                    System DMA_2 MPU
 ** \arg FMPURegion                     System USBFS_DMA MPU
 **
 ** \retval Enable                      Enable the specified MPU region read permission
 ** \retval Disable                     Disable the specified MPU region read permission
 **
 ******************************************************************************/
en_functional_state_t MPU_GetBkgdRegionReadPermission(en_mpu_region_type_t enMpuRegionType)
{
    uint32_t u32State = 0u;

    DDL_ASSERT(IS_VALID_MPU_REGION_TYPE(enMpuRegionType));

    switch (enMpuRegionType)
    {
    case SMPU1Region:
        u32State = M4_MPU->CR_f.SMPU1BRP;
        break;
    case SMPU2Region:
        u32State = M4_MPU->CR_f.SMPU2BRP;
        break;
    case FMPURegion:
        u32State = M4_MPU->CR_f.FMPUBRP;
        break;
    default:
        break;
    }

    return (u32State ? Disable : Enable);
}

/**
 *******************************************************************************
 ** \brief Set MPU write permission of the specified background region and enMpuRegionType.
 **
 ** \param [in] enMpuRegionType         MPU region type
 ** \arg SMPU1Region                    System DMA_1 MPU
 ** \arg SMPU2Region                    System DMA_2 MPU
 ** \arg FMPURegion                     System USBFS_DMA MPU
 ** \param [in] enState                 MPU region state
 ** \arg Enable                         Enable the specified MPU region write permission
 ** \arg Disable                        Disable the specified MPU region write permission
 **
 ** \retval Ok                          Set successfully.
 ** \retval ErrorInvalidParameter       enMpuRegionType is invalid
 **
 ******************************************************************************/
en_result_t MPU_SetBkgdRegionWritePermission(en_mpu_region_type_t enMpuRegionType,
                                             en_functional_state_t enState)
{
    en_result_t enRet = Ok;

    DDL_ASSERT(IS_FUNCTIONAL_STATE(enState));
    DDL_ASSERT(IS_VALID_MPU_REGION_TYPE(enMpuRegionType));

    switch (enMpuRegionType)
    {
    case SMPU1Region:
        M4_MPU->CR_f.SMPU1BWP = ((Enable == enState) ? 0ul : 1ul);
        break;
    case SMPU2Region:
        M4_MPU->CR_f.SMPU2BWP = ((Enable == enState) ? 0ul : 1ul);
        break;
    case FMPURegion:
        M4_MPU->CR_f.FMPUBWP = ((Enable == enState) ? 0ul : 1ul);
        break;
    default:
        enRet = ErrorInvalidParameter;
        break;
    }

    return enRet;
}

/**
 *******************************************************************************
 ** \brief Get MPU write permission of the specified background region and enMpuRegionType.
 **
 ** \param [in] enMpuRegionType         MPU region type
 ** \arg SMPU1Region                    System DMA_1 MPU
 ** \arg SMPU2Region                    System DMA_2 MPU
 ** \arg FMPURegion                     System USBFS_DMA MPU
 **
 ** \retval Enable                      Enable the specified MPU region read permission
 ** \retval Disable                     Disable the specified MPU region read permission
 **
 ******************************************************************************/
en_functional_state_t MPU_GetBkgdRegionWritePermission(en_mpu_region_type_t enMpuRegionType)
{
    uint32_t u32State = 0u;

    DDL_ASSERT(IS_VALID_MPU_REGION_TYPE(enMpuRegionType));

    switch (enMpuRegionType)
    {
    case SMPU1Region:
        u32State = M4_MPU->CR_f.SMPU1BWP;
        break;
    case SMPU2Region:
        u32State = M4_MPU->CR_f.SMPU2BWP;
        break;
    case FMPURegion:
        u32State = M4_MPU->CR_f.FMPUBWP;
        break;
    default:
        break;
    }

    return (u32State ? Disable : Enable);
}

/**
 *******************************************************************************
 ** \brief Set MPU function of the specified region and type.
 **
 ** \param [in] enState                 MPU write protection state
 ** \arg Enable                         Enable the write protection function
 ** \arg Disable                        Disable the write protection function
 **
 ** \retval Ok                          Set successfully.
 **
 ******************************************************************************/
en_result_t MPU_WriteProtCmd(en_functional_state_t enState)
{
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enState));

    M4_MPU->WP = (MPU_WRITE_PROT_KEY | ((Enable == enState) ? 0ul : 1ul));

    return Ok;
}

/**
 *******************************************************************************
 ** \brief Enable the specified IP Write/Read protection.
 **
 ** \param [in] u32ProtMode             Ip protection mode
 ** \arg AesReadProt                    AES read protection
 ** \arg AesWriteProt                   AES write protection
 ** \arg HashReadProt                   HASH read protection
 ** \arg HashWriteProt                  HASH write protection
 ** \arg TrngReadProt                   TRNG read protection
 ** \arg TrngWriteProt                  TRNG write protection
 ** \arg CrcReadProt                    CRC read protection
 ** \arg CrcWriteProt                   CRC write protection
 ** \arg FmcReadProt                    FMC read protection
 ** \arg FmcWriteProt                   FMC write protection
 ** \arg WdtReadProt                    WDT read protection
 ** \arg WdtWriteProt                   WDT write protection
 ** \arg SwdtReadProt                   WDT read protection
 ** \arg SwdtWriteProt                  WDT write protection
 ** \arg BksramReadProt                 BKSRAM read protection
 ** \arg BksramWriteProt                BKSRAM write protection
 ** \arg RtcReadProt                    RTC read protection
 ** \arg RtcWriteProt                   RTC write protection
 ** \arg DmpuReadProt                   DMPU read protection
 ** \arg DmpuWriteProt                  DMPU write protection
 ** \arg SramcReadProt                  SRAMC read protection
 ** \arg SramcWriteProt                 SRAMC write protection
 ** \arg IntcReadProt                   INTC read protection
 ** \arg IntcWriteProt                  INTC write protection
 ** \arg SyscReadProt                   SYSC read protection
 ** \arg SyscWriteProt                  SYSC write protection
 ** \arg MstpWriteProt                  MSTP write protection
 ** \arg MstpWriteProt                  MSTP write protection
 ** \arg BusErrProt                     BUSERR write protection
 ** \param [in] enState                 MPU IP protection state
 ** \arg Enable                         Enable the IP protection function
 ** \arg Disable                        Disable the IP protection function
 **
 ** \retval Ok                          Set successfully.
 **
 ******************************************************************************/
en_result_t MPU_IpProtCmd(uint32_t u32ProtMode,
                          en_functional_state_t enState)
{
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enState));

    if (Enable == enState)
    {
        M4_SYSREG->MPU_IPPR |= u32ProtMode;
    }
    else
    {
        M4_SYSREG->MPU_IPPR &= (~u32ProtMode);
    }

    return Ok;
}

//@} // MpuGroup

#endif /* DDL_MPU_ENABLE */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
