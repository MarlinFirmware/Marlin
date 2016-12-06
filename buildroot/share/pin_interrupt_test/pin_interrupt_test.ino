// Search pins uasable for endstop-interupts
// Compile with the same settings you'd use with Marlin.

#if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
    #undef  digitalPinToPCICR
    #define digitalPinToPCICR(p)    ( (((p) >= 10) && ((p) <= 15)) || \
                                    (((p) >= 50) && ((p) <= 53)) || \
                                    (((p) >= 62) && ((p) <= 69)) ? (&PCICR) : ((uint8_t *)0) )
#endif

void setup() {
  Serial.begin(9600);
  Serial.println("PINs causing interrups are:");
  for(int i=2; i<NUM_DIGITAL_PINS; i++){
    if( digitalPinToPCICR(i) != NULL || (int)digitalPinToInterrupt(i) != -1 ) {
      for (int j= 0; j<NUM_ANALOG_INPUTS; j++){
        if(analogInputToDigitalPin(j) == i) {
          Serial.print("A");
          Serial.print(j);
          Serial.print(" = ");
        }
      }
      Serial.print("D");
      Serial.println(i);
    }
  }
  Serial.println("Arduino pin numbering!");
}

void loop() {
  // put your main code here, to run repeatedly:
}
