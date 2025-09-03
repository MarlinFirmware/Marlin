/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

#ifdef STM32F0xx
  /*
  STM32F030x4 STM32F030x6 STM32F030x8 STM32F030xC     https://www.st.com/resource/en/datasheet/stm32f030f4.pdf
    --> TS_CAL2 not defined in datasheet
  STM32F031x4 STM32F031x6                             https://www.st.com/resource/en/datasheet/stm32f031c4.pdf
  STM32F038x6                                         https://www.st.com/resource/en/datasheet/stm32f038c6.pdf
  STM32F042x4 STM32F042x6                             https://www.st.com/resource/en/datasheet/stm32f042c4.pdf
  STM32F048C6 STM32F048G6 STM32F048T6                 https://www.st.com/resource/en/datasheet/stm32f048c6.pdf
  STM32F051x4 STM32F051x6 STM32F051x8                 https://www.st.com/resource/en/datasheet/dm00039193.pdf
  STM32F058C8 STM32F058R8 STM32F058T8                 https://www.st.com/resource/en/datasheet/stm32f058c8.pdf
  STM32F070CB STM32F070RB STM32F070C6 STM32F070F6     https://www.st.com/resource/en/datasheet/stm32f070c6.pdf
    --> TS_CAL2 not defined in datasheet
  STM32F071x8 STM32F071xB                             https://www.st.com/resource/en/datasheet/stm32f071cb.pdf
  STM32F072x8 STM32F072xB                             https://www.st.com/resource/en/datasheet/stm32f072c8.pdf
  STM32F078CB STM32F078RB STM32F078VB                 https://www.st.com/resource/en/datasheet/stm32f078cb.pdf
  STM32F091xB STM32F091xC                             https://www.st.com/resource/en/datasheet/stm32f091cc.pdf
  STM32F098CC STM32F098RC STM32F098VC                 https://www.st.com/resource/en/datasheet/stm32f098cc.pdf
  */
  #define TS_CAL1_TEMP      30            // Calibration temperature of TS_CAL1 (see specific SoC datasheet)
  #define TS_CAL1_REGOFFSET 0x1FFFF7B8    // Memory address of TS_CAL1 for STM32F030x4/x6/x8/xC (see specific SoC datasheet)
  #define TS_CAL2_TEMP      110           // Calibration temperature of TS_CAL2 (see specific SoC datasheet)
  #define TS_CAL2_REGOFFSET 0x1FFFF7C2    // Memory address of TS_CAL2 for STM32F030x4/x6/x8/xC (see specific SoC datasheet)

#elif defined(STM32F1xx)
  /*
  STM32F100xC STM32F100xD STM32F100xE                 https://www.st.com/resource/en/datasheet/stm32f100rc.pdf
    --> V=1.41
  STM32F100x4 STM32F100x6 STM32F100x8 STM32F100xB     https://www.st.com/resource/en/datasheet/stm32f100cb.pdf
    --> V=1.41
  STM32F101x8 STM32F101xB                             https://www.st.com/resource/en/datasheet/stm32f101r8.pdf
  STM32F101xC STM32F101xD STM32F101xE                 https://www.st.com/resource/en/datasheet/stm32f101rc.pdf
  STM32F101x4 STM32F101x6                             https://www.st.com/resource/en/datasheet/stm32f101c4.pdf
  STM32F101xF STM32F101xG                             https://www.st.com/resource/en/datasheet/stm32f101vf.pdf
  STM32F102x8 STM32F102xB                             https://www.st.com/resource/en/datasheet/stm32f102c8.pdf
    --> V=1.42 / Slope=4.35
  STM32F102x4 STM32F102x6                             https://www.st.com/resource/en/datasheet/stm32f102c4.pdf
    --> V=1.42 / Slope=4.35
  STM32F103x8 STM32F103xB                             https://www.st.com/resource/en/datasheet/stm32f103c8.pdf
  STM32F103xC STM32F103xD STM32F103xE                 https://www.st.com/resource/en/datasheet/stm32f103rc.pdf
  STM32F103x4 STM32F103x6                             https://www.st.com/resource/en/datasheet/stm32f103c4.pdf
  STM32F103xF STM32F103xG                             https://www.st.com/resource/en/datasheet/stm32f103rg.pdf
  STM32F105xx STM32F107xx                             https://www.st.com/resource/en/datasheet/stm32f105r8.pdf
  */
  #define TS_TYPICAL_V     1.43
  #define TS_TYPICAL_TEMP  25
  #define TS_TYPICAL_SLOPE 4.3

