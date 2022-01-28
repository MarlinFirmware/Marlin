#include "mks_wifi_ui.h"

#if ENABLED(TFT_480x320) || ENABLED(TFT_480x320_SPI)

#include "../../lcd/tft/tft.h"
#include "../../lcd/tft/tft_color.h"
#include "../../lcd/marlinui.h"
#include "../temperature.h"
#include "../../libs/numtostr.h"


extern TFT tft;
float total_size = 0;
float one_percent = 0;
//char str[100];

void mks_update_status(char *filename, uint32_t current_filesize, uint32_t file_size){
    static uint32_t call_count = 0;
    static uint32_t last_value = 200;
    // fit to 0.01 MB
    uint32_t percent_done = current_filesize / one_percent;
    
    // thermalManager.setTargetBed(0);
    // thermalManager.setTargetHotend(0,0);
    // thermalManager.manage_heater();
    // OUT_WRITE(FAN1_PIN,HIGH);

    if((percent_done != last_value)){
      call_count++;
      tft.queue.reset();
      tft.canvas(0, 0, TFT_WIDTH, TFT_HEIGHT);
      tft.set_background(COLOR_BACKGROUND);

      // filename
      tft_string.set(filename);
      tft.add_text(tft_string.center(TFT_WIDTH), 100, COLOR_CONTROL_ENABLED, tft_string);
     /*
      // size
      float current = current_filesize / 1048576;
      tft_string.set(ftostr32_62(current));
      tft_string.trim();
      tft_string.add(" / ");
      tft_string.add(ftostr32_62(total_size));
      tft_string.add(" MB ");
      tft_string.add(ftostr32_62(call_count));
      tft.add_text(tft_string.center(TFT_WIDTH), 140, COLOR_CONTROL_ENABLED, tft_string);
    
      tft.add_bar(2, 180, TFT_WIDTH - 4, 40, COLOR_PROGRESS_BG);
      tft.add_rectangle(2, 180, TFT_WIDTH - 4, 40, COLOR_PROGRESS_FRAME);
      if (percent_done)
        tft.add_bar(4, 182, ((TFT_WIDTH - 8) * percent_done) / 100, 36, COLOR_PROGRESS_BAR);
      tft_string.set(pcttostrpctrj(percent_done));
      tft_string.trim();
      tft.add_text(240 - tft_string.width() / 2, 188, COLOR_PROGRESS_TEXT, tft_string);
    */
      tft.queue.sync();
      last_value = percent_done;
      //ui.update();
    };

}


void mks_upload_screen(char *filename, uint32_t file_size){

      // fit to 0.01 MB
      total_size = (float)file_size / 1048576;
      one_percent = total_size / 100;
      #ifndef SHOW_PROGRESS
        tft.queue.reset();
        tft.canvas(0, 0, TFT_WIDTH, TFT_HEIGHT);
        tft.set_background(COLOR_BACKGROUND);
        tft_string.set("File uploading...");
        tft.add_text(tft_string.center(TFT_WIDTH), 100, COLOR_CONTROL_ENABLED, tft_string);
        tft_string.set(filename);
        tft.add_text(tft_string.center(TFT_WIDTH), 140, COLOR_CONTROL_ENABLED, tft_string);

        tft_string.set(ftostr32_62(total_size));
        tft_string.add(" MB ");
        tft.add_text(tft_string.center(TFT_WIDTH), 180, COLOR_CONTROL_ENABLED, tft_string);
        tft.queue.sync();
      #endif
}


void mks_end_transmit(void){
  tft.queue.reset();
  tft.fill(0, 0, TFT_WIDTH, TFT_HEIGHT, COLOR_BACKGROUND);
  tft.queue.sync();
}


#endif