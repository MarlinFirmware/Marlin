/* Remap pin name */
PA_9_R       = PA_9    | PREMAP,
PA_10_R      = PA_10   | PREMAP,

/* Alternate pin name */
PA_1_ALT1    = PA_1    | ALT1,
PA_2_ALT1    = PA_2    | ALT1,
PA_3_ALT1    = PA_3    | ALT1,
PA_4_ALT1    = PA_4    | ALT1,
PA_6_ALT1    = PA_6    | ALT1,
PA_6_ALT2    = PA_6    | ALT2,
PA_7_ALT1    = PA_7    | ALT1,
PA_7_ALT2    = PA_7    | ALT2,
PA_7_ALT3    = PA_7    | ALT3,
PA_9_ALT1    = PA_9    | ALT1,
PA_9_R_ALT1  = PA_9_R  | ALT1,
PA_10_ALT1   = PA_10   | ALT1,
PA_10_R_ALT1 = PA_10_R | ALT1,
PA_14_ALT1   = PA_14   | ALT1,
PA_15_ALT1   = PA_15   | ALT1,
PB_0_ALT1    = PB_0    | ALT1,
PB_1_ALT1    = PB_1    | ALT1,
PB_1_ALT2    = PB_1    | ALT2,
PB_3_ALT1    = PB_3    | ALT1,
PB_4_ALT1    = PB_4    | ALT1,
PB_5_ALT1    = PB_5    | ALT1,
PB_6_ALT1    = PB_6    | ALT1,
PB_6_ALT2    = PB_6    | ALT2,
PB_7_ALT1    = PB_7    | ALT1,
PB_8_ALT1    = PB_8    | ALT1,
PB_9_ALT1    = PB_9    | ALT1,
PB_13_ALT1   = PB_13   | ALT1,
PB_14_ALT1   = PB_14   | ALT1,
PB_15_ALT1   = PB_15   | ALT1,
PB_15_ALT2   = PB_15   | ALT2,
PC_0_ALT1    = PC_0    | ALT1,
PC_1_ALT1    = PC_1    | ALT1,
PC_4_ALT1    = PC_4    | ALT1,
PC_5_ALT1    = PC_5    | ALT1,
PC_6_ALT1    = PC_6    | ALT1,
PC_7_ALT1    = PC_7    | ALT1,
PC_8_ALT1    = PC_8    | ALT1,
PC_9_ALT1    = PC_9    | ALT1,
PC_10_ALT1   = PC_10   | ALT1,
PC_11_ALT1   = PC_11   | ALT1,
PD_4_ALT1    = PD_4    | ALT1,

/* SYS_WKUP */
#ifdef PWR_WAKEUP_PIN1
  SYS_WKUP1 = PA_0,
#endif
#ifdef PWR_WAKEUP_PIN2
  SYS_WKUP2 = PC_13,
#endif
#ifdef PWR_WAKEUP_PIN3
  SYS_WKUP3 = NC,
#endif
#ifdef PWR_WAKEUP_PIN4
  SYS_WKUP4 = PA_2,
#endif
#ifdef PWR_WAKEUP_PIN5
  SYS_WKUP5 = PC_5,
#endif
#ifdef PWR_WAKEUP_PIN6
  SYS_WKUP6 = PB_5,
#endif
#ifdef PWR_WAKEUP_PIN7
  SYS_WKUP7 = NC,
#endif
#ifdef PWR_WAKEUP_PIN8
  SYS_WKUP8 = NC,
#endif

/* USB */
#ifdef USBCON
  USB_DM  = PA_11,
  USB_DP  = PA_12,
  #ifdef USB_NOE_PA_4
    USB_NOE = PA_4,
  #endif
  #ifdef USB_NOE_PA_13
    USB_NOE = PA_13,
  #endif
  #ifdef USB_NOE_PA_15
    USB_NOE = PA_15,
  #endif
  #ifdef USB_NOE_PC_9
    USB_NOE = PC_9,
  #endif
#endif
