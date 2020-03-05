/*!
 * @file
 * @brief
 */

#include "line_detected.h"
#include "tactic.h"
#include "tiny_utils.h"
#include "tuning.h"

static void stop_running_timer_expired(tiny_timer_group_t* group, void* context) {
  reinterpret(self, context, line_detected_t*);
  (void)group;
  uint8_t stopped;
  tiny_key_value_store_read(
    self->key_value_store,
    self->keys->key_tactic_stopped,
    &stopped);

  stopped++;

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

  if(self->left_line_was_detected) {
    left_power = line_detection_near_wheel_power;
    right_power = line_detection_far_wheel_power;
  }
  else {
    left_power = line_detection_far_wheel_power;
    right_power = line_detection_near_wheel_power;
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
    line_detection_turn_time,
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
      self->left_line_was_detected = the_left_line_was_detected(self);
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
        line_detection_back_up_time,
        back_up_timer_expired,
        self);
    }
  }
}

void line_detected_init(
  line_detected_t* self,
  i_tiny_key_value_store_t* key_value_store,
  const line_detected_keys_t* keys,
  tiny_timer_group_t* timer_group) {
  self->key_value_store = key_value_store;
  self->keys = keys;
  self->timer_group = timer_group;

  tiny_event_subscription_init(&self->on_change_subscription, self, data_changed);
  tiny_event_subscribe(tiny_key_value_store_on_change(key_value_store), &self->on_change_subscription);
}
