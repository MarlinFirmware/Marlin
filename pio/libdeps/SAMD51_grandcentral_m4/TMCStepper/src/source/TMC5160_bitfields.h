#pragma once
#pragma pack(push, 1)

struct SHORT_CONF_t {
  constexpr static uint8_t address = 0x09;
  union {
    uint32_t sr : 19;
    struct {
      uint8_t s2vs_level  : 4,
                          : 4,
              s2g_level   : 4,
                          : 4,
              shortfilter : 2;
      bool shortdelay : 1;
    };
  };
};

struct DRV_CONF_t {
  constexpr static uint8_t address = 0x0A;
  union {
    uint32_t sr : 22;
    struct {
      uint8_t bbmtime : 5,
                      : 3,
              bbmclks : 4,
                      : 4,
              otselect : 2,
              drvstrength : 2,
              filt_isense : 2;
    };
  };
};

struct GLOBAL_SCALER_t {
  constexpr static uint8_t address = 0x0B;
  uint8_t sr;
};

struct OFFSET_READ_t {
  constexpr static uint8_t address = 0x0C;
};

struct ENC_DEVIATION_t {
  constexpr static uint8_t address = 0x3D;
  uint32_t sr : 20;
};

struct PWM_AUTO_t {
  constexpr static uint8_t address = 0x72;
  union {
    uint32_t sr : 24;
    struct {
      uint8_t pwm_ofs_auto : 8,
                           : 8,
              pwm_grad_auto : 8;
    };
  };
};

#pragma pack(pop)
