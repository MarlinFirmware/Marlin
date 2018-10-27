/**********************************************************************
* $Id$		lpc17xx_can.c				2011-03-09
*//**
* @file		lpc17xx_can.c
* @brief	Contains all functions support for CAN firmware library on LPC17xx
* @version	3.3
* @date		09. March. 2011
* @author	NXP MCU SW Application Team
*
* Copyright(C) 2011, NXP Semiconductor
* All rights reserved.
*
***********************************************************************
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
* NXP Semiconductors assumes no responsibility or liability for the
* use of the software, conveys no license or title under any patent,
* copyright, or mask work right to the product. NXP Semiconductors
* reserves the right to make changes in the software without
* notification. NXP Semiconductors also make no representation or
* warranty that such application will be suitable for the specified
* use without further testing or modification.
* Permission to use, copy, modify, and distribute this software and its
* documentation is hereby granted, under NXP Semiconductors'
* relevant copyright in the software, without fee, provided that it
* is used in conjunction with NXP Semiconductors microcontrollers.  This
* copyright, permission, and disclaimer notice must appear in all copies of
* this code.
**********************************************************************/

/* Peripheral group ----------------------------------------------------------- */
/** @addtogroup CAN
 * @{
 */

/* Includes ------------------------------------------------------------------- */
#include "lpc17xx_can.h"
#include "lpc17xx_clkpwr.h"

/* If this source file built with example, the LPC17xx FW library configuration
 * file in each example directory ("lpc17xx_libcfg.h") must be included,
 * otherwise the default FW library configuration file must be included instead
 */
#ifdef __BUILD_WITH_EXAMPLE__
#include "lpc17xx_libcfg.h"
#else
#include "lpc17xx_libcfg_default.h"
#endif /* __BUILD_WITH_EXAMPLE__ */


#ifdef _CAN

/* Private Variables ---------------------------------------------------------- */
/** @defgroup CAN_Private_Variables CAN Private Variables
 * @{
 */

FunctionalState FULLCAN_ENABLE;


/* Counts number of filters (CAN message objects) used */
uint16_t CANAF_FullCAN_cnt = 0;
uint16_t CANAF_std_cnt = 0;
uint16_t CANAF_gstd_cnt = 0;
uint16_t CANAF_ext_cnt = 0;
uint16_t CANAF_gext_cnt = 0;

/* End of Private Variables ----------------------------------------------------*/
/**
 * @}
 */

/* Private Variables ---------------------------------------------------------- */
static void can_SetBaudrate (LPC_CAN_TypeDef *CANx, uint32_t baudrate);

/*********************************************************************//**
 * @brief 		Setting CAN baud rate (bps)
 * @param[in] 	CANx point to LPC_CAN_TypeDef object, should be:
 * 				- LPC_CAN1: CAN1 peripheral
 * 				- LPC_CAN2: CAN2 peripheral
 * @param[in]	baudrate: is the baud rate value will be set
 * @return 		None
 ***********************************************************************/
static void can_SetBaudrate (LPC_CAN_TypeDef *CANx, uint32_t baudrate)
{
	uint32_t result = 0;
	uint8_t NT, TSEG1, TSEG2, BRFail;
	uint32_t CANPclk = 0;
	uint32_t BRP;
	CHECK_PARAM(PARAM_CANx(CANx));

	if (CANx == LPC_CAN1)
	{
		CANPclk = CLKPWR_GetPCLK (CLKPWR_PCLKSEL_CAN1);
	}
	else
	{
		CANPclk = CLKPWR_GetPCLK (CLKPWR_PCLKSEL_CAN2);
	}
	result = CANPclk / baudrate;
	/* Calculate suitable nominal time value
	 * NT (nominal time) = (TSEG1 + TSEG2 + 3)
	 * NT <= 24
	 * TSEG1 >= 2*TSEG2
	 */
	BRFail = 1;
	for(NT=24;NT>0;NT=NT-2)
	{
		if ((result%NT)==0)
		{
			BRP = result / NT - 1;
			NT--;
			TSEG2 = (NT/3) - 1;
			TSEG1 = NT -(NT/3) - 1;
			BRFail = 0;
			break;
		}
	}
	if(BRFail)
		while(1); // Failed to calculate exact CAN baud rate
	/* Enter reset mode */
	CANx->MOD = 0x01;
	/* Set bit timing
	 * Default: SAM = 0x00;
	 *          SJW = 0x03;
	 */
	CANx->BTR  = (TSEG2<<20)|(TSEG1<<16)|(3<<14)|BRP;
	/* Return to normal operating */
	CANx->MOD = 0;
}
/* End of Private Functions ----------------------------------------------------*/


/* Public Functions ----------------------------------------------------------- */
/** @addtogroup CAN_Public_Functions
 * @{
 */

/********************************************************************//**
 * @brief		Initialize CAN peripheral with given baudrate
 * @param[in]	CANx pointer to LPC_CAN_TypeDef, should be:
 * 				- LPC_CAN1: CAN1 peripheral
 * 				- LPC_CAN2: CAN2 peripheral
 * @param[in]	baudrate: the value of CAN baudrate will be set (bps)
 * @return 		None
 *********************************************************************/
void CAN_Init(LPC_CAN_TypeDef *CANx, uint32_t baudrate)
{
	uint16_t i;
	CHECK_PARAM(PARAM_CANx(CANx));

	if(CANx == LPC_CAN1)
	{
		/* Turn on power and clock for CAN1 */
		CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCAN1, ENABLE);
		/* Set clock divide for CAN1 */
	}
	else
	{
		/* Turn on power and clock for CAN1 */
		CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCAN2, ENABLE);
		/* Set clock divide for CAN2 */
	}
	CLKPWR_SetPCLKDiv (CLKPWR_PCLKSEL_CAN1, CLKPWR_PCLKSEL_CCLK_DIV_2);
	CLKPWR_SetPCLKDiv (CLKPWR_PCLKSEL_CAN2, CLKPWR_PCLKSEL_CCLK_DIV_2);
	CLKPWR_SetPCLKDiv (CLKPWR_PCLKSEL_ACF, CLKPWR_PCLKSEL_CCLK_DIV_2);

	CANx->MOD = 1; // Enter Reset Mode
	CANx->IER = 0; // Disable All CAN Interrupts
	CANx->GSR = 0;
	/* Request command to release Rx, Tx buffer and clear data overrun */
	//CANx->CMR = CAN_CMR_AT | CAN_CMR_RRB | CAN_CMR_CDO;
	CANx->CMR = (1<<1)|(1<<2)|(1<<3);
	/* Read to clear interrupt pending in interrupt capture register */
	i = CANx->ICR;
	CANx->MOD = 0;// Return Normal operating

	//Reset CANAF value
	LPC_CANAF->AFMR = 0x01;

	//clear ALUT RAM
	for (i = 0; i < 512; i++) {
		LPC_CANAF_RAM->mask[i] = 0x00;
	}

	LPC_CANAF->SFF_sa = 0x00;
	LPC_CANAF->SFF_GRP_sa = 0x00;
	LPC_CANAF->EFF_sa = 0x00;
	LPC_CANAF->EFF_GRP_sa = 0x00;
	LPC_CANAF->ENDofTable = 0x00;

	LPC_CANAF->AFMR = 0x00;
	/* Set baudrate */
	can_SetBaudrate (CANx, baudrate);
}

/********************************************************************//**
 * @brief		CAN deInit
 * @param[in]	CANx pointer to LPC_CAN_TypeDef, should be:
 * 				- LPC_CAN1: CAN1 peripheral
 * 				- LPC_CAN2: CAN2 peripheral
 * @return 		None
 *********************************************************************/
void CAN_DeInit(LPC_CAN_TypeDef *CANx)
{
	CHECK_PARAM(PARAM_CANx(CANx));

	if(CANx == LPC_CAN1)
	{
		/* Turn on power and clock for CAN1 */
		CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCAN1, DISABLE);
	}
	else
	{
		/* Turn on power and clock for CAN1 */
		CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCAN2, DISABLE);
	}
}

/********************************************************************//**
 * @brief		Setup Acceptance Filter Look-Up Table
 * @param[in]	CANAFx	pointer to LPC_CANAF_TypeDef
 * 				Should be: LPC_CANAF
 * @param[in]	AFSection	the pointer to AF_SectionDef structure
 * 				It contain information about 5 sections will be install in AFLUT
 * @return 		CAN Error	could be:
 * 				- CAN_OBJECTS_FULL_ERROR: No more rx or tx objects available
 * 				- CAN_AF_ENTRY_ERROR: table error-violation of ascending numerical order
 * 				- CAN_OK: ID is added into table successfully
 *********************************************************************/
