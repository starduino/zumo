/*!
 * @file
 * @brief
 */

extern "C" {
#include <string.h>
#include "spin_on_init.h"
#include "motor_power.h"
#include "tactic.h"
#include "tuning.h"
#include "enemy_location.h"
#include "tiny_utils.h"
#include "tiny_ram_key_value_store.h"
#include "hal/i_tiny_analog_input.h"
}

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "double/tiny_time_source_double.h"

#define degrees

enum {
  right,
  left,
  detected = true,
  not_detected = false,
  some_power = 70,
  some_other_power = -55,
  back_up = motor_power_min,
  back_up_time = line_detection_back_up_time,
  turn_time = line_detection_turn_time,
  some_time_has_passed = 17,
  is_running = 0,
  has_stopped,
  near_wheel_power = line_detection_near_wheel_power,
  far_wheel_power = line_detection_far_wheel_power,
  a_long_time = 200,
  max_counts = 0xFFFF
};

// clang-format off
#define data_model_key_value_pairs(pair) \
  pair(key_tactic,              tactic_t) \
  pair(key_knob_counts,              tiny_analog_input_counts_t) \
  pair(key_left_motor,          motor_power_t) \
  pair(key_right_motor,         motor_power_t) \
  pair(key_tactic_stopped,      uint8_t)

// clang-format on

enumerate_ram_key_value_pairs(data_model_key_value_pairs);

#define _storage_type_name data_model_storage_t

generate_storage_type_for_ram_key_value_pairs(data_model_key_value_pairs);

static const tiny_ram_key_value_store_key_value_pair_t key_value_pairs[] = {
  generate_configuration_pairs_from_ram_key_value_pairs(data_model_key_value_pairs)
};

static data_model_storage_t storage;

static const tiny_ram_key_value_store_configuration_t store_config = {
  key_value_pairs,
  element_count(key_value_pairs)
};

static const spin_on_init_keys_t keys = {
  .left_motor = key_left_motor,
  .right_motor = key_right_motor,
  .knob_counts = key_knob_counts,
  .tactic = key_tactic,
  .tactic_stopped = key_tactic_stopped
};

TEST_GROUP(spin_on_init)
{
  spin_on_init_t self;

  tiny_ram_key_value_store_t ram_key_value_store;
  i_tiny_key_value_store_t* i_key_value_store;

  tiny_time_source_double_t time_source;
  tiny_timer_group_t timer_group;

  void setup()
  {
    tiny_ram_key_value_store_init(
      &ram_key_value_store,
      &store_config,
      &storage);
    i_key_value_store = &ram_key_value_store.interface;

    tiny_time_source_double_init(&time_source);
    tiny_timer_group_init(&timer_group, &time_source.interface);
  }

  void when_it_is_initialized_and_the_knob_is_set_to(uint64_t degree)
  {
    tiny_analog_input_counts_t counts = ((uint64_t)max_counts * degree) / 360;
    spin_on_init_init(&self, i_key_value_store, &keys, &timer_group);
    tiny_key_value_store_write(i_key_value_store, key_knob_counts, &counts);
  }

  void given_it_has_been_initialized_and_the_knob_is_set_to(uint64_t degree)
  {
    when_it_is_initialized_and_the_knob_is_set_to(degree);
  }

  void when_another_tactic_is_selected()
  {
    tactic_t tactic = tactic_charge;
    tiny_key_value_store_write(i_key_value_store, key_tactic, &tactic);
  }

  void when_this_tactic_is_selected()
  {
    tactic_t tactic = tactic_init;
    tiny_key_value_store_write(i_key_value_store, key_tactic, &tactic);
  }

  void given_this_tactic_is_active()
  {
    when_this_tactic_is_selected();
  }

  void given_the_left_motor_has_been_set_to(motor_power_t value)
  {
    tiny_key_value_store_write(i_key_value_store, key_left_motor, &value);
  }

  void given_the_right_motor_has_been_set_to(motor_power_t value)
  {
    tiny_key_value_store_write(i_key_value_store, key_right_motor, &value);
  }

  void the_left_motor_should_be_set_to(motor_power_t expected)
  {
    motor_power_t actual;
    tiny_key_value_store_read(i_key_value_store, key_left_motor, &actual);
    CHECK_EQUAL(expected, actual);
  }

  void the_right_motor_should_be_set_to(motor_power_t expected)
  {
    motor_power_t actual;
    tiny_key_value_store_read(i_key_value_store, key_right_motor, &actual);
    CHECK_EQUAL(expected, actual);
  }

  void the_tactic_should_indicate_that_it(uint8_t expected)
  {
    uint8_t actual;
    tiny_key_value_store_read(i_key_value_store, key_tactic_stopped, &actual);
    CHECK_EQUAL(expected, actual);
  }

  void the_motors_should_be_turning(uint8_t direction)
  {
    if(direction == right) {
      the_left_motor_should_be_set_to(near_wheel_power);
      the_right_motor_should_be_set_to(far_wheel_power);
    }
    else {
      the_left_motor_should_be_set_to(far_wheel_power);
      the_right_motor_should_be_set_to(near_wheel_power);
    }
  }

  void after(tiny_time_source_ticks_t ticks)
  {
    tiny_time_source_double_tick(&time_source, ticks);
    tiny_timer_group_run(&timer_group);
  }

  void the_robot_should_spin(uint8_t direction, uint64_t degree)
  {
    tiny_timer_ticks_t ticks = (degree * turn_time_360_degree) / 360;
    the_motors_should_be_turning(direction);
    the_tactic_should_indicate_that_it(is_running);

    after(ticks - 1);
    the_motors_should_be_turning(direction);
    the_tactic_should_indicate_that_it(is_running);

    after(1);
    the_tactic_should_indicate_that_it(has_stopped);
  }
};

TEST(spin_on_init, should_do_nothing_on_init)
{
  given_the_left_motor_has_been_set_to(some_power);
  given_the_right_motor_has_been_set_to(some_other_power);

  when_it_is_initialized_and_the_knob_is_set_to(90);
  the_left_motor_should_be_set_to(some_power);
  the_right_motor_should_be_set_to(some_other_power);
}

TEST(spin_on_init, should_spin_right_when_the_tactic_is_selected)
{
  given_it_has_been_initialized_and_the_knob_is_set_to(90 degrees);
  when_this_tactic_is_selected();
  the_robot_should_spin(right, 90 degrees);
}

TEST(spin_on_init, should_spin_right_at_another_angle)
{
  given_it_has_been_initialized_and_the_knob_is_set_to(110 degrees);
  when_this_tactic_is_selected();
  the_robot_should_spin(right, 70 degrees);
}

TEST(spin_on_init, should_spin_left_when_the_tactic_is_selected)
{
  given_it_has_been_initialized_and_the_knob_is_set_to(270 degrees);
  when_this_tactic_is_selected();
  the_robot_should_spin(left, 90 degrees);
}

TEST(spin_on_init, should_spin_left_at_another_angle)
{
  given_it_has_been_initialized_and_the_knob_is_set_to(182 degrees);
  when_this_tactic_is_selected();
  the_robot_should_spin(left, 2 degrees);
}
