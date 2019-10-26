#include "charge.h"

static void charge_init(charge_t self, i_tiny_key_value_store_t* key_value_store) {

  tiny_event_subscription_init(&self->on_change_subscription, self, data_changed);
  tiny_event_subscribe(tiny_key_value_store_on_change(key_value_store), &self->on_change_subscription);
}