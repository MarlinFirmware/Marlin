#ifndef _core_pins_h_
#define _core_pins_h_

#include <avr/io.h>

#if (GCC_VERSION >= 40300) && (GCC_VERSION < 40302)
#error "Buggy GCC 4.3.0 compiler, please upgrade!"
#endif


// This will speed up digitalWrite slightly, but it
// adds to your code size.  However, if you don't
// have many digitalWrites, or most are compile time
// constant inputs, then this might be worthwhile
//#define DIGITAL_WRITE_EXPENSIVE_INLINE_OPTIMIZATION

// This removes checking in digitalWrite for pin numbers
// beyond the maximum number of pins.  Your program
// WILL CRASH is digitalWrite is called with a pin number
// too large, when this is enabled.  However, if you don't
// make such mistakes, you'll get a minor speedup.
//#define DIGITAL_WRITE_RISKY_OMIT_OVERFLOW_CHECK

#define HIGH 0x1
#define LOW  0x0

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2

#define LSBFIRST 0
#define MSBFIRST 1

#ifndef _BV
#define _BV(n) (1<<(n))
#endif

////////////////////////////////////
// Teensy 2.0
////////////////////////////////////
#if defined(__AVR_ATmega32U4__)
#define CORE_NUM_TOTAL_PINS	25
#define CORE_NUM_DIGITAL	13
#define CORE_NUM_ANALOG		12
#define CORE_NUM_PWM		7
#define CORE_NUM_INTERRUPT	4
#define PIN_B0		0
#define PIN_B1		1
#define PIN_B2		2
#define PIN_B3		3
#define PIN_B7		4
#define PIN_D0		5
#define PIN_D1		6
#define PIN_D2		7
#define PIN_D3		8
#define PIN_C6		9
#define PIN_C7		10
#define PIN_D6		11
#define PIN_D7		12
#define PIN_B4		13
#define PIN_B5		14
#define PIN_B6		15
#define PIN_F7		16
#define PIN_F6		17
#define PIN_F5		18
#define PIN_F4		19
#define PIN_F1		20
#define PIN_F0		21
#define PIN_D4		22
#define PIN_D5		23
#define PIN_E6		24
#define CORE_PIN0_BIT		0
#define CORE_PIN1_BIT		1
#define CORE_PIN2_BIT		2
#define CORE_PIN3_BIT		3
#define CORE_PIN4_BIT		7
#define CORE_PIN5_BIT		0
#define CORE_PIN6_BIT		1
#define CORE_PIN7_BIT		2
#define CORE_PIN8_BIT		3
#define CORE_PIN9_BIT		6
#define CORE_PIN10_BIT		7
#define CORE_PIN11_BIT		6
#define CORE_PIN12_BIT		7
#define CORE_PIN13_BIT		4
#define CORE_PIN14_BIT		5
#define CORE_PIN15_BIT		6
#define CORE_PIN16_BIT		7
#define CORE_PIN17_BIT		6
#define CORE_PIN18_BIT		5
#define CORE_PIN19_BIT		4
#define CORE_PIN20_BIT		1
#define CORE_PIN21_BIT		0
#define CORE_PIN22_BIT		4
#define CORE_PIN23_BIT		5
#define CORE_PIN24_BIT		6
#define CORE_PIN0_BITMASK	_BV(CORE_PIN0_BIT)
#define CORE_PIN1_BITMASK	_BV(CORE_PIN1_BIT)
#define CORE_PIN2_BITMASK	_BV(CORE_PIN2_BIT)
#define CORE_PIN3_BITMASK	_BV(CORE_PIN3_BIT)
#define CORE_PIN4_BITMASK	_BV(CORE_PIN4_BIT)
#define CORE_PIN5_BITMASK	_BV(CORE_PIN5_BIT)
#define CORE_PIN6_BITMASK	_BV(CORE_PIN6_BIT)
#define CORE_PIN7_BITMASK	_BV(CORE_PIN7_BIT)
#define CORE_PIN8_BITMASK	_BV(CORE_PIN8_BIT)
#define CORE_PIN9_BITMASK	_BV(CORE_PIN9_BIT)
#define CORE_PIN10_BITMASK	_BV(CORE_PIN10_BIT)
#define CORE_PIN11_BITMASK	_BV(CORE_PIN11_BIT)
#define CORE_PIN12_BITMASK	_BV(CORE_PIN12_BIT)
#define CORE_PIN13_BITMASK	_BV(CORE_PIN13_BIT)
#define CORE_PIN14_BITMASK	_BV(CORE_PIN14_BIT)
#define CORE_PIN15_BITMASK	_BV(CORE_PIN15_BIT)
#define CORE_PIN16_BITMASK	_BV(CORE_PIN16_BIT)
#define CORE_PIN17_BITMASK	_BV(CORE_PIN17_BIT)
#define CORE_PIN18_BITMASK	_BV(CORE_PIN18_BIT)
#define CORE_PIN19_BITMASK	_BV(CORE_PIN19_BIT)
#define CORE_PIN20_BITMASK	_BV(CORE_PIN20_BIT)
#define CORE_PIN21_BITMASK	_BV(CORE_PIN21_BIT)
#define CORE_PIN22_BITMASK	_BV(CORE_PIN22_BIT)
#define CORE_PIN23_BITMASK	_BV(CORE_PIN23_BIT)
#define CORE_PIN24_BITMASK	_BV(CORE_PIN24_BIT)
#define CORE_PIN0_PORTREG	PORTB
#define CORE_PIN1_PORTREG	PORTB
#define CORE_PIN2_PORTREG	PORTB
#define CORE_PIN3_PORTREG	PORTB
#define CORE_PIN4_PORTREG	PORTB
#define CORE_PIN5_PORTREG	PORTD
#define CORE_PIN6_PORTREG	PORTD
#define CORE_PIN7_PORTREG	PORTD
#define CORE_PIN8_PORTREG	PORTD
#define CORE_PIN9_PORTREG	PORTC
#define CORE_PIN10_PORTREG	PORTC
#define CORE_PIN11_PORTREG	PORTD
#define CORE_PIN12_PORTREG	PORTD
#define CORE_PIN13_PORTREG	PORTB
#define CORE_PIN14_PORTREG	PORTB
#define CORE_PIN15_PORTREG	PORTB
#define CORE_PIN16_PORTREG	PORTF
#define CORE_PIN17_PORTREG	PORTF
#define CORE_PIN18_PORTREG	PORTF
#define CORE_PIN19_PORTREG	PORTF
#define CORE_PIN20_PORTREG	PORTF
#define CORE_PIN21_PORTREG	PORTF
#define CORE_PIN22_PORTREG	PORTD
#define CORE_PIN23_PORTREG	PORTD
#define CORE_PIN24_PORTREG	PORTE
#define CORE_PIN0_DDRREG	DDRB
#define CORE_PIN1_DDRREG	DDRB
#define CORE_PIN2_DDRREG	DDRB
#define CORE_PIN3_DDRREG	DDRB
#define CORE_PIN4_DDRREG	DDRB
#define CORE_PIN5_DDRREG	DDRD
#define CORE_PIN6_DDRREG	DDRD
#define CORE_PIN7_DDRREG	DDRD
#define CORE_PIN8_DDRREG	DDRD
#define CORE_PIN9_DDRREG	DDRC
#define CORE_PIN10_DDRREG	DDRC
#define CORE_PIN11_DDRREG	DDRD
#define CORE_PIN12_DDRREG	DDRD
#define CORE_PIN13_DDRREG	DDRB
#define CORE_PIN14_DDRREG	DDRB
#define CORE_PIN15_DDRREG	DDRB
#define CORE_PIN16_DDRREG	DDRF
#define CORE_PIN17_DDRREG	DDRF
#define CORE_PIN18_DDRREG	DDRF
#define CORE_PIN19_DDRREG	DDRF
#define CORE_PIN20_DDRREG	DDRF
#define CORE_PIN21_DDRREG	DDRF
#define CORE_PIN22_DDRREG	DDRD
#define CORE_PIN23_DDRREG	DDRD
#define CORE_PIN24_DDRREG	DDRE
#define CORE_PIN0_PINREG	PINB
#define CORE_PIN1_PINREG	PINB
#define CORE_PIN2_PINREG	PINB
#define CORE_PIN3_PINREG	PINB
#define CORE_PIN4_PINREG	PINB
#define CORE_PIN5_PINREG	PIND
#define CORE_PIN6_PINREG	PIND
#define CORE_PIN7_PINREG	PIND
#define CORE_PIN8_PINREG	PIND
#define CORE_PIN9_PINREG	PINC
#define CORE_PIN10_PINREG	PINC
#define CORE_PIN11_PINREG	PIND
#define CORE_PIN12_PINREG	PIND
#define CORE_PIN13_PINREG	PINB
#define CORE_PIN14_PINREG	PINB
#define CORE_PIN15_PINREG	PINB
#define CORE_PIN16_PINREG	PINF
#define CORE_PIN17_PINREG	PINF
#define CORE_PIN18_PINREG	PINF
#define CORE_PIN19_PINREG	PINF
#define CORE_PIN20_PINREG	PINF
#define CORE_PIN21_PINREG	PINF
#define CORE_PIN22_PINREG	PIND
#define CORE_PIN23_PINREG	PIND
#define CORE_PIN24_PINREG	PINE
#define CORE_ADC0_PIN		PIN_F0
#define CORE_ADC1_PIN		PIN_F1
#define CORE_ADC4_PIN		PIN_F4
#define CORE_ADC5_PIN		PIN_F5
#define CORE_ADC6_PIN		PIN_F6
#define CORE_ADC7_PIN		PIN_F7
#define CORE_ADC8_PIN		PIN_D4
#define CORE_ADC9_PIN		PIN_D6
#define CORE_ADC10_PIN		PIN_D7
#define CORE_ADC11_PIN		PIN_B4
#define CORE_ADC12_PIN		PIN_B5
#define CORE_ADC13_PIN		PIN_B6
#define CORE_RXD1_PIN           PIN_D2
#define CORE_TXD1_PIN           PIN_D3
#define CORE_XCK1_PIN           PIN_D5
#define CORE_SDA0_PIN           PIN_D1
#define CORE_SCL0_PIN           PIN_D0
#define CORE_INT0_PIN           PIN_D0
#define CORE_INT1_PIN           PIN_D1
#define CORE_INT2_PIN           PIN_D2
#define CORE_INT3_PIN           PIN_D3
#define CORE_SS0_PIN            PIN_B0
#define CORE_MOSI0_PIN          PIN_B2
#define CORE_MISO0_PIN          PIN_B3
#define CORE_SCLK0_PIN          PIN_B1
#define CORE_T0_PIN             PIN_D7
#define CORE_T1_PIN             PIN_D6
#define CORE_ICP1_PIN           PIN_D4
#define CORE_ICP3_PIN           PIN_C7
#define CORE_OC0A_PIN           PIN_B7
#define CORE_OC0B_PIN           PIN_D0
#define CORE_OC1A_PIN           PIN_B5
#define CORE_OC1B_PIN           PIN_B6
#define CORE_OC1C_PIN           PIN_B7
#define CORE_OC3A_PIN           PIN_C6
#define CORE_OC4A_PIN           PIN_C7
#define CORE_OC4AN_PIN          PIN_C6
#define CORE_OC4B_PIN           PIN_B6
#define CORE_OC4BN_PIN          PIN_B5
#define CORE_OC4D_PIN           PIN_D7
#define CORE_OC4DN_PIN          PIN_D6
#define CORE_PCINT0_PIN		PIN_B0
#define CORE_PCINT1_PIN		PIN_B1
#define CORE_PCINT2_PIN		PIN_B2
#define CORE_PCINT3_PIN		PIN_B3
#define CORE_PCINT4_PIN		PIN_B4
#define CORE_PCINT5_PIN		PIN_B5
#define CORE_PCINT6_PIN		PIN_B6
#define CORE_PCINT7_PIN		PIN_B7
#define CORE_LED0_PIN		PIN_D6
#define CORE_PWM0_PIN		CORE_OC1C_PIN	// B7  4
#define CORE_PWM1_PIN		CORE_OC0B_PIN	// D0  5
#define CORE_PWM2_PIN		CORE_OC3A_PIN	// C6  9
#define CORE_PWM3_PIN		CORE_OC4A_PIN	// C7  10
#define CORE_PWM4_PIN		CORE_OC4D_PIN	// D7  12
#define CORE_PWM5_PIN		CORE_OC1A_PIN	// B5  14
#define CORE_PWM6_PIN		CORE_OC1B_PIN	// B6  15
#define CORE_ANALOG0_PIN	PIN_F0	// 21  ADC0
#define CORE_ANALOG1_PIN	PIN_F1	// 20  ADC1
#define CORE_ANALOG2_PIN	PIN_F4	// 19  ADC4
#define CORE_ANALOG3_PIN	PIN_F5	// 18  ADC5
#define CORE_ANALOG4_PIN	PIN_F6	// 17  ADC6
#define CORE_ANALOG5_PIN	PIN_F7	// 16  ADC7
#define CORE_ANALOG6_PIN	PIN_B6	// 15  ADC13
#define CORE_ANALOG7_PIN	PIN_B5	// 14  ADC12
#define CORE_ANALOG8_PIN	PIN_B4	// 13  ADC11
#define CORE_ANALOG9_PIN	PIN_D7	// 12  ADC10
#define CORE_ANALOG10_PIN	PIN_D6  // 11  ADC9
#define CORE_ANALOG11_PIN	PIN_D4	// 22  ADC8


