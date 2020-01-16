// Search pins usable for endstop-interrupts
// Compile with the same build settings you'd use for Marlin.

#if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
    #undef  digitalPinToPCICR
    #define digitalPinToPCICR(p)    ( ((p) >= 10 && (p) <= 15) || \
                                      ((p) >= 50 && (p) <= 53) || \
                                      ((p) >= 62 && (p) <= 69) ? (&PCICR) : nullptr)
#endif

void setup() {
  Serial.begin(9600);
  Serial.println("PINs causing interrupts are:");
  for (int i = 2; i < NUM_DIGITAL_PINS; i++) {
    if (digitalPinToPCICR(i) || (int)digitalPinToInterrupt(i) != -1) {
      for (int j = 0; j < NUM_ANALOG_INPUTS; j++) {
        if (analogInputToDigitalPin(j) == i) {
          Serial.print('A');
          Serial.print(j);
          Serial.print(" = ");
        }
      }
      Serial.print('D');
      Serial.println(i);
    }
  }
  Serial.println("Arduino pin numbering!");
}

void loop() {
  // put your main code here, to run repeatedly:
}
