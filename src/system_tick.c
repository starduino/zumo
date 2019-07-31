/*!
 * @file
 * @brief
 */

#include "system_tick.h"

static i_tiny_time_source_t self;
static volatile tiny_time_source_ticks_t ticks;

void system_tick_isr(void) __interrupt(ITC_IRQ_TIM2_OVF) {
  TIM2->SR1 &= ~TIM2_SR1_UIF;
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
  TIM2->PSCR = 2;
  TIM2->ARRH = 0x0F;
  TIM2->ARRL = 0x9F;

  TIM2->IER |= TIM2_IER_UIE;
  TIM2->CR1 |= TIM2_CR1_CEN;

  self.api = &api;
  return &self;
}