CAN_ERROR CAN_SetupAFLUT(LPC_CANAF_TypeDef* CANAFx, AF_SectionDef* AFSection)
{
	uint8_t ctrl1,ctrl2;
	uint8_t dis1, dis2;
	uint16_t SID, ID_temp,i, count = 0;
	uint32_t EID, entry, buf;
	uint16_t lowerSID, upperSID;
	uint32_t lowerEID, upperEID;

	CHECK_PARAM(PARAM_CANAFx(CANAFx));
	CANAFx->AFMR = 0x01;

/***** setup FullCAN Table *****/
	if(AFSection->FullCAN_Sec == NULL)
	{
		FULLCAN_ENABLE = DISABLE;
	}
	else
	{
		FULLCAN_ENABLE = ENABLE;
		for(i=0;i<(AFSection->FC_NumEntry);i++)
		{
			if(count + 1 > 64)
			{
				return CAN_OBJECTS_FULL_ERROR;
			}
			ctrl1 = AFSection->FullCAN_Sec->controller;
			SID = AFSection->FullCAN_Sec->id_11;
			dis1 = AFSection->FullCAN_Sec->disable;

			CHECK_PARAM(PARAM_CTRL(ctrl1));
			CHECK_PARAM(PARAM_ID_11(SID));
			CHECK_PARAM(PARAM_MSG_DISABLE(dis1));
			entry = 0x00; //reset entry value
			if((CANAF_FullCAN_cnt & 0x00000001)==0)
			{
				if(count!=0x00)
				{
					buf = LPC_CANAF_RAM->mask[count-1];
					ID_temp = (buf & 0xE7FF); //mask controller & identifier bits
					if(ID_temp > ((ctrl1<<13)|SID))
					{
						return CAN_AF_ENTRY_ERROR;
					}
				}
				entry = (ctrl1<<29)|(dis1<<28)|(SID<<16)|(1<<27);
				LPC_CANAF_RAM->mask[count] &= 0x0000FFFF;
				LPC_CANAF_RAM->mask[count] |= entry;
				CANAF_FullCAN_cnt++;
				if(CANAF_FullCAN_cnt == AFSection->FC_NumEntry) //this is the lastest FullCAN entry
					count++;
			}
			else
			{
				buf = LPC_CANAF_RAM->mask[count];
				ID_temp = (buf >>16) & 0xE7FF;
				if(ID_temp > ((ctrl1<<13)|SID))
				{
					return CAN_AF_ENTRY_ERROR;
				}
				entry = (ctrl1<<13)|(dis1<<12)|(SID<<0)|(1<<11);
				LPC_CANAF_RAM->mask[count] &= 0xFFFF0000;
				LPC_CANAF_RAM->mask[count]|= entry;
				count++;
				CANAF_FullCAN_cnt++;
			}
			AFSection->FullCAN_Sec = (FullCAN_Entry *)((uint32_t)(AFSection->FullCAN_Sec)+ sizeof(FullCAN_Entry));
		}
	}

/***** Setup Explicit Standard Frame Format Section *****/
	if(AFSection->SFF_Sec != NULL)
	{
		for(i=0;i<(AFSection->SFF_NumEntry);i++)
		{
			if(count + 1 > 512)
			{
				return CAN_OBJECTS_FULL_ERROR;
			}
			ctrl1 = AFSection->SFF_Sec->controller;
			SID = AFSection->SFF_Sec->id_11;
			dis1 = AFSection->SFF_Sec->disable;

			//check parameter
			CHECK_PARAM(PARAM_CTRL(ctrl1));
			CHECK_PARAM(PARAM_ID_11(SID));
			CHECK_PARAM(PARAM_MSG_DISABLE(dis1));

			entry = 0x00; //reset entry value
			if((CANAF_std_cnt & 0x00000001)==0)
			{
				if(CANAF_std_cnt !=0 )
				{
					buf = LPC_CANAF_RAM->mask[count-1];
					ID_temp = (buf & 0xE7FF); //mask controller & identifier bits
					if(ID_temp > ((ctrl1<<13)|SID))
					{
						return CAN_AF_ENTRY_ERROR;
					}
				}
				entry = (ctrl1<<29)|(dis1<<28)|(SID<<16);
				LPC_CANAF_RAM->mask[count] &= 0x0000FFFF;
				LPC_CANAF_RAM->mask[count] |= entry;
				CANAF_std_cnt++;
				if(CANAF_std_cnt == AFSection->SFF_NumEntry)//if this is the last SFF entry
					count++;
			}
			else
			{
				buf = LPC_CANAF_RAM->mask[count];
				ID_temp = (buf >>16) & 0xE7FF;
				if(ID_temp > ((ctrl1<<13)|SID))
				{
					return CAN_AF_ENTRY_ERROR;
				}
				entry = (ctrl1<<13)|(dis1<<12)|(SID<<0);
				LPC_CANAF_RAM->mask[count] &= 0xFFFF0000;
				LPC_CANAF_RAM->mask[count] |= entry;
				count++;
				CANAF_std_cnt++;
			}
			AFSection->SFF_Sec = (SFF_Entry *)((uint32_t)(AFSection->SFF_Sec)+ sizeof(SFF_Entry));
		}
	}

/***** Setup Group of Standard Frame Format Identifier Section *****/
	if(AFSection->SFF_GPR_Sec != NULL)
	{
		for(i=0;i<(AFSection->SFF_GPR_NumEntry);i++)
		{
			if(count + 1 > 512)
			{
				return CAN_OBJECTS_FULL_ERROR;
			}
			ctrl1 = AFSection->SFF_GPR_Sec->controller1;
			ctrl2 = AFSection->SFF_GPR_Sec->controller2;
			dis1 = AFSection->SFF_GPR_Sec->disable1;
			dis2 = AFSection->SFF_GPR_Sec->disable2;
			lowerSID = AFSection->SFF_GPR_Sec->lowerID;
			upperSID = AFSection->SFF_GPR_Sec->upperID;

			/* check parameter */
			CHECK_PARAM(PARAM_CTRL(ctrl1));
			CHECK_PARAM(PARAM_CTRL(ctrl2));
			CHECK_PARAM(PARAM_MSG_DISABLE(dis1));
			CHECK_PARAM(PARAM_MSG_DISABLE(dis2));
			CHECK_PARAM(PARAM_ID_11(lowerSID));
			CHECK_PARAM(PARAM_ID_11(upperSID));

			entry = 0x00;
			if(CANAF_gstd_cnt!=0)
			{
				buf = LPC_CANAF_RAM->mask[count-1];
				ID_temp = buf & 0xE7FF;
				if((ctrl1 != ctrl2)||(lowerSID > upperSID)||(ID_temp > ((ctrl1<<13)|lowerSID)))
				{
					return CAN_AF_ENTRY_ERROR;
				}
			}
			entry = (ctrl1 << 29)|(dis1 << 28)|(lowerSID << 16)|  \
					(ctrl2 << 13)|(dis2 << 12)|(upperSID << 0);
			LPC_CANAF_RAM->mask[count] = entry;
			CANAF_gstd_cnt++;
			count++;
			AFSection->SFF_GPR_Sec = (SFF_GPR_Entry *)((uint32_t)(AFSection->SFF_GPR_Sec)+ sizeof(SFF_GPR_Entry));
		}
	}

/***** Setup Explicit Extend Frame Format Identifier Section *****/
	if(AFSection->EFF_Sec != NULL)
	{
		for(i=0;i<(AFSection->EFF_NumEntry);i++)
		{
			if(count + 1 > 512)
			{
				return CAN_OBJECTS_FULL_ERROR;
			}
			EID = AFSection->EFF_Sec->ID_29;
			ctrl1 = AFSection->EFF_Sec->controller;

			// check parameter
			CHECK_PARAM(PARAM_ID_29(EID));
			CHECK_PARAM(PARAM_CTRL(ctrl1));

			entry = (ctrl1 << 29)|(EID << 0);
			if(CANAF_ext_cnt != 0)
			{
				buf = LPC_CANAF_RAM->mask[count-1];
//				EID_temp = buf & 0x0FFFFFFF;
				if(buf > entry)
				{
					return CAN_AF_ENTRY_ERROR;
				}
			}
			LPC_CANAF_RAM->mask[count] = entry;
			CANAF_ext_cnt ++;
			count++;
			AFSection->EFF_Sec = (EFF_Entry *)((uint32_t)(AFSection->EFF_Sec)+ sizeof(EFF_Entry));
		}
	}

/***** Setup Group of Extended Frame Format Identifier Section *****/
	if(AFSection->EFF_GPR_Sec != NULL)
	{
		for(i=0;i<(AFSection->EFF_GPR_NumEntry);i++)
		{
			if(count + 2 > 512)
			{
				return CAN_OBJECTS_FULL_ERROR;
			}
			ctrl1 = AFSection->EFF_GPR_Sec->controller1;
			ctrl2 = AFSection->EFF_GPR_Sec->controller2;
			lowerEID = AFSection->EFF_GPR_Sec->lowerEID;
			upperEID = AFSection->EFF_GPR_Sec->upperEID;

			//check parameter
			CHECK_PARAM(PARAM_CTRL(ctrl1));
			CHECK_PARAM(PARAM_CTRL(ctrl2));
			CHECK_PARAM(PARAM_ID_29(lowerEID));
			CHECK_PARAM(PARAM_ID_29(upperEID));

			entry = 0x00;
			if(CANAF_gext_cnt != 0)
			{
				buf = LPC_CANAF_RAM->mask[count-1];
//				EID_temp = buf & 0x0FFFFFFF;
				if((ctrl1 != ctrl2) || (lowerEID > upperEID) || (buf > ((ctrl1 << 29)|(lowerEID << 0))))
				{
					return CAN_AF_ENTRY_ERROR;
				}
			}
			entry = (ctrl1 << 29)|(lowerEID << 0);
			LPC_CANAF_RAM->mask[count++] = entry;
			entry = (ctrl2 << 29)|(upperEID << 0);
			LPC_CANAF_RAM->mask[count++] = entry;
			CANAF_gext_cnt++;
			AFSection->EFF_GPR_Sec = (EFF_GPR_Entry *)((uint32_t)(AFSection->EFF_GPR_Sec)+ sizeof(EFF_GPR_Entry));
		}
	}
	//update address values
	LPC_CANAF->SFF_sa = ((CANAF_FullCAN_cnt + 1)>>1)<<2;
	LPC_CANAF->SFF_GRP_sa = LPC_CANAF->SFF_sa + (((CANAF_std_cnt+1)>>1)<< 2);
	LPC_CANAF->EFF_sa = LPC_CANAF->SFF_GRP_sa + (CANAF_gstd_cnt << 2);
	LPC_CANAF->EFF_GRP_sa = LPC_CANAF->EFF_sa + (CANAF_ext_cnt << 2);
	LPC_CANAF->ENDofTable = LPC_CANAF->EFF_GRP_sa + (CANAF_gext_cnt << 3);

	if(FULLCAN_ENABLE == DISABLE)
	{
		LPC_CANAF->AFMR = 0x00; // Normal mode
	}
	else
	{
		LPC_CANAF->AFMR = 0x04;
	}
	return CAN_OK;
}
/********************************************************************//**
 * @brief		Add Explicit ID into AF Look-Up Table dynamically.
 * @param[in]	CANx pointer to LPC_CAN_TypeDef, should be:
 * 				- LPC_CAN1: CAN1 peripheral
 * 				- LPC_CAN2: CAN2 peripheral
 * @param[in]	id: The ID of entry will be added
 * @param[in]	format: is the type of ID Frame Format, should be:
 * 				- STD_ID_FORMAT: 11-bit ID value
 * 				- EXT_ID_FORMAT: 29-bit ID value
 * @return 		CAN Error, could be:
 * 				- CAN_OBJECTS_FULL_ERROR: No more rx or tx objects available
 * 				- CAN_ID_EXIT_ERROR: ID exited in table
 * 				- CAN_OK: ID is added into table successfully
 *********************************************************************/
