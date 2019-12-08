/*!
 * @file
 * @brief
 */

extern "C" {
#include <string.h>
#include "line_detected.h"
#include "motor_power.h"
#include "tactic.h"
#include "enemy_location.h"
#include "tiny_utils.h"
#include "tiny_ram_key_value_store.h"
}

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "tiny_time_source_double.h"

enum {
  right,
  left,
  detected = true,
  not_detected = false,
  some_power = 70,
  some_other_power = -55,
  back_up = motor_power_min,
  back_up_time = 70,
  turn_time = 30,
  some_time_has_passed = 17,
  is_running = 0,
  has_stopped,
  near_wheel_power = 30,
  far_wheel_power = 100,
  a_long_time = 200
};

// clang-format off
#define data_model_key_value_pairs(pair) \
  pair(key_tactic,              tactic_t) \
  pair(key_left_motor,          motor_power_t) \
  pair(key_right_motor,         motor_power_t) \
  pair(key_left_line_detected,  bool) \
  pair(key_right_line_detected, bool) \
  pair(key_tactic_stopped,      uint8_t) \
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

static const line_detected_keys_t keys = {
  .key_left_motor = key_left_motor,
  .key_right_motor = key_right_motor,
  .key_tactic = key_tactic,
  .key_left_line_detected = key_left_line_detected,
  .key_right_line_detected = key_right_line_detected,
  .key_tactic_stopped = key_tactic_stopped
};

static const line_detected_config_t config = {
  .near_wheel_power = near_wheel_power,
  .far_wheel_power = far_wheel_power,
  .back_up_time = back_up_time,
  .turn_time = turn_time,
};

TEST_GROUP(line_detected) {
  line_detected_t self;

  tiny_ram_key_value_store_t ram_key_value_store;
  i_tiny_key_value_store_t* i_key_value_store;

  tiny_time_source_double_t time_source;
  tiny_timer_group_t timer_group;

  void setup() {
    tiny_ram_key_value_store_init(
      &ram_key_value_store,
      &store_config,
      &storage);
    i_key_value_store = &ram_key_value_store.interface;

    tiny_time_source_double_init(&time_source);
    tiny_timer_group_init(&timer_group, &time_source.interface);
  }

  void when_it_is_initialized() {
    line_detected_init(&self, i_key_value_store, &keys, &config, &timer_group);
  }

  void given_it_has_been_initialized() {
    when_it_is_initialized();
  }

  void when_another_tactic_is_selected() {
    tactic_t tactic = tactic_charge;
    tiny_key_value_store_write(i_key_value_store, key_tactic, &tactic);
  }

  void when_this_tactic_is_selected() {
    tactic_t tactic = tactic_avoid_line;
    tiny_key_value_store_write(i_key_value_store, key_tactic, &tactic);
  }

  void given_this_tactic_is_active() {
    when_this_tactic_is_selected();
  }

  void given_the_left_motor_has_been_set_to(motor_power_t value) {
    tiny_key_value_store_write(i_key_value_store, key_left_motor, &value);
  }

  void given_the_right_motor_has_been_set_to(motor_power_t value) {
    tiny_key_value_store_write(i_key_value_store, key_right_motor, &value);
  }

  void when_the_left_line_is(bool detected) {
    tiny_key_value_store_write(i_key_value_store, key_left_line_detected, &detected);
  }

  void given_the_left_line_has_been(bool detected) {
    when_the_left_line_is(detected);
  }

  void when_the_right_line_is(bool detected) {
    tiny_key_value_store_write(i_key_value_store, key_right_line_detected, &detected);
  }

  void given_the_right_line_has_been(bool detected) {
    when_the_right_line_is(detected);
  }

  void the_left_motor_should_be_set_to(motor_power_t expected) {
    motor_power_t actual;
    tiny_key_value_store_read(i_key_value_store, key_left_motor, &actual);
    CHECK_EQUAL(expected, actual);
  }

  void the_right_motor_should_be_set_to(motor_power_t expected) {
    motor_power_t actual;
    tiny_key_value_store_read(i_key_value_store, key_right_motor, &actual);
    CHECK_EQUAL(expected, actual);
  }

  void the_tactic_should_indicate_that_it(uint8_t expected) {
    uint8_t actual;
    tiny_key_value_store_read(i_key_value_store, key_tactic_stopped, &actual);
    CHECK_EQUAL(expected, actual);
  }

  void the_motors_should_be_turning(uint8_t direction) {
    if (direction == right) {
      the_left_motor_should_be_set_to(far_wheel_power);
      the_right_motor_should_be_set_to(near_wheel_power);
    }
    else {
      the_left_motor_should_be_set_to(near_wheel_power);
      the_right_motor_should_be_set_to(far_wheel_power);
    }
  }

  void after(tiny_time_source_ticks_t ticks) {
    tiny_time_source_double_tick(&time_source, ticks);
    tiny_timer_group_run(&timer_group);
  }
};

