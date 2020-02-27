/*!
 * @file
 * @brief
 */

extern "C" {
#include "application.h"
#include "tiny_utils.h"
#include "tactic.h"
#include "tuning.h"
#include "data_model.h"
}

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "tiny_time_source_double.h"

enum {
  startup_delay = 5 * 1000,
  line_avoidance_maneuver_time = line_detection_back_up_time + line_detection_turn_time + 1 // why? no one knows... or cares
};

TEST_GROUP(application) {
  application_t instance;

  tiny_time_source_double_t time_source;
  tiny_timer_group_t timer_group;

  i_tiny_key_value_store_t* data_model;

  tiny_event_subscription_t tactic_creeper;

  void setup() {
    tiny_time_source_double_init(&time_source);
    tiny_timer_group_init(&timer_group, &time_source.interface);

    data_model = &instance.data_model.key_value_store.interface;
  }

  void given_that_the_application_has_been_initialized() {
    application_init(&instance, &timer_group);
  }

  static void maybe_tactic_changed(void* context, const void* _args) {
    (void)context;
    reinterpret(args, _args, const tiny_key_value_store_on_change_args_t*);

    if(args->key == key_current_tactic) {
      reinterpret(tactic, args->value, const tactic_t*);
      mock().actualCall("tactic_changed").withParameter("tactic", *tactic);
    }
  }

  void given_that_tactics_are_being_creeped_on() {
    tiny_event_subscription_init(&tactic_creeper, NULL, maybe_tactic_changed);
    tiny_event_subscribe(tiny_key_value_store_on_change(data_model), &tactic_creeper);
  }

  void after(tiny_time_source_ticks_t ticks) {
    for(uint32_t i = 0; i < ticks; i++) {
      tiny_time_source_double_tick(&time_source, 1);
      tiny_timer_group_run(&timer_group);
    }
  }

  void should_be_running() {
    bool running;
    tiny_key_value_store_read(data_model, key_robot_running, &running);
    CHECK(running);
  }

  void should_not_be_running() {
    bool running;
    tiny_key_value_store_read(data_model, key_robot_running, &running);
    CHECK_FALSE(running);
  }

  void given_that_startup_has_completed() {
    after(startup_delay);
  }

  void the_tactic_should_change_to(tactic_t tactic) {
    mock().expectOneCall("tactic_changed").withParameter("tactic", tactic);
  }

  void after_a_line_is_detected() {
    bool troo = true;
    tiny_key_value_store_write(data_model, key_line_detected, &troo);
  }

  void after_the_line_is_lost() {
    bool foolse = false;
    tiny_key_value_store_write(data_model, key_line_detected, &foolse);
  }
};

TEST(application, should_wait_to_do_stuff) {
  given_that_the_application_has_been_initialized();

  after(startup_delay - 1);
  should_not_be_running();

  after(1);
  should_be_running();
}

TEST(application, should_repeatedly_toe_the_line) {
  given_that_the_application_has_been_initialized();
  given_that_startup_has_completed();
  given_that_tactics_are_being_creeped_on();

  the_tactic_should_change_to(tactic_avoid_line);
  after_a_line_is_detected();

  the_tactic_should_change_to(tactic_seek_counterclockwise);
  after_the_line_is_lost();
  after(line_avoidance_maneuver_time);

  the_tactic_should_change_to(tactic_avoid_line);
  after_a_line_is_detected();

  the_tactic_should_change_to(tactic_seek_counterclockwise);
  after_the_line_is_lost();
  after(line_avoidance_maneuver_time);
}
