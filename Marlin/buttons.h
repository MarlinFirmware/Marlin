
#define BUTTONS_HAVEENCODER

extern volatile char buttons;
extern volatile int encoderpos;

//buttons are attached to a shift register
#define SHIFT_CLK 38
#define SHIFT_LD 42
#define SHIFT_OUT  40
#define SHIFT_EN  17

// blocking time for recognizing a new keypress of one key, ms
#define blocktime 500 


//bits in the shift register that carry the buttons for:
// left up center down right red
#define BL_LE 7
#define BL_UP 6
#define BL_MI 5
#define BL_DW 4
#define BL_RI 3
#define BL_ST 2

//if there is a rotary encoder, its two bits are at the shift register byte position:
#ifdef BUTTONS_HAVEENCODER

  #define BLEN_B 1
  #define BLEN_A 0

  //encoder rotation values
  #define encrot0 0
  #define encrot1 2
  #define encrot2 3
  #define encrot3 1

#endif  //BUTONS_HAVEENCODER


//atomatic, do not change
#define B_LE (1<<BL_LE)
#define B_UP (1<<BL_UP)
#define B_MI (1<<BL_MI)
#define B_DW (1<<BL_DW)
#define B_RI (1<<BL_RI)
#define B_ST (1<<BL_ST)
#ifdef BUTTONS_HAVEENCODER
  #define EN_B (1<<BLEN_B)
  #define EN_A (1<<BLEN_A)
#endif


//initialize buttons
void buttons_init();

//this routine reads in the button values into a byte, and if an encoder is present evaluate it
void buttons_check();


