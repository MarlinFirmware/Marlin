#include "adc.h"
#include "../gpio/gpio.h"

//
// ADC1 device
//
adc_dev adc1 = {
	.HAL_AdcDmaIrqFlag = 0,
	.HAL_adc_results = {0},

	.regs = M4_ADC1,
	.PeriphClock = PWC_FCG3_PERIPH_ADC1,
	.Channel = ADC1_SA_CHANNEL,

	.DMARegs = M4_DMA1,
	.DMAPeriphClock = PWC_FCG0_PERIPH_DMA1,
	.DMAChannel = DmaCh3,
	.DMAenSrc = EVT_ADC1_EOCA,
};
adc_dev *ADC1 = &adc1;

uint16_t adc_read(adc_dev *dev, uint8_t channel)
{
	// wait for adc result
	while (true)
	{
		if (dev->HAL_AdcDmaIrqFlag & ADC1_SA_DMA_IRQ_BIT)
		{
			break;
		}

		uint8_t c = 0;
		if (++c >= 100)
		{
			c = 0;
			return 0;
		}

		WDT_RefreshCounter();
	}

	// read result and clear irq flag
	dev->HAL_AdcDmaIrqFlag &= ~ADC1_SA_DMA_IRQ_BIT;
	return dev->HAL_adc_results[channel];
}

void adc_setCLK()
{
#ifdef use_default_clk
	CLK_SetPeriClkSource(ClkPeriSrcPclk);
#else

	// Use XTAL as UPLL source
	stc_clk_xtal_cfg_t xtalConf = {
		.enFastStartup = Enable,
		.enMode = ClkXtalModeOsc,
		.enDrv = ClkXtalLowDrv,
	};
	CLK_XtalConfig(&xtalConf);
	CLK_XtalCmd(Enable);

	stc_clk_upll_cfg_t upllConf = {
		// upll = 8M(XTAL) / pllmDiv * plln
		.PllpDiv = 16u,
		.PllqDiv = 16u,
		.PllrDiv = 16u,
		.plln = 60u,

		// Set UPLL out 240MHz
		.pllmDiv = 2u,
	};
	CLK_SetPllSource(ClkPllSrcXTAL);
	CLK_UpllConfig(&upllConf);
	CLK_UpllCmd(Enable);
	CLK_SetPeriClkSource(ClkPeriSrcUpllr);
#endif
}

void adc_initConfig(adc_dev *dev)
{
	// enable adc1
	PWC_Fcg3PeriphClockCmd(dev->PeriphClock, Enable);

	// initialize adc1
	stc_adc_init_t adcConf = {
		.enResolution = AdcResolution_10Bit,
		.enDataAlign = AdcDataAlign_Right,
		.enAutoClear = AdcClren_Enable,
		.enScanMode = AdcMode_SAContinuous,
		.enRschsel = AdcRschsel_Restart,
	};
	ADC_Init(dev->regs, &adcConf);
}

void adc_setPinMode(uint8_t adcPin, en_pin_mode_t mode)
{
	// translate adc input to pin
	uint8_t pin;
	switch (adcPin)
	{
	case ADC1_IN0:
		pin = PA0;
		break;
	case ADC1_IN1:
		pin = PA1;
		break;
	case ADC1_IN2:
		pin = PA2;
		break;
	case ADC1_IN3:
		pin = PA3;
		break;
	case ADC12_IN4:
		pin = PA4;
		break;
	case ADC12_IN5:
		pin = PA5;
		break;
	case ADC12_IN6:
		pin = PA6;
		break;
	case ADC12_IN7:
		pin = PA7;
		break;
	case ADC12_IN8:
		pin = PB0;
		break;
	case ADC12_IN9:
		pin = PB1;
		break;
	case ADC12_IN10:
		pin = PC0;
		break;
	case ADC12_IN11:
		pin = PC1;
		break;
	case ADC1_IN12:
		pin = PC2;
		break;
	case ADC1_IN13:
		pin = PC3;
		break;
	case ADC1_IN14:
		pin = PC4;
		break;
	case ADC1_IN15:
		pin = PC5;
		break;
	default:
		return;
	}

	// set pin mode
	stc_port_init_t portConf = {
		.enPinMode = mode,
		.enPullUp = Disable,
	};
	PORT_InitGPIO(pin, &portConf);
}

void adc_setChannelPinMode(const M4_ADC_TypeDef *ADCx, uint32_t channel, en_pin_mode_t mode)
{
	// get channel offset and mask
	uint8_t channelOffset = 0u;
	if (M4_ADC1 == ADCx)
	{
		channel &= ADC1_PIN_MASK_ALL;
	}
	else
	{
		channel &= ADC2_PIN_MASK_ALL;
		channelOffset = 4u;
	}

	// set pin mode of all pins in the channel
	for (uint8_t i = 0u; channel != 0u; i++)
	{
		if (channel & 0x1ul)
		{
			adc_setPinMode((channelOffset + i), mode);
		}

		channel >>= 1u;
	}
}

void adc_channelConfig(adc_dev *dev, en_pin_mode_t mode)
{
	static uint8_t samplingTimes[ADC1_SA_CHANNEL_COUNT] = {50, 50};

	// set pin mode to analog
	adc_setChannelPinMode(dev->regs, dev->Channel, mode);

	// init adc channel
	stc_adc_ch_cfg_t adcChannelConf = {
		.u32Channel = dev->Channel,
		.u8Sequence = ADC_SEQ_A,
		.pu8SampTime = samplingTimes,
	};
	ADC_AddAdcChannel(dev->regs, &adcChannelConf);
}

