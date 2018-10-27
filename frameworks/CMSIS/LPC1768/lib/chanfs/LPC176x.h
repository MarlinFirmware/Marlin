/*-------------------------------------------------------------------*/
/* LPC176x Register Definitions and Cortex-M3 Supplement Definitions */
/*       This file is a non-copyrighted public domain software       */
/*-------------------------------------------------------------------*/

#ifndef __LPC176x
#define __LPC176x
#include <stdint.h>

#define	USE_SV_SERVICE	0	/* Enable supervisor service for user mode task */


/* System Controls */
#define	EXTINT		(*(volatile uint32_t*)0x400FC140)
#define	EXTMODE		(*(volatile uint32_t*)0x400FC148)
#define	EXTPOLAR	(*(volatile uint32_t*)0x400FC14C)
#define	RSID		(*(volatile uint32_t*)0x400FC180)
#define	SCS			(*(volatile uint32_t*)0x400FC1A0)

/* Clocking and Power Controls */
#define	CLKSRCSEL	(*(volatile uint32_t*)0x400FC10C)
#define	PLL0CON		(*(volatile uint32_t*)0x400FC080)
#define	PLL0CFG		(*(volatile uint32_t*)0x400FC084)
#define	PLL0STAT	(*(volatile uint32_t*)0x400FC088)
#define	PLL0FEED	(*(volatile uint32_t*)0x400FC08C)
#define	PLL1CON		(*(volatile uint32_t*)0x400FC0A0)
#define	PLL1CFG		(*(volatile uint32_t*)0x400FC0A4)
#define	PLL1STAT	(*(volatile uint32_t*)0x400FC0A8)
#define	PLL1FEED	(*(volatile uint32_t*)0x400FC0AC)
#define	CCLKCFG		(*(volatile uint32_t*)0x400FC104)
#define	USBCLKCFG	(*(volatile uint32_t*)0x400FC108)
#define	PCLKSEL		( (volatile uint32_t*)0x400FC1A8)
#define	PCLKSEL0	(*(volatile uint32_t*)0x400FC1A8)
#define	PCLKSEL1	(*(volatile uint32_t*)0x400FC1AC)
#define	PCON		(*(volatile uint32_t*)0x400FC0C0)
#define	PCONP		(*(volatile uint32_t*)0x400FC0C4)
#define	CLKOUTCFG	(*(volatile uint32_t*)0x400FC1C8)

/* Flash Accelerator */
#define	FLASHCFG	(*(volatile uint32_t*)0x400FC000)

/* Pin Configurations */
#define	PINSEL		( (volatile uint32_t*)0x4002C000)
#define	PINSEL0		(*(volatile uint32_t*)0x4002C000)
#define	PINSEL1		(*(volatile uint32_t*)0x4002C004)
#define	PINSEL2		(*(volatile uint32_t*)0x4002C008)
#define	PINSEL3		(*(volatile uint32_t*)0x4002C00C)
#define	PINSEL4		(*(volatile uint32_t*)0x4002C010)
#define	PINSEL7		(*(volatile uint32_t*)0x4002C01C)
#define	PINSEL8		(*(volatile uint32_t*)0x4002C020)
#define	PINSEL9		(*(volatile uint32_t*)0x4002C024)
#define	PINSEL10	(*(volatile uint32_t*)0x4002C028)
#define	PINMODE		( (volatile uint32_t*)0x4002C040)
#define	PINMODE0	(*(volatile uint32_t*)0x4002C040)
#define	PINMODE1	(*(volatile uint32_t*)0x4002C044)
#define	PINMODE2	(*(volatile uint32_t*)0x4002C048)
#define	PINMODE3	(*(volatile uint32_t*)0x4002C04C)
#define	PINMODE4	(*(volatile uint32_t*)0x4002C050)
#define	PINMODE5	(*(volatile uint32_t*)0x4002C054)
#define	PINMODE6	(*(volatile uint32_t*)0x4002C058)
#define	PINMODE7	(*(volatile uint32_t*)0x4002C05C)
#define	PINMODE9	(*(volatile uint32_t*)0x4002C064)
#define	PINMODE_OD	( (volatile uint32_t*)0x4002C068)
#define	PINMODE_OD0	(*(volatile uint32_t*)0x4002C068)
#define	PINMODE_OD1	(*(volatile uint32_t*)0x4002C06C)
#define	PINMODE_OD2	(*(volatile uint32_t*)0x4002C070)
#define	PINMODE_OD3	(*(volatile uint32_t*)0x4002C074)
#define	PINMODE_OD4	(*(volatile uint32_t*)0x4002C078)
#define	I2CPADCFG	(*(volatile uint32_t*)0x4002C07C)

/* GPIO */
#define	FIO0DIR		(*(volatile uint32_t*)0x2009C000)
#define	FIO0DIRL	(*(volatile uint16_t*)0x2009C000)
#define	FIO0DIRH	(*(volatile uint16_t*)0x2009C002)
#define	FIO0DIR0	(*(volatile uint8_t*)0x2009C000)
#define	FIO0DIR1	(*(volatile uint8_t*)0x2009C001)
#define	FIO0DIR2	(*(volatile uint8_t*)0x2009C002)
#define	FIO0DIR3	(*(volatile uint8_t*)0x2009C003)

#define	FIO1DIR		(*(volatile uint32_t*)0x2009C020)
#define	FIO1DIRL	(*(volatile uint16_t*)0x2009C020)
#define	FIO1DIRH	(*(volatile uint16_t*)0x2009C022)
#define	FIO1DIR0	(*(volatile uint8_t*)0x2009C020)
#define	FIO1DIR1	(*(volatile uint8_t*)0x2009C021)
#define	FIO1DIR2	(*(volatile uint8_t*)0x2009C022)
#define	FIO1DIR3	(*(volatile uint8_t*)0x2009C023)

#define	FIO2DIR		(*(volatile uint32_t*)0x2009C040)
#define	FIO2DIRL	(*(volatile uint16_t*)0x2009C040)
#define	FIO2DIRH	(*(volatile uint16_t*)0x2009C042)
#define	FIO2DIR0	(*(volatile uint8_t*)0x2009C040)
#define	FIO2DIR1	(*(volatile uint8_t*)0x2009C041)
#define	FIO2DIR2	(*(volatile uint8_t*)0x2009C042)
#define	FIO2DIR3	(*(volatile uint8_t*)0x2009C043)

#define	FIO3DIR		(*(volatile uint32_t*)0x2009C060)
#define	FIO3DIRL	(*(volatile uint16_t*)0x2009C060)
#define	FIO3DIRH	(*(volatile uint16_t*)0x2009C062)
#define	FIO3DIR0	(*(volatile uint8_t*)0x2009C060)
#define	FIO3DIR1	(*(volatile uint8_t*)0x2009C061)
#define	FIO3DIR2	(*(volatile uint8_t*)0x2009C062)
#define	FIO3DIR3	(*(volatile uint8_t*)0x2009C063)

#define	FIO4DIR		(*(volatile uint32_t*)0x2009C080)
#define	FIO4DIRL	(*(volatile uint16_t*)0x2009C080)
#define	FIO4DIRH	(*(volatile uint16_t*)0x2009C082)
#define	FIO4DIR0	(*(volatile uint8_t*)0x2009C080)
#define	FIO4DIR1	(*(volatile uint8_t*)0x2009C081)
#define	FIO4DIR2	(*(volatile uint8_t*)0x2009C082)
#define	FIO4DIR3	(*(volatile uint8_t*)0x2009C083)

#define	FIO0MASK	(*(volatile uint32_t*)0x2009C010)
#define	FIO0MASKL	(*(volatile uint16_t*)0x2009C010)
#define	FIO0MASKH	(*(volatile uint16_t*)0x2009C012)
#define	FIO0MASK0	(*(volatile uint8_t*)0x2009C010)
#define	FIO0MASK1	(*(volatile uint8_t*)0x2009C011)
#define	FIO0MASK2	(*(volatile uint8_t*)0x2009C012)
#define	FIO0MASK3	(*(volatile uint8_t*)0x2009C013)

#define	FIO1MASK	(*(volatile uint32_t*)0x2009C030)
#define	FIO1MASKL	(*(volatile uint16_t*)0x2009C030)
#define	FIO1MASKH	(*(volatile uint16_t*)0x2009C032)
#define	FIO1MASK0	(*(volatile uint8_t*)0x2009C030)
#define	FIO1MASK1	(*(volatile uint8_t*)0x2009C031)
#define	FIO1MASK2	(*(volatile uint8_t*)0x2009C032)
#define	FIO1MASK3	(*(volatile uint8_t*)0x2009C033)

#define	FIO2MASK	(*(volatile uint32_t*)0x2009C050)
#define	FIO2MASKL	(*(volatile uint16_t*)0x2009C050)
#define	FIO2MASKH	(*(volatile uint16_t*)0x2009C052)
#define	FIO2MASK0	(*(volatile uint8_t*)0x2009C050)
#define	FIO2MASK1	(*(volatile uint8_t*)0x2009C051)
#define	FIO2MASK2	(*(volatile uint8_t*)0x2009C052)
#define	FIO2MASK3	(*(volatile uint8_t*)0x2009C053)

#define	FIO3MASK	(*(volatile uint32_t*)0x2009C070)
#define	FIO3MASKL	(*(volatile uint16_t*)0x2009C070)
#define	FIO3MASKH	(*(volatile uint16_t*)0x2009C072)
#define	FIO3MASK0	(*(volatile uint8_t*)0x2009C070)
#define	FIO3MASK1	(*(volatile uint8_t*)0x2009C071)
#define	FIO3MASK2	(*(volatile uint8_t*)0x2009C072)
#define	FIO3MASK3	(*(volatile uint8_t*)0x2009C073)

