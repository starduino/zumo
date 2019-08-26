/*!
 * @file
 * @brief
 *
 * PC6          => Left motor direction
 * PD3/TIM2_CH2 => Left motor PWM
 *
 * PC7          => Right motor direction
 * PA3/TIM2_CH3 => Right motor PWM
 */

#ifndef motors_plugin_h
#define motors_plugin_h

#include "i_tiny_key_value_store.h"

typedef struct {
  tiny_event_subscription_t on_change_subscription;
} motors_plugin_t;

void motors_plugin_init(motors_plugin_t* self, i_tiny_key_value_store_t* key_value_store);

#endif
