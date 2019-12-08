/*!
 * @file
 * @brief
 */

#ifndef metasensor_plugin_h
#define metasensor_plugin_h

#include "i_tiny_key_value_store.h"
#include "tiny_timer.h"
#include "strategist.h"
#include "line_detected.h"
#include "charge.h"
#include "seeking.h"
#include "start_running.h"

typedef struct {
  start_running_t start_running;
} metasensor_plugin_t;

void metasensor_plugin_init(
  metasensor_plugin_t* self,
  i_tiny_key_value_store_t* key_value_store,
  tiny_timer_group_t* timer_group);

#endif
