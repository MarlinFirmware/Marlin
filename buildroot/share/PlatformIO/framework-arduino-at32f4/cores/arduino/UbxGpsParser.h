#ifndef UBX_GPS_PARSER_H
#define UBX_GPS_PARSER_H

#include <Arduino.h>

class UbxGpsParser {
public:
    // GPS verilerini tutan yapı (public erişim için dışarıda)
    struct GpsData {
        double lat;       // derece
        double lon;       // derece
        int32_t height;   // mm MSL
        int32_t hMSL;     // mm WGS84 elipsoid
        int32_t velN;     // mm/s Kuzey
        int32_t velE;     // mm/s Doğu
        int32_t velD;     // mm/s Aşağı
        uint32_t speed;   // mm/s 3D hız
        uint32_t gSpeed;  // mm/s ground speed
        uint8_t fixType;  // 0=no fix, 3=3D fix, 4=GNSS+dead reckoning
        uint8_t numSV;    // kullanılan uydu sayısı
        uint16_t pDOP;    // Position DOP ×100
        uint16_t hDOP;    // Horizontal DOP ×100
        uint16_t vDOP;    // Vertical DOP ×100
        uint32_t tAcc;    // Time accuracy ns
        bool valid;       // fix geçerli mi?
        bool newData;     // yeni veri geldi mi? (flag)
    };

    // Yapıcı
    UbxGpsParser();

    // Yeni bir bayt geldiğinde çağrılır
    // true dönerse → yeni ve geçerli bir NAV-PVT mesajı tamamlandı
    bool parseByte(uint8_t c);

    // Son alınan geçerli NAV-PVT verisine erişim
    const GpsData& getData() const;

    // Veri yeni mi? (bir kere okunduktan sonra false olur)
    bool hasNewData();

    // Parser'ı sıfırlama
    void reset();

private:
    enum State {
        UBX_SYNC1,
        UBX_SYNC2,
        UBX_CLASS,
        UBX_ID,
        UBX_LEN1,
        UBX_LEN2,
        UBX_PAYLOAD,
        UBX_CKA,
        UBX_CKB
    };

    State state;
    uint8_t cls;
    uint8_t id;
    uint16_t len;
    uint16_t cnt;
    uint8_t ck_a;
    uint8_t ck_b;
    uint8_t payload[300];

    GpsData data;

    // NAV-PVT mesajını işler
    void processMessage();
};

#endif // UBX_GPS_PARSER_H