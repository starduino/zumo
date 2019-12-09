/*!
 * @file
 * @brief
 *
 * Copyright GE Appliances - Confidential - All rights reserved.
 */

#ifndef buzzer_plugin_h
#define buzzer_plugin_h

#include "i_tiny_key_value_store.h"

typedef struct {
  tiny_event_subscription_t on_change_subscription;
} buzzer_plugin_t;

void buzzer_plugin_init(buzzer_plugin_t* self, i_tiny_key_value_store_t* key_value_store);

#endif
