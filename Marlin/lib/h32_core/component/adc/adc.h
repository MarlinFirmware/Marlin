#ifndef _ADC_H
#define _ADC_H

#include "hc32_common.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/** ADC device type. */
	typedef struct adc_dev
	{
		__IO uint32_t HAL_AdcDmaIrqFlag;
		__IO uint16_t HAL_adc_results[ADC1_CH_COUNT];

		M4_ADC_TypeDef *regs;	   /**< Register map */
		__IO uint32_t PeriphClock; /**< clock information */
		__IO uint32_t Channel;

		M4_DMA_TypeDef *DMARegs;
		__IO uint32_t DMAPeriphClock;
		__IO uint8_t DMAChannel;
		__IO en_event_src_t DMAenSrc;
	} adc_dev;
	extern adc_dev adc1;
	extern struct adc_dev *ADC1;

/********************************************************/
/********************************************************/
// ADC1 channel definition ch0~ch3.
#define ADC1_SA_CHANNEL (ADC1_CH14 | ADC1_CH15)
#define ADC1_SA_CHANNEL_COUNT (ADC_CHANNEL_COUNT)
// ADC1 channel sampling time.      ADC1_CH0  ADC1_CH1 ADC1_CH2
/********************************************************/
/********************************************************/
// #define ADC1_SB_CHANNEL             (ADC1_CH4|ADC1_CH5|ADC1_CH6)
// #define ADC1_SB_CHANNEL_COUNT       (3u)
// #define ADC1_SB_CHANNEL_SAMPLE_TIME { 0x50,0x60,0x45 }
/********************************************************/
/********************************************************/

/* ADC interrupt flag bit mask definition. */
#define ADC1_SA_DMA_IRQ_BIT (1ul << 0u)

	extern void setup_adcs(void);
	extern void adc_main(void);
	uint16_t adc_read(adc_dev *dev, uint8_t channel);

#ifdef __cplusplus
}
#endif

#endif
