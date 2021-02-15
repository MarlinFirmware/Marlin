// Adafruit M0 Express CircuitPython Repair
// Author: Limor Fried
//
/* 
 *  This script can be useful if you seriously bork up your CircuitPython
 *  install. It will find any files named main.py, boot.py, main.txt, code.py
 *  or code.txt and move them to backup files. its a tad slow but then you
 *  can reload circuitpython safely. This example right now is only for
 *  the Metro M0 Express & Circuit Playground M0 but i have a...
 *  
 *  TODO: automagically detect if it's Feather/Metro/CircuitPlayground!
 */

#include <SPI.h>
#include <SdFat.h>
#include <Adafruit_SPIFlash.h>
#include <Adafruit_NeoPixel.h>

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

// file system object from SdFat
FatFileSystem fatfs;


#define NEOPIN         40       // neopixel pin
#define BUFFERSIZ      200

Adafruit_NeoPixel pixel = Adafruit_NeoPixel(1, NEOPIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  //while (!Serial);
  delay(1000); // small delay in case we want to watch it on the serial port  
  Serial.println("Adafruit Express CircuitPython Flash Repair");
  
  pixel.begin();             // This initializes the NeoPixel library
  pixel.setBrightness(30);   // not too bright!
  
  // Initialize flash library and check its chip ID.
  if (!flash.begin()) {
    Serial.println("Error, failed to initialize flash chip!");
    error(1);
  }
  Serial.print("Flash chip JEDEC ID: 0x"); Serial.println(flash.getJEDECID(), HEX);

  // First call begin to mount the filesystem.  Check that it returns true
  // to make sure the filesystem was mounted.
  if (!fatfs.begin(&flash)) {
    Serial.println("Failed to mount filesystem!");
    Serial.println("Was CircuitPython loaded on the board first to create the filesystem?");
    error(3);
  }
  Serial.println("Mounted filesystem!");
  
  moveFile("boot.py", "bootpy.bak");    
  moveFile("main.py", "mainpy.bak");    
  moveFile("main.txt", "maintxt.bak");    
  moveFile("code.py", "codepy.bak");    
  moveFile("code.txt", "codetxt.bak");    

  Serial.println("Finished!");
}

uint8_t i = 0;
void loop() {
  // white pixel pulse -> we're done
  pixel.setPixelColor(0, pixel.Color(i,i,i)); pixel.show();
  i++;
  delay(5);
}


boolean moveFile(char *file, char *dest) {
  if (! fatfs.exists(file)) {
    Serial.print(file); Serial.println(" not found");
    return false;
  }
  if(fatfs.exists(dest)) {
    Serial.println("Found old backup, removing...");
    if (!fatfs.remove(dest)) {
      Serial.println("Error, couldn't delete ");
      Serial.print(dest);
      Serial.println(" file!");
      error(4);
    }
  }

  pixel.setPixelColor(0, pixel.Color(100,100,0)); pixel.show();

  File source = fatfs.open(file, FILE_READ);
  File backup = fatfs.open(dest, FILE_WRITE);
  Serial.println("Making backup!");
  Serial.println("\n---------------------\n");

  while (1) {
    int avail = source.available();
    //Serial.print("**Available bytes: "); Serial.print(avail); Serial.print("**");
    if (avail == 0) {
      Serial.println("\n---------------------\n");
      break;
    }
    int toread = min(BUFFERSIZ-1, avail);
    char buffer[BUFFERSIZ];
    
    int numread = source.read(buffer, toread);
    if (numread != toread) {
      Serial.print("Failed to read ");
      Serial.print(toread);
      Serial.print(" bytes, got "); 
      Serial.print(numread);
      error(5);
    }
    buffer[toread] = 0;      
    Serial.print(buffer);
    if (backup.write(buffer, toread) != toread) {
        Serial.println("Error, couldn't write data to backup file!");
        error(6);
    }
  }

  pixel.setPixelColor(0, pixel.Color(100,0,100)); pixel.show();

  Serial.print("Original file size: "); Serial.println(source.size());
  Serial.print("Backup file size: "); Serial.println(backup.size());
  backup.close();
  if (source.size() == backup.size()) {
     if (!fatfs.remove(file)) {
        Serial.print("Error, couldn't delete ");
        Serial.println(file);
        error(10);
     } 
  }
  pixel.setPixelColor(0, pixel.Color(0,100,0)); pixel.show();
  delay(100);
  return true;
}

void error(uint8_t i) {
  while (1) {
    for (int x = 0; x< i; x++) {
      pixel.setPixelColor(0, pixel.Color(100,0,0)); pixel.show();
      delay(200);
      pixel.setPixelColor(0, pixel.Color(0,0,0)); pixel.show();
      delay(200);
    }
    delay(1000);    
  }
}
