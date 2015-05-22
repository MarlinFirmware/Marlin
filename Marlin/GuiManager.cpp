#include "GuiManager.h"

#include "Marlin.h"
#include "cardreader.h"
#include "ConfigurationStore.h"
#include "temperature.h"
#include "language.h"

#include "Screen.h"
#include "GuiImpl_witbox_2.h"

/////////////////////////////////////////////////////////////////////////
//                          Marlin interface                           //
/////////////////////////////////////////////////////////////////////////
void draw_status_screen(){};
void draw_wizard_change_filament(){};

// Configuration settings
int plaPreheatHotendTemp;
int plaPreheatHPBTemp;
int plaPreheatFanSpeed;
int absPreheatHotendTemp;
int absPreheatHPBTemp;
int absPreheatFanSpeed;


screen::Screen * active_view;

/*******************************************************************************
**   Variables
*******************************************************************************/

// Extern variables
extern bool stop_buffer;
extern int stop_buffer_code;
uint8_t prev_encoder_position;


// Display related variables
uint8_t    display_refresh_mode;
uint32_t   display_time_refresh = 0;

uint32_t   display_timeout;
bool       display_timeout_blocked;

view_t display_view_next;
view_t display_view;

uint32_t refresh_interval;

// Encoder related variables
uint8_t encoder_input;
uint8_t encoder_input_last;
bool    encoder_input_blocked;
bool    encoder_input_updated;

int16_t encoder_position;
uint8_t encoder_left_triggered;
uint8_t encoder_right_triggered;


// Button related variables
uint8_t button_input;
uint8_t button_input_last;
bool    button_input_blocked;
bool    button_input_updated;

bool    button_clicked_triggered;

// Beeper related variables
bool beeper_level = false;
uint32_t beeper_duration = 0;
uint8_t beep_count, frequency_ratio;

// ISR related variables
uint16_t lcd_timer = 0;

// Status screen drawer variables
uint8_t lcd_status_message_level;
char lcd_status_message[LCD_WIDTH+1] = WELCOME_MSG;


// View drawers variables
uint8_t display_view_menu_offset = 0;
uint8_t display_view_wizard_page = 0;

#    if (SDCARDDETECT > 0)
bool lcd_oldcardstatus;
#    endif // (SDCARDDETECT > 0)

// TODO : Review structure of this file to include it above.
#ifdef DOGLCD
#include "dogm_lcd_implementation.h"
#else
#include "ultralcd_implementation_hitachi_HD44780.h"
#endif



/*******************************************************************************
**   Function definitions
*******************************************************************************/

//
// General API definitions
// 

void lcd_init()
{
    // Low level init libraries for lcd & encoder
    lcd_implementation_init();

    pinMode(BTN_EN1,INPUT);
    pinMode(BTN_EN2,INPUT);
    WRITE(BTN_EN1,HIGH);
    WRITE(BTN_EN2,HIGH);

    pinMode(BTN_ENC,INPUT);
    WRITE(BTN_ENC,HIGH);

    // Init for SD card library
#  if (defined (SDSUPPORT) && defined(SDCARDDETECT) && (SDCARDDETECT > 0))
    pinMode(SDCARDDETECT,INPUT);
    WRITE(SDCARDDETECT, HIGH);
    lcd_oldcardstatus = IS_SD_INSERTED;
#  endif // (defined (SDSUPPORT) && defined(SDCARDDETECT) && (SDCARDDETECT > 0))

    // Init Timer 5 and set the OVF interrupt (triggered every 125 us)
    TCCR5A = 0x03;
    TCCR5B = 0x19;

    OCR5AH = 0x07;
    OCR5AL = 0xD0;

    lcd_enable_interrupt();

    display_time_refresh = millis();
    display_timeout = millis();
    refresh_interval = millis();
    lcd_enable_display_timeout();

    lcd_enable_encoder();
    lcd_get_encoder_updated();
    encoder_position = 0;

    lcd_enable_button();
    lcd_get_button_updated();
    lcd_get_button_clicked();

    active_view = screen::GuiBuild();
    active_view->draw();
    
    SERIAL_ECHOLN("LCD initialized!");
}


