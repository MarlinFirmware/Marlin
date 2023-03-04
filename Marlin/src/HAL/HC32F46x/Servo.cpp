/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 * Copyright (c) 2017 Victor Perez
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
#ifdef TARGET_HC32F46x

#include "../../inc/MarlinConfig.h"

#if HAS_SERVOS

uint8_t ServoCount = 0;

#include "Servo.h"

/* TIMERA unit and clock definition */
#define TIMERA_UNIT1 M4_TMRA1
#define TIMERA_UNIT1_CLOCK PWC_FCG2_PERIPH_TIMA1
#define TIMERA_UNIT1_OVERFLOW_INT INT_TMRA1_OVF

/* TIMERA channel 1 Port/Pin definition */
#define TIMERA_UNIT1_CH_BL TimeraCh6
#define TIMERA_UNIT1_CH1_PORT PortB
#define TIMERA_UNIT1_CH1_PIN Pin00
#define TIMERA_UNIT1_CH1_FUNC Func_Tima0

#include <boards.h>
#include <io.h>
#ifndef TARGET_HC32F46x
#include <pwm.h>
#endif
#include <wirish_math.h>

#define CYCLES_PER_MICROSECOND (F_CPU / 1000000UL)

/**
 * 20 millisecond period config. For a 1-based prescaler,
 *
 *    (prescaler * overflow / CYC_MSEC) msec = 1 timer cycle = 20 msec
 * => prescaler * overflow = 20 * CYC_MSEC
 *
 * This uses the smallest prescaler that allows an overflow < 2^16.
 */
#define MAX_OVERFLOW UINT16_MAX //((1 << 16) - 1)
#define CYC_MSEC (1000 * CYCLES_PER_MICROSECOND)
#define TAU_MSEC 20
#define TAU_USEC (TAU_MSEC * 1000)
#define TAU_CYC (TAU_MSEC * CYC_MSEC)
#define SERVO_PRESCALER (TAU_CYC / MAX_OVERFLOW + 1)
#define SERVO_OVERFLOW ((uint16_t)round((double)TAU_CYC / SERVO_PRESCALER))

// Unit conversions
#define US_TO_COMPARE(us) uint16_t(map((us), 0, TAU_USEC, 0, SERVO_OVERFLOW))
#define COMPARE_TO_US(c) uint32_t(map((c), 0, SERVO_OVERFLOW, 0, TAU_USEC))
#define ANGLE_TO_US(a) uint16_t(map((a), minAngle, maxAngle, SERVO_DEFAULT_MIN_PW, SERVO_DEFAULT_MAX_PW))
#define US_TO_ANGLE(us) int16_t(map((us), SERVO_DEFAULT_MIN_PW, SERVO_DEFAULT_MAX_PW, minAngle, maxAngle))

void H32Servo::servoWrite(uint8_t inPin, uint16_t duty_cycle)
{
#ifdef SERVO0_TIMER_NUM
  if (servoIndex == 0)
  {
    pwmSetDuty(duty_cycle);
    return;
  }
#endif
#ifndef TARGET_HC32F46x
  timer_dev *tdev = PIN_MAP[inPin].timer_device;
  uint8_t tchan = PIN_MAP[inPin].timer_channel;
  if (tdev)
    timer_set_compare(tdev, tchan, duty_cycle);
#else
  TIMERA_SetCompareValue(TIMERA_UNIT1, TIMERA_UNIT1_CH_BL, duty_cycle); // release
#endif
}

H32Servo::H32Servo()
{
  servoIndex = ServoCount < MAX_SERVOS ? ServoCount++ : INVALID_SERVO;
#ifndef TARGET_HC32F46x
  timer_set_interrupt_priority(SERVO0_TIMER_NUM, SERVO0_TIMER_IRQ_PRIO);
#endif
}

#ifdef TARGET_HC32F46x
void TimeraUnit1_IrqCallback(void)
{
  TIMERA_ClearFlag(TIMERA_UNIT1, TimeraFlagOverflow);
}
#endif

