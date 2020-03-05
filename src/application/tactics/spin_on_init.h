/*!
 * @file
 * @brief
 */

#ifndef spin_on_init_h
#define spin_on_init_h

#include "enemy_location.h"
#include "i_tiny_key_value_store.h"
#include "tiny_event_subscription.h"
#include "tiny_fsm.h"
#include "tiny_timer.h"
#include "motor_power.h"

typedef struct {
  tiny_key_value_store_key_t left_motor;
  tiny_key_value_store_key_t right_motor;
  tiny_key_value_store_key_t knob_counts;
  tiny_key_value_store_key_t tactic;
  tiny_key_value_store_key_t tactic_stopped;
} spin_on_init_keys_t;

typedef struct {
  tiny_event_subscription_t on_change_subscription;
  i_tiny_key_value_store_t* key_value_store;
  const spin_on_init_keys_t* keys;
  tiny_timer_t back_up_timer;
  tiny_timer_t stop_running_timer;
  tiny_timer_group_t* timer_group;
} spin_on_init_t;

void spin_on_init_init(
  spin_on_init_t* self,
  i_tiny_key_value_store_t* key_value_store,
  const spin_on_init_keys_t* keys,
  tiny_timer_group_t* timer_group);

#endif
