/**
 * pins_arduino.h - AT32 pin definitions for Marlin
 */

#ifndef PINS_ARDUINO_H
#define PINS_ARDUINO_H

// GPIO pin macros for AT32F403
#define PA0  0
#define PA1  1
#define PA2  2
#define PA3  3
#define PA4  4
#define PA5  5
#define PA6  6
#define PA7  7
#define PA8  8
#define PA9  9
#define PA10 10
#define PA11 11
#define PA12 12
#define PA13 13
#define PA14 14
#define PA15 15
#define PB0  16
#define PB1  17
#define PB2  18
#define PB3  19
#define PB4  20
#define PB5  21
#define PB6  22
#define PB7  23
#define PB8  24
#define PB9  25
#define PB10 26
#define PB11 27
#define PB12 28
#define PB13 29
#define PB14 30
#define PB15 31
#define PC0  32
#define PC1  33
#define PC2  34
#define PC3  35
#define PC4  36
#define PC5  37
#define PC6  38
#define PC7  39
#define PC8  40
#define PC9  41
#define PC10 42
#define PC11 43
#define PC12 44
#define PC13 45
#define PC14 46
#define PC15 47
#define PD0  48
#define PD1  49
#define PD2  50
#define PD3  51
#define PD4  52
#define PD5  53
#define PD6  54
#define PD7  55
#define PD8  56
#define PD9  57
#define PD10 58
#define PD11 59
#define PD12 60
#define PD13 61
#define PD14 62
#define PD15 63
#define PE0  64
#define PE1  65
#define PE2  66
#define PE3  67
#define PE4  68
#define PE5  69
#define PE6  70
#define PE7  71
#define PE8  72
#define PE9  73
#define PE10 74
#define PE11 75
#define PE12 76
#define PE13 77
#define PE14 78
#define PE15 79
#define PF0  80
#define PF1  81
#define PF2  82
#define PF3  83
#define PF4  84
#define PF5  85
#define PF6  86
#define PF7  87
#define PF8  88
#define PF9  89
#define PF10 90
#define PF11 91
#define PF12 92
#define PF13 93
#define PF14 94
#define PF15 95
#define PG0  96
#define PG1  97
#define PG2  98
#define PG3  99
#define PG4  100
#define PG5  101
#define PG6  102
#define PG7  103
#define PG8  104
#define PG9  105
#define PG10 106
#define PG11 107
#define PG12 108
#define PG13 109
#define PG14 110
#define PG15 111
#define PH0  112
#define PH1  113

// Board USART pin definitions (can be overridden by board pins files)
#ifndef BOARD_USART1_TX_PIN
  #define BOARD_USART1_TX_PIN PA9
#endif
#ifndef BOARD_USART1_RX_PIN
  #define BOARD_USART1_RX_PIN PA10
#endif

#endif // PINS_ARDUINO_H
