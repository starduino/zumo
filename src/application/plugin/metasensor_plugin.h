/*!
 * @file
 * @brief
 */

#ifndef metasensor_plugin_h
#define metasensor_plugin_h

#include "start_running.h"
#include "enemy_location_metasensor.h"

typedef struct {
  start_running_t start_running;
  enemy_location_metasensor_t enemy_location_metasensor;
} metasensor_plugin_t;

void metasensor_plugin_init(
  metasensor_plugin_t* self,
  i_tiny_key_value_store_t* key_value_store,
  tiny_timer_group_t* timer_group);

#endif
