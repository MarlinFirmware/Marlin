#ifndef TMCStepper_UTILITY_h
#define TMCStepper_UTILITY_h

void print_HEX(uint32_t data) {
  for(uint8_t B=24; B>=4; B-=8){
    Serial.print((data>>(B+4))&0xF, HEX);
    Serial.print((data>>B)&0xF, HEX);
    Serial.print(":");
  }
  Serial.print((data>>4)&0xF, HEX);
  Serial.print(data&0xF, HEX);
}

void print_BIN(uint32_t data) {
  int b = 31;
  for(; b>=24; b--){
    Serial.print((data>>b)&0b1);
  }
  Serial.print(".");
  for(; b>=16; b--){
    Serial.print((data>>b)&0b1);
  }
  Serial.print(".");
  for(; b>=8; b--){
    Serial.print((data>>b)&0b1);
  }
  Serial.print(".");
  for(; b>=0; b--){
    Serial.print((data>>b)&0b1);
  }
}

#endif
