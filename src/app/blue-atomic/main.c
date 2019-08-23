/*!
 * @file
 * @brief
 */

#include <stdbool.h>
#include <stddef.h>
#include "stm8s.h"
#include "clock.h"
#include "atom.h"
#include "atomport-private.h"

enum {
  default_thread_priority = 16,
  pin_5 = (1 << 5)
};

static ATOM_TCB on_tcb;
static uint8_t on_thread_stack[128];

static ATOM_TCB off_tcb;
static uint8_t off_thread_stack[128];
static uint8_t idle_thread_stack[128];

static void on_thread_func(uint32_t param) {
  (void)param;

  GPIOB->CR1 |= pin_5;
  GPIOB->DDR |= pin_5;

  while(1) {
    atomTimerDelay(300);
    GPIOB->ODR = pin_5;
  }
}

static void off_thread_func(uint32_t param) {
  (void)param;

  GPIOB->CR1 |= pin_5;
  GPIOB->DDR |= pin_5;

  while(1) {
    atomTimerDelay(300);
    GPIOB->ODR = 0;
  }
}

static void main_thread_func(uint32_t param);

void main(void) {
  disableInterrupts();

  clock_init();

  if(atomOSInit(&idle_thread_stack, sizeof(idle_thread_stack), true) == ATOM_OK) {
    archInitSystemTickTimer();

    int8_t status = atomThreadCreate(
      &on_tcb,
      default_thread_priority,
      on_thread_func,
      0,
      on_thread_stack,
      sizeof(on_thread_stack),
      true);

    status += atomThreadCreate(
      &off_tcb,
      default_thread_priority,
      off_thread_func,
      0,
      off_thread_stack,
      sizeof(off_thread_stack),
      true);

    if(status == ATOM_OK) {
      atomOSStart();
    }
  }

  while(1) {
  }
}
