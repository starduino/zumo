
/*!
 * @file
 * @brief
 */

extern "C" {
#include "seeking.h"
#include "motor_power.h"
#include "tiny_ram_key_value_store.h"
#include "tiny_utils.h"
#include "tactic.h"
}

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

enum {
  near_wheel_power = 30,
  far_wheel_power = 100,
  some_power = 77,
  some_other_power = -22,
  some_other_tactic = tactic_charge
};

// clang-format off
#define data_model_key_value_pairs(pair) \
  pair(key_tactic,              tactic_t) \
  pair(key_left_motor,          motor_power_t) \
  pair(key_right_motor,         motor_power_t) \
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

static const seeking_keys_t keys = {
  .left_motor = key_left_motor,
  .right_motor = key_right_motor,
  .tactic = key_tactic
};

TEST_GROUP(seeking) {
  seeking_t self;

  tiny_ram_key_value_store_t ram_key_value_store;
  i_tiny_key_value_store_t* i_key_value_store;

  void setup() {
    tiny_ram_key_value_store_init(
      &ram_key_value_store,
      &store_config,
      &storage);
    i_key_value_store = &ram_key_value_store.interface;
  }

  void when_it_is_initialized() {
    seeking_init(&self, i_key_value_store, &keys);
  }

  void given_it_has_been_initialized() {
    when_it_is_initialized();
  }

  void given_the_left_motor_has_been_set_to(motor_power_t value) {
    tiny_key_value_store_write(i_key_value_store, key_left_motor, &value);
  }

  void given_the_right_motor_has_been_set_to(motor_power_t value) {
    tiny_key_value_store_write(i_key_value_store, key_right_motor, &value);
  }

  void when_the_tactic_is(tactic_t tactic) {
    tiny_key_value_store_write(i_key_value_store, key_tactic, &tactic);
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
};

TEST(seeking, should_do_nothing_when_another_tactic_becomes_active) {
  given_it_has_been_initialized();
  given_the_left_motor_has_been_set_to(some_power);
  given_the_right_motor_has_been_set_to(some_other_power);

  when_the_tactic_is(some_other_tactic);
  the_left_motor_should_be_set_to(some_power);
  the_right_motor_should_be_set_to(some_other_power);
}

TEST(seeking, should_turn_clockwise_when_the_clockwise_tactic_is_selected) {
  given_it_has_been_initialized();

  when_the_tactic_is(tactic_seek_clockwise);
  the_left_motor_should_be_set_to(far_wheel_power);
  the_right_motor_should_be_set_to(near_wheel_power);
}

TEST(seeking, should_turn_counterclockwise_when_the_counterclockwise_tactic_is_selected) {
  given_it_has_been_initialized();

  when_the_tactic_is(tactic_seek_counterclockwise);
  the_left_motor_should_be_set_to(near_wheel_power);
  the_right_motor_should_be_set_to(far_wheel_power);
}
