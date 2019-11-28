/*!
 * @file
 * @brief
 */

#include "start_running.h"
#include <stddef.h>
#include "tiny_timer.h"
#include "tiny_utils.h"
#include "data_model.h"

static void timer_expired(tiny_timer_group_t* group, void* context) {
  (void)group;
  reinterpret(self, context, start_running_t*);
  bool ready = true;

  tiny_key_value_store_write(
    self->key_value_store,
    key_robot_running,
    &ready);
}

void start_running_init(
  start_running_t* self,
  tiny_timer_group_t* timer_group,
  i_tiny_key_value_store_t* key_value_store) {
  self->key_value_store = key_value_store;
  tiny_timer_start(timer_group, &self->timer, 5000, timer_expired, self);
}