////////////////////////////////////
// Teensy 1.0
////////////////////////////////////
#elif defined(__AVR_AT90USB162__)
#define CORE_NUM_TOTAL_PINS	21
#define CORE_NUM_DIGITAL	21
#define CORE_NUM_ANALOG		0
#define CORE_NUM_PWM		4
#define CORE_NUM_INTERRUPT	8
#define PIN_D0		0
#define PIN_D1		1
#define PIN_D2		2
#define PIN_D3		3
#define PIN_D4		4
#define PIN_D5		5
#define PIN_D6		6
#define PIN_D7		7
#define PIN_SS		8
#define PIN_SCLK	9
#define PIN_MOSI	10
#define PIN_MISO	11
#define PIN_B0		8
#define PIN_B1		9
#define PIN_B2		10
#define PIN_B3		11
#define PIN_B4		12
#define PIN_B5		13
#define PIN_B6		14
#define PIN_B7		15
#define PIN_C7		16
#define PIN_C6		17
#define PIN_C5		18
#define PIN_C4		19
#define PIN_C2		20
#define CORE_PIN0_BIT		0
#define CORE_PIN1_BIT		1
#define CORE_PIN2_BIT		2
#define CORE_PIN3_BIT		3
#define CORE_PIN4_BIT		4
#define CORE_PIN5_BIT		5
#define CORE_PIN6_BIT		6
#define CORE_PIN7_BIT		7
#define CORE_PIN8_BIT		0
#define CORE_PIN9_BIT		1
#define CORE_PIN10_BIT		2
#define CORE_PIN11_BIT		3
#define CORE_PIN12_BIT		4
#define CORE_PIN13_BIT		5
#define CORE_PIN14_BIT		6
#define CORE_PIN15_BIT		7
#define CORE_PIN16_BIT		7
#define CORE_PIN17_BIT		6
#define CORE_PIN18_BIT		5
#define CORE_PIN19_BIT		4
#define CORE_PIN20_BIT		2
#define CORE_PIN0_BITMASK	_BV(CORE_PIN0_BIT)
#define CORE_PIN1_BITMASK	_BV(CORE_PIN1_BIT)
#define CORE_PIN2_BITMASK	_BV(CORE_PIN2_BIT)
#define CORE_PIN3_BITMASK	_BV(CORE_PIN3_BIT)
#define CORE_PIN4_BITMASK	_BV(CORE_PIN4_BIT)
#define CORE_PIN5_BITMASK	_BV(CORE_PIN5_BIT)
#define CORE_PIN6_BITMASK	_BV(CORE_PIN6_BIT)
#define CORE_PIN7_BITMASK	_BV(CORE_PIN7_BIT)
#define CORE_PIN8_BITMASK	_BV(CORE_PIN8_BIT)
#define CORE_PIN9_BITMASK	_BV(CORE_PIN9_BIT)
#define CORE_PIN10_BITMASK	_BV(CORE_PIN10_BIT)
#define CORE_PIN11_BITMASK	_BV(CORE_PIN11_BIT)
#define CORE_PIN12_BITMASK	_BV(CORE_PIN12_BIT)
#define CORE_PIN13_BITMASK	_BV(CORE_PIN13_BIT)
#define CORE_PIN14_BITMASK	_BV(CORE_PIN14_BIT)
#define CORE_PIN15_BITMASK	_BV(CORE_PIN15_BIT)
#define CORE_PIN16_BITMASK	_BV(CORE_PIN16_BIT)
#define CORE_PIN17_BITMASK	_BV(CORE_PIN17_BIT)
#define CORE_PIN18_BITMASK	_BV(CORE_PIN18_BIT)
#define CORE_PIN19_BITMASK	_BV(CORE_PIN19_BIT)
#define CORE_PIN20_BITMASK	_BV(CORE_PIN20_BIT)
#define CORE_PIN0_PORTREG	PORTD
#define CORE_PIN1_PORTREG	PORTD
#define CORE_PIN2_PORTREG	PORTD
#define CORE_PIN3_PORTREG	PORTD
#define CORE_PIN4_PORTREG	PORTD
#define CORE_PIN5_PORTREG	PORTD
#define CORE_PIN6_PORTREG	PORTD
#define CORE_PIN7_PORTREG	PORTD
#define CORE_PIN8_PORTREG	PORTB
#define CORE_PIN9_PORTREG	PORTB
#define CORE_PIN10_PORTREG	PORTB
#define CORE_PIN11_PORTREG	PORTB
#define CORE_PIN12_PORTREG	PORTB
#define CORE_PIN13_PORTREG	PORTB
#define CORE_PIN14_PORTREG	PORTB
#define CORE_PIN15_PORTREG	PORTB
#define CORE_PIN16_PORTREG	PORTC
#define CORE_PIN17_PORTREG	PORTC
#define CORE_PIN18_PORTREG	PORTC
#define CORE_PIN19_PORTREG	PORTC
#define CORE_PIN20_PORTREG	PORTC
#define CORE_PIN0_DDRREG	DDRD
#define CORE_PIN1_DDRREG	DDRD
#define CORE_PIN2_DDRREG	DDRD
#define CORE_PIN3_DDRREG	DDRD
#define CORE_PIN4_DDRREG	DDRD
#define CORE_PIN5_DDRREG	DDRD
#define CORE_PIN6_DDRREG	DDRD
#define CORE_PIN7_DDRREG	DDRD
#define CORE_PIN8_DDRREG	DDRB
#define CORE_PIN9_DDRREG	DDRB
#define CORE_PIN10_DDRREG	DDRB
#define CORE_PIN11_DDRREG	DDRB
#define CORE_PIN12_DDRREG	DDRB
#define CORE_PIN13_DDRREG	DDRB
#define CORE_PIN14_DDRREG	DDRB
#define CORE_PIN15_DDRREG	DDRB
#define CORE_PIN16_DDRREG	DDRC
#define CORE_PIN17_DDRREG	DDRC
#define CORE_PIN18_DDRREG	DDRC
#define CORE_PIN19_DDRREG	DDRC
#define CORE_PIN20_DDRREG	DDRC
#define CORE_PIN0_PINREG	PIND
#define CORE_PIN1_PINREG	PIND
#define CORE_PIN2_PINREG	PIND
#define CORE_PIN3_PINREG	PIND
#define CORE_PIN4_PINREG	PIND
#define CORE_PIN5_PINREG	PIND
#define CORE_PIN6_PINREG	PIND
#define CORE_PIN7_PINREG	PIND
#define CORE_PIN8_PINREG	PINB
#define CORE_PIN9_PINREG	PINB
#define CORE_PIN10_PINREG	PINB
#define CORE_PIN11_PINREG	PINB
#define CORE_PIN12_PINREG	PINB
#define CORE_PIN13_PINREG	PINB
#define CORE_PIN14_PINREG	PINB
#define CORE_PIN15_PINREG	PINB
#define CORE_PIN16_PINREG	PINC
#define CORE_PIN17_PINREG	PINC
#define CORE_PIN18_PINREG	PINC
#define CORE_PIN19_PINREG	PINC
#define CORE_PIN20_PINREG	PINC
#define CORE_T1_PIN		PIN_B4
#define CORE_ICP1_PIN		PIN_C7
#define CORE_OC0A_PIN		PIN_B7
#define CORE_OC0B_PIN		PIN_D0
#define CORE_OC1A_PIN		PIN_C6
#define CORE_OC1B_PIN		PIN_C5
#define CORE_OC1C_PIN		PIN_B7
#define CORE_RXD1_PIN		PIN_D2
#define CORE_TXD1_PIN		PIN_D3
#define CORE_XCK1_PIN		PIN_D5
#define CORE_INT0_PIN		PIN_D0
#define CORE_INT1_PIN		PIN_D1
#define CORE_INT2_PIN		PIN_D2
#define CORE_INT3_PIN		PIN_D3
#define CORE_INT4_PIN		PIN_C2
#define CORE_INT5_PIN		PIN_D4
#define CORE_INT6_PIN		PIN_D6
#define CORE_INT7_PIN		PIN_D7
#define CORE_SS0_PIN		PIN_B0
#define CORE_MOSI0_PIN		PIN_B2
#define CORE_MISO0_PIN		PIN_B3
#define CORE_SCLK0_PIN		PIN_B1
#define CORE_AIN0_PIN		PIN_D1
#define CORE_AIN1_PIN		PIN_D2
#define CORE_PCINT0_PIN		PIN_B0
#define CORE_PCINT1_PIN		PIN_B1
#define CORE_PCINT2_PIN		PIN_B2
#define CORE_PCINT3_PIN		PIN_B3
#define CORE_PCINT4_PIN		PIN_B4
#define CORE_PCINT5_PIN		PIN_B5
#define CORE_PCINT6_PIN		PIN_B6
#define CORE_PCINT7_PIN		PIN_B7
#define CORE_PCINT8_PIN		PIN_C6
#define CORE_PCINT9_PIN		PIN_C5
#define CORE_PCINT10_PIN	PIN_C4
#define CORE_PCINT11_PIN	PIN_C2
#define CORE_PCINT12_PIN	PIN_D5
#define CORE_LED0_PIN		PIN_D6
#define CORE_PWM0_PIN		CORE_OC0B_PIN	// D0, 0
#define CORE_PWM1_PIN		CORE_OC1C_PIN	// B7, 15
#define CORE_PWM2_PIN		CORE_OC1A_PIN	// C6, 17
#define CORE_PWM3_PIN		CORE_OC1B_PIN	// C5, 18


