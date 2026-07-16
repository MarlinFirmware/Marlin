#include <cstddef>
#include "Arduino.h"
#include "servo.h"




tmr_output_config_type tmr_oc_init_structure;
static bool timer_configured = false;

Servo::Servo() {
    attached = false;
    pin = 0;
    tmr = TMR3;
    min_pulse = MIN_PULSE_WIDTH;
    max_pulse = MAX_PULSE_WIDTH;
    current_angle = 0;
}

uint16_t Servo::angle_to_pulse(uint16_t angle) {
    uint32_t pulse_width = min_pulse + (angle * (max_pulse - min_pulse)) / 180;
    return (uint16_t)pulse_width;
}

void Servo::configure_timer(void) {
    if (timer_configured) return;
    
    // TMR clock enable
   if(PIN_MAP[pin].TIMx == TMR1){
        crm_periph_clock_enable(CRM_TMR1_PERIPH_CLOCK, TRUE);}
   if(PIN_MAP[pin].TIMx == TMR2){
        crm_periph_clock_enable(CRM_TMR2_PERIPH_CLOCK, TRUE);}
   if(PIN_MAP[pin].TIMx == TMR3){
        crm_periph_clock_enable(CRM_TMR3_PERIPH_CLOCK, TRUE);}
   if(PIN_MAP[pin].TIMx == TMR4){
        crm_periph_clock_enable(CRM_TMR4_PERIPH_CLOCK, TRUE);}
   if(PIN_MAP[pin].TIMx == TMR9){
        crm_periph_clock_enable(CRM_TMR9_PERIPH_CLOCK, TRUE);}
   if(PIN_MAP[pin].TIMx == TMR10){
        crm_periph_clock_enable(CRM_TMR10_PERIPH_CLOCK, TRUE);}
    
    crm_clocks_freq_type clocks;
    crm_clocks_freq_get(&clocks);
    
    uint32_t timer_clock = clocks.apb1_freq * 2;  // APB1 prescaler >1 ise 2× multiplier uygulanır
    uint16_t prescaler_value = (timer_clock / 1000000UL) - 1;  // 1 MHz tick hedefi
    
    uint16_t period = 19999;  // 20 ms @ 1 MHz

    // Timer frekansını 24MHz'den 1MHz'e ayarla (24 prescaler)
    // 24MHz / (23+1) = 1MHz (her tick 1µs)
    // uint16_t prescaler_value = 119;
    // 50Hz için period hesabı: 1MHz / 50Hz = 20000 ticks

    
    // Timer base konfigürasyonu
    tmr_base_init(tmr, period, prescaler_value);
    tmr_cnt_dir_set(tmr, TMR_COUNT_UP);
    tmr_clock_source_div_set(tmr, TMR_CLOCK_DIV1);
    
    timer_configured = true;
}

void Servo::configure_pin(void) {
    gpio_init_type gpio_init_struct;
    gpio_default_para_init(&gpio_init_struct);
    
        
    // GPIOA clock enable
    if(PIN_MAP[pin].GPIOx == GPIOA){
        crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
    }
    if(PIN_MAP[pin].GPIOx == GPIOB){
        crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
    }


    crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);

    if (pin == PB4 || pin == PA15 || pin == PB3) {
        gpio_pin_remap_config(SWJTAG_MUX_010, TRUE);
    }

    if (pin == PB4 || pin == PB5) {
        gpio_pin_remap_config(TMR3_GMUX_0010, TRUE);
    }

    if (pin == PB10 || pin == PB11 || pin == PA15 || pin == PB3) {
        gpio_pin_remap_config(TMR2_GMUX_11, TRUE);
    }    
    
    // GPIO yapılandırması
    gpio_init_struct.gpio_pins = PIN_MAP[pin].GPIO_Pin_x;
    gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
    gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
    gpio_init_struct.gpio_mode = GPIO_MODE_MUX;  // Alternatif fonksiyon modu
    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
    gpio_init(GPIOA, &gpio_init_struct);
        
}