CAN_ERROR CAN_LoadExplicitEntry(LPC_CAN_TypeDef* CANx, uint32_t id, CAN_ID_FORMAT_Type format)
{
	uint32_t tmp0 = 0;
	uint32_t buf0=0, buf1=0;
	int16_t cnt1=0, cnt2=0, bound1=0, total=0;


	CHECK_PARAM(PARAM_CANx(CANx));
	CHECK_PARAM(PARAM_ID_FORMAT(format));

	if (CANx == LPC_CAN1)
	{
		tmp0 = 0;
	}
	else if (CANx == LPC_CAN2)
	{
		tmp0 = 1;
	}

	/* Acceptance Filter Memory full - return */
	total =((CANAF_FullCAN_cnt+1)>>1)+ CANAF_FullCAN_cnt*3 +((CANAF_std_cnt + 1) >> 1)+  \
			CANAF_gstd_cnt + CANAF_ext_cnt + (CANAF_gext_cnt<<1);
	if (total >= 512){ //don't have enough space
		return CAN_OBJECTS_FULL_ERROR;
	}

	/* Setup Acceptance Filter Configuration
    Acceptance Filter Mode Register = Off */
	LPC_CANAF->AFMR = 0x00000001;

/*********** Add Explicit Standard Identifier Frame Format entry *********/
 	if(format == STD_ID_FORMAT)
 	{
 		id &= 0x07FF;
 		id |= (tmp0 << 13); /* Add controller number */
		/* Move all remaining sections one place up
		if new entry will increase FullCAN list */
		if ((CANAF_std_cnt & 0x0001) == 0)
		{
			cnt1   = ((CANAF_FullCAN_cnt+1)>>1)+((CANAF_std_cnt+1)>>1);
			bound1 = total - cnt1;
			buf0   = LPC_CANAF_RAM->mask[cnt1];
			while(bound1--)
			{
				cnt1++;
				buf1 = LPC_CANAF_RAM->mask[cnt1];
				LPC_CANAF_RAM->mask[cnt1] = buf0;
				buf0 = buf1;
			}
		}
		if (CANAF_std_cnt == 0)
		{
			cnt2 = (CANAF_FullCAN_cnt + 1)>>1;
			/* For entering first ID */
			LPC_CANAF_RAM->mask[cnt2] = 0x0000FFFF | (id << 16);
		}
		else if (CANAF_std_cnt == 1)
		{
			cnt2 = (CANAF_FullCAN_cnt + 1)>>1;
			/* For entering second ID */
			if (((LPC_CANAF_RAM->mask[cnt2] >> 16)& 0xE7FF) > id)
			{
				LPC_CANAF_RAM->mask[cnt2] = (LPC_CANAF_RAM->mask[cnt2] >> 16) | (id << 16);
			}
			else
			{
				LPC_CANAF_RAM->mask[cnt2] = (LPC_CANAF_RAM->mask[cnt2] & 0xFFFF0000) | id;
			}
		}
		else
		{
			/* Find where to insert new ID */
			cnt1 = (CANAF_FullCAN_cnt+1)>>1;
			cnt2 = CANAF_std_cnt;
			bound1 = ((CANAF_FullCAN_cnt+1)>>1)+((CANAF_std_cnt+1)>>1);
			while (cnt1 < bound1)
			{
				/* Loop through standard existing IDs */
				if (((LPC_CANAF_RAM->mask[cnt1] >> 16) & 0xE7FF) > id)
				{
					cnt2 = cnt1 * 2;
					break;
				}

				if ((LPC_CANAF_RAM->mask[cnt1] & 0x0000E7FF) > id)
				{
					cnt2 = cnt1 * 2 + 1;
					break;
				}

				cnt1++;
			}
			/* cnt1 = U32 where to insert new ID */
			/* cnt2 = U16 where to insert new ID */

			if (cnt1 == bound1)
			{
				/* Adding ID as last entry */
				/* Even number of IDs exists */
				if ((CANAF_std_cnt & 0x0001) == 0)
				{
					LPC_CANAF_RAM->mask[cnt1]  = 0x0000FFFF | (id << 16);
				}
				/* Odd  number of IDs exists */
				else
				{
					LPC_CANAF_RAM->mask[cnt1]  = (LPC_CANAF_RAM->mask[cnt1] & 0xFFFF0000) | id;
				}
			}
			else
			{
				buf0 = LPC_CANAF_RAM->mask[cnt1]; /* Remember current entry */
				if ((cnt2 & 0x0001) == 0)
				{
					/* Insert new mask to even address*/
					buf1 = (id << 16) | (buf0 >> 16);
				}
				else
				{
					/* Insert new mask to odd  address */
					buf1 = (buf0 & 0xFFFF0000) | id;
				}
				LPC_CANAF_RAM->mask[cnt1] = buf1;/* Insert mask */
				bound1 = ((CANAF_FullCAN_cnt+1)>>1)+((CANAF_std_cnt+1)>>1)-1;
				/* Move all remaining standard mask entries one place up */
				while (cnt1 < bound1)
				{
					cnt1++;
					buf1  = LPC_CANAF_RAM->mask[cnt1];
					LPC_CANAF_RAM->mask[cnt1] = (buf1 >> 16) | (buf0 << 16);
					buf0  = buf1;
				}

				if ((CANAF_std_cnt & 0x0001) == 0)
				{
					/* Even number of IDs exists */
					LPC_CANAF_RAM->mask[cnt1+1] = (buf0 <<16) |(0x0000FFFF);
				}
			}
		}
		CANAF_std_cnt++;
		//update address values
		LPC_CANAF->SFF_GRP_sa +=0x04 ;
		LPC_CANAF->EFF_sa     +=0x04 ;
		LPC_CANAF->EFF_GRP_sa +=0x04;
		LPC_CANAF->ENDofTable +=0x04;
 	}

/*********** Add Explicit Extended Identifier Frame Format entry *********/
 	else
 	{
 		/* Add controller number */
 		id |= (tmp0) << 29;

 		cnt1 = ((CANAF_FullCAN_cnt+1)>>1)+(((CANAF_std_cnt + 1) >> 1) + CANAF_gstd_cnt);
 		cnt2 = 0;
 		while (cnt2 < CANAF_ext_cnt)
 		{
 			/* Loop through extended existing masks*/
 			if (LPC_CANAF_RAM->mask[cnt1] > id)
 			{
 				break;
 			}
 			cnt1++;/* cnt1 = U32 where to insert new mask */
			cnt2++;
 		}

 		buf0 = LPC_CANAF_RAM->mask[cnt1];  /* Remember current entry */
 		LPC_CANAF_RAM->mask[cnt1] = id;    /* Insert mask */

 		CANAF_ext_cnt++;

 		bound1 = total;
 		/* Move all remaining extended mask entries one place up*/
 		while (cnt2 < bound1)
 		{
 			cnt1++;
 			cnt2++;
 			buf1 = LPC_CANAF_RAM->mask[cnt1];
 			LPC_CANAF_RAM->mask[cnt1] = buf0;
 			buf0 = buf1;
 		}
 		/* update address values */
 		LPC_CANAF->EFF_GRP_sa += 4;
 		LPC_CANAF->ENDofTable += 4;
 	}
 	if(CANAF_FullCAN_cnt == 0) //not use FullCAN mode
 	{
 		LPC_CANAF->AFMR = 0x00;//not use FullCAN mode
 	}
 	else
 	{
 		LPC_CANAF->AFMR = 0x04;
 	}

 	return CAN_OK;
}

/********************************************************************//**
 * @brief		Load FullCAN entry into AFLUT
 * @param[in]	CANx: CAN peripheral selected, should be:
 * 				- LPC_CAN1: CAN1 peripheral
 * 				- LPC_CAN2: CAN2 peripheral
 * @param[in]	id: identifier of entry that will be added
 * @return 		CAN_ERROR, could be:
 * 				- CAN_OK: loading is successful
 * 				- CAN_ID_EXIT_ERROR: ID exited in FullCAN Section
 * 				- CAN_OBJECTS_FULL_ERROR: no more space available
 *********************************************************************/