#define	FIO4MASK	(*(volatile uint32_t*)0x2009C090)
#define	FIO4MASKL	(*(volatile uint16_t*)0x2009C090)
#define	FIO4MASKH	(*(volatile uint16_t*)0x2009C092)
#define	FIO4MASK0	(*(volatile uint8_t*)0x2009C090)
#define	FIO4MASK1	(*(volatile uint8_t*)0x2009C091)
#define	FIO4MASK2	(*(volatile uint8_t*)0x2009C092)
#define	FIO4MASK3	(*(volatile uint8_t*)0x2009C093)

#define	FIO0PIN		(*(volatile uint32_t*)0x2009C014)
#define	FIO0PINL	(*(volatile uint16_t*)0x2009C014)
#define	FIO0PINH	(*(volatile uint16_t*)0x2009C016)
#define	FIO0PIN0	(*(volatile uint8_t*)0x2009C014)
#define	FIO0PIN1	(*(volatile uint8_t*)0x2009C015)
#define	FIO0PIN2	(*(volatile uint8_t*)0x2009C016)
#define	FIO0PIN3	(*(volatile uint8_t*)0x2009C017)

#define	FIO1PIN		(*(volatile uint32_t*)0x2009C034)
#define	FIO1PINL	(*(volatile uint16_t*)0x2009C034)
#define	FIO1PINH	(*(volatile uint16_t*)0x2009C036)
#define	FIO1PIN0	(*(volatile uint8_t*)0x2009C034)
#define	FIO1PIN1	(*(volatile uint8_t*)0x2009C035)
#define	FIO1PIN2	(*(volatile uint8_t*)0x2009C036)
#define	FIO1PIN3	(*(volatile uint8_t*)0x2009C037)

#define	FIO2PIN		(*(volatile uint32_t*)0x2009C054)
#define	FIO2PINL	(*(volatile uint16_t*)0x2009C054)
#define	FIO2PINH	(*(volatile uint16_t*)0x2009C056)
#define	FIO2PIN0	(*(volatile uint8_t*)0x2009C054)
#define	FIO2PIN1	(*(volatile uint8_t*)0x2009C055)
#define	FIO2PIN2	(*(volatile uint8_t*)0x2009C056)
#define	FIO2PIN3	(*(volatile uint8_t*)0x2009C057)

#define	FIO3PIN		(*(volatile uint32_t*)0x2009C074)
#define	FIO3PINL	(*(volatile uint16_t*)0x2009C074)
#define	FIO3PINH	(*(volatile uint16_t*)0x2009C076)
#define	FIO3PIN0	(*(volatile uint8_t*)0x2009C074)
#define	FIO3PIN1	(*(volatile uint8_t*)0x2009C075)
#define	FIO3PIN2	(*(volatile uint8_t*)0x2009C076)
#define	FIO3PIN3	(*(volatile uint8_t*)0x2009C077)

#define	FIO4PIN		(*(volatile uint32_t*)0x2009C094)
#define	FIO4PINL	(*(volatile uint16_t*)0x2009C094)
#define	FIO4PINH	(*(volatile uint16_t*)0x2009C096)
#define	FIO4PIN0	(*(volatile uint8_t*)0x2009C094)
#define	FIO4PIN1	(*(volatile uint8_t*)0x2009C095)
#define	FIO4PIN2	(*(volatile uint8_t*)0x2009C096)
#define	FIO4PIN3	(*(volatile uint8_t*)0x2009C097)

#define	FIO0SET		(*(volatile uint32_t*)0x2009C018)
#define	FIO0SETL	(*(volatile uint16_t*)0x2009C018)
#define	FIO0SETH	(*(volatile uint16_t*)0x2009C01A)
#define	FIO0SET0	(*(volatile uint8_t*)0x2009C018)
#define	FIO0SET1	(*(volatile uint8_t*)0x2009C019)
#define	FIO0SET2	(*(volatile uint8_t*)0x2009C01A)
#define	FIO0SET3	(*(volatile uint8_t*)0x2009C01B)

#define	FIO1SET		(*(volatile uint32_t*)0x2009C038)
#define	FIO1SETL	(*(volatile uint16_t*)0x2009C038)
#define	FIO1SETH	(*(volatile uint16_t*)0x2009C03A)
#define	FIO1SET0	(*(volatile uint8_t*)0x2009C038)
#define	FIO1SET1	(*(volatile uint8_t*)0x2009C039)
#define	FIO1SET2	(*(volatile uint8_t*)0x2009C03A)
#define	FIO1SET3	(*(volatile uint8_t*)0x2009C03B)

#define	FIO2SET		(*(volatile uint32_t*)0x2009C058)
#define	FIO2SETL	(*(volatile uint16_t*)0x2009C058)
#define	FIO2SETH	(*(volatile uint16_t*)0x2009C05A)
#define	FIO2SET0	(*(volatile uint8_t*)0x2009C058)
#define	FIO2SET1	(*(volatile uint8_t*)0x2009C059)
#define	FIO2SET2	(*(volatile uint8_t*)0x2009C05A)
#define	FIO2SET3	(*(volatile uint8_t*)0x2009C05B)

#define	FIO3SET		(*(volatile uint32_t*)0x2009C078)
#define	FIO3SETL	(*(volatile uint16_t*)0x2009C078)
#define	FIO3SETH	(*(volatile uint16_t*)0x2009C07A)
#define	FIO3SET0	(*(volatile uint8_t*)0x2009C078)
#define	FIO3SET1	(*(volatile uint8_t*)0x2009C079)
#define	FIO3SET2	(*(volatile uint8_t*)0x2009C07A)
#define	FIO3SET3	(*(volatile uint8_t*)0x2009C07B)

#define	FIO4SET		(*(volatile uint32_t*)0x2009C098)
#define	FIO4SETL	(*(volatile uint16_t*)0x2009C098)
#define	FIO4SETH	(*(volatile uint16_t*)0x2009C09A)
#define	FIO4SET0	(*(volatile uint8_t*)0x2009C098)
#define	FIO4SET1	(*(volatile uint8_t*)0x2009C099)
#define	FIO4SET2	(*(volatile uint8_t*)0x2009C09A)
#define	FIO4SET3	(*(volatile uint8_t*)0x2009C09B)

#define	FIO0CLR		(*(volatile uint32_t*)0x2009C01C)
#define	FIO0CLRL	(*(volatile uint16_t*)0x2009C01C)
#define	FIO0CLRH	(*(volatile uint16_t*)0x2009C01E)
#define	FIO0CLR0	(*(volatile uint8_t*)0x2009C01C)
#define	FIO0CLR1	(*(volatile uint8_t*)0x2009C01D)
#define	FIO0CLR2	(*(volatile uint8_t*)0x2009C01E)
#define	FIO0CLR3	(*(volatile uint8_t*)0x2009C01F)

#define	FIO1CLR		(*(volatile uint32_t*)0x2009C03C)
#define	FIO1CLRL	(*(volatile uint16_t*)0x2009C03C)
#define	FIO1CLRH	(*(volatile uint16_t*)0x2009C03E)
#define	FIO1CLR0	(*(volatile uint8_t*)0x2009C03C)
#define	FIO1CLR1	(*(volatile uint8_t*)0x2009C03D)
#define	FIO1CLR2	(*(volatile uint8_t*)0x2009C03E)
#define	FIO1CLR3	(*(volatile uint8_t*)0x2009C03F)

#define	FIO2CLR		(*(volatile uint32_t*)0x2009C05C)
#define	FIO2CLRL	(*(volatile uint16_t*)0x2009C05C)
#define	FIO2CLRH	(*(volatile uint16_t*)0x2009C05E)
#define	FIO2CLR0	(*(volatile uint8_t*)0x2009C05C)
#define	FIO2CLR1	(*(volatile uint8_t*)0x2009C05D)
#define	FIO2CLR2	(*(volatile uint8_t*)0x2009C05E)
#define	FIO2CLR3	(*(volatile uint8_t*)0x2009C05F)

#define	FIO3CLR		(*(volatile uint32_t*)0x2009C07C)
#define	FIO3CLRL	(*(volatile uint16_t*)0x2009C07C)
#define	FIO3CLRH	(*(volatile uint16_t*)0x2009C07E)
#define	FIO3CLR0	(*(volatile uint8_t*)0x2009C07C)
#define	FIO3CLR1	(*(volatile uint8_t*)0x2009C07D)
#define	FIO3CLR2	(*(volatile uint8_t*)0x2009C07E)
#define	FIO3CLR3	(*(volatile uint8_t*)0x2009C07F)

#define	FIO4CLR		(*(volatile uint32_t*)0x2009C09C)
#define	FIO4CLRL	(*(volatile uint16_t*)0x2009C09C)
#define	FIO4CLRH	(*(volatile uint16_t*)0x2009C09E)
#define	FIO4CLR0	(*(volatile uint8_t*)0x2009C09C)
#define	FIO4CLR1	(*(volatile uint8_t*)0x2009C09D)
#define	FIO4CLR2	(*(volatile uint8_t*)0x2009C09E)
#define	FIO4CLR3	(*(volatile uint8_t*)0x2009C09F)

#define	IOIntStatus	(*(volatile uint32_t*)0x40028080)
#define	IO0IntStatR	(*(volatile uint32_t*)0x40028084)
#define	IO0IntStatF	(*(volatile uint32_t*)0x40028088)
#define	IO0IntClr	(*(volatile uint32_t*)0x4002808C)
#define	IO0IntEnR	(*(volatile uint32_t*)0x40028090)
#define	IO0IntEnF	(*(volatile uint32_t*)0x40028094)
#define	IO2IntStatR	(*(volatile uint32_t*)0x400280A4)
#define	IO2IntStatF	(*(volatile uint32_t*)0x400280A8)
#define	IO2IntClr	(*(volatile uint32_t*)0x400280AC)
#define	IO2IntEnR	(*(volatile uint32_t*)0x400280B0)
#define	IO2IntEnF	(*(volatile uint32_t*)0x400280B4)

