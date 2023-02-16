#define _ADC_HAL_C_
#include "startup.h"
#include "../MarlinHAL.h"

adc_dev adc1 = {
	.HAL_AdcDmaIrqFlag = 0,
	.HAL_adc_results = {0},

	.regs = M4_ADC1,
	.PeriphClock = PWC_FCG3_PERIPH_ADC1,
	.Channel = ADC1_SA_CHANNEL /*|ADC1_SB_CHANNEL*/,

	.DMARegs = M4_DMA1,
	.DMAPeriphClock = PWC_FCG0_PERIPH_DMA1,
	.DMAChannel = DmaCh3,
	.DMAenSrc = EVT_ADC1_EOCA,
};
/** ADC1 device. */
adc_dev *ADC1 = &adc1;

extern void adc_main(void)
{
	/* Check ADC1 SA. */
	if (ADC1->HAL_AdcDmaIrqFlag & ADC1_SA_DMA_IRQ_BIT)
	{
		ADC1->HAL_AdcDmaIrqFlag &= ~ADC1_SA_DMA_IRQ_BIT;
		printf("\nADC1={%u,%u,%u,%u,%u,%u,%u}\r\n", ADC1->HAL_adc_results[0], ADC1->HAL_adc_results[1], ADC1->HAL_adc_results[2], ADC1->HAL_adc_results[3], ADC1->HAL_adc_results[4], ADC1->HAL_adc_results[5], ADC1->HAL_adc_results[6]);
		// Do something with m_au16Adc1SaValue.
		// ADC_StartConvert(ADC1->regs);
	}
}

uint16_t __O0 adc_read(adc_dev *dev, uint8_t channel)
{
	static uint16_t adc_results = 0;
	do
	{
		if (dev->HAL_AdcDmaIrqFlag & ADC1_SA_DMA_IRQ_BIT)
			break;
		uint8_t cnt3 = 0;
		if (++cnt3 >= 100)
		{
			cnt3 = 0;
			return (adc_results);
		}
		MarlinHAL::watchdog_refresh();
	} while (1);
	dev->HAL_AdcDmaIrqFlag &= ~ADC1_SA_DMA_IRQ_BIT;
	if (dev->HAL_adc_results[channel] == 0)
	{
		return (adc_results);
	}
	adc_results = (dev->HAL_adc_results[channel]);
	return (uint16_t)(dev->HAL_adc_results[channel]);
}

static void __O0 adc_setclk(void)
{
#ifdef use_default_clk
	CLK_SetPeriClkSource(ClkPeriSrcPclk);
#else
	stc_clk_xtal_cfg_t stcXtalCfg;
	stc_clk_upll_cfg_t stcUpllCfg;

	MEM_ZERO_STRUCT(stcXtalCfg);
	MEM_ZERO_STRUCT(stcUpllCfg);

	/* Use XTAL as UPLL source. */
	stcXtalCfg.enFastStartup = Enable;
	stcXtalCfg.enMode = ClkXtalModeOsc;
	stcXtalCfg.enDrv = ClkXtalLowDrv;
	CLK_XtalConfig(&stcXtalCfg);
	CLK_XtalCmd(Enable);

	/* Set UPLL out 240MHz. */
	stcUpllCfg.pllmDiv = 2u;
	/* upll = 8M(XTAL) / pllmDiv * plln */
	stcUpllCfg.plln = 60u;
	stcUpllCfg.PllpDiv = 16u;
	stcUpllCfg.PllqDiv = 16u;
	stcUpllCfg.PllrDiv = 16u;
	CLK_SetPllSource(ClkPllSrcXTAL);
	CLK_UpllConfig(&stcUpllCfg);
	CLK_UpllCmd(Enable);
	CLK_SetPeriClkSource(ClkPeriSrcUpllr);
#endif
}

static void __O0 adc_initconfig(adc_dev *dev)
{
	stc_adc_init_t stcAdcInit;
	MEM_ZERO_STRUCT(stcAdcInit);
	stcAdcInit.enResolution = AdcResolution_10Bit;
	stcAdcInit.enDataAlign = AdcDataAlign_Right;
	stcAdcInit.enAutoClear = AdcClren_Enable;
	stcAdcInit.enScanMode = AdcMode_SAContinuous;
	stcAdcInit.enRschsel = AdcRschsel_Restart;
	/* 1. Enable ADC1. */
	PWC_Fcg3PeriphClockCmd(dev->PeriphClock, Enable);
	/* 2. Initialize ADC1. */
	ADC_Init(dev->regs, &stcAdcInit);
}