#elif defined(STM32F2xx)
  /*
  STM32F205xx STM32F207xx                             https://www.st.com/resource/en/datasheet/stm32f205rb.pdf
  STM32F215xx STM32F217xx                             https://www.st.com/resource/en/datasheet/stm32f215re.pdf
  */
  #define TS_TYPICAL_V     0.76
  #define TS_TYPICAL_TEMP  25
  #define TS_TYPICAL_SLOPE 2.5

#elif defined(STM32F3xx)
  /*
  STM32F301x6 STM32F301x8                             https://www.st.com/resource/en/datasheet/stm32f301c6.pdf
  STM32F302xD STM32F302xE                             https://www.st.com/resource/en/datasheet/stm32f302re.pdf
  STM32F302x6 STM32F302x8                             https://www.st.com/resource/en/datasheet/stm32f302r6.pdf
  STM32F302xB STM32F302xC                             https://www.st.com/resource/en/datasheet/stm32f302cb.pdf
  STM32F303xD STM32F303xE                             https://www.st.com/resource/en/datasheet/stm32f303re.pdf
  STM32F303xB STM32F303xC                             https://www.st.com/resource/en/datasheet/stm32f303cb.pdf
  STM32F303x6/x8                                      https://www.st.com/resource/en/datasheet/stm32f303c6.pdf
  STM32F334x4 STM32F334x6 STM32F334x8                 https://www.st.com/resource/en/datasheet/stm32f334k4.pdf
  STM32F373xx                                         https://www.st.com/resource/en/datasheet/stm32f373cc.pdf
  STM32F358xC                                         https://www.st.com/resource/en/datasheet/stm32f358cc.pdf
  STM32F378xx                                         https://www.st.com/resource/en/datasheet/stm32f378cc.pdf
  STM32F318C8 STM32F318K8                             https://www.st.com/resource/en/datasheet/stm32f318c8.pdf
  STM32F328C8                                         https://www.st.com/resource/en/datasheet/stm32f328c8.pdf
  STM32F398VE                                         https://www.st.com/resource/en/datasheet/stm32f398ve.pdf
  */
  #define TS_CAL1_TEMP      30
  #define TS_CAL1_REGOFFSET 0x1FFFF7B8
  #define TS_CAL2_TEMP      110
  #define TS_CAL2_REGOFFSET 0x1FFFF7C2

#elif defined(STM32F4xx)
  /*
  STM32F401xD STM32F401xE                             https://www.st.com/resource/en/datasheet/stm32f401re.pdf
  STM32F411xC STM32F411xE                             https://www.st.com/resource/en/datasheet/stm32f411ce.pdf
  STM32F446xC/E                                       https://www.st.com/resource/en/datasheet/stm32f446mc.pdf
  STM32F479xx                                         https://www.st.com/resource/en/datasheet/stm32f479ai.pdf
  STM32F412xE STM32F412xG                             https://www.st.com/resource/en/datasheet/stm32f412ce.pdf
  STM32F410x8 STM32F410xB                             https://www.st.com/resource/en/datasheet/stm32f410cb.pdf
  STM32F469xx                                         https://www.st.com/resource/en/datasheet/stm32f469ae.pdf
  STM32F423xH                                         https://www.st.com/resource/en/datasheet/stm32f423ch.pdf
  STM32F413xG STM32F413xH                             https://www.st.com/resource/en/datasheet/stm32f413cg.pdf
  STM32F415xx STM32F417xx                             https://www.st.com/resource/en/datasheet/stm32f415rg.pdf
  STM32F405xx STM32F407xx                             https://www.st.com/resource/en/datasheet/stm32f405rg.pdf
  STM32F427xx STM32F429xx                             https://www.st.com/resource/en/datasheet/stm32f427vg.pdf
  STM32F437xx STM32F439xx                             https://www.st.com/resource/en/datasheet/stm32f437vg.pdf
  STM32F401xB STM32F401xC                             https://www.st.com/resource/en/datasheet/stm32f401cb.pdf
  */
  #define TS_CAL1_TEMP      30
  #define TS_CAL1_REGOFFSET 0x1FFF7A2C
  #define TS_CAL2_TEMP      110
  #define TS_CAL2_REGOFFSET 0x1FFF7A2E

