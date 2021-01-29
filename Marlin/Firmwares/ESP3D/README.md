# Marlin 3D Printer Firmware for Delta QQS-Pro with HISPEEDv1 Board.

![GitHub](https://img.shields.io/github/license/marlinfirmware/marlin.svg)
![GitHub contributors](https://img.shields.io/github/contributors/marlinfirmware/marlin.svg)
![GitHub Release Date](https://img.shields.io/github/release-date/marlinfirmware/marlin.svg)
[![Build Status](https://github.com/MarlinFirmware/Marlin/workflows/CI/badge.svg?branch=bugfix-2.0.x)](https://github.com/MarlinFirmware/Marlin/actions)

[**Group FaceBook**](https://www.facebook.com/groups/120961628750040)

[**My Posts on Group FB**](https://www.facebook.com/hashtag/deltafoxies/?__gid__=120961628750040)


![QQS](../../../docs/images/FLSunMarlin.png)

__Not for production use. Use with caution!__


  # For the Wifi Module ESP8266 or Mks Wifi:

![UI ESP3D with Module Wifi MKS](../../../docs/images/QQSPro_ESP3D.png)![UI ESP3D2](../../../docs/images/ESP_UI2.png)

Put the firmware (MksWifi.bin) on the scard with the firmware FLSUN (Robin_mini.bin)
 - 1) Flash original firmware + original mkswifi 
 - 2) Flash original firmware + Custum mkswifi 
 - 3) Inspect the Wifi Access Point and if you see: AP-FLSUN => Flash ok 
 - 4) Flash Marlin 
 - 5) After that, you can connect to the hotspot AP-FLSUN with password "makerbase"

to run the update of Mks_Wifi or You also can do by web page of the AP (192.168.4.1).

 ### Initial Configuration after the flash.

1. Open device web page on the AP connected device (ap:AP-FLSUN/pwd:makerbase)
2. Accept Captive portal redirect or Open a web browser and navigate to http://192.168.4.1
3. Upload 4 files in the directory "FilesToUpload' and configure the device to your choosing.
4. I recommend changing to Station mode and connecting to your home/office Wifi instead of staying in AP mode
5. You may want to change the Baud rate to accord with your printer.
6. You can change to DHCP, or at the very least setup a Static IP you are familiar with.

![UI ESP3D](../../../docs/images/ESP-UI.png)

Once reconnected to the module's web page, you can update it through the interface with:
- New files "index.html.gz", "preferences.json" or "macros"
- A new ESP firmware "MksWifi.bin or" ESP_firmware.bin "

Enjoy....🙃

More information: [ESP3Dv2.1](https://github.com/luc-github/ESP3D/wiki/Install-Instructions)

 ## Support my work

  This QQS-Pro porting project for the Marlin firmware was only possible thanks to its supporters, you can participate via
 <br/><br/>
  [![paypal.me](../../../docs/icons/paypal_50px.png)](https://www.paypal.me/Foxies40)<br/>[![Donate](https://img.shields.io/badge/Donate-Thanks-green)](https://paypal.me/Foxies40)
 <br/><br/>
 <br/><br/>
  [![BuyMeCoffee](../../../docs/icons/BMC_Black.png)](https://www.buymeacoffee.com/Foxies)<br/>[![Donate](https://img.shields.io/badge/BuyMeACoffee-Thanks-orange)](https://www.buymeacoffee.com/Foxies)
 <br/><br/>

  You can also send me a tip via [Thingiverse](https://www.thingiverse.com/FamStel/about) if you prefer.

 Massive thank you in advance :heart:

# Version history
## Marlin 2.0 Bugfix Branch
 Update ESP3D 20201213 
  - Last release v2.1.1b8,

## Marlin 2.0.7 
 Update ESP3D 20201001 
  - Last release v2.1.0,



