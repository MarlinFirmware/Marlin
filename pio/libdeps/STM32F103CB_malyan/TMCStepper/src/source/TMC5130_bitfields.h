#pragma once
#pragma pack(push, 1)

struct SLAVECONF_t {
  constexpr static uint8_t address = 0x03;
  union {
    uint16_t sr : 12;
    struct {
      uint8_t slaveaddr : 8;
      uint8_t senddelay : 4;
    };
  };
};

namespace TMC5130_n {
  struct IOIN_t {
    constexpr static uint8_t address = 0x04;
    union {
      uint32_t sr;
      struct {
        bool  refl_step : 1,
              refr_dir : 1,
              encb_dcen_cfg4 : 1,
              enca_dcin_cfg5 : 1,
              drv_enn_cfg6 : 1,
              enc_n_dco : 1,
              sd_mode : 1,
              swcomp_in : 1;
        uint16_t : 16;
        uint8_t version : 8;
      };
    };
  };
}

struct OUTPUT_t {
  constexpr static uint8_t address = 0x04;
  bool sr : 1;
};

struct X_COMPARE_t {
  constexpr static uint8_t address = 0x05;
  uint32_t sr;
};

struct RAMPMODE_t {
  constexpr static uint8_t address = 0x20;
  uint8_t sr : 2;
};

struct XACTUAL_t {
  constexpr static uint8_t address = 0x21;
  uint32_t sr;
};

struct VSTART_t {
  constexpr static uint8_t address = 0x23;
  uint32_t sr : 18;
};

struct A1_t {
  constexpr static uint8_t address = 0x24;
  uint16_t sr : 16;
};

struct V1_t {
  constexpr static uint8_t address = 0x25;
  uint32_t sr : 20;
};

struct AMAX_t {
  constexpr static uint8_t address = 0x26;
  uint16_t sr : 16;
};

struct VMAX_t {
  constexpr static uint8_t address = 0x27;
  uint32_t sr : 23;
};

struct DMAX_t {
  constexpr static uint8_t address = 0x28;
  uint16_t sr : 16;
};

struct D1_t {
  constexpr static uint8_t address = 0x2A;
  uint16_t sr : 16;
};

struct VSTOP_t {
  constexpr static uint8_t address = 0x2B;
  uint32_t sr : 18;
};

struct TZEROWAIT_t {
  constexpr static uint8_t address = 0x2C;
  uint16_t sr : 16;
};

struct SW_MODE_t {
  constexpr static uint8_t address = 0x34;
  union {
    uint16_t sr : 12;
    struct {
      bool  stop_l_enable : 1,
            stop_r_enable : 1,
            pol_stop_l : 1,
            pol_stop_r : 1,
            swap_lr : 1,
            latch_l_active : 1,
            latch_l_inactive : 1,
            latch_r_active : 1,
            latch_r_inactive : 1,
            en_latch_encoder : 1,
            sg_stop : 1,
            en_softstop : 1;
    };
  };
};

struct RAMP_STAT_t {
  constexpr static uint8_t address = 0x35;
  union {
    uint16_t sr : 14;
    struct {
      bool  status_stop_l : 1,
            status_stop_r : 1,
            status_latch_l : 1,
            status_latch_r : 1,
            event_stop_l : 1,
            event_stop_r : 1,
            event_stop_sg : 1,
            event_pos_reached : 1,
            velocity_reached : 1,
            position_reached : 1,
            vzero : 1,
            t_zerowait_active : 1,
            second_move : 1,
            status_sg : 1;
    };
  };
};

struct ENCMODE_t {
  constexpr static uint8_t address = 0x38;
  union {
    uint16_t sr : 11;
    struct {
      bool  pol_a : 1,
            pol_b : 1,
            pol_n : 1,
            ignore_ab : 1,
            clr_cont : 1,
            clr_once : 1,
            pos_edge : 1,
            neg_edge : 1,
            clr_enc_x : 1,
            latch_x_act : 1,
            enc_sel_decimal : 1;
    };
  };
};

struct ENC_CONST_t {
  constexpr static uint8_t address = 0x3A;
  uint32_t sr;
};

struct MSLUT0_t {
  constexpr static uint8_t address = 0x60;
  uint32_t sr;
};

struct MSLUT1_t {
  constexpr static uint8_t address = 0x61;
  uint32_t sr;
};

struct MSLUT2_t {
  constexpr static uint8_t address = 0x62;
  uint32_t sr;
};

struct MSLUT3_t {
  constexpr static uint8_t address = 0x63;
  uint32_t sr;
};

struct MSLUT4_t {
  constexpr static uint8_t address = 0x64;
  uint32_t sr;
};

struct MSLUT5_t {
  constexpr static uint8_t address = 0x65;
  uint32_t sr;
};

struct MSLUT6_t {
  constexpr static uint8_t address = 0x66;
  uint32_t sr;
};

struct MSLUT7_t {
  constexpr static uint8_t address = 0x67;
  uint32_t sr;
};

struct MSLUTSEL_t {
  constexpr static uint8_t address = 0x68;
  union {
    uint32_t sr;
    struct {
      uint8_t w0 : 2,
              w1 : 2,
              w2 : 2,
              w3 : 2,
              x1 : 8,
              x2 : 8,
              x3 : 8;
    };
  };
};

struct MSLUTSTART_t {
  constexpr static uint8_t address = 0x69;
  union {
    uint32_t sr : 24;
    struct {
      uint8_t start_sin : 8,
                        : 8,
              start_sin90 : 8;
    };
  };
};

#pragma pack(pop)
