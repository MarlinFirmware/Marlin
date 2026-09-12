
typedef struct {
    double lat;
    double lon;
    uint8_t fix;
    uint8_t sats;
    float hdop;
} gps_t;
gps_t gps;


void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
}

void loop() {
  while (Serial2.available()) {
    char c = Serial2.read();
    static char buffer[128];
    static uint8_t idx = 0;
    if (c == '\n' || c == '\r') {
      if (idx > 5) { // min $xxxx, 
        buffer[idx] = '\0';
        if (strstr(buffer, "GGA") != NULL) {
	        if (parseGGA(buffer, &gps))
	        {
	            Serial.print("---- GPS ----");
	            Serial.print("Fix: ");
	            Serial.print(gps.fix);

	            Serial.print(" Sats: ");
	            Serial.print(gps.sats);

	            Serial.print(" HDOP: ");
	            Serial.print(gps.hdop, 2);

	            Serial.print(" Lat: ");
	            Serial.print(gps.lat, 10);

	            Serial.print(" Lon: ");
	            Serial.println(gps.lon, 10);
	        }
         }
      }
      idx = 0;
    } else if (idx < sizeof(buffer) - 1) {
      buffer[idx++] = c;
    }
  }
}


bool gpsReady(void)
{
    if (gps.fix == 0) return false;
    if (gps.sats < 6) return false;
    if (gps.hdop > 2.5) return false;
    return true;
}

double nmeaToDecimal(double raw)
{
    int deg = (int)(raw / 100);
    double min = raw - (deg * 100);
    return deg + (min / 60.0);
}


bool parseGGA(char *str, gps_t *gps)
{
    double lat_raw, lon_raw;
    char ns, ew;
    int fix, sats;
    float hdop;

    int r = sscanf(str,
        "$%*2cGGA,%*[^,],%lf,%c,%lf,%c,%d,%d,%f",
        &lat_raw, &ns,
        &lon_raw, &ew,
        &fix, &sats, &hdop);

    if (r < 7){
    	Serial.println("No GPS Signal");
        return false;
    }

    // DDMM → derece
    gps->lat = nmeaToDecimal(lat_raw);
    gps->lon = nmeaToDecimal(lon_raw);

    if (ns == 'S') gps->lat = -gps->lat;
    if (ew == 'W') gps->lon = -gps->lon;

    gps->fix  = fix;
    gps->sats = sats;
    gps->hdop = hdop;

    return true;
}

