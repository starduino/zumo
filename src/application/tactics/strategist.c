/*!
 * @file
 * @brief
 */

#include "strategist.h"
#include "direction.h"
#include "tiny_utils.h"
#include "tactic.h"

static void change_tactic_to(strategist_t* self, tactic_t tactic) {
  tiny_key_value_store_write(
    self->key_value_store,
    self->keys->key_tactic,
    &tactic);
}

static void choose_initial_tactic(strategist_t* self) {
  direction_t initial_direction;
  tactic_t tactic;
  tiny_key_value_store_read(
    self->key_value_store,
    self->keys->key_initial_direction,
    &initial_direction);

  tactic = initial_direction == direction_clockwise ? tactic_seek_clockwise : tactic_seek_counterclockwise;
  change_tactic_to(self, tactic);
}

static void choose_seeking_tactic(strategist_t* self) {
  bool previously_seen_on_right = self->previous_location == enemy_location_front_right;
  tactic_t tactic = previously_seen_on_right ? tactic_seek_clockwise : tactic_seek_counterclockwise;
  change_tactic_to(self, tactic);
}

static bool the_enemy_is_visible(strategist_t* self) {
  enemy_location_t location;
  tiny_key_value_store_read(
    self->key_value_store,
    self->keys->key_enemy_location,
    &location);
  return location != enemy_location_unknown;
}

static void data_changed(void* context, const void* _args) {
  reinterpret(self, context, strategist_t*);
  reinterpret(args, _args, const tiny_key_value_store_on_change_args_t*);

  if(args->key == self->keys->key_enemy_location) {
    reinterpret(location, args->value, const enemy_location_t*);

    if(*location == enemy_location_unknown) {
      choose_seeking_tactic(self);
    }
    else {
      change_tactic_to(self, tactic_charge);
      self->previous_location = *location;
    }
  }
  else if(args->key == self->keys->key_robot_running) {
    choose_initial_tactic(self);
  }
  else if(args->key == self->keys->key_line_detected) {
    change_tactic_to(self, tactic_avoid_line);
  }
  else if(args->key == self->keys->key_tactic_stopped) {
    if(the_enemy_is_visible(self)) {
      change_tactic_to(self, tactic_charge);
    }
    else {
      choose_seeking_tactic(self);
    }
  }
}

void strategist_init(
  strategist_t* self,
  i_tiny_key_value_store_t* key_value_store,
  const strategist_keys_t* keys) {
  self->key_value_store = key_value_store;
  self->keys = keys;

  tiny_event_subscription_init(&self->on_change_subscription, self, data_changed);
  tiny_event_subscribe(tiny_key_value_store_on_change(key_value_store), &self->on_change_subscription);
}
