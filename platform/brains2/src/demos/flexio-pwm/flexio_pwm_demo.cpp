#include "lib/board_init.h"

#include "fsl_device_registers.h"
#include "fsl_flexio.h"

typedef enum _pin_mux_direction {
  kPIN_MUX_DirectionInput = 0U,        /* Input direction */
  kPIN_MUX_DirectionOutput = 1U,       /* Output direction */
  kPIN_MUX_DirectionInputOrOutput = 2U /* Input or output direction */
} pin_mux_direction_t;

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_TIME_DELAY_FOR_DUTY_CYCLE_UPDATE (2000000U)
#define DEMO_FLEXIO_BASEADDR FLEXIO1
#define DEMO_FLEXIO_OUTPUTPIN                                                  \
  (26U)                           /* Select FLEXIO1_FLEXIO05 as PWM output     \
                                   */
#define DEMO_FLEXIO_TIMER_CH (0U) /* Flexio timer0 used */

/* Select USB1 PLL (480 MHz) as flexio clock source */
#define FLEXIO_CLOCK_SELECT (3U)
/* Clock pre divider for flexio clock source */
#define FLEXIO_CLOCK_PRE_DIVIDER (4U)
/* Clock divider for flexio clock source */
#define FLEXIO_CLOCK_DIVIDER (7U)
#define DEMO_FLEXIO_CLOCK_FREQUENCY                                            \
  (CLOCK_GetFreq(kCLOCK_Usb1PllClk) / (FLEXIO_CLOCK_PRE_DIVIDER + 1U) /        \
   (FLEXIO_CLOCK_DIVIDER + 1U))
/* FLEXIO output PWM frequency */
#define DUTY_CYCLE (50)
#define FLEXIO_MAX_FREQUENCY (DEMO_FLEXIO_CLOCK_FREQUENCY / 2U)
#define FLEXIO_MIN_FREQUENCY (DEMO_FLEXIO_CLOCK_FREQUENCY / 512U)
// #define DEMO_FLEXIO_FREQUENCY (48000U)
#define DEMO_FLEXIO_FREQUENCY (FLEXIO_MIN_FREQUENCY)

/* flexio timer number */
#define FLEXIO_TIMER_CHANNELS (8)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Configures the timer as a 8-bits PWM mode to generate the PWM waveform
 *
 * @param freq_Hz PWM frequency in hertz, range is [FLEXIO_MIN_FREQUENCY,
 * FLEXIO_MAX_FREQUENCY]
 * @param duty Specified duty in unit of %, with a range of [0, 100]
 */
static status_t flexio_pwm_init(uint32_t freq_Hz, uint32_t duty);

/*!
 * @brief Set PWM output in idle status (high or low).
 *
 * @param base               FlexIO peripheral base address
 * @param timerChannel       FlexIO timer channel
 * @param idleStatus         True: PWM output is high in idle status; false: PWM
 * output is low in idle status
 */
static void FLEXIO_SetPwmOutputToIdle(FLEXIO_Type *base, uint8_t timerChannel,
                                      bool idleStatus);

/*!
 * @brief Get pwm duty cycle value.
 */
static uint8_t s_flexioGetPwmDutyCycle[FLEXIO_TIMER_CHANNELS] = {0};

/*******************************************************************************
 * Variables
 *******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
static status_t flexio_pwm_init(const uint32_t freq_Hz, const uint32_t duty) {
  assert((freq_Hz < FLEXIO_MAX_FREQUENCY) && (freq_Hz > FLEXIO_MIN_FREQUENCY));

  uint32_t lowerValue =
      0; /* Number of clock cycles in high logic state in one period */
  uint32_t upperValue =
      0; /* Number of clock cycles in low logic state in one period */
  uint32_t sum = 0; /* Number of clock cycles in one period */
  flexio_timer_config_t fxioTimerConfig;

  /* Configure the timer DEMO_FLEXIO_TIMER_CH for generating PWM */
  fxioTimerConfig.triggerSelect = FLEXIO_TIMER_TRIGGER_SEL_SHIFTnSTAT(0U);
  fxioTimerConfig.triggerSource = kFLEXIO_TimerTriggerSourceInternal;
  fxioTimerConfig.triggerPolarity = kFLEXIO_TimerTriggerPolarityActiveLow;
  fxioTimerConfig.pinConfig = kFLEXIO_PinConfigOutput;
  fxioTimerConfig.pinPolarity = kFLEXIO_PinActiveHigh;
  fxioTimerConfig.pinSelect = DEMO_FLEXIO_OUTPUTPIN; /* Set pwm output */
  fxioTimerConfig.timerMode = kFLEXIO_TimerModeDisabled;
  fxioTimerConfig.timerOutput = kFLEXIO_TimerOutputOneNotAffectedByReset;
  fxioTimerConfig.timerDecrement =
      kFLEXIO_TimerDecSrcOnFlexIOClockShiftTimerOutput;
  fxioTimerConfig.timerDisable = kFLEXIO_TimerDisableNever;
  fxioTimerConfig.timerEnable = kFLEXIO_TimerEnabledAlways;
  fxioTimerConfig.timerReset = kFLEXIO_TimerResetNever;
  fxioTimerConfig.timerStart = kFLEXIO_TimerStartBitDisabled;
  fxioTimerConfig.timerStop = kFLEXIO_TimerStopBitDisabled;

  /* Calculate timer lower and upper values of TIMCMP */
  /* Calculate the nearest integer value for sum, using formula round(x) = (2 *
   * floor(x) + 1) / 2 */
  /* sum = DEMO_FLEXIO_CLOCK_FREQUENCY / freq_H */
  sum = (DEMO_FLEXIO_CLOCK_FREQUENCY * 2 / freq_Hz + 1) / 2;

  /* Calculate the nearest integer value for lowerValue, the high period of the
   * pwm output */
  lowerValue = (sum * duty) / 100;
  /* Calculate upper value, the low period of the pwm output */
  upperValue = sum - lowerValue - 2;

  fxioTimerConfig.timerCompare = ((upperValue << 8U) | (lowerValue));

  if ((duty > 0) && (duty < 100)) {
    /* Set Timer mode to kFLEXIO_TimerModeDual8BitPWM to start timer */
    fxioTimerConfig.timerMode = kFLEXIO_TimerModeDual8BitPWM;
  } else if (duty == 100) {
    fxioTimerConfig.pinPolarity = kFLEXIO_PinActiveLow;
  } else if (duty == 0) {
    /* Set high level as active level */
    fxioTimerConfig.pinPolarity = kFLEXIO_PinActiveHigh;
  } else {
    return kStatus_Fail;
  }

  FLEXIO_SetTimerConfig(DEMO_FLEXIO_BASEADDR, DEMO_FLEXIO_TIMER_CH,
                        &fxioTimerConfig);

  s_flexioGetPwmDutyCycle[DEMO_FLEXIO_TIMER_CH] = duty;

  return kStatus_Success;
}

