/*!
 * @file
 * @brief
 */

#ifndef line_detected_h
#define line_detected_h

#include "enemy_location.h"
#include "i_tiny_key_value_store.h"
#include "tiny_event_subscription.h"
#include "tiny_fsm.h"
#include "tiny_timer.h"
#include "motor_power.h"

typedef struct {
  tiny_key_value_store_key_t key_left_motor;
  tiny_key_value_store_key_t key_right_motor;
  tiny_key_value_store_key_t key_tactic;
  tiny_key_value_store_key_t key_left_line_detected;
  tiny_key_value_store_key_t key_right_line_detected;
  tiny_key_value_store_key_t key_tactic_stopped;
} line_detected_keys_t;

typedef struct {
  motor_power_t near_wheel_power;
  motor_power_t far_wheel_power;
  tiny_timer_ticks_t back_up_time;
  tiny_timer_ticks_t turn_time;
} line_detected_config_t;

typedef struct {
  tiny_event_subscription_t on_change_subscription;
  i_tiny_key_value_store_t* key_value_store;
  const line_detected_keys_t* keys;
  const line_detected_config_t* config;
  tiny_timer_t back_up_timer;
  tiny_timer_t stop_running_timer;
  tiny_timer_group_t* timer_group;
} line_detected_t;

void line_detected_init(
  line_detected_t* self,
  i_tiny_key_value_store_t* key_value_store,
  const line_detected_keys_t* keys,
  const line_detected_config_t* config,
  tiny_timer_group_t* timer_group);

#endif
