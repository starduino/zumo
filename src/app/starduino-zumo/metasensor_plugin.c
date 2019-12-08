/*!
 * @file
 * @brief
 */

#include "metasensor_plugin.h"
#include "data_model.h"

void metasensor_plugin_init(metasensor_plugin_t* self, i_tiny_key_value_store_t* key_value_store, tiny_timer_group_t* timer_group) {
  start_running_init(&self->start_running, timer_group, key_value_store);
}
