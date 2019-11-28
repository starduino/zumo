/*!
 * @file
 * @brief
 */

#include "charge.h"
#include "tiny_utils.h"

static void foo(void* a, void* b) {
  if(a == b) {
    while(1) {
    }
  }
}

void charge_init(
  charge_t* self,
  i_tiny_key_value_store_t* key_value_store) {
  self->key_value_store = key_value_store;
  foo(self, &self->on_change_subscription);
  tiny_event_subscribe(tiny_key_value_store_on_change(key_value_store), &self->on_change_subscription);
}
