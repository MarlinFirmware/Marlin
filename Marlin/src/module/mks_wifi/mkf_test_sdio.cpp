#include "mks_test_sdio.h"
#include "../../MarlinCore.h"
#include "../../inc/MarlinConfig.h"
#include "../../libs/Segger/log.h"
#include "../../sd/cardreader.h"
#ifdef MKS_WIFI

void mks_test_sdio(void){
    uint8_t data[512];
    CardReader::mount();

    DEBUG("Start sdio test");
    
    for(uint32_t i=0; i<100000; i++){
        if((i%1000) == 0){
            DEBUG("Progress %d blocks",i);
        }
        SDIO_ReadBlock(i,data);
    }
    
    DEBUG("Finish sdio test");

}


#endif
