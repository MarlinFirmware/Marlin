#define _BOARD_C_
#include "startup.h"

#define APP_START_ADDRESS 0xC000
uint32_t CPU_FREQ = 0;

void getResetCause(void)
{
	stc_rmu_rstcause_t stcRmuRstCause;
	MEM_ZERO_STRUCT(stcRmuRstCause);

	RMU_GetResetCause(&stcRmuRstCause);
	printf("ResetCause:%s", (Set == stcRmuRstCause.enMultiRst) ? "[Multiply reset cause]->" : "");
	printf("[%s]\n", (Set == stcRmuRstCause.enXtalErr) ? "Xtal error reset" : (Set == stcRmuRstCause.enClkFreqErr) ? "Clk freqence error reset"
																		  : (Set == stcRmuRstCause.enRamEcc)	   ? "Ram ECC reset"
																		  : (Set == stcRmuRstCause.enRamParityErr) ? "Ram parity error reset"
																		  : (Set == stcRmuRstCause.enMpuErr)	   ? "Mpu error reset"
																		  : (Set == stcRmuRstCause.enSoftware)	   ? "Software reset"
																		  : (Set == stcRmuRstCause.enPowerDown)	   ? "Power down reset"
																		  : (Set == stcRmuRstCause.enSwdt)		   ? "Special watchdog timer reset"
																		  : (Set == stcRmuRstCause.enWdt)		   ? "Watchdog timer reset"
																		  : (Set == stcRmuRstCause.enPvd2)		   ? "Program voltage Dectection 2 reset"
																		  : (Set == stcRmuRstCause.enPvd1)		   ? "Program voltage Dectection 1 reset"
																		  : (Set == stcRmuRstCause.enBrownOut)	   ? "Brown out reset"
																		  : (Set == stcRmuRstCause.enRstPin)	   ? "Reset pin reset"
																		  : (Set == stcRmuRstCause.enPowerOn)	   ? "Power on reset"
																												   : "unknowCause");
}

static void setup_SysClk(void)
{
	stc_clk_xtal_cfg_t stcXtalCfg;
	stc_clk_mpll_cfg_t stcMpllCfg;
	en_clk_sys_source_t enSysClkSrc;
	stc_clk_sysclk_cfg_t stcSysClkCfg;
	stc_clk_freq_t stcClkFreq;
	MEM_ZERO_STRUCT(enSysClkSrc);
	MEM_ZERO_STRUCT(stcSysClkCfg);
	MEM_ZERO_STRUCT(stcXtalCfg);
	MEM_ZERO_STRUCT(stcMpllCfg);

	/* Set bus clk div. */
	stcSysClkCfg.enHclkDiv = ClkSysclkDiv1;	 /* Max 168MHz */
	stcSysClkCfg.enExclkDiv = ClkSysclkDiv2; /* Max 84MHz */
	stcSysClkCfg.enPclk0Div = ClkSysclkDiv1; /* Max 168MHz */
	stcSysClkCfg.enPclk1Div = ClkSysclkDiv2; /* Max 84MHz */
	stcSysClkCfg.enPclk2Div = ClkSysclkDiv4; /* Max 60MHz */
	stcSysClkCfg.enPclk3Div = ClkSysclkDiv4; /* Max 42MHz */
	stcSysClkCfg.enPclk4Div = ClkSysclkDiv2; /* Max 84MHz */
	CLK_SysClkConfig(&stcSysClkCfg);

	/* Switch system clock source to MPLL. */
	/* Use Xtal as MPLL source. */
	stcXtalCfg.enMode = ClkXtalModeOsc;
	stcXtalCfg.enDrv = ClkXtalLowDrv;
	stcXtalCfg.enFastStartup = Enable;
	CLK_XtalConfig(&stcXtalCfg);
	CLK_XtalCmd(Enable);

	/* MPLL config. */
	stcMpllCfg.pllmDiv = 1u;
	stcMpllCfg.plln = 50u;
	stcMpllCfg.PllpDiv = 4u;
	stcMpllCfg.PllqDiv = 4u;
	stcMpllCfg.PllrDiv = 4u;
	CLK_SetPllSource(ClkPllSrcXTAL);
	CLK_MpllConfig(&stcMpllCfg);

	/* flash read wait cycle setting */
	EFM_Unlock();
	EFM_SetLatency(EFM_LATENCY_5);
	EFM_InstructionCacheCmd(Enable);
	EFM_Lock();

	/* Enable MPLL. */
	CLK_MpllCmd(Enable);

	/* Wait MPLL ready. */
	while (Set != CLK_GetFlagStatus(ClkFlagMPLLRdy))
	{
	}
	/* Switch system clock source to MPLL. */
	CLK_SetSysClkSource(CLKSysSrcMPLL);
	/* Check source and frequence. */
	// todo: the 2 below are testers and can be delete in the near future
	enSysClkSrc = CLK_GetSysClkSource();
	CLK_GetClockFreq(&stcClkFreq);
	CPU_FREQ = stcClkFreq.pclk1Freq;
}

void init(void)
{
#ifdef use_02uf_cap
	set_vcap();
#endif
#if defined(__CC_ARM) && defined(__TARGET_FPU_VFP)
	SCB->CPACR |= 0x00F00000;
#endif
	SCB->VTOR = ((uint32_t)APP_START_ADDRESS & SCB_VTOR_TBLOFF_Msk); // bootloader vector startup addr
	setup_SysClk();
	setup_systick();
	setup_time();
	setup_gpio();
	setup_adcs();
	Ddl_UartInit(); // Uart printf port initialize, disable uart3 send
	// setup_I2C1_eeprom();
	setup_Extinterrupt();
	getResetCause();
}
#undef _BOARD_C_
