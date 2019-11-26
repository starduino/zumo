/*!
 * @file
 * @brief
 */

extern "C" {
#include "enemy_location_metasensor.h"
#include "enemy_location.h"
#include "tiny_ram_key_value_store.h"
#include "tiny_utils.h"
}

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

enum {
  in_front = enemy_location_front_center,
  in_front_left = enemy_location_front_left,
  in_front_right = enemy_location_front_right,
  out_of_view = enemy_location_unknown,
  active = true,
  inactive = false
};

// clang-format off
#define data_model_key_value_pairs(pair) \
  pair(key_left_distance_sensor_active,     bool) \
  pair(key_right_distance_sensor_active,    bool) \
  pair(key_enemy_location,                  enemy_location_t) \
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

static const enemy_location_metasensor_keys_t keys = {
  .enemy_location = key_enemy_location,
  .left_sensor_active = key_left_distance_sensor_active,
  .right_sensor_active = key_right_distance_sensor_active
};

TEST_GROUP(enemy_location_metasensor) {
  enemy_location_metasensor_t self;
  tiny_ram_key_value_store_t ram_key_value_store;
  i_tiny_key_value_store_t* i_key_value_store;

  void given_it_has_been_initialized() {
    tiny_ram_key_value_store_init(
      &ram_key_value_store,
      &store_config,
      &storage);
    i_key_value_store = &ram_key_value_store.interface;
    enemy_location_metasensor_init(&self, i_key_value_store, &keys);
  }

  void when_the_left_sensor_is(bool active) {
    tiny_key_value_store_write(i_key_value_store, key_left_distance_sensor_active, &active);
  }

  void when_the_right_sensor_is(bool active) {
    tiny_key_value_store_write(i_key_value_store, key_right_distance_sensor_active, &active);
  }

  void the_enemy_location_should_be(enemy_location_t expected) {
    enemy_location_t actual;
    tiny_key_value_store_read(i_key_value_store, key_enemy_location, &actual);
    CHECK_EQUAL(expected, actual);
  }
};

TEST(enemy_location_metasensor, should_say_the_enemy_is_not_visible_initially) {
  given_it_has_been_initialized();
  the_enemy_location_should_be(out_of_view);
}

TEST(enemy_location_metasensor, should_say_when_the_enemy_is_in_front) {
  given_it_has_been_initialized();

  when_the_left_sensor_is(active);
  when_the_right_sensor_is(active);
  the_enemy_location_should_be(in_front);
}

TEST(enemy_location_metasensor, should_say_when_the_enemy_is_in_the_front_left) {
  given_it_has_been_initialized();

  when_the_left_sensor_is(active);
  when_the_right_sensor_is(inactive);
  the_enemy_location_should_be(in_front_left);
}

TEST(enemy_location_metasensor, should_say_when_the_enemy_is_in_the_front_right) {
  given_it_has_been_initialized();

  when_the_left_sensor_is(inactive);
  when_the_right_sensor_is(active);
  the_enemy_location_should_be(in_front_right);
}

TEST(enemy_location_metasensor, should_say_when_the_enemy_leaves_view) {
  given_it_has_been_initialized();

  when_the_left_sensor_is(active);
  when_the_left_sensor_is(inactive);
  the_enemy_location_should_be(out_of_view);

  when_the_right_sensor_is(active);
  when_the_right_sensor_is(inactive);
  the_enemy_location_should_be(out_of_view);

  when_the_right_sensor_is(active);
  when_the_left_sensor_is(active);
  when_the_right_sensor_is(inactive);
  when_the_left_sensor_is(inactive);

  the_enemy_location_should_be(out_of_view);
}