////////////////////////////////////
// Teensy++ 1.0 & 2.0
////////////////////////////////////
#elif defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__)
#define CORE_NUM_TOTAL_PINS	46
#define CORE_NUM_DIGITAL	38
#define CORE_NUM_ANALOG		8
#define CORE_NUM_PWM		9
#define CORE_NUM_INTERRUPT	8
#define PIN_D0		0
#define PIN_D1		1
#define PIN_D2		2
#define PIN_D3		3
#define PIN_D4		4
#define PIN_D5		5
#define PIN_D6		6
#define PIN_D7		7
#define PIN_E0		8
#define PIN_E1		9
#define PIN_C0		10
#define PIN_C1		11
#define PIN_C2		12
#define PIN_C3		13
#define PIN_C4		14
#define PIN_C5		15
#define PIN_C6		16
#define PIN_C7		17
#define PIN_E6		18
#define PIN_E7		19
#define PIN_SS		20
#define PIN_SCLK	21
#define PIN_MOSI	22
#define PIN_MISO	23
#define PIN_B0		20
#define PIN_B1		21
#define PIN_B2		22
#define PIN_B3		23
#define PIN_B4		24
#define PIN_B5		25
#define PIN_B6		26
#define PIN_B7		27
#define PIN_A0		28
#define PIN_A1		29
#define PIN_A2		30
#define PIN_A3		31
#define PIN_A4		32
#define PIN_A5		33
#define PIN_A6		34
#define PIN_A7		35
#define PIN_E4		36
#define PIN_E5		37
#define PIN_F0		38
#define PIN_F1		39
#define PIN_F2		40
#define PIN_F3		41
#define PIN_F4		42
#define PIN_F5		43
#define PIN_F6		44
#define PIN_F7		45
#define CORE_PIN0_BIT		0
#define CORE_PIN1_BIT		1
#define CORE_PIN2_BIT		2
#define CORE_PIN3_BIT		3
#define CORE_PIN4_BIT		4
#define CORE_PIN5_BIT		5
#define CORE_PIN6_BIT		6
#define CORE_PIN7_BIT		7
#define CORE_PIN8_BIT		0
#define CORE_PIN9_BIT		1
#define CORE_PIN10_BIT		0
#define CORE_PIN11_BIT		1
#define CORE_PIN12_BIT		2
#define CORE_PIN13_BIT		3
#define CORE_PIN14_BIT		4
#define CORE_PIN15_BIT		5
#define CORE_PIN16_BIT		6
#define CORE_PIN17_BIT		7
#define CORE_PIN18_BIT		6
#define CORE_PIN19_BIT		7
#define CORE_PIN20_BIT		0
#define CORE_PIN21_BIT		1
#define CORE_PIN22_BIT		2
#define CORE_PIN23_BIT		3
#define CORE_PIN24_BIT		4
#define CORE_PIN25_BIT		5
#define CORE_PIN26_BIT		6
#define CORE_PIN27_BIT		7
#define CORE_PIN28_BIT		0
#define CORE_PIN29_BIT		1
#define CORE_PIN30_BIT		2
#define CORE_PIN31_BIT		3
#define CORE_PIN32_BIT		4
#define CORE_PIN33_BIT		5
#define CORE_PIN34_BIT		6
#define CORE_PIN35_BIT		7
#define CORE_PIN36_BIT		4
#define CORE_PIN37_BIT		5
#define CORE_PIN38_BIT		0
#define CORE_PIN39_BIT		1
#define CORE_PIN40_BIT		2
#define CORE_PIN41_BIT		3
#define CORE_PIN42_BIT		4
#define CORE_PIN43_BIT		5
#define CORE_PIN44_BIT		6
#define CORE_PIN45_BIT		7
#define CORE_PIN0_BITMASK	_BV(CORE_PIN0_BIT)
#define CORE_PIN1_BITMASK	_BV(CORE_PIN1_BIT)
#define CORE_PIN2_BITMASK	_BV(CORE_PIN2_BIT)
#define CORE_PIN3_BITMASK	_BV(CORE_PIN3_BIT)
#define CORE_PIN4_BITMASK	_BV(CORE_PIN4_BIT)
#define CORE_PIN5_BITMASK	_BV(CORE_PIN5_BIT)
#define CORE_PIN6_BITMASK	_BV(CORE_PIN6_BIT)
#define CORE_PIN7_BITMASK	_BV(CORE_PIN7_BIT)
#define CORE_PIN8_BITMASK	_BV(CORE_PIN8_BIT)
#define CORE_PIN9_BITMASK	_BV(CORE_PIN9_BIT)
#define CORE_PIN10_BITMASK	_BV(CORE_PIN10_BIT)
#define CORE_PIN11_BITMASK	_BV(CORE_PIN11_BIT)
#define CORE_PIN12_BITMASK	_BV(CORE_PIN12_BIT)
#define CORE_PIN13_BITMASK	_BV(CORE_PIN13_BIT)
#define CORE_PIN14_BITMASK	_BV(CORE_PIN14_BIT)
#define CORE_PIN15_BITMASK	_BV(CORE_PIN15_BIT)
#define CORE_PIN16_BITMASK	_BV(CORE_PIN16_BIT)
#define CORE_PIN17_BITMASK	_BV(CORE_PIN17_BIT)
#define CORE_PIN18_BITMASK	_BV(CORE_PIN18_BIT)
#define CORE_PIN19_BITMASK	_BV(CORE_PIN19_BIT)
#define CORE_PIN20_BITMASK	_BV(CORE_PIN20_BIT)
#define CORE_PIN21_BITMASK	_BV(CORE_PIN21_BIT)
#define CORE_PIN22_BITMASK	_BV(CORE_PIN22_BIT)
#define CORE_PIN23_BITMASK	_BV(CORE_PIN23_BIT)
#define CORE_PIN24_BITMASK	_BV(CORE_PIN24_BIT)
#define CORE_PIN25_BITMASK	_BV(CORE_PIN25_BIT)
#define CORE_PIN26_BITMASK	_BV(CORE_PIN26_BIT)
#define CORE_PIN27_BITMASK	_BV(CORE_PIN27_BIT)
#define CORE_PIN28_BITMASK	_BV(CORE_PIN28_BIT)
#define CORE_PIN29_BITMASK	_BV(CORE_PIN29_BIT)
#define CORE_PIN30_BITMASK	_BV(CORE_PIN30_BIT)
#define CORE_PIN31_BITMASK	_BV(CORE_PIN31_BIT)
#define CORE_PIN32_BITMASK	_BV(CORE_PIN32_BIT)
#define CORE_PIN33_BITMASK	_BV(CORE_PIN33_BIT)
#define CORE_PIN34_BITMASK	_BV(CORE_PIN34_BIT)
#define CORE_PIN35_BITMASK	_BV(CORE_PIN35_BIT)
#define CORE_PIN36_BITMASK	_BV(CORE_PIN36_BIT)
#define CORE_PIN37_BITMASK	_BV(CORE_PIN37_BIT)
#define CORE_PIN38_BITMASK	_BV(CORE_PIN38_BIT)
#define CORE_PIN39_BITMASK	_BV(CORE_PIN39_BIT)
#define CORE_PIN40_BITMASK	_BV(CORE_PIN40_BIT)
#define CORE_PIN41_BITMASK	_BV(CORE_PIN41_BIT)
#define CORE_PIN42_BITMASK	_BV(CORE_PIN42_BIT)
#define CORE_PIN43_BITMASK	_BV(CORE_PIN43_BIT)
#define CORE_PIN44_BITMASK	_BV(CORE_PIN44_BIT)
#define CORE_PIN45_BITMASK	_BV(CORE_PIN45_BIT)
#define CORE_PIN0_PORTREG	PORTD
#define CORE_PIN1_PORTREG	PORTD
#define CORE_PIN2_PORTREG	PORTD
#define CORE_PIN3_PORTREG	PORTD
#define CORE_PIN4_PORTREG	PORTD
#define CORE_PIN5_PORTREG	PORTD
#define CORE_PIN6_PORTREG	PORTD
#define CORE_PIN7_PORTREG	PORTD
#define CORE_PIN8_PORTREG	PORTE
#define CORE_PIN9_PORTREG	PORTE
#define CORE_PIN10_PORTREG	PORTC
#define CORE_PIN11_PORTREG	PORTC
#define CORE_PIN12_PORTREG	PORTC
#define CORE_PIN13_PORTREG	PORTC
#define CORE_PIN14_PORTREG	PORTC
#define CORE_PIN15_PORTREG	PORTC
#define CORE_PIN16_PORTREG	PORTC
#define CORE_PIN17_PORTREG	PORTC
#define CORE_PIN18_PORTREG	PORTE
#define CORE_PIN19_PORTREG	PORTE
#define CORE_PIN20_PORTREG	PORTB
#define CORE_PIN21_PORTREG	PORTB
#define CORE_PIN22_PORTREG	PORTB
#define CORE_PIN23_PORTREG	PORTB
#define CORE_PIN24_PORTREG	PORTB
#define CORE_PIN25_PORTREG	PORTB
#define CORE_PIN26_PORTREG	PORTB
#define CORE_PIN27_PORTREG	PORTB
#define CORE_PIN28_PORTREG	PORTA
#define CORE_PIN29_PORTREG	PORTA
#define CORE_PIN30_PORTREG	PORTA
#define CORE_PIN31_PORTREG	PORTA
#define CORE_PIN32_PORTREG	PORTA
#define CORE_PIN33_PORTREG	PORTA
#define CORE_PIN34_PORTREG	PORTA
#define CORE_PIN35_PORTREG	PORTA
#define CORE_PIN36_PORTREG	PORTE
#define CORE_PIN37_PORTREG	PORTE
#define CORE_PIN38_PORTREG	PORTF
#define CORE_PIN39_PORTREG	PORTF
#define CORE_PIN40_PORTREG	PORTF
#define CORE_PIN41_PORTREG	PORTF
#define CORE_PIN42_PORTREG	PORTF
#define CORE_PIN43_PORTREG	PORTF
#define CORE_PIN44_PORTREG	PORTF
#define CORE_PIN45_PORTREG	PORTF
#define CORE_PIN0_DDRREG	DDRD
#define CORE_PIN1_DDRREG	DDRD
#define CORE_PIN2_DDRREG	DDRD
#define CORE_PIN3_DDRREG	DDRD
#define CORE_PIN4_DDRREG	DDRD
#define CORE_PIN5_DDRREG	DDRD
#define CORE_PIN6_DDRREG	DDRD
#define CORE_PIN7_DDRREG	DDRD
#define CORE_PIN8_DDRREG	DDRE
#define CORE_PIN9_DDRREG	DDRE
#define CORE_PIN10_DDRREG	DDRC
#define CORE_PIN11_DDRREG	DDRC
#define CORE_PIN12_DDRREG	DDRC
#define CORE_PIN13_DDRREG	DDRC
#define CORE_PIN14_DDRREG	DDRC
#define CORE_PIN15_DDRREG	DDRC
#define CORE_PIN16_DDRREG	DDRC
#define CORE_PIN17_DDRREG	DDRC
#define CORE_PIN18_DDRREG	DDRE
#define CORE_PIN19_DDRREG	DDRE
#define CORE_PIN20_DDRREG	DDRB
#define CORE_PIN21_DDRREG	DDRB
#define CORE_PIN22_DDRREG	DDRB
#define CORE_PIN23_DDRREG	DDRB
#define CORE_PIN24_DDRREG	DDRB
#define CORE_PIN25_DDRREG	DDRB
#define CORE_PIN26_DDRREG	DDRB
#define CORE_PIN27_DDRREG	DDRB
#define CORE_PIN28_DDRREG	DDRA
#define CORE_PIN29_DDRREG	DDRA
#define CORE_PIN30_DDRREG	DDRA
#define CORE_PIN31_DDRREG	DDRA
#define CORE_PIN32_DDRREG	DDRA
#define CORE_PIN33_DDRREG	DDRA
#define CORE_PIN34_DDRREG	DDRA
#define CORE_PIN35_DDRREG	DDRA
#define CORE_PIN36_DDRREG	DDRE
#define CORE_PIN37_DDRREG	DDRE
#define CORE_PIN38_DDRREG	DDRF
#define CORE_PIN39_DDRREG	DDRF
#define CORE_PIN40_DDRREG	DDRF
#define CORE_PIN41_DDRREG	DDRF
#define CORE_PIN42_DDRREG	DDRF
#define CORE_PIN43_DDRREG	DDRF
#define CORE_PIN44_DDRREG	DDRF
#define CORE_PIN45_DDRREG	DDRF
#define CORE_PIN0_PINREG	PIND
#define CORE_PIN1_PINREG	PIND
#define CORE_PIN2_PINREG	PIND
#define CORE_PIN3_PINREG	PIND
#define CORE_PIN4_PINREG	PIND
#define CORE_PIN5_PINREG	PIND
#define CORE_PIN6_PINREG	PIND
#define CORE_PIN7_PINREG	PIND
#define CORE_PIN8_PINREG	PINE
#define CORE_PIN9_PINREG	PINE
#define CORE_PIN10_PINREG	PINC
#define CORE_PIN11_PINREG	PINC
#define CORE_PIN12_PINREG	PINC
#define CORE_PIN13_PINREG	PINC
#define CORE_PIN14_PINREG	PINC
#define CORE_PIN15_PINREG	PINC
#define CORE_PIN16_PINREG	PINC
#define CORE_PIN17_PINREG	PINC
#define CORE_PIN18_PINREG	PINE
#define CORE_PIN19_PINREG	PINE
#define CORE_PIN20_PINREG	PINB
#define CORE_PIN21_PINREG	PINB
#define CORE_PIN22_PINREG	PINB
#define CORE_PIN23_PINREG	PINB
#define CORE_PIN24_PINREG	PINB
#define CORE_PIN25_PINREG	PINB
#define CORE_PIN26_PINREG	PINB
#define CORE_PIN27_PINREG	PINB
#define CORE_PIN28_PINREG	PINA
#define CORE_PIN29_PINREG	PINA
#define CORE_PIN30_PINREG	PINA
#define CORE_PIN31_PINREG	PINA
#define CORE_PIN32_PINREG	PINA
#define CORE_PIN33_PINREG	PINA
#define CORE_PIN34_PINREG	PINA
#define CORE_PIN35_PINREG	PINA
#define CORE_PIN36_PINREG	PINE
#define CORE_PIN37_PINREG	PINE
#define CORE_PIN38_PINREG	PINF
#define CORE_PIN39_PINREG	PINF
#define CORE_PIN40_PINREG	PINF
#define CORE_PIN41_PINREG	PINF
#define CORE_PIN42_PINREG	PINF
#define CORE_PIN43_PINREG	PINF
#define CORE_PIN44_PINREG	PINF
#define CORE_PIN45_PINREG	PINF
#define CORE_ADC0_PIN		PIN_F0
#define CORE_ADC1_PIN		PIN_F1
#define CORE_ADC2_PIN		PIN_F2
#define CORE_ADC3_PIN		PIN_F3
#define CORE_ADC4_PIN		PIN_F4
#define CORE_ADC5_PIN		PIN_F5
#define CORE_ADC6_PIN		PIN_F6
#define CORE_ADC7_PIN		PIN_F7
#define CORE_RXD1_PIN           PIN_D2
#define CORE_TXD1_PIN           PIN_D3
#define CORE_XCK1_PIN           PIN_D5
#define CORE_SDA0_PIN           PIN_D1
#define CORE_SCL0_PIN           PIN_D0
#define CORE_INT0_PIN           PIN_D0
#define CORE_INT1_PIN           PIN_D1
#define CORE_INT2_PIN           PIN_D2
#define CORE_INT3_PIN           PIN_D3
#define CORE_INT4_PIN           PIN_E4
#define CORE_INT5_PIN           PIN_E5
#define CORE_INT6_PIN           PIN_E6
#define CORE_INT7_PIN           PIN_E7
#define CORE_SS0_PIN            PIN_B0
#define CORE_MOSI0_PIN          PIN_B2
#define CORE_MISO0_PIN          PIN_B3
#define CORE_SCLK0_PIN          PIN_B1
#define CORE_T0_PIN             PIN_D7
#define CORE_T1_PIN             PIN_D6
#define CORE_ICP1_PIN           PIN_D4
#define CORE_ICP3_PIN           PIN_C7
#define CORE_OC0A_PIN           PIN_B7
#define CORE_OC0B_PIN           PIN_D0
#define CORE_OC1A_PIN           PIN_B5
#define CORE_OC1B_PIN           PIN_B6
#define CORE_OC1C_PIN           PIN_B7
#define CORE_OC2A_PIN           PIN_B4
#define CORE_OC2B_PIN           PIN_D1
#define CORE_OC3A_PIN           PIN_C6
#define CORE_OC3B_PIN           PIN_C5
#define CORE_OC3C_PIN           PIN_C4
#define CORE_PCINT0_PIN		PIN_B0
#define CORE_PCINT1_PIN		PIN_B1
#define CORE_PCINT2_PIN		PIN_B2
#define CORE_PCINT3_PIN		PIN_B3
#define CORE_PCINT4_PIN		PIN_B4
#define CORE_PCINT5_PIN		PIN_B5
#define CORE_PCINT6_PIN		PIN_B6
#define CORE_PCINT7_PIN		PIN_B7
#define CORE_LED0_PIN		PIN_D6
#define CORE_PWM0_PIN		CORE_OC0B_PIN	// D0, 0
#define CORE_PWM1_PIN		CORE_OC2B_PIN	// D1, 1
#define CORE_PWM2_PIN		CORE_OC3C_PIN	// C4, 14
#define CORE_PWM3_PIN		CORE_OC3B_PIN	// C5, 15
#define CORE_PWM4_PIN		CORE_OC3A_PIN	// C6, 16
#define CORE_PWM5_PIN		CORE_OC2A_PIN	// B4, 24
#define CORE_PWM6_PIN		CORE_OC1A_PIN	// B5, 25
#define CORE_PWM7_PIN		CORE_OC1B_PIN	// B6, 26
#define CORE_PWM8_PIN		CORE_OC1C_PIN	// B7, 27
#define CORE_ANALOG0_PIN	PIN_F0
#define CORE_ANALOG1_PIN	PIN_F1
#define CORE_ANALOG2_PIN	PIN_F2
#define CORE_ANALOG3_PIN	PIN_F3
#define CORE_ANALOG4_PIN	PIN_F4
#define CORE_ANALOG5_PIN	PIN_F5
#define CORE_ANALOG6_PIN	PIN_F6
#define CORE_ANALOG7_PIN	PIN_F7

