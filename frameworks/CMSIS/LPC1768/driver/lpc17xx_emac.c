/**********************************************************************
* $Id$		lpc17xx_emac.c				2010-05-21
*//**
* @file		lpc17xx_emac.c
* @brief	Contains all functions support for Ethernet MAC firmware
* 			library on LPC17xx
* @version	2.0
* @date		21. May. 2010
* @author	NXP MCU SW Application Team
*
* Copyright(C) 2010, NXP Semiconductor
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
/** @addtogroup EMAC
 * @{
 */

/* Includes ------------------------------------------------------------------- */
#include "lpc17xx_emac.h"
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


#ifdef _EMAC

/* Private Variables ---------------------------------------------------------- */
/** @defgroup EMAC_Private_Variables EMAC Private Variables
 * @{
 */

/* MII Mgmt Configuration register - Clock divider setting */
const uint8_t EMAC_clkdiv[] = { 4, 6, 8, 10, 14, 20, 28, 36, 40, 44, 48, 52, 56, 60, 64};

/* EMAC local DMA Descriptors */

/** Rx Descriptor data array */
static RX_Desc Rx_Desc[EMAC_NUM_RX_FRAG];

/** Rx Status data array - Must be 8-Byte aligned */
#if defined ( __CC_ARM   )
static __align(8) RX_Stat Rx_Stat[EMAC_NUM_RX_FRAG];
#elif defined ( __ICCARM__ )
#pragma data_alignment=8
static RX_Stat Rx_Stat[EMAC_NUM_RX_FRAG];
#elif defined   (  __GNUC__  )
static __attribute__ ((aligned (8))) RX_Stat Rx_Stat[EMAC_NUM_RX_FRAG];
#endif

/** Tx Descriptor data array */
static TX_Desc Tx_Desc[EMAC_NUM_TX_FRAG];
/** Tx Status data array */
static TX_Stat Tx_Stat[EMAC_NUM_TX_FRAG];

/* EMAC local DMA buffers */
/** Rx buffer data */
static uint32_t rx_buf[EMAC_NUM_RX_FRAG][EMAC_ETH_MAX_FLEN>>2];
/** Tx buffer data */
static uint32_t tx_buf[EMAC_NUM_TX_FRAG][EMAC_ETH_MAX_FLEN>>2];

/**
 * @}
 */

/* Private Functions ---------------------------------------------------------- */
static void rx_descr_init (void);
static void tx_descr_init (void);
static int32_t write_PHY (uint32_t PhyReg, uint16_t Value);
static int32_t  read_PHY (uint32_t PhyReg);

static void setEmacAddr(uint8_t abStationAddr[]);
static int32_t emac_CRCCalc(uint8_t frame_no_fcs[], int32_t frame_len);


/*--------------------------- rx_descr_init ---------------------------------*/
/*********************************************************************//**
 * @brief 		Initializes RX Descriptor
 * @param[in] 	None
 * @return 		None
 ***********************************************************************/
static void rx_descr_init (void)
{
	/* Initialize Receive Descriptor and Status array. */
	uint32_t i;

	for (i = 0; i < EMAC_NUM_RX_FRAG; i++) {
		Rx_Desc[i].Packet  = (uint32_t)&rx_buf[i];
		Rx_Desc[i].Ctrl    = EMAC_RCTRL_INT | (EMAC_ETH_MAX_FLEN - 1);
		Rx_Stat[i].Info    = 0;
		Rx_Stat[i].HashCRC = 0;
	}

	/* Set EMAC Receive Descriptor Registers. */
	LPC_EMAC->RxDescriptor       = (uint32_t)&Rx_Desc[0];
	LPC_EMAC->RxStatus           = (uint32_t)&Rx_Stat[0];
	LPC_EMAC->RxDescriptorNumber = EMAC_NUM_RX_FRAG - 1;

	/* Rx Descriptors Point to 0 */
	LPC_EMAC->RxConsumeIndex  = 0;
}


/*--------------------------- tx_descr_init ---- ----------------------------*/
/*********************************************************************//**
 * @brief 		Initializes TX Descriptor
 * @param[in] 	None
 * @return 		None
 ***********************************************************************/
static void tx_descr_init (void) {
	/* Initialize Transmit Descriptor and Status array. */
	uint32_t i;

	for (i = 0; i < EMAC_NUM_TX_FRAG; i++) {
		Tx_Desc[i].Packet = (uint32_t)&tx_buf[i];
		Tx_Desc[i].Ctrl   = 0;
		Tx_Stat[i].Info   = 0;
	}

	/* Set EMAC Transmit Descriptor Registers. */
	LPC_EMAC->TxDescriptor       = (uint32_t)&Tx_Desc[0];
	LPC_EMAC->TxStatus           = (uint32_t)&Tx_Stat[0];
	LPC_EMAC->TxDescriptorNumber = EMAC_NUM_TX_FRAG - 1;

	/* Tx Descriptors Point to 0 */
	LPC_EMAC->TxProduceIndex  = 0;
}