// Interrupt-driven functions
static void lcd_update_button()
{
    static uint16_t button_pressed_count = 0;

    // Read the hardware
    button_input = lcd_implementation_update_buttons();

    // Process button click/keep-press events
    bool button_clicked = ((button_input & EN_C) && (~(button_input_last) & EN_C));
    bool button_pressed = ((button_input & EN_C) && (button_input_last & EN_C));

    if (button_pressed == true) {
        button_pressed_count++;        
    } else { 
        button_pressed_count = 0;
    }

    // Beeper feedback
    if ((button_clicked == true) || (button_pressed_count > 50)) 
    {
        lcd_implementation_quick_feedback();
	//if (button_pressed_count == 200)
	//    lcd_emergency_stop();
    }

    // Update button trigger
    if ((button_clicked == true) && (button_input_blocked == false)) {
        button_clicked_triggered = true;
    }

    if ((button_input != button_input_last) && (button_input_blocked == false)) {
        button_input_updated = true;
    }

    button_input_last = button_input;
}

static void lcd_update_encoder()
{
    // Read the input hardware
    encoder_input = lcd_implementation_update_buttons();

    // Process rotatory encoder events if they are not disabled 
    if (encoder_input != encoder_input_last && encoder_input_blocked == false) {
        prev_encoder_position = encoder_position;
        switch (encoder_input & (EN_A | EN_B)) {
        case encrot0:
            if ( (encoder_input_last & (EN_A | EN_B)) == encrot3 )
            {
                encoder_position++;
            }
            else if ( (encoder_input_last & (EN_A | EN_B)) == encrot1 )
            {
                encoder_position--;
            }
            break;
        
        case encrot1:
            if ( (encoder_input_last & (EN_A | EN_B)) == encrot0 )
            {
                encoder_position++;
            }
            else if ( (encoder_input_last & (EN_A | EN_B)) == encrot2 )
            {
                encoder_position--;
            }
            break;
        
        case encrot2:
            if ( (encoder_input_last & (EN_A | EN_B)) == encrot1 )
            {
                encoder_position++;
            }
            else if ( (encoder_input_last & (EN_A | EN_B)) == encrot3 )
            {
                encoder_position--;
            }
            break;
        
        case encrot3:
            if ( (encoder_input_last & (EN_A | EN_B)) == encrot2 )
            {
                encoder_position++;
            }
            else if ( (encoder_input_last & (EN_A | EN_B)) == encrot0 )
            {
                encoder_position--;
            }
            break;
        }

        // Check if the menu item must be change
        if (encoder_position/ENCODER_STEPS_PER_MENU_ITEM >= 1)
        {
            encoder_right_triggered++;
            encoder_position = 0;
        }
        else if (encoder_position/ENCODER_STEPS_PER_MENU_ITEM<=-1)
        {
            encoder_left_triggered++;
            encoder_position = 0;
        }

        encoder_input_updated = true;
    }

    // Update the phases
    encoder_input_last = encoder_input;
}

void lcd_update(bool force)
{
    // Check if the SD card has been inserted/removed.
    if ((lcd_oldcardstatus != IS_SD_INSERTED)) {
        if(lcd_oldcardstatus) {
            card.initsd();
        } else {
            card.release();
        }
    }

    // Manage the events triggered in ISR (Timer 5 Overflow)
    for (int8_t times = lcd_get_encoder_right(); times > 0; times--)
    {
        active_view->right();
    }

    for (int8_t times = lcd_get_encoder_left(); times > 0; times--)
    {
        active_view->left();
    }

    if (lcd_get_button_clicked())
    {
        active_view = &active_view->press(active_view);
    }

    // Refresh the content of the display
    active_view->draw();
}

void lcd_set_menu(view_t menu)
{
    display_view_next = menu;
    display_view_menu_offset = 0;
    
    encoder_position = 0;

    lcd_clear_triggered_flags();
}
void lcd_set_picture(view_t picture)
{
    display_view_next = picture;

    lcd_clear_triggered_flags();
}
void lcd_set_wizard(view_t wizard)
{
    display_view_next = wizard;
    lcd_wizard_set_page(0);

    lcd_clear_triggered_flags();
}

// Get and clear trigger functions
bool lcd_get_button_updated()
{
    bool status = button_input_updated;
    button_input_updated = false;
    return status;
}

bool lcd_get_encoder_updated()
{
    bool status = encoder_input_updated;
    encoder_input_updated = false;
    return status;
}
bool lcd_get_button_clicked()
{
    bool status = button_clicked_triggered;
    button_clicked_triggered = false;
    return status;
}
uint8_t lcd_get_encoder_right()
{
    uint8_t times = encoder_right_triggered;
    encoder_right_triggered = 0;
    return times;
}
uint8_t lcd_get_encoder_left()
{
    uint8_t times = encoder_left_triggered;
    encoder_left_triggered = 0;
    return times;
}
void lcd_clear_triggered_flags() {
    button_input_updated = false;
    encoder_input_updated = false;
    button_clicked_triggered = false;
    encoder_right_triggered = 0;
    encoder_left_triggered = 0;
}


