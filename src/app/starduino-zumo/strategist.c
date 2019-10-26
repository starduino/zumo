/*!
 * @file
 * @brief
 */

#include "strategist.h"
#include "data_model.h"
#include "tiny_utils.h"

static tiny_fsm

static void data_changed(void* context, const void* _args) {
  reinterpret(self, context, strategist_t*);
  reinterpret(args, _args, const tiny_key_value_store_on_change_args_t*);
  reinterpret(power, args->value, const motor_power_t*);

  // inputs: current location (left / )
  // line avoidance
  // attack
  // searching

  switch(args->key) {
    case key_left_motor:
      break;

    case key_right_motor:
      break;
  }
}

static void strategist_init(strategist_t* self, i_tiny_key_value_store_t* key_value_store) {
  tiny_fsm_init(&self->fsm, state_init);
  tiny_event_subscription_init(&self->on_change_subscription, self, data_changed);
  tiny_event_subscribe(tiny_key_value_store_on_change(key_value_store), &self->on_change_subscription);
}