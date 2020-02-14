/*!
 * @file
 * @brief
 */

#include "stm8s.h"
#include "stm8s_clk.h"
#include "stm8s_tim3.h"
#include "buzzer_plugin.h"
#include "data_model.h"
#include "tiny_utils.h"

static void data_changed(void* context, const void* _args) {
  (void)context;

  reinterpret(args, _args, const tiny_key_value_store_on_change_args_t*);
  reinterpret(frequency, args->value, const buzzer_frequency_t*);

  if(args->key == key_buzzer_frequency) {
    if(*frequency) {
      uint16_t period = 31250 / *frequency;

      // Set frequency via reload register
      TIM3->ARRH = period >> 8;
      TIM3->ARRL = period & 0xFF;

      uint16_t half_period = period / 2;

      // 50% duty cycle
      TIM3->CCR1H = half_period >> 8;
      TIM3->CCR1L = half_period & 0xFF;
    }
    else {
      // 0% duty cycle
      TIM3->CCR1H = 0;
      TIM3->CCR1L = 0;
    }
  }
}

static void initialize_tim3(void) {
  // Un-gate clock for TIM3
  CLK->PCKENR1 |= (1 << CLK_PERIPHERAL_TIMER3);

  // Prescalar == 512, 31.25 KHz
  TIM3->PSCR = TIM3_PRESCALER_512;

  // Default to 0% duty cycle
  TIM3->CCR1H = 0;
  TIM3->CCR1L = 0;

  // PWM mode 1
  // Use shadow register for CCMR
  TIM3->CCMR1 = TIM3_OCMODE_PWM1 | TIM3_CCMR_OCxPE;

  // Enable output for channel 1
  TIM3->CCER1 |= TIM3_CCER1_CC1E;

  // Enable TIM3
  // Use shadow register for ARR in case we unlock frequency
  TIM3->CR1 = TIM3_CR1_CEN | TIM3_CR1_ARPE;
}

void buzzer_plugin_init(buzzer_plugin_t* self, i_tiny_key_value_store_t* key_value_store) {
  initialize_tim3();

  tiny_event_subscription_init(&self->on_change_subscription, self, data_changed);
  tiny_event_subscribe(tiny_key_value_store_on_change(key_value_store), &self->on_change_subscription);
}