/* Ethernet MAC */
#define	MAC1			(*(volatile uint32_t*)0x50000000)
#define	MAC2			(*(volatile uint32_t*)0x50000004)
#define	IPGT			(*(volatile uint32_t*)0x50000008)
#define	IPGR			(*(volatile uint32_t*)0x5000000C)
#define	CLRT			(*(volatile uint32_t*)0x50000010)
#define	MAXF			(*(volatile uint32_t*)0x50000014)
#define	SUPP			(*(volatile uint32_t*)0x50000018)
#define	TEST			(*(volatile uint32_t*)0x5000001C)
#define	MCFG			(*(volatile uint32_t*)0x50000020)
#define	MCMD			(*(volatile uint32_t*)0x50000024)
#define	MADR			(*(volatile uint32_t*)0x50000028)
#define	MWTD			(*(volatile uint32_t*)0x5000002C)
#define	MRDD			(*(volatile uint32_t*)0x50000030)
#define	MIND			(*(volatile uint32_t*)0x50000034)
#define	SA0				(*(volatile uint32_t*)0x50000040)
#define	SA1				(*(volatile uint32_t*)0x50000044)
#define	SA2				(*(volatile uint32_t*)0x50000048)
#define	Command			(*(volatile uint32_t*)0x50000100)
#define	Status			(*(volatile uint32_t*)0x50000104)
#define	RxDescriptor	(*(volatile uint32_t*)0x50000108)
#define	RxStatus		(*(volatile uint32_t*)0x5000010C)
#define	RxDescriptorNumber	(*(volatile uint32_t*)0x50000110)
#define	RxProduceIndex	(*(volatile uint32_t*)0x50000114)
#define	RxConsumeIndex	(*(volatile uint32_t*)0x50000118)
#define	TxDescriptor	(*(volatile uint32_t*)0x5000011C)
#define	TxStatus		(*(volatile uint32_t*)0x50000120)
#define	TxDescriptorNumber	(*(volatile uint32_t*)0x50000124)
#define	TxProduceIndex	(*(volatile uint32_t*)0x50000128)
#define	TxConsumeIndex	(*(volatile uint32_t*)0x5000012C)
#define	TSV0			(*(volatile uint32_t*)0x50000158)
#define	TSV1			(*(volatile uint32_t*)0x5000015C)
#define	RSV				(*(volatile uint32_t*)0x50000160)
#define	FlowControlCounter	(*(volatile uint32_t*)0x50000170)
#define	FlowControlStatus	(*(volatile uint32_t*)0x50000174)
#define	RxFliterCtrl		(*(volatile uint32_t*)0x50000200)
#define	RxFilterWoLStatus	(*(volatile uint32_t*)0x50000204)
#define	RxFilterWoLClear	(*(volatile uint32_t*)0x50000208)
#define	HashFilterL		(*(volatile uint32_t*)0x50000210)
#define	HashFilterH		(*(volatile uint32_t*)0x50000214)
#define	IntStatus		(*(volatile uint32_t*)0x50000FE0)
#define	IntEnable		(*(volatile uint32_t*)0x50000FE4)
#define	IntClear		(*(volatile uint32_t*)0x50000FE8)
#define	IntSet			(*(volatile uint32_t*)0x50000FEC)
#define	PowerDown		(*(volatile uint32_t*)0x50000FF4)

/* USB Device */
#define	USBClkCtrl		(*(volatile uint32_t*)0x5000CFF4)
#define	USBClkSt		(*(volatile uint32_t*)0x5000CFF8)
#define	USBIntSt		(*(volatile uint32_t*)0x400FC1C0)
#define	USBDevIntSt		(*(volatile uint32_t*)0x5000C200)
#define	USBDevIntEn		(*(volatile uint32_t*)0x5000C204)
#define	USBDevIntClr	(*(volatile uint32_t*)0x5000C208)
#define	USBDevIntSet	(*(volatile uint32_t*)0x5000C20C)
#define	USBDevIntPri	(*(volatile uint32_t*)0x5000C22C)
#define	USBEpIntSt		(*(volatile uint32_t*)0x5000C230)
#define	USBEpIntEn		(*(volatile uint32_t*)0x5000C234)
#define	USBEpIntClr		(*(volatile uint32_t*)0x5000C238)
#define	USBEpIntSet		(*(volatile uint32_t*)0x5000C23C)
#define	USBEpIntPri		(*(volatile uint32_t*)0x5000C240)
#define	USBReEp			(*(volatile uint32_t*)0x5000C244)
#define	USBEpIn			(*(volatile uint32_t*)0x5000C248)
#define	USBMaxPSize		(*(volatile uint32_t*)0x5000C24C)
#define	USBRxData		(*(volatile uint32_t*)0x5000C218)
#define	USBRxPLen		(*(volatile uint32_t*)0x5000C220)
#define	USBTxData		(*(volatile uint32_t*)0x5000C21C)
#define	USBTxPLen		(*(volatile uint32_t*)0x5000C224)
#define	USBCtrl			(*(volatile uint32_t*)0x5000C228)
#define	USBCmdCode		(*(volatile uint32_t*)0x5000C210)
#define	USBCmdData		(*(volatile uint32_t*)0x5000C214)
#define	USBDMARSt		(*(volatile uint32_t*)0x5000C250)
#define	USBDMARClr		(*(volatile uint32_t*)0x5000C254)
#define	USBDMARSet		(*(volatile uint32_t*)0x5000C258)
#define	USBUDCAH		(*(volatile uint32_t*)0x5000C280)
#define	USBEpDMASt		(*(volatile uint32_t*)0x5000C284)
#define	USBEpDMAEn		(*(volatile uint32_t*)0x5000C288)
#define	USBEpDMADis		(*(volatile uint32_t*)0x5000C28C)
#define	USBDMAIntSt		(*(volatile uint32_t*)0x5000C290)
#define	USBDMAIntEn		(*(volatile uint32_t*)0x5000C294)
#define	USBEoTIntSt		(*(volatile uint32_t*)0x5000C2A0)
#define	USBEoTIntClr	(*(volatile uint32_t*)0x5000C2A4)
#define	USBEoTIntSet	(*(volatile uint32_t*)0x5000C2A8)
#define	USBNDDRIntSt	(*(volatile uint32_t*)0x5000C2AC)
#define	USBNDDRIntClr	(*(volatile uint32_t*)0x5000C2B0)
#define	USBNDDRIntSet	(*(volatile uint32_t*)0x5000C2B4)
#define	USBSysErrIntSt	(*(volatile uint32_t*)0x5000C2B8)
#define	USBSysErrIntClr	(*(volatile uint32_t*)0x5000C2BC)
#define	USBSysErrIntSet	(*(volatile uint32_t*)0x5000C2C0)

/* USB OTG */
#define	USBIntSt	(*(volatile uint32_t*)0x400FC1C0)
#define	OTGIntSt	(*(volatile uint32_t*)0x5000C100)
#define	OTGIntEn	(*(volatile uint32_t*)0x5000C104)
#define	OTGIntSet	(*(volatile uint32_t*)0x5000C108)
#define	OTGIntClr	(*(volatile uint32_t*)0x5000C10C)
#define	OTGStCtrl	(*(volatile uint32_t*)0x5000C110)
#define	OTGTmr		(*(volatile uint32_t*)0x5000C114)
#define	I2C_RX		(*(volatile uint32_t*)0x5000C300)
#define	I2C_TX		(*(volatile uint32_t*)0x5000C300)
#define	I2C_STS		(*(volatile uint32_t*)0x5000C304)
#define	I2C_CTL		(*(volatile uint32_t*)0x5000C308)
#define	I2C_CLKHI	(*(volatile uint32_t*)0x5000C30C)
#define	I2C_CLKLO	(*(volatile uint32_t*)0x5000C310)
#define	OTGClkCtrl	(*(volatile uint32_t*)0x5000CFF4)
#define	OTGClkSt	(*(volatile uint32_t*)0x5000CFF8)

/* UART0,UART1,UART2,UART3 */
#define	U0RBR		(*(volatile uint32_t*)0x4000C000)
#define	U0THR		(*(volatile uint32_t*)0x4000C000)
#define	U0DLL		(*(volatile uint32_t*)0x4000C000)
#define	U0DLM		(*(volatile uint32_t*)0x4000C004)
#define	U0IER		(*(volatile uint32_t*)0x4000C004)
#define	U0IIR		(*(volatile uint32_t*)0x4000C008)
#define	U0FCR		(*(volatile uint32_t*)0x4000C008)
#define	U0LCR		(*(volatile uint32_t*)0x4000C00C)
#define	U0LSR		(*(volatile uint32_t*)0x4000C014)
#define	U0SCR		(*(volatile uint32_t*)0x4000C01C)
#define	U0ACR		(*(volatile uint32_t*)0x4000C020)
#define	U0ICR		(*(volatile uint32_t*)0x4000C024)
#define	U0FDR		(*(volatile uint32_t*)0x4000C028)
#define	U0TER		(*(volatile uint32_t*)0x4000C030)

#define	U1RBR		(*(volatile uint32_t*)0x40010000)
#define	U1THR		(*(volatile uint32_t*)0x40010000)
#define	U1DLL		(*(volatile uint32_t*)0x40010000)
#define	U1DLM		(*(volatile uint32_t*)0x40010004)
#define	U1IER		(*(volatile uint32_t*)0x40010004)
#define	U1IIR		(*(volatile uint32_t*)0x40010008)
#define	U1FCR		(*(volatile uint32_t*)0x40010008)
#define	U1LCR		(*(volatile uint32_t*)0x4001000C)
#define	U1MCR		(*(volatile uint32_t*)0x40010010)
#define	U1LSR		(*(volatile uint32_t*)0x40010014)
#define	U1MSR		(*(volatile uint32_t*)0x40010018)
#define	U1SCR		(*(volatile uint32_t*)0x4001001C)
#define	U1ACR		(*(volatile uint32_t*)0x40010020)
#define	U1FDR		(*(volatile uint32_t*)0x40010028)
#define	U1TER		(*(volatile uint32_t*)0x40010030)
#define	U1RS485CTRL	(*(volatile uint32_t*)0x4001004C)
#define	U1ADRMATCH	(*(volatile uint32_t*)0x40010050)
#define	U1RS485DLY	(*(volatile uint32_t*)0x40010054)

