
/*!
 * @file
 * @brief
 */

extern "C" {
#include "charge.h"
#include "motor_power.h"
#include "tiny_ram_key_value_store.h"
#include "tiny_utils.h"
#include "tactic.h"
#include "enemy_location.h"
}

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

enum {
  near_wheel_power = 30,
  far_wheel_power = 100,
  charge = 100,
  a_reduced_charge = 80,
  some_power = 77,
  some_other_power = -22,
  some_other_tactic = tactic_avoid_line,
  in_front = enemy_location_front_center,
  toward_the_left = enemy_location_front_left,
  toward_the_right = enemy_location_front_right
};

// clang-format off
#define data_model_key_value_pairs(pair) \
  pair(key_tactic,              tactic_t) \
  pair(key_left_motor,          motor_power_t) \
  pair(key_right_motor,         motor_power_t) \
  pair(key_enemy_location,      enemy_location_t) \
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

static const charge_keys_t keys = {
  .left_motor = key_left_motor,
  .right_motor = key_right_motor,
  .tactic = key_tactic,
  .enemy_location = key_enemy_location
};

TEST_GROUP(charge) {
  charge_t self;

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
    charge_init(&self, i_key_value_store, &keys);
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

  void when_the_enemy_is_seen(enemy_location_t location) {
    tiny_key_value_store_write(i_key_value_store, key_enemy_location, &location);
  }

  void when_the_tactic_is(tactic_t tactic) {
    tiny_key_value_store_write(i_key_value_store, key_tactic, &tactic);
  }

  void given_the_tactic_is(tactic_t tactic) {
    when_the_tactic_is(tactic);
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

TEST(charge, should_do_nothing_when_another_tactic_becomes_active) {
  given_it_has_been_initialized();
  given_the_left_motor_has_been_set_to(some_power);
  given_the_right_motor_has_been_set_to(some_other_power);

  when_the_tactic_is(some_other_tactic);
  the_left_motor_should_be_set_to(some_power);
  the_right_motor_should_be_set_to(some_other_power);
}

TEST(charge, should_charge_forward_when_the_enemy_is_in_front) {
  given_it_has_been_initialized();
  given_the_tactic_is(tactic_charge);

  when_the_enemy_is_seen(in_front);
  the_left_motor_should_be_set_to(charge);
  the_right_motor_should_be_set_to(charge);
}

TEST(charge, should_charge_left_when_the_enemy_is_in_front_and_to_the_left) {
  given_it_has_been_initialized();
  given_the_tactic_is(tactic_charge);

  when_the_enemy_is_seen(toward_the_left);
  the_left_motor_should_be_set_to(a_reduced_charge);
  the_right_motor_should_be_set_to(charge);
}

TEST(charge, should_charge_right_when_the_enemy_is_in_front_and_to_the_right) {
  given_it_has_been_initialized();
  given_the_tactic_is(tactic_charge);

  when_the_enemy_is_seen(toward_the_right);
  the_left_motor_should_be_set_to(charge);
  the_right_motor_should_be_set_to(a_reduced_charge);
}