#endif


#define CORE_BIT(pin) CORE_PIN_CONCATENATE(pin, BIT)
#define CORE_BITMASK(pin) CORE_PIN_CONCATENATE(pin, BITMASK)
#define CORE_PORTREG(pin) CORE_PIN_CONCATENATE(pin, PORTREG)
#define CORE_DDRREG(pin) CORE_PIN_CONCATENATE(pin, DDRREG)
#define CORE_PINREG(pin) CORE_PIN_CONCATENATE(pin, PINREG)
#define CORE_PIN_CONCATENATE(pin, reg) (CORE_PIN ## pin ## _ ## reg)

#ifdef __cplusplus
extern "C"{
#endif


extern void _digitalWrite(void);
extern void _digitalWrite_HIGH(void);
extern void _digitalWrite_LOW(void);

static inline void digitalWrite(uint8_t, uint8_t) __attribute__((always_inline, unused));
static inline void digitalWrite(uint8_t pin, uint8_t val)
{
	if (__builtin_constant_p(pin)) {
		if (val) {
			if (pin == 0) {
				CORE_PIN0_PORTREG |= CORE_PIN0_BITMASK;
			} else if (pin == 1) {
				CORE_PIN1_PORTREG |= CORE_PIN1_BITMASK;
			} else if (pin == 2) {
				CORE_PIN2_PORTREG |= CORE_PIN2_BITMASK;
			} else if (pin == 3) {
				CORE_PIN3_PORTREG |= CORE_PIN3_BITMASK;
			} else if (pin == 4) {
				CORE_PIN4_PORTREG |= CORE_PIN4_BITMASK;
			} else if (pin == 5) {
				CORE_PIN5_PORTREG |= CORE_PIN5_BITMASK;
			} else if (pin == 6) {
				CORE_PIN6_PORTREG |= CORE_PIN6_BITMASK;
			} else if (pin == 7) {
				CORE_PIN7_PORTREG |= CORE_PIN7_BITMASK;
			} else if (pin == 8) {
				CORE_PIN8_PORTREG |= CORE_PIN8_BITMASK;
			} else if (pin == 9) {
				CORE_PIN9_PORTREG |= CORE_PIN9_BITMASK;
			} else if (pin == 10) {
				CORE_PIN10_PORTREG |= CORE_PIN10_BITMASK;
			} else if (pin == 11) {
				CORE_PIN11_PORTREG |= CORE_PIN11_BITMASK;
			} else if (pin == 12) {
				CORE_PIN12_PORTREG |= CORE_PIN12_BITMASK;
			} else if (pin == 13) {
				CORE_PIN13_PORTREG |= CORE_PIN13_BITMASK;
			} else if (pin == 14) {
				CORE_PIN14_PORTREG |= CORE_PIN14_BITMASK;
			} else if (pin == 15) {
				CORE_PIN15_PORTREG |= CORE_PIN15_BITMASK;
			} else if (pin == 16) {
				CORE_PIN16_PORTREG |= CORE_PIN16_BITMASK;
			} else if (pin == 17) {
				CORE_PIN17_PORTREG |= CORE_PIN17_BITMASK;
			} else if (pin == 18) {
				CORE_PIN18_PORTREG |= CORE_PIN18_BITMASK;
			} else if (pin == 19) {
				CORE_PIN19_PORTREG |= CORE_PIN19_BITMASK;
			} else if (pin == 20) {
				CORE_PIN20_PORTREG |= CORE_PIN20_BITMASK;
			}
			#if CORE_NUM_TOTAL_PINS > 21
			  else if (pin == 21) {
				CORE_PIN21_PORTREG |= CORE_PIN21_BITMASK;
			} else if (pin == 22) {
				CORE_PIN22_PORTREG |= CORE_PIN22_BITMASK;
			} else if (pin == 23) {
				CORE_PIN23_PORTREG |= CORE_PIN23_BITMASK;
			} else if (pin == 24) {
				CORE_PIN24_PORTREG |= CORE_PIN24_BITMASK;
			}
			#endif
			#if CORE_NUM_TOTAL_PINS > 25
			  else if (pin == 25) {
				CORE_PIN25_PORTREG |= CORE_PIN25_BITMASK;
			} else if (pin == 26) {
				CORE_PIN26_PORTREG |= CORE_PIN26_BITMASK;
			} else if (pin == 27) {
				CORE_PIN27_PORTREG |= CORE_PIN27_BITMASK;
			} else if (pin == 28) {
				CORE_PIN28_PORTREG |= CORE_PIN28_BITMASK;
			} else if (pin == 29) {
				CORE_PIN29_PORTREG |= CORE_PIN29_BITMASK;
			} else if (pin == 30) {
				CORE_PIN30_PORTREG |= CORE_PIN30_BITMASK;
			} else if (pin == 31) {
				CORE_PIN31_PORTREG |= CORE_PIN31_BITMASK;
			} else if (pin == 32) {
				CORE_PIN32_PORTREG |= CORE_PIN32_BITMASK;
			} else if (pin == 33) {
				CORE_PIN33_PORTREG |= CORE_PIN33_BITMASK;
			} else if (pin == 34) {
				CORE_PIN34_PORTREG |= CORE_PIN34_BITMASK;
			} else if (pin == 35) {
				CORE_PIN35_PORTREG |= CORE_PIN35_BITMASK;
			} else if (pin == 36) {
				CORE_PIN36_PORTREG |= CORE_PIN36_BITMASK;
			} else if (pin == 37) {
				CORE_PIN37_PORTREG |= CORE_PIN37_BITMASK;
			} else if (pin == 38) {
				CORE_PIN38_PORTREG |= CORE_PIN38_BITMASK;
			} else if (pin == 39) {
				CORE_PIN39_PORTREG |= CORE_PIN39_BITMASK;
			} else if (pin == 40) {
				CORE_PIN40_PORTREG |= CORE_PIN40_BITMASK;
			} else if (pin == 41) {
				CORE_PIN41_PORTREG |= CORE_PIN41_BITMASK;
			} else if (pin == 42) {
				CORE_PIN42_PORTREG |= CORE_PIN42_BITMASK;
			} else if (pin == 43) {
				CORE_PIN43_PORTREG |= CORE_PIN43_BITMASK;
			} else if (pin == 44) {
				CORE_PIN44_PORTREG |= CORE_PIN44_BITMASK;
			} else if (pin == 45) {
				CORE_PIN45_PORTREG |= CORE_PIN45_BITMASK;
			}
			#endif
		} else {
			if (pin == 0) {
				CORE_PIN0_PORTREG &= ~CORE_PIN0_BITMASK;
			} else if (pin == 1) {
				CORE_PIN1_PORTREG &= ~CORE_PIN1_BITMASK;
			} else if (pin == 2) {
				CORE_PIN2_PORTREG &= ~CORE_PIN2_BITMASK;
			} else if (pin == 3) {
				CORE_PIN3_PORTREG &= ~CORE_PIN3_BITMASK;
			} else if (pin == 4) {
				CORE_PIN4_PORTREG &= ~CORE_PIN4_BITMASK;
			} else if (pin == 5) {
				CORE_PIN5_PORTREG &= ~CORE_PIN5_BITMASK;
			} else if (pin == 6) {
				CORE_PIN6_PORTREG &= ~CORE_PIN6_BITMASK;
			} else if (pin == 7) {
				CORE_PIN7_PORTREG &= ~CORE_PIN7_BITMASK;
			} else if (pin == 8) {
				CORE_PIN8_PORTREG &= ~CORE_PIN8_BITMASK;
			} else if (pin == 9) {
				CORE_PIN9_PORTREG &= ~CORE_PIN9_BITMASK;
			} else if (pin == 10) {
				CORE_PIN10_PORTREG &= ~CORE_PIN10_BITMASK;
			} else if (pin == 11) {
				CORE_PIN11_PORTREG &= ~CORE_PIN11_BITMASK;
			} else if (pin == 12) {
				CORE_PIN12_PORTREG &= ~CORE_PIN12_BITMASK;
			} else if (pin == 13) {
				CORE_PIN13_PORTREG &= ~CORE_PIN13_BITMASK;
			} else if (pin == 14) {
				CORE_PIN14_PORTREG &= ~CORE_PIN14_BITMASK;
			} else if (pin == 15) {
				CORE_PIN15_PORTREG &= ~CORE_PIN15_BITMASK;
			} else if (pin == 16) {
				CORE_PIN16_PORTREG &= ~CORE_PIN16_BITMASK;
			} else if (pin == 17) {
				CORE_PIN17_PORTREG &= ~CORE_PIN17_BITMASK;
			} else if (pin == 18) {
				CORE_PIN18_PORTREG &= ~CORE_PIN18_BITMASK;
			} else if (pin == 19) {
				CORE_PIN19_PORTREG &= ~CORE_PIN19_BITMASK;
			} else if (pin == 20) {
				CORE_PIN20_PORTREG &= ~CORE_PIN20_BITMASK;
			}
			#if CORE_NUM_TOTAL_PINS > 21
			  else if (pin == 21) {
				CORE_PIN21_PORTREG &= ~CORE_PIN21_BITMASK;
			} else if (pin == 22) {
				CORE_PIN22_PORTREG &= ~CORE_PIN22_BITMASK;
			} else if (pin == 23) {
				CORE_PIN23_PORTREG &= ~CORE_PIN23_BITMASK;
			} else if (pin == 24) {
				CORE_PIN24_PORTREG &= ~CORE_PIN24_BITMASK;
			}
			#endif
			#if CORE_NUM_TOTAL_PINS > 25
			  else if (pin == 25) {
				CORE_PIN25_PORTREG &= ~CORE_PIN25_BITMASK;
			} else if (pin == 26) {
				CORE_PIN26_PORTREG &= ~CORE_PIN26_BITMASK;
			} else if (pin == 27) {
				CORE_PIN27_PORTREG &= ~CORE_PIN27_BITMASK;
			} else if (pin == 28) {
				CORE_PIN28_PORTREG &= ~CORE_PIN28_BITMASK;
			} else if (pin == 29) {
				CORE_PIN29_PORTREG &= ~CORE_PIN29_BITMASK;
			} else if (pin == 30) {
				CORE_PIN30_PORTREG &= ~CORE_PIN30_BITMASK;
			} else if (pin == 31) {
				CORE_PIN31_PORTREG &= ~CORE_PIN31_BITMASK;
			} else if (pin == 32) {
				CORE_PIN32_PORTREG &= ~CORE_PIN32_BITMASK;
			} else if (pin == 33) {
				CORE_PIN33_PORTREG &= ~CORE_PIN33_BITMASK;
			} else if (pin == 34) {
				CORE_PIN34_PORTREG &= ~CORE_PIN34_BITMASK;
			} else if (pin == 35) {
				CORE_PIN35_PORTREG &= ~CORE_PIN35_BITMASK;
			} else if (pin == 36) {
				CORE_PIN36_PORTREG &= ~CORE_PIN36_BITMASK;
			} else if (pin == 37) {
				CORE_PIN37_PORTREG &= ~CORE_PIN37_BITMASK;
			} else if (pin == 38) {
				CORE_PIN38_PORTREG &= ~CORE_PIN38_BITMASK;
			} else if (pin == 39) {
				CORE_PIN39_PORTREG &= ~CORE_PIN39_BITMASK;
			} else if (pin == 40) {
				CORE_PIN40_PORTREG &= ~CORE_PIN40_BITMASK;
			} else if (pin == 41) {
				CORE_PIN41_PORTREG &= ~CORE_PIN41_BITMASK;
			} else if (pin == 42) {
				CORE_PIN42_PORTREG &= ~CORE_PIN42_BITMASK;
			} else if (pin == 43) {
				CORE_PIN43_PORTREG &= ~CORE_PIN43_BITMASK;
			} else if (pin == 44) {
				CORE_PIN44_PORTREG &= ~CORE_PIN44_BITMASK;
			} else if (pin == 45) {
				CORE_PIN45_PORTREG &= ~CORE_PIN45_BITMASK;
			}
			#endif
		}
		if (pin == CORE_OC0B_PIN) {
			_SFR_BYTE(TCCR0A) &= ~(1<<COM0B1);
		} else if (pin == CORE_OC1A_PIN) {
			TCCR1A &= ~(1<<COM1A1);
		} else if (pin == CORE_OC1B_PIN) {
			TCCR1A &= ~(1<<COM1B1);
		} else if (pin == CORE_OC1C_PIN) {
			TCCR1A &= ~(1<<COM1C1);
		}
		#if defined(__AVR_ATmega32U4__)
		  else if (pin == CORE_OC3A_PIN) {
			TCCR3A &= ~(1<<COM3A1);
		} else if (pin == CORE_OC4A_PIN) {
			TCCR4A &= ~(1<<COM4A1);
		} else if (pin == CORE_OC4D_PIN) {
			TCCR4C &= ~(1<<COM4D1);
		}
		#endif
		#if defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__)
		  else if (pin == CORE_OC2A_PIN) {
			TCCR2A &= ~(1<<COM2A1);
		} else if (pin == CORE_OC2B_PIN) {
			TCCR2A &= ~(1<<COM2B1);
		} else if (pin == CORE_OC3A_PIN) {
			TCCR3A &= ~(1<<COM3A1);
		} else if (pin == CORE_OC3B_PIN) {
			TCCR3A &= ~(1<<COM3B1);
		} else if (pin == CORE_OC3C_PIN) {
			TCCR3A &= ~(1<<COM3C1);
		}
		#endif
	} else {
		if (__builtin_constant_p(val)) {
			if (val) {
				uint16_t tmp = (pin);
				asm volatile(
					#if defined(DIGITAL_WRITE_EXPENSIVE_INLINE_OPTIMIZATION)
					#if !defined(DIGITAL_WRITE_RISKY_OMIT_OVERFLOW_CHECK)
					"cpi	r30, %1"					"\n\t"
					"brsh	L%=end1"					"\n\t"
					#endif
					"lsl	r30"						"\n\t"
					//"clr	r31"						"\n\t"
					"subi	r30, lo8(-(pm(_digitalWrite_HIGH_TABLE)))"	"\n\t"
					"sbci	r31, hi8(-(pm(_digitalWrite_HIGH_TABLE)))"	"\n\t"
					"icall"							"\n\t"
					"L%=end1:"						"\n"
					#else
					"call _digitalWrite_HIGH"
					#endif
					: "+z" (tmp)
					: "I" (CORE_NUM_TOTAL_PINS)
				);
			} else {
				uint16_t tmp = (pin);
				asm volatile(
					#if defined(DIGITAL_WRITE_EXPENSIVE_INLINE_OPTIMIZATION)
					#if !defined(DIGITAL_WRITE_RISKY_OMIT_OVERFLOW_CHECK)
					"cpi	r30, %1"					"\n\t"
					"brsh	L%=end2"					"\n\t"
					#endif
					"lsl	r30"						"\n\t"
					//"clr	r31"						"\n\t"
					"subi	r30, lo8(-(pm(_digitalWrite_LOW_TABLE)))"	"\n\t"
					"sbci	r31, hi8(-(pm(_digitalWrite_LOW_TABLE)))"	"\n\t"
					"icall"							"\n\t"
					"L%=end2:"						"\n"
					#else
					"call _digitalWrite_LOW"
					#endif
					: "+z" (tmp)
					: "I" (CORE_NUM_TOTAL_PINS)
				);
			}
		} else {
			uint16_t tmp1 = (pin);
			uint8_t tmp2 = (val);
			asm volatile(
				#if defined(DIGITAL_WRITE_EXPENSIVE_INLINE_OPTIMIZATION)
				#if !defined(DIGITAL_WRITE_RISKY_OMIT_OVERFLOW_CHECK)
				"cpi	%0, %2"						"\n\t"
				"brsh	L%=end3"					"\n\t"
				#endif
				"lsl	r30"						"\n\t"
				//"clr	r31"						"\n\t"
				"tst	%1"						"\n\t"
				"breq	L%=low"						"\n\t"
				"subi	r30, lo8(-(pm(_digitalWrite_HIGH_TABLE)))"	"\n\t"
				"sbci	r31, hi8(-(pm(_digitalWrite_HIGH_TABLE)))"	"\n\t"
				"icall"							"\n\t"
				"rjmp	L%=end3"					"\n\t"
				"L%=low:"						"\n\t"
				"subi	r30, lo8(-(pm(_digitalWrite_LOW_TABLE)))"	"\n\t"
				"sbci	r31, hi8(-(pm(_digitalWrite_LOW_TABLE)))"	"\n\t"
				"icall"							"\n\t"
				"L%=end3:"						"\n\t"
				: "+z" (tmp1)
				: "d" (tmp2), "I" (CORE_NUM_TOTAL_PINS)
				#else
				"mov __tmp_reg__, %1"  "\n\t"
				"call _digitalWrite"
				: "+z" (tmp1)
				: "r" (tmp2)
				#endif
			);
		}
	}
}

extern void _digitalRead(void) __attribute__((noinline));

static inline uint8_t digitalRead(uint8_t) __attribute__((always_inline, unused));
static inline uint8_t digitalRead(uint8_t pin)
{
	if (__builtin_constant_p(pin)) {
		if (pin == 0) {
			return (CORE_PIN0_PINREG & CORE_PIN0_BITMASK) ? 1 : 0;
		} else if (pin == 1) {
			return (CORE_PIN1_PINREG & CORE_PIN1_BITMASK) ? 1 : 0;
		} else if (pin == 2) {
			return (CORE_PIN2_PINREG & CORE_PIN2_BITMASK) ? 1 : 0;
		} else if (pin == 3) {
			return (CORE_PIN3_PINREG & CORE_PIN3_BITMASK) ? 1 : 0;
		} else if (pin == 4) {
			return (CORE_PIN4_PINREG & CORE_PIN4_BITMASK) ? 1 : 0;
		} else if (pin == 5) {
			return (CORE_PIN5_PINREG & CORE_PIN5_BITMASK) ? 1 : 0;
		} else if (pin == 6) {
			return (CORE_PIN6_PINREG & CORE_PIN6_BITMASK) ? 1 : 0;
		} else if (pin == 7) {
			return (CORE_PIN7_PINREG & CORE_PIN7_BITMASK) ? 1 : 0;
		} else if (pin == 8) {
			return (CORE_PIN8_PINREG & CORE_PIN8_BITMASK) ? 1 : 0;
		} else if (pin == 9) {
			return (CORE_PIN9_PINREG & CORE_PIN9_BITMASK) ? 1 : 0;
		} else if (pin == 10) {
			return (CORE_PIN10_PINREG & CORE_PIN10_BITMASK) ? 1 : 0;
		} else if (pin == 11) {
			return (CORE_PIN11_PINREG & CORE_PIN11_BITMASK) ? 1 : 0;
		} else if (pin == 12) {
			return (CORE_PIN12_PINREG & CORE_PIN12_BITMASK) ? 1 : 0;
		} else if (pin == 13) {
			return (CORE_PIN13_PINREG & CORE_PIN13_BITMASK) ? 1 : 0;
		} else if (pin == 14) {
			return (CORE_PIN14_PINREG & CORE_PIN14_BITMASK) ? 1 : 0;
		} else if (pin == 15) {
			return (CORE_PIN15_PINREG & CORE_PIN15_BITMASK) ? 1 : 0;
		} else if (pin == 16) {
			return (CORE_PIN16_PINREG & CORE_PIN16_BITMASK) ? 1 : 0;
		} else if (pin == 17) {
			return (CORE_PIN17_PINREG & CORE_PIN17_BITMASK) ? 1 : 0;
		} else if (pin == 18) {
			return (CORE_PIN18_PINREG & CORE_PIN18_BITMASK) ? 1 : 0;
		} else if (pin == 19) {
			return (CORE_PIN19_PINREG & CORE_PIN19_BITMASK) ? 1 : 0;
		} else if (pin == 20) {
			return (CORE_PIN20_PINREG & CORE_PIN20_BITMASK) ? 1 : 0;
		}
		#if CORE_NUM_TOTAL_PINS > 21
		  else if (pin == 21) {
			return (CORE_PIN21_PINREG & CORE_PIN21_BITMASK) ? 1 : 0;
		} else if (pin == 22) {
			return (CORE_PIN22_PINREG & CORE_PIN22_BITMASK) ? 1 : 0;
		} else if (pin == 23) {
			return (CORE_PIN23_PINREG & CORE_PIN23_BITMASK) ? 1 : 0;
		} else if (pin == 24) {
			return (CORE_PIN24_PINREG & CORE_PIN24_BITMASK) ? 1 : 0;
		}
		#endif
		#if CORE_NUM_TOTAL_PINS > 25
		  else if (pin == 25) {
			return (CORE_PIN25_PINREG & CORE_PIN25_BITMASK) ? 1 : 0;
		} else if (pin == 26) {
			return (CORE_PIN26_PINREG & CORE_PIN26_BITMASK) ? 1 : 0;
		} else if (pin == 27) {
			return (CORE_PIN27_PINREG & CORE_PIN27_BITMASK) ? 1 : 0;
		} else if (pin == 28) {
			return (CORE_PIN28_PINREG & CORE_PIN28_BITMASK) ? 1 : 0;
		} else if (pin == 29) {
			return (CORE_PIN29_PINREG & CORE_PIN29_BITMASK) ? 1 : 0;
		} else if (pin == 30) {
			return (CORE_PIN30_PINREG & CORE_PIN30_BITMASK) ? 1 : 0;
		} else if (pin == 31) {
			return (CORE_PIN31_PINREG & CORE_PIN31_BITMASK) ? 1 : 0;
		} else if (pin == 32) {
			return (CORE_PIN32_PINREG & CORE_PIN32_BITMASK) ? 1 : 0;
		} else if (pin == 33) {
			return (CORE_PIN33_PINREG & CORE_PIN33_BITMASK) ? 1 : 0;
		} else if (pin == 34) {
			return (CORE_PIN34_PINREG & CORE_PIN34_BITMASK) ? 1 : 0;
		} else if (pin == 35) {
			return (CORE_PIN35_PINREG & CORE_PIN35_BITMASK) ? 1 : 0;
		} else if (pin == 36) {
			return (CORE_PIN36_PINREG & CORE_PIN36_BITMASK) ? 1 : 0;
		} else if (pin == 37) {
			return (CORE_PIN37_PINREG & CORE_PIN37_BITMASK) ? 1 : 0;
		} else if (pin == 38) {
			return (CORE_PIN38_PINREG & CORE_PIN38_BITMASK) ? 1 : 0;
		} else if (pin == 39) {
			return (CORE_PIN39_PINREG & CORE_PIN39_BITMASK) ? 1 : 0;
		} else if (pin == 40) {
			return (CORE_PIN40_PINREG & CORE_PIN40_BITMASK) ? 1 : 0;
		} else if (pin == 41) {
			return (CORE_PIN41_PINREG & CORE_PIN41_BITMASK) ? 1 : 0;
		} else if (pin == 42) {
			return (CORE_PIN42_PINREG & CORE_PIN42_BITMASK) ? 1 : 0;
		} else if (pin == 43) {
			return (CORE_PIN43_PINREG & CORE_PIN43_BITMASK) ? 1 : 0;
		} else if (pin == 44) {
			return (CORE_PIN44_PINREG & CORE_PIN44_BITMASK) ? 1 : 0;
		} else if (pin == 45) {
			return (CORE_PIN45_PINREG & CORE_PIN45_BITMASK) ? 1 : 0;
		}
		#endif
		  else {
			return 0;
		}
	} else {
		uint16_t tmp = (pin);
		asm volatile(
			"call _digitalRead"
			: "+z" (tmp)
			:);
		return (uint8_t)tmp;
	}
}

extern void _pinMode(uint8_t pin, uint8_t mode) __attribute__((noinline));
extern void _pinMode_output(uint8_t pin) __attribute__((noinline));
extern void _pinMode_input(uint8_t pin) __attribute__((noinline));
extern void _pinMode_input_pullup(uint8_t pin) __attribute__((noinline));

static inline void pinMode(uint8_t, uint8_t) __attribute__((always_inline, unused));
static inline void pinMode(uint8_t pin, uint8_t mode)
{
	if (__builtin_constant_p(pin) && __builtin_constant_p(mode)) {
		if (mode == OUTPUT) {
			if (pin == 0) {
				CORE_PIN0_DDRREG |= CORE_PIN0_BITMASK;
			} else if (pin == 1) {
				CORE_PIN1_DDRREG |= CORE_PIN1_BITMASK;
			} else if (pin == 2) {
				CORE_PIN2_DDRREG |= CORE_PIN2_BITMASK;
			} else if (pin == 3) {
				CORE_PIN3_DDRREG |= CORE_PIN3_BITMASK;
			} else if (pin == 4) {
				CORE_PIN4_DDRREG |= CORE_PIN4_BITMASK;
			} else if (pin == 5) {
				CORE_PIN5_DDRREG |= CORE_PIN5_BITMASK;
			} else if (pin == 6) {
				CORE_PIN6_DDRREG |= CORE_PIN6_BITMASK;
			} else if (pin == 7) {
				CORE_PIN7_DDRREG |= CORE_PIN7_BITMASK;
			} else if (pin == 8) {
				CORE_PIN8_DDRREG |= CORE_PIN8_BITMASK;
			} else if (pin == 9) {
				CORE_PIN9_DDRREG |= CORE_PIN9_BITMASK;
			} else if (pin == 10) {
				CORE_PIN10_DDRREG |= CORE_PIN10_BITMASK;
			} else if (pin == 11) {
				CORE_PIN11_DDRREG |= CORE_PIN11_BITMASK;
			} else if (pin == 12) {
				CORE_PIN12_DDRREG |= CORE_PIN12_BITMASK;
			} else if (pin == 13) {
				CORE_PIN13_DDRREG |= CORE_PIN13_BITMASK;
			} else if (pin == 14) {
				CORE_PIN14_DDRREG |= CORE_PIN14_BITMASK;
			} else if (pin == 15) {
				CORE_PIN15_DDRREG |= CORE_PIN15_BITMASK;
			} else if (pin == 16) {
				CORE_PIN16_DDRREG |= CORE_PIN16_BITMASK;
			} else if (pin == 17) {
				CORE_PIN17_DDRREG |= CORE_PIN17_BITMASK;
			} else if (pin == 18) {
				CORE_PIN18_DDRREG |= CORE_PIN18_BITMASK;
			} else if (pin == 19) {
				CORE_PIN19_DDRREG |= CORE_PIN19_BITMASK;
			} else if (pin == 20) {
				CORE_PIN20_DDRREG |= CORE_PIN20_BITMASK;
			}
			#if CORE_NUM_TOTAL_PINS > 21
			  else if (pin == 21) {
				CORE_PIN21_DDRREG |= CORE_PIN21_BITMASK;
			} else if (pin == 22) {
				CORE_PIN22_DDRREG |= CORE_PIN22_BITMASK;
			} else if (pin == 23) {
				CORE_PIN23_DDRREG |= CORE_PIN23_BITMASK;
			} else if (pin == 24) {
				CORE_PIN24_DDRREG |= CORE_PIN24_BITMASK;
			}
			#endif
			#if CORE_NUM_TOTAL_PINS > 25
			  else if (pin == 25) {
				CORE_PIN25_DDRREG |= CORE_PIN25_BITMASK;
			} else if (pin == 26) {
				CORE_PIN26_DDRREG |= CORE_PIN26_BITMASK;
			} else if (pin == 27) {
				CORE_PIN27_DDRREG |= CORE_PIN27_BITMASK;
			} else if (pin == 28) {
				CORE_PIN28_DDRREG |= CORE_PIN28_BITMASK;
			} else if (pin == 29) {
				CORE_PIN29_DDRREG |= CORE_PIN29_BITMASK;
			} else if (pin == 30) {
				CORE_PIN30_DDRREG |= CORE_PIN30_BITMASK;
			} else if (pin == 31) {
				CORE_PIN31_DDRREG |= CORE_PIN31_BITMASK;
			} else if (pin == 32) {
				CORE_PIN32_DDRREG |= CORE_PIN32_BITMASK;
			} else if (pin == 33) {
				CORE_PIN33_DDRREG |= CORE_PIN33_BITMASK;
			} else if (pin == 34) {
				CORE_PIN34_DDRREG |= CORE_PIN34_BITMASK;
			} else if (pin == 35) {
				CORE_PIN35_DDRREG |= CORE_PIN35_BITMASK;
			} else if (pin == 36) {
				CORE_PIN36_DDRREG |= CORE_PIN36_BITMASK;
			} else if (pin == 37) {
				CORE_PIN37_DDRREG |= CORE_PIN37_BITMASK;
			} else if (pin == 38) {
				CORE_PIN38_DDRREG |= CORE_PIN38_BITMASK;
			} else if (pin == 39) {
				CORE_PIN39_DDRREG |= CORE_PIN39_BITMASK;
			} else if (pin == 40) {
				CORE_PIN40_DDRREG |= CORE_PIN40_BITMASK;
			} else if (pin == 41) {
				CORE_PIN41_DDRREG |= CORE_PIN41_BITMASK;
			} else if (pin == 42) {
				CORE_PIN42_DDRREG |= CORE_PIN42_BITMASK;
			} else if (pin == 43) {
				CORE_PIN43_DDRREG |= CORE_PIN43_BITMASK;
			} else if (pin == 44) {
				CORE_PIN44_DDRREG |= CORE_PIN44_BITMASK;
			} else if (pin == 45) {
				CORE_PIN45_DDRREG |= CORE_PIN45_BITMASK;
			}
			#endif
		} else if (mode == INPUT_PULLUP) {
			if (pin == 0) {
				CORE_PIN0_DDRREG  &= ~CORE_PIN0_BITMASK;
				CORE_PIN0_PORTREG |=  CORE_PIN0_BITMASK;
			} else if (pin == 1) {
				CORE_PIN1_DDRREG  &= ~CORE_PIN1_BITMASK;
				CORE_PIN1_PORTREG |=  CORE_PIN1_BITMASK;
			} else if (pin == 2) {
				CORE_PIN2_DDRREG  &= ~CORE_PIN2_BITMASK;
				CORE_PIN2_PORTREG |=  CORE_PIN2_BITMASK;
			} else if (pin == 3) {
				CORE_PIN3_DDRREG  &= ~CORE_PIN3_BITMASK;
				CORE_PIN3_PORTREG |=  CORE_PIN3_BITMASK;
			} else if (pin == 4) {
				CORE_PIN4_DDRREG  &= ~CORE_PIN4_BITMASK;
				CORE_PIN4_PORTREG |=  CORE_PIN4_BITMASK;
			} else if (pin == 5) {
				CORE_PIN5_DDRREG  &= ~CORE_PIN5_BITMASK;
				CORE_PIN5_PORTREG |=  CORE_PIN5_BITMASK;
			} else if (pin == 6) {
				CORE_PIN6_DDRREG  &= ~CORE_PIN6_BITMASK;
				CORE_PIN6_PORTREG |=  CORE_PIN6_BITMASK;
			} else if (pin == 7) {
				CORE_PIN7_DDRREG  &= ~CORE_PIN7_BITMASK;
				CORE_PIN7_PORTREG |=  CORE_PIN7_BITMASK;
			} else if (pin == 8) {
				CORE_PIN8_DDRREG  &= ~CORE_PIN8_BITMASK;
				CORE_PIN8_PORTREG |=  CORE_PIN8_BITMASK;
			} else if (pin == 9) {
				CORE_PIN9_DDRREG  &= ~CORE_PIN9_BITMASK;
				CORE_PIN9_PORTREG |=  CORE_PIN9_BITMASK;
			} else if (pin == 10) {
				CORE_PIN10_DDRREG  &= ~CORE_PIN10_BITMASK;
				CORE_PIN10_PORTREG |=  CORE_PIN10_BITMASK;
			} else if (pin == 11) {
				CORE_PIN11_DDRREG  &= ~CORE_PIN11_BITMASK;
				CORE_PIN11_PORTREG |=  CORE_PIN11_BITMASK;
			} else if (pin == 12) {
				CORE_PIN12_DDRREG  &= ~CORE_PIN12_BITMASK;
				CORE_PIN12_PORTREG |=  CORE_PIN12_BITMASK;
			} else if (pin == 13) {
				CORE_PIN13_DDRREG  &= ~CORE_PIN13_BITMASK;
				CORE_PIN13_PORTREG |=  CORE_PIN13_BITMASK;
			} else if (pin == 14) {
				CORE_PIN14_DDRREG  &= ~CORE_PIN14_BITMASK;
				CORE_PIN14_PORTREG |=  CORE_PIN14_BITMASK;
			} else if (pin == 15) {
				CORE_PIN15_DDRREG  &= ~CORE_PIN15_BITMASK;
				CORE_PIN15_PORTREG |=  CORE_PIN15_BITMASK;
			} else if (pin == 16) {
				CORE_PIN16_DDRREG  &= ~CORE_PIN16_BITMASK;
				CORE_PIN16_PORTREG |=  CORE_PIN16_BITMASK;
			} else if (pin == 17) {
				CORE_PIN17_DDRREG  &= ~CORE_PIN17_BITMASK;
				CORE_PIN17_PORTREG |=  CORE_PIN17_BITMASK;
			} else if (pin == 18) {
				CORE_PIN18_DDRREG  &= ~CORE_PIN18_BITMASK;
				CORE_PIN18_PORTREG |=  CORE_PIN18_BITMASK;
			} else if (pin == 19) {
				CORE_PIN19_DDRREG  &= ~CORE_PIN19_BITMASK;
				CORE_PIN19_PORTREG |=  CORE_PIN19_BITMASK;
			} else if (pin == 20) {
				CORE_PIN20_DDRREG  &= ~CORE_PIN20_BITMASK;
				CORE_PIN20_PORTREG |=  CORE_PIN20_BITMASK;
			}
			#if CORE_NUM_TOTAL_PINS > 21
			  else if (pin == 21) {
				CORE_PIN21_DDRREG  &= ~CORE_PIN21_BITMASK;
				CORE_PIN21_PORTREG |=  CORE_PIN21_BITMASK;
			} else if (pin == 22) {
				CORE_PIN22_DDRREG  &= ~CORE_PIN22_BITMASK;
				CORE_PIN22_PORTREG |=  CORE_PIN22_BITMASK;
			} else if (pin == 23) {
				CORE_PIN23_DDRREG  &= ~CORE_PIN23_BITMASK;
				CORE_PIN23_PORTREG |=  CORE_PIN23_BITMASK;
			} else if (pin == 24) {
				CORE_PIN24_DDRREG  &= ~CORE_PIN24_BITMASK;
				CORE_PIN24_PORTREG |=  CORE_PIN24_BITMASK;
			}
			#endif
			#if CORE_NUM_TOTAL_PINS > 25
			  else if (pin == 25) {
				CORE_PIN25_DDRREG  &= ~CORE_PIN25_BITMASK;
				CORE_PIN25_PORTREG |=  CORE_PIN25_BITMASK;
			} else if (pin == 26) {
				CORE_PIN26_DDRREG  &= ~CORE_PIN26_BITMASK;
				CORE_PIN26_PORTREG |=  CORE_PIN26_BITMASK;
			} else if (pin == 27) {
				CORE_PIN27_DDRREG  &= ~CORE_PIN27_BITMASK;
				CORE_PIN27_PORTREG |=  CORE_PIN27_BITMASK;
			} else if (pin == 28) {
				CORE_PIN28_DDRREG  &= ~CORE_PIN28_BITMASK;
				CORE_PIN28_PORTREG |=  CORE_PIN28_BITMASK;
			} else if (pin == 29) {
				CORE_PIN29_DDRREG  &= ~CORE_PIN29_BITMASK;
				CORE_PIN29_PORTREG |=  CORE_PIN29_BITMASK;
			} else if (pin == 30) {
				CORE_PIN30_DDRREG  &= ~CORE_PIN30_BITMASK;
				CORE_PIN30_PORTREG |=  CORE_PIN30_BITMASK;
			} else if (pin == 31) {
				CORE_PIN31_DDRREG  &= ~CORE_PIN31_BITMASK;
				CORE_PIN31_PORTREG |=  CORE_PIN31_BITMASK;
			} else if (pin == 32) {
				CORE_PIN32_DDRREG  &= ~CORE_PIN32_BITMASK;
				CORE_PIN32_PORTREG |=  CORE_PIN32_BITMASK;
			} else if (pin == 33) {
				CORE_PIN33_DDRREG  &= ~CORE_PIN33_BITMASK;
				CORE_PIN33_PORTREG |=  CORE_PIN33_BITMASK;
			} else if (pin == 34) {
				CORE_PIN34_DDRREG  &= ~CORE_PIN34_BITMASK;
				CORE_PIN34_PORTREG |=  CORE_PIN34_BITMASK;
			} else if (pin == 35) {
				CORE_PIN35_DDRREG  &= ~CORE_PIN35_BITMASK;
				CORE_PIN35_PORTREG |=  CORE_PIN35_BITMASK;
			} else if (pin == 36) {
				CORE_PIN36_DDRREG  &= ~CORE_PIN36_BITMASK;
				CORE_PIN36_PORTREG |=  CORE_PIN36_BITMASK;
			} else if (pin == 37) {
				CORE_PIN37_DDRREG  &= ~CORE_PIN37_BITMASK;
				CORE_PIN37_PORTREG |=  CORE_PIN37_BITMASK;
			} else if (pin == 38) {
				CORE_PIN38_DDRREG  &= ~CORE_PIN38_BITMASK;
				CORE_PIN38_PORTREG |=  CORE_PIN38_BITMASK;
			} else if (pin == 39) {
				CORE_PIN39_DDRREG  &= ~CORE_PIN39_BITMASK;
				CORE_PIN39_PORTREG |=  CORE_PIN39_BITMASK;
			} else if (pin == 40) {
				CORE_PIN40_DDRREG  &= ~CORE_PIN40_BITMASK;
				CORE_PIN40_PORTREG |=  CORE_PIN40_BITMASK;
			} else if (pin == 41) {
				CORE_PIN41_DDRREG  &= ~CORE_PIN41_BITMASK;
				CORE_PIN41_PORTREG |=  CORE_PIN41_BITMASK;
			} else if (pin == 42) {
				CORE_PIN42_DDRREG  &= ~CORE_PIN42_BITMASK;
				CORE_PIN42_PORTREG |=  CORE_PIN42_BITMASK;
			} else if (pin == 43) {
				CORE_PIN43_DDRREG  &= ~CORE_PIN43_BITMASK;
				CORE_PIN43_PORTREG |=  CORE_PIN43_BITMASK;
			} else if (pin == 44) {
				CORE_PIN44_DDRREG  &= ~CORE_PIN44_BITMASK;
				CORE_PIN44_PORTREG |=  CORE_PIN44_BITMASK;
			} else if (pin == 45) {
				CORE_PIN45_DDRREG  &= ~CORE_PIN45_BITMASK;
				CORE_PIN45_PORTREG |=  CORE_PIN45_BITMASK;
			}
			#endif
		} else {
			if (pin == 0) {
				CORE_PIN0_DDRREG  &= ~CORE_PIN0_BITMASK;
				CORE_PIN0_PORTREG &= ~CORE_PIN0_BITMASK;
			} else if (pin == 1) {
				CORE_PIN1_DDRREG  &= ~CORE_PIN1_BITMASK;
				CORE_PIN1_PORTREG &= ~CORE_PIN1_BITMASK;
			} else if (pin == 2) {
				CORE_PIN2_DDRREG  &= ~CORE_PIN2_BITMASK;
				CORE_PIN2_PORTREG &= ~CORE_PIN2_BITMASK;
			} else if (pin == 3) {
				CORE_PIN3_DDRREG  &= ~CORE_PIN3_BITMASK;
				CORE_PIN3_PORTREG &= ~CORE_PIN3_BITMASK;
			} else if (pin == 4) {
				CORE_PIN4_DDRREG  &= ~CORE_PIN4_BITMASK;
				CORE_PIN4_PORTREG &= ~CORE_PIN4_BITMASK;
			} else if (pin == 5) {
				CORE_PIN5_DDRREG  &= ~CORE_PIN5_BITMASK;
				CORE_PIN5_PORTREG &= ~CORE_PIN5_BITMASK;
			} else if (pin == 6) {
				CORE_PIN6_DDRREG  &= ~CORE_PIN6_BITMASK;
				CORE_PIN6_PORTREG &= ~CORE_PIN6_BITMASK;
			} else if (pin == 7) {
				CORE_PIN7_DDRREG  &= ~CORE_PIN7_BITMASK;
				CORE_PIN7_PORTREG &= ~CORE_PIN7_BITMASK;
			} else if (pin == 8) {
				CORE_PIN8_DDRREG  &= ~CORE_PIN8_BITMASK;
				CORE_PIN8_PORTREG &= ~CORE_PIN8_BITMASK;
			} else if (pin == 9) {
				CORE_PIN9_DDRREG  &= ~CORE_PIN9_BITMASK;
				CORE_PIN9_PORTREG &= ~CORE_PIN9_BITMASK;
			} else if (pin == 10) {
				CORE_PIN10_DDRREG  &= ~CORE_PIN10_BITMASK;
				CORE_PIN10_PORTREG &= ~CORE_PIN10_BITMASK;
			} else if (pin == 11) {
				CORE_PIN11_DDRREG  &= ~CORE_PIN11_BITMASK;
				CORE_PIN11_PORTREG &= ~CORE_PIN11_BITMASK;
			} else if (pin == 12) {
				CORE_PIN12_DDRREG  &= ~CORE_PIN12_BITMASK;
				CORE_PIN12_PORTREG &= ~CORE_PIN12_BITMASK;
			} else if (pin == 13) {
				CORE_PIN13_DDRREG  &= ~CORE_PIN13_BITMASK;
				CORE_PIN13_PORTREG &= ~CORE_PIN13_BITMASK;
			} else if (pin == 14) {
				CORE_PIN14_DDRREG  &= ~CORE_PIN14_BITMASK;
				CORE_PIN14_PORTREG &= ~CORE_PIN14_BITMASK;
			} else if (pin == 15) {
				CORE_PIN15_DDRREG  &= ~CORE_PIN15_BITMASK;
				CORE_PIN15_PORTREG &= ~CORE_PIN15_BITMASK;
			} else if (pin == 16) {
				CORE_PIN16_DDRREG  &= ~CORE_PIN16_BITMASK;
				CORE_PIN16_PORTREG &= ~CORE_PIN16_BITMASK;
			} else if (pin == 17) {
				CORE_PIN17_DDRREG  &= ~CORE_PIN17_BITMASK;
				CORE_PIN17_PORTREG &= ~CORE_PIN17_BITMASK;
			} else if (pin == 18) {
				CORE_PIN18_DDRREG  &= ~CORE_PIN18_BITMASK;
				CORE_PIN18_PORTREG &= ~CORE_PIN18_BITMASK;
			} else if (pin == 19) {
				CORE_PIN19_DDRREG  &= ~CORE_PIN19_BITMASK;
				CORE_PIN19_PORTREG &= ~CORE_PIN19_BITMASK;
			} else if (pin == 20) {
				CORE_PIN20_DDRREG  &= ~CORE_PIN20_BITMASK;
				CORE_PIN20_PORTREG &= ~CORE_PIN20_BITMASK;
			}
			#if CORE_NUM_TOTAL_PINS > 21
			  else if (pin == 21) {
				CORE_PIN21_DDRREG  &= ~CORE_PIN21_BITMASK;
				CORE_PIN21_PORTREG &= ~CORE_PIN21_BITMASK;
			} else if (pin == 22) {
				CORE_PIN22_DDRREG  &= ~CORE_PIN22_BITMASK;
				CORE_PIN22_PORTREG &= ~CORE_PIN22_BITMASK;
			} else if (pin == 23) {
				CORE_PIN23_DDRREG  &= ~CORE_PIN23_BITMASK;
				CORE_PIN23_PORTREG &= ~CORE_PIN23_BITMASK;
			} else if (pin == 24) {
				CORE_PIN24_DDRREG  &= ~CORE_PIN24_BITMASK;
				CORE_PIN24_PORTREG &= ~CORE_PIN24_BITMASK;
			}
			#endif
			#if CORE_NUM_TOTAL_PINS > 25
			  else if (pin == 25) {
				CORE_PIN25_DDRREG  &= ~CORE_PIN25_BITMASK;
				CORE_PIN25_PORTREG &= ~CORE_PIN25_BITMASK;
			} else if (pin == 26) {
				CORE_PIN26_DDRREG  &= ~CORE_PIN26_BITMASK;
				CORE_PIN26_PORTREG &= ~CORE_PIN26_BITMASK;
			} else if (pin == 27) {
				CORE_PIN27_DDRREG  &= ~CORE_PIN27_BITMASK;
				CORE_PIN27_PORTREG &= ~CORE_PIN27_BITMASK;
			} else if (pin == 28) {
				CORE_PIN28_DDRREG  &= ~CORE_PIN28_BITMASK;
				CORE_PIN28_PORTREG &= ~CORE_PIN28_BITMASK;
			} else if (pin == 29) {
				CORE_PIN29_DDRREG  &= ~CORE_PIN29_BITMASK;
				CORE_PIN29_PORTREG &= ~CORE_PIN29_BITMASK;
			} else if (pin == 30) {
				CORE_PIN30_DDRREG  &= ~CORE_PIN30_BITMASK;
				CORE_PIN30_PORTREG &= ~CORE_PIN30_BITMASK;
			} else if (pin == 31) {
				CORE_PIN31_DDRREG  &= ~CORE_PIN31_BITMASK;
				CORE_PIN31_PORTREG &= ~CORE_PIN31_BITMASK;
			} else if (pin == 32) {
				CORE_PIN32_DDRREG  &= ~CORE_PIN32_BITMASK;
				CORE_PIN32_PORTREG &= ~CORE_PIN32_BITMASK;
			} else if (pin == 33) {
				CORE_PIN33_DDRREG  &= ~CORE_PIN33_BITMASK;
				CORE_PIN33_PORTREG &= ~CORE_PIN33_BITMASK;
			} else if (pin == 34) {
				CORE_PIN34_DDRREG  &= ~CORE_PIN34_BITMASK;
				CORE_PIN34_PORTREG &= ~CORE_PIN34_BITMASK;
			} else if (pin == 35) {
				CORE_PIN35_DDRREG  &= ~CORE_PIN35_BITMASK;
				CORE_PIN35_PORTREG &= ~CORE_PIN35_BITMASK;
			} else if (pin == 36) {
				CORE_PIN36_DDRREG  &= ~CORE_PIN36_BITMASK;
				CORE_PIN36_PORTREG &= ~CORE_PIN36_BITMASK;
			} else if (pin == 37) {
				CORE_PIN37_DDRREG  &= ~CORE_PIN37_BITMASK;
				CORE_PIN37_PORTREG &= ~CORE_PIN37_BITMASK;
			} else if (pin == 38) {
				CORE_PIN38_DDRREG  &= ~CORE_PIN38_BITMASK;
				CORE_PIN38_PORTREG &= ~CORE_PIN38_BITMASK;
			} else if (pin == 39) {
				CORE_PIN39_DDRREG  &= ~CORE_PIN39_BITMASK;
				CORE_PIN39_PORTREG &= ~CORE_PIN39_BITMASK;
			} else if (pin == 40) {
				CORE_PIN40_DDRREG  &= ~CORE_PIN40_BITMASK;
				CORE_PIN40_PORTREG &= ~CORE_PIN40_BITMASK;
			} else if (pin == 41) {
				CORE_PIN41_DDRREG  &= ~CORE_PIN41_BITMASK;
				CORE_PIN41_PORTREG &= ~CORE_PIN41_BITMASK;
			} else if (pin == 42) {
				CORE_PIN42_DDRREG  &= ~CORE_PIN42_BITMASK;
				CORE_PIN42_PORTREG &= ~CORE_PIN42_BITMASK;
			} else if (pin == 43) {
				CORE_PIN43_DDRREG  &= ~CORE_PIN43_BITMASK;
				CORE_PIN43_PORTREG &= ~CORE_PIN43_BITMASK;
			} else if (pin == 44) {
				CORE_PIN44_DDRREG  &= ~CORE_PIN44_BITMASK;
				CORE_PIN44_PORTREG &= ~CORE_PIN44_BITMASK;
			} else if (pin == 45) {
				CORE_PIN45_DDRREG  &= ~CORE_PIN45_BITMASK;
				CORE_PIN45_PORTREG &= ~CORE_PIN45_BITMASK;
			}
			#endif
		}
		#if defined(__AVR_ATmega32U4__)
		if (mode == INPUT || mode == INPUT_PULLUP) {
			if (pin == 11) {
				DIDR2 &= ~0x02;
			} else if (pin == 12) {
				DIDR2 &= ~0x04;
			} else if (pin == 13) {
				DIDR2 &= ~0x08;
			} else if (pin == 14) {
				DIDR2 &= ~0x10;
			} else if (pin == 15) {
				DIDR2 &= ~0x20;
			} else if (pin == 16) {
				DIDR0 &= ~0x80;
			} else if (pin == 17) {
				DIDR0 &= ~0x40;
			} else if (pin == 18) {
				DIDR0 &= ~0x20;
			} else if (pin == 19) {
				DIDR0 &= ~0x10;
			} else if (pin == 20) {
				DIDR0 &= ~0x02;
			} else if (pin == 21) {
				DIDR0 &= ~0x01;
			} else if (pin == 22) {
				DIDR2 &= ~0x01;
			}
		}
		#elif defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__)
		if (mode == INPUT || mode == INPUT_PULLUP) {
			if (pin == 38) {
				DIDR0 &= ~0x01;
			} else if (pin == 39) {
				DIDR0 &= ~0x02;
			} else if (pin == 40) {
				DIDR0 &= ~0x04;
			} else if (pin == 41) {
				DIDR0 &= ~0x08;
			} else if (pin == 42) {
				DIDR0 &= ~0x10;
			} else if (pin == 43) {
				DIDR0 &= ~0x20;
			} else if (pin == 44) {
				DIDR0 &= ~0x40;
			} else if (pin == 45) {
				DIDR0 &= ~0x80;
			}
		}
		#endif
	} else if (__builtin_constant_p(mode)) {
		if (mode == OUTPUT) {
			_pinMode_output(pin);
		} else if (mode == INPUT_PULLUP) {
			_pinMode_input_pullup(pin);
		} else {
			_pinMode_input(pin);
		}
	} else {
		_pinMode(pin, mode);
	}
}

extern void _init_Teensyduino_internal_(void);
extern int analogRead(uint8_t);

extern void _analogWrite(uint8_t pin, int val) __attribute__((noinline));

static inline void analogWrite(uint8_t, int) __attribute__((always_inline, unused));
static inline void analogWrite(uint8_t pin, int val)
{
	if (__builtin_constant_p(pin)) {
		if (pin == CORE_OC0B_PIN) {	// TIMER0B
			CORE_DDRREG(CORE_OC0B_PIN) |= CORE_BITMASK(CORE_OC0B_PIN);
			if (val) {
				OCR0B = val;
				_SFR_BYTE(TCCR0A) |= (1<<COM0B1);
			} else {
				CORE_PORTREG(CORE_OC0B_PIN) &= ~CORE_BITMASK(CORE_OC0B_PIN);
				_SFR_BYTE(TCCR0A) &= ~(1<<COM0B1);
			}
		} else if (pin == CORE_OC1A_PIN) { //TIMER1A
			CORE_DDRREG(CORE_OC1A_PIN) |= CORE_BITMASK(CORE_OC1A_PIN);
			OCR1A = val;
			TCCR1A |= (1<<COM1A1);
		} else if (pin == CORE_OC1B_PIN) { //TIMER1B
			CORE_DDRREG(CORE_OC1B_PIN) |= CORE_BITMASK(CORE_OC1B_PIN);
			OCR1B = val;
			TCCR1A |= (1<<COM1B1);
		} else if (pin == CORE_OC1C_PIN) { //TIMER1C
			CORE_DDRREG(CORE_OC1C_PIN) |= CORE_BITMASK(CORE_OC1C_PIN);
			OCR1C = val;
			TCCR1A |= (1<<COM1C1);
		}
#if defined(__AVR_ATmega32U4__)
		  else if (pin == CORE_OC3A_PIN) { //TIMER3A
			CORE_DDRREG(CORE_OC3A_PIN) |= CORE_BITMASK(CORE_OC3A_PIN);
			OCR3A = val;
			TCCR3A |= (1<<COM3A1);
		} else if (pin == CORE_OC4A_PIN) { //TIMER4A
			CORE_DDRREG(CORE_OC4A_PIN) |= CORE_BITMASK(CORE_OC4A_PIN);
			OCR4A = val;
			TCCR4A |= (1<<COM4A1);
		} else if (pin == CORE_OC4D_PIN) { //TIMER4D
			CORE_DDRREG(CORE_OC4D_PIN) |= CORE_BITMASK(CORE_OC4D_PIN);
			OCR4D = val;
			TCCR4C |= (1<<COM4D1);
		}
#endif
#if defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__)
		  else if (pin == CORE_OC2A_PIN) { //TIMER2A
			CORE_DDRREG(CORE_OC2A_PIN) |= CORE_BITMASK(CORE_OC2A_PIN);
			OCR2A = val;
			TCCR2A |= (1<<COM2A1);
		} else if (pin == CORE_OC2B_PIN) { //TIMER2B
			CORE_DDRREG(CORE_OC2B_PIN) |= CORE_BITMASK(CORE_OC2B_PIN);
			OCR2B = val;
			TCCR2A |= (1<<COM2B1);
		} else if (pin == CORE_OC3A_PIN) { //TIMER3A
			CORE_DDRREG(CORE_OC3A_PIN) |= CORE_BITMASK(CORE_OC3A_PIN);
			OCR3A = val;
			TCCR3A |= (1<<COM3A1);
		} else if (pin == CORE_OC3B_PIN) { //TIMER3B
			CORE_DDRREG(CORE_OC3B_PIN) |= CORE_BITMASK(CORE_OC3B_PIN);
			OCR3B = val;
			TCCR3A |= (1<<COM3B1);
		} else if (pin == CORE_OC3C_PIN) { //TIMER3C
			CORE_DDRREG(CORE_OC3C_PIN) |= CORE_BITMASK(CORE_OC3C_PIN);
			OCR3C = val;
			TCCR3A |= (1<<COM3C1);
		}
#endif
		  else {
			if (pin < CORE_NUM_TOTAL_PINS) {
				pinMode(pin, OUTPUT);
				if (val < 128) {
					digitalWrite(pin, LOW);
				} else {
					digitalWrite(pin, HIGH);
				}
			}
		}
	} else {
		_analogWrite(pin, val);
	}
}

static inline void shiftOut(uint8_t, uint8_t, uint8_t, uint8_t) __attribute__((always_inline, unused));
extern void _shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t value) __attribute__((noinline));
extern void shiftOut_lsbFirst(uint8_t dataPin, uint8_t clockPin, uint8_t value) __attribute__((noinline));
extern void shiftOut_msbFirst(uint8_t dataPin, uint8_t clockPin, uint8_t value) __attribute__((noinline));