#elif defined(STM32F7xx)
  /*
  STM32F756xx                                         https://www.st.com/resource/en/datasheet/stm32f756bg.pdf
  STM32F745xx STM32F746xx                             https://www.st.com/resource/en/datasheet/stm32f745ie.pdf
  STM32F777xx STM32F778Ax STM32F779xx                 https://www.st.com/resource/en/datasheet/stm32f777bi.pdf
  STM32F765xx STM32F767xx STM32F768Ax STM32F769xx     https://www.st.com/resource/en/datasheet/stm32f765bi.pdf
  STM32F722xx STM32F723xx                             https://www.st.com/resource/en/datasheet/stm32f722ic.pdf
    --> TS_CAL1/2 = 0x1FF07A2C / 0x1FF07A2E
  STM32F732xx STM32F733xx                             https://www.st.com/resource/en/datasheet/stm32f732ie.pdf
    --> TS_CAL1/2 = 0x1FF07A2C / 0x1FF07A2E
  STM32F750x8                                         https://www.st.com/resource/en/datasheet/stm32f750n8.pdf
  STM32F730x8                                         https://www.st.com/resource/en/datasheet/stm32f730i8.pdf
    --> TS_CAL1/2 = 0x1FF07A2C / 0x1FF07A2E
  */
  #define TS_CAL1_TEMP      30
  #define TS_CAL1_REGOFFSET 0x1FF0F44C
  #define TS_CAL2_TEMP      110
  #define TS_CAL2_REGOFFSET 0x1FF0F44E

#elif defined(STM32G0xx)
  /*
  STM32G030x6/x8                                      https://www.st.com/resource/en/datasheet/stm32g030c6.pdf
    --> TS_CAL2 not defined in datasheet
  STM32G050x6/x8                                      https://www.st.com/resource/en/datasheet/stm32g050c6.pdf
  STM32G0B0KE/CE/RE/VE                                https://www.st.com/resource/en/datasheet/stm32g0b0ce.pdf
    --> TS_CAL2 not defined in datasheet
  STM32G081xB                                         https://www.st.com/resource/en/datasheet/stm32g081cb.pdf
  STM32G071x8/xB                                      https://www.st.com/resource/en/datasheet/stm32g071c8.pdf
  STM32G031x4/x6/x8                                   https://www.st.com/resource/en/datasheet/stm32g031c6.pdf
  STM32G041x6/x8                                      https://www.st.com/resource/en/datasheet/stm32g041c8.pdf
  STM32G051x6/x8                                      https://www.st.com/resource/en/datasheet/stm32g051c6.pdf
  STM32G061x6/x8                                      https://www.st.com/resource/en/datasheet/stm32g061c6.pdf
  STM32G0B1xB/xC/xE                                   https://www.st.com/resource/en/datasheet/stm32g0b1cc.pdf
  STM32G0C1xC/xE                                      https://www.st.com/resource/en/datasheet/stm32g0c1cc.pdf
  */
  #define TS_CAL1_TEMP      30
  #define TS_CAL1_REGOFFSET 0x1FFF75A8
  #define TS_CAL2_TEMP      130
  #define TS_CAL2_REGOFFSET 0x1FFF75CA

#elif defined(STM32G4xx)
  /*
  STM32G431x6 STM32G431x8 STM32G431xB             https://www.st.com/resource/en/datasheet/stm32g431c6.pdf
  STM32G441xB                                     https://www.st.com/resource/en/datasheet/stm32g441cb.pdf
  STM32G491xC STM32G491xE                         https://www.st.com/resource/en/datasheet/stm32g491cc.pdf
  STM32G4A1xE                                     https://www.st.com/resource/en/datasheet/stm32g4a1ce.pdf
  STM32G473xB STM32G473xC STM32G473xE             https://www.st.com/resource/en/datasheet/stm32g473cb.pdf
  STM32G483xE                                     https://www.st.com/resource/en/datasheet/stm32g483ce.pdf
    --> TS_CAL1/TS_CAL2 not defined in datasheet
  STM32G474xB STM32G474xC STM32G474xE             https://www.st.com/resource/en/datasheet/stm32g474cb.pdf
  STM32G484xE                                     https://www.st.com/resource/en/datasheet/stm32g484ce.pdf
  */
  #define TS_CAL1_TEMP      30
  #define TS_CAL1_REGOFFSET 0x1FFF75A8
  #define TS_CAL2_TEMP      130
  #define TS_CAL2_REGOFFSET 0x1FFF75CA

