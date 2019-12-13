# Creality Ender 3

This directory contains the default stock configuration examples for [Creality](../Creality) [Ender 3](../Creality/Ender-3).
## Environments
In addition to the environments in [platform.ini](../../../../platformio.ini) this folder offers in [printer.ini](../Creality/Ender-3/printer.ini) completed envs:

  | Environment | Board<br/>Features |
  | :-- | --: |
  | [env:__printer_creality_ender_3__] | Creality Melzi default stock board<br />with onboard A4988 steppers |
  | [env:printer_creality_ender_3_with_bigtreetech_skr_mini_e3_v1_0] | Bigtreetech Skr Mini E3 1.0 replacement board<br />with onboard TMC2209 steppers |
  | [env:printer_creality_ender_3_with_bigtreetech_skr_mini_e3_v1_2] | Bigtreetech Skr Mini E3 1.2 replacement board<br />with onboard TMC2209 steppers |

## Configurations
If you want to start optimization, tuning and adoption, the following configurations are a good starting point.
### default
- based on __Creality's Melzi__ board with __A4988__ steppers
  - [Configuration.h](../Creality/Ender-3/Configuration.h)
  - [Configuration_adv.h](../Creality/Ender-3/Configuration_adv.h)
  - [_Bootscreen.h](../Creality/Ender-3/_Bootscreen.h)
  - [_Statusscreen.h](../Creality/Ender-3/_Statusscreen.h)
  
### custom
- based on __[SKR Mini E3 1.0](../BigTreeTech/SKR%20Mini%20E3%201.0)__ board with __TMC2209__ steppers
  - [Configuration.h](../BigTreeTech/SKR%20Mini%20E3%201.0/Configuration.h)
  - [Configuration_adv.h](../BigTreeTech/SKR%20Mini%20E3%201.0/Configuration_adv.h)
- based on __[SKR Mini E3 1.2](../BigTreeTech/SKR%20Mini%20E3%201.2)__ board with __TMC2209__ steppers
  - [Configuration.h](../BigTreeTech/SKR%20Mini%20E3%201.2/Configuration.h)
  - [Configuration_adv.h](../BigTreeTech/SKR%20Mini%20E3%201.2/Configuration_adv.h)
 