/*--------------------------- write_PHY -------------------------------------*/
/*********************************************************************//**
 * @brief 		Write value to PHY device
 * @param[in] 	PhyReg: PHY Register address
 * @param[in] 	Value:  Value to write
 * @return 		0 - if success
 * 				1 - if fail
 ***********************************************************************/
static int32_t write_PHY (uint32_t PhyReg, uint16_t Value)
{
	/* Write a data 'Value' to PHY register 'PhyReg'. */
	uint32_t tout;

	LPC_EMAC->MADR = EMAC_DEF_ADR | PhyReg;
	LPC_EMAC->MWTD = Value;

	/* Wait until operation completed */
	tout = 0;
	for (tout = 0; tout < EMAC_MII_WR_TOUT; tout++) {
		if ((LPC_EMAC->MIND & EMAC_MIND_BUSY) == 0) {
			return (0);
		}
	}
	// Time out!
	return (-1);
}


/*--------------------------- read_PHY --------------------------------------*/
/*********************************************************************//**
 * @brief 		Read value from PHY device
 * @param[in] 	PhyReg: PHY Register address
 * @return 		0 - if success
 * 				1 - if fail
 ***********************************************************************/
static int32_t read_PHY (uint32_t PhyReg)
{
	/* Read a PHY register 'PhyReg'. */
	uint32_t tout;

	LPC_EMAC->MADR = EMAC_DEF_ADR | PhyReg;
	LPC_EMAC->MCMD = EMAC_MCMD_READ;

	/* Wait until operation completed */
	tout = 0;
	for (tout = 0; tout < EMAC_MII_RD_TOUT; tout++) {
		if ((LPC_EMAC->MIND & EMAC_MIND_BUSY) == 0) {
			LPC_EMAC->MCMD = 0;
			return (LPC_EMAC->MRDD);
		}
	}
	// Time out!
	return (-1);
}

/*********************************************************************//**
 * @brief		Set Station MAC address for EMAC module
 * @param[in]	abStationAddr Pointer to Station address that contains 6-bytes
 * 				of MAC address (should be in order from MAC Address 1 to MAC Address 6)
 * @return		None
 **********************************************************************/
static void setEmacAddr(uint8_t abStationAddr[])
{
	/* Set the Ethernet MAC Address registers */
	LPC_EMAC->SA0 = ((uint32_t)abStationAddr[5] << 8) | (uint32_t)abStationAddr[4];
	LPC_EMAC->SA1 = ((uint32_t)abStationAddr[3] << 8) | (uint32_t)abStationAddr[2];
	LPC_EMAC->SA2 = ((uint32_t)abStationAddr[1] << 8) | (uint32_t)abStationAddr[0];
}


/*********************************************************************//**
 * @brief		Calculates CRC code for number of bytes in the frame
 * @param[in]	frame_no_fcs	Pointer to the first byte of the frame
 * @param[in]	frame_len		length of the frame without the FCS
 * @return		the CRC as a 32 bit integer
 **********************************************************************/
static int32_t emac_CRCCalc(uint8_t frame_no_fcs[], int32_t frame_len)
{
	int i; 		// iterator
	int j; 		// another iterator
	char byte; 	// current byte
	int crc; 	// CRC result
	int q0, q1, q2, q3; // temporary variables
	crc = 0xFFFFFFFF;
	for (i = 0; i < frame_len; i++) {
		byte = *frame_no_fcs++;
		for (j = 0; j < 2; j++) {
			if (((crc >> 28) ^ (byte >> 3)) & 0x00000001) {
				q3 = 0x04C11DB7;
			} else {
				q3 = 0x00000000;
			}
			if (((crc >> 29) ^ (byte >> 2)) & 0x00000001) {
				q2 = 0x09823B6E;
			} else {
				q2 = 0x00000000;
			}
			if (((crc >> 30) ^ (byte >> 1)) & 0x00000001) {
				q1 = 0x130476DC;
			} else {
				q1 = 0x00000000;
			}
			if (((crc >> 31) ^ (byte >> 0)) & 0x00000001) {
				q0 = 0x2608EDB8;
			} else {
				q0 = 0x00000000;
			}
			crc = (crc << 4) ^ q3 ^ q2 ^ q1 ^ q0;
			byte >>= 4;
		}
	}
	return crc;
}
/* End of Private Functions --------------------------------------------------- */


/* Public Functions ----------------------------------------------------------- */
/** @addtogroup EMAC_Public_Functions
 * @{
 */


