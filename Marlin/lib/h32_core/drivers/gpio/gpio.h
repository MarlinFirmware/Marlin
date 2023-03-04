
#include "../../hdsc/common/hc32_ddl.h"
#include "../adc/adc.h"
#include "../../hdsc/addon/addon_gpio.h"

#ifdef __cplusplus
extern "C"
{
#endif

	//
	// GPIO defines
	//

#define BOARD_NR_GPIO_PINS 83
#define BOARD_NR_ADC_PINS 16

#define BOARD_NR_SPI 3
#define BOARD_SPI1_NSS_PIN PA4
#define BOARD_SPI1_SCK_PIN PA5
#define BOARD_SPI1_MISO_PIN PA6
#define BOARD_SPI1_MOSI_PIN PA7

#define BOARD_SPI2_NSS_PIN PB12
#define BOARD_SPI2_SCK_PIN PB13
#define BOARD_SPI2_MISO_PIN PB14
#define BOARD_SPI2_MOSI_PIN PB15

#define BOARD_SPI3_NSS_PIN PA15
#define BOARD_SPI3_SCK_PIN PB3
#define BOARD_SPI3_MISO_PIN PB4
#define BOARD_SPI3_MOSI_PIN PB5

	//
	// GPIO pin map
	//
	typedef struct hdsc_pin_info
	{
		uint8_t gpio_bit;
		__IO en_port_t gpio_port;
		__IO uint16_t gpio_pin;
		adc_dev *adc_device;
		__IO uint8_t adc_channel;
		__IO en_port_func_t function;
	} pin_info_t;

	extern const pin_info_t PIN_MAP[];
	extern const uint8_t ADC_PINS[BOARD_NR_ADC_PINS];

	//
	// Pin Definitions
	//
	enum PIN
	{
		PA0,
		PA1,
		PA2,
		PA3,
		PA4,
		PA5,
		PA6,
		PA7,
		PA8,
		PA9,
		PA10,
		PA11,
		PA12,
		PA13,
		PA14,
		PA15,
		PB0,
		PB1,
		PB2,
		PB3,
		PB4,
		PB5,
		PB6,
		PB7,
		PB8,
		PB9,
		PB10,
		PB11,
		PB12,
		PB13,
		PB14,
		PB15,
		PC0,
		PC1,
		PC2,
		PC3,
		PC4,
		PC5,
		PC6,
		PC7,
		PC8,
		PC9,
		PC10,
		PC11,
		PC12,
		PC13,
		PC14,
		PC15,
		PD0,
		PD1,
		PD2,
		PD3,
		PD4,
		PD5,
		PD6,
		PD7,
		PD8,
		PD9,
		PD10,
		PD11,
		PD12,
		PD13,
		PD14,
		PD15,
		PE0,
		PE1,
		PE2,
		PE3,
		PE4,
		PE5,
		PE6,
		PE7,
		PE8,
		PE9,
		PE10,
		PE11,
		PE12,
		PE13,
		PE14,
		PE15,
		PH0,
		PH1,
		PH2,
	};

	//
	// GPIO wrappers for PORT_*
	//
	extern inline en_result_t PORT_SetFuncGPIO(uint8_t pin, en_functional_state_t subFunction)
	{
		if (pin > BOARD_NR_GPIO_PINS)
		{
			return Error;
		}

		return PORT_SetFunc(PIN_MAP[pin].gpio_port, PIN_MAP[pin].gpio_pin, PIN_MAP[pin].function, subFunction);
	}

	extern inline en_result_t PORT_InitGPIO(uint8_t pin, const stc_port_init_t *portConf)
	{
		if (pin > BOARD_NR_GPIO_PINS)
		{
			return Error;
		}

		return PORT_Init(PIN_MAP[pin].gpio_port, PIN_MAP[pin].gpio_pin, portConf);
	}

	extern inline en_result_t PORT_GetConfigGPIO(uint8_t pin, stc_port_init_t *portConf)
	{
		if (pin > BOARD_NR_GPIO_PINS)
		{
			return Error;
		}

		return PORT_GetConfig(PIN_MAP[pin].gpio_port, PIN_MAP[pin].gpio_pin, portConf);
	}

	extern inline en_result_t PORT_ToggleGPIO(uint8_t pin)
	{
		if (pin > BOARD_NR_GPIO_PINS)
		{
			return Error;
		}

		return PORT_Toggle(PIN_MAP[pin].gpio_port, PIN_MAP[pin].gpio_pin);
	}

	extern inline en_result_t PORT_SetBitsGPIO(uint8_t pin)
	{
		if (pin > BOARD_NR_GPIO_PINS)
		{
			return Error;
		}

		return PORT_SetBits(PIN_MAP[pin].gpio_port, PIN_MAP[pin].gpio_pin);
	}

	extern inline en_result_t PORT_ResetBitsGPIO(uint8_t pin)
	{
		if (pin > BOARD_NR_GPIO_PINS)
		{
			return Error;
		}

		return PORT_ResetBits(PIN_MAP[pin].gpio_port, PIN_MAP[pin].gpio_pin);
	}

	extern inline uint8_t PORT_GetBitGPIO(uint8_t pin)
	{
		if (pin > BOARD_NR_GPIO_PINS)
		{
			return Error;
		}

		return (PORT_GetBit(PIN_MAP[pin].gpio_port, PIN_MAP[pin].gpio_pin) == Reset) ? false : true;
	}
#ifdef __cplusplus
}
#endif
