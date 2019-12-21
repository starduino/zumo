/*!
 * @file
 * @brief
 */

#include "detect_enemy.h"
#include "distance_in_cm.h"
#include "tiny_utils.h"

enum {
  threshold = 20
};

static void data_changed(void* context, const void* _args) {
  reinterpret(self, context, detect_enemy_t*);
  reinterpret(args, _args, const tiny_key_value_store_on_change_args_t*);

  if(args->key == self->keys->sensor_distance) {
    reinterpret(distance, args->value, distance_in_cm_t*);
    bool is_active = *distance <= threshold;
    tiny_key_value_store_write(self->key_value_store, self->keys->sensor_active, &is_active);
  }
}

void detect_enemy_init(
  detect_enemy_t* self,
  i_tiny_key_value_store_t* key_value_store,
  const detect_enemy_keys_t* keys) {
  self->key_value_store = key_value_store;
  self->keys = keys;

  tiny_event_subscription_init(&self->on_change_subscription, self, data_changed);
  tiny_event_subscribe(tiny_key_value_store_on_change(key_value_store), &self->on_change_subscription);
}