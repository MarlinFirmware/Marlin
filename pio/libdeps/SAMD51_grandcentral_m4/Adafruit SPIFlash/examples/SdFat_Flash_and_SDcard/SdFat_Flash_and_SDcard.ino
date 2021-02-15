// Adafruit Grand Central M4 QSPI Flash and SD Card Setup Example
// Author: Joshua Scoggins
//
// This is an example of how to bring up both the QSPI Flash and SD Card found
// on the Adafruit Grand Central M4. This example will setup both the QSPI
// Flash and SD card (if present) and display information about the QSPI flash.
//

#include <SdFat.h>
#include <Adafruit_SPIFlash.h>


// On-board external flash (QSPI or SPI) macros should already
// defined in your board variant if supported
// - EXTERNAL_FLASH_USE_QSPI
// - EXTERNAL_FLASH_USE_CS/EXTERNAL_FLASH_USE_SPI
#if defined(EXTERNAL_FLASH_USE_QSPI)
Adafruit_FlashTransport_QSPI flashTransport;

#elif defined(EXTERNAL_FLASH_USE_SPI)
Adafruit_FlashTransport_SPI flashTransport(EXTERNAL_FLASH_USE_CS, EXTERNAL_FLASH_USE_SPI);

#else
#error No QSPI/SPI flash are defined on your board variant.h !
#endif

Adafruit_SPIFlash onboardFlash(&flashTransport);
SdFat onboardSdCard;

constexpr int getSDCardPin() noexcept {
#ifdef SDCARD_SS_PIN
	return SDCARD_SS_PIN;
#else
	// modify to fit your needs
	// by default, pin 4 is the SD_CS pin used by the Adafruit 1.8" TFT SD Shield
	return 4; 
#endif
}
void setup() {
	Serial.begin(9600);
	while (!Serial) {
		// wait for native usb
		delay(100); 
	}
    Serial.print("Starting up onboard QSPI Flash...");
    onboardFlash.begin();
    Serial.println("Done");
    Serial.println("Onboard Flash information");
    Serial.print("JEDEC ID: 0x");
    Serial.println(onboardFlash.getJEDECID(), HEX);
    Serial.print("Flash size: ");
    Serial.print(onboardFlash.size() / 1024);
    Serial.println(" KB");
    Serial.print("Starting up SD Card...");
    if (!onboardSdCard.begin(getSDCardPin())) {
        Serial.println("No card found (is one inserted?)");
    } else {
        Serial.println("Card found!");
    }
}

void loop() {
	// nothing to do
}