#elif defined(STM32H7xx)
  /*
  STM32H7A3xI/G
    --> TS_CAL1/2 = 0x08FFF814 / 0x08FFF818
  STM32H7B0xB
    --> TS_CAL1/2 = 0x08FFF814 / 0x08FFF818
  STM32H7B3xI
    --> TS_CAL1/2 = 0x08FFF814 / 0x08FFF818
  STM32H725xE/G
  STM32H735xG
  STM32H723VE STM32H723VG STM32H723ZE STM32H723ZG
  STM32H730AB STM32H730IB STM32H730VB STM32H730ZB
  STM32H733VG STM32H733ZG
  STM32H742xI/G STM32H743xI/G
    --> CAL2_TEMP = 110
  STM32H745xI/G
  STM32H747xI/G
  STM32H753xI
  STM32H755xI
  STM32H757xI
  STM32H750VB STM32H750ZB STM32H750IB STM32H750XB
    --> CAL2_TEMP = 110
  */
  #define TS_CAL1_TEMP      30
  #define TS_CAL1_REGOFFSET 0x1FF1E820
  #define TS_CAL2_TEMP      130
  #define TS_CAL2_REGOFFSET 0x1FF1E840

#elif defined(STM32L0xx)
  /*
  STM32L010RB
    --> TS_CAL1/TS_CAL2 not defined in datasheet
  STM32L010F4 STM32L010K4
    --> TS_CAL1/TS_CAL2 not defined in datasheet
  STM32L010C6
    --> TS_CAL1/TS_CAL2 not defined in datasheet
  STM32L010K8 STM32L010R8
    --> TS_CAL1/TS_CAL2 not defined in datasheet
  STM32L011x3 STM32L011x4
    --> TS_CAL1 not defined in datasheet
  STM32L021D4 STM32L021F4 STM32L021G4 STM32L021K4
    --> TS_CAL1 not defined in datasheet
  STM32L031x4 STM32L031x6
  STM32L041x6
  STM32L051x6 STM32L051x8
  STM32L071x8 STM32L071xB STM32L071xZ
  STM32L081CB STM32L081CZ STM32L081KZ
  STM32L052x6 STM32L052x8
  STM32L062K8 STM32L062T8 STM32L062C8
  STM32L072x8 STM32L072xB STM32L072xZ
  STM32L082KB STM32L082KZ STM32L082CZ
  STM32L053C6 STM32L053C8 STM32L053R6 STM32L053R8
  STM32L063C8 STM32L063R8
  STM32L073x8 STM32L073xB STM32L073xZ
  STM32L083x8 STM32L083xB STM32L083xZ
  */
  #define TS_CAL1_TEMP      30
  #define TS_CAL1_REGOFFSET 0x1FF8007A
  #define TS_CAL2_TEMP      130
  #define TS_CAL2_REGOFFSET 0x1FF8007E

#elif defined(STM32L1xx)
  /*
  STM32L100x6/8/B-A
    --> TS_CAL1/TS_CAL2 not defined in datasheet
  STM32L100RC
    --> TS_CAL1/TS_CAL2 not defined in datasheet
  STM32L100C6 STM32L100R8/RB
    --> TS_CAL1/TS_CAL2 not defined in datasheet
  STM32L151x6/8/B-A STM32L152x6/8/B-A
    --> TS_CAL1/2 = 0x08FFF814 / 0x08FFF818
  STM32L151xD STM32L152xD
  STM32L151VD-X STM32L152VD-X
  STM32L15xCC STM32L15xRC STM32L15xUC STM32L15xVC
  STM32L15xQC STM32L15xRC-A STM32L15xVC-A STM32L15xZC
  STM32L162xE
  STM32L162VD STM32L162ZD STM32L162QD STM32L162RD
  STM32L162VC STM32L162RC
  STM32L162VD-X
  STM32L162QC STM32L162VC-A STM32L162ZC STM32L162RC-A
  */
  #define TS_CAL1_TEMP      30
  #define TS_CAL1_REGOFFSET 0x1FF800FA
  #define TS_CAL2_TEMP      110
  #define TS_CAL2_REGOFFSET 0x1FF800FE

