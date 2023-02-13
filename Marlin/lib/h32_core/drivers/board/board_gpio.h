#ifndef _BOARD_GPIO_H
#define _BOARD_GPIO_H

#include "hc32_common.h"
#include "adc.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct hdsc_pin_info
	{
		uint8_t gpio_bit; /**< Pin's GPIO port bit. */
		__IO en_port_t gpio_port;
		__IO uint16_t gpio_pin;
		adc_dev *adc_device;
		__IO uint8_t adc_channel;
		__IO en_port_func_t FuncSel;
	} cfg_pin_info;

	extern const cfg_pin_info PIN_MAP[];
	extern const uint8_t boardPWMPins[];

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

	/**
	 * @brief Feature test: nonzero iff the board has SerialUSB.
	 */
	// Roger Clark. Change so that BOARD_HAVE_SERIALUSB is always true, so that it can be controller by -DSERIAL_USB
#define BOARD_HAVE_SERIALUSB 1

	/*(defined(BOARD_USB_DISC_DEV) && defined(BOARD_USB_DISC_BIT))*/

	//
	// Pins Definitions
	//
	enum
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

	extern inline en_result_t PORT_SetFuncMapp(uint8_t PinNum, en_functional_state_t enSubFunc)
	{
		if (PinNum > BOARD_NR_GPIO_PINS)
			return (Error);
		return (PORT_SetFunc(PIN_MAP[PinNum].gpio_port, PIN_MAP[PinNum].gpio_pin, PIN_MAP[PinNum].FuncSel, enSubFunc));
	}

	extern inline en_result_t PORT_InitMapp(uint8_t PinNum, const stc_port_init_t *pstcPortInit)
	{
		if (PinNum > BOARD_NR_GPIO_PINS)
			return (Error);
		return (PORT_Init(PIN_MAP[PinNum].gpio_port, PIN_MAP[PinNum].gpio_pin, pstcPortInit));
	}

	extern inline en_result_t PORT_ToggleMapp(uint8_t PinNum)
	{
		if (PinNum > BOARD_NR_GPIO_PINS)
			return (Error);
		return (PORT_Toggle(PIN_MAP[PinNum].gpio_port, PIN_MAP[PinNum].gpio_pin));
	}

	extern inline en_result_t PORT_SetBitsMapp(uint8_t PinNum)
	{
		if (PinNum > BOARD_NR_GPIO_PINS)
			return (Error);
		return (PORT_SetBits(PIN_MAP[PinNum].gpio_port, PIN_MAP[PinNum].gpio_pin));
	}

	extern inline en_result_t PORT_ResetBitsMapp(uint8_t PinNum)
	{
		if (PinNum > BOARD_NR_GPIO_PINS)
			return (Error);
		return (PORT_ResetBits(PIN_MAP[PinNum].gpio_port, PIN_MAP[PinNum].gpio_pin));
	}

	extern inline uint8_t PORT_GetBitMapp(uint8_t PinNum)
	{
		en_flag_status_t getbit = Reset;
		if (PinNum > BOARD_NR_GPIO_PINS)
			return (false);
		getbit = PORT_GetBit(PIN_MAP[PinNum].gpio_port, PIN_MAP[PinNum].gpio_pin);
		return (getbit == Reset ? false : true);
	}

	extern void setup_gpio(void);

#ifdef __cplusplus
} // extern "C"
#endif
#endif
