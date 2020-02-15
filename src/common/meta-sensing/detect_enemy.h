/*!
 * @file
 * @brief
 */

#ifndef detect_enemy_h
#define detect_enemy_h

#include "i_tiny_key_value_store.h"

typedef struct {
  tiny_key_value_store_key_t sensor_distance;
  tiny_key_value_store_key_t enemy_detected;
} detect_enemy_keys_t;

typedef struct {
  const detect_enemy_keys_t* keys;
  tiny_event_subscription_t on_change_subscription;
  i_tiny_key_value_store_t* key_value_store;
} detect_enemy_t;

void detect_enemy_init(
  detect_enemy_t* self,
  i_tiny_key_value_store_t* key_value_store,
  const detect_enemy_keys_t* keys);

#endif