/*********************************************************************//**
 * @brief		Initializes the EMAC peripheral according to the specified
*               parameters in the EMAC_ConfigStruct.
 * @param[in]	EMAC_ConfigStruct Pointer to a EMAC_CFG_Type structure
*                    that contains the configuration information for the
*                    specified EMAC peripheral.
 * @return		None
 *
 * Note: This function will initialize EMAC module according to procedure below:
 *  - Remove the soft reset condition from the MAC
 *  - Configure the PHY via the MIIM interface of the MAC
 *  - Select RMII mode
 *  - Configure the transmit and receive DMA engines, including the descriptor arrays
 *  - Configure the host registers (MAC1,MAC2 etc.) in the MAC
 *  - Enable the receive and transmit data paths
 *  In default state after initializing, only Rx Done and Tx Done interrupt are enabled,
 *  all remain interrupts are disabled
 *  (Ref. from LPC17xx UM)
 **********************************************************************/
Status EMAC_Init(EMAC_CFG_Type *EMAC_ConfigStruct)
{
	/* Initialize the EMAC Ethernet controller. */
	int32_t regv,tout, tmp;

	/* Set up clock and power for Ethernet module */
	CLKPWR_ConfigPPWR (CLKPWR_PCONP_PCENET, ENABLE);

	/* Reset all EMAC internal modules */
	LPC_EMAC->MAC1    = EMAC_MAC1_RES_TX | EMAC_MAC1_RES_MCS_TX | EMAC_MAC1_RES_RX |
					EMAC_MAC1_RES_MCS_RX | EMAC_MAC1_SIM_RES | EMAC_MAC1_SOFT_RES;

	LPC_EMAC->Command = EMAC_CR_REG_RES | EMAC_CR_TX_RES | EMAC_CR_RX_RES | EMAC_CR_PASS_RUNT_FRM;

	/* A short delay after reset. */
	for (tout = 100; tout; tout--);

	/* Initialize MAC control registers. */
	LPC_EMAC->MAC1 = EMAC_MAC1_PASS_ALL;
	LPC_EMAC->MAC2 = EMAC_MAC2_CRC_EN | EMAC_MAC2_PAD_EN;
	LPC_EMAC->MAXF = EMAC_ETH_MAX_FLEN;
	/*
	 * Find the clock that close to desired target clock
	 */
	tmp = SystemCoreClock / EMAC_MCFG_MII_MAXCLK;
	for (tout = 0; tout < sizeof (EMAC_clkdiv); tout++){
		if (EMAC_clkdiv[tout] >= tmp) break;
	}
	tout++;
	// Write to MAC configuration register and reset
	LPC_EMAC->MCFG = EMAC_MCFG_CLK_SEL(tout) | EMAC_MCFG_RES_MII;
	// release reset
	LPC_EMAC->MCFG &= ~(EMAC_MCFG_RES_MII);
	LPC_EMAC->CLRT = EMAC_CLRT_DEF;
	LPC_EMAC->IPGR = EMAC_IPGR_P2_DEF;

	/* Enable Reduced MII interface. */
	LPC_EMAC->Command = EMAC_CR_RMII | EMAC_CR_PASS_RUNT_FRM;

	/* Reset Reduced MII Logic. */
//	LPC_EMAC->SUPP = EMAC_SUPP_RES_RMII;

	for (tout = 100; tout; tout--);
	LPC_EMAC->SUPP = 0;

	/* Put the DP83848C in reset mode */
	write_PHY (EMAC_PHY_REG_BMCR, EMAC_PHY_BMCR_RESET);

	/* Wait for hardware reset to end. */
	for (tout = EMAC_PHY_RESP_TOUT; tout>=0; tout--) {
		regv = read_PHY (EMAC_PHY_REG_BMCR);
		if (!(regv & (EMAC_PHY_BMCR_RESET | EMAC_PHY_BMCR_POWERDOWN))) {
			/* Reset complete, device not Power Down. */
			break;
		}
		if (tout == 0){
			// Time out, return ERROR
			return (ERROR);
		}
	}

	// Set PHY mode
	if (EMAC_SetPHYMode(EMAC_ConfigStruct->Mode) < 0){
		return (ERROR);
	}

	// Set EMAC address
	setEmacAddr(EMAC_ConfigStruct->pbEMAC_Addr);

	/* Initialize Tx and Rx DMA Descriptors */
	rx_descr_init ();
	tx_descr_init ();

	// Set Receive Filter register: enable broadcast and multicast
	LPC_EMAC->RxFilterCtrl = EMAC_RFC_MCAST_EN | EMAC_RFC_BCAST_EN | EMAC_RFC_PERFECT_EN;

	/* Enable Rx Done and Tx Done interrupt for EMAC */
	LPC_EMAC->IntEnable = EMAC_INT_RX_DONE | EMAC_INT_TX_DONE;

	/* Reset all interrupts */
	LPC_EMAC->IntClear  = 0xFFFF;

	/* Enable receive and transmit mode of MAC Ethernet core */
	LPC_EMAC->Command  |= (EMAC_CR_RX_EN | EMAC_CR_TX_EN);
	LPC_EMAC->MAC1     |= EMAC_MAC1_REC_EN;

	return SUCCESS;
}


