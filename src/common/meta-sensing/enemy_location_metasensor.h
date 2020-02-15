/*!
 * @file
 * @brief
 */

#ifndef enemy_location_metasensor_h
#define enemy_location_metasensor_h

#include "i_tiny_key_value_store.h"

typedef struct {
  tiny_key_value_store_key_t enemy_location;
  tiny_key_value_store_key_t left_sensor_active;
  tiny_key_value_store_key_t right_sensor_active;
} enemy_location_metasensor_keys_t;

typedef struct {
  const enemy_location_metasensor_keys_t* keys;
  tiny_event_subscription_t on_change_subscription;
  i_tiny_key_value_store_t* key_value_store;
} enemy_location_metasensor_t;

void enemy_location_metasensor_init(
  enemy_location_metasensor_t* self,
  i_tiny_key_value_store_t* key_value_store,
  const enemy_location_metasensor_keys_t* keys);

#endif
