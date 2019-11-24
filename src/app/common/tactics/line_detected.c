/*!
 * @file
 * @brief
 */

#include "line_detected.h"
#include "tactic.h"
#include "tiny_utils.h"

static void stop_running_timer_expired(tiny_timer_group_t* group, void* context) {
  reinterpret(self, context, line_detected_t*);
  (void)group;
  bool stopped = true;

  tiny_key_value_store_write(
    self->key_value_store,
    self->keys->key_tactic_stopped,
    &stopped);
}

static bool the_left_line_was_detected(line_detected_t* self) {
  bool detected;
  tiny_key_value_store_read(
    self->key_value_store,
    self->keys->key_left_line_detected,
    &detected);
  return detected;
}

static void back_up_timer_expired(tiny_timer_group_t* group, void* context) {
  (void)group;
  reinterpret(self, context, line_detected_t*);
  motor_power_t left_power;
  motor_power_t right_power;

  if(the_left_line_was_detected(self)) {
    left_power = 100;
    right_power = 30;
  }
  else {
    left_power = 30;
    right_power = 100;
  }

  tiny_key_value_store_write(
    self->key_value_store,
    self->keys->key_left_motor,
    &left_power);

  tiny_key_value_store_write(
    self->key_value_store,
    self->keys->key_right_motor,
    &right_power);

  tiny_timer_start(
    self->timer_group,
    &self->stop_running_timer,
    self->config->turn_time,
    stop_running_timer_expired,
    self);
}

static void data_changed(void* context, const void* _args) {
  reinterpret(self, context, line_detected_t*);
  reinterpret(args, _args, const tiny_key_value_store_on_change_args_t*);

  if(args->key == self->keys->key_tactic) {
    reinterpret(tactic, args->value, const tactic_t*);

    if(*tactic == tactic_avoid_line) {
      motor_power_t power = motor_power_min;
      tiny_key_value_store_write(
        self->key_value_store,
        self->keys->key_left_motor,
        &power);
      tiny_key_value_store_write(
        self->key_value_store,
        self->keys->key_right_motor,
        &power);

      tiny_timer_start(
        self->timer_group,
        &self->back_up_timer,
        self->config->back_up_time,
        back_up_timer_expired,
        self);
    }
  }
}

void line_detected_init(
  line_detected_t* self,
  i_tiny_key_value_store_t* key_value_store,
  const line_detected_keys_t* keys,
  const line_detected_config_t* config,
  tiny_timer_group_t* timer_group) {
  self->key_value_store = key_value_store;
  self->keys = keys;
  self->timer_group = timer_group;
  self->config = config;

  tiny_event_subscription_init(&self->on_change_subscription, self, data_changed);
  tiny_event_subscribe(tiny_key_value_store_on_change(key_value_store), &self->on_change_subscription);
}
