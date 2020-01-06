/*!
 * @file
 * @brief
 */

#include "metasensor_plugin.h"
#include "data_model.h"

static const enemy_location_metasensor_keys_t enemy_location_metasensor_keys = {
  .enemy_location = key_enemy_location,
  .left_sensor_active = key_left_sensor_enemy_detected,
  .right_sensor_active = key_right_sensor_enemy_detected
};

void metasensor_plugin_init(metasensor_plugin_t* self, i_tiny_key_value_store_t* key_value_store, tiny_timer_group_t* timer_group) {
  start_running_init(&self->start_running, timer_group, key_value_store);
  enemy_location_metasensor_init(&self->enemy_location_metasensor, key_value_store, &enemy_location_metasensor_keys);
}
