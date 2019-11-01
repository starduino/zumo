/*!
 * @file
 * @brief
 */

extern "C" {
#include <string.h>
#include "line_detected.h"
#include "motors.h"
#include "tactic.h"
#include "tiny_utils.h"
#include "tiny_ram_key_value_store.h"
}

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

enum {
  on = true
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

static const line_detected_keys = {
  .key_motors = key_motors,
  .key_last_enemy_location = key_last_enemy_location,
  .key_line_detected = key_line_detected,
  .key_tactic_stopped = key_tactic_stopped
};

TEST_GROUP(line_detected) {
  line_detected_t self;
  tiny_ram_key_value_store_t ram_key_value_store;
  i_tiny_key_value_store_t* i_key_value_store;

  void given_it_has_been_initialized() {
    tiny_ram_key_value_store_init(
      &ram_key_value_store,
      &store_config,
      &storage);
    i_key_value_store = &ram_key_value_store.interface;
    line_detected_init(&self, i_key_value_store, &keys);
  }

  void given_the_initial_seeking_direction_is(direction_t direction) {
    tiny_key_value_store_write(i_key_value_store, key_initial_direction, &direction);
  }

  void when_the_enemy_is(enemy_location_t location) {
    tiny_key_value_store_write(i_key_value_store, key_enemy_location, &location);
  }

  void given_the_enemy_was(enemy_location_t location) {
    when_the_enemy_is(location);
  }

  void when_a_line_is(bool detected) {
    tiny_key_value_store_write(i_key_value_store, key_line_detected, &detected);
  }

  void given_a_line_has(bool detected) {
    when_a_line_is(detected);
  }

  void when_the_tactic_stops() {
    uint8_t signal;
    tiny_key_value_store_read(i_key_value_store, key_tactic_stopped, &signal);
    signal++;
    tiny_key_value_store_write(i_key_value_store, key_tactic_stopped, &signal);
  }

  void when_the_robot_starts_running() {
    bool running = true;
    tiny_key_value_store_write(i_key_value_store, key_robot_running, &running);
  }

  void the_selected_tactic_should_be(tactic_t expected) {
    tactic_t actual;
    tiny_key_value_store_read(i_key_value_store, key_tactic, &actual);
    CHECK_EQUAL(expected, actual);
  }
};

TEST(line_detected, should_idle_on_init) {
  // given_it_has_been_initialized();
  // the_selected_tactic_should_be(tactic_idle);
}