#define	U2RBR		(*(volatile uint32_t*)0x40098000)
#define	U2THR		(*(volatile uint32_t*)0x40098000)
#define	U2DLL		(*(volatile uint32_t*)0x40098000)
#define	U2DLM		(*(volatile uint32_t*)0x40098004)
#define	U2IER		(*(volatile uint32_t*)0x40098004)
#define	U2IIR		(*(volatile uint32_t*)0x40098008)
#define	U2FCR		(*(volatile uint32_t*)0x40098008)
#define	U2LCR		(*(volatile uint32_t*)0x4009800C)
#define	U2LSR		(*(volatile uint32_t*)0x40098014)
#define	U2SCR		(*(volatile uint32_t*)0x4009801C)
#define	U2ACR		(*(volatile uint32_t*)0x40098020)
#define	U2ICR		(*(volatile uint32_t*)0x40098024)
#define	U2FDR		(*(volatile uint32_t*)0x40098028)
#define	U2TER		(*(volatile uint32_t*)0x40098030)

#define	U3RBR		(*(volatile uint32_t*)0x4009C000)
#define	U3THR		(*(volatile uint32_t*)0x4009C000)
#define	U3DLL		(*(volatile uint32_t*)0x4009C000)
#define	U3DLM		(*(volatile uint32_t*)0x4009C004)
#define	U3IER		(*(volatile uint32_t*)0x4009C004)
#define	U3IIR		(*(volatile uint32_t*)0x4009C008)
#define	U3FCR		(*(volatile uint32_t*)0x4009C008)
#define	U3LCR		(*(volatile uint32_t*)0x4009C00C)
#define	U3LSR		(*(volatile uint32_t*)0x4009C014)
#define	U3SCR		(*(volatile uint32_t*)0x4009C01C)
#define	U3ACR		(*(volatile uint32_t*)0x4009C020)
#define	U3ICR		(*(volatile uint32_t*)0x4009C024)
#define	U3FDR		(*(volatile uint32_t*)0x4009C028)
#define	U3TER		(*(volatile uint32_t*)0x4009C030)

/* CAN1,CAN2 */
#define	AFMR		(*(volatile uint32_t*)0x4003C000)
#define	SFF_sa		(*(volatile uint32_t*)0x4003C004)
#define	SFF_GRP_sa	(*(volatile uint32_t*)0x4003C008)
#define	EFF_sa		(*(volatile uint32_t*)0x4003C00C)
#define	EFF_GRP_sa	(*(volatile uint32_t*)0x4003C010)
#define	ENDofTable	(*(volatile uint32_t*)0x4003C014)
#define	LUTerrAd	(*(volatile uint32_t*)0x4003C018)
#define	LUTerr		(*(volatile uint32_t*)0x4003C01C)

#define	CANTxSR		(*(volatile uint32_t*)0x40040000)
#define	CANRxSR		(*(volatile uint32_t*)0x40040004)
#define	CANMSR		(*(volatile uint32_t*)0x40040008)
#define	CAN1MOD		(*(volatile uint32_t*)0x40044000)
#define	CAN1CMR		(*(volatile uint32_t*)0x40044004)
#define	CAN1GSR		(*(volatile uint32_t*)0x40044008)
#define	CAN1ICR		(*(volatile uint32_t*)0x4004400C)
#define	CAN1IER		(*(volatile uint32_t*)0x40044010)
#define	CAN1BTR		(*(volatile uint32_t*)0x40044014)
#define	CAN1EWL		(*(volatile uint32_t*)0x40044018)
#define	CAN1SR		(*(volatile uint32_t*)0x4004401C)
#define	CAN1RFS		(*(volatile uint32_t*)0x40044020)
#define	CAN1RID		(*(volatile uint32_t*)0x40044024)
#define	CAN1RDA		(*(volatile uint32_t*)0x40044028)
#define	CAN1RDB		(*(volatile uint32_t*)0x4004402C)
#define	CAN1TFI1	(*(volatile uint32_t*)0x40044030)
#define	CAN1TID1	(*(volatile uint32_t*)0x40044034)
#define	CAN1TDA1	(*(volatile uint32_t*)0x40044038)
#define	CAN1TDB1	(*(volatile uint32_t*)0x4004403C)
#define	CAN1TFI2	(*(volatile uint32_t*)0x40044040)
#define	CAN1TID2	(*(volatile uint32_t*)0x40044044)
#define	CAN1TDA2	(*(volatile uint32_t*)0x40044048)
#define	CAN1TDB2	(*(volatile uint32_t*)0x4004404C)
#define	CAN1TFI3	(*(volatile uint32_t*)0x40044050)
#define	CAN1TID3	(*(volatile uint32_t*)0x40044054)
#define	CAN1TDA3	(*(volatile uint32_t*)0x40044058)
#define	CAN1TDB3	(*(volatile uint32_t*)0x4004405C)

#define	CAN2MOD		(*(volatile uint32_t*)0x40048000)
#define	CAN2CMR		(*(volatile uint32_t*)0x40048004)
#define	CAN2GSR		(*(volatile uint32_t*)0x40048008)
#define	CAN2ICR		(*(volatile uint32_t*)0x4004800C)
#define	CAN2IER		(*(volatile uint32_t*)0x40048010)
#define	CAN2BTR		(*(volatile uint32_t*)0x40048014)
#define	CAN2EWL		(*(volatile uint32_t*)0x40048018)
#define	CAN2SR		(*(volatile uint32_t*)0x4004801C)
#define	CAN2RFS		(*(volatile uint32_t*)0x40048020)
#define	CAN2RID		(*(volatile uint32_t*)0x40048024)
#define	CAN2RDA		(*(volatile uint32_t*)0x40048028)
#define	CAN2RDB		(*(volatile uint32_t*)0x4004802C)
#define	CAN2TFI1	(*(volatile uint32_t*)0x40048030)
#define	CAN2TID1	(*(volatile uint32_t*)0x40048034)
#define	CAN2TDA1	(*(volatile uint32_t*)0x40048038)
#define	CAN2TDB1	(*(volatile uint32_t*)0x4004803C)
#define	CAN2TFI2	(*(volatile uint32_t*)0x40048040)
#define	CAN2TID2	(*(volatile uint32_t*)0x40048044)
#define	CAN2TDA2	(*(volatile uint32_t*)0x40048048)
#define	CAN2TDB2	(*(volatile uint32_t*)0x4004804C)
#define	CAN2TFI3	(*(volatile uint32_t*)0x40048050)
#define	CAN2TID3	(*(volatile uint32_t*)0x40048054)
#define	CAN2TDA3	(*(volatile uint32_t*)0x40048058)
#define	CAN2TDB3	(*(volatile uint32_t*)0x4004805C)

/* SPI0 */
#define	S0SPCR		(*(volatile uint32_t*)0x40020000)
#define	S0SPSR		(*(volatile uint32_t*)0x40020004)
#define	S0SPDR		(*(volatile uint32_t*)0x40020008)
#define	S0SPCCR		(*(volatile uint32_t*)0x4002000C)
#define	S0SPINT		(*(volatile uint32_t*)0x4002001C)

/* SSP0,SSP1 */
#define	SSP0CR0		(*(volatile uint32_t*)0x40088000)
#define	SSP0CR1		(*(volatile uint32_t*)0x40088004)
#define	SSP0DR		(*(volatile uint32_t*)0x40088008)
#define	SSP0SR		(*(volatile uint32_t*)0x4008800C)
#define	SSP0CPSR	(*(volatile uint32_t*)0x40088010)
#define	SSP0IMSC	(*(volatile uint32_t*)0x40088014)
#define	SSP0RIS		(*(volatile uint32_t*)0x40088018)
#define	SSP0MIS		(*(volatile uint32_t*)0x4008801C)
#define	SSP0ICR		(*(volatile uint32_t*)0x40088020)
#define	SSP0DMACR	(*(volatile uint32_t*)0x40088024)
#define	SSP1CR0		(*(volatile uint32_t*)0x40030000)
#define	SSP1CR1		(*(volatile uint32_t*)0x40030004)
#define	SSP1DR		(*(volatile uint32_t*)0x40030008)
#define	SSP1SR		(*(volatile uint32_t*)0x4003000C)
#define	SSP1CPSR	(*(volatile uint32_t*)0x40030010)
#define	SSP1IMSC	(*(volatile uint32_t*)0x40030014)
#define	SSP1RIS		(*(volatile uint32_t*)0x40030018)
#define	SSP1MIS		(*(volatile uint32_t*)0x4003001C)
#define	SSP1ICR		(*(volatile uint32_t*)0x40030020)
#define	SSP1DMACR	(*(volatile uint32_t*)0x40030024)

