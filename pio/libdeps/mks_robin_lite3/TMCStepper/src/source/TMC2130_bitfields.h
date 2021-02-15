#pragma once
#pragma pack(push, 1)

struct GCONF_t {
  constexpr static uint8_t address = 0x00;
  union {
    uint32_t sr : 18;
    struct {
      bool  i_scale_analog : 1, // 2130, 5130
            internal_rsense : 1, // 2130, 5130
            en_pwm_mode : 1,
            enc_commutation : 1, // 2130, 5130
            shaft : 1,
            diag0_error : 1,
            diag0_otpw : 1,
            diag0_stall : 1,
            diag1_stall : 1,
            diag1_index : 1,
            diag1_onstate : 1,
            diag1_steps_skipped : 1,
            diag0_int_pushpull : 1,
            diag1_pushpull : 1,
            small_hysteresis : 1,
            stop_enable : 1,
            direct_mode : 1;
    };
    struct { // TMC5160
      bool recalibrate : 1,
           faststandstill : 1,
                          : 1,
           multistep_filt : 1,
                    : 3,
           diag0_step : 1,
           diag1_dir : 1,
                 : 4,
           diag1_poscomp_pushpull : 1;
    };
  };
};

struct IHOLD_IRUN_t {
  constexpr static uint8_t address = 0x10;
  union {
    uint32_t sr : 20;
    struct {
      uint8_t ihold : 5,
                    : 3,
              irun : 5,
                   : 3,
              iholddelay : 4;
    };
  };
};

struct GSTAT_t {
  constexpr static uint8_t address = 0x01;
  union {
    uint8_t sr : 3;
    struct {
      bool  reset : 1,
            drv_err : 1,
            uv_cp : 1;
    };
  };
};

struct IOIN_t {
  constexpr static uint8_t address = 0x04;
  union {
    uint32_t sr;
    struct {
      bool  step : 1,
            dir : 1,
            dcen_cfg4 : 1,
            dcin_cfg5 : 1,
            drv_enn_cfg6 : 1,
            dco : 1,
            : 2;
      uint16_t : 16;
      uint8_t version : 8;
    };
  };
};

struct TPOWERDOWN_t {
  constexpr static uint8_t address = 0x11;
  uint8_t sr : 8;
};

struct TPWMTHRS_t {
  constexpr static uint8_t address = 0x13;
  uint32_t sr : 20;
};

struct TCOOLTHRS_t {
  constexpr static uint8_t address = 0x14;
  uint32_t sr : 20;
};

struct THIGH_t {
  constexpr static uint8_t address = 0x15;
  uint32_t sr : 20;
};

struct XDIRECT_t {
  constexpr static uint8_t address = 0x2D;
  union {
    uint32_t sr : 25;
    struct {
      int16_t coil_A : 9;
      int8_t         : 7;
      int16_t coil_B : 9;
    };
  };
};

struct VDCMIN_t {
  constexpr static uint8_t address = 0x33;
  uint32_t sr : 23;
};

struct CHOPCONF_t {
  constexpr static uint8_t address = 0x6C;
  union {
    uint32_t sr : 32;
    struct {
      uint8_t toff : 4,
              hstrt : 3,
              hend : 4,
                   : 1;
      bool    disfdcc : 1,
              rndtf : 1,
              chm : 1;
      uint8_t tbl : 2;
      bool    vsense : 1,
              vhighfs : 1,
              vhighchm : 1;
      uint8_t sync : 4, // 2130, 5130
              mres : 4;
      bool    intpol : 1,
              dedge : 1,
              diss2g : 1;
    };
    struct { // TMC5160
      uint32_t     : 20;
      uint8_t tpfd : 4; // 5160
      uint16_t     : 7;
      bool diss2vs : 1; // TMC5160 only
    };
  };
};

struct COOLCONF_t {
  constexpr static uint8_t address = 0x6D;
  union {
    uint32_t sr : 25;
    struct {
      uint8_t semin : 4,
                    : 1,
              seup : 2,
                    : 1,
              semax : 4,
                    : 1,
              sedn : 2;
      bool    seimin : 1;
      int8_t  sgt : 7,
                  : 1;
      bool    sfilt : 1;
    };
  };
};

struct DCCTRL_t {
	constexpr static uint8_t address = 0x6E;
	union {
		uint32_t sr : 24;
		struct {
			uint16_t dc_time : 10,
				: 6;
			uint8_t dc_sg : 8;
		};
	};
};

namespace TMC2130_n {
  struct DRV_STATUS_t {
    constexpr static uint8_t address = 0x6F;
    union {
      uint32_t sr;
      struct {
        uint16_t sg_result : 10;
        uint8_t            : 5;
        bool fsactive : 1;
        uint8_t cs_actual : 5,
                          : 3;
        bool  stallGuard : 1,
              ot : 1,
              otpw : 1,
              s2ga : 1,
              s2gb : 1,
              ola : 1,
              olb : 1,
              stst : 1;
      };
    };
  };
}

struct PWMCONF_t {
  constexpr static uint8_t address = 0x70;
  union {
    uint32_t sr : 22;
    struct {
      uint8_t pwm_ampl : 8,
              pwm_grad : 8,
              pwm_freq : 2;
      bool pwm_autoscale : 1,
           pwm_symmetric : 1;
      uint8_t freewheel : 2;
    };
  };
};

struct ENCM_CTRL_t {
  constexpr static uint8_t address = 0x72;
  union {
    uint8_t sr : 2;
    struct {
      bool  inv : 1,
            maxspeed : 1;
    };
  };
};

#pragma pack(pop)