static inline void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t value)
{
	if (__builtin_constant_p(bitOrder)) {
		if (bitOrder == LSBFIRST) {
			shiftOut_lsbFirst(dataPin, clockPin, value);
		} else {
			shiftOut_msbFirst(dataPin, clockPin, value);
		}
	} else {
		_shiftOut(dataPin, clockPin, bitOrder, value);
	}
}

static inline uint8_t shiftIn(uint8_t, uint8_t, uint8_t) __attribute__((always_inline, unused));
extern uint8_t _shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder) __attribute__((noinline));
extern uint8_t shiftIn_lsbFirst(uint8_t dataPin, uint8_t clockPin) __attribute__((noinline));
extern uint8_t shiftIn_msbFirst(uint8_t dataPin, uint8_t clockPin) __attribute__((noinline));

static inline uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder)
{
	if (__builtin_constant_p(bitOrder)) {
		if (bitOrder == LSBFIRST) {
			return shiftIn_lsbFirst(dataPin, clockPin);
		} else {
			return shiftIn_msbFirst(dataPin, clockPin);
		}
	} else {
		return _shiftIn(dataPin, clockPin, bitOrder);
	}
}

void _reboot_Teensyduino_(void) __attribute__((noreturn));
void _restart_Teensyduino_(void) __attribute__((noreturn));


