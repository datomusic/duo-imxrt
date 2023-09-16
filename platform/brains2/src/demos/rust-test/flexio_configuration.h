#include "fsl_device_registers.h"
#include "fsl_flexio.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

const uint32_t CLOCK_DIVIDER =
    10; // Timer toggles; meaning we need two cycles for one timer clock cycle,
        // so this is half the total cycle length
const uint8_t LOW_BIT_CYCLES_ON = 5;
const uint8_t HIGH_BIT_CYCLES_ON = 15;

// Newest WS2812 Version requires 300us latch time
const uint16_t LATCH_DELAY_PIXELS = 240;

const uint8_t CYCLE_LENGTH = CLOCK_DIVIDER * 2;
const uint8_t LOW_BIT_CYCLES_OFF = CYCLE_LENGTH - LOW_BIT_CYCLES_ON;
const uint8_t HIGH_BIT_CYCLES_OFF = CYCLE_LENGTH - HIGH_BIT_CYCLES_ON;
const uint16_t LATCH_DELAY = CYCLE_LENGTH * LATCH_DELAY_PIXELS;

void configure_flexio_clock() {
  // Generate 16MHz clock
  CLOCK_SetMux(kCLOCK_Flexio1Mux, kCLOCK_PerClk);
  CLOCK_SetDiv(kCLOCK_Flexio1PreDiv, kCLOCK_DivideBy5);
  CLOCK_SetDiv(kCLOCK_Flexio1Div, kCLOCK_DivideBy6);
}

void configure_low_bit_timer(const uint8_t timer_id,
                             const uint8_t shift_timer_pin,
                             const uint8_t output_pin) {
  // timer_id setup begin (dual 8-bit counters PWM mode)
  //
  // a rising edge on ws2812_generator_clk enables this timer;
  // when enabled this timer runs for 1 WS2812 bit period
  // generating the WS2812 "0 code" waveform and then it stops;
  // ws2812_generator_clk is the trigger
  //=========================================================
  // FLEXIO1->TIMCTL[timer_id] = 0; // disable timer

  FLEXIO1->TIMCMP[timer_id] =
      ((LOW_BIT_CYCLES_OFF - 1) << 8) | ((LOW_BIT_CYCLES_ON - 1));

  FLEXIO1->TIMCTL[timer_id] =
      FLEXIO_TIMCTL_TRGSEL(shift_timer_pin * 2) |
      FLEXIO_TIMCTL_TRGPOL(0) | // trigger polarity: active high
      FLEXIO_TIMCTL_TRGSRC(1) | // trigger source: internal
      FLEXIO_TIMCTL_PINCFG(3) | // timer pin output
      FLEXIO_TIMCTL_PINSEL(output_pin) |
      FLEXIO_TIMCTL_PINPOL(0) | // pin is active high
      FLEXIO_TIMCTL_TIMOD(2);   // dual 8-bit counters PWM mode
                                //
  FLEXIO1->TIMCFG[timer_id] =
      // timer outputs 1 when enabled and not affected by timer reset
      FLEXIO_TIMCFG_TIMOUT(0) |
      FLEXIO_TIMCFG_TIMDEC(0) | // decrement on FlexIO clock
      FLEXIO_TIMCFG_TIMRST(0) | // never reset
      FLEXIO_TIMCFG_TIMDIS(2) | // disable on compare
      FLEXIO_TIMCFG_TIMENA(6) | // enable on trigger rising edge
      FLEXIO_TIMCFG_TSTOP(0) |  // stop bit disabled
      FLEXIO_TIMCFG_TSTART(0);  // start bit disabled
}

void configure_high_bit_timer(const uint8_t timer_id, const uint8_t shift_pin,
                              const uint8_t output_pin) {
  // timer_1 setup begin (dual 8-bit counters PWM mode)
  //
  // a rising edge on ws2812_generator_data enables this timer and
  // a falling edge on ws2812_generator_data disables this timer;
  // when enabled this timer generates back-to-back WS2812 "1 code"
  // waveform;
  // ws2812_generator_data is the trigger
  //=========================================================
  // FLEXIO1->TIMCTL[timer_id] = 0; // disable timer

  FLEXIO1->TIMCMP[timer_id] =
      ((HIGH_BIT_CYCLES_OFF - 1) << 8) | ((HIGH_BIT_CYCLES_ON - 1));

  FLEXIO1->TIMCTL[timer_id] =
      FLEXIO_TIMCTL_TRGSEL(shift_pin * 2) |
      FLEXIO_TIMCTL_TRGPOL(0) | // trigger polarity: active high
      FLEXIO_TIMCTL_TRGSRC(1) | // trigger source: internal
      FLEXIO_TIMCTL_PINCFG(3) | // timer pin output
      FLEXIO_TIMCTL_PINSEL(output_pin) |
      FLEXIO_TIMCTL_PINPOL(0) | // pin is active high
      FLEXIO_TIMCTL_TIMOD(2);   // dual 8-bit counters PWM mode

  FLEXIO1->TIMCFG[timer_id] =
      // timer outputs 1 when enabled and not affected by timer reset
      FLEXIO_TIMCFG_TIMOUT(0) |
      FLEXIO_TIMCFG_TIMDEC(0) | // decrement on FlexIO clock
      FLEXIO_TIMCFG_TIMRST(0) | // never reset
      FLEXIO_TIMCFG_TIMDIS(6) | // disable on trigger falling edge
      FLEXIO_TIMCFG_TIMENA(6) | // enable on trigger rising edge
      FLEXIO_TIMCFG_TSTOP(0) |  // stop bit disabled
      FLEXIO_TIMCFG_TSTART(0);  // start bit disabled
}

