/*!
 * @file
 * @brief
 */

#ifndef distance_sensors_plugin_h
#define distance_sensors_plugin_h

#include "i_tiny_key_value_store.h"
#include "tiny_timer.h"
#include "hal/i_tiny_analog_input_group.h"
#include "detect_enemy.h"

typedef struct {
  tiny_timer_t timer;
  i_tiny_key_value_store_t* key_value_store;
  i_tiny_analog_input_group_t* adc_group;
  detect_enemy_t detect_left;
  detect_enemy_t detect_right;
} distance_sensors_plugin_t;

void distance_sensors_plugin_init(
  distance_sensors_plugin_t* self,
  i_tiny_key_value_store_t* key_value_store,
  tiny_timer_group_t* timer_group);

#endif
