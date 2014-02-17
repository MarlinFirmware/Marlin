#ifndef usb_common_h__
#define usb_common_h__

#include <stdint.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#ifdef __cplusplus
extern "C"{
#endif

#define MAX_ENDPOINT            6

#define LSB(n) (n & 255)
#define MSB(n) ((n >> 8) & 255)


// constants corresponding to the various serial parameters
#define USB_SERIAL_DTR                  0x01
#define USB_SERIAL_RTS                  0x02
#define USB_SERIAL_1_STOP               0
#define USB_SERIAL_1_5_STOP             1
#define USB_SERIAL_2_STOP               2
#define USB_SERIAL_PARITY_NONE          0
#define USB_SERIAL_PARITY_ODD           1
#define USB_SERIAL_PARITY_EVEN          2
#define USB_SERIAL_PARITY_MARK          3
#define USB_SERIAL_PARITY_SPACE         4
#define USB_SERIAL_DCD                  0x01
#define USB_SERIAL_DSR                  0x02
#define USB_SERIAL_BREAK                0x04
#define USB_SERIAL_RI                   0x08
#define USB_SERIAL_FRAME_ERR            0x10
#define USB_SERIAL_PARITY_ERR           0x20
#define USB_SERIAL_OVERRUN_ERR          0x40

#define EP_TYPE_CONTROL                 0x00
#define EP_TYPE_BULK_IN                 0x81
#define EP_TYPE_BULK_OUT                0x80
#define EP_TYPE_INTERRUPT_IN            0xC1
#define EP_TYPE_INTERRUPT_OUT           0xC0
#define EP_TYPE_ISOCHRONOUS_IN          0x41
#define EP_TYPE_ISOCHRONOUS_OUT         0x40
#define EP_SINGLE_BUFFER                0x02
#define EP_DOUBLE_BUFFER                0x06
#define EP_SIZE(s)      ((s) == 64 ? 0x30 :     \
                        ((s) == 32 ? 0x20 :     \
                        ((s) == 16 ? 0x10 :     \
                                     0x00)))

#if defined(__AVR_AT90USB162__)
#define HW_CONFIG() 
#define PLL_CONFIG() (PLLCSR = ((1<<PLLE)|(1<<PLLP0)))
#define USB_CONFIG() (USBCON = (1<<USBE))
#define USB_FREEZE() (USBCON = ((1<<USBE)|(1<<FRZCLK)))
#elif defined(__AVR_ATmega32U4__)
#define HW_CONFIG() (UHWCON = 0x01)
#define PLL_CONFIG() (PLLCSR = 0x12)
#define USB_CONFIG() (USBCON = ((1<<USBE)|(1<<OTGPADE)))
#define USB_FREEZE() (USBCON = ((1<<USBE)|(1<<FRZCLK)))
#elif defined(__AVR_AT90USB646__)
#define HW_CONFIG() (UHWCON = 0x81)
#define PLL_CONFIG() (PLLCSR = 0x1A)
#define USB_CONFIG() (USBCON = ((1<<USBE)|(1<<OTGPADE)))
#define USB_FREEZE() (USBCON = ((1<<USBE)|(1<<FRZCLK)))
#elif defined(__AVR_AT90USB1286__)
#define HW_CONFIG() (UHWCON = 0x81)
#define PLL_CONFIG() (PLLCSR = 0x16)
#define USB_CONFIG() (USBCON = ((1<<USBE)|(1<<OTGPADE)))
#define USB_FREEZE() (USBCON = ((1<<USBE)|(1<<FRZCLK)))
#endif

// standard control endpoint request types
#define GET_STATUS                      0
#define CLEAR_FEATURE                   1
#define SET_FEATURE                     3
#define SET_ADDRESS                     5
#define GET_DESCRIPTOR                  6
#define GET_CONFIGURATION               8
#define SET_CONFIGURATION               9
#define GET_INTERFACE                   10
#define SET_INTERFACE                   11
// CDC (communication class device)
#define CDC_SET_LINE_CODING             0x20
#define CDC_GET_LINE_CODING             0x21
#define CDC_SET_CONTROL_LINE_STATE      0x22
#define CDC_SEND_BREAK                  0x23
// HID (human interface device)
#define HID_GET_REPORT                  1
#define HID_GET_IDLE                    2
#define HID_GET_PROTOCOL                3
#define HID_SET_REPORT                  9
#define HID_SET_IDLE                    10
#define HID_SET_PROTOCOL                11
// Mass Storage
#define MS_BULK_ONLY_RESET              0xFF
#define MS_GET_MAX_LUN                  0xFE  /* stall = 0 */


#define pgm_read_byte_postinc(val, addr) \
        asm ("lpm  %0, Z+\n" : "=r" (val), "+z" (addr) : )
#define pgm_read_word_postinc(val, addr) \
        asm ("lpm  %A0, Z+\n\tlpm  %B0, Z+\n" : "=r" (val), "+z" (addr) : )

#define read_word_lsbfirst(val, reg) \
        asm volatile( \
                "lds  %A0, %1\n\tlds  %B0, %1\n" \
                : "=r" (val) : "M" ((int)(&reg)) )
#define read_word_msbfirst(val, reg) \
        asm volatile( \
                "lds  %B0, %1\n\tlds  %A0, %1\n" \
                : "=r" (val) : "M" ((int)(&reg)) )
#define read_dword_lsbfirst(val, reg) \
        asm volatile( \
                "lds  %A0, %1\n\tlds  %B0, %1\n\t" \
                "lds  %C0, %1\n\tlds  %D0, %1\n" \
                : "=r" (val) : "M" ((int)(&reg)) )
#define read_dword_msbfirst(val, reg) \
        asm volatile( \
                "lds  %D0, %1\n\tlds  %C0, %1\n\t" \
                "lds  %B0, %1\n\tlds  %A0, %1\n" \
                : "=r" (val) : "M" ((int)(&reg)) )

#define write_word_lsbfirst(val, reg) \
        asm volatile( \
                "sts  %1, %A0\n\tsts  %1, %B0\n" \
                : : "r" (val) , "M" ((int)(&reg)) )
#define write_word_msbfirst(val, reg) \
        asm volatile( \
                "sts  %1, %B0\n\tsts  %1, %A0\n" \
                : : "r" (val) , "M" ((int)(&reg)) )
#define write_dword_lsbfirst(val, reg) \
        asm volatile( \
                "sts  %1, %A0\n\tsts  %1, %B0\n\t" \
                "sts  %1, %C0\n\tsts  %1, %D0\n" \
                : : "r" (val) , "M" ((int)(&reg)) )
#define write_dword_msbfirst(val, reg) \
        asm volatile( \
                "sts  %1, %D0\n\tsts  %1, %C0\n\t" \
                "sts  %1, %B0\n\tsts  %1, %A0\n" \
                : : "r" (val) , "M" ((int)(&reg)) )

#define USBSTATE __attribute__ ((section (".noinit")))

extern void _reboot_Teensyduino_(void) __attribute__((noreturn));
extern void _restart_Teensyduino_(void) __attribute__((noreturn));

#ifdef __cplusplus
} // extern "C"
#endif
#endif

