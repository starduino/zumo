/*!
 * @file
 * @brief
 */

#include "line_detected.h"
#include "motor_power.h"
#include "tactic.h"
#include "tiny_utils.h"

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
    }
  }
}

void line_detected_init(
  line_detected_t* self,
  i_tiny_key_value_store_t* key_value_store,
  const line_detected_keys_t* keys) {
  self->key_value_store = key_value_store;
  self->keys = keys;

  tiny_event_subscription_init(&self->on_change_subscription, self, data_changed);
  tiny_event_subscribe(tiny_key_value_store_on_change(key_value_store), &self->on_change_subscription);
}
