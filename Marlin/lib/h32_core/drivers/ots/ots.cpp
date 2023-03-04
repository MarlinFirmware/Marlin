#include "ots.h"
#include "../../hdsc/common/hc32_ddl.h"

void H32OTS::init()
{
    // enable and configure OTS
    stc_ots_init_t otsConf = {
        .enAutoOff = OtsAutoOff_Disable,
#if OTS_USE_XTAL
        .enClkSel = OtsClkSel_Xtal,
#else
        .enClkSel = OtsClkSel_Hrc,
#endif
        .u8ClkFreq = uint8_t(SystemCoreClock / 1000000ul),
    };
    PWC_Fcg3PeriphClockCmd(PWC_FCG3_PERIPH_OTS, Enable);
    OTS_Init(&otsConf);

// setup the OTS clock
#if OTS_USE_XTAL
    // enable XTAL
    CLK_XtalCmd(Enable);
#else
    // enable HRC
    CLK_HrcCmd(Enable);
    CLK_Xtal32Cmd(Enable);
#endif

    // LRC is always needed
    CLK_LrcCmd(Enable);
}

bool H32OTS::read(float &temperature)
{
    en_result_t err = OTS_StartGetTemp(&temperature, OTS_READ_TIMEOUT);
    if (err != Ok)
    {
        printf("OTS_StartGetTemp failed: err=%d\n", err);
        return false;
    }

    return true;
}
