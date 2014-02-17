#if defined(USB_SERIAL)
#include "../usb_serial/usb_api.h"
#elif defined(USB_HID)
#include "../usb_hid/usb_api.h"
#elif defined(USB_SERIAL_HID)
#include "../usb_serial_hid/usb_api.h"
#elif defined(USB_DISK) || defined(USB_DISK_SDFLASH)
#include "../usb_disk/usb_api.h"
#elif defined(USB_MIDI)
#include "../usb_midi/usb_api.h"
#elif defined(USB_RAWHID)
#include "../usb_rawhid/usb_api.h"
#elif defined(USB_FLIGHTSIM)
#include "../usb_flightsim/usb_api.h"
#endif
