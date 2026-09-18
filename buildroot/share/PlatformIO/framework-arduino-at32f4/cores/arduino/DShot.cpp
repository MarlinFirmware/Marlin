#include "DShot.h"
#include "at32f403a_407_crm.h"
#include "at32f403a_407_gpio.h"
#include "at32f403a_407_tmr.h"
#include "at32f403a_407_dma.h"

#define DSHOT_TIMER        TMR2
#define DSHOT_CHANNEL      TMR_SELECT_CHANNEL_2
#define DSHOT_DMA_CHANNEL  DMA1_CHANNEL7

static uint16_t dshot_buffer[DSHOT_BUFFER_SIZE];

static uint16_t period;
static uint16_t duty_1;
static uint16_t duty_0;


typedef struct
{
    gpio_type* GPIOx;
    tmr_type* TIMx;
    dma_type* DMAx;
    uint16_t GPIO_Pin_x;
    tmr_channel_select_type TimerChannel;
    dma_channel_type * DMA_Channel;
    uint32_t  fdtFlag;
} PinInfo_TypeDMA;

const PinInfo_TypeDMA PIN_MAPS[30] =
{
    /*GPIO_Type* GPIOx;
    TIM_Type* TIMx;
    DMA_Type* DMAx;

    uint16_t GPIO_Pin_x;
    tmr_channel_select_type TimerChannel;
    dma_channel_type * DMA_CHANNEL;*/
    {GPIOA, TMR5, DMA2,  GPIO_PINS_0, TMR_SELECT_CHANNEL_1, DMA2_CHANNEL5, DMA2_FDT5_FLAG}, /* PA0 */
    {GPIOA, TMR5, DMA2,  GPIO_PINS_1, TMR_SELECT_CHANNEL_2, DMA2_CHANNEL4, DMA2_FDT4_FLAG}, /* PA1 */
    {GPIOA, TMR5, DMA2,  GPIO_PINS_2, TMR_SELECT_CHANNEL_3, DMA2_CHANNEL2, DMA2_FDT2_FLAG}, /* PA2 */
    {GPIOA, TMR5, DMA2,  GPIO_PINS_3, TMR_SELECT_CHANNEL_4, DMA2_CHANNEL1, DMA2_FDT1_FLAG}, /* PA3 */
    {GPIOA, NULL, NULL,  GPIO_PINS_4, TMR_SELECT_CHANNEL_1, NULL}, /* PA4 */
    {GPIOA, NULL, NULL,  GPIO_PINS_5, TMR_SELECT_CHANNEL_1, NULL}, /* PA5 */
    {GPIOA, TMR3, DMA1,  GPIO_PINS_6, TMR_SELECT_CHANNEL_1, DMA1_CHANNEL6, DMA1_FDT6_FLAG}, /* PA6 */
    {GPIOA, TMR3, DMA1,  GPIO_PINS_7, TMR_SELECT_CHANNEL_2, NULL}, /* PA7 */
    {GPIOA, TMR1, DMA1,  GPIO_PINS_8, TMR_SELECT_CHANNEL_1, DMA1_CHANNEL2, DMA1_FDT2_FLAG}, /* PA8 */
    {GPIOA, TMR1, DMA1,  GPIO_PINS_9, TMR_SELECT_CHANNEL_2, DMA1_CHANNEL3, DMA1_FDT3_FLAG}, /* PA9 */
    {GPIOA, TMR1, DMA1, GPIO_PINS_10, TMR_SELECT_CHANNEL_3, DMA1_CHANNEL6, DMA1_FDT6_FLAG}, /* PA10 */
    {GPIOA, TMR1, DMA1, GPIO_PINS_11, TMR_SELECT_CHANNEL_4, DMA1_CHANNEL4, DMA1_FDT4_FLAG}, /* PA11 */
    {GPIOA, NULL, NULL, GPIO_PINS_12, TMR_SELECT_CHANNEL_1, NULL}, /* PA12 */
    {GPIOA, NULL, NULL, GPIO_PINS_13, TMR_SELECT_CHANNEL_1, NULL}, /* PA13 */
    {GPIOA, NULL, NULL, GPIO_PINS_14, TMR_SELECT_CHANNEL_1, NULL}, /* PA14 */
    {GPIOA, TMR2, DMA1, GPIO_PINS_15, TMR_SELECT_CHANNEL_1, DMA1_CHANNEL5, DMA1_FDT5_FLAG}, /* PA15  TMR2_GMUX_11 */

    {GPIOB, TMR3, DMA1,  GPIO_PINS_0, TMR_SELECT_CHANNEL_3, DMA1_CHANNEL2, DMA1_FDT2_FLAG}, /* PB0 */
    {GPIOB, TMR3, DMA1,  GPIO_PINS_1, TMR_SELECT_CHANNEL_4, DMA1_CHANNEL3, DMA1_FDT3_FLAG}, /* PB1 */
    {GPIOB, NULL, NULL,  GPIO_PINS_2, TMR_SELECT_CHANNEL_1, NULL}, /* PB2 */
    {GPIOB, TMR2, DMA1,  GPIO_PINS_3, TMR_SELECT_CHANNEL_2, DMA1_CHANNEL7, DMA1_FDT7_FLAG}, /* PB3  TMR2_GMUX_11 */
    {GPIOB, TMR3, DMA1,  GPIO_PINS_4, TMR_SELECT_CHANNEL_1, DMA1_CHANNEL6, DMA1_FDT6_FLAG}, /* PB4 */
    {GPIOB, NULL, NULL,  GPIO_PINS_5, TMR_SELECT_CHANNEL_2, NULL}, /* PB5 DMA YOK */
    {GPIOB, TMR4, DMA1,  GPIO_PINS_6, TMR_SELECT_CHANNEL_1, DMA1_CHANNEL1, DMA1_FDT1_FLAG}, /* PB6 */
    {GPIOB, TMR4, DMA1,  GPIO_PINS_7, TMR_SELECT_CHANNEL_2, DMA1_CHANNEL4, DMA1_FDT4_FLAG}, /* PB7 */
    {GPIOB, TMR4, DMA1,  GPIO_PINS_8, TMR_SELECT_CHANNEL_3, DMA1_CHANNEL5, DMA1_FDT5_FLAG}, /* PB8 */
    {GPIOB, TMR4, DMA1,  GPIO_PINS_9, TMR_SELECT_CHANNEL_4, DMA1_CHANNEL7, DMA1_FDT7_FLAG}, /* PB9 */
    {GPIOB, TMR2, DMA1, GPIO_PINS_10, TMR_SELECT_CHANNEL_3, DMA1_CHANNEL1, DMA1_FDT1_FLAG}, /* PB10  TMR2_GMUX_11*/
    {GPIOB, TMR2, DMA1, GPIO_PINS_11, TMR_SELECT_CHANNEL_4, DMA1_CHANNEL7, DMA1_FDT7_FLAG}, /* PB11  TMR2_GMUX_11*/
    {GPIOB, NULL, NULL, GPIO_PINS_12, TMR_SELECT_CHANNEL_1, NULL}, /* PB12 */
    {GPIOB, NULL, NULL, GPIO_PINS_13, TMR_SELECT_CHANNEL_1, NULL}, /* PB13 */
};


