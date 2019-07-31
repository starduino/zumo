/*!
 * @file
 * @brief
 */

#include "clock.h"

void clock_init(void) {
  CLK->CKDIVR = 0;

  while(!(CLK->ICKR & CLK_ICKR_HSIRDY)) {
  }
}