CAN_ERROR CAN_LoadFullCANEntry (LPC_CAN_TypeDef* CANx, uint16_t id)
{
	uint32_t ctrl0 = 0;
	uint32_t buf0=0, buf1=0, buf2=0;
	uint32_t tmp0=0, tmp1=0, tmp2=0;
	int16_t cnt1=0, cnt2=0, bound1=0, total=0;

	CHECK_PARAM(PARAM_CANx(CANx));

	if (CANx == LPC_CAN1)
	{
		ctrl0 = 0;
	}
	else if (CANx == LPC_CAN2)
	{
		ctrl0 = 1;
	}

	/* Acceptance Filter Memory full - return */
	total =((CANAF_FullCAN_cnt+1)>>1)+ CANAF_FullCAN_cnt*3 +((CANAF_std_cnt + 1) >> 1)+  \
			CANAF_gstd_cnt + CANAF_ext_cnt + (CANAF_gext_cnt<<1);
	//don't have enough space for this fullCAN Entry and its Object(3*32 bytes)
	if ((total >=508)||(CANAF_FullCAN_cnt>=64)){
		return CAN_OBJECTS_FULL_ERROR;
	}
	/* Setup Acceptance Filter Configuration
    Acceptance Filter Mode Register = Off */
	LPC_CANAF->AFMR = 0x00000001;

	/* Add mask for standard identifiers   */
	id &= 0x07FF;
	id |= (ctrl0 << 13) | (1 << 11); /* Add controller number */
//	total = ((CANAF_std_cnt + 1) >> 1)+ CANAF_gstd_cnt + CANAF_ext_cnt + (CANAF_gext_cnt<<1);
	/* Move all remaining sections one place up
	if new entry will increase FullCAN list */
	if (((CANAF_FullCAN_cnt & 0x0001) == 0)&&(total!=0))
	{
		//then remove remaining section
		cnt1   = (CANAF_FullCAN_cnt >> 1);
		bound1 = total;
		buf0   = LPC_CANAF_RAM->mask[cnt1];

		while (bound1--)
		{
			cnt1++;
			buf1 = LPC_CANAF_RAM->mask[cnt1];
			LPC_CANAF_RAM->mask[cnt1] = buf0;
			buf0 = buf1;
		}
	}
	if (CANAF_FullCAN_cnt == 0)
	{
		/* For entering first ID */
		LPC_CANAF_RAM->mask[0] = 0x0000FFFF | (id << 16);
	}
	else if (CANAF_FullCAN_cnt == 1)
	{
		/* For entering second ID */
		if (((LPC_CANAF_RAM->mask[0] >> 16)& 0xE7FF) > id)
		{
			LPC_CANAF_RAM->mask[0] = (LPC_CANAF_RAM->mask[0] >> 16) | (id << 16);
		}
		else
		{
			LPC_CANAF_RAM->mask[0] = (LPC_CANAF_RAM->mask[0] & 0xFFFF0000) | id;
		}
	}
	else
	{
		/* Find where to insert new ID */
		cnt1 = 0;
		cnt2 = CANAF_FullCAN_cnt;
		bound1 = (CANAF_FullCAN_cnt - 1) >> 1;
		while (cnt1 <= bound1)
		{
			/* Loop through standard existing IDs */
			if (((LPC_CANAF_RAM->mask[cnt1] >> 16) & 0xE7FF) > (id & 0xE7FF))
			{
				cnt2 = cnt1 * 2;
				break;
			}

			if ((LPC_CANAF_RAM->mask[cnt1] & 0x0000E7FF) > (id & 0xE7FF))
			{
				cnt2 = cnt1 * 2 + 1;
				break;
			}

			cnt1++;
		}
		/* cnt1 = U32 where to insert new ID */
		/* cnt2 = U16 where to insert new ID */

		if (cnt1 > bound1)
		{
			/* Adding ID as last entry */
			/* Even number of IDs exists */
			if ((CANAF_FullCAN_cnt & 0x0001) == 0)
			{
				LPC_CANAF_RAM->mask[cnt1]  = 0x0000FFFF | (id << 16);
			}
			/* Odd  number of IDs exists */
			else
			{
				LPC_CANAF_RAM->mask[cnt1]  = (LPC_CANAF_RAM->mask[cnt1] & 0xFFFF0000) | id;
			}
		}
		else
		{
			buf0 = LPC_CANAF_RAM->mask[cnt1]; /* Remember current entry */
			if ((cnt2 & 0x0001) == 0)
			{
				/* Insert new mask to even address*/
				buf1 = (id << 16) | (buf0 >> 16);
			}
			else
			{
				/* Insert new mask to odd  address */
				buf1 = (buf0 & 0xFFFF0000) | id;
			}
			LPC_CANAF_RAM->mask[cnt1] = buf1;/* Insert mask */
			bound1 = CANAF_FullCAN_cnt >> 1;
			/* Move all remaining standard mask entries one place up */
			while (cnt1 < bound1)
			{
				cnt1++;
				buf1  = LPC_CANAF_RAM->mask[cnt1];
				LPC_CANAF_RAM->mask[cnt1] = (buf1 >> 16) | (buf0 << 16);
				buf0  = buf1;
			}

			if ((CANAF_FullCAN_cnt & 0x0001) == 0)
			{
				/* Even number of IDs exists */
				LPC_CANAF_RAM->mask[cnt1] = (LPC_CANAF_RAM->mask[cnt1] & 0xFFFF0000)
											| (0x0000FFFF);
			}
		}
	}
	//restruct FulCAN Object Section
	bound1 = CANAF_FullCAN_cnt - cnt2;
	cnt1 = total - (CANAF_FullCAN_cnt)*3 + cnt2*3 + 1;
	buf0 = LPC_CANAF_RAM->mask[cnt1];
	buf1 = LPC_CANAF_RAM->mask[cnt1+1];
	buf2 = LPC_CANAF_RAM->mask[cnt1+2];
	LPC_CANAF_RAM->mask[cnt1]=LPC_CANAF_RAM->mask[cnt1+1]= LPC_CANAF_RAM->mask[cnt1+2]=0x00;
	cnt1+=3;
	while(bound1--)
	{
		tmp0 = LPC_CANAF_RAM->mask[cnt1];
		tmp1 = LPC_CANAF_RAM->mask[cnt1+1];
		tmp2 = LPC_CANAF_RAM->mask[cnt1+2];
		LPC_CANAF_RAM->mask[cnt1]= buf0;
		LPC_CANAF_RAM->mask[cnt1+1]= buf1;
		LPC_CANAF_RAM->mask[cnt1+2]= buf2;
		buf0 = tmp0;
		buf1 = tmp1;
		buf2 = tmp2;
		cnt1+=3;
	}
	CANAF_FullCAN_cnt++;
	//update address values
	LPC_CANAF->SFF_sa 	  +=0x04;
	LPC_CANAF->SFF_GRP_sa +=0x04 ;
	LPC_CANAF->EFF_sa     +=0x04 ;
	LPC_CANAF->EFF_GRP_sa +=0x04;
	LPC_CANAF->ENDofTable +=0x04;

	LPC_CANAF->AFMR = 0x04;
 	return CAN_OK;
}

/********************************************************************//**
 * @brief		Load Group entry into AFLUT
 * @param[in]	CANx: CAN peripheral selected, should be:
 * 				- LPC_CAN1: CAN1 peripheral
 * 				- LPC_CAN2: CAN2 peripheral
 * @param[in]	lowerID, upperID: lower and upper identifier of entry
 * @param[in]	format: type of ID format, should be:
 * 				- STD_ID_FORMAT: Standard ID format (11-bit value)
 * 				- EXT_ID_FORMAT: Extended ID format (29-bit value)
 * @return 		CAN_ERROR, could be:
 * 				- CAN_OK: loading is successful
 * 				- CAN_CONFLICT_ID_ERROR: Conflict ID occurs
 * 				- CAN_OBJECTS_FULL_ERROR: no more space available
 *********************************************************************/
