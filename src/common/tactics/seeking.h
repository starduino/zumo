
/*!
 * @file
 * @brief
 */

#ifndef seeking_h
#define seeking_h

#include "tiny_ram_key_value_store.h"

typedef struct {
  tiny_key_value_store_key_t left_motor;
  tiny_key_value_store_key_t right_motor;
  tiny_key_value_store_key_t tactic;
} seeking_keys_t;

typedef struct {
  tiny_event_subscription_t on_change_subscription;
  i_tiny_key_value_store_t* key_value_store;
  const seeking_keys_t* keys;
} seeking_t;

void seeking_init(
  seeking_t* self,
  i_tiny_key_value_store_t* key_value_store,
  const seeking_keys_t* keys);

#endif
