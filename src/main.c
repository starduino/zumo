/*!
 * @file
 * @brief
 */

#include <stdbool.h>
#include <stddef.h>
#include "stm8s.h"
#include "clock.h"
#include "interrupts.h"
#include "tim4_system_tick.h"
#include "pa3_heartbeat.h"
#include "pc5_heartbeat.h"
#include "tiny_timer.h"
#include "watchdog.h"
#include "data_model.h"
#include "application.h"

static application_t application;
static tiny_timer_group_t timer_group;

void main(void)
{
  interrupts_disable();
  {
    clock_init();
    tiny_timer_group_init(&timer_group, tim4_system_tick_init());
    watchdog_init(&timer_group);
    pa3_heartbeat_init(&timer_group);
    pc5_heartbeat_init(&timer_group);
    application_init(&application, &timer_group);
  }
  interrupts_enable();

  while(true) {
    tiny_timer_group_run(&timer_group);
    interrupts_wait_for_interrupt();
  }
}
