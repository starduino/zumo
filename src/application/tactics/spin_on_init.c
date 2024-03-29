/*!
 * @file
 * @brief
 */

#include "spin_on_init.h"
#include "tactic.h"
#include "tiny_utils.h"
#include "tuning.h"
#include "hal/i_tiny_analog_input.h"
#include <stdlib.h>

enum {
  max_counts = UINT16_MAX
};

static void stop_running_timer_expired(void* context)
{
  reinterpret(self, context, spin_on_init_t*);
  uint8_t stopped;
  tiny_key_value_store_read(
    self->key_value_store,
    self->keys->tactic_stopped,
    &stopped);

  stopped++;

  tiny_key_value_store_write(
    self->key_value_store,
    self->keys->tactic_stopped,
    &stopped);
}

static tiny_analog_input_counts_t potentiometer_counts(spin_on_init_t* self)
{
  tiny_analog_input_counts_t counts;
  tiny_key_value_store_read(
    self->key_value_store,
    self->keys->knob_counts,
    &counts);
  return counts;
}

static void spin(spin_on_init_t* self, bool spin_right)
{
  motor_power_t left_power = spin_right ? spin_on_init_near_wheel_power : spin_on_init_far_wheel_power;
  motor_power_t right_power = spin_right ? spin_on_init_far_wheel_power : spin_on_init_near_wheel_power;
  tiny_key_value_store_write(
    self->key_value_store,
    self->keys->left_motor,
    &left_power);
  tiny_key_value_store_write(
    self->key_value_store,
    self->keys->right_motor,
    &right_power);
}

static tiny_timer_ticks_t counts_to_ticks(tiny_analog_input_counts_t counts)
{
  return abs(counts - max_counts / 2) * (turn_time_360_degree / 2) / (max_counts / 2);
}

static void data_changed(void* context, const void* _args)
{
  reinterpret(self, context, spin_on_init_t*);
  reinterpret(args, _args, const tiny_key_value_store_on_change_args_t*);

  if(args->key == self->keys->tactic) {
    reinterpret(tactic, args->value, const tactic_t*);

    if(*tactic == tactic_init) {
      tiny_analog_input_counts_t counts = potentiometer_counts(self);
      bool spin_right = counts < (max_counts / 2);
      spin(self, spin_right);

      tiny_timer_start(
        self->timer_group,
        &self->back_up_timer,
        counts_to_ticks(counts),
        self,
        stop_running_timer_expired);
    }
  }
}

void spin_on_init_init(
  spin_on_init_t* self,
  i_tiny_key_value_store_t* key_value_store,
  const spin_on_init_keys_t* keys,
  tiny_timer_group_t* timer_group)
{
  self->key_value_store = key_value_store;
  self->keys = keys;
  self->timer_group = timer_group;

  tiny_event_subscription_init(&self->on_change_subscription, self, data_changed);
  tiny_event_subscribe(tiny_key_value_store_on_change(key_value_store), &self->on_change_subscription);
}
