#!/bin/bash

# upload works through serial
#env=mega2560

# firmware upgrade requires copying firmware.bin to an SD card and powercycle the MCU
#env=STM32G0B1RE_manta_btt

# changes upload to uses correct method to copy file to sdcard
#env=STM32G0B1RE_manta_btt_xfer	# MANTA M5P

env=STM32H723ZE_btt	# MANTA M8P
#platformio run --target clean -e $env && 
platformio run -e $env #&& platformio run --target upload -e $env
