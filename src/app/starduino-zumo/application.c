/*!
 * @file
 * @brief
 */

#include "application.h"

#include <stddef.h>
#include "tiny_utils.h"
static tiny_event_subscription_t derp_sub;

static void derp_change(void* context, const void* _args) {
  reinterpret(args, _args, const tiny_key_value_store_on_change_args_t*);
  (void)context;

  if(args->key == key_right_line_detected) {
    volatile uint8_t derpity = 10;
    derpity++;
  }
}

static void derp(i_tiny_key_value_store_t* store) {
  motor_power_t power = 50;
  tiny_key_value_store_write(store, key_left_motor, &power);
  tiny_key_value_store_write(store, key_right_motor, &power);

  tiny_event_subscription_init(&derp_sub, NULL, derp_change);
}

void application_init(application_t* self, tiny_timer_group_t* timer_group) {
  data_model_init(&self->data_model);
  i_tiny_key_value_store_t* store = data_model_key_value_store(&self->data_model);

  motors_plugin_init(&self->motors_plugin, store);
  beep_plugin_init(&self->beep_plugin, store);
  line_sensors_plugin_init(&self->line_sensors_plugin, store, timer_group);

  derp(store);
}
