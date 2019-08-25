/*!
 * @file
 * @brief
 */

#include "application.h"

static void derp(application_t* self) {
  i_tiny_key_value_store_t* store = data_model_key_value_store(&self->data_model);
  int8_t power = 25;
  tiny_key_value_store_write(store, key_left_motor, &power);
  tiny_key_value_store_write(store, key_right_motor, &power);
}

void application_init(application_t* self, tiny_timer_group_t* timer_group) {
  (void)timer_group;

  data_model_init(&self->data_model);
  motors_plugin_init(&self->motors, data_model_key_value_store(&self->data_model));

  derp(self);
}
