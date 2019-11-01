/*!
 * @file
 * @brief
 */

#include "line_detected.h"

void line_detected_init(
  line_detected_t* self,
  i_tiny_key_value_store_t* key_value_store,
  const line_detected_keys_t* keys) {
  self->key_value_store = key_value_store;
  self->keys = keys;

  // tiny_event_subscription_init(&self->on_change_subscription, self, data_changed);
  // tiny_event_subscribe(tiny_key_value_store_on_change(key_value_store), &self->on_change_subscription);
}
