// Benchmark comparing SdFile and StdioStream.
#include <SPI.h>
#include "SdFat.h"

// Define PRINT_FIELD nonzero to use printField.
#define PRINT_FIELD 0

// Number of lines to list on Serial.
#define STDIO_LIST_COUNT 0
#define VERIFY_CONTENT 0

const uint8_t SD_CS_PIN = SS;
SdFat sd;

SdFile printFile;
StdioStream stdioFile;

float f[100];
char buf[20];
const char* label[] =
{ "uint8_t 0 to 255, 100 times ", "uint16_t 0 to 20000",
  "uint32_t 0 to 20000", "uint32_t 1000000000 to 1000010000",
  "float nnn.ffff, 10000 times"
};
//------------------------------------------------------------------------------
void setup() {
  uint32_t printSize;
  uint32_t stdioSize = 0;
  uint32_t printTime;
  uint32_t stdioTime = 0;

  Serial.begin(9600);
  while (!Serial) {
    SysCall::yield();
  }

  Serial.println(F("Type any character to start"));
  while (!Serial.available()) {
    SysCall::yield();
  }
  Serial.println(F("Starting test"));

  // Initialize at the highest speed supported by the board that is
  // not over 50 MHz. Try a lower speed if SPI errors occur.  
  if (!sd.begin(SD_CS_PIN, SD_SCK_MHZ(50))) {
    sd.errorHalt();
  }

  for (uint8_t i = 0; i < 100; i++) {
    f[i] = 123.0 + 0.1234*i;
  }

  for (uint8_t dataType = 0; dataType < 5; dataType++) {
    for (uint8_t fileType = 0; fileType < 2; fileType++) {
      if (!fileType) {
        if (!printFile.open("print.txt", O_RDWR | O_CREAT | O_TRUNC)) {
          Serial.println(F("open fail"));
          return;
        }
        printTime = millis();
        switch (dataType) {
        case 0:
          for (uint16_t i =0; i < 100; i++) {
            for (uint8_t j = 0; j < 255; j++) {
              printFile.println(j);
            }
          }
          break;
        case 1:
          for (uint16_t i = 0; i < 20000; i++) {
            printFile.println(i);
          }
          break;

        case 2:
          for (uint32_t i = 0; i < 20000; i++) {
            printFile.println(i);
          }
          break;

        case 3:
          for (uint16_t i = 0; i < 10000; i++) {
            printFile.println(i + 1000000000UL);
          }
          break;

        case 4:
          for (int j = 0; j < 100; j++) {
            for (uint8_t i = 0; i < 100; i++) {
              printFile.println(f[i], 4);
            }
          }
          break;
        default:
          break;
        }
        printFile.sync();
        printTime = millis() - printTime;
        printFile.rewind();
        printSize = printFile.fileSize();

      } else {
        if (!stdioFile.fopen("stream.txt", "w+")) {
          Serial.println(F("fopen fail"));
          return;
        }
        stdioTime = millis();

        switch (dataType) {
        case 0:
          for (uint16_t i =0; i < 100; i++) {
            for (uint8_t j = 0; j < 255; j++) {
#if PRINT_FIELD
              stdioFile.printField(j, '\n');
#else  // PRINT_FIELD
              stdioFile.println(j);
#endif  // PRINT_FIELD
            }
          }
          break;
        case 1:
          for (uint16_t i = 0; i < 20000; i++) {
#if PRINT_FIELD
            stdioFile.printField(i, '\n');
#else  // PRINT_FIELD
            stdioFile.println(i);
#endif  // PRINT_FIELD
          }
          break;

        case 2:
          for (uint32_t i = 0; i < 20000; i++) {
#if PRINT_FIELD
            stdioFile.printField(i, '\n');
#else  // PRINT_FIELD
            stdioFile.println(i);
#endif  // PRINT_FIELD
          }
          break;

        case 3:
          for (uint16_t i = 0; i < 10000; i++) {
            uint32_t n = i + 1000000000UL;
#if PRINT_FIELD
            stdioFile.printField(n, '\n');
#else  // PRINT_FIELD
            stdioFile.println(n);
#endif  // PRINT_FIELD      
          }
          break;

        case 4:
          for (int j = 0; j < 100; j++) {
            for (uint8_t i = 0; i < 100; i++) {
#if PRINT_FIELD
              stdioFile.printField(f[i], '\n', 4);
#else  // PRINT_FIELD
              stdioFile.println(f[i], 4);
#endif  // PRINT_FIELD                            
            }
          }
          break;
        default:
          break;
        }
        stdioFile.fflush();
        stdioTime = millis() - stdioTime;
        stdioSize = stdioFile.ftell();
        if (STDIO_LIST_COUNT) {
          size_t len;
          stdioFile.rewind();
          for (int i = 0; i < STDIO_LIST_COUNT; i++) {
            stdioFile.fgets(buf, sizeof(buf), &len);
            Serial.print(len);
            Serial.print(',');
            Serial.print(buf);
          }
        }

      }

    }
    Serial.println(label[dataType]);
    if (VERIFY_CONTENT && printSize == stdioSize) {
      printFile.rewind();
      stdioFile.rewind();
      for (uint32_t i = 0; i < stdioSize; i++) {
        if (printFile.read() != stdioFile.getc()) {
          Serial.print(F("Files differ at pos: "));
          Serial.println(i);
          return;
        }
      }
    }

    Serial.print(F("fileSize: "));
    if (printSize != stdioSize) {
      Serial.print(printSize);
      Serial.print(F(" != "));
    }
    Serial.println(stdioSize);
    Serial.print(F("print millis: "));
    Serial.println(printTime);
    Serial.print(F("stdio millis: "));
    Serial.println(stdioTime);
    Serial.print(F("ratio: "));
    Serial.println((float)printTime/(float)stdioTime);
    Serial.println();
    printFile.close();
    stdioFile.fclose();
  }
  Serial.println(F("Done"));
}
void loop() {}