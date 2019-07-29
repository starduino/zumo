/*!
 * @file
 * @brief
 */

#include "system_tick.h"
#include "stm8.h"

static i_tiny_time_source_t instance;
static volatile tiny_time_source_ticks_t ticks;

void system_tick_isr(void) __interrupt(TIM2_OVR_UIF_IRQ) {
  TIM2_SR1 &= ~TIM_SR1_UIF;
  ticks++;
}

static tiny_time_source_ticks_t _ticks(i_tiny_time_source_t* self) __critical {
  (void)self;
  return ticks;
}

static const i_tiny_time_source_api_t api = { _ticks };

i_tiny_time_source_t* system_tick_init(void) {
  // 16,000,000 / 2 ^ $PSCR) / ($ARR + 1) = 1000
  // => $PSCR = 2
  // => $ARR = 399 = 0xF9F
  TIM2_PSCR = 2;
  TIM2_ARRH = 0x0F;
  TIM2_ARRL = 0x9F;

  TIM2_IER |= TIM_IER_UIE;
  TIM2_CR1 |= TIM_CR1_CEN;

  instance.api = &api;
  return &instance;
}
