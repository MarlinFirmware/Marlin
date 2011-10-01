#ifndef __ULTRALCDH
#define __ULTRALCDH

const int nrSubmenus=5;
const bool statusRight=true;

//buttons are attached to a shift register
#define SHIFT_CLK 38
#define SHIFT_LD 42
#define SHIFT_OUT 40
#define SHIFT_EN 17

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

#define BLEN_B 1
#define BLEN_A 0

//encoder rotation values
#define encrot0 0
#define encrot1 2
#define encrot2 3
#define encrot3 1

//atomatic, do not change
#define B_LE (1<<BL_LE)
#define B_UP (1<<BL_UP)
#define B_MI (1<<BL_MI)
#define B_DW (1<<BL_DW)
#define B_RI (1<<BL_RI)
#define B_ST (1<<BL_ST)
#define EN_B (1<<BLEN_B)
#define EN_A (1<<BLEN_A)




class MainMenu{
public:
	MainMenu();
	void update();
	
	uint8_t activemenu;
	bool subactive;
	uint8_t displayStartingRow;
};

char *fillto(int8_t n,char *c);
char *ftostr51(const float &x);
char *ftostr31(const float &x);
char *ftostr3(const float &x);





#endif //ULTRALCD

