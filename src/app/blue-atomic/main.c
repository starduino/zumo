/*!
 * @file
 * @brief
 */

#include <stdbool.h>
#include <stddef.h>
#include "stm8s.h"
#include "clock.h"
#include "atom.h"
#include "atomqueue.h"
#include "atomport-private.h"

enum {
  default_thread_priority = 16,
  pin_5 = (1 << 5)
};

enum {
  queue_message_size = sizeof(bool),
  queue_message_count = 3
};

static ATOM_QUEUE queue;
static uint8_t queue_buffer[queue_message_size * queue_message_count];

static ATOM_TCB producer_tcb;
static uint8_t producer_thread_stack[128];

static ATOM_TCB consumer_tcb;
static uint8_t consumer_thread_stack[128];

static uint8_t idle_thread_stack[128];

static void producer_thread_func(uint32_t param) {
  (void)param;

  bool state = false;

  while(1) {
    (void)atomQueuePut(&queue, 0, &state);
    state = !state;
    atomTimerDelay(250);
  }
}

static void consumer_thread_func(uint32_t param) {
  (void)param;

  GPIOB->CR1 |= pin_5;
  GPIOB->DDR |= pin_5;

  while(1) {
    bool state;
    (void)atomQueueGet(&queue, 0, &state);
    GPIOB->ODR = pin_5 * state;
  }
}

static void main_thread_func(uint32_t param);

void main(void) {
  disableInterrupts();

  clock_init();

  if(atomOSInit(&idle_thread_stack, sizeof(idle_thread_stack), true) == ATOM_OK) {
    int8_t status = ATOM_OK;

    archInitSystemTickTimer();

    status += atomQueueCreate(
      &queue,
      queue_buffer,
      queue_message_size,
      queue_message_count);

    status += atomThreadCreate(
      &producer_tcb,
      default_thread_priority,
      producer_thread_func,
      0,
      producer_thread_stack,
      sizeof(producer_thread_stack),
      true);

    status += atomThreadCreate(
      &consumer_tcb,
      default_thread_priority,
      consumer_thread_func,
      0,
      consumer_thread_stack,
      sizeof(consumer_thread_stack),
      true);

    if(status == ATOM_OK) {
      atomOSStart();
    }
  }

  while(1) {
  }
}
