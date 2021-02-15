#pragma once
#pragma pack(push, 1)

namespace TMC2208_n {
  struct GCONF_t {
    constexpr static uint8_t address = 0x00;
    union {
      uint16_t sr : 10;
      struct {
        bool  i_scale_analog : 1,
              internal_rsense : 1,
              en_spreadcycle : 1,
              shaft : 1,
              index_otpw : 1,
              index_step : 1,
              pdn_disable : 1,
              mstep_reg_select : 1,
              multistep_filt : 1,
              test_mode : 1;
      };
    };
  };
}

namespace TMC2208_n {
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
              sel_a : 1,
              dir : 1;
        uint16_t : 14;
        uint8_t version : 8;
      };
    };
  };
}

namespace TMC2224_n {
  struct IOIN_t {
    constexpr static uint8_t address = 0x06;
    union {
      uint32_t sr;
      struct {
        bool  : 1,
              pdn_uart : 1,
              spread : 1,
              dir : 1,
              enn : 1,
              step : 1,
              ms1 : 1,
              ms2 : 1,
              sel_a : 1;
        uint16_t : 15;
        uint8_t version : 8;
      };
    };
  };
}

struct FACTORY_CONF_t {
  constexpr static uint8_t address = 0x07;
  union {
    uint16_t sr;
    struct {
        uint8_t fclktrim : 5,
                         : 3,
                ottrim : 2;
    };
  };
};

namespace TMC2208_n {
  struct VACTUAL_t {
    constexpr static uint8_t address = 0x22;
    uint32_t sr;
  };
}

struct MSCURACT_t {
  constexpr static uint8_t address = 0x6B;
  union {
    uint32_t sr : 25;
    struct {
      int16_t cur_a : 9,
                    : 7,
              cur_b : 9;
    };
  };
};

namespace TMC2208_n {
  struct CHOPCONF_t {
    constexpr static uint8_t address = 0x6C;
    union {
      uint32_t sr;
      struct {
        uint8_t toff : 4,
                hstrt : 3,
                hend : 4,
                     : 4,
                tbl : 2;
        bool    vsense : 1;
        uint8_t : 6,
                mres : 4;
        bool    intpol : 1,
                dedge : 1,
                diss2g : 1,
                diss2vs : 1;
      };
    };
  };

  struct PWMCONF_t {
    constexpr static uint8_t address = 0x70;
    union {
      uint32_t sr;
      struct {
        uint8_t pwm_ofs : 8,
                pwm_grad : 8,
                pwm_freq : 2;
        bool pwm_autoscale : 1,
             pwm_autograd : 1;
        uint8_t freewheel : 2,
                          : 2,
                pwm_reg : 4,
                pwm_lim : 4;
      };
    };
  };

  struct DRV_STATUS_t {
    constexpr static uint8_t address = 0x6F;
    union {
      uint32_t sr;
      struct {
        bool otpw : 1,
             ot : 1,
             s2ga : 1,
             s2gb : 1,
             s2vsa : 1,
             s2vsb : 1,
             ola : 1,
             olb : 1,
             t120 : 1,
             t143 : 1,
             t150 : 1,
             t157 : 1;
        uint8_t : 4,
                cs_actual : 5,
                : 3,
                : 6;
        bool stealth : 1,
             stst : 1;
      };
    };
  };

  struct PWM_SCALE_t {
    constexpr static uint8_t address = 0x71;
    union {
      uint32_t sr;
      struct {
        uint8_t pwm_scale_sum : 8,
                : 8;
        int16_t pwm_scale_auto : 9;
      };
    };
  };
}

#pragma pack(pop)
