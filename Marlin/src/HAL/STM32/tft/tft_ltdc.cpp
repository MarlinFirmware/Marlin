/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#include "../../platforms.h"

#ifdef HAL_STM32

#include "../../../inc/MarlinConfig.h"

#if HAS_LTDC_TFT

#include "tft_ltdc.h"
#include "pinconfig.h"

#define FRAME_BUFFER_ADDRESS 0XC0000000  // SDRAM address

#define SDRAM_TIMEOUT                            ((uint32_t)0xFFFF)
#define REFRESH_COUNT                            ((uint32_t)0x02A5)  // SDRAM refresh counter

#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)

void SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef *hsdram, FMC_SDRAM_CommandTypeDef *Command) {

  __IO uint32_t tmpmrd =0;
  /* Step 1:  Configure a clock configuration enable command */
  Command->CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;
  Command->CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
  Command->AutoRefreshNumber = 1;
  Command->ModeRegisterDefinition = 0;
  /* Send the command */
  HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT);

  /* Step 2: Insert 100 us minimum delay */
  /* Inserted delay is equal to 1 ms due to systick time base unit (ms) */
  HAL_Delay(1);

  /* Step 3: Configure a PALL (precharge all) command */
  Command->CommandMode = FMC_SDRAM_CMD_PALL;
  Command->CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
  Command->AutoRefreshNumber = 1;
  Command->ModeRegisterDefinition = 0;
  /* Send the command */
  HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT);

  /* Step 4 : Configure a Auto-Refresh command */
  Command->CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
  Command->CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
  Command->AutoRefreshNumber = 8;
  Command->ModeRegisterDefinition = 0;
  /* Send the command */
  HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT);

  /* Step 5: Program the external memory mode register */
  tmpmrd = (uint32_t)(SDRAM_MODEREG_BURST_LENGTH_1          |
                      SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |
                      SDRAM_MODEREG_CAS_LATENCY_2           |
                      SDRAM_MODEREG_OPERATING_MODE_STANDARD |
                      SDRAM_MODEREG_WRITEBURST_MODE_SINGLE);

  Command->CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
  Command->CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
  Command->AutoRefreshNumber = 1;
  Command->ModeRegisterDefinition = tmpmrd;
  /* Send the command */
  HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT);

  /* Step 6: Set the refresh rate counter */
  /* Set the device refresh rate */
  HAL_SDRAM_ProgramRefreshRate(hsdram, REFRESH_COUNT);
}

void SDRAM_Config() {

  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_FMC_CLK_ENABLE();

  SDRAM_HandleTypeDef      hsdram;
  FMC_SDRAM_TimingTypeDef  SDRAM_Timing;
  FMC_SDRAM_CommandTypeDef command;

  /* Configure the SDRAM device */
  hsdram.Instance = FMC_SDRAM_DEVICE;
  hsdram.Init.SDBank             = FMC_SDRAM_BANK1;
  hsdram.Init.ColumnBitsNumber   = FMC_SDRAM_COLUMN_BITS_NUM_9;
  hsdram.Init.RowBitsNumber      = FMC_SDRAM_ROW_BITS_NUM_13;
  hsdram.Init.MemoryDataWidth    = FMC_SDRAM_MEM_BUS_WIDTH_16;
  hsdram.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
  hsdram.Init.CASLatency         = FMC_SDRAM_CAS_LATENCY_2;
  hsdram.Init.WriteProtection    = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
  hsdram.Init.SDClockPeriod      = FMC_SDRAM_CLOCK_PERIOD_2;
  hsdram.Init.ReadBurst          = FMC_SDRAM_RBURST_ENABLE;
  hsdram.Init.ReadPipeDelay      = FMC_SDRAM_RPIPE_DELAY_0;

  /* Timing configuration for 100Mhz as SDRAM clock frequency (System clock is up to 200Mhz) */
  SDRAM_Timing.LoadToActiveDelay    = 2;
  SDRAM_Timing.ExitSelfRefreshDelay = 8;
  SDRAM_Timing.SelfRefreshTime      = 6;
  SDRAM_Timing.RowCycleDelay        = 6;
  SDRAM_Timing.WriteRecoveryTime    = 2;
  SDRAM_Timing.RPDelay              = 2;
  SDRAM_Timing.RCDDelay             = 2;

  /* Initialize the SDRAM controller */
  if (HAL_SDRAM_Init(&hsdram, &SDRAM_Timing) != HAL_OK)
  {
    /* Initialization Error */
  }

  /* Program the SDRAM external device */
  SDRAM_Initialization_Sequence(&hsdram, &command);
}

