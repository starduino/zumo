
/*!
 * @file
 * @brief
 */

#ifndef charge_h
#define charge_h

#include "tiny_ram_key_value_store.h"

typedef struct {
  tiny_key_value_store_key_t left_motor;
  tiny_key_value_store_key_t right_motor;
  tiny_key_value_store_key_t tactic;
  tiny_key_value_store_key_t enemy_location;
} charge_keys_t;

typedef struct {
  tiny_event_subscription_t on_change_subscription;
  i_tiny_key_value_store_t* key_value_store;
  const charge_keys_t* keys;
} charge_t;

void charge_init(
  charge_t* self,
  i_tiny_key_value_store_t* key_value_store,
  const charge_keys_t* keys);

#endif
