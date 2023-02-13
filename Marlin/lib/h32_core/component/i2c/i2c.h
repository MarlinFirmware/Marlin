#ifndef _I2C_EEPROM_H_
#define _I2C_EEPROM_H_

#include "hc32_common.h"

#ifdef __cplusplus
extern "C"
{
#endif

/********************************************/
#define EEPROM_I2C_CH (M4_I2C1)
#define I2C_ADDRESS_W 0x00u
#define I2C_ADDRESS_R 0x01u
#define TIMEOUT 0x10000ul // 0x10000ul
/********************************************/
#define EEPROM_ADDR 0xA0u
    /********************************************/

    typedef enum en_i2c_ret
    {
        I2C_RET_OK = 0u,
        I2C_RET_ERROR = 1u,
    } i2c_ret_t;

    typedef enum en_generate
    {
        GENERATE_START = 0u,
        GENERATE_RESTART = 1u,
    } i2c_generate_t;

    typedef enum i2c_result
    {
        default_t = 0u,
        successfully = 1u,
        start_fail = 2u,
        drvAddr_fail = 3u,
        Addr_fail = 4u,
        data_fail = 5u,
        stop_fail = 6u,
        restart_fail = 7u,
        read_drvAddr_fail = 8u,
    } i2c_result_t;

    extern void setup_I2C1_eeprom(void);
    extern i2c_result_t WritePageData(M4_I2C_TypeDef *pstcI2Cx, uint8_t drvAddr, uint16_t addr, const uint8_t *startPtr, uint8_t len);
    extern i2c_result_t ReadPageData(M4_I2C_TypeDef *pstcI2Cx, uint8_t drvAddr, uint16_t addr, uint8_t len, uint8_t *target);
    extern uint8_t WriteFlashPageData(uint16_t addr, const uint8_t *p_startaddr, uint8_t len);
    extern uint8_t ReadFlashPageData(uint16_t addr, uint8_t len, uint8_t *p_startaddr);
    extern void readFlash(void);
    extern i2c_result_t WriteOneByteData(uint16_t WriteAddr, uint8_t DataToWrite);
    extern uint8_t ReadOneByteData(uint16_t ReadAddr);

#ifdef __cplusplus
}
#endif

#endif
