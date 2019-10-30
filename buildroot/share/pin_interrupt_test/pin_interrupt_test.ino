// Search pins usable for endstop-interrupts
// Compile with the same build settings you'd use for Marlin.

#if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
  #define moreDigitalPinToPCICR(p) digitalPinToPCICR(WITHIN(p, 13, 14) ? 10 : p)
#else
  #define moreDigitalPinToPCICR(p) digitalPinToPCICR(p)
#endif

void setup() {
  Serial.begin(9600);
  Serial.println("PINs causing interrupts are:");
  for (int i = 2; i < NUM_DIGITAL_PINS; i++) {
    if (moreDigitalPinToPCICR(i) || (int)digitalPinToInterrupt(i) != -1) {
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
