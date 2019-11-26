/*!
 * @file
 * @brief
 */

#ifndef strategist_h
#define strategist_h

#include "enemy_location.h"
#include "i_tiny_key_value_store.h"
#include "tiny_event_subscription.h"

typedef struct {
  tiny_key_value_store_key_t key_tactic;
  tiny_key_value_store_key_t key_enemy_location;
  tiny_key_value_store_key_t key_initial_direction;
  tiny_key_value_store_key_t key_robot_running;
  tiny_key_value_store_key_t key_line_detected;
  tiny_key_value_store_key_t key_tactic_stopped;
} strategist_keys_t;

typedef struct {
  tiny_event_subscription_t on_change_subscription;
  i_tiny_key_value_store_t* key_value_store;
  const strategist_keys_t* keys;
  enemy_location_t previous_location;
} strategist_t;

void strategist_init(
  strategist_t* self,
  i_tiny_key_value_store_t* key_value_store,
  const strategist_keys_t* keys);

#endif
