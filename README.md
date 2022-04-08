# Marlin for TronXY CXY-V10-20210309

TRONXY-X5SA-PRO, TRONXY-X5SA-PRO-2E, TRONXY-X5SA-500 with new "V10" board that uses STM32F446ZET6 MCU and 480x320, 3.5", FSMC Stock Display from TronxXY (#define TFT_TRONXY_X5SA)

Working
* EEPROM
* SD CARD
* Steppers
* Sensors
* Touch interface
* LCD, color and classic ui are fine
* Buzzer
* LED

Not working
* Flash memory
* Mystery 8mhz crystal

You will need an STLINK V2 and to copy the json file to {$home}/.platformio/ststm32/boards