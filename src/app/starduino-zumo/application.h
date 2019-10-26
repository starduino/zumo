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

typedef struct {
  data_model_t data_model;
  motors_plugin_t motors_plugin;
  line_sensors_plugin_t line_sensors_plugin;
} application_t;

void application_init(application_t* self, tiny_timer_group_t* timer_group);

#endif
