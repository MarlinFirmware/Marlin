#include "UbxGpsParser.h"

UbxGpsParser::UbxGpsParser() {
    reset();
}

bool UbxGpsParser::parseByte(uint8_t c) {
    switch (state) {
        case UBX_SYNC1:
            if (c == 0xB5) state = UBX_SYNC2;
            break;

        case UBX_SYNC2:
            state = (c == 0x62) ? UBX_CLASS : UBX_SYNC1;
            break;

        case UBX_CLASS:
            cls = c;
            ck_a = c;
            ck_b = ck_a;
            state = UBX_ID;
            break;

        case UBX_ID:
            id = c;
            ck_a += c;
            ck_b += ck_a;
            state = UBX_LEN1;
            break;

        case UBX_LEN1:
            len = c;
            ck_a += c;
            ck_b += ck_a;
            state = UBX_LEN2;
            break;

        case UBX_LEN2:
            len |= (c << 8);
            ck_a += c;
            ck_b += ck_a;
            cnt = 0;
            if (len > sizeof(payload)) {
                reset();
            } else {
                state = UBX_PAYLOAD;
            }
            break;

        case UBX_PAYLOAD:
            payload[cnt++] = c;
            ck_a += c;
            ck_b += ck_a;
            if (cnt >= len) {
                state = UBX_CKA;
            }
            break;

        case UBX_CKA:
            state = (c == ck_a) ? UBX_CKB : UBX_SYNC1;
            break;

case UBX_CKB:
        bool valid = (c == ck_b);
        // Serial.print("UBX mesajı tamamlandı - Class: 0x"); Serial.print(cls, HEX);
        // Serial.print(" ID: 0x"); Serial.print(id, HEX);
        // Serial.print(" Len: "); Serial.print(len);
        // Serial.print(" Checksum valid: "); Serial.println(valid ? "Evet" : "Hayır");
        
        
        if (valid) {
            processMessage();
        }
        reset();
        return valid;
    }
    return false;
}

const UbxGpsParser::GpsData& UbxGpsParser::getData() const {
    return data;
}

bool UbxGpsParser::hasNewData() {
    if (data.newData) {
        data.newData = false;
        return true;
    }
    return false;
}

void UbxGpsParser::reset() {
    state = UBX_SYNC1;
    cnt = 0;
    ck_a = 0;
    ck_b = 0;
    len = 0;
    cls = 0;
    id = 0;
    data.newData = false;
}

void UbxGpsParser::processMessage() {
    if (len < 92) {
        return;
    }


    if (cls != 0x01 || id != 0x07 || len < 92) {
        Serial.println("Geçersiz NAV-PVT mesajı");
        return;
    }

    data.fixType = payload[20];
    data.numSV   = payload[23];

    data.lon = (*(int32_t*)&payload[24]) / 10000000.0;
    data.lat = (*(int32_t*)&payload[28]) / 10000000.0;

    data.height = *(int32_t*)&payload[32];   // mm MSL
    data.hMSL   = *(int32_t*)&payload[36];   // mm WGS84

    data.velN = *(int32_t*)&payload[48];
    data.velE = *(int32_t*)&payload[52];
    data.velD = *(int32_t*)&payload[56];

    data.speed  = *(uint32_t*)&payload[60];
    data.gSpeed = *(uint32_t*)&payload[64];

    data.pDOP = *(uint16_t*)&payload[76];
    data.hDOP = *(uint16_t*)&payload[78];
    data.vDOP = *(uint16_t*)&payload[80];

    data.tAcc = *(uint32_t*)&payload[88];

    data.valid   = (data.fixType >= 3);
    data.newData = true;
}