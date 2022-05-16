#pragma once

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(BTT_TFT)
#include "bttSerial.h"
#include "btt_tft_gcode_request.h"
#include "../extui/ui_api.h"

#include "../marlinui.h"
#include "../../MarlinCore.h"
#include "../../gcode/gcode.h"
#include "../../gcode/queue.h"

#define MAX_FOLDER_DEPTH 4                 // Limit folder depth TFT has a limit for the file path
#define MAX_CMND_LEN 16 * MAX_FOLDER_DEPTH // Maximum Length for a Panel command
#define MAX_PATH_LEN 16 * MAX_FOLDER_DEPTH // Maximum number of characters in a SD file path

class btt_tft_api
{
private:
  /* data */
public:
  void update();
  void init();
};
void M();
void G();
void T();
bool ReadTFTCommand();
void command_gcode();

#endif