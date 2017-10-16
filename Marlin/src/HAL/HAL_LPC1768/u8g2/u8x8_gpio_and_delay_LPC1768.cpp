#ifdef TARGET_LPC1768
  
#include <clib/u8g2.h>
#include <clib/u8x8.h>

void delayMicroseconds(uint32_t us);
extern "C" void delay(const int msec);
//void delay(const int msec);

typedef int16_t pin_t;
void pinMode(pin_t pin, uint8_t mode);
void digitalWrite(pin_t pin, uint8_t pin_status);
uint8_t digitalRead(pin_t pin);

#define INPUT        0x00
#define OUTPUT       0x01
#define INPUT_PULLUP 0x02
 
uint8_t u8x8_gpio_and_delay_LPC1768(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  uint8_t* pins = u8x8->pins;
  switch(msg)
  {
    case U8X8_MSG_GPIO_AND_DELAY_INIT:	// called once during init phase of u8g2/u8x8
      for (uint8_t i = 0; i < U8X8_PIN_OUTPUT_CNT; i++) 
        if (pins[i] != U8X8_PIN_NONE) 
          pinMode(pins[i], OUTPUT); 
      break;						
    case U8X8_MSG_DELAY_NANO:			// delay arg_int * 1 nano second
      if (arg_int < 1000) break;
      delayMicroseconds(arg_int/1000);
      break;    
    case U8X8_MSG_DELAY_100NANO:		// delay arg_int * 100 nano seconds
      if (arg_int < 100) break;
      delayMicroseconds(arg_int/100);
      break;
    case U8X8_MSG_DELAY_10MICRO:		// delay arg_int * 10 micro seconds
        delayMicroseconds(arg_int * 10);    
      break;
    case U8X8_MSG_DELAY_MILLI:			// delay arg_int * 1 milli second
      delay(arg_int); 
      break;
    case U8X8_MSG_DELAY_I2C:				// arg_int is the I2C speed in 100KHz, e.g. 4 = 400 KHz
      if (arg_int == 1) 
        delayMicroseconds(5);
      else
        delayMicroseconds(1);
      break;							// arg_int=1: delay by 5us, arg_int = 4: delay by 1.25us
    case U8X8_MSG_GPIO_D0:				// D0 or SPI clock pin: Output level in arg_int
      digitalWrite(pins[U8X8_PIN_SPI_CLOCK], arg_int);
    //case U8X8_MSG_GPIO_SPI_CLOCK:
      break;
    case U8X8_MSG_GPIO_D1:				// D1 or SPI data pin: Output level in arg_int
      digitalWrite(pins[U8X8_PIN_SPI_DATA], arg_int);
    //case U8X8_MSG_GPIO_SPI_DATA:
      break;
    case U8X8_MSG_GPIO_D2:				// D2 pin: Output level in arg_int
      digitalWrite(pins[U8X8_PIN_D2], arg_int);
      break;
    case U8X8_MSG_GPIO_D3:				// D3 pin: Output level in arg_int
      digitalWrite(pins[U8X8_PIN_D3], arg_int);
      break;
    case U8X8_MSG_GPIO_D4:				// D4 pin: Output level in arg_int
      digitalWrite(pins[U8X8_PIN_D4], arg_int);
      break;
    case U8X8_MSG_GPIO_D5:				// D5 pin: Output level in arg_int
      digitalWrite(pins[U8X8_PIN_D5], arg_int);
      break;
    case U8X8_MSG_GPIO_D6:				// D6 pin: Output level in arg_int
      digitalWrite(pins[U8X8_PIN_D6], arg_int);
      break;
    case U8X8_MSG_GPIO_D7:				// D7 pin: Output level in arg_int
      digitalWrite(pins[U8X8_PIN_D7], arg_int);
      break;
    case U8X8_MSG_GPIO_E:				// E/WR pin: Output level in arg_int
      digitalWrite(pins[U8X8_PIN_E], arg_int);
      break;
    case U8X8_MSG_GPIO_CS:				// CS (chip select) pin: Output level in arg_int
      digitalWrite(pins[U8X8_PIN_CS], arg_int);
      break;
    case U8X8_MSG_GPIO_DC:				// DC (data/cmd, A0, register select) pin: Output level in arg_int
      digitalWrite(pins[U8X8_PIN_DC], arg_int);
      break;
    case U8X8_MSG_GPIO_RESET:			// Reset pin: Output level in arg_int
      digitalWrite(pins[U8X8_PIN_RESET], arg_int);
      break;
    case U8X8_MSG_GPIO_CS1:				// CS1 (chip select) pin: Output level in arg_int
      digitalWrite(pins[U8X8_PIN_CS1], arg_int);
      break;
    case U8X8_MSG_GPIO_CS2:				// CS2 (chip select) pin: Output level in arg_int
      digitalWrite(pins[U8X8_PIN_CS2], arg_int);
      break;
    case U8X8_MSG_GPIO_I2C_CLOCK:		// arg_int=0: Output low at I2C clock pin
      digitalWrite(pins[U8X8_PIN_I2C_CLOCK], arg_int);
      break;							// arg_int=1: Input dir with pullup high for I2C clock pin
    case U8X8_MSG_GPIO_I2C_DATA:			// arg_int=0: Output low at I2C data pin
    digitalWrite(pins[U8X8_PIN_I2C_DATA], arg_int);
      break;							// arg_int=1: Input dir with pullup high for I2C data pin
//    case U8X8_MSG_GPIO_MENU_SELECT:
//      u8x8_SetGPIOResult(u8x8, /* get menu select pin state */ 0);
//      break;
//    case U8X8_MSG_GPIO_MENU_NEXT:
//      u8x8_SetGPIOResult(u8x8, /* get menu next pin state */ 0);
//      break;
//    case U8X8_MSG_GPIO_MENU_PREV:
//      u8x8_SetGPIOResult(u8x8, /* get menu prev pin state */ 0);
//      break;
//    case U8X8_MSG_GPIO_MENU_HOME:
//      u8x8_SetGPIOResult(u8x8, /* get menu home pin state */ 0);
//      break;
    default:
      u8x8_SetGPIOResult(u8x8, 1);			// default return value
      break;
  }
  return 1;
}


