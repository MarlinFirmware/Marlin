
String nmeaBuffer;
bool nmeaMode = false;

void parseSerialByte(uint8_t c) {
   if (c == '$') {
        // NMEA mesajı başlıyor
        nmeaMode = true;
        nmeaBuffer = "$";
    }
    else if (nmeaMode) {
        if (c == '\n') {
            // NMEA satırı tamamlandı
            if (nmeaBuffer.startsWith("$GNGGA") || nmeaBuffer.startsWith("$GPGGA")) {
                parseNMEA_GGA(nmeaBuffer);
            }
            nmeaBuffer = "";
        }
        else if (nmeaBuffer.length() < 100) {
            nmeaBuffer += (char)c;
        }
    }
}

double nmeaToDecimal(double raw) {
  int deg = (int)(raw / 100);
  double min = raw - (deg * 100);
  return deg + (min / 60.0);
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  Serial.println("GPS Parser Started");
}

void loop() {
  while (Serial2.available()) {
    uint8_t c = Serial2.read();
    parseSerialByte(c);
  }
}



void parseNMEA_GGA(String nmea) {
    // Basit GGA parser
    int commas[15];
    int commaCount = 0;
    
    for (int i = 0; i < nmea.length(); i++) {
        if (nmea[i] == ',') {
            commas[commaCount++] = i;
            if (commaCount >= 14) break;
        }
    }
    
    if (commaCount >= 6) {
        String time = nmea.substring(commas[0]+1, commas[1]);
        String lat = nmea.substring(commas[1]+1, commas[2]);
        String latDir = nmea.substring(commas[2]+1, commas[3]);
        String lon = nmea.substring(commas[3]+1, commas[4]);
        String lonDir = nmea.substring(commas[4]+1, commas[5]);
        String fix = nmea.substring(commas[5]+1, commas[6]);
        String sats = nmea.substring(commas[6]+1, commas[7]);
        
        Serial.print("NMEA Fix: "); Serial.print(fix.toInt());
        Serial.print(" NMEA Sats: "); Serial.print(sats.toInt());
        Serial.print(" Lat: "); Serial.print(nmeaToDecimal(lat.toDouble()),7);
        Serial.print(" Lon: "); Serial.println(nmeaToDecimal(lon.toDouble()),7);
    }
}
