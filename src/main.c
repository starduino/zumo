/*!
 * @file
 * @brief
 */

#include <stdbool.h>
#include <stddef.h>
#include "clock.h"
#include "system_tick.h"
#include "heartbeat.h"
#include "tiny_timer.h"

static tiny_timer_group_t timer_group;

void main(void) {
  clock_init();
  tiny_timer_group_init(&timer_group, system_tick_init());
  heartbeat_init(&timer_group);

  while(true) {
    tiny_timer_group_run(&timer_group);
  }
}
