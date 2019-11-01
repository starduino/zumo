/*!
 * @file
 * @brief
 */

#ifndef line_detected_h
#define line_detected_h

#include "enemy_location.h"
#include "i_tiny_key_value_store.h"
#include "tiny_event_subscription.h"
#include "tiny_fsm.h"

typedef struct {
  tiny_key_value_store_key_t key_motors;
  tiny_key_value_store_key_t key_tactic;
  tiny_key_value_store_key_t key_last_enemy_location;
  tiny_key_value_store_key_t key_line_detected;
  tiny_key_value_store_key_t key_tactic_stopped;
} line_detected_keys_t;

typedef struct {
  tiny_event_subscription_t on_change_subscription;
  tiny_fsm_t fsm;
  i_tiny_key_value_store_t* key_value_store;
  const line_detected_keys_t* keys;

  struct {
    enemy_location_t previous_location;
  } _private;
} line_detected_t;

void line_detected_init(
  line_detected_t* self,
  i_tiny_key_value_store_t* key_value_store,
  const line_detected_keys_t* keys);

#endif
