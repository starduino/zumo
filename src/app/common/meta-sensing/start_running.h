/*!
 * @file
 * @brief
 *
 *
 */

#ifndef start_running
#define start_running
#include "i_tiny_key_value_store.h"
#include "tiny_timer.h"

typedef struct {
  i_tiny_key_value_store_t* key_value_store;
  tiny_timer_t timer;
} start_running_t;

void start_running_init(
  start_running_t* self,
  tiny_timer_group_t* timer_group,
  i_tiny_key_value_store_t* key_value_store);

#endif