CAN_ERROR CAN_LoadGroupEntry(LPC_CAN_TypeDef* CANx, uint32_t lowerID, \
		uint32_t upperID, CAN_ID_FORMAT_Type format)
{
	uint16_t tmp = 0;
	uint32_t buf0, buf1, entry1, entry2, LID,UID;
	int16_t cnt1, bound1, total;
	//LPC_CANAF_RAM_TypeDef *AFLUTTest = LPC_CANAF_RAM;

	CHECK_PARAM(PARAM_CANx(CANx));
	CHECK_PARAM(PARAM_ID_FORMAT(format));

	if(lowerID > upperID) return CAN_CONFLICT_ID_ERROR;
	if(CANx == LPC_CAN1)
	{
		tmp = 0;
	}
	else
	{
		tmp = 1;
	}

	total =((CANAF_FullCAN_cnt+1)>>1)+ CANAF_FullCAN_cnt*3 +((CANAF_std_cnt + 1) >> 1)+  \
			CANAF_gstd_cnt + CANAF_ext_cnt + (CANAF_gext_cnt<<1);

	/* Setup Acceptance Filter Configuration
	Acceptance Filter Mode Register = Off */
	LPC_CANAF->AFMR = 0x00000001;

/*********Add Group of Standard Identifier Frame Format************/
	if(format == STD_ID_FORMAT)
	{
		if ((total >= 512)){//don't have enough space
			return CAN_OBJECTS_FULL_ERROR;
		}
		lowerID &=0x7FF; //mask ID
		upperID &=0x7FF;
		entry1  = (tmp << 29)|(lowerID << 16)|(tmp << 13)|(upperID << 0);
		cnt1 = ((CANAF_FullCAN_cnt+1)>>1) + ((CANAF_std_cnt + 1) >> 1);

		//if this is the first Group standard ID entry
		if(CANAF_gstd_cnt == 0)
		{
			LPC_CANAF_RAM->mask[cnt1] = entry1;
		}
		else
		{
			//find the position to add new Group entry
			bound1 = ((CANAF_FullCAN_cnt+1)>>1) + ((CANAF_std_cnt + 1) >> 1) + CANAF_gstd_cnt;
			while(cnt1 < bound1)
			{
				//compare controller first
				while((LPC_CANAF_RAM->mask[cnt1] >> 29)< (entry1 >> 29))//increase until meet greater or equal controller
					cnt1++;
				buf0 = LPC_CANAF_RAM->mask[cnt1];
				if((LPC_CANAF_RAM->mask[cnt1] >> 29)> (entry1 >> 29)) //meet greater controller
				{
					//add at this position
					LPC_CANAF_RAM->mask[cnt1] = entry1;
					break;
				}
				else //meet equal controller
				{
					LID  = (buf0 >> 16)&0x7FF;
					UID  = buf0 & 0x7FF;
					if (upperID <= LID)
					{
						//add new entry before this entry
						LPC_CANAF_RAM->mask[cnt1] = entry1;
						break;
					}
					else if (lowerID >= UID)
					{
						//load next entry to compare
						cnt1 ++;
					}
					else
						return CAN_CONFLICT_ID_ERROR;
				}
			}
			if(cnt1 >= bound1)
			{
				//add new entry at the last position in this list
				buf0 = LPC_CANAF_RAM->mask[cnt1];
				LPC_CANAF_RAM->mask[cnt1] = entry1;
			}

			//remove all remaining entry of this section one place up
			bound1 = total - cnt1;
			while(bound1--)
			{
				cnt1++;
				buf1 = LPC_CANAF_RAM->mask[cnt1];
				LPC_CANAF_RAM->mask[cnt1] = buf0;
				buf0 = buf1;
			}
		}
		CANAF_gstd_cnt++;
		//update address values
		LPC_CANAF->EFF_sa     +=0x04 ;
		LPC_CANAF->EFF_GRP_sa +=0x04;
		LPC_CANAF->ENDofTable +=0x04;
	}


/*********Add Group of Extended Identifier Frame Format************/
	else
	{
		if ((total >= 511)){//don't have enough space
			return CAN_OBJECTS_FULL_ERROR;
		}
		lowerID  &= 0x1FFFFFFF; //mask ID
		upperID &= 0x1FFFFFFF;
		entry1   = (tmp << 29)|(lowerID << 0);
		entry2   = (tmp << 29)|(upperID << 0);

		cnt1 = ((CANAF_FullCAN_cnt+1)>>1) + ((CANAF_std_cnt + 1) >> 1) + CANAF_gstd_cnt + CANAF_ext_cnt;
		//if this is the first Group standard ID entry
		if(CANAF_gext_cnt == 0)
		{
			LPC_CANAF_RAM->mask[cnt1] = entry1;
			LPC_CANAF_RAM->mask[cnt1+1] = entry2;
		}
		else
		{
			//find the position to add new Group entry
			bound1 = ((CANAF_FullCAN_cnt+1)>>1) + ((CANAF_std_cnt + 1) >> 1) + CANAF_gstd_cnt \
						+ CANAF_ext_cnt + (CANAF_gext_cnt<<1);
			while(cnt1 < bound1)
			{
				while((LPC_CANAF_RAM->mask[cnt1] >>29)< tmp) //increase until meet greater or equal controller
					cnt1++;
				buf0 = LPC_CANAF_RAM->mask[cnt1];
				buf1 = LPC_CANAF_RAM->mask[cnt1+1];
				if((LPC_CANAF_RAM->mask[cnt1] >> 29)> (entry1 >> 29)) //meet greater controller
				{
					//add at this position
					LPC_CANAF_RAM->mask[cnt1] = entry1;
					LPC_CANAF_RAM->mask[++cnt1] = entry2;
					break;
				}
				else //meet equal controller
				{
					LID  = buf0 & 0x1FFFFFFF; //mask ID
					UID  = buf1 & 0x1FFFFFFF;
					if (upperID <= LID)
					{
						//add new entry before this entry
						LPC_CANAF_RAM->mask[cnt1] = entry1;
						LPC_CANAF_RAM->mask[++cnt1] = entry2;
						break;
					}
					else if (lowerID >= UID)
					{
						//load next entry to compare
						cnt1 +=2;
					}
					else
						return CAN_CONFLICT_ID_ERROR;
				}
			}
			if(cnt1 >= bound1)
			{
				//add new entry at the last position in this list
				buf0 = LPC_CANAF_RAM->mask[cnt1];
				buf1 = LPC_CANAF_RAM->mask[cnt1+1];
				LPC_CANAF_RAM->mask[cnt1]   = entry1;
				LPC_CANAF_RAM->mask[++cnt1] = entry2;
			}
			//remove all remaining entry of this section two place up
			bound1 = total - cnt1 + 1;
			cnt1++;
			while(bound1>0)
			{
				entry1 = LPC_CANAF_RAM->mask[cnt1];
				entry2 = LPC_CANAF_RAM->mask[cnt1+1];
				LPC_CANAF_RAM->mask[cnt1]   = buf0;
				LPC_CANAF_RAM->mask[cnt1+1] = buf1;
				buf0 = entry1;
				buf1 = entry2;
				cnt1   +=2;
				bound1 -=2;
			}
		}
		CANAF_gext_cnt++;
		//update address values
		LPC_CANAF->ENDofTable +=0x08;
	}
	LPC_CANAF->AFMR = 0x04;
 	return CAN_OK;
}

/********************************************************************//**
 * @brief		Remove AFLUT entry (FullCAN entry and Explicit Standard entry)
 * @param[in]	EntryType: the type of entry that want to remove, should be:
 * 				- FULLCAN_ENTRY
 * 				- EXPLICIT_STANDARD_ENTRY
 * 				- GROUP_STANDARD_ENTRY
 * 				- EXPLICIT_EXTEND_ENTRY
 * 				- GROUP_EXTEND_ENTRY
 * @param[in]	position: the position of this entry in its section
 * Note: the first position is 0
 * @return 		CAN_ERROR, could be:
 * 				- CAN_OK: removing is successful
 * 				- CAN_ENTRY_NOT_EXIT_ERROR: entry want to remove is not exit
 *********************************************************************/
CAN_ERROR CAN_RemoveEntry(AFLUT_ENTRY_Type EntryType, uint16_t position)
{
	uint16_t cnt, bound, total;
	uint32_t buf0, buf1;
	CHECK_PARAM(PARAM_AFLUT_ENTRY_TYPE(EntryType));
	CHECK_PARAM(PARAM_POSITION(position));

	/* Setup Acceptance Filter Configuration
	Acceptance Filter Mode Register = Off */
	LPC_CANAF->AFMR = 0x00000001;
	total = ((CANAF_FullCAN_cnt+1)>>1)+((CANAF_std_cnt + 1) >> 1) + \
			CANAF_gstd_cnt + CANAF_ext_cnt + (CANAF_gext_cnt<<1);


/************** Remove FullCAN Entry *************/
	if(EntryType == FULLCAN_ENTRY)
	{
		if((CANAF_FullCAN_cnt==0)||(position >= CANAF_FullCAN_cnt))
		{
			return CAN_ENTRY_NOT_EXIT_ERROR;
		}
		else
		{
			cnt = position >> 1;
			buf0 = LPC_CANAF_RAM->mask[cnt];
			bound = (CANAF_FullCAN_cnt - position -1)>>1;
			if((position & 0x0001) == 0) //event position
			{
				while(bound--)
				{
					//remove all remaining FullCAN entry one place down
					buf1  = LPC_CANAF_RAM->mask[cnt+1];
					LPC_CANAF_RAM->mask[cnt] = (buf1 >> 16) | (buf0 << 16);
					buf0  = buf1;
					cnt++;
				}
			}
			else //odd position
			{
				while(bound--)
				{
					//remove all remaining FullCAN entry one place down
					buf1  = LPC_CANAF_RAM->mask[cnt+1];
					LPC_CANAF_RAM->mask[cnt] = (buf0 & 0xFFFF0000)|(buf1 >> 16);
					LPC_CANAF_RAM->mask[cnt+1] = LPC_CANAF_RAM->mask[cnt+1] << 16;
					buf0  = buf1<<16;
					cnt++;
				}
			}
			if((CANAF_FullCAN_cnt & 0x0001) == 0)
			{
				if((position & 0x0001)==0)
					LPC_CANAF_RAM->mask[cnt] = (buf0 << 16) | (0x0000FFFF);
				else
					LPC_CANAF_RAM->mask[cnt] = buf0 | 0x0000FFFF;
			}
			else
			{
				//remove all remaining section one place down
				cnt = (CANAF_FullCAN_cnt + 1)>>1;
				bound = total + CANAF_FullCAN_cnt * 3;
				while(bound>cnt)
				{
					LPC_CANAF_RAM->mask[cnt-1] = LPC_CANAF_RAM->mask[cnt];
					cnt++;
				}
				LPC_CANAF_RAM->mask[cnt-1]=0x00;
				//update address values
				LPC_CANAF->SFF_sa 	  -=0x04;
				LPC_CANAF->SFF_GRP_sa -=0x04 ;
				LPC_CANAF->EFF_sa     -=0x04 ;
				LPC_CANAF->EFF_GRP_sa -=0x04;
				LPC_CANAF->ENDofTable -=0x04;
			}
			CANAF_FullCAN_cnt--;

			//delete its FullCAN Object in the FullCAN Object section
			//remove all remaining FullCAN Object three place down
			cnt = total + position * 3;
			bound = (CANAF_FullCAN_cnt - position + 1) * 3;

			while(bound)
			{
				LPC_CANAF_RAM->mask[cnt]=LPC_CANAF_RAM->mask[cnt+3];;
				LPC_CANAF_RAM->mask[cnt+1]=LPC_CANAF_RAM->mask[cnt+4];
				LPC_CANAF_RAM->mask[cnt+2]=LPC_CANAF_RAM->mask[cnt+5];
				bound -=3;
				cnt   +=3;
			}
		}
	}

/************** Remove Explicit Standard ID Entry *************/
	else if(EntryType == EXPLICIT_STANDARD_ENTRY)
	{
		if((CANAF_std_cnt==0)||(position >= CANAF_std_cnt))
		{
			return CAN_ENTRY_NOT_EXIT_ERROR;
		}
		else
		{
			cnt = ((CANAF_FullCAN_cnt+1)>>1)+ (position >> 1);
			buf0 = LPC_CANAF_RAM->mask[cnt];
			bound = (CANAF_std_cnt - position - 1)>>1;
			if((position & 0x0001) == 0) //event position
			{
				while(bound--)
				{
					//remove all remaining FullCAN entry one place down
					buf1  = LPC_CANAF_RAM->mask[cnt+1];
					LPC_CANAF_RAM->mask[cnt] = (buf1 >> 16) | (buf0 << 16);
					buf0  = buf1;
					cnt++;
				}
			}
			else //odd position
			{
				while(bound--)
				{
					//remove all remaining FullCAN entry one place down
					buf1  = LPC_CANAF_RAM->mask[cnt+1];
					LPC_CANAF_RAM->mask[cnt] = (buf0 & 0xFFFF0000)|(buf1 >> 16);
					LPC_CANAF_RAM->mask[cnt+1] = LPC_CANAF_RAM->mask[cnt+1] << 16;
					buf0  = buf1<<16;
					cnt++;
				}
			}
			if((CANAF_std_cnt & 0x0001) == 0)
			{
				if((position & 0x0001)==0)
					LPC_CANAF_RAM->mask[cnt] = (buf0 << 16) | (0x0000FFFF);
				else
					LPC_CANAF_RAM->mask[cnt] = buf0 | 0x0000FFFF;
			}
			else
			{
				//remove all remaining section one place down
				cnt = ((CANAF_FullCAN_cnt + 1)>>1) + ((CANAF_std_cnt + 1) >> 1);
				bound = total + CANAF_FullCAN_cnt * 3;
				while(bound>cnt)
				{
					LPC_CANAF_RAM->mask[cnt-1] = LPC_CANAF_RAM->mask[cnt];
					cnt++;
				}
				LPC_CANAF_RAM->mask[cnt-1]=0x00;
				//update address value
				LPC_CANAF->SFF_GRP_sa -=0x04 ;
				LPC_CANAF->EFF_sa     -=0x04 ;
				LPC_CANAF->EFF_GRP_sa -=0x04;
				LPC_CANAF->ENDofTable -=0x04;
			}
			CANAF_std_cnt--;
		}
	}

/************** Remove Group of Standard ID Entry *************/
	else if(EntryType == GROUP_STANDARD_ENTRY)
	{
		if((CANAF_gstd_cnt==0)||(position >= CANAF_gstd_cnt))
		{
			return CAN_ENTRY_NOT_EXIT_ERROR;
		}
		else
		{
			cnt = ((CANAF_FullCAN_cnt + 1)>>1) + ((CANAF_std_cnt + 1) >> 1)+ position + 1;
			bound = total + CANAF_FullCAN_cnt * 3;
			while (cnt<bound)
			{
				LPC_CANAF_RAM->mask[cnt-1] = LPC_CANAF_RAM->mask[cnt];
				cnt++;
			}
			LPC_CANAF_RAM->mask[cnt-1]=0x00;
		}
		CANAF_gstd_cnt--;
		//update address value
		LPC_CANAF->EFF_sa     -=0x04;
		LPC_CANAF->EFF_GRP_sa -=0x04;
		LPC_CANAF->ENDofTable -=0x04;
	}

/************** Remove Explicit Extended ID Entry *************/
	else if(EntryType == EXPLICIT_EXTEND_ENTRY)
	{
		if((CANAF_ext_cnt==0)||(position >= CANAF_ext_cnt))
		{
			return CAN_ENTRY_NOT_EXIT_ERROR;
		}
		else
		{
			cnt = ((CANAF_FullCAN_cnt + 1)>>1) + ((CANAF_std_cnt + 1) >> 1)+ CANAF_gstd_cnt + position + 1;
			bound = total + CANAF_FullCAN_cnt * 3;
			while (cnt<bound)
			{
				LPC_CANAF_RAM->mask[cnt-1] = LPC_CANAF_RAM->mask[cnt];
				cnt++;
			}
			LPC_CANAF_RAM->mask[cnt-1]=0x00;
		}
		CANAF_ext_cnt--;
		LPC_CANAF->EFF_GRP_sa -=0x04;
		LPC_CANAF->ENDofTable -=0x04;
	}

/************** Remove Group of Extended ID Entry *************/
	else
	{
		if((CANAF_gext_cnt==0)||(position >= CANAF_gext_cnt))
		{
			return CAN_ENTRY_NOT_EXIT_ERROR;
		}
		else
		{
			cnt = total - (CANAF_gext_cnt<<1) + (position<<1);
			bound = total + CANAF_FullCAN_cnt * 3;
			while (cnt<bound)
			{
				//remove all remaining entry two place up
				LPC_CANAF_RAM->mask[cnt] = LPC_CANAF_RAM->mask[cnt+2];
				LPC_CANAF_RAM->mask[cnt+1] = LPC_CANAF_RAM->mask[cnt+3];
				cnt+=2;
			}
		}
		CANAF_gext_cnt--;
		LPC_CANAF->ENDofTable -=0x08;
	}
	LPC_CANAF->AFMR = 0x04;
	return CAN_OK;
}