bool Servo::attach(uint32_t servo_pin, uint16_t min, uint16_t max) {

    TIM_TypeDef* tdev = PIN_MAP[servo_pin].TIMx;

    if (tdev == NULL)
    {
        // don't reset any fields or ASSERT(0), to keep driving any
        // previously attach()ed servo.
        return false;
    }
    
    pin = servo_pin;
    min_pulse = (min < max) ? min : MIN_PULSE_WIDTH;
    max_pulse = (max > min) ? max : MAX_PULSE_WIDTH;
    

    // Şimdilik sadece PA6 (TMR3 CH1) destekleniyor
    // if (pin != GPIO_PINS_6) {
    //     return false;
    // }

    tmr = PIN_MAP[servo_pin].TIMx;

    if(PIN_MAP[servo_pin].TimerChannel == 1){
        channel = TMR_SELECT_CHANNEL_1;
    }
    if(PIN_MAP[servo_pin].TimerChannel == 2){
        channel = TMR_SELECT_CHANNEL_2;
    }
    if(PIN_MAP[servo_pin].TimerChannel == 3){
        channel = TMR_SELECT_CHANNEL_3;
    }
    if(PIN_MAP[servo_pin].TimerChannel == 4){
        channel = TMR_SELECT_CHANNEL_4;
    }

    //channel = TMR_SELECT_CHANNEL_1;
    
    if (attached) {
        detach();
    }

    configure_timer();
    configure_pin();
    
    // PWM channel configuration
    tmr_output_default_para_init(&tmr_oc_init_structure);
    tmr_oc_init_structure.oc_mode = TMR_OUTPUT_CONTROL_PWM_MODE_A;
    tmr_oc_init_structure.oc_idle_state = FALSE;
    tmr_oc_init_structure.oc_polarity = TMR_OUTPUT_ACTIVE_HIGH;
    tmr_oc_init_structure.oc_output_state = TRUE;
    
    tmr_output_channel_config(tmr, channel, &tmr_oc_init_structure);
    tmr_channel_value_set(tmr, channel, 1000);
    
    // Buffer enable
    tmr_output_channel_buffer_enable(tmr, channel, TRUE);
    tmr_period_buffer_enable(tmr, TRUE);
    
    // Timer counter'ı aktif et
    tmr_counter_enable(tmr, TRUE);
    delay(300);
    // Opsiyonel: Arming sırası burada yap (çoğu ESC için yeterli)
    tmr_channel_value_set(tmr, channel, 1000);  // min
    delay(500);  // 2 sn bekle
    tmr_channel_value_set(tmr, channel, DEFAULT_PULSE_WIDTH);  // nötr
    delay(300);
    attached = true;
    return true;
}

void Servo::detach() {
    if (!attached) return;
    
    // PWM çıkışını devre dışı bırak
    tmr_output_channel_buffer_enable(tmr, channel, FALSE);
    tmr_channel_value_set(tmr, channel, 0);
    
    // Timer'ı durdur
    //tmr_counter_enable(TMR3, FALSE);
    
    // Pini input yap
    gpio_init_type gpio_init_struct;
    gpio_default_para_init(&gpio_init_struct);
    gpio_init_struct.gpio_pins = PIN_MAP[pin].GPIO_Pin_x;
    gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
    gpio_init_struct.gpio_pull = GPIO_PULL_DOWN;
    gpio_init(GPIOA, &gpio_init_struct);
    
    attached = false;
    pin = 0;
}

void Servo::write(uint16_t angle) {
    if (!attached) return;
    

    angle = map(angle, 0, 180, 1000, 2000);
    writeMicroseconds(angle);
}

void Servo::writeMicroseconds(uint16_t pulse_width) {
    if (!attached) return;
    
    // // Pulse width'i limitler içinde tut
    // if (pulse_width < min_pulse) pulse_width = min_pulse;
    // if (pulse_width > max_pulse) pulse_width = max_pulse;

    pulse_width = constrain(pulse_width, 1000, 2000);

    
    // 1MHz timer clock için pulse width direkt olarak timer değeri
    // Not: Period 20000 (20ms), pulse width 544-2400 arası olmalı
    tmr_channel_value_set(tmr, channel, pulse_width);
    
    // Hemen güncellemek için
    //tmr_counter_enable(TMR3, FALSE);
    //tmr_counter_enable(TMR3, TRUE);
}

uint16_t Servo::read() {
    return current_angle;
}

bool Servo::attached_state() {
    return attached;
}