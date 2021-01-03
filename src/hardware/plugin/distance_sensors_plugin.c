/*!
 * @file
 * @brief
 */

#include "distance_sensors_plugin.h"
#include "distance_in_cm.h"
#include "gp2y0a21yk0f.h"
#include "data_model.h"
#include "adc2.h"
#include "tiny_utils.h"

enum {
  period = 100,
  left_channel = 2,
  center_channel = 4, // this is unused currently
  right_channel = 3
};

static const detect_enemy_keys_t detect_left_keys = {
  .sensor_distance = key_left_sensor_distance,
  .enemy_detected = key_left_sensor_enemy_detected
};

static const detect_enemy_keys_t detect_right_keys = {
  .sensor_distance = key_right_sensor_distance,
  .enemy_detected = key_right_sensor_enemy_detected
};

static void read_sensor(
  distance_sensors_plugin_t* self,
  tiny_analog_input_channel_t channel,
  tiny_key_value_store_key_t key)
{
  tiny_analog_input_counts_t counts = tiny_analog_input_group_read(self->adc_group, channel);
  distance_in_cm_t distance = gp2y0a21yk0f_counts_to_distance(counts);
  tiny_key_value_store_write(self->key_value_store, key, &distance);
  if(channel == left_channel) {
    tiny_key_value_store_write(self->key_value_store, key_knob_counts, &counts);
  }
}

static void poll(tiny_timer_group_t* timer_group, void* context)
{
  reinterpret(self, context, distance_sensors_plugin_t*);

  read_sensor(self, left_channel, key_left_sensor_distance);
  read_sensor(self, center_channel, key_center_sensor_distance);
  read_sensor(self, right_channel, key_right_sensor_distance);

  tiny_timer_start(timer_group, &self->timer, period, poll, self);
}

void distance_sensors_plugin_init(
  distance_sensors_plugin_t* self,
  i_tiny_key_value_store_t* key_value_store,
  tiny_timer_group_t* timer_group)
{
  self->key_value_store = key_value_store;
  self->adc_group = adc2_init();

  poll(timer_group, self);
  detect_enemy_init(&self->detect_left, key_value_store, &detect_left_keys);
  detect_enemy_init(&self->detect_right, key_value_store, &detect_right_keys);
}
