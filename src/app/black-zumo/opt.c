/*!
 * @file
 * @brief
 */

#include "stm8s.h"

enum {
  afr6 = (1 << 6)
};

static const uint8_t __at(0x4803) opt2 = (uint8_t)afr6;
static const uint8_t __at(0x4804) nopt2 = (uint8_t)~afr6;
