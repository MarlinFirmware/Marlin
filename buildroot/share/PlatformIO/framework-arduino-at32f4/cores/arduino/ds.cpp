#include "ds.h"
#include "at32f403a_407.h"
#include "at32f403a_407_crm.h"
#include "at32f403a_407_gpio.h"
#include "at32f403a_407_tmr.h"
#include "at32f403a_407_dma.h"



// ================= GLOBAL =================

uint16_t dshot_buffer[MOTOR_COUNT][DSHOT_BUFFER_LEN];
uint16_t dma_buffer[MOTOR_COUNT * DSHOT_BUFFER_LEN];

uint16_t period;
uint16_t duty_1;
uint16_t duty_0;

// ================= CRC =================

uint16_t dshot_preparee(uint16_t value)
{
    value <<= 1;

    uint16_t csum = 0;
    uint16_t csum_data = value;

    for (int i = 0; i < 3; i++) {
        csum ^= csum_data;
        csum_data >>= 4;
    }

    return (value << 4) | (csum & 0xF);
}

// ================= INIT =================

void dshot_initt(void)
{
    crm_clocks_freq_type clocks;
    crm_clocks_freq_get(&clocks);

    uint32_t timer_clk = clocks.apb1_freq * 2;

    period = timer_clk / DSHOT600;
    duty_1 = (period * 3) / 4;
    duty_0 = (period * 3) / 8;

    // CLOCK
    crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_TMR2_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_DMA1_PERIPH_CLOCK, TRUE);

    // GPIO (PA0-PA3 → CH1-CH4)
    gpio_init_type gpio;
    gpio_default_para_init(&gpio);

    gpio.gpio_mode = GPIO_MODE_MUX;
    gpio.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
    gpio.gpio_pull = GPIO_PULL_NONE;
    gpio.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;

    gpio.gpio_pins = GPIO_PINS_0 | GPIO_PINS_1 | GPIO_PINS_2 | GPIO_PINS_3;
    gpio_init(GPIOA, &gpio);

    // TIMER
    tmr_base_init(TMR2, period - 1, 0);
    tmr_cnt_dir_set(TMR2, TMR_COUNT_UP);

    tmr_output_config_type oc;
    tmr_output_default_para_init(&oc);

    oc.oc_mode = TMR_OUTPUT_CONTROL_PWM_MODE_A;
    oc.oc_output_state = TRUE;
    oc.oc_polarity = TMR_OUTPUT_ACTIVE_HIGH;

    tmr_output_channel_config(TMR2, TMR_SELECT_CHANNEL_1, &oc);
    tmr_output_channel_config(TMR2, TMR_SELECT_CHANNEL_2, &oc);
    tmr_output_channel_config(TMR2, TMR_SELECT_CHANNEL_3, &oc);
    tmr_output_channel_config(TMR2, TMR_SELECT_CHANNEL_4, &oc);

    tmr_output_channel_buffer_enable(TMR2, TMR_SELECT_CHANNEL_1, TRUE);
    tmr_output_channel_buffer_enable(TMR2, TMR_SELECT_CHANNEL_2, TRUE);
    tmr_output_channel_buffer_enable(TMR2, TMR_SELECT_CHANNEL_3, TRUE);
    tmr_output_channel_buffer_enable(TMR2, TMR_SELECT_CHANNEL_4, TRUE);

    // 🔥 DMA FLEXIBLE → OVERFLOW
    dma_flexible_config(DMA1, FLEX_CHANNEL1, DMA_FLEXIBLE_TMR2_OVERFLOW);

    // DMA CONFIG
    dma_init_type dma;
    dma_default_para_init(&dma);

    dma.buffer_size = MOTOR_COUNT * DSHOT_BUFFER_LEN;
    dma.direction = DMA_DIR_MEMORY_TO_PERIPHERAL;
    dma.memory_base_addr = (uint32_t)dma_buffer;
    dma.memory_inc_enable = TRUE;

    dma.peripheral_base_addr = (uint32_t)&TMR2->dmadt;
    dma.peripheral_inc_enable = FALSE;

    dma.memory_data_width = DMA_MEMORY_DATA_WIDTH_HALFWORD;
    dma.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_HALFWORD;

    dma.priority = DMA_PRIORITY_HIGH;

    dma_init(DMA1_CHANNEL5, &dma);

    dma_channel_enable(DMA1_CHANNEL5, FALSE);

    // 🔥 TIMER DMA REQUEST (OVERFLOW)
    tmr_dma_request_enable(TMR2, TMR_OVERFLOW_DMA_REQUEST, TRUE);

    tmr_output_enable(TMR2, TRUE);
    tmr_counter_enable(TMR2, TRUE);
}

// ================= BUILD BUFFER =================

void dshot_build_framee(uint16_t *buf, uint16_t throttle)
{
    if(throttle != 0) {
        if(throttle < 48) throttle = 48;
        if(throttle > 2047) throttle = 2047;
    }

    uint16_t packet = dshot_preparee(throttle);

    for(int i=0;i<16;i++){
        if(packet & (1 << (15-i)))
            buf[i] = duty_1;
        else
            buf[i] = duty_0;
    }

    buf[16] = 0;
    buf[17] = 0;
}

// ================= SEND =================

void dshot_send_all(uint16_t m1, uint16_t m2, uint16_t m3, uint16_t m4)
{
    dshot_build_framee(dshot_buffer[0], m1);
    dshot_build_framee(dshot_buffer[1], m2);
    dshot_build_framee(dshot_buffer[2], m3);
    dshot_build_framee(dshot_buffer[3], m4);

    // interleave → DMA için
    int idx = 0;
    for(int i=0;i<DSHOT_BUFFER_LEN;i++){
        dma_buffer[idx++] = dshot_buffer[0][i];
        dma_buffer[idx++] = dshot_buffer[1][i];
        dma_buffer[idx++] = dshot_buffer[2][i];
        dma_buffer[idx++] = dshot_buffer[3][i];
    }

    // restart DMA
    tmr_counter_value_set(TMR2, 0);

    dma_channel_enable(DMA1_CHANNEL5, FALSE);
    dma_flag_clear(DMA1_FDT5_FLAG);
    dma_data_number_set(DMA1_CHANNEL5, MOTOR_COUNT * DSHOT_BUFFER_LEN);
    dma_channel_enable(DMA1_CHANNEL5, TRUE);
}

// ================= MAIN =================

// int main(void)
// {
//     system_clock_config();

//     dshot_init();

//     // ARM
//     for(int i=0;i<1000;i++){
//         dshot_send_all(0,0,0,0);
//         delay_ms(1);
//     }

//     while(1)
//     {
//         dshot_send_all(100, 200, 300, 400);
//         delay_ms(1);
//     }
// }