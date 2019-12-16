# Predefined stock printer environments
In addition to the board and platform related environments in [platform.ini](../../platformio.ini) the file [printer.ini](printer.ini) defines final example environments picking up example configurations of the subfolders.

If you want to start optimization, tuning and adoption, the following configurations are a good starting point.

  | Manufacturer<br />Printer | Configuration<br />Environment | Motherboard<br />Description |
  | :------------------------ | :----------------------------- | :--------------------------- |
  | Creality<br />Ender 3 | [config/examples/Creality/Ender-3](Creality/Ender-3)<br />`printer_creality_ender_3` | [Creality](Creality) __[Melzi](../../Marlin/src/pins/sanguino/pins_MELZI_CREALITY.h)__<br />default stock board<br />with onboard A4988 steppers |
  | Creality<br />Ender 3 | [config/examples/SKR Mini E3 V1.0](BigTreeTech/SKR%20Mini%20E3%201.0)<br />`printer_creality_ender_3_with_skr_mini_e3_v1_0` | [Bigtreetech](BigTreeTech) __[SKR Mini E3 1.0](../../Marlin/src/pins/stm32/pins_BTT_SKR_MINI_E3_V1_0.h)__<br />replacement board<br />with onboard TMC2209 steppers |
  | Creality<br />Ender 3 | [config/examples/SKR Mini E3 V1.2](BigTreeTech/SKR%20Mini%20E3%201.2)<br />`printer_creality_ender_3_with_skr_mini_e3_v1_2` | [Bigtreetech](Bigtreetech) __[SKR Mini E3 1.2](../../Marlin/src/pins/stm32/pins_BTT_SKR_MINI_E3_V1_2.h)__<br />replacement board<br />with onboard TMC2209 steppers |