void LTDC_Config() {

  __HAL_RCC_LTDC_CLK_ENABLE();
  __HAL_RCC_DMA2D_CLK_ENABLE();

  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

  /* The PLL3R is configured to provide the LTDC PCLK clock */
  /* PLL3_VCO Input = HSE_VALUE / PLL3M = 25Mhz / 5 = 5 Mhz */
  /* PLL3_VCO Output = PLL3_VCO Input * PLL3N = 5Mhz * 160 = 800 Mhz */
  /* PLLLCDCLK = PLL3_VCO Output/PLL3R = 800Mhz / 16 = 50Mhz */
  /* LTDC clock frequency = PLLLCDCLK = 50 Mhz */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  PeriphClkInitStruct.PLL3.PLL3M = 5;
  PeriphClkInitStruct.PLL3.PLL3N = 160;
  PeriphClkInitStruct.PLL3.PLL3FRACN = 0;
  PeriphClkInitStruct.PLL3.PLL3P = 2;
  PeriphClkInitStruct.PLL3.PLL3Q = 2;
  PeriphClkInitStruct.PLL3.PLL3R = (800 / LTDC_LCD_CLK);
  PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOWIDE;
  PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_2;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

  LTDC_HandleTypeDef hltdc_F;
  LTDC_LayerCfgTypeDef pLayerCfg;

  /* LTDC Initialization -------------------------------------------------------*/

  /* Polarity configuration */
  /* Initialize the horizontal synchronization polarity as active low */
  hltdc_F.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  /* Initialize the vertical synchronization polarity as active low */
  hltdc_F.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  /* Initialize the data enable polarity as active low */
  hltdc_F.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  /* Initialize the pixel clock polarity as input pixel clock */
  hltdc_F.Init.PCPolarity = LTDC_PCPOLARITY_IPC;

  /* Timing configuration */
  hltdc_F.Init.HorizontalSync = (LTDC_LCD_HSYNC - 1);
  hltdc_F.Init.VerticalSync = (LTDC_LCD_VSYNC - 1);
  hltdc_F.Init.AccumulatedHBP = (LTDC_LCD_HSYNC + LTDC_LCD_HBP - 1);
  hltdc_F.Init.AccumulatedVBP = (LTDC_LCD_VSYNC + LTDC_LCD_VBP - 1);
  hltdc_F.Init.AccumulatedActiveH = (TFT_HEIGHT + LTDC_LCD_VSYNC + LTDC_LCD_VBP - 1);
  hltdc_F.Init.AccumulatedActiveW = (TFT_WIDTH + LTDC_LCD_HSYNC + LTDC_LCD_HBP - 1);
  hltdc_F.Init.TotalHeigh = (TFT_HEIGHT + LTDC_LCD_VSYNC + LTDC_LCD_VBP + LTDC_LCD_VFP - 1);
  hltdc_F.Init.TotalWidth = (TFT_WIDTH + LTDC_LCD_HSYNC + LTDC_LCD_HBP + LTDC_LCD_HFP - 1);

  /* Configure R,G,B component values for LCD background color : all black background */
  hltdc_F.Init.Backcolor.Blue = 0;
  hltdc_F.Init.Backcolor.Green = 0;
  hltdc_F.Init.Backcolor.Red = 0;

  hltdc_F.Instance = LTDC;

  /* Layer0 Configuration ------------------------------------------------------*/

  /* Windowing configuration */
  pLayerCfg.WindowX0 = 0;
  pLayerCfg.WindowX1 = TFT_WIDTH;
  pLayerCfg.WindowY0 = 0;
  pLayerCfg.WindowY1 = TFT_HEIGHT;

  /* Pixel Format configuration*/
  pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;

  /* Start Address configuration : frame buffer is located at SDRAM memory */
  pLayerCfg.FBStartAdress = (uint32_t)(FRAME_BUFFER_ADDRESS);

  /* Alpha constant (255 == totally opaque) */
  pLayerCfg.Alpha = 255;

  /* Default Color configuration (configure A,R,G,B component values) : no background color */
  pLayerCfg.Alpha0 = 0; /* fully transparent */
  pLayerCfg.Backcolor.Blue = 0;
  pLayerCfg.Backcolor.Green = 0;
  pLayerCfg.Backcolor.Red = 0;

  /* Configure blending factors */
  pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
  pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;

  /* Configure the number of lines and number of pixels per line */
  pLayerCfg.ImageWidth  = TFT_WIDTH;
  pLayerCfg.ImageHeight = TFT_HEIGHT;

  /* Configure the LTDC */
  if (HAL_LTDC_Init(&hltdc_F) != HAL_OK)
  {
    /* Initialization Error */
  }

  /* Configure the Layer*/
  if (HAL_LTDC_ConfigLayer(&hltdc_F, &pLayerCfg, 0) != HAL_OK)
  {
    /* Initialization Error */
  }
}

uint16_t TFT_LTDC::x_min = 0;
uint16_t TFT_LTDC::x_max = 0;
uint16_t TFT_LTDC::y_min = 0;
uint16_t TFT_LTDC::y_max = 0;
uint16_t TFT_LTDC::x_cur = 0;
uint16_t TFT_LTDC::y_cur = 0;
uint8_t TFT_LTDC::reg = 0;
volatile uint16_t* TFT_LTDC::framebuffer = (volatile uint16_t* )FRAME_BUFFER_ADDRESS;

