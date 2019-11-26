/*!
 * @file
 * @brief
 */

#include "seeking.h"
#include "tactic.h"
#include "tiny_utils.h"
#include "motor_power.h"

enum {
  near_wheel_power = 30,
  far_wheel_power = 100
};

static void set_right_motor_to(seeking_t* self, motor_power_t power) {
  tiny_key_value_store_write(self->key_value_store, self->keys->right_motor, &power);
}

static void set_left_motor_to(seeking_t* self, motor_power_t power) {
  tiny_key_value_store_write(self->key_value_store, self->keys->left_motor, &power);
}

static void data_changed(void* context, const void* _args) {
  reinterpret(self, context, seeking_t*);
  reinterpret(args, _args, const tiny_key_value_store_on_change_args_t*);

  if(args->key == self->keys->tactic) {
    reinterpret(tactic, args->value, const tactic_t*);

    if(*tactic == tactic_seek_clockwise) {
      set_right_motor_to(self, near_wheel_power);
      set_left_motor_to(self, far_wheel_power);
    }
    else if(*tactic == tactic_seek_counterclockwise) {
      set_right_motor_to(self, far_wheel_power);
      set_left_motor_to(self, near_wheel_power);
    }
  }
}

void seeking_init(
  seeking_t* self,
  i_tiny_key_value_store_t* key_value_store,
  const seeking_keys_t* keys) {
  self->key_value_store = key_value_store;
  self->keys = keys;
  tiny_event_subscription_init(&self->on_change_subscription, self, data_changed);
  tiny_event_subscribe(tiny_key_value_store_on_change(key_value_store), &self->on_change_subscription);
}