/*!
 * brief Set PWM output in idle status (high or low).
 *
 * param base               FlexIO peripheral base address
 * param timerChannel       FlexIO timer channel
 * param idleStatus         True: PWM output is high in idle status; false: PWM
 * output is low in idle status
 */
static void FLEXIO_SetPwmOutputToIdle(FLEXIO_Type *base, uint8_t timerChannel,
                                      bool idleStatus) {
  flexio_timer_config_t fxioTimerConfig;

  /* Configure the timer DEMO_FLEXIO_TIMER_CH for generating PWM */
  fxioTimerConfig.triggerSelect = FLEXIO_TIMER_TRIGGER_SEL_SHIFTnSTAT(0U);
  fxioTimerConfig.triggerSource = kFLEXIO_TimerTriggerSourceInternal;
  fxioTimerConfig.triggerPolarity = kFLEXIO_TimerTriggerPolarityActiveLow;
  fxioTimerConfig.pinConfig = kFLEXIO_PinConfigOutput;
  fxioTimerConfig.pinSelect = DEMO_FLEXIO_OUTPUTPIN; /* Set pwm output */
  fxioTimerConfig.timerMode = kFLEXIO_TimerModeDisabled;
  fxioTimerConfig.timerOutput = kFLEXIO_TimerOutputOneNotAffectedByReset;
  fxioTimerConfig.timerDecrement =
      kFLEXIO_TimerDecSrcOnFlexIOClockShiftTimerOutput;
  fxioTimerConfig.timerDisable = kFLEXIO_TimerDisableNever;
  fxioTimerConfig.timerEnable = kFLEXIO_TimerEnabledAlways;
  fxioTimerConfig.timerReset = kFLEXIO_TimerResetNever;
  fxioTimerConfig.timerStart = kFLEXIO_TimerStartBitDisabled;
  fxioTimerConfig.timerStop = kFLEXIO_TimerStopBitDisabled;
  fxioTimerConfig.timerCompare = 0U;

  /* Clear TIMCMP register */
  base->TIMCMP[timerChannel] = 0;

  if (idleStatus) {
    /* Set low level as active level */
    fxioTimerConfig.pinPolarity = kFLEXIO_PinActiveLow;
  } else {
    /* Set high level as active level */
    fxioTimerConfig.pinPolarity = kFLEXIO_PinActiveHigh;
  }

  FLEXIO_SetTimerConfig(DEMO_FLEXIO_BASEADDR, timerChannel, &fxioTimerConfig);

  s_flexioGetPwmDutyCycle[timerChannel] = 0;
}

/*!
 * @brief Main function
 */
int main(void) {
  board_init();

  flexio_config_t fxioUserConfig;

  /* Clock setting for Flexio */
  CLOCK_SetMux(kCLOCK_Flexio1Mux, FLEXIO_CLOCK_SELECT);
  CLOCK_SetDiv(kCLOCK_Flexio1PreDiv, FLEXIO_CLOCK_PRE_DIVIDER);
  CLOCK_SetDiv(kCLOCK_Flexio1Div, FLEXIO_CLOCK_DIVIDER);

  /* Init flexio, use default configure
   * Disable doze and fast access mode
   * Enable in debug mode
   */
  FLEXIO_GetDefaultConfig(&fxioUserConfig);
  FLEXIO_Init(DEMO_FLEXIO_BASEADDR, &fxioUserConfig);

  // PRINTF("\r\nFLEXIO_PWM demo start.\r\n");

  if (flexio_pwm_init(DEMO_FLEXIO_FREQUENCY, DUTY_CYCLE) == kStatus_Fail) {
    // PRINTF("FLEXIO PWM initialization failed\n");
    return -1;
  }

  while (1) {
  }
}
