/*!
 * @file
 * @brief
 */

extern "C" {
#include <string.h>
#include "strategist.h"
#include "direction.h"
#include "enemy_location.h"
#include "tactic.h"
#include "tiny_utils.h"
#include "tiny_ram_key_value_store.h"
}

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

enum {
  in_front = enemy_location_front_center,
  in_front_left = enemy_location_front_left,
  in_front_right = enemy_location_front_right,
  out_of_view = enemy_location_unknown,
  detected = true,
  been_detected = true
};

// clang-format off
#define data_model_key_value_pairs(pair) \
  pair(key_tactic,            tactic_t) \
  pair(key_enemy_location,    enemy_location_t) \
  pair(key_initial_direction, direction_t) \
  pair(key_robot_running,     bool) \
  pair(key_line_detected,     bool) \
  pair(key_tactic_stopped,    uint8_t) \
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

static const strategist_keys_t keys = {
  .key_tactic = key_tactic,
  .key_enemy_location = key_enemy_location,
  .key_initial_direction = key_initial_direction,
  .key_robot_running = key_robot_running,
  .key_line_detected = key_line_detected,
  .key_tactic_stopped = key_tactic_stopped
};

TEST_GROUP(strategist) {
  strategist_t self;
  tiny_ram_key_value_store_t ram_key_value_store;
  i_tiny_key_value_store_t* i_key_value_store;

  void given_it_has_been_initialized() {
    tiny_ram_key_value_store_init(
      &ram_key_value_store,
      &store_config,
      &storage);
    i_key_value_store = &ram_key_value_store.interface;
    strategist_init(&self, i_key_value_store, &keys);
  }

  void given_the_initial_seeking_direction_is(direction_t direction) {
    tiny_key_value_store_write(i_key_value_store, key_initial_direction, &direction);
  }

  void when_the_enemy_moves(enemy_location_t location) {
    tiny_key_value_store_write(i_key_value_store, key_enemy_location, &location);
  }

  void given_the_enemy_was(enemy_location_t location) {
    when_the_enemy_moves(location);
  }

  void when_a_line_becomes(bool detected) {
    tiny_key_value_store_write(i_key_value_store, key_line_detected, &detected);
  }

  void given_a_line_has(bool detected) {
    when_a_line_becomes(detected);
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

  void the_selected_tactic_should_become(tactic_t expected) {
    the_selected_tactic_should_be(expected);
  }
};

TEST(strategist, should_idle_on_init) {
  given_it_has_been_initialized();
  the_selected_tactic_should_be(tactic_idle);
}

TEST(strategist, should_seek_clockwise_when_the_robot_is_running_and_clockwise_is_selected) {
  given_it_has_been_initialized();
  given_the_initial_seeking_direction_is(direction_clockwise);
  the_selected_tactic_should_be(tactic_idle);

  when_the_robot_starts_running();
  the_selected_tactic_should_become(tactic_seek_clockwise);
}

TEST(strategist, should_seek_counterclockwise_when_the_robot_is_running_and_counterclockwise_is_selected) {
  given_it_has_been_initialized();
  given_the_initial_seeking_direction_is(direction_counterclockwise);
  the_selected_tactic_should_be(tactic_idle);

  when_the_robot_starts_running();
  the_selected_tactic_should_become(tactic_seek_counterclockwise);
}

TEST(strategist, should_charge_when_the_enemy_moves_detected) {
  given_it_has_been_initialized();
  when_the_enemy_moves(in_front);
  the_selected_tactic_should_become(tactic_charge);

  when_the_enemy_moves(in_front_left);
  the_selected_tactic_should_become(tactic_charge);

  when_the_enemy_moves(in_front_right);
  the_selected_tactic_should_become(tactic_charge);
}

TEST(strategist, should_seek_clockwise_when_the_enemy_was_last_seen_to_the_right) {
  given_it_has_been_initialized();
  given_the_enemy_was(in_front_right);

  when_the_enemy_moves(out_of_view);
  the_selected_tactic_should_become(tactic_seek_clockwise);
}

TEST(strategist, should_seek_counterclockwise_when_the_enemy_was_last_seen_to_the_left) {
  given_it_has_been_initialized();
  given_the_enemy_was(in_front_left);

  when_the_enemy_moves(out_of_view);
  the_selected_tactic_should_become(tactic_seek_counterclockwise);
}

TEST(strategist, should_stop_charging_when_a_line_becomes_detected) {
  given_it_has_been_initialized();
  given_the_enemy_was(in_front);

  when_a_line_becomes(detected);
  the_selected_tactic_should_become(tactic_avoid_line);
}

TEST(strategist, should_stop_charging_when_a_line_becomes_detected_while_the_enemy_is_left) {
  given_it_has_been_initialized();
  given_the_enemy_was(in_front_left);

  when_a_line_becomes(detected);
  the_selected_tactic_should_become(tactic_avoid_line);
}

TEST(strategist, should_stop_charging_when_a_line_becomes_detected_while_the_enemy_is_right) {
  given_it_has_been_initialized();
  given_the_enemy_was(in_front_left);

  when_a_line_becomes(detected);
  the_selected_tactic_should_become(tactic_avoid_line);
}

TEST(strategist, should_stop_charging_when_a_line_becomes_detected_while_the_enemy_is_out_of_view) {
  given_it_has_been_initialized();
  given_the_enemy_was(out_of_view);

  when_a_line_becomes(detected);
  the_selected_tactic_should_become(tactic_avoid_line);
}

TEST(strategist, should_continue_charging_when_line_avoidance_stops_running) {
  given_it_has_been_initialized();
  given_the_enemy_was(in_front);
  given_a_line_has(been_detected);

  when_the_tactic_stops();
  the_selected_tactic_should_become(tactic_charge);
}

TEST(strategist, should_continue_seeking_when_line_avoidance_stops_running_if_the_enemy_is_out_of_view) {
  given_it_has_been_initialized();
  given_a_line_has(been_detected);

  when_the_tactic_stops();
  the_selected_tactic_should_become(tactic_seek_counterclockwise);
}

TEST(strategist, should_continue_seeking_counterclockwise_when_line_avoidance_stops_running_and_the_enemy_was_last_seen_in_that_direction) {
  given_it_has_been_initialized();
  given_the_enemy_was(in_front_left);
  given_the_enemy_was(out_of_view);
  given_a_line_has(been_detected);

  when_the_tactic_stops();
  the_selected_tactic_should_become(tactic_seek_counterclockwise);
}

TEST(strategist, should_continue_seeking_clockwise_when_line_avoidance_stops_running_and_the_enemy_was_last_seen_in_that_direction) {
  given_it_has_been_initialized();
  given_the_enemy_was(in_front_right);
  given_the_enemy_was(out_of_view);
  given_a_line_has(been_detected);

  when_the_tactic_stops();
  the_selected_tactic_should_become(tactic_seek_clockwise);
}