static void adc_SetPinMode(uint8_t u8AdcPin, en_pin_mode_t enMode)
{
	bool bFlag = true;
	stc_port_init_t stcPortInit;
	uint8_t PIN;

	MEM_ZERO_STRUCT(stcPortInit);
	stcPortInit.enPinMode = enMode;
	stcPortInit.enPullUp = Disable;

	switch (u8AdcPin)
	{
	case ADC1_IN0:
		PIN = PA0;
		break;

	case ADC1_IN1:
		PIN = PA1;
		break;

	case ADC1_IN2:
		PIN = PA2;
		break;

	case ADC1_IN3:
		PIN = PA3;
		break;

	case ADC12_IN4:
		PIN = PA4;
		break;

	case ADC12_IN5:
		PIN = PA5;
		break;

	case ADC12_IN6:
		PIN = PA6;
		break;

	case ADC12_IN7:
		PIN = PA7;
		break;

	case ADC12_IN8:
		PIN = PB0;
		break;

	case ADC12_IN9:
		PIN = PB1;
		break;

	case ADC12_IN10:
		PIN = PC0;
		break;

	case ADC12_IN11:
		PIN = PC1;
		break;

	case ADC1_IN12:
		PIN = PC2;
		break;

	case ADC1_IN13:
		PIN = PC3;
		break;

	case ADC1_IN14:
		PIN = PC4;
		break;

	case ADC1_IN15:
		PIN = PC5;
		break;

	default:
		bFlag = false;
		break;
	}

	if (true == bFlag)
	{
		PORT_InitMapp(PIN, &stcPortInit);
	}
}

static void __O0 adc_SetChannelPinMode(const M4_ADC_TypeDef *ADCx, uint32_t u32Channel, en_pin_mode_t enMode)
{
	uint8_t u8ChIndex;
	uint8_t u8ChOffset = 0u;

	if (M4_ADC1 == ADCx)
	{
		u32Channel &= ADC1_PIN_MASK_ALL;
	}
	else
	{
		u32Channel &= ADC2_PIN_MASK_ALL;
		u8ChOffset = 4u;
	}

	u8ChIndex = 0u;
	while (0u != u32Channel)
	{
		if (u32Channel & 0x1ul)
		{
			adc_SetPinMode((u8ChIndex + u8ChOffset), enMode);
		}

		u32Channel >>= 1u;
		u8ChIndex++;
	}
}

static void __O0 adc_channelchonfig(adc_dev *dev, en_pin_mode_t enMode) /* onay ADC_SEQ_A and ADC1*/
{
	stc_adc_ch_cfg_t stcChCfg;
#define ADC1_SA_CHANNEL_SAMPLE_TIME {50, 50}
	uint8_t au8Adc1SaSampTime[ADC1_SA_CHANNEL_COUNT] = ADC1_SA_CHANNEL_SAMPLE_TIME;
	MEM_ZERO_STRUCT(stcChCfg);

	/**************************** Add ADC1 channels ****************************/
	/* 1. Set the ADC pin to analog mode. */
	adc_SetChannelPinMode(dev->regs, dev->Channel, enMode);

	stcChCfg.u32Channel = dev->Channel;
	stcChCfg.u8Sequence = ADC_SEQ_A;
	stcChCfg.pu8SampTime = au8Adc1SaSampTime;
	/* 2. Add ADC channel. */
	ADC_AddAdcChannel(dev->regs, &stcChCfg);
}
static void __O0 adc_TriggerConfig(adc_dev *dev, uint32_t u32Fcg0Periph)
{
	stc_adc_trg_cfg_t stcTrgCfg;
	MEM_ZERO_STRUCT(stcTrgCfg);
	PWC_Fcg0PeriphClockCmd(u32Fcg0Periph, Enable);
	/* Select EVT_TMR02_GCMA as ADC1 sequence A trigger source. */
	/*
	 stcTrgCfg.u8Sequence = ADC_SEQ_A;
	stcTrgCfg.enTrgSel   = AdcTrgsel_TRGX0;
	stcTrgCfg.enInTrg0   = EVT_TMR02_GCMA;
	ADC_ConfigTriggerSrc(dev->regs, &stcTrgCfg);
	ADC_TriggerSrcCmd(dev->regs, ADC_SEQ_A, Enable);
	*/
	/* ADC1 sequence A will be started by software. */
	ADC_TriggerSrcCmd(dev->regs, ADC_SEQ_A, Disable);
}