/* I2C0,I2C1,I2C2 */
#define	I2C0CONSET		(*(volatile uint32_t*)0x4001C000)
#define	I2C0STAT		(*(volatile uint32_t*)0x4001C004)
#define	I2C0DAT			(*(volatile uint32_t*)0x4001C008)
#define	I2C0ADR0		(*(volatile uint32_t*)0x4001C00C)
#define	I2C0SCLH		(*(volatile uint32_t*)0x4001C010)
#define	I2C0SCLL		(*(volatile uint32_t*)0x4001C014)
#define	I2C0CONCLR		(*(volatile uint32_t*)0x4001C018)
#define	I2C0MMCTRL		(*(volatile uint32_t*)0x4001C01C)
#define	I2C0ADR1		(*(volatile uint32_t*)0x4001C020)
#define	I2C0ADR2		(*(volatile uint32_t*)0x4001C024)
#define	I2C0ADR3		(*(volatile uint32_t*)0x4001C028)
#define	I2C0DATA_BUFFER	(*(volatile uint32_t*)0x4001C02C)
#define	I2C0MASK		( (volatile uint32_t*)0x4001C030)
#define	I2C0MASK0		(*(volatile uint32_t*)0x4001C030)
#define	I2C0MASK1		(*(volatile uint32_t*)0x4001C034)
#define	I2C0MASK2		(*(volatile uint32_t*)0x4001C038)
#define	I2C0MASK3		(*(volatile uint32_t*)0x4001C03C)
#define	I2C1CONSET		(*(volatile uint32_t*)0x4005C000)
#define	I2C1STAT		(*(volatile uint32_t*)0x4005C004)
#define	I2C1DAT			(*(volatile uint32_t*)0x4005C008)
#define	I2C1ADR0		(*(volatile uint32_t*)0x4005C00C)
#define	I2C1SCLH		(*(volatile uint32_t*)0x4005C010)
#define	I2C1SCLL		(*(volatile uint32_t*)0x4005C014)
#define	I2C1CONCLR		(*(volatile uint32_t*)0x4005C018)
#define	I2C1MMCTRL		(*(volatile uint32_t*)0x4005C01C)
#define	I2C1ADR1		(*(volatile uint32_t*)0x4005C020)
#define	I2C1ADR2		(*(volatile uint32_t*)0x4005C024)
#define	I2C1ADR3		(*(volatile uint32_t*)0x4005C028)
#define	I2C1DATA_BUFFER	(*(volatile uint32_t*)0x4005C02C)
#define	I2C1MASK		( (volatile uint32_t*)0x4005C030)
#define	I2C1MASK0		(*(volatile uint32_t*)0x4005C030)
#define	I2C1MASK1		(*(volatile uint32_t*)0x4005C034)
#define	I2C1MASK2		(*(volatile uint32_t*)0x4005C038)
#define	I2C1MASK3		(*(volatile uint32_t*)0x4005C03C)
#define	I2C2CONSET		(*(volatile uint32_t*)0x400A0000)
#define	I2C2STAT		(*(volatile uint32_t*)0x400A0004)
#define	I2C2DAT			(*(volatile uint32_t*)0x400A0008)
#define	I2C2ADR0		(*(volatile uint32_t*)0x400A000C)
#define	I2C2SCLH		(*(volatile uint32_t*)0x400A0010)
#define	I2C2SCLL		(*(volatile uint32_t*)0x400A0014)
#define	I2C2CONCLR		(*(volatile uint32_t*)0x400A0018)
#define	I2C2MMCTRL		(*(volatile uint32_t*)0x400A001C)
#define	I2C2ADR1		(*(volatile uint32_t*)0x400A0020)
#define	I2C2ADR2		(*(volatile uint32_t*)0x400A0024)
#define	I2C2ADR3		(*(volatile uint32_t*)0x400A0028)
#define	I2C2DATA_BUFFER	(*(volatile uint32_t*)0x400A002C)
#define	I2C2MASK		( (volatile uint32_t*)0x400A0030)
#define	I2C2MASK0		(*(volatile uint32_t*)0x400A0030)
#define	I2C2MASK1		(*(volatile uint32_t*)0x400A0034)
#define	I2C2MASK2		(*(volatile uint32_t*)0x400A0038)
#define	I2C2MASK3		(*(volatile uint32_t*)0x400A003C)

/* I2S */
#define	I2SDAO		(*(volatile uint32_t*)0x400A8000)
#define	I2SDAI		(*(volatile uint32_t*)0x400A8004)
#define	I2STXFIFO	(*(volatile uint32_t*)0x400A8008)
#define	I2SRXFIFO	(*(volatile uint32_t*)0x400A800C)
#define	I2SSTATE	(*(volatile uint32_t*)0x400A8010)
#define	I2SDMA1		(*(volatile uint32_t*)0x400A8014)
#define	I2SDMA2		(*(volatile uint32_t*)0x400A8018)
#define	I2SIRQ		(*(volatile uint32_t*)0x400A801C)
#define	I2STXRATE	(*(volatile uint32_t*)0x400A8020)
#define	I2SRXRATE	(*(volatile uint32_t*)0x400A8024)
#define	I2STXBITRATE	(*(volatile uint32_t*)0x400A8028)
#define	I2SRXBITRATE	(*(volatile uint32_t*)0x400A802C)
#define	I2STXMODE	(*(volatile uint32_t*)0x400A8030)
#define	I2SRXMODE	(*(volatile uint32_t*)0x400A8034)

/* Timer0,Timer1,Timer2,Timer3 */
#define	T0IR		(*(volatile uint32_t*)0x40004000)
#define	T0TCR		(*(volatile uint32_t*)0x40004004)
#define	T0TC		(*(volatile uint32_t*)0x40004008)
#define	T0PR		(*(volatile uint32_t*)0x4000400C)
#define	T0PC		(*(volatile uint32_t*)0x40004010)
#define	T0MCR		(*(volatile uint32_t*)0x40004014)
#define	T0MR		( (volatile uint32_t*)0x40004018)
#define	T0MR0		(*(volatile uint32_t*)0x40004018)
#define	T0MR1		(*(volatile uint32_t*)0x4000401C)
#define	T0MR2		(*(volatile uint32_t*)0x40004020)
#define	T0MR3		(*(volatile uint32_t*)0x40004024)
#define	T0CCR		(*(volatile uint32_t*)0x40004028)
#define	T0CR0		(*(volatile uint32_t*)0x4000402C)
#define	T0CR1		(*(volatile uint32_t*)0x40004030)
#define	T0EMR		(*(volatile uint32_t*)0x4000403C)
#define	T0CTCR		(*(volatile uint32_t*)0x40004070)

#define	T1IR		(*(volatile uint32_t*)0x40008000)
#define	T1TCR		(*(volatile uint32_t*)0x40008004)
#define	T1TC		(*(volatile uint32_t*)0x40008008)
#define	T1PR		(*(volatile uint32_t*)0x4000800C)
#define	T1PC		(*(volatile uint32_t*)0x40008010)
#define	T1MCR		(*(volatile uint32_t*)0x40008014)
#define	T1MR		( (volatile uint32_t*)0x40008018)
#define	T1MR0		(*(volatile uint32_t*)0x40008018)
#define	T1MR1		(*(volatile uint32_t*)0x4000801C)
#define	T1MR2		(*(volatile uint32_t*)0x40008020)
#define	T1MR3		(*(volatile uint32_t*)0x40008024)
#define	T1CCR		(*(volatile uint32_t*)0x40008028)
#define	T1CR0		(*(volatile uint32_t*)0x4000802C)
#define	T1CR1		(*(volatile uint32_t*)0x40008030)
#define	T1EMR		(*(volatile uint32_t*)0x4000803C)
#define	T1CTCR		(*(volatile uint32_t*)0x40008070)

#define	T2IR		(*(volatile uint32_t*)0x40090000)
#define	T2TCR		(*(volatile uint32_t*)0x40090004)
#define	T2TC		(*(volatile uint32_t*)0x40090008)
#define	T2PR		(*(volatile uint32_t*)0x4009000C)
#define	T2PC		(*(volatile uint32_t*)0x40090010)
#define	T2MCR		(*(volatile uint32_t*)0x40090014)
#define	T2MR		( (volatile uint32_t*)0x40090018)
#define	T2MR0		(*(volatile uint32_t*)0x40090018)
#define	T2MR1		(*(volatile uint32_t*)0x4009001C)
#define	T2MR2		(*(volatile uint32_t*)0x40090020)
#define	T2MR3		(*(volatile uint32_t*)0x40090024)
#define	T2CCR		(*(volatile uint32_t*)0x40090028)
#define	T2CR0		(*(volatile uint32_t*)0x4009002C)
#define	T2CR1		(*(volatile uint32_t*)0x40090030)
#define	T2EMR		(*(volatile uint32_t*)0x4009003C)
#define	T2CTCR		(*(volatile uint32_t*)0x40090070)

#define	T3IR		(*(volatile uint32_t*)0x40094000)
#define	T3TCR		(*(volatile uint32_t*)0x40094004)
#define	T3TC		(*(volatile uint32_t*)0x40094008)
#define	T3PR		(*(volatile uint32_t*)0x4009400C)
#define	T3PC		(*(volatile uint32_t*)0x40094010)
#define	T3MCR		(*(volatile uint32_t*)0x40094014)
#define	T3MR		( (volatile uint32_t*)0x40094018)
#define	T3MR0		(*(volatile uint32_t*)0x40094018)
#define	T3MR1		(*(volatile uint32_t*)0x4009401C)
#define	T3MR2		(*(volatile uint32_t*)0x40094020)
#define	T3MR3		(*(volatile uint32_t*)0x40094024)
#define	T3CCR		(*(volatile uint32_t*)0x40094028)
#define	T3CR0		(*(volatile uint32_t*)0x4009402C)
#define	T3CR1		(*(volatile uint32_t*)0x40094030)
#define	T3EMR		(*(volatile uint32_t*)0x4009403C)
#define	T3CTCR		(*(volatile uint32_t*)0x40094070)

/* Repeative Interrupt Timer */
#define	RICOMPVAL	(*(volatile uint32_t*)0x400B0000)
#define	RIMASK		(*(volatile uint32_t*)0x400B0004)
#define	RICTRL		(*(volatile uint32_t*)0x400B0008)
#define	RICOUNTER	(*(volatile uint32_t*)0x400B000C)

