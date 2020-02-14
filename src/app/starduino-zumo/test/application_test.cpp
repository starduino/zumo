/*!
 * @file
 * @brief
 */

extern "C" {
#include "application.h"
}

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "tiny_time_source_double.h"

TEST_GROUP(application) {
  application_t instance;

  tiny_time_source_double_t time_source;
  tiny_timer_group_t timer_group;

  i_tiny_key_value_store_t* data_model;

  void setup() {
    tiny_time_source_double_init(&time_source);
    tiny_timer_group_init(&timer_group, &time_source.interface);

    data_model = &instance.data_model.key_value_store.interface;
  }

  void given_that_the_application_has_been_initialized() {
    application_init(&instance, &timer_group);
  }

  void after(tiny_time_source_ticks_t ticks) {
    tiny_time_source_double_tick(&time_source, ticks);
    tiny_timer_group_run(&timer_group);
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
};

TEST(application, should_wait_to_do_stuff) {
  given_that_the_application_has_been_initialized();

  after(5 * 1000 - 1);
  should_not_be_running();

  after(1);
  should_be_running();
}
