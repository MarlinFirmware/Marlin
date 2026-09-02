#include "UbxGpsParser.h"

UbxGpsParser gps;

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  Serial.println("UBX GPS Parser Started");
}

void loop() {
  while (Serial2.available()) {
    uint8_t c = Serial2.read();

    if (gps.parseByte(c)) {
        const auto& d = gps.getData();

        if (d.valid) {
          Serial.printf("Lat: %11.8f   Lon: %11.8f\r\n", d.lat, d.lon);
          Serial.printf("Fix: %d  SV: %2d  Height: %ld m\r\n", d.fixType, d.numSV, d.height / 1000);
        } else {
          Serial.println("No Fix");
        }
    }
  }
}