/* PWM1 */
#define	PWM1IR		(*(volatile uint32_t*)0x40018000)
#define	PWM1TCR		(*(volatile uint32_t*)0x40018004)
#define	PWM1TC		(*(volatile uint32_t*)0x40018008)
#define	PWM1PR		(*(volatile uint32_t*)0x4001800C)
#define	PWM1PC		(*(volatile uint32_t*)0x40018010)
#define	PWM1MCR		(*(volatile uint32_t*)0x40018014)
#define	PWM1MR0		(*(volatile uint32_t*)0x40018018)
#define	PWM1MR1		(*(volatile uint32_t*)0x4001801C)
#define	PWM1MR2		(*(volatile uint32_t*)0x40018020)
#define	PWM1MR3		(*(volatile uint32_t*)0x40018024)
#define	PWM1CCR		(*(volatile uint32_t*)0x40018028)
#define	PWM1CR0		(*(volatile uint32_t*)0x4001802C)
#define	PWM1CR1		(*(volatile uint32_t*)0x40018030)
#define	PWM1CR2		(*(volatile uint32_t*)0x40018034)
#define	PWM1CR3		(*(volatile uint32_t*)0x40018038)
#define	PWM1MR4		(*(volatile uint32_t*)0x40018040)
#define	PWM1MR5		(*(volatile uint32_t*)0x40018044)
#define	PWM1MR6		(*(volatile uint32_t*)0x40018048)
#define	PWM1PCR		(*(volatile uint32_t*)0x4001804C)
#define	PWM1LER		(*(volatile uint32_t*)0x40018050)
#define	PWM1CTCR	(*(volatile uint32_t*)0x40018070)

/* Motor Control PWM */
#define	MCCON		(*(volatile uint32_t*)0x400B8000)
#define	MCCON_SET	(*(volatile uint32_t*)0x400B8004)
#define	MCCON_CLR	(*(volatile uint32_t*)0x400B8008)
#define	MCCAPCON	(*(volatile uint32_t*)0x400B800C)
#define	MCCAPCON_SET	(*(volatile uint32_t*)0x400B8010)
#define	MCCAPCON_CLR	(*(volatile uint32_t*)0x400B8014)
#define	MCTC		( (volatile uint32_t*)0x400B8018)
#define	MCTC0		(*(volatile uint32_t*)0x400B8018)
#define	MCTC1		(*(volatile uint32_t*)0x400B801C)
#define	MCTC2		(*(volatile uint32_t*)0x400B8020)
#define	MCLIM		( (volatile uint32_t*)0x400B8024)
#define	MCLIM0		(*(volatile uint32_t*)0x400B8024)
#define	MCLIM1		(*(volatile uint32_t*)0x400B8028)
#define	MCLIM2		(*(volatile uint32_t*)0x400B802C)
#define	MCMAT		( (volatile uint32_t*)0x400B8030)
#define	MCMAT0		(*(volatile uint32_t*)0x400B8030)
#define	MCMAT1		(*(volatile uint32_t*)0x400B8034)
#define	MCMAT2		(*(volatile uint32_t*)0x400B8038)
#define	MCDT		(*(volatile uint32_t*)0x400B803C)
#define	MCCP		(*(volatile uint32_t*)0x400B8040)
#define	MCCAP		( (volatile uint32_t*)0x400B8044)
#define	MCCAP0		(*(volatile uint32_t*)0x400B8044)
#define	MCCAP1		(*(volatile uint32_t*)0x400B8048)
#define	MCCAP2		(*(volatile uint32_t*)0x400B804C)
#define	MCINTEN		(*(volatile uint32_t*)0x400B8050)
#define	MCINTEN_SET	(*(volatile uint32_t*)0x400B8054)
#define	MCINTEN_CLR	(*(volatile uint32_t*)0x400B8058)
#define	MCCNTCON	(*(volatile uint32_t*)0x400B805C)
#define	MCCNTCON_SET	(*(volatile uint32_t*)0x400B8060)
#define	MCCNTCON_CLR	(*(volatile uint32_t*)0x400B8064)
#define	MCINTF		(*(volatile uint32_t*)0x400B8068)
#define	MCINTF_SET	(*(volatile uint32_t*)0x400B806C)
#define	MCINTF_CLR	(*(volatile uint32_t*)0x400B8070)
#define	MCCAP_CLR	(*(volatile uint32_t*)0x400B8074)

/* Quadrature Encoder Interface */
#define	QEICON		(*(volatile uint32_t*)0x400BC000)
#define	QEICONF		(*(volatile uint32_t*)0x400BC008)
#define	QEISTAT		(*(volatile uint32_t*)0x400BC004)
#define	QEIPOS		(*(volatile uint32_t*)0x400BC00C)
#define	QEIMAXPOS	(*(volatile uint32_t*)0x400BC010)
#define	CMPOS0		(*(volatile uint32_t*)0x400BC014)
#define	CMPOS1		(*(volatile uint32_t*)0x400BC018)
#define	CMPOS2		(*(volatile uint32_t*)0x400BC01C)
#define	INXCNT		(*(volatile uint32_t*)0x400BC020)
#define	INXCMP		(*(volatile uint32_t*)0x400BC024)
#define	QEILOAD		(*(volatile uint32_t*)0x400BC028)
#define	QEITIME		(*(volatile uint32_t*)0x400BC02C)
#define	QEIVEL		(*(volatile uint32_t*)0x400BC030)
#define	QEICAP		(*(volatile uint32_t*)0x400BC034)
#define	VELCOMP		(*(volatile uint32_t*)0x400BC038)
#define	FILTER		(*(volatile uint32_t*)0x400BC03C)
#define	QEIINTSTAT	(*(volatile uint32_t*)0x400BCFE0)
#define	QEISET		(*(volatile uint32_t*)0x400BCFEC)
#define	QEICLR		(*(volatile uint32_t*)0x400BCFE8)
#define	QEIIE		(*(volatile uint32_t*)0x400BCFE4)
#define	QEIIES		(*(volatile uint32_t*)0x400BCFDC)
#define	QEIIEC		(*(volatile uint32_t*)0x400BCFD8)

/* RTC */
#define	RTC_ILR		(*(volatile uint32_t*)0x40024000)
#define	RTC_CCR		(*(volatile uint32_t*)0x40024008)
#define	RTC_CIIR	(*(volatile uint32_t*)0x4002400C)
#define	RTC_AMR		(*(volatile uint32_t*)0x40024010)
#define	RTC_AUX		(*(volatile uint32_t*)0x4002405C)
#define	RTC_AUXEN	(*(volatile uint32_t*)0x40024058)
#define	RTC_CTIME0	(*(volatile uint32_t*)0x40024014)
#define	RTC_CTIME1	(*(volatile uint32_t*)0x40024018)
#define	RTC_CTIME2	(*(volatile uint32_t*)0x4002401C)
#define	RTC_SEC		(*(volatile uint32_t*)0x40024020)
#define	RTC_MIN		(*(volatile uint32_t*)0x40024024)
#define	RTC_HOUR	(*(volatile uint32_t*)0x40024028)
#define	RTC_DOM		(*(volatile uint32_t*)0x4002402C)
#define	RTC_DOW		(*(volatile uint32_t*)0x40024030)
#define	RTC_DOY		(*(volatile uint32_t*)0x40024034)
#define	RTC_MONTH	(*(volatile uint32_t*)0x40024038)
#define	RTC_YEAR	(*(volatile uint32_t*)0x4002403C)
#define	RTC_CALIBRATION	(*(volatile uint32_t*)0x40024040)
#define	RTC_GPREG	( (volatile uint32_t*)0x40024044)
#define	RTC_GPREG0	(*(volatile uint32_t*)0x40024044)
#define	RTC_GPREG1	(*(volatile uint32_t*)0x40024048)
#define	RTC_GPREG2	(*(volatile uint32_t*)0x4002404C)
#define	RTC_GPREG3	(*(volatile uint32_t*)0x40024050)
#define	RTC_GPREG4	(*(volatile uint32_t*)0x40024054)
#define	RTC_ALSEC	(*(volatile uint32_t*)0x40024060)
#define	RTC_ALMIN	(*(volatile uint32_t*)0x40024064)
#define	RTC_ALHOUR	(*(volatile uint32_t*)0x40024068)
#define	RTC_ALDOM	(*(volatile uint32_t*)0x4002406C)
#define	RTC_ALDOW	(*(volatile uint32_t*)0x40024070)
#define	RTC_ALDOY	(*(volatile uint32_t*)0x40024074)
#define	RTC_ALMON	(*(volatile uint32_t*)0x40024078)
#define	RTC_ALYEAR	(*(volatile uint32_t*)0x4002407C)

/* WDT */
#define	WDMOD		(*(volatile uint32_t*)0x40000000)
#define	WDTC		(*(volatile uint32_t*)0x40000004)
#define	WDFEED		(*(volatile uint32_t*)0x40000008)
#define	WDTV		(*(volatile uint32_t*)0x4000000C)
#define	WDCLKSEL	(*(volatile uint32_t*)0x40000010)

/* ADC0 */
#define	AD0CR		(*(volatile uint32_t*)0x40034000)
#define	AD0GDR		(*(volatile uint32_t*)0x40034004)
#define	AD0INTEN	(*(volatile uint32_t*)0x4003400C)
#define	AD0DR		( (volatile uint32_t*)0x40034010)
#define	AD0DR0		(*(volatile uint32_t*)0x40034010)
#define	AD0DR1		(*(volatile uint32_t*)0x40034014)
#define	AD0DR2		(*(volatile uint32_t*)0x40034018)
#define	AD0DR3		(*(volatile uint32_t*)0x4003401C)
#define	AD0DR4		(*(volatile uint32_t*)0x40034020)
#define	AD0DR5		(*(volatile uint32_t*)0x40034024)
#define	AD0DR6		(*(volatile uint32_t*)0x40034028)
#define	AD0DR7		(*(volatile uint32_t*)0x4003402C)
#define	AD0STAT		(*(volatile uint32_t*)0x40034030)
#define	AD0TRM		(*(volatile uint32_t*)0x40034034)

