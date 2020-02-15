/*!
 * @file
 * @brief
 */

#include "stm8s.h"
#include "stm8s_tim1.h"
#include "stm8s_gpio.h"
#include "stm8s_clk.h"
#include "line_sensors_plugin.h"
#include "data_model.h"
#include "tiny_utils.h"

enum {
  setup_period = 2,
  sample_period = 9,
  input_filter_8 = (8 << 4),
  pin_3 = (1 << 3),
  pin_4 = (1 << 4),
  line_threshold = 10000
};

static void start_charge(void) {
  // Configure pins as push pull outputs, output high
  GPIOC->CR1 |= pin_3 | pin_4;
  GPIOC->ODR |= pin_3 | pin_4;
  GPIOC->DDR |= pin_3 | pin_4;
}

static void stop_charge(void) {
  // Configure pins as floating inputs
  GPIOC->CR1 &= ~(pin_3 | pin_4);
  GPIOC->DDR &= ~(pin_3 | pin_4);
}

static void start_capture(void) {
  // Enable capture on channels 1 and 2
  TIM1->CCER2 |= TIM1_CCER2_CC3E | TIM1_CCER2_CC4E;

  // Reset counter
  TIM1->CNTRH = 0;
  TIM1->CNTRL = 0;

  // Start counting
  TIM1->CR1 |= TIM1_CR1_CEN;
}

static void end_capture(void) {
  // Disable capture on channels 1 and 2
  TIM1->CCER2 &= ~(TIM1_CCER2_CC3E | TIM1_CCER2_CC4E);

  // Stop counting
  TIM1->CR1 &= ~TIM1_CR1_CEN;
}

static volatile uint16_t a;
static volatile uint16_t b;
static void measure(line_sensors_plugin_t* self) {
  uint16_t left_count = (TIM1->CCR3H << 8) + TIM1->CCR3L;
  bool left_line_detected = left_count < line_threshold;
  tiny_key_value_store_write(self->key_value_store, key_left_line_detected, &left_line_detected);

  uint16_t right_count = (TIM1->CCR4H << 8) + TIM1->CCR4L;
  bool right_line_detected = right_count < line_threshold;
  tiny_key_value_store_write(self->key_value_store, key_right_line_detected, &right_line_detected);

  a = left_count;
  b = right_count;
  bool line_detected = left_line_detected || right_line_detected;
  tiny_key_value_store_write(self->key_value_store, key_line_detected, &line_detected);
}

static void sample(tiny_timer_group_t* timer_group, void* context);

static void setup(tiny_timer_group_t* timer_group, void* context) {
  reinterpret(self, context, line_sensors_plugin_t*);

  end_capture();
  measure(self);
  start_charge();

  tiny_timer_start(timer_group, &self->timer, setup_period, sample, self);
}

static void sample(tiny_timer_group_t* timer_group, void* context) {
  reinterpret(self, context, line_sensors_plugin_t*);

  stop_charge();
  start_capture();

  tiny_timer_start(timer_group, &self->timer, sample_period, setup, self);
}

static void initialize_tim1(void) {
  // Un-gate clock for TIM1
  CLK->PCKENR1 |= (1 << CLK_PERIPHERAL_TIMER1);

  // Prescalar 4, 4 MHz (rolls over after ~16 msec)
  TIM1->PSCRH = 0;
  TIM1->PSCRL = 4;

  // Count up, no auto-reload, update event (UEV) disabled
  TIM1->CR1 = TIM1_CR1_UDIS;
}

static void initialize_tim1_channel3(void) {
  // Direct input capture, filter of 8, no prescalar
  TIM1->CCMR3 = TIM1_ICSELECTION_DIRECTTI | input_filter_8;

  // Trigger on falling edge
  TIM1->CCER2 |= TIM1_CCER2_CC3P;
}

static void initialize_tim1_channel2(void) {
  // Direct input capture, filter of 8, no prescalar
  TIM1->CCMR4 = TIM1_ICSELECTION_DIRECTTI | input_filter_8;

  // Trigger on falling edge
  TIM1->CCER2 |= TIM1_CCER2_CC4P;
}

void line_sensors_plugin_init(
  line_sensors_plugin_t* self,
  i_tiny_key_value_store_t* key_value_store,
  tiny_timer_group_t* timer_group) {
  self->key_value_store = key_value_store;

  initialize_tim1();
  initialize_tim1_channel3();
  initialize_tim1_channel2();

  setup(timer_group, self);
}
