#include "gpio.h"

extern const pin_info_t PIN_MAP[BOARD_NR_GPIO_PINS] = {
	// PA
	{0, PortA, Pin00, &adc1, ADC1_IN0, Func_Gpio},			   // PA0   OK
	{1, PortA, Pin01, &adc1, ADC1_IN1, Func_Gpio},			   // PA1   OK
	{2, PortA, Pin02, &adc1, ADC1_IN2, Func_Gpio},			   // PA2   OK
	{3, PortA, Pin03, &adc1, ADC1_IN3, Func_Gpio},			   // PA3   OK
	{4, PortA, Pin04, &adc1, ADC12_IN4, Func_Gpio},			   // PA4   OK
	{5, PortA, Pin05, &adc1, ADC12_IN5, Func_Gpio},			   // PA5   OK
	{6, PortA, Pin06, &adc1, ADC12_IN6, Func_Gpio},			   // PA6   OK
	{7, PortA, Pin07, &adc1, ADC12_IN7, Func_Gpio},			   // PA7   OK
	{8, PortA, Pin08, NULL, ADC_PIN_INVALID, Func_Usart1_Rts}, // PA8   OK
	{9, PortA, Pin09, NULL, ADC_PIN_INVALID, Func_Usart2_Tx},  // PA9   OK
	{10, PortA, Pin10, NULL, ADC_PIN_INVALID, Func_Sdio},	   // PA10  OK
	{11, PortA, Pin11, NULL, ADC_PIN_INVALID, Func_I2c1_Sda},  // PA11  OK
	{12, PortA, Pin12, NULL, ADC_PIN_INVALID, Func_I2c1_Scl},  // PA12  OK
	{13, PortA, Pin13, NULL, ADC_PIN_INVALID, Func_Gpio},	   // PA13  SWDIO
	{14, PortA, Pin14, NULL, ADC_PIN_INVALID, Func_Gpio},	   // PA14  SWCLK
	{15, PortA, Pin15, NULL, ADC_PIN_INVALID, Func_Usart2_Rx}, // PA15  OK

	// PB
	{0, PortB, Pin00, &adc1, ADC12_IN8, Func_Gpio},		  // PB0   OK
	{1, PortB, Pin01, &adc1, ADC12_IN9, Func_Gpio},		  // PB1   OK
	{2, PortB, Pin02, NULL, ADC_PIN_INVALID, Func_Gpio},  // PB2   OK
	{3, PortB, Pin03, NULL, ADC_PIN_INVALID, Func_Gpio},  // PB3   OK
	{4, PortB, Pin04, NULL, ADC_PIN_INVALID, Func_Gpio},  // PB4   OK
	{5, PortB, Pin05, NULL, ADC_PIN_INVALID, Func_Gpio},  // PB5   OK
	{6, PortB, Pin06, NULL, ADC_PIN_INVALID, Func_Gpio},  // PB6   OK
	{7, PortB, Pin07, NULL, ADC_PIN_INVALID, Func_Gpio},  // PB7   OK
	{8, PortB, Pin08, NULL, ADC_PIN_INVALID, Func_Gpio},  // PB8   OK
	{9, PortB, Pin09, NULL, ADC_PIN_INVALID, Func_Gpio},  // PB9   OK
	{10, PortB, Pin10, NULL, ADC_PIN_INVALID, Func_Gpio}, // PB10  OK
	{11, PortB, Pin11, NULL, ADC_PIN_INVALID, Func_Gpio}, // PB11  OK (10k -> VCC)
	{12, PortB, Pin12, NULL, ADC_PIN_INVALID, Func_Gpio}, // PB12  OK
	{13, PortB, Pin13, NULL, ADC_PIN_INVALID, Func_Gpio}, // PB13  OK
	{14, PortB, Pin14, NULL, ADC_PIN_INVALID, Func_Gpio}, // PB14  OK
	{15, PortB, Pin15, NULL, ADC_PIN_INVALID, Func_Gpio}, // PB15  OK

	// PC
	{0, PortC, Pin00, &adc1, ADC12_IN10, Func_Usart1_Tx}, // PC0   OK
	{1, PortC, Pin01, &adc1, ADC12_IN11, Func_Usart1_Rx}, // PC1   OK
	{2, PortC, Pin02, &adc1, ADC1_IN12, Func_Gpio},		  // PC2   OK
	{3, PortC, Pin03, &adc1, ADC1_IN13, Func_Gpio},		  // PC3   OK
	{4, PortC, Pin04, &adc1, ADC1_IN14, Func_Gpio},		  // PC4   OK
	{5, PortC, Pin05, &adc1, ADC1_IN15, Func_Gpio},		  // PC5   OK
	{6, PortC, Pin06, NULL, ADC_PIN_INVALID, Func_Gpio},  // PC6   OK
	{7, PortC, Pin07, NULL, ADC_PIN_INVALID, Func_Gpio},  // PC7   OK
	{8, PortC, Pin08, NULL, ADC_PIN_INVALID, Func_Sdio},  // PC8   OK
	{9, PortC, Pin09, NULL, ADC_PIN_INVALID, Func_Sdio},  // PC9   OK
	{10, PortC, Pin10, NULL, ADC_PIN_INVALID, Func_Sdio}, // PC10  OK
	{11, PortC, Pin11, NULL, ADC_PIN_INVALID, Func_Sdio}, // PC11  OK
	{12, PortC, Pin12, NULL, ADC_PIN_INVALID, Func_Sdio}, // PC12  OK
	{13, PortC, Pin13, NULL, ADC_PIN_INVALID, Func_Gpio}, // PC13  OK
	{14, PortC, Pin14, NULL, ADC_PIN_INVALID, Func_Gpio}, // PC14  OK
	{15, PortC, Pin15, NULL, ADC_PIN_INVALID, Func_Gpio}, // PC15  OK

	// PD
	{0, PortD, Pin00, NULL, ADC_PIN_INVALID, Func_Gpio},  // PD0   ?
	{1, PortD, Pin01, NULL, ADC_PIN_INVALID, Func_Gpio},  // PD1   ?
	{2, PortD, Pin02, NULL, ADC_PIN_INVALID, Func_Sdio},  // PD2   OK
	{3, PortD, Pin03, NULL, ADC_PIN_INVALID, Func_Gpio},  // PD3   ?
	{4, PortD, Pin04, NULL, ADC_PIN_INVALID, Func_Gpio},  // PD4   ?
	{5, PortD, Pin05, NULL, ADC_PIN_INVALID, Func_Gpio},  // PD5   ?
	{6, PortD, Pin06, NULL, ADC_PIN_INVALID, Func_Gpio},  // PD6   ?
	{7, PortD, Pin07, NULL, ADC_PIN_INVALID, Func_Gpio},  // PD7   ?
	{8, PortD, Pin08, NULL, ADC_PIN_INVALID, Func_Gpio},  // PD8   ?
	{9, PortD, Pin09, NULL, ADC_PIN_INVALID, Func_Gpio},  // PD9   ?
	{10, PortD, Pin10, NULL, ADC_PIN_INVALID, Func_Gpio}, // PD10  ?
	{11, PortD, Pin11, NULL, ADC_PIN_INVALID, Func_Gpio}, // PD11  ?
	{12, PortD, Pin12, NULL, ADC_PIN_INVALID, Func_Gpio}, // PD12  ?
	{13, PortD, Pin13, NULL, ADC_PIN_INVALID, Func_Gpio}, // PD13  ?
	{14, PortD, Pin14, NULL, ADC_PIN_INVALID, Func_Gpio}, // PD14  ?
	{15, PortD, Pin15, NULL, ADC_PIN_INVALID, Func_Gpio}, // PD15  ?

	// PE
	{0, PortE, Pin00, NULL, ADC_PIN_INVALID, Func_Gpio},	  // PE0   ?
	{1, PortE, Pin01, NULL, ADC_PIN_INVALID, Func_Gpio},	  // PE1   ?
	{2, PortE, Pin02, NULL, ADC_PIN_INVALID, Func_Gpio},	  // PE2   ?
	{3, PortE, Pin03, NULL, ADC_PIN_INVALID, Func_Gpio},	  // PE3   ?
	{4, PortE, Pin04, NULL, ADC_PIN_INVALID, Func_Usart3_Rx}, // PE4   OK
	{5, PortE, Pin05, NULL, ADC_PIN_INVALID, Func_Usart3_Tx}, // PE5   OK
	{6, PortE, Pin06, NULL, ADC_PIN_INVALID, Func_Gpio},	  // PE6   ?
	{7, PortE, Pin07, NULL, ADC_PIN_INVALID, Func_Gpio},	  // PE7   ?
	{8, PortE, Pin08, NULL, ADC_PIN_INVALID, Func_Gpio},	  // PE8   ?
	{9, PortE, Pin09, NULL, ADC_PIN_INVALID, Func_Gpio},	  // PE9   ?
	{10, PortE, Pin10, NULL, ADC_PIN_INVALID, Func_Gpio},	  // PE10  ?
	{11, PortE, Pin11, NULL, ADC_PIN_INVALID, Func_Gpio},	  // PE11  ?
	{12, PortE, Pin12, NULL, ADC_PIN_INVALID, Func_Gpio},	  // PE12  ?
	{13, PortE, Pin13, NULL, ADC_PIN_INVALID, Func_Gpio},	  // PE13  ?
	{14, PortE, Pin14, NULL, ADC_PIN_INVALID, Func_Sdio},	  // PE14  OK
	{15, PortE, Pin15, NULL, ADC_PIN_INVALID, Func_Gpio},	  // PE15  ?

	// PH
	{0, PortH, Pin00, NULL, ADC_PIN_INVALID, Func_Gpio}, // PH0   OSC_IN
	{1, PortH, Pin01, NULL, ADC_PIN_INVALID, Func_Gpio}, // PH1   OSC_OUT
	{2, PortH, Pin02, NULL, ADC_PIN_INVALID, Func_Gpio}, // PH2   OK
};

extern const uint8_t ADC_PINS[BOARD_NR_ADC_PINS] = {
	PA0,
	PA1,
	PA2,
	PA3,
	PA4,
	PA5,
	PA6,
	PA7,
	PB0,
	PB1,
	PC0,
	PC1,
	PC2,
	PC3,
	PC4,
	PC5,
};
