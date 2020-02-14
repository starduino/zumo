/*!
 * @file
 * @brief
 */

#ifndef tactic_feedback_plugin_h
#define tactic_feedback_plugin_h

#include "i_tiny_key_value_store.h"
#include "tiny_timer.h"
#include "buzzer_frequency.h"

typedef struct {
  tiny_timer_t timer;
  tiny_event_subscription_t subscription;
  i_tiny_key_value_store_t* key_value_store;
  buzzer_frequency_t frequency;
  bool at_base_frequency;
} tactic_feedback_plugin_t;

void tactic_feedback_plugin_init(
  tactic_feedback_plugin_t* self,
  i_tiny_key_value_store_t* key_value_store,
  tiny_timer_group_t* timer_group);

#endif