/*********************************************************************//**
 * @brief		De-initializes the EMAC peripheral registers to their
*                  default reset values.
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void EMAC_DeInit(void)
{
	// Disable all interrupt
	LPC_EMAC->IntEnable = 0x00;
	// Clear all pending interrupt
	LPC_EMAC->IntClear = (0xFF) | (EMAC_INT_SOFT_INT | EMAC_INT_WAKEUP);

	/* TurnOff clock and power for Ethernet module */
	CLKPWR_ConfigPPWR (CLKPWR_PCONP_PCENET, DISABLE);
}


/*********************************************************************//**
 * @brief		Check specified PHY status in EMAC peripheral
 * @param[in]	ulPHYState	Specified PHY Status Type, should be:
 * 							- EMAC_PHY_STAT_LINK: Link Status
 * 							- EMAC_PHY_STAT_SPEED: Speed Status
 * 							- EMAC_PHY_STAT_DUP: Duplex Status
 * @return		Status of specified PHY status (0 or 1).
 * 				(-1) if error.
 *
 * Note:
 * For EMAC_PHY_STAT_LINK, return value:
 * - 0: Link Down
 * - 1: Link Up
 * For EMAC_PHY_STAT_SPEED, return value:
 * - 0: 10Mbps
 * - 1: 100Mbps
 * For EMAC_PHY_STAT_DUP, return value:
 * - 0: Half-Duplex
 * - 1: Full-Duplex
 **********************************************************************/
