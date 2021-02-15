#pragma once

#define DEBUG_PRINT(CFG, VAL) Serial.print(CFG); Serial.print('('); Serial.print(VAL, HEX); Serial.println(')')
//#define WRITE_REG(R) write(R##_register.address, R##_register.sr)
//#define READ_REG(R) read(R##_register.address)
