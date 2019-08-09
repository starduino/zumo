/*!
 * @file
 * @brief
 */

#include <stdbool.h>
#include <stddef.h>
#include "stm8s.h"
#include "clock.h"
#include "system_tick.h"
#include "heartbeat.h"
#include "tiny_timer.h"
#include "watchdog.h"
#include "uart1.h"
#include "spi.h"

static tiny_timer_group_t timer_group;
static tiny_timer_t timer;
static i_tiny_uart_t* uart;
static i_tiny_spi_t* spi;

static void spi_read_callback(void* context, uint8_t byte) {
  (void)context;
  (void)byte;
}

static void spi_write_callback(void* context) {
  (void)context;
  tiny_spi_read(spi, spi_read_callback, NULL);
}

static void kick_watchdog(tiny_timer_group_t* _timer_group, void* context) {
  (void)context;
  (void)_timer_group;
  watchdog_kick();
  tiny_timer_start(&timer_group, &timer, 1, kick_watchdog, NULL);
}

void main(void) {
  disableInterrupts();
  {
    watchdog_init();
    clock_init();
    tiny_timer_group_init(&timer_group, system_tick_init());
    uart = uart1_init();
    heartbeat_init(&timer_group);
    spi = spi_init();
  }
  enableInterrupts();

  tiny_timer_start(&timer_group, &timer, 1, kick_watchdog, NULL);

  tiny_spi_write(spi, 0, spi_write_callback, NULL);

  while(true) {
    tiny_timer_group_run(&timer_group);
  }
}