void TFT_LTDC::Init() {

  // SDRAM pins init
  for (uint16_t i = 0; PinMap_SDRAM[i].pin != NC; i++)
    pinmap_pinout(PinMap_SDRAM[i].pin, PinMap_SDRAM);

  // SDRAM peripheral config
  SDRAM_Config();

  // LTDC pins init
  for (uint16_t i = 0; PinMap_LTDC[i].pin != NC; i++)
    pinmap_pinout(PinMap_LTDC[i].pin, PinMap_LTDC);

  // LTDC peripheral config
  LTDC_Config();
}

uint32_t TFT_LTDC::GetID() {
  return 0xABAB;
}

uint32_t TFT_LTDC::ReadID(tft_data_t Reg) {
  return 0xABAB;
}

bool TFT_LTDC::isBusy() {
  return false;
}

uint16_t TFT_LTDC::ReadPoint(uint16_t x, uint16_t y) {
  return framebuffer[(TFT_WIDTH * y) + x];
}

void TFT_LTDC::DrawPoint(uint16_t x, uint16_t y, uint16_t color) {
  framebuffer[(TFT_WIDTH * y) + x] = color;
}

void TFT_LTDC::DrawRect(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t color) {

  if (sx == ex || sy == ey) return;

  uint16_t offline = TFT_WIDTH - (ex - sx);
  uint32_t addr = (uint32_t)&framebuffer[(TFT_WIDTH * sy) + sx];

  CBI(DMA2D->CR, 0);
  DMA2D->CR = 3 << 16;
  DMA2D->OPFCCR = 0X02;
  DMA2D->OOR = offline;
  DMA2D->OMAR = addr;
  DMA2D->NLR = (ey - sy) | ((ex - sx) << 16);
  DMA2D->OCOLR = color;
  SBI(DMA2D->CR, 0);

  uint32_t timeout = 0;
  while (!TEST(DMA2D->ISR, 1)) {
    timeout++;
    if (timeout > 0x1FFFFF) break;
  }
  SBI(DMA2D->IFCR, 1);
}

void TFT_LTDC::DrawImage(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t *colors) {

  if (sx == ex || sy == ey) return;

  uint16_t offline = TFT_WIDTH - (ex - sx);
  uint32_t addr = (uint32_t)&framebuffer[(TFT_WIDTH * sy) + sx];

  CBI(DMA2D->CR, 0);
  DMA2D->CR = 0 << 16;
  DMA2D->FGPFCCR = 0X02;
  DMA2D->FGOR = 0;
  DMA2D->OOR = offline;
  DMA2D->FGMAR = (uint32_t)colors;
  DMA2D->OMAR = addr;
  DMA2D->NLR = (ey - sy) | ((ex - sx) << 16);
  SBI(DMA2D->CR, 0);

  uint32_t timeout = 0;
  while (!TEST(DMA2D->ISR, 1)) {
    timeout++;
    if (timeout > 0x1FFFFF) break;
  }
  SBI(DMA2D->IFCR, 1);
}

void TFT_LTDC::WriteData(uint16_t data) {
  switch (reg) {
    case 0x01: x_cur = x_min = data; return;
    case 0x02: x_max = data; return;
    case 0x03: y_cur = y_min = data; return;
    case 0x04: y_max = data; return;
  }
  Transmit(data);
}

void TFT_LTDC::Transmit(tft_data_t Data) {
  DrawPoint(x_cur, y_cur, Data);
  x_cur++;
  if (x_cur > x_max) {
    x_cur = x_min;
    y_cur++;
    if (y_cur > y_max) y_cur = y_min;
  }
}

void TFT_LTDC::WriteReg(uint16_t Reg) {
  reg = Reg;
}

void TFT_LTDC::TransmitDMA(uint32_t MemoryIncrease, uint16_t *Data, uint16_t Count) {

  while (x_cur != x_min && Count) {
    Transmit(*Data);
    if (MemoryIncrease == DMA_PINC_ENABLE) Data++;
    Count--;
  }

  uint16_t width = x_max - x_min + 1;
  uint16_t height = Count / width;
  uint16_t x_end_cnt = Count - (width * height);

  if (height) {
    if (MemoryIncrease == DMA_PINC_ENABLE) {
      DrawImage(x_min, y_cur, x_min + width, y_cur + height, Data);
      Data += width * height;
    }
    else
      DrawRect(x_min, y_cur, x_min + width, y_cur + height, *Data);
    y_cur += height;
  }

  while (x_end_cnt) {
    Transmit(*Data);
    if (MemoryIncrease == DMA_PINC_ENABLE) Data++;
    x_end_cnt--;
  }
}

#endif // HAS_LTDC_TFT
#endif // HAL_STM32