void lcd_disable_buzzer()
{
    beep_count = 0;
    beeper_duration = 0;
    beeper_level = false;
    WRITE(BEEPER, beeper_level);
}

// Enable/disable function
void lcd_enable_button() {
    button_input = lcd_implementation_update_buttons();
    button_input_last = button_input;
    button_input_blocked = false;
}
void lcd_disable_button() {
    button_input_blocked = true;
}

void lcd_enable_encoder()
{
    encoder_input = lcd_implementation_update_buttons();
    encoder_input_last = encoder_input;
    encoder_input_blocked = false;
}
void lcd_disable_encoder()
{
    encoder_input_blocked = true;
}

void lcd_enable_display_timeout()
{
    display_timeout_blocked = false;
}
void lcd_disable_display_timeout()
{
    display_timeout_blocked = true;
}

void lcd_enable_interrupt()
{
    TIMSK5 |= 0x01;
    lcd_enable_button();
    lcd_enable_encoder();
}

void lcd_disable_interrupt()
{
    lcd_disable_button();
    lcd_disable_encoder();
    lcd_disable_buzzer();
    TIMSK5 &= ~(0x01);
}


// Control flow functions
void lcd_wizard_set_page(uint8_t page)
{
    display_view_wizard_page = page;
    display_refresh_mode = CLEAR_AND_UPDATE_SCREEN;
}


// Temporal
void lcd_beep()
{
    lcd_implementation_quick_feedback();
}

void lcd_beep_ms(uint16_t ms)
{
    frequency_ratio = 0;
    beeper_duration = 8 * ms;
    while (beeper_duration) {
        lcd_update();
    }
}

void lcd_beep_hz_ms(uint16_t frequency, uint16_t ms)
{
    frequency_ratio = (4000 / frequency) - 1;
    beeper_duration = 8 * ms;
    while (beeper_duration) {
        lcd_update();
    }
}

void lcd_set_refresh(uint8_t mode)
{
    display_refresh_mode = mode;
}

uint8_t lcd_get_encoder_position()
{
    return encoder_position;
}

#ifdef DOGLCD
void lcd_setcontrast(uint8_t value) {
    pinMode(39, OUTPUT);   //Contraste = 4.5V
    digitalWrite(39, HIGH);
}
#endif // DOGLCD

// Alert/status messages
void lcd_setstatus(const char* message)
{
    if (lcd_status_message_level > 0)
        return;
    strncpy(lcd_status_message, message, LCD_WIDTH);
}

void lcd_setstatuspgm(const char* message)
{
    if (lcd_status_message_level > 0)
        return;
    strncpy_P(lcd_status_message, message, LCD_WIDTH);
}

void lcd_setalertstatuspgm(const char* message)
{
    lcd_setstatuspgm(message);
    lcd_status_message_level = 1;

    //display_view_next = view_status_screen;

}

void lcd_reset_alert_level()
{
    lcd_status_message_level = 0;
}

static void lcd_set_encoder_position(int8_t position)
{
    encoder_position = position;
}


/********************************/
/** Float conversion utilities **/
/********************************/
//  convert float to string with +123.4 format
char conv[8];

char *ftostr3(const float &x)
{
    return itostr3((int)x);
}

char *itostr2(const uint8_t &x)
{
    //sprintf(conv,"%5.1f",x);
    int xx=x;
    conv[0]=(xx/10)%10+'0';
    conv[1]=(xx)%10+'0';
    conv[2]=0;
    return conv;
}

//  convert float to string with +123.4 format
char *ftostr31(const float &x)
{
    int xx=x*10;
    conv[0]=(xx>=0)?'+':'-';
    xx=abs(xx);
    conv[1]=(xx/1000)%10+'0';
    conv[2]=(xx/100)%10+'0';
    conv[3]=(xx/10)%10+'0';
    conv[4]='.';
    conv[5]=(xx)%10+'0';
    conv[6]=0;
    return conv;
}

//  convert float to string with 123.4 format
char *ftostr31ns(const float &x)
{
    int xx=x*10;
    //conv[0]=(xx>=0)?'+':'-';
    xx=abs(xx);
    conv[0]=(xx/1000)%10+'0';
    conv[1]=(xx/100)%10+'0';
    conv[2]=(xx/10)%10+'0';
    conv[3]='.';
    conv[4]=(xx)%10+'0';
    conv[5]=0;
    return conv;
}

