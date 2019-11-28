/*!
 * @file
 * @brief
 */

#include "enemy_location_metasensor.h"
#include "enemy_location.h"
#include "tiny_utils.h"

static void change_enemy_location_to(enemy_location_metasensor_t* self, enemy_location_t location) {
  tiny_key_value_store_write(
    self->key_value_store,
    self->keys->enemy_location,
    &location);
}

static bool left_sensor_is_active(enemy_location_metasensor_t* self) {
  bool active;
  tiny_key_value_store_read(
    self->key_value_store,
    self->keys->left_sensor_active,
    &active);
  return active;
}

static bool right_sensor_is_active(enemy_location_metasensor_t* self) {
  bool active;
  tiny_key_value_store_read(
    self->key_value_store,
    self->keys->right_sensor_active,
    &active);
  return active;
}

static void do_location_thing(enemy_location_metasensor_t* self) {
  if(left_sensor_is_active(self)) {
    if(right_sensor_is_active(self)) {
      change_enemy_location_to(self, enemy_location_front_center);
    }
    else {
      change_enemy_location_to(self, enemy_location_front_left);
    }
  }
  else {
    if(right_sensor_is_active(self)) {
      change_enemy_location_to(self, enemy_location_front_right);
    }
    else {
      change_enemy_location_to(self, enemy_location_unknown);
    }
  }
}

static void data_changed(void* context, const void* _args) {
  reinterpret(self, context, enemy_location_metasensor_t*);
  reinterpret(args, _args, const tiny_key_value_store_on_change_args_t*);
  if(args->key == self->keys->left_sensor_active || args->key == self->keys->right_sensor_active) {
    do_location_thing(self);
  }
}

void enemy_location_metasensor_init(
  enemy_location_metasensor_t* self,
  i_tiny_key_value_store_t* key_value_store,
  const enemy_location_metasensor_keys_t* keys) {
  self->key_value_store = key_value_store;
  self->keys = keys;

  change_enemy_location_to(self, enemy_location_unknown);

  tiny_event_subscription_init(&self->on_change_subscription, self, data_changed);
  tiny_event_subscribe(tiny_key_value_store_on_change(key_value_store), &self->on_change_subscription);
}
