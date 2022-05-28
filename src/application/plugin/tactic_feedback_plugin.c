/*!
 * @file
 * @brief
 */

#include "tactic_feedback_plugin.h"
#include "data_model.h"
#include "tiny_utils.h"

enum {
  half_period = 50
};

static const buzzer_frequency_t base_frequency = 440;

static const buzzer_frequency_t frequency_map[] = {
  440,
  523,
  659,
  784,
  880,
  16160
};

static void update_frequency(void* context)
{
  reinterpret(self, context, tactic_feedback_plugin_t*);

  self->at_base_frequency = !self->at_base_frequency;

  tiny_key_value_store_write(
    self->key_value_store,
    key_buzzer_frequency,
    self->at_base_frequency ? &base_frequency : &self->frequency);
}

static void arm_timer(tactic_feedback_plugin_t* self, tiny_timer_group_t* timer_group)
{
  tiny_timer_start_periodic(timer_group, &self->timer, half_period, self, update_frequency);
}

static void data_changed(void* context, const void* _args)
{
  reinterpret(self, context, tactic_feedback_plugin_t*);
  reinterpret(args, _args, const tiny_key_value_store_on_change_args_t*);
  reinterpret(tactic, args->value, const tactic_t*);

  if(args->key == key_current_tactic) {
    self->frequency = frequency_map[*tactic];
  }
}

void tactic_feedback_plugin_init(
  tactic_feedback_plugin_t* self,
  i_tiny_key_value_store_t* key_value_store,
  tiny_timer_group_t* timer_group)
{
  self->key_value_store = key_value_store;

  tiny_event_subscription_init(&self->subscription, self, data_changed);
  tiny_event_subscribe(tiny_key_value_store_on_change(key_value_store), &self->subscription);
  arm_timer(self, timer_group);
}
