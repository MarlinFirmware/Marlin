//#include "../../inc/MarlinConfig.h"
#include "../gcode.h"
#include "../../lcd/menu/touch/calibration.h"
extern TouchCalibration calibration;
#ifdef LONGER3D
#include "../../longer/display_image.h"
#include "../../longer/sdfile.h"
#include "../../longer/LGT_MACRO.h"
 #include "../../longer/W25Qxx.h"
extern E_WINDOW_ID current_window_ID;
extern display_image Display_Screen;
extern char cur_pstatus;
extern bool is_filament_check_disable;
extern  W25Qxx W25QxxFlash;
extern  SAVEDATA flashsave;
//bool nofilastart=true;
    void GcodeSuite::M995()    //Calibration of LCD Screen
    {
       calibration.init_calibration(1);
        for(int i=0;i<4;i++)
        flashsave.s_result[i]=calibration.results[i];
        flashsave.checksflag=SCREEN_CALIBRATE;
        W25QxxFlash.W25QXX_Write((uint8_t*)&flashsave,SAVE_BASE_ADDR,sizeof(SAVEDATA));
        Display_Screen.displayWindowHome();
    }
    void GcodeSuite::M2000()
    {
        //current_window_ID=eMENU_DIALOG_NO_FIL;
        //Display_Screen.displayNofilament();
        if(is_filament_check_disable)  ////true: no filament
            cur_pstatus=2;
       // if(!nofilastart)
            savePausePositonMove();   //save current position and move to a specified location
    }
#else
 void GcodeSuite::M995()    //Calibration of LCD Screen 
 {
      calibration.init_calibration(1);
 }
#endif
