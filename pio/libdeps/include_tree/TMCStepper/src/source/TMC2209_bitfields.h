#pragma once
#pragma pack(push, 1)

namespace TMC2209_n {
  struct IOIN_t {
    constexpr static uint8_t address = 0x06;
    union {
      uint32_t sr;
      struct {
        bool  enn : 1,
                  : 1,
              ms1 : 1,
              ms2 : 1,
              diag : 1,
                   : 1,
              pdn_uart : 1,
              step : 1,
              spread_en : 1,
              dir : 1;
        uint16_t : 14;
        uint8_t version : 8;
      };
    };
  };

  struct SGTHRS_t {
    constexpr static uint8_t address = 0x40;
    uint8_t sr : 8;
  };

  struct SG_RESULT_t {
    constexpr static uint8_t address = 0x41;
    uint16_t sr : 10;
  };

  struct COOLCONF_t {
    constexpr static uint8_t address = 0x42;
    union {
      uint16_t sr;
      struct {
        uint8_t semin : 4,
                      : 1,
                seup : 2,
                      : 1,
                semax : 4,
                      : 1,
                sedn : 2;
        bool    seimin : 1;
      };
    };
  };
}

#pragma pack(pop)