/* DAC */
#define	DACR		(*(volatile uint32_t*)0x4008C000)
#define	DACCTRL		(*(volatile uint32_t*)0x4008C004)
#define	DACCNTVAL	(*(volatile uint32_t*)0x4008C008)

/* GPDMA */
#define	DMACIntStat		(*(volatile uint32_t*)0x50004000)
#define	DMACIntTCStat	(*(volatile uint32_t*)0x50004004)
#define	DMACIntTCClear	(*(volatile uint32_t*)0x50004008)
#define	DMACIntErrStat	(*(volatile uint32_t*)0x5000400C)
#define	DMACIntErrClr	(*(volatile uint32_t*)0x50004010)
#define	DMACRawIntTCStat	(*(volatile uint32_t*)0x50004014)
#define	DMACRawIntErrStat	(*(volatile uint32_t*)0x50004018)
#define	DMACEnbldChns	(*(volatile uint32_t*)0x5000401C)
#define	DMACSoftBReq	(*(volatile uint32_t*)0x50004020)
#define	DMACSoftSReq	(*(volatile uint32_t*)0x50004024)
#define	DMACSoftLBReq	(*(volatile uint32_t*)0x50004028)
#define	DMACSoftLSReq	(*(volatile uint32_t*)0x5000402C)
#define	DMACConfig		(*(volatile uint32_t*)0x50004030)
#define	DMACSync		(*(volatile uint32_t*)0x50004034)
#define	DMAREQSEL		(*(volatile uint32_t*)0x400FC1C4)
#define	DMACC0SrcAddr	(*(volatile uint32_t*)0x50004100)
#define	DMACC0DestAddr	(*(volatile uint32_t*)0x50004104)
#define	DMACC0LLI		(*(volatile uint32_t*)0x50004108)
#define	DMACC0Control	(*(volatile uint32_t*)0x5000410C)
#define	DMACC0Config	(*(volatile uint32_t*)0x50004110)
#define	DMACC1SrcAddr	(*(volatile uint32_t*)0x50004120)
#define	DMACC1DestAddr	(*(volatile uint32_t*)0x50004124)
#define	DMACC1LLI		(*(volatile uint32_t*)0x50004128)
#define	DMACC1Control	(*(volatile uint32_t*)0x5000412C)
#define	DMACC1Config	(*(volatile uint32_t*)0x50004130)
#define	DMACC2SrcAddr	(*(volatile uint32_t*)0x50004140)
#define	DMACC2DestAddr	(*(volatile uint32_t*)0x50004144)
#define	DMACC2LLI		(*(volatile uint32_t*)0x50004148)
#define	DMACC2Control	(*(volatile uint32_t*)0x5000414C)
#define	DMACC2Config	(*(volatile uint32_t*)0x50004150)
#define	DMACC3SrcAddr	(*(volatile uint32_t*)0x50004160)
#define	DMACC3DestAddr	(*(volatile uint32_t*)0x50004164)
#define	DMACC3LLI		(*(volatile uint32_t*)0x50004168)
#define	DMACC3Control	(*(volatile uint32_t*)0x5000416C)
#define	DMACC3Config	(*(volatile uint32_t*)0x50004170)
#define	DMACC4SrcAddr	(*(volatile uint32_t*)0x50004180)
#define	DMACC4DestAddr	(*(volatile uint32_t*)0x50004184)
#define	DMACC4LLI		(*(volatile uint32_t*)0x50004188)
#define	DMACC4Control	(*(volatile uint32_t*)0x5000418C)
#define	DMACC4Config	(*(volatile uint32_t*)0x50004190)
#define	DMACC5SrcAddr	(*(volatile uint32_t*)0x500041A0)
#define	DMACC5DestAddr	(*(volatile uint32_t*)0x500041A4)
#define	DMACC5LLI		(*(volatile uint32_t*)0x500041A8)
#define	DMACC5Control	(*(volatile uint32_t*)0x500041AC)
#define	DMACC5Config	(*(volatile uint32_t*)0x500041B0)
#define	DMACC6SrcAddr	(*(volatile uint32_t*)0x500041C0)
#define	DMACC6DestAddr	(*(volatile uint32_t*)0x500041C4)
#define	DMACC6LLI		(*(volatile uint32_t*)0x500041C8)
#define	DMACC6Control	(*(volatile uint32_t*)0x500041CC)
#define	DMACC6Config	(*(volatile uint32_t*)0x500041D0)
#define	DMACC7SrcAddr	(*(volatile uint32_t*)0x500041E0)
#define	DMACC7DestAddr	(*(volatile uint32_t*)0x500041E4)
#define	DMACC7LLI		(*(volatile uint32_t*)0x500041E8)
#define	DMACC7Control	(*(volatile uint32_t*)0x500041EC)
#define	DMACC7Config	(*(volatile uint32_t*)0x500041F0)


/* Cortex-M3 System timer */
#define	SYST_CSR	(*(volatile uint32_t*)0xE000E010)
#define	SYST_RVR	(*(volatile uint32_t*)0xE000E014)
#define	SYST_CVR	(*(volatile uint32_t*)0xE000E018)
#define	SYST_CALIB	(*(volatile uint32_t*)0xE000E01C)

/* Cortex-M3 NVIC */
#define	ISER		( (volatile uint32_t*)0xE000E100)
#define	ICER		( (volatile uint32_t*)0xE000E180)
#define	ISPR		( (volatile uint32_t*)0xE000E200)
#define	ICPR		( (volatile uint32_t*)0xE000E280)
#define	IABR		( (volatile uint32_t*)0xE000E300)
#define	IPR			( (volatile uint8_t *)0xE000E400)
#define	STIR		(*(volatile uint32_t*)0xE000EF00)

/* Cortex-M3 SCB */
#define	ACTLR		(*(volatile uint32_t*)0xE000E008)
#define	CPUID		(*(volatile uint32_t*)0xE000ED00)
#define	ICSR		(*(volatile uint32_t*)0xE000ED04)
#define	VTOR		(*(volatile uint32_t*)0xE000ED08)
#define	AIRCR		(*(volatile uint32_t*)0xE000ED0C)
#define	SCR			(*(volatile uint32_t*)0xE000ED10)
#define	CCR			(*(volatile uint32_t*)0xE000ED14)
#define	SHPR		( (volatile uint8_t *)0xE000ED14)
#define	CFSR		(*(volatile uint32_t*)0xE000ED28)
#define	MMSR		(*(volatile uint32_t*)0xE000ED28)
#define	BFSR		(*(volatile uint32_t*)0xE000ED29)
#define	UFSR		(*(volatile uint32_t*)0xE000ED2A)
#define	HFSR		(*(volatile uint32_t*)0xE000ED2C)
#define	MMFAR		(*(volatile uint32_t*)0xE000ED34)
#define	BFAR		(*(volatile uint32_t*)0xE000ED38)



/*--------------------------------------------------------------*/
/* Cortex-M3 core/peripheral access macros                      */
/*--------------------------------------------------------------*/

/* These are for only privileged mode */
#define __enable_irq() asm volatile ("CPSIE i\n")
#define __disable_irq() asm volatile ("CPSID i\n")
#define __enable_irqn(n) ISER[(n) / 32] = 1 << ((n) % 32)
#define __disable_irqn(n) ICER[(n) / 32] = 1 << ((n) % 32)
#define __test_irqn_enabled(n) (ISER[(n) / 32] & (1 << ((n) % 32)))
#define __set_irqn(n) ISPR[(n) / 32] = 1 << ((n) % 32)
#define __clear_irqn(n) ICPR[(n) / 32] = 1 << ((n) % 32)
#define __test_irqn(n) (ICPR[(n) / 32] & (1 << ((n) % 32)))
#define __test_irqn_active(n) (IABR[n / 32] & (1 << ((n) % 32)))
#define __set_irqn_priority(n,v) IPR[n] = (v)
#define __set_faultn_priority(n,v) SHPR[(n) + 16] = (v)
#define __get_MSP()			({uint32_t __rv; asm ("MRS %0, MSP\n"		: "=r" (__rv)); __rv;})
#define __get_PSP()			({uint32_t __rv; asm ("MRS %0, PSP\n"		: "=r" (__rv)); __rv;})
#define __get_PRIMASK() 	({uint32_t __rv; asm ("MRS %0, PRIMASK\n"	: "=r" (__rv)); __rv;})
#define __get_FAULTMASK()	({uint32_t __rv; asm ("MRS %0, FAULTMASK\n"	: "=r" (__rv)); __rv;})
#define __get_BASEPRI() 	({uint32_t __rv; asm ("MRS %0, BASEPRI\n"	: "=r" (__rv)); __rv;})
#define __get_CONTROL()		({uint32_t __rv; asm ("MRS %0, CONTROL\n"	: "=r" (__rv)); __rv;})
#define __set_MSP(arg)		{uint32_t __v=arg; asm ("MSR MSP, %0\n" 		:: "r" (__v));}
#define __set_PSP(arg)		{uint32_t __v=arg; asm ("MSR PSP, %0\n" 		:: "r" (__v));}
#define __set_PRIMASK(arg)	{uint32_t __v=arg; asm ("MSR PRIMASK, %0\n" 	:: "r" (__v));}
#define __set_FAULTMASK(arg) {uint32_t __v=arg; asm ("MSR FAULTMASK, %0\n" 	:: "r" (__v));}
#define __set_BASEPRI(arg)	{uint32_t __v=arg; asm ("MSR BASEPRI, %0\n" 	:: "r" (__v));}
#define __set_CONTORL(arg)	{uint32_t __v=arg; asm ("MSR CONTROL, %0\nISB\n" :: "r" (__v));}

