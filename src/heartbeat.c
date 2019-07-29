/*!
 * @file
 * @brief
 */

#include <stddef.h>
#include <stdbool.h>
#include "heartbeat.h"
#include "stm8.h"

enum {
  pin_5 = (1 << 5)
};

static tiny_timer_t timer;

static void blink(tiny_timer_group_t* group, void* context) {
  (void)group;
  (void)context;
  PB_ODR ^= pin_5;
  tiny_timer_start(group, &timer, 500, blink, NULL);
}

void heartbeat_init(tiny_timer_group_t* timer_group) {
  PB_CR1 |= pin_5;
  PB_DDR |= pin_5;
  tiny_timer_start(timer_group, &timer, 500, blink, NULL);
}
