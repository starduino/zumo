/*!
 * @file
 * @brief
 */

#ifndef application_h
#define application_h

#include "tiny_timer.h"
#include "data_model.h"
#include "motors_plugin.h"
#include "line_sensors_plugin.h"
#include "distance_sensors_plugin.h"
#include "accelerometer_plugin.h"
#include "buzzer_plugin.h"
#include "strategy_plugin.h"
#include "metasensor_plugin.h"
#include "tactic_feedback_plugin.h"

typedef struct {
  data_model_t data_model;
  motors_plugin_t motors_plugin;
  line_sensors_plugin_t line_sensors_plugin;
  distance_sensors_plugin_t distance_sensors_plugin;
  accelerometer_plugin_t accelerometer_plugin;
  buzzer_plugin_t buzzer_plugin;
  strategy_plugin_t strategy_plugin;
  metasensor_plugin_t metasensor_plugin;
  tactic_feedback_plugin_t tactic_feedback_plugin;
} application_t;

void application_init(application_t* self, tiny_timer_group_t* timer_group);

#endif
