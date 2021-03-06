/*!
 * @file
 * @brief
 */

#include "strategy_plugin.h"
#include "data_model.h"
#include "tuning.h"

static const strategist_keys_t strategist_keys = {
  .key_tactic = key_current_tactic,
  .key_enemy_location = key_enemy_location,
  .key_initial_direction = key_initial_direction,
  .key_robot_running = key_robot_running,
  .key_line_detected = key_line_detected,
  .key_tactic_stopped = key_tactic_stopped
};

static const line_detected_keys_t line_detected_keys = {
  .key_left_motor = key_left_motor,
  .key_right_motor = key_right_motor,
  .key_tactic = key_current_tactic,
  .key_left_line_detected = key_left_line_detected,
  .key_right_line_detected = key_right_line_detected,
  .key_tactic_stopped = key_tactic_stopped
};

static const charge_keys_t charge_keys = {
  .left_motor = key_left_motor,
  .right_motor = key_right_motor,
  .tactic = key_current_tactic,
  .enemy_location = key_enemy_location
};

static const seeking_keys_t seeking_keys = {
  .left_motor = key_left_motor,
  .right_motor = key_right_motor,
  .tactic = key_current_tactic
};

static const spin_on_init_keys_t spin_on_init_keys = {
  .left_motor = key_left_motor,
  .right_motor = key_right_motor,
  .knob_counts = key_knob_counts,
  .tactic = key_current_tactic,
  .tactic_stopped = key_tactic_stopped
};

void strategy_plugin_init(strategy_plugin_t* self, i_tiny_key_value_store_t* key_value_store, tiny_timer_group_t* timer_group)
{
  strategist_init(&self->strategist, key_value_store, &strategist_keys);

  spin_on_init_init(
    &self->spin_on_init,
    key_value_store,
    &spin_on_init_keys,
    timer_group);

  line_detected_init(
    &self->line_detected,
    key_value_store,
    &line_detected_keys,
    timer_group);

  charge_init(&self->charge, key_value_store, &charge_keys);
  seeking_init(&self->seeking, key_value_store, &seeking_keys);
}
