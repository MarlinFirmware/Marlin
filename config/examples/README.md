# Predefined stock printer environments
In addition to the board and platform related environments in [platform.ini](../../platformio.ini) the file [printer.ini](printer.ini) defines final example environments picking up example configurations of the subfolders.

If you want to start optimization, tuning and adoption, the following configurations are a good starting point.

#### Creality
...

###### Creality<br>Ender 3

  | Environment<br />Configuration | Description |
  | :---------- | :------------ |
  | [[env:__printer_creality_ender_3__]](printer.ini#L1)<br />[config/examples/Creality/Ender-3](Creality/Ender-3) | [Creality](Creality) Melzi default stock board<br />with onboard A4988 steppers |
  | [[env:__printer_creality_ender_3_with_skr_mini_e3_v1_0__]](printer.ini#L5)<br />[config/examples/SKR Mini E3 V1.0](BigTreeTech/SKR%20Mini%20E3%201.0) | Bigtreetech __[SKR Mini E3 1.0](../BigTreeTech/SKR%20Mini%20E3%201.0)__ replacement board<br />with onboard TMC2209 steppers |
  | [[env:__printer_creality_ender_3_with_skr_mini_e3_v1_2__]](printer.ini#L9)<br />[config/examples/SKR Mini E3 V1.2](BigTreeTech/SKR%20Mini%20E3%201.2) | Bigtreetech __[SKR Mini E3 1.2](../BigTreeTech/SKR%20Mini%20E3%201.2)__ replacement board<br />with onboard TMC2209 steppers |