#if defined(__AVR_AT90USB162__)
#define analogReference(mode)
#else
extern uint8_t w_analog_reference;
static inline void analogReference(uint8_t mode)
{
	w_analog_reference = (mode << 6);
}
#endif


extern void delay(uint32_t);

extern volatile uint32_t timer0_millis_count;

static inline uint32_t millis(void) __attribute__((always_inline, unused));
static inline uint32_t millis(void)
{
	uint32_t out;
	asm volatile(
		"in	__tmp_reg__, __SREG__"		"\n\t"
		"cli"					"\n\t"
		"lds	%A0, timer0_millis_count"	"\n\t"
		"lds	%B0, timer0_millis_count+1"	"\n\t"
		"lds	%C0, timer0_millis_count+2"	"\n\t"
		"lds	%D0, timer0_millis_count+3"	"\n\t"
		"out	__SREG__, __tmp_reg__"
		: "=r" (out) : : "r0"
	);
	return out;
}

extern uint32_t _micros(void) __attribute__((noinline));

static inline uint32_t micros(void) __attribute__((always_inline, unused));
static inline uint32_t micros(void)
{
	register uint32_t out asm("r22");
	asm volatile("call _micros" : "=d" (out) : : "r0");
	return out;
}


static inline void delayMicroseconds(uint16_t) __attribute__((always_inline, unused));
static inline void delayMicroseconds(uint16_t usec)
{
	if (__builtin_constant_p(usec)) {
		#if F_CPU == 16000000L
		uint16_t tmp = usec * 4;
		#elif F_CPU == 8000000L
		uint16_t tmp = usec * 2;
		#elif F_CPU == 4000000L
		uint16_t tmp = usec;
		#elif F_CPU == 2000000L
		uint16_t tmp = usec / 2;
		if (usec == 1) {
			asm volatile("rjmp L%=\nL%=:\n" ::);
		}
		#elif F_CPU == 1000000L
		uint16_t tmp = usec / 4;
		if (usec == 1) {
			asm volatile("nop\n");
		} else if (usec == 2) {
			asm volatile("rjmp L%=\nL%=:\n" ::);
		} else if (usec == 3) {
			asm volatile("rjmp L%=\nL%=:\n" ::);
			asm volatile("nop\n");
		}
		#else
		#error "Clock must be 16, 8, 4, 2 or 1 MHz"
		#endif
		if (tmp > 0) {
			if (tmp < 256) {
				uint8_t tmp2 = tmp;
				asm volatile(
				"L_%=_loop:"				// 1 to load
					"subi	%0, 1"		"\n\t"	// 2
					"brne	L_%=_loop"	"\n\t"	// 2 (1 on last)
					: "=d" (tmp2)
					: "0" (tmp2)
				);
			} else {
				asm volatile(
				"L_%=_loop:"				// 2 to load
					"sbiw	%A0, 1"		"\n\t"	// 2
					"brne	L_%=_loop"	"\n\t"	// 2 (1 on last)
					: "=w" (tmp)
					: "0" (tmp)
				);
			}
		}
	} else {
		asm volatile(
		#if F_CPU == 16000000L
			"sbiw	%A0, 2"			"\n\t"	// 2
			"brcs	L_%=_end"		"\n\t"	// 1
			"breq	L_%=_end"		"\n\t"	// 1
			"lsl	%A0"			"\n\t"	// 1
			"rol	%B0"			"\n\t"	// 1
			"lsl	%A0"			"\n\t"	// 1
			"rol	%B0"			"\n\t"	// 1  overhead: (8)/4 = 2us
		#elif F_CPU == 8000000L
			"sbiw	%A0, 3"			"\n\t"	// 2
			"brcs	L_%=_end"		"\n\t"	// 1
			"breq	L_%=_end"		"\n\t"	// 1
			"lsl	%A0"			"\n\t"	// 1
			"rol	%B0"			"\n\t"	// 1  overhead: (6)/2 = 3 us
		#elif F_CPU == 4000000L
			"sbiw	%A0, 4"			"\n\t"	// 2
			"brcs	L_%=_end"		"\n\t"	// 1
			"breq	L_%=_end"		"\n\t"	// 1  overhead: (4) = 4 us
		#elif F_CPU == 2000000L
			"sbiw	%A0, 12"		"\n\t"	// 2
			"brcs	L_%=_end"		"\n\t"	// 1
			"breq	L_%=_end"		"\n\t"	// 1
			"lsr	%B0"			"\n\t"	// 1
			"ror	%A0"			"\n\t"	// 1  overhead: (6)*2 = 12 us
		#elif F_CPU == 1000000L
			"sbiw	%A0, 32"		"\n\t"	// 2
			"brcs	L_%=_end"		"\n\t"	// 1
			"breq	L_%=_end"		"\n\t"	// 1
			"lsr	%B0"			"\n\t"	// 1
			"ror	%A0"			"\n\t"	// 1
			"lsr	%B0"			"\n\t"	// 1
			"ror	%A0"			"\n\t"	// 1  overhead: (8)*4 = 32 us
		#endif
		"L_%=_loop:"
			"sbiw	%A0, 1"			"\n\t"	// 2
			"brne	L_%=_loop"		"\n\t"	// 2
		"L_%=_end:"
			: "=w" (usec)
			: "0" (usec)
		);
	}
}




#ifdef __cplusplus
} // extern "C"
#endif
#endif