/********************************************************************//**
 * @brief		Send message data
 * @param[in]	CANx pointer to LPC_CAN_TypeDef, should be:
 * 				- LPC_CAN1: CAN1 peripheral
 * 				- LPC_CAN2: CAN2 peripheral
 * @param[in]	CAN_Msg point to the CAN_MSG_Type Structure, it contains message
 * 				information such as: ID, DLC, RTR, ID Format
 * @return 		Status:
 * 				- SUCCESS: send message successfully
 * 				- ERROR: send message unsuccessfully
 *********************************************************************/
Status CAN_SendMsg (LPC_CAN_TypeDef *CANx, CAN_MSG_Type *CAN_Msg)
{
	uint32_t data;
	CHECK_PARAM(PARAM_CANx(CANx));
	CHECK_PARAM(PARAM_ID_FORMAT(CAN_Msg->format));
	if(CAN_Msg->format==STD_ID_FORMAT)
	{
		CHECK_PARAM(PARAM_ID_11(CAN_Msg->id));
	}
	else
	{
		CHECK_PARAM(PARAM_ID_29(CAN_Msg->id));
	}
	CHECK_PARAM(PARAM_DLC(CAN_Msg->len));
	CHECK_PARAM(PARAM_FRAME_TYPE(CAN_Msg->type));

	//Check status of Transmit Buffer 1
	if (CANx->SR & (1<<2))
	{
		/* Transmit Channel 1 is available */
		/* Write frame informations and frame data into its CANxTFI1,
		 * CANxTID1, CANxTDA1, CANxTDB1 register */
		CANx->TFI1 &= ~0x000F0000;
		CANx->TFI1 |= (CAN_Msg->len)<<16;
		if(CAN_Msg->type == REMOTE_FRAME)
		{
			CANx->TFI1 |= (1<<30); //set bit RTR
		}
		else
		{
			CANx->TFI1 &= ~(1<<30);
		}
		if(CAN_Msg->format == EXT_ID_FORMAT)
		{
			CANx->TFI1 |= (0x80000000); //set bit FF
		}
		else
		{
			CANx->TFI1 &= ~(0x80000000);
		}

		/* Write CAN ID*/
		CANx->TID1 = CAN_Msg->id;

		/*Write first 4 data bytes*/
		data = (CAN_Msg->dataA[0])|(((CAN_Msg->dataA[1]))<<8)|((CAN_Msg->dataA[2])<<16)|((CAN_Msg->dataA[3])<<24);
		CANx->TDA1 = data;

		/*Write second 4 data bytes*/
		data = (CAN_Msg->dataB[0])|(((CAN_Msg->dataB[1]))<<8)|((CAN_Msg->dataB[2])<<16)|((CAN_Msg->dataB[3])<<24);
		CANx->TDB1 = data;

		 /*Write transmission request*/
		 CANx->CMR = 0x21;
		 return SUCCESS;
	}
	//check status of Transmit Buffer 2
	else if(CANx->SR & (1<<10))
	{
		/* Transmit Channel 2 is available */
		/* Write frame informations and frame data into its CANxTFI2,
		 * CANxTID2, CANxTDA2, CANxTDB2 register */
		CANx->TFI2 &= ~0x000F0000;
		CANx->TFI2 |= (CAN_Msg->len)<<16;
		if(CAN_Msg->type == REMOTE_FRAME)
		{
			CANx->TFI2 |= (1<<30); //set bit RTR
		}
		else
		{
			CANx->TFI2 &= ~(1<<30);
		}
		if(CAN_Msg->format == EXT_ID_FORMAT)
		{
			CANx->TFI2 |= (0x80000000); //set bit FF
		}
		else
		{
			CANx->TFI2 &= ~(0x80000000);
		}

		/* Write CAN ID*/
		CANx->TID2 = CAN_Msg->id;

		/*Write first 4 data bytes*/
		data = (CAN_Msg->dataA[0])|(((CAN_Msg->dataA[1]))<<8)|((CAN_Msg->dataA[2])<<16)|((CAN_Msg->dataA[3])<<24);
		CANx->TDA2 = data;

		/*Write second 4 data bytes*/
		data = (CAN_Msg->dataB[0])|(((CAN_Msg->dataB[1]))<<8)|((CAN_Msg->dataB[2])<<16)|((CAN_Msg->dataB[3])<<24);
		CANx->TDB2 = data;

		/*Write transmission request*/
		CANx->CMR = 0x41;
		return SUCCESS;
	}
	//check status of Transmit Buffer 3
	else if (CANx->SR & (1<<18))
	{
		/* Transmit Channel 3 is available */
		/* Write frame informations and frame data into its CANxTFI3,
		 * CANxTID3, CANxTDA3, CANxTDB3 register */
		CANx->TFI3 &= ~0x000F0000;
		CANx->TFI3 |= (CAN_Msg->len)<<16;
		if(CAN_Msg->type == REMOTE_FRAME)
		{
			CANx->TFI3 |= (1<<30); //set bit RTR
		}
		else
		{
			CANx->TFI3 &= ~(1<<30);
		}
		if(CAN_Msg->format == EXT_ID_FORMAT)
		{
			CANx->TFI3 |= (0x80000000); //set bit FF
		}
		else
		{
			CANx->TFI3 &= ~(0x80000000);
		}

		/* Write CAN ID*/
		CANx->TID3 = CAN_Msg->id;

		/*Write first 4 data bytes*/
		data = (CAN_Msg->dataA[0])|(((CAN_Msg->dataA[1]))<<8)|((CAN_Msg->dataA[2])<<16)|((CAN_Msg->dataA[3])<<24);
		CANx->TDA3 = data;

		/*Write second 4 data bytes*/
		data = (CAN_Msg->dataB[0])|(((CAN_Msg->dataB[1]))<<8)|((CAN_Msg->dataB[2])<<16)|((CAN_Msg->dataB[3])<<24);
		CANx->TDB3 = data;

		/*Write transmission request*/
		CANx->CMR = 0x81;
		return SUCCESS;
	}
	else
	{
		return ERROR;
	}
}

