# Predefined stock printer environments
In addition to the board and platform related environments in [platform.ini](../../platformio.ini) the file [printer.ini](printer.ini) defines final example environments picking up example configurations of the subfolders.

If you want to start optimization, tuning and adoption, the following configurations are a good starting point.

  | Manufacturer<br />Printer | Configuration<br />Environment | Description<br />Motherboard |
  | :------------------------ | :----------------------------- | :--------------------------- |
  | Creality<br />__CR-8__ | [config/examples/Creality/CR-8](Creality/CR-8)<br />`printer_creality_cr_8` | [Creality](Creality) __[Ramps](../../Marlin/src/pins/ramps/pins_RAMPS_ENDER_4.h)__ (w/ A4988)<br />`BOARD_RAMPS_ENDER_4` |
  | Creality<br />__CR-10__ | [config/examples/Creality/CR-10](Creality/CR-10)<br />`printer_creality_cr_10` | [Creality](Creality) __[Melzi](../../Marlin/src/pins/sanguino/pins_MELZI_CREALITY.h)__ (w/ A4988)<br />`BOARD_MELZI_CREALITY` |
  | Creality<br />__CR-10 mini__ | [config/examples/Creality/CR-10mini](Creality/CR-10mini)<br />`printer_creality_cr_10_mini` | [Creality](Creality) __[Melzi](../../Marlin/src/pins/sanguino/pins_MELZI_CREALITY.h)__ (w/ A4988)<br />`BOARD_MELZI_CREALITY` |
  | Creality<br />__CR-10S__ | [config/examples/Creality/CR-10S](Creality/CR-10S)<br />`printer_creality_cr_10_s` | [Creality](Creality) __[Ramps](../../Marlin/src/pins/ramps/pins_BOARD_RAMPS_CREALITY.h)__ (w/ A4988)<br />`BOARD_RAMPS_CREALITY` |
  | Creality<br />__CR-10 S5__ | [config/examples/Creality/CR-10_5S](Creality/CR-10%5FS5)<br />`printer_creality_cr_10_5s` | [Creality](Creality) __[Ramps](../../Marlin/src/pins/ramps/pins_BOARD_RAMPS_CREALITY.h)__ (w/ A4988)<br />`BOARD_RAMPS_CREALITY` |
  | Creality<br />__CR-20__ | [config/examples/Creality/CR-20](Creality/CR-20)<br />`printer_creality_cr_20` | [Creality](Creality) __[Ramps](../../Marlin/src/pins/ramps/pins_BOARD_RAMPS_14_EFB.h)__ (w/ A4988)<br />`BOARD_RAMPS_14_EFB` |
  | Creality<br />__CR-20 pro__ | [config/examples/Creality/CR-20 Pro](Creality/CR-20%20Pro)<br />`printer_creality_cr_20_pro` | [Creality](Creality) __[Ramps](../../Marlin/src/pins/ramps/pins_BOARD_RAMPS_14_EFB.h)__ (w/ A4988)<br />`BOARD_RAMPS_14_EFB` |
  | Creality<br />__Ender 2__ | [config/examples/Creality/Ender-2](Creality/Ender-2)<br />`printer_creality_ender_2` | [Creality](Creality) __[Melzi](../../Marlin/src/pins/sanguino/pins_MELZI_CREALITY.h)__ (w/ A4988)<br />`BOARD_MELZI_CREALITY` |
  | Creality<br />__Ender 3__ | [config/examples/Creality/Ender-3](Creality/Ender-3)<br />`printer_creality_ender_3` | [Creality](Creality) __[Melzi](../../Marlin/src/pins/sanguino/pins_MELZI_CREALITY.h)__ (w/ A4988)<br />`BOARD_MELZI_CREALITY` |
  | Creality<br />__Ender 3__ | [config/examples/SKR Mini E3 V1.0](BigTreeTech/SKR%20Mini%20E3%201.0)<br />`printer_creality_ender_3_with_skr_mini_e3_v1_0` | [Bigtreetech](BigTreeTech) __[SKR Mini E3 1.0](../../Marlin/src/pins/stm32/pins_BTT_SKR_MINI_E3_V1_0.h)__ (w/ TMC2209)<br />`BOARD_BTT_SKR_MINI_E3_V1_0` |
  | Creality<br />__Ender 3__ | [config/examples/SKR Mini E3 V1.2](BigTreeTech/SKR%20Mini%20E3%201.2)<br />`printer_creality_ender_3_with_skr_mini_e3_v1_2` | [Bigtreetech](Bigtreetech) __[SKR Mini E3 1.2](../../Marlin/src/pins/stm32/pins_BTT_SKR_MINI_E3_V1_2.h)__ (w/ TMC22009)<br />`BOARD_BTT_SKR_MINI_E3_V1_2` |
  | Creality<br />__Ender 4__ | [config/examples/Creality/Ender-4](Creality/Ender-4)<br />`printer_creality_ender_4` | [Creality](Creality) __[Ramps](../../Marlin/src/pins/ramps/pins_RAMPS_ENDER_4.h)__ (w/ A4988)<br />`BOARD_RAMPS_ENDER_4` |
  | Creality<br />__Ender 5__ | [config/examples/Creality/Ender-5](Creality/Ender-5)<br />`printer_creality_ender_5` | [Creality](Creality) __[Melzi](../../Marlin/src/pins/sanguino/pins_MELZI_CREALITY.h)__ (w/ A4988)<br />`BOARD_MELZI_CREALITY` |
