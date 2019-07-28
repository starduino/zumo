#include <stdbool.h>
#include "clock.h"
#include "user_led.h"

void main(void) {
  clock_init();
  user_led_init();
  user_led_write(0);

  while(true) {
  }
}
