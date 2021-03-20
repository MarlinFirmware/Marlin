#pragma once
#include "../inc/MarlinConfig.h"

#ifdef LONGER3D
#include "../../sd/cardreader.h"
static char *strchr_vpointer;
class sdfile
{
private:
    /* data */
public:

    bool filenameIsFolder=false;
    bool is_gettime=false;
    sdfile(/* args */);
    ~sdfile();
    uint16_t getsdfilecount();
    uint16_t getsdfilepage();
    char* getsdfilename(uint16_t nr);
    char* getprintfilename(void);

    void ChoseFile(uint8_t index);
    bool sdcardcheckstatus();
    void sdopenAndPrintFile();
    void getfiletotaltime();
    void actPrintTimeCalculateLegacy(void);
    void actPrintTimeCalculate(void);
    void calcCountUpTime(void);
    void calcCountDownTime(void);
    bool ReadTF_code_seen(char *Buffer,char code)
    {
        strchr_vpointer = strchr(Buffer,code);
        return(strchr_vpointer != NULL);
    }
    float ReadTF_code_value(char *Buffer)
    {
	    return(strtod(&Buffer[strchr_vpointer - Buffer + 1],NULL));
    }
    float ReadTF_code_value1(char *Buffer)
    {
	    return(strtod(&Buffer[strchr_vpointer - Buffer + 17],NULL));
    }
    char* sdcode_seen(char code,char* data)
    {
        return strchr( data, code );  //Return True if a character was found
    }
    float sdcode_value(char* data)
    {
        return (strtod(data, NULL));
    }
};
void checkFilament(void);
void checkFilamentReset(void);
void savePausePositonMove(void);

// typedef struct
// {
//     uint32_t    wtimes;
//     uint32_t    zheight;
//     char        sd_filename[MAXPATHNAMELENGTH];
//     uint32_t    filepos;
//     uint16_t    feedrate;
//     int16_t     tgetemp[HOTENDS];
//  #if HAS_HEATED_BED
//     int16_t     tgbedtemp;
//  #endif
//     uint8_t     fanspeed;
//     millis_t print_job_elapsed;
//     uint32_t totaltime;

// }save_recovery_info;
// uint32_t getFileSaveBlockAddr(void);
// void getSaveData( char* data );
// void setFlashSave( bool set);
// void saveRecoveryData(void);
// void checkRecoveryStatus(void);
// void recoveryResume(void);
#endif