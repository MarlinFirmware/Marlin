/* This is my go-to example for erasing, dumping, writing and verifying SPI flash!  */

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


Adafruit_SPIFlash flash(&flashTransport);

// file system on SD card
SdFat sd;

#define SD_CS 10
#define MAXPAGESIZE 256

File dataFile;
uint8_t buffer[MAXPAGESIZE], buffer2[MAXPAGESIZE];
uint32_t results;

void error(char *str) {
  Serial.println(str);
  while (1) delay(1);
}

void setup(void) 
{
  Serial.begin(115200);
  while (!Serial) delay(1);

  flash.begin();

  Serial.println("Adafruit Serial Flash Manipulator example");
  Serial.print("JEDEC ID: "); Serial.println(flash.getJEDECID(), HEX);
  Serial.print("Flash size: "); Serial.println(flash.size());

  Serial.print("Initializing SD card... ");
  // see if the card is present and can be initialized:
  if (!sd.begin(SD_CS, SD_SCK_MHZ(50))) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }

  Serial.println("OK");
}

void loop(void) 
{
  while (Serial.available()) { Serial.read(); }
  Serial.println(F("FLASH DUMPER - Hello! Press E (rase), W (rite), V (erify), or D (ump) to begin"));

  while (!Serial.available());
  char cmd = toupper(Serial.read());
  
  uint16_t pagesize = flash.pageSize();
  Serial.print("Page size: ");
  Serial.println(pagesize);

  if (cmd == 'D') {
     // open the file. note that only one file can be open at a time,
     // so you have to close this one before opening another.
     // Open up the file we're going to log to!
     dataFile = sd.open("flshdump.bin", FILE_WRITE);
     if (! dataFile) {
       error("error opening flshdump.bin");
     }
    
    Serial.println("Dumping FLASH to disk");
    for (uint32_t page=0; page < flash.numPages() ; page++)  {
      memset(buffer, 0, pagesize);
      Serial.print("// Reading page ");
      Serial.println(page);
      uint16_t r = flash.readBuffer (page * pagesize, buffer, pagesize);
      //Serial.print(r); Serial.print(' '); PrintHex(buffer, r);
      dataFile.write(buffer, r);
    }  
    dataFile.flush();
    dataFile.close();
  }
  if (cmd == 'E') {
    Serial.println("Erasing chip");
    flash.eraseChip();
    
  }
  if (cmd == 'V') {
     dataFile = sd.open("flshdump.bin", FILE_READ);
     if (! dataFile) {
       error("error opening flshdump.bin");
     }
    Serial.println("Verifying FLASH from disk");
    for (uint32_t page=0; page < flash.numPages() ; page++)  {
      memset(buffer, 0, pagesize);
      memset(buffer2, 0, pagesize);
      
      Serial.print("// Verifying page ");  Serial.println(page);
      
      uint16_t r = flash.readBuffer (page * pagesize, buffer, pagesize);
      if (r != pagesize) {
        error("Flash read failure");
      }
      
      if (r != dataFile.read(buffer2, r)) {
        error("SD read failure");
      }

      if (memcmp(buffer, buffer2, r) != 0) {
        PrintHex(buffer, r);
        PrintHex(buffer2, r);
        Serial.println("verification failed");
        return;
      }      
    }  
    Serial.println("Done!");
    dataFile.close();
  }  

  if (cmd == 'W') {
     dataFile = sd.open("flshdump.bin", FILE_READ);
     if (! dataFile) {
       error("error opening flshdump.bin");
     }

    Serial.println("Writing FLASH from disk");
    for (uint32_t page=0; page < flash.numPages() ; page++)  {
      memset(buffer, 0, pagesize);
      
      int16_t r = dataFile.read(buffer, pagesize);
      if (r == 0) 
        break;
      Serial.print("// Writing page ");  Serial.println(page);

      if (r != flash.writeBuffer (page * r, buffer, r)) {
        error("Flash write failure");
      }
    }  
    Serial.println("Done!");
    dataFile.close();
  } 
}


/**************************************************************************/
/*! 
    @brief  Prints a hexadecimal value in plain characters, along with
            the char equivalents in the following format:

            00 00 00 00 00 00  ......

    @param  data      Pointer to the byte data
    @param  numBytes  Data length in bytes
*/
/**************************************************************************/
void PrintHexChar(const byte * data, const uint32_t numBytes)
{
  uint32_t szPos;
  for (szPos=0; szPos < numBytes; szPos++) 
  {
	// Append leading 0 for small values
	if (data[szPos] <= 0xF)
      Serial.print("0");
	Serial.print(data[szPos], HEX);
	if ((numBytes > 1) && (szPos != numBytes - 1))
	{
	  Serial.print(" ");
	}
  }
  Serial.print("  ");
  for (szPos=0; szPos < numBytes; szPos++) 
  {
    if (data[szPos] <= 0x1F)
	  Serial.print('.');
	else
	  Serial.write(data[szPos]);
  }
  Serial.println("");
}

/**************************************************************************/
/*! 
    @brief  Prints a hexadecimal value in plain characters

    @param  data      Pointer to the byte data
    @param  numBytes  Data length in bytes
*/
/**************************************************************************/
void PrintHex(const byte * data, const uint32_t numBytes)
{
  uint32_t szPos;
  for (szPos=0; szPos < numBytes; szPos++) 
  {
    Serial.print("0x");
	// Append leading 0 for small values
	if (data[szPos] <= 0xF)
      Serial.print("0");
	Serial.print(data[szPos], HEX);
	if ((numBytes > 1) && (szPos != numBytes - 1))
	{
	  Serial.print(" ");
	}
  }
  Serial.println("");
}