void adc_triggerConfig(adc_dev *dev, uint32_t fcg0Periph)
{
	PWC_Fcg0PeriphClockCmd(fcg0Periph, Enable);

	/*
	// select EVT_TMR02_GCMA as ADC1 trigger source
	stc_adc_trg_cfg_t triggerConf = {
		.u8Sequence = ADC_SEQ_A,
		.enTrgSel = AdcTrgsel_TRGX0,
		.enInTrg0 = EVT_TMR02_GCMA,
	};
	ADC_ConfigTriggerSrc(dev->regs, &triggerConf);
	ADC_TriggerSrcCmd(dev->regs, ADC_SEQ_A, Enable);
	*/

	// ADC1 is always triggered by software
	ADC_TriggerSrcCmd(dev->regs, ADC_SEQ_A, Disable);
}

void adc_dmaInitConfig(adc_dev *dev)
{
	// setup dma config
	stc_dma_config_t dmaConf = {
		.u16BlockSize = ADC1_CH_COUNT,
		.u16TransferCnt = 0u,
		.u32SrcAddr = (uint32_t)(&dev->regs->DR0),
		.u32DesAddr = (uint32_t)(&dev->HAL_adc_results[0]),
		.u16SrcRptSize = ADC1_CH_COUNT,
		.u16DesRptSize = ADC1_CH_COUNT,
		.u32DmaLlp = 0u,
		.stcSrcNseqCfg = {
			.u32Offset = 0u,
			.u16Cnt = 0u,
		},
		.stcDesNseqCfg = {
			.u32Offset = 0u,
			.u16Cnt = 0u,
		},
		.stcDmaChCfg = {
			.enSrcInc = AddressIncrease,
			.enDesInc = AddressIncrease,
			.enSrcRptEn = Enable,
			.enDesRptEn = Enable,
			.enSrcNseqEn = Disable,
			.enDesNseqEn = Disable,
			.enTrnWidth = Dma16Bit,
			.enLlpEn = Disable,
			.enLlpMd = LlpWaitNextReq,
			.enIntEn = Disable,
		},
	};

	// enable DMA interrupt
	dmaConf.stcDmaChCfg.enIntEn = Enable;

	PWC_Fcg0PeriphClockCmd(dev->DMAPeriphClock, Enable);
	DMA_InitChannel(dev->DMARegs, dev->DMAChannel, &dmaConf);

	DMA_Cmd(dev->DMARegs, Enable);
	DMA_ChannelCmd(dev->DMARegs, dev->DMAChannel, Enable);
	DMA_ClearIrqFlag(dev->DMARegs, dev->DMAChannel, TrnCpltIrq);
	DMA_ClearIrqFlag(dev->DMARegs, dev->DMAChannel, BlkTrnCpltIrq);

	// AOS must be enabled to use DMA
	// AOS enabled at first
	// If you have enabled AOS before, then the following statement is not needed
	PWC_Fcg0PeriphClockCmd(PWC_FCG0_PERIPH_PTDIS, Enable);
	DMA_SetTriggerSrc(dev->DMARegs, dev->DMAChannel, dev->DMAenSrc);
}

void adc_dmaRegisterIRQ(stc_irq_regi_conf_t *pstcCfg, uint32_t priority)
{
	int16_t irqNum = pstcCfg->enIRQn;
	if (((irqNum >= Int000_IRQn) && (irqNum <= Int031_IRQn)) ||
		((irqNum >= Int038_IRQn) && (irqNum <= Int043_IRQn)))
	{
		if (enIrqRegistration(pstcCfg) != Ok)
		{
			return;
		}
	}
	else if (irqNum == Int129_IRQn)
	{
		enShareIrqEnable(pstcCfg->enIntSrc);
	}
	else
	{
		return;
	}

	NVIC_ClearPendingIRQ(pstcCfg->enIRQn);
	NVIC_SetPriority(pstcCfg->enIRQn, priority);
	NVIC_EnableIRQ(pstcCfg->enIRQn);
}

/**
 * ADC DMA IRQ handler
 */
void Dma1Btc3_IrqHandler(void)
{
	// DMA_ClearIrqFlag(ADC1->DMARegs, ADC1->DMAChannel, BlkTrnCpltIrq);
	ADC1->HAL_AdcDmaIrqFlag |= ADC1_SA_DMA_IRQ_BIT;
}

void adc_dmaIRQConfig(void)
{
	stc_irq_regi_conf_t stcAdcIrqCfg = {
		.enIntSrc = INT_DMA1_BTC3,
		.enIRQn = Int029_IRQn,
		.pfnCallback = &Dma1Btc3_IrqHandler,
	};
	adc_dmaRegisterIRQ(&stcAdcIrqCfg, DDL_IRQ_PRIORITY_DEFAULT);
}

void adc_setDefaultConfig(adc_dev *dev)
{
	// init and config adc and channels
	adc_initConfig(dev);
	adc_channelConfig(dev, Pin_Mode_Ana);
	adc_triggerConfig(dev, PWC_FCG0_PERIPH_PTDIS);

	// init and config DMA
	adc_dmaInitConfig(dev);
	adc_dmaIRQConfig();
	ADC_StartConvert(dev->regs);
}

void adc_init(void)
{
	// set ADC clock (default is MRC @ 8MHz)
	adc_setCLK();

	// configure ADC
	adc_setDefaultConfig(ADC1);
}
