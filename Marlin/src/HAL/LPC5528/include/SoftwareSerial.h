/**
 * Forwarding header so that <SoftwareSerial.h> resolves to the LPC5528 HAL
 * implementation. The include/ subdirectory is already on the include path
 * (-IMarlin/src/HAL/LPC5528/include in lpc5528.ini), so placing this file here
 * makes __has_include(<SoftwareSerial.h>) return true for the TMCStepper library
 * without polluting the path with other HAL headers (e.g. usb_host.h).
 */
#pragma once
#include "../SoftwareSerial.h"
