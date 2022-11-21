## Two Trees Sapphire Configurations

This folder contains default configurations for the Two Trees Sapphire Plus V2. The V2 has Dual Z Axis limit switches.

By default the standard Marlin Touch UI will be used. Enable the `SAPPHIRE_PLUS_MKS_UI` option for the MKS UI.

Enable the `SAPPHIRE_PLUS_BLTOUCH` option for BLTouch support.

Enable the `SAPPHIRE_PLUS_TMC_UART` option for TMC UART support for the X, Y and E steppers. Note: you will need some hardware modification skills to modify the stepper sticks. A solder pad needs to be bridged to connect PDN to USART pin. Then a 1K resistor and wire is needed to connect the USART pin to the appropriate spare pin on MKS Robin Nano V1.2 board.
