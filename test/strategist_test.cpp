/*!
 * @file
 * @brief
 */

extern "C" {
#include <string.h>
#include "strategist.h"
#include "enemy_location.h"
#include "tactic.h"
#include "tiny_utils.h"
#include "tiny_ram_key_value_store.h"
}

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

enum {
  in_front = enemy_location_front_center,
  not_visible = enemy_location_unknown
};

// clang-format off
#define data_model_key_value_pairs(pair) \
  pair(key_tactic,            tactic_t) \
  pair(key_enemy_location,    enemy_location_t)
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
  .key_enemy_location = key_enemy_location
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

  void when_an_enemy_is(enemy_location_t location) {
    tiny_key_value_store_write(i_key_value_store, key_enemy_location, &location);
  }

  void given_the_enemy_was(enemy_location_t location) {
    when_an_enemy_is(location);
  }

  void the_selected_tactic_should_be(tactic_t expected) {
    tactic_t actual;
    tiny_key_value_store_read(i_key_value_store, key_tactic, &actual);
    CHECK_EQUAL(expected, actual);
  }
};

TEST(strategist, should_seek_on_init) {
  given_it_has_been_initialized();
  the_selected_tactic_should_be(tactic_seeking);
}

TEST(strategist, should_charge_when_an_enemy_is_detected) {
  given_it_has_been_initialized();
  when_an_enemy_is(in_front);
  the_selected_tactic_should_be(tactic_charge);
}

TEST(strategist, should_seek_when_an_enemy_is_no_longer_detected) {
  given_it_has_been_initialized();
  given_the_enemy_was(in_front);

  when_an_enemy_is(not_visible);
  the_selected_tactic_should_be(tactic_seeking);
}