/* list of U8x8 pins */
/*
#define U8X8_PIN_D0 0
#define U8X8_PIN_SPI_CLOCK 0
#define U8X8_PIN_D1 1
#define U8X8_PIN_SPI_DATA 1
#define U8X8_PIN_D2 2
#define U8X8_PIN_D3 3
#define U8X8_PIN_D4 4
#define U8X8_PIN_D5 5
#define U8X8_PIN_D6 6
#define U8X8_PIN_D7 7

#define U8X8_PIN_E 8
#define U8X8_PIN_CS 9			// parallel, SPI 
#define U8X8_PIN_DC 10			// parallel, SPI 
#define U8X8_PIN_RESET 11		// parallel, SPI, I2C 

#define U8X8_PIN_I2C_CLOCK 12	// 1 = Input/high impedance, 0 = drive low 
#define U8X8_PIN_I2C_DATA 13	// 1 = Input/high impedance, 0 = drive low 

#define U8X8_PIN_CS1 14			// KS0108 extra chip select 
#define U8X8_PIN_CS2 15			// KS0108 extra chip select 

#define U8X8_PIN_OUTPUT_CNT 16

#define U8X8_PIN_MENU_SELECT 16
#define U8X8_PIN_MENU_NEXT 17
#define U8X8_PIN_MENU_PREV 18
#define U8X8_PIN_MENU_HOME 19
#define U8X8_PIN_MENU_UP 20
#define U8X8_PIN_MENU_DOWN 21

#define U8X8_PIN_INPUT_CNT 6

#ifdef U8X8_USE_PINS 
#define U8X8_PIN_CNT (U8X8_PIN_OUTPUT_CNT+U8X8_PIN_INPUT_CNT)
#define U8X8_PIN_NONE 255
#endif
*/

#endif