char *ftostr32(const float &x)
{
    long xx=x*100;
    if (xx >= 0)
        conv[0]=(xx/10000)%10+'0';
    else
        conv[0]='-';
    xx=abs(xx);
    conv[1]=(xx/1000)%10+'0';
    conv[2]=(xx/100)%10+'0';
    conv[3]='.';
    conv[4]=(xx/10)%10+'0';
    conv[5]=(xx)%10+'0';
    conv[6]=0;
    return conv;
}

char *itostr31(const int &xx)
{
    conv[0]=(xx>=0)?'+':'-';
    conv[1]=(xx/1000)%10+'0';
    conv[2]=(xx/100)%10+'0';
    conv[3]=(xx/10)%10+'0';
    conv[4]='.';
    conv[5]=(xx)%10+'0';
    conv[6]=0;
    return conv;
}

char *itostr3(const int &x)
{
  int xx = x;
  if (xx < 0) {
     conv[0]='-';
     xx = -xx;
  } else if (xx >= 100)
    conv[0]=(xx/100)%10+'0';
  else
    conv[0]=' ';
  if (xx >= 10)
    conv[1]=(xx/10)%10+'0';
  else
    conv[1]=' ';
  conv[2]=(xx)%10+'0';
  conv[3]=0;
  return conv;
}

char *itostr3left(const int &xx)
{
    if (xx >= 100) {
        conv[0]=(xx/100)%10+'0';
        conv[1]=(xx/10)%10+'0';
        conv[2]=(xx)%10+'0';
        conv[3]=0;
    } else if (xx >= 10) {
        conv[0]=(xx/10)%10+'0';
        conv[1]=(xx)%10+'0';
        conv[2]=0;
    } else {
        conv[0]=(xx)%10+'0';
        conv[1]=0;
    }
    return conv;
}

char *itostr4(const int &xx)
{
    if (xx >= 1000)
        conv[0]=(xx/1000)%10+'0';
    else
        conv[0]=' ';
    if (xx >= 100)
        conv[1]=(xx/100)%10+'0';
    else
        conv[1]=' ';
    if (xx >= 10)
        conv[2]=(xx/10)%10+'0';
    else
        conv[2]=' ';
    conv[3]=(xx)%10+'0';
    conv[4]=0;
    return conv;
}

//  convert float to string with 12345 format
char *ftostr5(const float &x)
{
    long xx=abs(x);
    if (xx >= 10000)
        conv[0]=(xx/10000)%10+'0';
    else
        conv[0]=' ';
    if (xx >= 1000)
        conv[1]=(xx/1000)%10+'0';
    else
        conv[1]=' ';
    if (xx >= 100)
        conv[2]=(xx/100)%10+'0';
    else
        conv[2]=' ';
    if (xx >= 10)
        conv[3]=(xx/10)%10+'0';
    else
        conv[3]=' ';
    conv[4]=(xx)%10+'0';
    conv[5]=0;
    return conv;
}

//  convert float to string with +1234.5 format
char *ftostr51(const float &x)
{
    long xx=x*10;
    conv[0]=(xx>=0)?'+':'-';
    xx=abs(xx);
    conv[1]=(xx/10000)%10+'0';
    conv[2]=(xx/1000)%10+'0';
    conv[3]=(xx/100)%10+'0';
    conv[4]=(xx/10)%10+'0';
    conv[5]='.';
    conv[6]=(xx)%10+'0';
    conv[7]=0;
    return conv;
}

//  convert float to string with +123.45 format
char *ftostr52(const float &x)
{
    long xx=x*100;
    conv[0]=(xx>=0)?'+':'-';
    xx=abs(xx);
    conv[1]=(xx/10000)%10+'0';
    conv[2]=(xx/1000)%10+'0';
    conv[3]=(xx/100)%10+'0';
    conv[4]='.';
    conv[5]=(xx/10)%10+'0';
    conv[6]=(xx)%10+'0';
    conv[7]=0;
    return conv;
}

ISR(TIMER5_OVF_vect)
{
    lcd_timer++;

#if ( defined(BEEPER) && (BEEPER > 0) )
    if (beeper_duration) {
        if (beep_count == 0) {
            beeper_level = !beeper_level;
            beep_count = frequency_ratio;
        } else {
            beep_count--;
        }
        beeper_duration--;
    } else {
        beeper_level = false;
        beep_count = 0;
    }
    WRITE(BEEPER, beeper_level);    // Tone: 4 KHz (every 125 us)
#endif // ( defined(BEEPER) && (BEEPER > 0) )

    if (lcd_timer % 4 == 0)     // Every 500 us
        lcd_update_encoder();

    if (lcd_timer % 80 == 0) {  // Every 10 ms
        lcd_update_button();
        lcd_timer = 0;
    }
}