/********************************************************************//**
 * @brief		Receive message data
 * @param[in]	CANx pointer to LPC_CAN_TypeDef, should be:
 * 				- LPC_CAN1: CAN1 peripheral
 * 				- LPC_CAN2: CAN2 peripheral
 * @param[in]	CAN_Msg point to the CAN_MSG_Type Struct, it will contain received
 *  			message information such as: ID, DLC, RTR, ID Format
 * @return 		Status:
 * 				- SUCCESS: receive message successfully
 * 				- ERROR: receive message unsuccessfully
 *********************************************************************/
Status CAN_ReceiveMsg (LPC_CAN_TypeDef *CANx, CAN_MSG_Type *CAN_Msg)
{
	uint32_t data;

	CHECK_PARAM(PARAM_CANx(CANx));

	//check status of Receive Buffer
	if((CANx->SR &0x00000001))
	{
		/* Receive message is available */
		/* Read frame informations */
		CAN_Msg->format   = (uint8_t)(((CANx->RFS) & 0x80000000)>>31);
		CAN_Msg->type     = (uint8_t)(((CANx->RFS) & 0x40000000)>>30);
		CAN_Msg->len      = (uint8_t)(((CANx->RFS) & 0x000F0000)>>16);


		/* Read CAN message identifier */
		CAN_Msg->id = CANx->RID;

		/* Read the data if received message was DATA FRAME */
		if (CAN_Msg->type == DATA_FRAME)
		{
			/* Read first 4 data bytes */
			data = CANx->RDA;
			*((uint8_t *) &CAN_Msg->dataA[0])= data & 0x000000FF;
			*((uint8_t *) &CAN_Msg->dataA[1])= (data & 0x0000FF00)>>8;;
			*((uint8_t *) &CAN_Msg->dataA[2])= (data & 0x00FF0000)>>16;
			*((uint8_t *) &CAN_Msg->dataA[3])= (data & 0xFF000000)>>24;

			/* Read second 4 data bytes */
			data = CANx->RDB;
			*((uint8_t *) &CAN_Msg->dataB[0])= data & 0x000000FF;
			*((uint8_t *) &CAN_Msg->dataB[1])= (data & 0x0000FF00)>>8;
			*((uint8_t *) &CAN_Msg->dataB[2])= (data & 0x00FF0000)>>16;
			*((uint8_t *) &CAN_Msg->dataB[3])= (data & 0xFF000000)>>24;

		/*release receive buffer*/
		CANx->CMR = 0x04;
		}
		else
		{
			/* Received Frame is a Remote Frame, not have data, we just receive
			 * message information only */
			CANx->CMR = 0x04; /*release receive buffer*/
			return SUCCESS;
		}
	}
	else
	{
		// no receive message available
		return ERROR;
	}
	return SUCCESS;
}

/********************************************************************//**
 * @brief		Receive FullCAN Object
 * @param[in]	CANAFx: CAN Acceptance Filter register, should be: LPC_CANAF
 * @param[in]	CAN_Msg point to the CAN_MSG_Type Struct, it will contain received
 *  			message information such as: ID, DLC, RTR, ID Format
 * @return 		CAN_ERROR, could be:
 * 				- CAN_FULL_OBJ_NOT_RCV: FullCAN Object is not be received
 * 				- CAN_OK: Received FullCAN Object successful
 *
 *********************************************************************/
CAN_ERROR FCAN_ReadObj (LPC_CANAF_TypeDef* CANAFx, CAN_MSG_Type *CAN_Msg)
{
	uint32_t *pSrc, data;
	uint32_t interrut_word, msg_idx, test_bit, head_idx, tail_idx;

	CHECK_PARAM(PARAM_CANAFx(CANAFx));

	interrut_word = 0;

	if (LPC_CANAF->FCANIC0 != 0)
	{
		interrut_word = LPC_CANAF->FCANIC0;
		head_idx = 0;
		tail_idx = 31;
	}
	else if (LPC_CANAF->FCANIC1 != 0)
	{
		interrut_word = LPC_CANAF->FCANIC1;
		head_idx = 32;
		tail_idx = 63;
	}

	if (interrut_word != 0)
	{
		/* Detect for interrupt pending */
		msg_idx = 0;
		for (msg_idx = head_idx; msg_idx <= tail_idx; msg_idx++)
		{
			test_bit = interrut_word & 0x1;
			interrut_word = interrut_word >> 1;

			if (test_bit)
			{
				pSrc = (uint32_t *) (LPC_CANAF->ENDofTable + LPC_CANAF_RAM_BASE + msg_idx * 12);

	    	 	/* Has been finished updating the content */
	    	 	if ((*pSrc & 0x03000000L) == 0x03000000L)
	    	 	{
	    	 		/*clear semaphore*/
	    	 		*pSrc &= 0xFCFFFFFF;

	    	 		/*Set to DatA*/
	    	 		pSrc++;
	    	 		/* Copy to dest buf */
	    	 		data = *pSrc;
	    			*((uint8_t *) &CAN_Msg->dataA[0])= data & 0x000000FF;
	    			*((uint8_t *) &CAN_Msg->dataA[1])= (data & 0x0000FF00)>>8;
	    			*((uint8_t *) &CAN_Msg->dataA[2])= (data & 0x00FF0000)>>16;
	    			*((uint8_t *) &CAN_Msg->dataA[3])= (data & 0xFF000000)>>24;

	    	 		/*Set to DatB*/
	    	 		pSrc++;
	    	 		/* Copy to dest buf */
	    	 		data = *pSrc;
	    			*((uint8_t *) &CAN_Msg->dataB[0])= data & 0x000000FF;
	    			*((uint8_t *) &CAN_Msg->dataB[1])= (data & 0x0000FF00)>>8;
	    			*((uint8_t *) &CAN_Msg->dataB[2])= (data & 0x00FF0000)>>16;
	    			*((uint8_t *) &CAN_Msg->dataB[3])= (data & 0xFF000000)>>24;
	    	 		/*Back to Dat1*/
	    	 		pSrc -= 2;

	    	 		CAN_Msg->id = *pSrc & 0x7FF;
	    	 		CAN_Msg->len = (uint8_t) (*pSrc >> 16) & 0x0F;
					CAN_Msg->format = 0; //FullCAN Object ID always is 11-bit value
					CAN_Msg->type = (uint8_t)(*pSrc >> 30) &0x01;
	    	 		/*Re-read semaphore*/
	    	 		if ((*pSrc & 0x03000000L) == 0)
	    	 		{
	    	 			return CAN_OK;
	    	 		}
	    	 	}
			}
		}
	}
	return CAN_FULL_OBJ_NOT_RCV;
}
/********************************************************************//**
 * @brief		Get CAN Control Status
 * @param[in]	CANx pointer to LPC_CAN_TypeDef, should be:
 * 				- LPC_CAN1: CAN1 peripheral
 * 				- LPC_CAN2: CAN2 peripheral
 * @param[in]	arg: type of CAN status to get from CAN status register
 * 				Should be:
 * 				- CANCTRL_GLOBAL_STS: CAN Global Status
 * 				- CANCTRL_INT_CAP: CAN Interrupt and Capture
 * 				- CANCTRL_ERR_WRN: CAN Error Warning Limit
 * 				- CANCTRL_STS: CAN Control Status
 * @return 		Current Control Status that you want to get value
 *********************************************************************/
uint32_t CAN_GetCTRLStatus (LPC_CAN_TypeDef* CANx, CAN_CTRL_STS_Type arg)
{
	CHECK_PARAM(PARAM_CANx(CANx));
	CHECK_PARAM(PARAM_CTRL_STS_TYPE(arg));

	switch (arg)
	{
	case CANCTRL_GLOBAL_STS:
		return CANx->GSR;

	case CANCTRL_INT_CAP:
		return CANx->ICR;

	case CANCTRL_ERR_WRN:
		return CANx->EWL;

	default: // CANCTRL_STS
		return CANx->SR;
	}
}
/********************************************************************//**
 * @brief		Get CAN Central Status
 * @param[in]	CANCRx point to LPC_CANCR_TypeDef, should be: LPC_CANCR
 * @param[in]	arg: type of CAN status to get from CAN Central status register
 * 				Should be:
 * 				- CANCR_TX_STS: Central CAN Tx Status
 * 				- CANCR_RX_STS: Central CAN Rx Status
 * 				- CANCR_MS: Central CAN Miscellaneous Status
 * @return 		Current Central Status that you want to get value
 *********************************************************************/
