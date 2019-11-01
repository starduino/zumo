/*!
 * @file
 * @brief
 */

extern "C" {
#include <string.h>
#include "line_detected.h"
#include "motors.h"
#include "tactic.h"
#include "enemy_location.h"
#include "tiny_utils.h"
#include "tiny_ram_key_value_store.h"
}

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

enum {
  on = true
};

motors_t some_setting = {
  .left_power = 70,
  .right_power = -22
};

// clang-format off
#define data_model_key_value_pairs(pair) \
  pair(key_tactic,              tactic_t) \
  pair(key_motors,              motors_t) \
  pair(key_line_detected,       bool) \
  pair(key_tactic_stopped,      uint8_t) \
  pair(key_last_enemy_location, enemy_location_t) \
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
  .key_motors = key_motors,
  .key_tactic = key_tactic,
  .key_last_enemy_location = key_last_enemy_location,
  .key_line_detected = key_line_detected,
  .key_tactic_stopped = key_tactic_stopped
};

TEST_GROUP(line_detected) {
  line_detected_t self;
  tiny_ram_key_value_store_t ram_key_value_store;
  i_tiny_key_value_store_t* i_key_value_store;

  void setup() {
    tiny_ram_key_value_store_init(
      &ram_key_value_store,
      &store_config,
      &storage);
    i_key_value_store = &ram_key_value_store.interface;
  }

  void given_it_has_been_initialized() {
    line_detected_init(&self, i_key_value_store, &keys);
  }

  void given_the_motors_were_set_to(motors_t motors) {
    tiny_key_value_store_write(i_key_value_store, key_motors, &motors);
  }

  void when_a_line_is(bool detected) {
    tiny_key_value_store_write(i_key_value_store, key_line_detected, &detected);
  }

  void given_a_line_has(bool detected) {
    when_a_line_is(detected);
  }

  void the_motors_should_be_set_to(motors_t expected) {
    motors_t actual;
    tiny_key_value_store_read(i_key_value_store, key_motors, &actual);
    CHECK_EQUAL(expected.left_power, actual.left_power);
    CHECK_EQUAL(expected.right_power, actual.right_power);
  }
};

TEST(line_detected, should_do_nothing_on_init) {
   given_it_has_been_initialized();
   given_the_motors_were_set_to(some_setting);
   the_motors_should_be_set_to(some_setting);
}
