#ifndef _ADC_H
#define _ADC_H

#include "../../hdsc/common/hc32_ddl.h"

#ifdef __cplusplus
extern "C"
{
#endif
#define ADC_CHANNEL_COUNT 2u
#define ADC1_SA_CHANNEL (ADC1_CH14 | ADC1_CH15)
#define ADC1_SA_CHANNEL_COUNT (ADC_CHANNEL_COUNT)

// ADC irq flag bit mask
#define ADC1_SA_DMA_IRQ_BIT (1ul << 0u)

	//
	// ADC device definition
	//
	typedef struct adc_dev
	{
		__IO uint32_t HAL_AdcDmaIrqFlag;
		__IO uint16_t HAL_adc_results[ADC1_CH_COUNT];

		M4_ADC_TypeDef *regs; 
		__IO uint32_t PeriphClock;
		__IO uint32_t Channel;

		M4_DMA_TypeDef *DMARegs;
		__IO uint32_t DMAPeriphClock;
		__IO uint8_t DMAChannel;
		__IO en_event_src_t DMAenSrc;
	} adc_dev;

	extern adc_dev adc1;
	extern struct adc_dev *ADC1;

	void adc_init();
	uint16_t adc_read(adc_dev *dev, uint8_t channel);
#ifdef __cplusplus
}
#endif

#endif
