#ifndef charge_h
#define charge_h
#include "i_tiny_key_value_store.h"

typedef struct {
  tiny_event_subscription_t on_change_subscription;
} charge_t;

#endif
