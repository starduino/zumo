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
#include "watchdog.h"

static tiny_timer_group_t timer_group;
static tiny_timer_t timer;

static void kick_watchdog(tiny_timer_group_t* _timer_group, void* context) {
  (void)context;
  (void)_timer_group;
  watchdog_kick();
  tiny_timer_start(&timer_group, &timer, 1, kick_watchdog, NULL);
}

void main(void) {
  watchdog_init();
  clock_init();
  tiny_timer_group_init(&timer_group, system_tick_init());
  heartbeat_init(&timer_group);

  tiny_timer_start(&timer_group, &timer, 1, kick_watchdog, NULL);

  while(true) {
    tiny_timer_group_run(&timer_group);
  }
}