int32_t EMAC_CheckPHYStatus(uint32_t ulPHYState)
{
	int32_t regv, tmp;
#ifdef MCB_LPC_1768
	regv = read_PHY (EMAC_PHY_REG_STS);
	switch(ulPHYState){
	case EMAC_PHY_STAT_LINK:
		tmp = (regv & EMAC_PHY_SR_LINK) ? 1 : 0;
		break;
	case EMAC_PHY_STAT_SPEED:
		tmp = (regv & EMAC_PHY_SR_SPEED) ? 0 : 1;
		break;
	case EMAC_PHY_STAT_DUP:
		tmp = (regv & EMAC_PHY_SR_FULL_DUP) ? 1 : 0;
		break;
#elif defined(IAR_LPC_1768)
	/* Use IAR_LPC_1768 board:
	 * FSZ8721BL doesn't have Status Register
	 * so we read Basic Mode Status Register (0x01h) instead
	 */
	regv = read_PHY (EMAC_PHY_REG_BMSR);
	switch(ulPHYState){
	case EMAC_PHY_STAT_LINK:
		tmp = (regv & EMAC_PHY_BMSR_LINK_STATUS) ? 1 : 0;
		break;
	case EMAC_PHY_STAT_SPEED:
		tmp = (regv & EMAC_PHY_SR_100_SPEED) ? 1 : 0;
		break;
	case EMAC_PHY_STAT_DUP:
		tmp = (regv & EMAC_PHY_SR_FULL_DUP) ? 1 : 0;
		break;
#endif
	default:
		tmp = -1;
		break;
	}
	return (tmp);
}


/*********************************************************************//**
 * @brief		Set specified PHY mode in EMAC peripheral
 * @param[in]	ulPHYMode	Specified PHY mode, should be:
 * 							- EMAC_MODE_AUTO
 * 							- EMAC_MODE_10M_FULL
 * 							- EMAC_MODE_10M_HALF
 * 							- EMAC_MODE_100M_FULL
 * 							- EMAC_MODE_100M_HALF
 * @return		Return (0) if no error, otherwise return (-1)
 **********************************************************************/
int32_t EMAC_SetPHYMode(uint32_t ulPHYMode)
{
	int32_t id1, id2, tout;

	/* Check if this is a DP83848C PHY. */
	id1 = read_PHY (EMAC_PHY_REG_IDR1);
	id2 = read_PHY (EMAC_PHY_REG_IDR2);

#ifdef MCB_LPC_1768
	if (((id1 << 16) | (id2 & 0xFFF0)) == EMAC_DP83848C_ID) {
		switch(ulPHYMode){
		case EMAC_MODE_AUTO:
			write_PHY (EMAC_PHY_REG_BMCR, EMAC_PHY_AUTO_NEG);
#elif defined(IAR_LPC_1768) /* Use IAR LPC1768 KickStart board */
	if (((id1 << 16) | id2) == EMAC_KSZ8721BL_ID) {
		/* Configure the PHY device */
		switch(ulPHYMode){
		case EMAC_MODE_AUTO:
			/* Use auto-negotiation about the link speed. */
			write_PHY (EMAC_PHY_REG_BMCR, EMAC_PHY_AUTO_NEG);
//			write_PHY (EMAC_PHY_REG_BMCR, EMAC_PHY_BMCR_AN);
#endif
			/* Wait to complete Auto_Negotiation */
			for (tout = EMAC_PHY_RESP_TOUT; tout>=0; tout--) {

			}
			break;
		case EMAC_MODE_10M_FULL:
			/* Connect at 10MBit full-duplex */
			write_PHY (EMAC_PHY_REG_BMCR, EMAC_PHY_FULLD_10M);
			break;
		case EMAC_MODE_10M_HALF:
			/* Connect at 10MBit half-duplex */
			write_PHY (EMAC_PHY_REG_BMCR, EMAC_PHY_HALFD_10M);
			break;
		case EMAC_MODE_100M_FULL:
			/* Connect at 100MBit full-duplex */
			write_PHY (EMAC_PHY_REG_BMCR, EMAC_PHY_FULLD_100M);
			break;
		case EMAC_MODE_100M_HALF:
			/* Connect at 100MBit half-duplex */
			write_PHY (EMAC_PHY_REG_BMCR, EMAC_PHY_HALFD_100M);
			break;
		default:
			// un-supported
			return (-1);
		}
	}
	// It's not correct module ID
	else {
		return (-1);
	}

	// Update EMAC configuration with current PHY status
	if (EMAC_UpdatePHYStatus() < 0){
		return (-1);
	}

	// Complete
	return (0);
}


/*********************************************************************//**
 * @brief		Auto-Configures value for the EMAC configuration register to
 * 				match with current PHY mode
 * @param[in]	None
 * @return		Return (0) if no error, otherwise return (-1)
 *
 * Note: The EMAC configuration will be auto-configured:
 * 		- Speed mode.
 * 		- Half/Full duplex mode
 **********************************************************************/
int32_t EMAC_UpdatePHYStatus(void)
{
	int32_t regv, tout;

	/* Check the link status. */
#ifdef MCB_LPC_1768
	for (tout = EMAC_PHY_RESP_TOUT; tout>=0; tout--) {
		regv = read_PHY (EMAC_PHY_REG_STS);
		if (regv & EMAC_PHY_SR_LINK) {
			/* Link is on. */
			break;
		}
		if (tout == 0){
			// time out
			return (-1);
		}
	}
	/* Configure Full/Half Duplex mode. */
	if (regv & EMAC_PHY_SR_DUP) {
	/* Full duplex is enabled. */
			LPC_EMAC->MAC2    |= EMAC_MAC2_FULL_DUP;
			LPC_EMAC->Command |= EMAC_CR_FULL_DUP;
			LPC_EMAC->IPGT     = EMAC_IPGT_FULL_DUP;
	} else {
		/* Half duplex mode. */
		LPC_EMAC->IPGT = EMAC_IPGT_HALF_DUP;
	}
	if (regv & EMAC_PHY_SR_SPEED) {
	/* 10MBit mode. */
		LPC_EMAC->SUPP = 0;
	} else {
		/* 100MBit mode. */
		LPC_EMAC->SUPP = EMAC_SUPP_SPEED;
	}
#elif defined(IAR_LPC_1768)
	for (tout = EMAC_PHY_RESP_TOUT; tout>=0; tout--) {
		regv = read_PHY (EMAC_PHY_REG_BMSR);
		if (regv & EMAC_PHY_BMSR_LINK_STATUS) {
			/* Link is on. */
			break;
		}
		if (tout == 0){
			// time out
			return (-1);
		}
	}

	/* Configure Full/Half Duplex mode. */
	if (regv & EMAC_PHY_SR_FULL_DUP) {
		/* Full duplex is enabled. */
		LPC_EMAC->MAC2    |= EMAC_MAC2_FULL_DUP;
		LPC_EMAC->Command |= EMAC_CR_FULL_DUP;
		LPC_EMAC->IPGT     = EMAC_IPGT_FULL_DUP;
	} else {
		/* Half duplex mode. */
		LPC_EMAC->IPGT = EMAC_IPGT_HALF_DUP;
	}

	/* Configure 100MBit/10MBit mode. */
	if (!(regv & EMAC_PHY_SR_100_SPEED)) {
		/* 10MBit mode. */
		LPC_EMAC->SUPP = 0;
	} else {
		/* 100MBit mode. */
		LPC_EMAC->SUPP = EMAC_SUPP_SPEED;
	}
#endif
	// Complete
	return (0);
}


/*********************************************************************//**
 * @brief		Enable/Disable hash filter functionality for specified destination
 * 				MAC address in EMAC module
 * @param[in]	dstMAC_addr		Pointer to the first MAC destination address, should
 * 								be 6-bytes length, in order LSB to the MSB
 * @param[in]	NewState		New State of this command, should be:
 *									- ENABLE.
 *									- DISABLE.
 * @return		None
 *
 * Note:
 * The standard Ethernet cyclic redundancy check (CRC) function is calculated from
 * the 6 byte destination address in the Ethernet frame (this CRC is calculated
 * anyway as part of calculating the CRC of the whole frame), then bits [28:23] out of
 * the 32 bits CRC result are taken to form the hash. The 6 bit hash is used to access
 * the hash table: it is used as an index in the 64 bit HashFilter register that has been
 * programmed with accept values. If the selected accept value is 1, the frame is
 * accepted.
 **********************************************************************/
void EMAC_SetHashFilter(uint8_t dstMAC_addr[], FunctionalState NewState)
{
	uint32_t *pReg;
	uint32_t tmp;
	int32_t crc;

	// Calculate the CRC from the destination MAC address
	crc = emac_CRCCalc(dstMAC_addr, 6);
	// Extract the value from CRC to get index value for hash filter table
	crc = (crc >> 23) & 0x3F;

	pReg = (crc > 31) ? ((uint32_t *)&LPC_EMAC->HashFilterH) \
								: ((uint32_t *)&LPC_EMAC->HashFilterL);
	tmp = (crc > 31) ? (crc - 32) : crc;
	if (NewState == ENABLE) {
		(*pReg) |= (1UL << tmp);
	} else {
		(*pReg) &= ~(1UL << tmp);
	}
	// Enable Rx Filter
	LPC_EMAC->Command &= ~EMAC_CR_PASS_RX_FILT;
}

/*********************************************************************//**
 * @brief		Enable/Disable Filter mode for each specified type EMAC peripheral
 * @param[in]	ulFilterMode	Filter mode, should be:
 * 								- EMAC_RFC_UCAST_EN: all frames of unicast types
 * 								will be accepted
 * 								- EMAC_RFC_BCAST_EN: broadcast frame will be
 * 								accepted
 * 								- EMAC_RFC_MCAST_EN: all frames of multicast
 * 								types will be accepted
 * 								- EMAC_RFC_UCAST_HASH_EN: The imperfect hash
 * 								filter will be applied to unicast addresses
 * 								- EMAC_RFC_MCAST_HASH_EN: The imperfect hash
 * 								filter will be applied to multicast addresses
 * 								- EMAC_RFC_PERFECT_EN: the destination address
 * 								will be compared with the 6 byte station address
 * 								programmed in the station address by the filter
 * 								- EMAC_RFC_MAGP_WOL_EN: the result of the magic
 * 								packet filter will generate a WoL interrupt when
 * 								there is a match
 * 								- EMAC_RFC_PFILT_WOL_EN: the result of the perfect address
 * 								matching filter and the imperfect hash filter will
 * 								generate a WoL interrupt when there is a match
 * @param[in]	NewState	New State of this command, should be:
 * 								- ENABLE
 * 								- DISABLE
 * @return		None
 **********************************************************************/
void EMAC_SetFilterMode(uint32_t ulFilterMode, FunctionalState NewState)
{
	if (NewState == ENABLE){
		LPC_EMAC->RxFilterCtrl |= ulFilterMode;
	} else {
		LPC_EMAC->RxFilterCtrl &= ~ulFilterMode;
	}
}

/*********************************************************************//**
 * @brief		Get status of Wake On LAN Filter for each specified
 * 				type in EMAC peripheral, clear this status if it is set
 * @param[in]	ulWoLMode	WoL Filter mode, should be:
 * 								- EMAC_WOL_UCAST: unicast frames caused WoL
 * 								- EMAC_WOL_UCAST: broadcast frame caused WoL
 * 								- EMAC_WOL_MCAST: multicast frame caused WoL
 * 								- EMAC_WOL_UCAST_HASH: unicast frame that passes the
 * 								imperfect hash filter caused WoL
 * 								- EMAC_WOL_MCAST_HASH: multicast frame that passes the
 * 								imperfect hash filter caused WoL
 * 								- EMAC_WOL_PERFECT:perfect address matching filter
 * 								caused WoL
 * 								- EMAC_WOL_RX_FILTER: the receive filter caused WoL
 * 								- EMAC_WOL_MAG_PACKET: the magic packet filter caused WoL
 * @return		SET/RESET
 **********************************************************************/
FlagStatus EMAC_GetWoLStatus(uint32_t ulWoLMode)
{
	if (LPC_EMAC->RxFilterWoLStatus & ulWoLMode) {
		LPC_EMAC->RxFilterWoLClear = ulWoLMode;
		return SET;
	} else {
		return RESET;
	}
}


/*********************************************************************//**
 * @brief		Write data to Tx packet data buffer at current index due to
 * 				TxProduceIndex
 * @param[in]	pDataStruct		Pointer to a EMAC_PACKETBUF_Type structure
 * 							data that contain specified information about
 * 							Packet data buffer.
 * @return		None
 **********************************************************************/
void EMAC_WritePacketBuffer(EMAC_PACKETBUF_Type *pDataStruct)
{
	uint32_t idx,len;
	uint32_t *sp,*dp;

	idx = LPC_EMAC->TxProduceIndex;
	sp  = (uint32_t *)pDataStruct->pbDataBuf;
	dp  = (uint32_t *)Tx_Desc[idx].Packet;
	/* Copy frame data to EMAC packet buffers. */
	for (len = (pDataStruct->ulDataLen + 3) >> 2; len; len--) {
		*dp++ = *sp++;
	}
	Tx_Desc[idx].Ctrl = (pDataStruct->ulDataLen - 1) | (EMAC_TCTRL_INT | EMAC_TCTRL_LAST);
}

/*********************************************************************//**
 * @brief		Read data from Rx packet data buffer at current index due
 * 				to RxConsumeIndex
 * @param[in]	pDataStruct		Pointer to a EMAC_PACKETBUF_Type structure
 * 							data that contain specified information about
 * 							Packet data buffer.
 * @return		None
 **********************************************************************/
void EMAC_ReadPacketBuffer(EMAC_PACKETBUF_Type *pDataStruct)
{
	uint32_t idx, len;
	uint32_t *dp, *sp;

	idx = LPC_EMAC->RxConsumeIndex;
	dp = (uint32_t *)pDataStruct->pbDataBuf;
	sp = (uint32_t *)Rx_Desc[idx].Packet;

	if (pDataStruct->pbDataBuf != NULL) {
		for (len = (pDataStruct->ulDataLen + 3) >> 2; len; len--) {
			*dp++ = *sp++;
		}
	}
}

/*********************************************************************//**
 * @brief 		Enable/Disable interrupt for each type in EMAC
 * @param[in]	ulIntType	Interrupt Type, should be:
 * 							- EMAC_INT_RX_OVERRUN: Receive Overrun
 * 							- EMAC_INT_RX_ERR: Receive Error
 * 							- EMAC_INT_RX_FIN: Receive Descriptor Finish
 * 							- EMAC_INT_RX_DONE: Receive Done
 * 							- EMAC_INT_TX_UNDERRUN: Transmit Under-run
 * 							- EMAC_INT_TX_ERR: Transmit Error
 * 							- EMAC_INT_TX_FIN: Transmit descriptor finish
 * 							- EMAC_INT_TX_DONE: Transmit Done
 * 							- EMAC_INT_SOFT_INT: Software interrupt
 * 							- EMAC_INT_WAKEUP: Wakeup interrupt
 * @param[in]	NewState	New State of this function, should be:
 * 							- ENABLE.
 * 							- DISABLE.
 * @return		None
 **********************************************************************/
void EMAC_IntCmd(uint32_t ulIntType, FunctionalState NewState)
{
	if (NewState == ENABLE) {
		LPC_EMAC->IntEnable |= ulIntType;
	} else {
		LPC_EMAC->IntEnable &= ~(ulIntType);
	}
}

/*********************************************************************//**
 * @brief 		Check whether if specified interrupt flag is set or not
 * 				for each interrupt type in EMAC and clear interrupt pending
 * 				if it is set.
 * @param[in]	ulIntType	Interrupt Type, should be:
 * 							- EMAC_INT_RX_OVERRUN: Receive Overrun
 * 							- EMAC_INT_RX_ERR: Receive Error
 * 							- EMAC_INT_RX_FIN: Receive Descriptor Finish
 * 							- EMAC_INT_RX_DONE: Receive Done
 * 							- EMAC_INT_TX_UNDERRUN: Transmit Under-run
 * 							- EMAC_INT_TX_ERR: Transmit Error
 * 							- EMAC_INT_TX_FIN: Transmit descriptor finish
 * 							- EMAC_INT_TX_DONE: Transmit Done
 * 							- EMAC_INT_SOFT_INT: Software interrupt
 * 							- EMAC_INT_WAKEUP: Wakeup interrupt
 * @return		New state of specified interrupt (SET or RESET)
 **********************************************************************/
IntStatus EMAC_IntGetStatus(uint32_t ulIntType)
{
	if (LPC_EMAC->IntStatus & ulIntType) {
		LPC_EMAC->IntClear = ulIntType;
		return SET;
	} else {
		return RESET;
	}
}


/*********************************************************************//**
 * @brief		Check whether if the current RxConsumeIndex is not equal to the
 * 				current RxProduceIndex.
 * @param[in]	None
 * @return		TRUE if they're not equal, otherwise return FALSE
 *
 * Note: In case the RxConsumeIndex is not equal to the RxProduceIndex,
 * it means there're available data has been received. They should be read
 * out and released the Receive Data Buffer by updating the RxConsumeIndex value.
 **********************************************************************/
Bool EMAC_CheckReceiveIndex(void)
{
	if (LPC_EMAC->RxConsumeIndex != LPC_EMAC->RxProduceIndex) {
		return TRUE;
	} else {
		return FALSE;
	}
}


/*********************************************************************//**
 * @brief		Check whether if the current TxProduceIndex is not equal to the
 * 				current RxProduceIndex - 1.
 * @param[in]	None
 * @return		TRUE if they're not equal, otherwise return FALSE
 *
 * Note: In case the RxConsumeIndex is equal to the RxProduceIndex - 1,
 * it means the transmit buffer is available and data can be written to transmit
 * buffer to be sent.
 **********************************************************************/
Bool EMAC_CheckTransmitIndex(void)
{
    uint32_t tmp = LPC_EMAC->TxConsumeIndex;
    if (LPC_EMAC->TxProduceIndex == ( tmp - 1 ))
    {
        return FALSE;
    }
    else if( ( tmp == 0 ) && ( LPC_EMAC->TxProduceIndex == ( EMAC_NUM_TX_FRAG - 1 ) ) )
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}



/*********************************************************************//**
 * @brief		Get current status value of receive data (due to RxConsumeIndex)
 * @param[in]	ulRxStatType	Received Status type, should be one of following:
 * 							- EMAC_RINFO_CTRL_FRAME: Control Frame
 * 							- EMAC_RINFO_VLAN: VLAN Frame
 * 							- EMAC_RINFO_FAIL_FILT: RX Filter Failed
 * 							- EMAC_RINFO_MCAST: Multicast Frame
 * 							- EMAC_RINFO_BCAST: Broadcast Frame
 * 							- EMAC_RINFO_CRC_ERR: CRC Error in Frame
 * 							- EMAC_RINFO_SYM_ERR: Symbol Error from PHY
 * 							- EMAC_RINFO_LEN_ERR: Length Error
 * 							- EMAC_RINFO_RANGE_ERR: Range error(exceeded max size)
 * 							- EMAC_RINFO_ALIGN_ERR: Alignment error
 * 							- EMAC_RINFO_OVERRUN: Receive overrun
 * 							- EMAC_RINFO_NO_DESCR: No new Descriptor available
 * 							- EMAC_RINFO_LAST_FLAG: last Fragment in Frame
 * 							- EMAC_RINFO_ERR: Error Occurred (OR of all error)
 * @return		Current value of receive data (due to RxConsumeIndex)
 **********************************************************************/
FlagStatus EMAC_CheckReceiveDataStatus(uint32_t ulRxStatType)
{
	uint32_t idx;
	idx = LPC_EMAC->RxConsumeIndex;
	return (((Rx_Stat[idx].Info) & ulRxStatType) ? SET : RESET);
}


/*********************************************************************//**
 * @brief		Get size of current Received data in received buffer (due to
 * 				RxConsumeIndex)
 * @param[in]	None
 * @return		Size of received data
 **********************************************************************/
uint32_t EMAC_GetReceiveDataSize(void)
{
	uint32_t idx;
	idx =LPC_EMAC->RxConsumeIndex;
	return ((Rx_Stat[idx].Info) & EMAC_RINFO_SIZE);
}

/*********************************************************************//**
 * @brief		Increase the RxConsumeIndex (after reading the Receive buffer
 * 				to release the Receive buffer) and wrap-around the index if
 * 				it reaches the maximum Receive Number
 * @param[in]	None
 * @return		None
 **********************************************************************/
void EMAC_UpdateRxConsumeIndex(void)
{
	// Get current Rx consume index
	uint32_t idx = LPC_EMAC->RxConsumeIndex;

	/* Release frame from EMAC buffer */
	if (++idx == EMAC_NUM_RX_FRAG) idx = 0;
	LPC_EMAC->RxConsumeIndex = idx;
}

/*********************************************************************//**
 * @brief		Increase the TxProduceIndex (after writting to the Transmit buffer
 * 				to enable the Transmit buffer) and wrap-around the index if
 * 				it reaches the maximum Transmit Number
 * @param[in]	None
 * @return		None
 **********************************************************************/
void EMAC_UpdateTxProduceIndex(void)
{
	// Get current Tx produce index
	uint32_t idx = LPC_EMAC->TxProduceIndex;

	/* Start frame transmission */
	if (++idx == EMAC_NUM_TX_FRAG) idx = 0;
	LPC_EMAC->TxProduceIndex = idx;
}


/**
 * @}
 */

#endif /* _EMAC */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
