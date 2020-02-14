/*!
 * @file
 * @brief
 */

#include "tiny_utils.h"

#include "accelerometer_plugin.h"
#include "buzzer_plugin.h"
#include "distance_sensors_plugin.h"
#include "line_sensors_plugin.h"
#include "motors_plugin.h"

void accelerometer_plugin_init(
  accelerometer_plugin_t* self,
  i_tiny_key_value_store_t* store,
  tiny_timer_group_t* timer_group) {
  (void)self;
  (void)store;
  (void)timer_group;
}

void buzzer_plugin_init(buzzer_plugin_t* self, i_tiny_key_value_store_t* key_value_store) {
  (void)self;
  (void)key_value_store;
}

void distance_sensors_plugin_init(
  distance_sensors_plugin_t* self,
  i_tiny_key_value_store_t* key_value_store,
  tiny_timer_group_t* timer_group) {
  (void)self;
  (void)key_value_store;
  (void)timer_group;
}

void line_sensors_plugin_init(
  line_sensors_plugin_t* self,
  i_tiny_key_value_store_t* key_value_store,
  tiny_timer_group_t* timer_group) {
  (void)self;
  (void)key_value_store;
  (void)timer_group;
}

void motors_plugin_init(motors_plugin_t* self, i_tiny_key_value_store_t* key_value_store) {
  (void)self;
  (void)key_value_store;
}