uint32_t CAN_GetCRStatus (LPC_CANCR_TypeDef* CANCRx, CAN_CR_STS_Type arg)
{
	CHECK_PARAM(PARAM_CANCRx(CANCRx));
	CHECK_PARAM(PARAM_CR_STS_TYPE(arg));

	switch (arg)
	{
	case CANCR_TX_STS:
		return CANCRx->CANTxSR;

	case CANCR_RX_STS:
		return CANCRx->CANRxSR;

	default:	// CANCR_MS
		return CANCRx->CANMSR;
	}
}
/********************************************************************//**
 * @brief		Enable/Disable CAN Interrupt
 * @param[in]	CANx pointer to LPC_CAN_TypeDef, should be:
 * 				- LPC_CAN1: CAN1 peripheral
 * 				- LPC_CAN2: CAN2 peripheral
 * @param[in]	arg: type of CAN interrupt that you want to enable/disable
 * 				Should be:
 * 				- CANINT_RIE: CAN Receiver Interrupt Enable
 * 				- CANINT_TIE1: CAN Transmit Interrupt Enable
 * 				- CANINT_EIE: CAN Error Warning Interrupt Enable
 * 				- CANINT_DOIE: CAN Data Overrun Interrupt Enable
 * 				- CANINT_WUIE: CAN Wake-Up Interrupt Enable
 * 				- CANINT_EPIE: CAN Error Passive Interrupt Enable
 * 				- CANINT_ALIE: CAN Arbitration Lost Interrupt Enable
 * 				- CANINT_BEIE: CAN Bus Error Interrupt Enable
 * 				- CANINT_IDIE: CAN ID Ready Interrupt Enable
 * 				- CANINT_TIE2: CAN Transmit Interrupt Enable for Buffer2
 * 				- CANINT_TIE3: CAN Transmit Interrupt Enable for Buffer3
 * 				- CANINT_FCE: FullCAN Interrupt Enable
 * @param[in]	NewState: New state of this function, should be:
 * 				- ENABLE
 * 				- DISABLE
 * @return 		none
 *********************************************************************/
void CAN_IRQCmd (LPC_CAN_TypeDef* CANx, CAN_INT_EN_Type arg, FunctionalState NewState)
{
	CHECK_PARAM(PARAM_CANx(CANx));
	CHECK_PARAM(PARAM_INT_EN_TYPE(arg));
	CHECK_PARAM(PARAM_FUNCTIONALSTATE(NewState));

	if(NewState == ENABLE)
	{
		if(arg==CANINT_FCE)
		{
			LPC_CANAF->AFMR = 0x01;
			LPC_CANAF->FCANIE = 0x01;
			LPC_CANAF->AFMR = 0x04;
		}
		else
			CANx->IER |= (1 << arg);
	}
	else
	{
		if(arg==CANINT_FCE){
			LPC_CANAF->AFMR = 0x01;
			LPC_CANAF->FCANIE = 0x01;
			LPC_CANAF->AFMR = 0x00;
		}
		else
			CANx->IER &= ~(1 << arg);
	}
}

/********************************************************************//**
 * @brief		Setting Acceptance Filter mode
 * @param[in]	CANAFx point to LPC_CANAF_TypeDef object, should be: LPC_CANAF
 * @param[in]	AFMode: type of AF mode that you want to set, should be:
 * 				- CAN_Normal: Normal mode
 * 				- CAN_AccOff: Acceptance Filter Off Mode
 * 				- CAN_AccBP: Acceptance Fileter Bypass Mode
 * 				- CAN_eFCAN: FullCAN Mode Enhancement
 * @return 		none
 *********************************************************************/
void CAN_SetAFMode (LPC_CANAF_TypeDef* CANAFx, CAN_AFMODE_Type AFMode)
{
	CHECK_PARAM(PARAM_CANAFx(CANAFx));
	CHECK_PARAM(PARAM_AFMODE_TYPE(AFMode));

	switch(AFMode)
	{
	case CAN_Normal:
		CANAFx->AFMR = 0x00;
		break;
	case CAN_AccOff:
		CANAFx->AFMR = 0x01;
		break;
	case CAN_AccBP:
		CANAFx->AFMR = 0x02;
		break;
	case CAN_eFCAN:
		CANAFx->AFMR = 0x04;
		break;
	}
}

/********************************************************************//**
 * @brief		Enable/Disable CAN Mode
 * @param[in]	CANx pointer to LPC_CAN_TypeDef, should be:
 * 				- LPC_CAN1: CAN1 peripheral
 * 				- LPC_CAN2: CAN2 peripheral
 * @param[in]	mode: type of CAN mode that you want to enable/disable, should be:
 * 				- CAN_OPERATING_MODE: Normal Operating Mode
 * 				- CAN_RESET_MODE: Reset Mode
 * 				- CAN_LISTENONLY_MODE: Listen Only Mode
 * 				- CAN_SELFTEST_MODE: Self Test Mode
 * 				- CAN_TXPRIORITY_MODE: Transmit Priority Mode
 * 				- CAN_SLEEP_MODE: Sleep Mode
 * 				- CAN_RXPOLARITY_MODE: Receive Polarity Mode
 * 				- CAN_TEST_MODE: Test Mode
 * @param[in]	NewState: New State of this function, should be:
 * 				- ENABLE
 * 				- DISABLE
 * @return 		none
 *********************************************************************/
void CAN_ModeConfig(LPC_CAN_TypeDef* CANx, CAN_MODE_Type mode, FunctionalState NewState)
{
	CHECK_PARAM(PARAM_CANx(CANx));
	CHECK_PARAM(PARAM_MODE_TYPE(mode));
	CHECK_PARAM(PARAM_FUNCTIONALSTATE(NewState));

	switch(mode)
	{
	case CAN_OPERATING_MODE:
		CANx->MOD = 0x00;
		break;
	case CAN_RESET_MODE:
		if(NewState == ENABLE)
			CANx->MOD |=CAN_MOD_RM;
		else
			CANx->MOD &= ~CAN_MOD_RM;
		break;
	case CAN_LISTENONLY_MODE:
		CANx->MOD |=CAN_MOD_RM;//Enter Reset mode
		if(NewState == ENABLE)
			CANx->MOD |=CAN_MOD_LOM;
		else
			CANx->MOD &=~CAN_MOD_LOM;
		CANx->MOD &=~CAN_MOD_RM;//Release Reset mode
		break;
	case CAN_SELFTEST_MODE:
		CANx->MOD |=CAN_MOD_RM;//Enter Reset mode
		if(NewState == ENABLE)
			CANx->MOD |=CAN_MOD_STM;
		else
			CANx->MOD &=~CAN_MOD_STM;
		CANx->MOD &=~CAN_MOD_RM;//Release Reset mode
		break;
	case CAN_TXPRIORITY_MODE:
		if(NewState == ENABLE)
			CANx->MOD |=CAN_MOD_TPM;
		else
			CANx->MOD &=~CAN_MOD_TPM;
		break;
	case CAN_SLEEP_MODE:
		if(NewState == ENABLE)
			CANx->MOD |=CAN_MOD_SM;
		else
			CANx->MOD &=~CAN_MOD_SM;
		break;
	case CAN_RXPOLARITY_MODE:
		if(NewState == ENABLE)
			CANx->MOD |=CAN_MOD_RPM;
		else
			CANx->MOD &=~CAN_MOD_RPM;
		break;
	case CAN_TEST_MODE:
		if(NewState == ENABLE)
			CANx->MOD |=CAN_MOD_TM;
		else
			CANx->MOD &=~CAN_MOD_TM;
		break;
	}
}
/*********************************************************************//**
 * @brief		Set CAN command request
 * @param[in]	CANx point to CAN peripheral selected, should be:
 * 				- LPC_CAN1: CAN1 peripheral
 * 				- LPC_CAN2: CAN2 peripheral
 * @param[in]	CMRType	command request type, should be:
 * 				- CAN_CMR_TR: Transmission request
 * 				- CAN_CMR_AT: Abort Transmission request
 * 				- CAN_CMR_RRB: Release Receive Buffer request
 * 				- CAN_CMR_CDO: Clear Data Overrun request
 * 				- CAN_CMR_SRR: Self Reception request
 * 				- CAN_CMR_STB1: Select Tx Buffer 1 request
 * 				- CAN_CMR_STB2: Select Tx Buffer 2 request
 * 				- CAN_CMR_STB3: Select Tx Buffer 3 request
 * @return		CANICR (CAN interrupt and Capture register) value
 **********************************************************************/
void CAN_SetCommand(LPC_CAN_TypeDef* CANx, uint32_t CMRType)
{
	CHECK_PARAM(PARAM_CANx(CANx));
	CANx->CMR |= CMRType;
}

/*********************************************************************//**
 * @brief		Get CAN interrupt status
 * @param[in]	CANx point to CAN peripheral selected, should be:
 * 				- LPC_CAN1: CAN1 peripheral
 * 				- LPC_CAN2: CAN2 peripheral
 * @return		CANICR (CAN interrupt and Capture register) value
 **********************************************************************/
uint32_t CAN_IntGetStatus(LPC_CAN_TypeDef* CANx)
{
	CHECK_PARAM(PARAM_CANx(CANx));
	return CANx->ICR;
}

/*********************************************************************//**
 * @brief		Check if FullCAN interrupt enable or not
 * @param[in]	CANAFx point to LPC_CANAF_TypeDef object, should be: LPC_CANAF
 * @return		IntStatus, could be:
 * 				- SET: if FullCAN interrupt is enable
 * 				- RESET: if FullCAN interrupt is disable
 **********************************************************************/
IntStatus CAN_FullCANIntGetStatus (LPC_CANAF_TypeDef* CANAFx)
{
	CHECK_PARAM( PARAM_CANAFx(CANAFx));
	if (CANAFx->FCANIE)
		return SET;
	return RESET;
}

/*********************************************************************//**
 * @brief		Get value of FullCAN interrupt and capture register
 * @param[in]	CANAFx point to LPC_CANAF_TypeDef object, should be: LPC_CANAF
 * @param[in]	type: FullCAN IC type, should be:
 * 				- FULLCAN_IC0: FullCAN Interrupt Capture 0
 * 				- FULLCAN_IC1: FullCAN Interrupt Capture 1
 * @return		FCANIC0 or FCANIC1 (FullCAN interrupt and Capture register) value
 **********************************************************************/
uint32_t CAN_FullCANPendGetStatus(LPC_CANAF_TypeDef* CANAFx, FullCAN_IC_Type type)
{
	CHECK_PARAM(PARAM_CANAFx(CANAFx));
	CHECK_PARAM( PARAM_FULLCAN_IC(type));
	if (type == FULLCAN_IC0)
		return CANAFx->FCANIC0;
	return CANAFx->FCANIC1;
}
/* End of Public Variables ---------------------------------------------------------- */
/**
 * @}
 */

#endif /* _CAN */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
