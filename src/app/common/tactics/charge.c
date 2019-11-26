/*!
 * @file
 * @brief
 */

#include "charge.h"
#include "enemy_location.h"
#include "tactic.h"
#include "tiny_utils.h"
#include "motor_power.h"

enum {
  near_wheel_power = 30,
  far_wheel_power = 100,
  reduced_charge = 80,
  charge = 100,
  toward_the_left = enemy_location_front_left,
  toward_the_right = enemy_location_front_right
};

static void set_right_motor_to(charge_t* self, motor_power_t power) {
  tiny_key_value_store_write(self->key_value_store, self->keys->right_motor, &power);
}

static void set_left_motor_to(charge_t* self, motor_power_t power) {
  tiny_key_value_store_write(self->key_value_store, self->keys->left_motor, &power);
}

static bool the_charge_tactic_is_selected(charge_t* self) {
  tactic_t tactic;
  tiny_key_value_store_read(self->key_value_store, self->keys->tactic, &tactic);
  return tactic == tactic_charge;
}

static bool the_enemy_is(charge_t* self, enemy_location_t expected) {
  enemy_location_t actual;
  tiny_key_value_store_read(self->key_value_store, self->keys->enemy_location, &actual);
  return expected == actual;
}

static void data_changed(void* context, const void* _args) {
  reinterpret(self, context, charge_t*);
  reinterpret(args, _args, const tiny_key_value_store_on_change_args_t*);
  if(args->key == self->keys->tactic || args->key == self->keys->enemy_location) {
    if(the_charge_tactic_is_selected(self)) {
      if(the_enemy_is(self, toward_the_left)) {
        set_left_motor_to(self, reduced_charge);
        set_right_motor_to(self, charge);
      }
      else if(the_enemy_is(self, toward_the_right)) {
        set_left_motor_to(self, charge);
        set_right_motor_to(self, reduced_charge);
      }
      else {
        set_left_motor_to(self, charge);
        set_right_motor_to(self, charge);
      }
    }
  }
}

void charge_init(
  charge_t* self,
  i_tiny_key_value_store_t* key_value_store,
  const charge_keys_t* keys) {
  self->key_value_store = key_value_store;
  self->keys = keys;
  tiny_event_subscription_init(&self->on_change_subscription, self, data_changed);
  tiny_event_subscribe(tiny_key_value_store_on_change(key_value_store), &self->on_change_subscription);
}