bool H32Servo::attach(const int32_t inPin, const int32_t inMinAngle, const int32_t inMaxAngle)
{
  if (servoIndex >= MAX_SERVOS)
    return false;
  if (inPin >= BOARD_NR_GPIO_PINS)
    return false;

  minAngle = inMinAngle;
  maxAngle = inMaxAngle;
  angle = -1;

#ifdef SERVO0_TIMER_NUM
  if (servoIndex == 0 && setupSoftPWM(inPin))
  {
    pin = inPin; // set attached()
    return true;
  }
#endif

#ifndef TARGET_HC32F46x

  if (!PWM_PIN(inPin))
    return false;

  timer_dev *tdev = PIN_MAP[inPin].timer_device;
  // uint8_t tchan = PIN_MAP[inPin].timer_channel;

  SET_PWM(inPin);
  servoWrite(inPin, 0);

  timer_pause(tdev);
  timer_set_prescaler(tdev, SERVO_PRESCALER - 1); // prescaler is 1-based
  timer_set_reload(tdev, SERVO_OVERFLOW);
  timer_generate_update(tdev);
  timer_resume(tdev);

  pin = inPin; // set attached()
  return true;
#else
  stc_timera_base_init_t stcTimeraInit;
  stc_timera_compare_init_t stcTimerCompareInit;
  stc_irq_regi_conf_t stcIrqRegiConf;
  stc_timera_hw_startup_cofig_t stcTimeraHwConfig;
  stc_port_init_t stcPortInit;

  uint32_t u32Pclk1;
  stc_clk_freq_t stcClkTmp;

  /* Get pclk1 */
  CLK_GetClockFreq(&stcClkTmp);
  u32Pclk1 = stcClkTmp.pclk1Freq; // 84MHZ

  /* configuration structure initialization */
  MEM_ZERO_STRUCT(stcTimeraInit);
  MEM_ZERO_STRUCT(stcIrqRegiConf);
  MEM_ZERO_STRUCT(stcTimerCompareInit);
  MEM_ZERO_STRUCT(stcTimeraHwConfig);
  MEM_ZERO_STRUCT(stcPortInit);

  /* Configuration peripheral clock */
  PWC_Fcg2PeriphClockCmd(TIMERA_UNIT1_CLOCK, Enable);
  PWC_Fcg0PeriphClockCmd(PWC_FCG0_PERIPH_PTDIS, Enable);

  /* Configuration TIMERA compare pin */
  PORT_SetFunc(TIMERA_UNIT1_CH1_PORT, TIMERA_UNIT1_CH1_PIN, TIMERA_UNIT1_CH1_FUNC, Disable);

  /* Configuration timera unit 1 base structure */
  stcTimeraInit.enClkDiv = TimeraPclkDiv16;
  stcTimeraInit.enCntMode = TimeraCountModeTriangularWave;
  stcTimeraInit.enCntDir = TimeraCountDirUp;
  stcTimeraInit.enSyncStartupEn = Disable;
  stcTimeraInit.u16PeriodVal = (uint16_t)((u32Pclk1 / 16) / (50 * 2)); // freq: 100Hz
  TIMERA_BaseInit(TIMERA_UNIT1, &stcTimeraInit);

  /* Configuration timera unit 1 compare structure */
  stcTimerCompareInit.u16CompareVal = 0;
  stcTimerCompareInit.enStartCountOutput = TimeraCountStartOutputLow;
  stcTimerCompareInit.enStopCountOutput = TimeraCountStopOutputLow;
  stcTimerCompareInit.enCompareMatchOutput = TimeraCompareMatchOutputLow;
  stcTimerCompareInit.enPeriodMatchOutput = TimeraPeriodMatchOutputHigh;
  stcTimerCompareInit.enSpecifyOutput = TimeraSpecifyOutputInvalid;
  stcTimerCompareInit.enCacheEn = Enable;
  stcTimerCompareInit.enTriangularTroughTransEn = Enable;
  stcTimerCompareInit.enTriangularCrestTransEn = Disable;
  stcTimerCompareInit.u16CompareCacheVal = stcTimerCompareInit.u16CompareVal;
  /* Configure Channel 1 */
  TIMERA_CompareInit(TIMERA_UNIT1, TIMERA_UNIT1_CH_BL, &stcTimerCompareInit);
  TIMERA_CompareCmd(TIMERA_UNIT1, TIMERA_UNIT1_CH_BL, Enable);

  /* Enable period count interrupt */
  TIMERA_IrqCmd(TIMERA_UNIT1, TimeraIrqOverflow, Enable);
  /* Interrupt of timera unit 1 */
  stcIrqRegiConf.enIntSrc = TIMERA_UNIT1_OVERFLOW_INT;
  stcIrqRegiConf.enIRQn = Int006_IRQn;
  stcIrqRegiConf.pfnCallback = &TimeraUnit1_IrqCallback;
  enIrqRegistration(&stcIrqRegiConf);
  NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
  NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_15);
  NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);

  /* Sync startup timera unit 2 when timera unit 1 startup */
  TIMERA_Cmd(TIMERA_UNIT1, Enable);
  pin = inPin;
  return true;
#endif
}

bool H32Servo::detach()
{
  if (!attached())
    return false;
  angle = -1;
  servoWrite(pin, 0);
  return true;
}