void configure_shifter(const uint8_t shifter_id, const uint8_t input_timer,
                       const uint8_t output_start_pin) {

  FLEXIO1->SHIFTCTL[shifter_id] =
      FLEXIO_SHIFTCTL_TIMSEL(input_timer) |
      FLEXIO_SHIFTCTL_TIMPOL(0) | // shift on posedge of shift clock
      FLEXIO_SHIFTCTL_PINCFG(3) | // shifter_id pin output
      FLEXIO_SHIFTCTL_PINSEL(output_start_pin) |
      FLEXIO_SHIFTCTL_PINPOL(0) | // pin is active high
      FLEXIO_SHIFTCTL_SMOD(2);    // shifter_id mode: transmitter

  FLEXIO1->SHIFTCFG[shifter_id] =
      FLEXIO_SHIFTCFG_PWIDTH(3) | // NA
      FLEXIO_SHIFTCFG_INSRC(0) |  // NA
      FLEXIO_SHIFTCFG_SSTOP(0) |  // STOP bit disabled
      FLEXIO_SHIFTCFG_SSTART(1); // START bit disabled; load data on first shift
}

void configure_shift_timer(const uint8_t timer_id, const uint8_t shifter_id,
                           const uint8_t output_pin) {
  const uint32_t CYCLES_PER_SHIFTBUFFER = 16;

  FLEXIO1->TIMCMP[timer_id] =
      ((CYCLES_PER_SHIFTBUFFER - 1) << 8) | (CLOCK_DIVIDER - 1);

  FLEXIO1->TIMCTL[timer_id] =
      FLEXIO_TIMCTL_TRGSEL((uint32_t)(shifter_id * 4) + 1) |
      FLEXIO_TIMCTL_TRGSRC(1) | // internal trigger
      FLEXIO_TIMCTL_PINCFG(3) | // timer pin output
      FLEXIO_TIMCTL_PINPOL(0) | // output active high
      FLEXIO_TIMCTL_TIMOD(1) |  // dual 8-bit counter baud/bit mode
      FLEXIO_TIMCTL_PINSEL((uint32_t)output_pin) |
      FLEXIO_TIMCTL_TRGPOL(1); // trigger active low

  FLEXIO1->TIMCFG[timer_id] =
      FLEXIO_TIMCFG_TIMOUT(1) | // Zero when enabled, not affected by reset
      FLEXIO_TIMCFG_TIMDEC(0) | // Input clock from FlexIO clock
      FLEXIO_TIMCFG_TIMRST(0) | // never reset
      FLEXIO_TIMCFG_TIMDIS(2) | // disable on timer compare
      FLEXIO_TIMCFG_TIMENA(2) | // enabled on trigger high
      FLEXIO_TIMCFG_TSTOP(0) |  // STOP bit disabled
      FLEXIO_TIMCFG_TSTART(0);  // START bit disabled
}

void configure_idle_timer(const uint8_t timer_id,
                          const uint8_t shift_timer_pin) {
  const uint8_t CYCLE_LENGTH = CLOCK_DIVIDER * 2;
  const uint16_t LATCH_DELAY = CYCLE_LENGTH * LATCH_DELAY_PIXELS;

  FLEXIO1->TIMCMP[timer_id] = LATCH_DELAY;

  FLEXIO1->TIMCTL[timer_id] =
      FLEXIO_TIMCTL_TRGSEL(shift_timer_pin * 2) |
      FLEXIO_TIMCTL_TRGPOL(0) | // trigger polarity: active high
      FLEXIO_TIMCTL_TRGSRC(1) | // trigger source: internal
      FLEXIO_TIMCTL_PINSEL(0) | // timer pin output enbled
      FLEXIO_TIMCTL_PINCFG(0) | // timer pin output disabled
      FLEXIO_TIMCTL_PINPOL(0) | // pin polarity: NA
      FLEXIO_TIMCTL_TIMOD(3);   // dual 8-bit counter baud/bit mode
                                //
  FLEXIO1->TIMCFG[timer_id] =
      FLEXIO_TIMCFG_TIMOUT(2) | // timer outputs logic 1 when enabled and not
                                // affected by timer reset
      FLEXIO_TIMCFG_TIMDEC(0) | // decrement on FlexIO clock
      FLEXIO_TIMCFG_TIMRST(6) | // reset on trigger rising edge
      FLEXIO_TIMCFG_TIMDIS(2) | // disable on compare
      FLEXIO_TIMCFG_TIMENA(6) | // enable on trigger rising edge
      FLEXIO_TIMCFG_TSTOP(0) |  // stop bit disabled
      FLEXIO_TIMCFG_TSTART(0);  // start bit disabled
}

#ifdef __cplusplus
}
#endif
