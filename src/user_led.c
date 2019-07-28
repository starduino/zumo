/*!
 * @file
 * @brief
 */

#include "user_led.h"
#include "stm8.h"

enum {
  pin_5 = (1 << 5)
};

void user_led_init(void) {
  // PB.5 push/pull output
  PB_CR1 |= pin_5;
  PB_DDR |= pin_5;
}

void user_led_write(bool state) {
  if(state) {
    PB_ODR |= pin_5;
  }
  else {
    PB_ODR &= ~pin_5;
  }
}