static void __O0 adc_DMAInitConfig(adc_dev *dev)
{
	stc_dma_config_t stcDmaCfg;

	MEM_ZERO_STRUCT(stcDmaCfg);

	stcDmaCfg.u16BlockSize = ADC1_CH_COUNT;
	stcDmaCfg.u16TransferCnt = 0u;
	stcDmaCfg.u32SrcAddr = (uint32_t)(&dev->regs->DR0);
	stcDmaCfg.u32DesAddr = (uint32_t)(&dev->HAL_adc_results[0]);
	stcDmaCfg.u16DesRptSize = ADC1_CH_COUNT;
	stcDmaCfg.u16SrcRptSize = ADC1_CH_COUNT;
	stcDmaCfg.u32DmaLlp = 0u;
	stcDmaCfg.stcSrcNseqCfg.u16Cnt = 0u;
	stcDmaCfg.stcSrcNseqCfg.u32Offset = 0u;
	stcDmaCfg.stcDesNseqCfg.u16Cnt = 0u;
	stcDmaCfg.stcDesNseqCfg.u32Offset = 0u;
	stcDmaCfg.stcDmaChCfg.enSrcInc = AddressIncrease;
	stcDmaCfg.stcDmaChCfg.enDesInc = AddressIncrease;
	stcDmaCfg.stcDmaChCfg.enSrcRptEn = Enable;
	stcDmaCfg.stcDmaChCfg.enDesRptEn = Enable;
	stcDmaCfg.stcDmaChCfg.enSrcNseqEn = Disable;
	stcDmaCfg.stcDmaChCfg.enDesNseqEn = Disable;
	stcDmaCfg.stcDmaChCfg.enTrnWidth = Dma16Bit;
	stcDmaCfg.stcDmaChCfg.enLlpEn = Disable;
	/* Enable DMA interrupt. */
	stcDmaCfg.stcDmaChCfg.enIntEn = Enable;

	PWC_Fcg0PeriphClockCmd(dev->DMAPeriphClock, Enable);
	DMA_InitChannel(dev->DMARegs, dev->DMAChannel, &stcDmaCfg);
	DMA_Cmd(dev->DMARegs, Enable);
	DMA_ChannelCmd(dev->DMARegs, dev->DMAChannel, Enable);
	DMA_ClearIrqFlag(dev->DMARegs, dev->DMAChannel, TrnCpltIrq);
	DMA_ClearIrqFlag(dev->DMARegs, dev->DMAChannel, BlkTrnCpltIrq);
	/* AOS must be enabled to use DMA */
	/* AOS enabled at first. */
	/* If you have enabled AOS before, then the following statement is not needed. */
	PWC_Fcg0PeriphClockCmd(PWC_FCG0_PERIPH_PTDIS, Enable);
	DMA_SetTriggerSrc(dev->DMARegs, dev->DMAChannel, dev->DMAenSrc);
}

static void __O0 adc_DmaIrqRegister(stc_irq_regi_conf_t *pstcCfg, uint32_t u32Priority)
{
	int16_t s16Vnum = pstcCfg->enIRQn;

	if (((s16Vnum >= Int000_IRQn) && (s16Vnum <= Int031_IRQn)) ||
		((s16Vnum >= Int038_IRQn) && (s16Vnum <= Int043_IRQn)))
	{
		if (Ok != enIrqRegistration(pstcCfg))
		{
			return;
		}
	}
	else if (Int129_IRQn == s16Vnum)
	{
		enShareIrqEnable(pstcCfg->enIntSrc);
	}
	else
	{
		return;
	}
	NVIC_ClearPendingIRQ(pstcCfg->enIRQn);
	NVIC_SetPriority(pstcCfg->enIRQn, u32Priority);
	NVIC_EnableIRQ(pstcCfg->enIRQn);
}

/**
 *******************************************************************************
 ** \brief DMA IRQ callbacks.
 **
 ******************************************************************************/
void Dma1Btc3_IrqHandler(void)
{
	// DMA_ClearIrqFlag(ADC1->DMARegs, ADC1->DMAChannel, BlkTrnCpltIrq);
	ADC1->HAL_AdcDmaIrqFlag |= ADC1_SA_DMA_IRQ_BIT;
}

static void __O0 adc_DMAIrqConfig(void)
{
	stc_irq_regi_conf_t stcAdcIrqCfg;
	MEM_ZERO_STRUCT(stcAdcIrqCfg);
	stcAdcIrqCfg.enIntSrc = INT_DMA1_BTC3;
	stcAdcIrqCfg.enIRQn = Int029_IRQn;
	stcAdcIrqCfg.pfnCallback = &Dma1Btc3_IrqHandler;
	adc_DmaIrqRegister(&stcAdcIrqCfg, DDL_IRQ_PRIORITY_DEFAULT);
}

static void __O0 adc_default_config(adc_dev *dev)
{
	adc_initconfig(dev);
	adc_channelchonfig(dev, Pin_Mode_Ana);
	adc_TriggerConfig(dev, PWC_FCG0_PERIPH_PTDIS);
	/* Config DMA. */
	adc_DMAInitConfig(dev);
	adc_DMAIrqConfig();
	ADC_StartConvert(dev->regs);
}

static void adc_foreach(void (*fn)(adc_dev *))
{
	fn(ADC1);
}

extern void __O0 setup_adcs(void)
{
	/* Default clock is MRC(8MHz). */
	adc_setclk();
	/* Config ADCs. */
	adc_foreach(adc_default_config);
}

#undef _ADC_HAL_C_
/************end of file********************/