DShot::DShot(Mode mode) : _mode(mode), _timer(NULL), _channel(TMR_SELECT_CHANNEL_2), _dmaChannel(NULL) {
    
}


void DShot::begin(uint8_t pin) {
    _pin = pin;
    const PinInfo_TypeDMA &p = PIN_MAPS[pin];

    _timer      = p.TIMx;
    _channel    = p.TimerChannel;
    _dmaChannel = p.DMA_Channel;
    _dmaFdt     = p.fdtFlag;

    if (_timer == NULL || _dmaChannel == NULL) {
        Serial.println("Hata: Bu pin DShot için desteklenmiyor!");
        return;
    }

    // === Clock Enable ===
    if (_timer == TMR1) crm_periph_clock_enable(CRM_TMR1_PERIPH_CLOCK, TRUE);
    else if (_timer == TMR2) crm_periph_clock_enable(CRM_TMR2_PERIPH_CLOCK, TRUE);
    else if (_timer == TMR3) crm_periph_clock_enable(CRM_TMR3_PERIPH_CLOCK, TRUE);
    else if (_timer == TMR4) crm_periph_clock_enable(CRM_TMR4_PERIPH_CLOCK, TRUE);
    else if (_timer == TMR5) crm_periph_clock_enable(CRM_TMR5_PERIPH_CLOCK, TRUE);

    crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);

    if (pin == PB4 || pin == PA15 || pin == PB3) {
        gpio_pin_remap_config(SWJTAG_MUX_010, TRUE); 
    }

    if(pin == PB4){
        gpio_pin_remap_config(TMR3_MUX_10, TRUE);
    }

    if(pin == PA15 || pin == PB3 || pin == PB10 || pin == PB11){
         gpio_pin_remap_config(TMR2_MUX_11, TRUE);
    }


    if (p.GPIOx == GPIOA)
        crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
    else
        crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);

    if (p.DMAx == DMA1)
        crm_periph_clock_enable(CRM_DMA1_PERIPH_CLOCK, TRUE);
    else
        crm_periph_clock_enable(CRM_DMA2_PERIPH_CLOCK, TRUE);


    // === GPIO ===
    gpio_init_type gpio_init_struct;
    gpio_default_para_init(&gpio_init_struct);
    gpio_init_struct.gpio_pins = p.GPIO_Pin_x;
    gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
    gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
    gpio_init(p.GPIOx, &gpio_init_struct);



    // === Timing ===
    crm_clocks_freq_type clocks;
    crm_clocks_freq_get(&clocks);
    uint32_t timer_clk = clocks.apb1_freq * 2;

    period = timer_clk / DSHOT600_HZ;
    duty_1 = (period * 78) / 100;
    duty_0 = (period * 35) / 100;

    // === Timer ===
    tmr_base_init(_timer, period - 1, 0);
    tmr_cnt_dir_set(_timer, TMR_COUNT_UP);
    tmr_clock_source_div_set(_timer, TMR_CLOCK_DIV1);

    tmr_output_config_type oc;
    tmr_output_default_para_init(&oc);
    oc.oc_mode = TMR_OUTPUT_CONTROL_PWM_MODE_A;
    oc.oc_output_state = TRUE;
    oc.oc_polarity = TMR_OUTPUT_ACTIVE_HIGH;

    tmr_output_channel_config(_timer, _channel, &oc);
    tmr_output_channel_buffer_enable(_timer, _channel, TRUE);


    // === DMA ===
    dma_init_type dma_init_struct;
    dma_default_para_init(&dma_init_struct);

    // *** EN ÖNEMLİ DÜZELTME ***