TEST(line_detected, should_do_nothing_on_init) {
   given_the_left_motor_has_been_set_to(some_power);
   given_the_right_motor_has_been_set_to(some_other_power);

   when_it_is_initialized();
   the_left_motor_should_be_set_to(some_power);
   the_right_motor_should_be_set_to(some_other_power);
}

TEST(line_detected, should_do_nothing_when_a_line_is_detected_and_it_is_not_the_active_tactic) {
   when_it_is_initialized();
   given_the_left_motor_has_been_set_to(some_power);
   given_the_right_motor_has_been_set_to(some_other_power);

   when_the_left_line_is(detected);
   the_left_motor_should_be_set_to(some_power);
   the_right_motor_should_be_set_to(some_other_power);

   when_the_right_line_is(detected);
   the_left_motor_should_be_set_to(some_power);
   the_right_motor_should_be_set_to(some_other_power);
}

TEST(line_detected, should_back_up_for_a_moment_when_the_tactic_is_selected) {
  given_it_has_been_initialized();

  when_this_tactic_is_selected();
  the_left_motor_should_be_set_to(back_up);
  the_right_motor_should_be_set_to(back_up);
}

TEST(line_detected, should_do_nothing_when_another_tactic_becomes_active) {
  given_it_has_been_initialized();
  given_the_left_motor_has_been_set_to(some_power);
  given_the_right_motor_has_been_set_to(some_other_power);

  when_another_tactic_is_selected();
  after(a_long_time);
  the_left_motor_should_be_set_to(some_power);
  the_right_motor_should_be_set_to(some_other_power);
}

TEST(line_detected, should_turn_right_after_it_has_backed_up_when_the_left_line_is_detected) {
  given_it_has_been_initialized();
  after(some_time_has_passed);
  given_the_left_line_has_been(detected);
  given_this_tactic_is_active();

  after(back_up_time - 1);
  the_left_motor_should_be_set_to(back_up);
  the_right_motor_should_be_set_to(back_up);

  after(1);
  the_motors_should_be_turning(right);
}

TEST(line_detected, should_indicate_that_it_is_complete_after_it_has_finished_turning_right) {
  given_it_has_been_initialized();
  after(some_time_has_passed);
  given_the_left_line_has_been(detected);
  given_this_tactic_is_active();

  after(back_up_time);
  the_tactic_should_indicate_that_it(is_running);

  after(turn_time - 1);
  the_tactic_should_indicate_that_it(is_running);

  after(1);
  the_tactic_should_indicate_that_it(has_stopped);
}

TEST(line_detected, should_turn_left_after_it_has_backed_up_when_the_right_line_is_detected) {
  given_it_has_been_initialized();
  after(some_time_has_passed);
  given_the_right_line_has_been(detected);
  given_this_tactic_is_active();

  after(back_up_time - 1);
  the_left_motor_should_be_set_to(back_up);
  the_right_motor_should_be_set_to(back_up);

  after(1);
  the_motors_should_be_turning(left);
}

TEST(line_detected, should_indicate_that_it_is_complete_after_it_has_finished_turning_left) {
  given_it_has_been_initialized();
  after(some_time_has_passed);
  given_the_right_line_has_been(detected);
  given_this_tactic_is_active();

  after(back_up_time);
  the_tactic_should_indicate_that_it(is_running);

  after(turn_time - 1);
  the_tactic_should_indicate_that_it(is_running);

  after(1);
  the_tactic_should_indicate_that_it(has_stopped);
}