int32_t H32Servo::read() const
{
  if (attached())
  {
#ifdef SERVO0_TIMER_NUM
    if (servoIndex == 0)
      return angle;
#endif
#ifndef TARGET_HC32F46x
    timer_dev *tdev = PIN_MAP[pin].timer_device;
    uint8_t tchan = PIN_MAP[pin].timer_channel;
    return US_TO_ANGLE(COMPARE_TO_US(timer_get_compare(tdev, tchan)));
#else
    return US_TO_ANGLE(TIMERA_GetCompareValue(TIMERA_UNIT1, TIMERA_UNIT1_CH_BL));
#endif
  }
  return 0;
}

void H32Servo::move(const int32_t value)
{
  constexpr uint16_t servo_delay[] = SERVO_DELAY;
  static_assert(COUNT(servo_delay) == NUM_SERVOS, "SERVO_DELAY must be an array NUM_SERVOS long.");

  if (attached())
  {
    angle = constrain(value, minAngle, maxAngle);
    servoWrite(pin, US_TO_COMPARE(ANGLE_TO_US(angle)));
    safe_delay(servo_delay[servoIndex]);
    TERN_(DEACTIVATE_SERVOS_AFTER_MOVE, detach());
  }
}

#ifdef SERVO0_TIMER_NUM
extern "C" void Servo_IRQHandler()
{
  static timer_dev *tdev = get_timer_dev(SERVO0_TIMER_NUM);
  uint16_t SR = timer_get_status(tdev);
  if (SR & TIMER_SR_CC1IF)
  { // channel 1 off
#ifdef SERVO0_PWM_OD
    OUT_WRITE_OD(SERVO0_PIN, 1); // off
#else
    OUT_WRITE(SERVO0_PIN, 0);
#endif
    timer_reset_status_bit(tdev, TIMER_SR_CC1IF_BIT);
  }
  if (SR & TIMER_SR_CC2IF)
  { // channel 2 resume
#ifdef SERVO0_PWM_OD
    OUT_WRITE_OD(SERVO0_PIN, 0); // on
#else
    OUT_WRITE(SERVO0_PIN, 1);
#endif
    timer_reset_status_bit(tdev, TIMER_SR_CC2IF_BIT);
  }
}

bool H32Servo::setupSoftPWM(const int32_t inPin)
{
  timer_dev *tdev = get_timer_dev(SERVO0_TIMER_NUM);
  if (!tdev)
    return false;
#ifdef SERVO0_PWM_OD
  OUT_WRITE_OD(inPin, 1);
#else
  OUT_WRITE(inPin, 0);
#endif

  timer_pause(tdev);
  timer_set_mode(tdev, 1, TIMER_OUTPUT_COMPARE);       // counter with isr
  timer_oc_set_mode(tdev, 1, TIMER_OC_MODE_FROZEN, 0); // no pin output change
  timer_oc_set_mode(tdev, 2, TIMER_OC_MODE_FROZEN, 0); // no pin output change
  timer_set_prescaler(tdev, SERVO_PRESCALER - 1);      // prescaler is 1-based
  timer_set_reload(tdev, SERVO_OVERFLOW);
  timer_set_compare(tdev, 1, SERVO_OVERFLOW);
  timer_set_compare(tdev, 2, SERVO_OVERFLOW);
  timer_attach_interrupt(tdev, 1, Servo_IRQHandler);
  timer_attach_interrupt(tdev, 2, Servo_IRQHandler);
  timer_generate_update(tdev);
  timer_resume(tdev);

  return true;
}

void H32Servo::pwmSetDuty(const uint16_t duty_cycle)
{
  timer_dev *tdev = get_timer_dev(SERVO0_TIMER_NUM);
  timer_set_compare(tdev, 1, duty_cycle);
  timer_generate_update(tdev);
  if (duty_cycle)
  {
    timer_enable_irq(tdev, 1);
    timer_enable_irq(tdev, 2);
  }
  else
  {
    timer_disable_irq(tdev, 1);
    timer_disable_irq(tdev, 2);
#ifdef SERVO0_PWM_OD
    OUT_WRITE_OD(pin, 1); // off
#else
    OUT_WRITE(pin, 0);
#endif
  }
}

void H32Servo::pauseSoftPWM()
{ // detach
  timer_dev *tdev = get_timer_dev(SERVO0_TIMER_NUM);
  timer_pause(tdev);
  pwmSetDuty(0);
}

#else

bool H32Servo::setupSoftPWM(const int32_t inPin)
{
  return false;
}
void H32Servo::pwmSetDuty(const uint16_t duty_cycle) {}
void H32Servo::pauseSoftPWM() {}

#endif

#endif // HAS_SERVOS
#endif // TARGET_HC32F46x