uint8_t ch_index = 0;
tmr_dma_request_type dma_req = TMR_C1_DMA_REQUEST;

    switch (_channel) {
        case TMR_SELECT_CHANNEL_1C:
        case TMR_SELECT_CHANNEL_2C:
        case TMR_SELECT_CHANNEL_3C:
        case TMR_SELECT_CHANNEL_1: ch_index = 0; dma_req = TMR_C1_DMA_REQUEST; break;
        case TMR_SELECT_CHANNEL_2: ch_index = 1; dma_req = TMR_C2_DMA_REQUEST; break;
        case TMR_SELECT_CHANNEL_3: ch_index = 2; dma_req = TMR_C3_DMA_REQUEST; break;
        case TMR_SELECT_CHANNEL_4: ch_index = 3; dma_req = TMR_C4_DMA_REQUEST; break;
    }

    dma_init_struct.peripheral_base_addr = (uint32_t)&(_timer->c1dt) + (ch_index * 4);

    dma_init_struct.memory_base_addr      = (uint32_t)dshot_buffer;
    dma_init_struct.buffer_size           = DSHOT_BUFFER_SIZE;
    dma_init_struct.direction             = DMA_DIR_MEMORY_TO_PERIPHERAL;
    dma_init_struct.memory_inc_enable     = TRUE;
    dma_init_struct.peripheral_inc_enable = FALSE;
    dma_init_struct.memory_data_width     = DMA_MEMORY_DATA_WIDTH_HALFWORD;
    dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_HALFWORD;
    dma_init_struct.priority              = DMA_PRIORITY_HIGH;

    dma_init(_dmaChannel, &dma_init_struct);
    dma_channel_enable(_dmaChannel, FALSE);
    tmr_dma_request_enable(_timer, dma_req, TRUE);

    tmr_output_enable(_timer, TRUE);
    tmr_counter_enable(_timer, TRUE);

    delay_ms(10);

    // delay(1000);

    // for(int i = 0; i < 2000; i++)
    // {
    //     write(0);
    //     delay_ms(1);
    // }

    // delay(1000);

}



uint16_t DShot::dshot_prepare_packet(uint16_t value) {
    uint16_t packet = value << 1;
    uint16_t csum = (packet ^ (packet >> 4) ^ (packet >> 8)) & 0x0F;
    return (packet << 4) | csum;
}

void DShot::write(uint16_t throttle) {

    //throttle = constrain(throttle, 0, 2047);
    //throttle = map(throttle, 1000, 2000, 48, 2047);
    
    if (throttle <= 1000) {
        throttle = 0;
    } else {
        // PWM adım farkını 2 ile çarparak doğrudan DShot aralığına oturtur
        throttle = ((throttle - 1000) * 2) + 46; 
    }


    uint16_t packet = dshot_prepare_packet(throttle);

    for (int i = 0; i < 16; i++) {
        dshot_buffer[i] = (packet & (1U << (15 - i))) ? duty_1 : duty_0;
    }
    for (int i = 16; i < 24; i++) dshot_buffer[i] = 0;


    tmr_counter_value_set(_timer, 0);

    if (dma_flag_get(_dmaFdt) != RESET) dma_flag_clear(_dmaFdt);
    
    // DMA restart
    dma_channel_enable(_dmaChannel, FALSE);
    dma_data_number_set(_dmaChannel, DSHOT_BUFFER_SIZE);
    dma_channel_enable(_dmaChannel, TRUE);
    delay_ms(1);
}


// #include "DShot.h"
// #include <Arduino.h>

// DShot esc1( DShot::DSHOT600_HZ );   // veya DSHOT300
// DShot esc2( DShot::DSHOT600_HZ );
// DShot esc3( DShot::DSHOT600_HZ );
// DShot esc4( DShot::DSHOT600_HZ );

// void setup() {
//   Serial.begin(115200);
//   esc1.begin(PA0);
//   esc2.begin(PA1);
//   esc3.begin(PA2);
//   esc4.begin(PA3);

//   // ESC ARM
//   for(int i = 0; i < 1000; i++){
//       esc1.write(0);
//       esc2.write(0);
//       esc3.write(0);
//       esc4.write(0);
//   }
//   delay(10);

// }

// void loop() {
//    esc1.write(100);
//    esc2.write(100);
//    esc3.write(100);
//    esc4.write(100);    
// }