/* These functions and macros are alternative of above for user mode */
#if USE_SV_SERVICE
#define __enable_irq_user()		asm volatile ("SVC #0\n")	/* CPSIE i */
#define __disable_irq_user()	asm volatile ("SVC #1\n")	/* CPSID i */
#define __enable_irq_user()		asm volatile ("SVC #2\n")	/* CPSIE f */
#define __disable_irq_user()	asm volatile ("SVC #3\n")	/* CPSID f */
uint32_t __get_scs_reg (volatile uint32_t* reg);			/* Read a register in SCS */
void __set_scs_reg (volatile uint32_t* reg, uint32_t val);	/* Write a register in SCS */
#define __enable_irqn_user(n) __set_scs_reg(&ISER[((n) / 32)], 1 << ((n) % 32))
#define __disable_irqn_user(n) __set_scs_reg(&ISCR[((n) / 32)], 1 << ((n) % 32))
#define __test_irqn_enabled_user(n) (__get_scs_reg(&ISCR[(n) / 32]) & (1 << ((n) % 32)))
#define __set_irqn_user(n) __set_scs_reg(&ISPR[((n) / 32)], 1 << ((n) % 32))
#define __clear_irqn_user(n) __set_scs_reg(&ICPR[((n) / 32)], 1 << ((n) % 32))
#define __test_irqn_user(n) (__get_scs_reg(&ICPR[(n) / 32]) & (1 << ((n) % 32)))
#define __test_active_irqn_user(n) (__get_scs_reg(&IABR[(n) / 32]) & (1 << ((n) % 32)))
#define __set_irqn_priority_user(n,v) __set_scs_reg(&IPR[n], (v))
#define __set_faultn_priority_user(n,v) __set_scs_reg(&SHPR[(n) + 16], (v))
#endif

/* These functions/macros can be used at user/privileged mode */
#define __REV(arg)		({uint32_t __r, __v=arg; asm ("REV %0,%1\n"   : "=r" (__r) : "r" (__v) ); __r;})
#define __REV16(arg)	({uint32_t __r, __v=arg; asm ("REV16 %0,%1\n" : "=r" (__r) : "r" (__v) ); __r;})
#define __REVSH(arg)	({uint32_t __r, __v=arg; asm ("REVSH %0,%1\n" : "=r" (__r) : "r" (__v) ); __r;})
#define __RBIT(arg)		({uint32_t __r, __v=arg; asm ("RBIT %0,%1\n"  : "=r" (__r) : "r" (__v) ); __r;})
#define __LDREXB(p)		({uint8_t __r;	asm ("LDREXB %0,[%1]\n" : "=r" (__r) : "r" (p)); __r;})
#define __LDREXH(p)		({uint16_t __r;	asm ("LDREXH %0,[%1]\n" : "=r" (__r) : "r" (p)); __r;})
#define __LDREXW(p)		({uint32_t __r;	asm ("LDREX  %0,[%1]\n" : "=r" (__r) : "r" (p)); __r;})
#define __STREXB(d,p)	({register uint32_t __r asm("r2"); register uint8_t __d asm("r1") = d; register volatile uint8_t *__p asm("r0") = p; asm ("STREXB %0,%2,[%1]\n" : "=r" (__r) : "r" (__p), "r" (__d)); __r;})
#define __STREXH(d,p)	({register uint32_t __r asm("r2"); register uint16_t __d asm("r1") = d; register volatile uint16_t *__p asm("r0") = p; asm ("STREXH %0,%2,[%1]\n" : "=r" (__r) : "r" (p), "r" (__d)); __r;})
#define __STREXW(d,p)	({register uint32_t __r asm("r2"); register uint32_t __d asm("r1") = d; register volatile uint32_t *__p asm("r0") = p; asm ("STREX  %0,%2,[%1]\n" : "=r" (__r) : "r" (p), "r" (__d)); __r;})
#define __CLREX() asm volatile ("CLREX\n")
#define __SEV() asm volatile ("SEV\n")
#define __WFE() asm volatile ("WFE\n")
#define __WFI() asm volatile ("WFI\n")

/* LPC176x IRQ number */
#define	MemManage_IRQn	(-12)
#define	BusFault_IRQn	(-11)
#define	UsageFault_IRQn	(-10)
#define	SVC_IRQn		(-5)
#define	DebugMon_IRQn	(-4)
#define	PendSV_IRQn		(-2)
#define	SysTick_IRQn	(-1)
#define	WDT_IRQn		0
#define	TIMER0_IRQn		1
#define	TIMER1_IRQn		2
#define	TIMER2_IRQn		3
#define	TIMER3_IRQn		4
#define	UART0_IRQn		5
#define	UART1_IRQn		6
#define	UART2_IRQn		7
#define	UART3_IRQn		8
#define	PWM1_IRQn		9
#define	I2C0_IRQn		10
#define	I2C1_IRQn		11
#define	I2C2_IRQn		12
#define	SPI_IRQn		13
#define	SSP0_IRQn		14
#define	SSP1_IRQn		15
#define	PLL0_IRQn		16
#define	RTC_IRQn		17
#define	EINT0_IRQn		18
#define	EINT1_IRQn		19
#define	EINT2_IRQn		20
#define	EINT3_IRQn		21
#define	ADC_IRQn		22
#define	BOD_IRQn		23
#define	USB_IRQn		24
#define	CAN_IRQn		25
#define	GPDMA_IRQn		26
#define	I2S_IRQn		27
#define	ETHER_IRQn		28
#define	RIT_IRQn		29
#define	MCPWM_IRQn		30
#define	QEI_IRQn		31
#define	PLL1_IRQn		32
#define	USBACT_IRQn		33
#define	CANACT_IRQn		34

/* LPC176x Peripheral Divider */
#define	__set_PCLKSEL(p,v)	PCLKSEL[(p) / 16] = (PCLKSEL[(p) / 16] & ~(3 << ((p) * 2 % 32))) | (v << ((p) * 2 % 32))
#define PCLKDIV_4	0
#define PCLKDIV_1	1
#define PCLKDIV_2	2
#define PCLKDIV_8	3
#define	PCLK_WDT	0
#define	PCLK_TIMER0	1
#define	PCLK_TIMER1	2
#define	PCLK_UART0	3
#define	PCLK_UART1	4
#define	PCLK_PWM1	6
#define	PCLK_I2C0	7
#define	PCLK_SPI	8
#define	PCLK_SSP1	10
#define	PCLK_DAC	11
#define	PCLK_ADC	12
#define	PCLK_CAN1	13
#define	PCLK_CAN2	14
#define	PCLK_ACF	15
#define	PCLK_QEI	16
#define	PCLK_GPIOINT	17
#define	PCLK_PCB	18
#define	PCLK_I2C1	19
#define	PCLK_SSP0	21
#define	PCLK_TIMER2	22
#define	PCLK_TIMER3	23
#define	PCLK_UART2	24
#define	PCLK_UART3	25
#define	PCLK_I2C2	26
#define	PCLK_I2S	27
#define	PCLK_RIT	28
#define	PCLK_SYSCON	29
#define	PCLK_MC		30

/* LPC176x Pin Configuration */
#define __set_PINSEL(p,b,v)		PINSEL[(p) * 2 + (b) / 16] = (PINSEL[(p) * 2 + (b) / 16] & ~(3 << ((b) * 2 % 32))) | (v << ((b) * 2 % 32))
#define __set_PINMODE(p,b,v)	PINMODE[(p) * 2 + (b) / 16] = (PINMODE[(p) * 2 + (b) / 16] & ~(3 << ((b) * 2 % 32))) | (v << ((b) * 2 % 32))
#define __set_PINOD(p,b,v)		PINOD[p] = (PINOD[p] & ~(1 << (b))) | ((v) << (b))

/* LPC176x Power Control */
#define	__set_PCONP(p,v)	PCONP = (PCONP & ~(1 << (p))) | (1 << (p))
#define	PCTIM0	1
#define	PCTIM1	2
#define	PCUART0	3
#define	PCUART1	4
#define	PCPWM1	6
#define	PCIIC0	7
#define	PCSPI	8
#define	PCRTC	9
#define	PCSSP1	10
#define	PCADC	12
#define	PCCAN1	13
#define	PCCAN2	14
#define	PCGPIO	15
#define	PCRIT	16
#define	PCMCPWM	17
#define	PCQEI	18
#define	PCI2C1	19
#define	PCSSP0	21
#define	PCTIM2	22
#define	PCTIM3	23
#define	PCUART2	24
#define	PCUART3	25
#define	PCI2C2	26
#define	PCI2S	27
#define	PCGPDMA	29
#define	PCENET	30
#define	PCUSB	31


/*--------------------------------------------------------------*/
/* Misc Macros                                                  */
/*--------------------------------------------------------------*/


#define	_BV(bit) (1<<(bit))

#define	IMPORT_BIN(sect, file, sym) asm (\
		".section " #sect "\n"\
		".balign 4\n"\
		".global " #sym "\n"\
		#sym ":\n"\
		".incbin \"" file "\"\n"\
		".global _sizeof_" #sym "\n"\
		".set _sizeof_" #sym ", . - " #sym "\n"\
		".balign 4\n"\
		".section \".text\"\n")

#define	IMPORT_BIN_PART(sect, file, ofs, siz, sym) asm (\
		".section " #sect "\n"\
		".balign 4\n"\
		".global " #sym "\n"\
		#sym ":\n"\
		".incbin \"" file "\"," #ofs "," #siz "\n"\
		".global _sizeof_" #sym "\n"\
		".set _sizeof_" #sym ", . - " #sym "\n"\
		".balign 4\n"\
		".section \".text\"\n")

/* Jump to secondary application */
#define JUMP_APP(appvt) asm (\
		"LDR SP, [%0]\n"          /* Initialize SP */\
		"LDR PC, [%0, #4]\n"      /* Go to reset vector */\
        : : "r" (appvt))


#endif /* __LPC176x */
