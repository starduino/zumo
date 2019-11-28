
/*!
 * @file
 * @brief
 */

#ifndef charge_h
#define charge_h

#include "tiny_ram_key_value_store.h"

typedef struct {
  i_tiny_key_value_store_t* key_value_store;
  tiny_event_subscription_t on_change_subscription;
} charge_t;

void charge_init(
  charge_t* self,
  i_tiny_key_value_store_t* key_value_store);

#endif