#elif defined(STM32L4xx)
  /*
  STM32L431xx
  STM32L451xx
  STM32L471xx
    --> CAL2_TEMP = 110
  STM32L412xx
  STM32L422xx
  STM32L432KB STM32L432KC
  STM32L442KC
  STM32L452xx
  STM32L462CE STM32L462RE STM32L462VE
  STM32L433xx
  STM32L443CC STM32L443RC STM32L443VC
  STM32L475xx
    --> CAL2_TEMP = 110
  STM32L476xx
    --> CAL2_TEMP = 110
  STM32L486xx :
    --> CAL2_TEMP = 110
  STM32L496xx
  STM32L4A6xG
  */
  #define TS_CAL1_TEMP      30
  #define TS_CAL1_REGOFFSET 0x1FFF75A8
  #define TS_CAL2_TEMP      130
  #define TS_CAL2_REGOFFSET 0x1FFF75CA

#elif defined(STM32MP1xx)
  /*
  STM32MP131A STM32MP131D
  STM32MP131C STM32MP131F
  STM32MP133A STM32MP133D
  STM32MP133C STM32MP133F
  STM32MP135A STM32MP135D
  STM32MP135C STM32MP135F
  STM32MP151A/D
  STM32MP151C/F
  STM32MP153A/D
  STM32MP153C/F
  STM32MP157A/D
  STM32MP157C/F
  */
  // BSEC -> RCC
  //#define TS_CAL1_TEMP      30
  //#define TS_CAL1_REGOFFSET 0x5C00 525C[15:0]
  //#define TS_CAL2_TEMP      130
  //#define TS_CAL2_REGOFFSET 0x5C00 525C[31:16]

#elif defined(STM32WBxx)
  /*
  STM32WB10CC
  STM32WB50CG STM32WB30CE
  STM32WB15CC
  STM32WB55xx STM32WB35xx
  */
  #define TS_CAL1_TEMP      30
  #define TS_CAL1_REGOFFSET 0x1FFF75A8
  #define TS_CAL2_TEMP      130
  #define TS_CAL2_REGOFFSET 0x1FFF75CA
#endif

// TODO implement voltage scaling (calibrated Vrefint) and ADC resolution scaling (when applicable)

/**
 * When provided in datasheet, the use of calibrated values (TS_CAL1, TS_CAL2) should always be preferred over typical values.
 * Typical values may result in important variation from the actual temperature.
 *
 * If calibrated values are not provided in datasheet, it is encouraged to calibrate your specific chip yourself.
 */
#if defined(TS_CAL1_TEMP) && defined(TS_CAL1_REGOFFSET) && defined(TS_CAL2_TEMP) && defined(TS_CAL2_REGOFFSET)

  #define READMEMORY(ADDR) (*((uint16_t const *)(ADDR)))
  #define TEMP_SOC_SENSOR(RAW) (float((TS_CAL2_TEMP) - (TS_CAL1_TEMP)) / (READMEMORY(TS_CAL2_REGOFFSET) - READMEMORY(TS_CAL1_REGOFFSET)) * ((RAW) / float(OVERSAMPLENR) - READMEMORY(TS_CAL1_REGOFFSET)) + (TS_CAL1_TEMP))

#elif defined(TS_TYPICAL_V) && defined(TS_TYPICAL_SLOPE) && defined(TS_TYPICAL_TEMP)

  #define TEMP_SOC_SENSOR(RAW) ((TS_TYPICAL_V - (RAW) / float(OVERSAMPLENR) / float(HAL_ADC_RANGE) * (float(ADC_VREF_MV) / 1000.0f)) / ((TS_TYPICAL_SLOPE) / 1000) + TS_TYPICAL_TEMP)

#endif
