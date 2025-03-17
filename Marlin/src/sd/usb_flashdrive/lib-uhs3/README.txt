==== USB HOST SHIELD 3.0 LIBRARY ====

The lib-uhs3/ folder contains a subset of the files from the USB Host Shield
3.0 library:

  https://github.com/felis/UHS30

==== LICENSE SUMMARY ====

Source Path:              Repository:                    License:
------------              -----------                    --------
usb_flashdrive/lib        github.com/felis/UHS30         GPLv2 or later

==== MARLIN INTEGRATION WORK ====

All additional work done to integrate USB into Marlin was performed by
LulzBot and is licensed under the GPLv3.

This version of UHS3 has been modified for better compatibility with Marlin.
The upstream version of UHS 3.0 runs a frame timer interrupt every 1 ms to
handle device polling. This timer interrupt interferes with Marlin's stepper
IRQ, so the flag USB_HOST_MANUAL_POLL has been added to move the polling to
the idle task. Additional logic was added to disable and enable the frame
IRQ.

SKIP_PAGE3F and USB_NO_TEST_UNIT_READY were added to work around bugs with
certain devices.
